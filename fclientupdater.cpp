#include "fclientupdater.h"
#include "libfusion.h"

FClientUpdater::FClientUpdater(QObject *parent) : QObject(parent)
{

}

//Gets current client version from API.
VersionCheckResult FClientUpdater::getLatestVersion(FusionVersions version)
{
    VersionCheckResult latestStable;
    VersionCheckResult latestStable_alt;
    VersionCheckResult latestNightly;
    VersionCheckResult latestNightly_alt;
    VersionCheckResult latestVersion;

    FusionSources nightlySource = FusionSources::srcNightly;
    FusionSources stableSource  = FusionSources::srcStable;

    FusionSources latestSource;



    //we check the Stable-Version anyway.
    latestStable = readOnlineVersionFile(UPDATER_VERSION_STBL_FILE);
    latestStable_alt = readOnlineVersionFile(UPDATER_VERSION_STBL_FILE_ALT);

    if (latestStable_alt.VersionOnline > latestStable.VersionOnline)
    {
        latestStable = latestStable_alt;
        stableSource = FusionSources::srcStable_Alt;
    }

    latestSource = stableSource;
    latestVersion = latestStable;

    if (version == FusionVersions::Nightly)
    {
        latestNightly = readOnlineVersionFile(UPDATER_VERSION_NIGHTLY_FILE);
        latestNightly_alt = readOnlineVersionFile(UPDATER_VERSION_NIGHTLY_FILE_ALT);

        if (latestNightly_alt.VersionOnline > latestNightly.VersionOnline)
        {
            latestNightly = latestNightly_alt;
            nightlySource = FusionSources::srcNightly_Alt;
        }

        if (latestNightly.VersionOnline > latestStable.VersionOnline)
        {
            latestSource = nightlySource;
            latestVersion = latestNightly;
        }
    }

    latestVersion.Source = latestSource;


    return latestVersion;
}

VersionCheckResult FClientUpdater::readOnlineVersionFile(QString URL) {

    VersionCheckResult vcr;
    manager = new QNetworkAccessManager(this);
    QEventLoop loop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkRequest request(URL);
    QNetworkReply *reply = manager->get(request);
    reply->ignoreSslErrors();

    loop.exec();

    QNetworkReply::NetworkError err = reply->error();

    if (err != QNetworkReply::NoError)
    {
        vcr.Error = reply->errorString();
        return vcr;
    }

    QString text = reply->readAll();
    reply->deleteLater();
    text.remove('"');

    if ((text.isEmpty()) || (text.isNull())) {
        qCCritical(fLibUpdater) << "Client version from API is empty or null. There may be no connection to the API.";
    }

    qCDebug(fLibUpdater) << "Current client version: " << text;

    FusionVersion v = strToVersion(text);
    vcr.VersionOnline = v;
    vcr.Error = "NoError";
    return vcr;
}

QString FClientUpdater::VersionToStr(FusionVersion v)
{
    return QString::number(v.Major) + "." +  QString::number(v.Minor) + "." +  QString::number(v.Build);
}

VersionCheckResult FClientUpdater::checkForUpdate(bool useNightly)
{
    VersionCheckResult result;
    VersionCheckResult latestOnline;
    FusionVersion installedVersion = getInstalledVersion();

    if (useNightly)
        latestOnline = getLatestVersion(FusionVersions::Nightly);
    else
        latestOnline = getLatestVersion(FusionVersions::Stable);

    result = latestOnline;

    if (latestOnline.Error != "NoError")
    {
        result.Status = ErrorOnCheckingOnline;
        return result;
    }

    if (!installedVersion.initialized)
    {
        result.Status = ErrorOnCheckingLocal;
        return result;
    }

    if (latestOnline.VersionOnline > installedVersion)
    {
        result = latestOnline;
        if (latestOnline.Source == FusionSources::srcStable || latestOnline.Source == FusionSources::srcStable_Alt)
            result.Status = FUpdaterResult::StableAvailable;
        else
            result.Status = FUpdaterResult::NightlyAvailable;
    }
    else
    {
        result.Status = FUpdaterResult::UpToDate;
    }

    result.VersionLocal = installedVersion;
    return result;
}

//Gets downloaded client version from file.
FusionVersion FClientUpdater::getInstalledVersion()
{
    QString filePath = LibFusion::getWorkingDir().absolutePath() + "/" + QString(UPDATER_LOCAL_VERSIONFILE_NAME);
    if (fileExists(filePath))
    {
        return strToVersion(FClientUpdater::readVersion(filePath));
    }
    else
    {
        qCWarning(fLibUpdater) << "Unable to find version file.";
        return strToVersion("");//Returns 0.0.0
    }
}

FusionVersion FClientUpdater::strToVersion(QString VStr)
{
    FusionVersion v;
    v.Build = v.Minor = v.Major = 0;
    //Proper Version: 1.2.3

    QStringList tmp = VStr.split("\n", QString::SkipEmptyParts);

    if (tmp.length()!= 2)
        return v;


    tmp[0] = QString(tmp.at(0)).replace("\r", "");

    QStringList versionParts = tmp[0].split(".", QString::SkipEmptyParts);

    if (versionParts.length() != 3)
        return v;

    bool convOK;

    int Major = versionParts[0].toInt(&convOK);

    if (!convOK)
        return v;


    int Minor = versionParts[1].toInt(&convOK);

    if (!convOK)
        return v;


    int Build = versionParts[2].toInt(&convOK);

    if (!convOK)
        return v;

    QString Name = tmp[1];

    if (Name.length() <= 0)
        return v;

    v.Build = Build;
    v.Minor = Minor;
    v.Major = Major;
    v.Name = Name;
    v.initialized = true;
    return v;
}


bool FClientUpdater::fileExists(QString filePath)
{

    qCDebug(fLibUpdater) << filePath << " exists: " << qd->exists(filePath);
    return qd->exists(filePath);
}


void FClientUpdater::writeVersion(QString version, QString currentPath)
{
    QFile file(LibFusion::getWorkingDir().absolutePath() + "/" + QString(UPDATER_LOCAL_VERSIONFILE_NAME));

    if (!file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate))
        return;

    file.write(version.toLatin1(), version.length());
    file.close();



    QFile filePath(LibFusion::getWorkingDir().absolutePath() + "/FPath.txt");

    if (!filePath.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate))
            return;

    filePath.write(currentPath.toLatin1(), currentPath.length());
    filePath.close();
}

QString FClientUpdater::readVersion(QString filePath)
{

    QFile File;
    File.setFileName(filePath);
    File.open(QIODevice::ReadOnly|QIODevice::Text);
    QString fileContent = File.readAll();
    File.close();

    return fileContent;
}


QString FClientUpdater::readPath()
{
    QFile File;
    File.setFileName(LibFusion::getWorkingDir().absolutePath() + "/FPath.txt");
    File.open(QIODevice::ReadOnly|QIODevice::Text);
    QString fileContent = File.readAll();
    File.close();

    return fileContent;
}
