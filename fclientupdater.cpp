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

    if (latestStable_alt.version > latestStable.version)
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

        if (latestNightly_alt.version > latestNightly.version)
        {
            latestNightly = latestNightly_alt;
            nightlySource = FusionSources::srcNightly_Alt;
        }

        if (latestNightly.version > latestStable.version)
        {
            latestSource = nightlySource;
            latestVersion = latestNightly;
        }
    }

    latestVersion.source = latestSource;


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
        vcr.error = reply->errorString();
        return vcr;
    }

    QString text = reply->readAll();
    reply->deleteLater();
    text.remove('"');

    if ((text.isEmpty()) || (text.isNull()))
    {
        qDebug() << "[ERROR] Client version from API is empty or null. There may be no connection to the API.";;
    }

    qDebug() << "Current client version: " << text;

    FusionVersion v = strToVersion(text);
    vcr.version = v;
    vcr.error = "NoError";
    return vcr;
}

QString FClientUpdater::VersionToStr(FusionVersion v)
{
    return QString::number(v.Major) + "." +  QString::number(v.Minor) + "." +  QString::number(v.Build);
}

FUpdaterResult FClientUpdater::checkForUpdate()
{
    bool useNightly = db.getBoolPref("useNightlyVersions", false);

    FUpdaterResult result;
    FusionVersion latestOnline;
    FusionVersion installedVersion = getInstalledVersion();

    if (useNightly)
        latestOnline = getLatestVersion(FusionVersions::Nightly).version;
    else
        latestOnline = getLatestVersion(FusionVersions::Stable).version;

    if(latestOnline>installedVersion)
        result = FUpdaterResult::
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
        qDebug() << "Unable to find version file.";
        return strToVersion("");//Returns 0.0.0
    }
}

FusionVersion FClientUpdater::strToVersion(QString VStr)
{
    FusionVersion v;
    v.Build = v.Minor = v.Major = 0;
    //Proper Version: 1.2.3

    QStringList tmp = VStr.split("\n", QString::SkipEmptyParts);
    tmp[0] = QString(tmp.at(0)).replace("\r", "");

    if (tmp.length()!= 2)
        return v;

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

 //   qDebug() << filePath << " exists: " << qd->exists(filePath);
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

    QFile oLogFile;
    oLogFile.setFileName(filePath);
    oLogFile.open(QIODevice::ReadOnly|QIODevice::Text);
    QString fileContent = oLogFile.readAll();

    oLogFile.close();

    return fileContent;
}


QString FClientUpdater::readPath()
{
    QString fileVersion;

    QFile file(LibFusion::getWorkingDir().absolutePath() + "/FPath.txt");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);

    in >> fileVersion;
    fileVersion = file.readAll();
    file.close();

    return fileVersion;
}
