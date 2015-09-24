#include "fclientupdater.h"
#include "libfusion.h"

FClientUpdater::FClientUpdater(QObject *parent) : QObject(parent)
{

}

//Gets current client version from API.
FusionVersion FClientUpdater::getCRClientVersion()
{
    manager = new QNetworkAccessManager(this);
    QEventLoop loop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkRequest request(QUrl("http://projfusion.com/files/Releases/version.txt"));
    QNetworkReply *reply = manager->get(request);
    reply->ignoreSslErrors();

    loop.exec();

    QString text = reply->readAll();
    reply->deleteLater();
    text.remove('"');

    if ((text.isEmpty()) || (text.isNull())) {
        qDebug() << "[ERROR] Client version from API is empty or null. There may be no connection to the API.";;
    }

    qDebug() << "Current client version: " << text;

    FusionVersion v = strToVersion(text);
    return v;
}

QString FClientUpdater::VersionToStr(FusionVersion v) {
    return QString::number(v.Major) + "." +  QString::number(v.Minor) + "." +  QString::number(v.Build);
}

//Gets downloaded client version from file.
FusionVersion FClientUpdater::getDLClientVersion(QString filePath)
{
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

FusionVersion FClientUpdater::strToVersion(QString VStr) {
    FusionVersion v;
    v.Build = v.Minor = v.Major = 0;
    //Proper Version: 1.2.3

    if(VStr.length() != 5)
        return v;

    bool convOK;

    int Major = VStr.left(1).toInt(&convOK);

    if(!convOK)
        return v;


    int Minor = VStr.mid(2,1).toInt(&convOK);

    if(!convOK)
        return v;


    int Build = VStr.mid(4,1).toInt(&convOK);

    if(!convOK)
        return v;

    v.Build = Build;
    v.Minor = Minor;
    v.Major = Major;
    v.initialized = true;
    return v;
}


//Returns true if file exists
bool FClientUpdater::fileExists(QString filePath)
{

 //   qDebug() << filePath << " exists: " << qd->exists(filePath);
    return qd->exists(filePath);
}

//Write version info
void FClientUpdater::writeVersion(QString version, QString currentPath)
{
    QFile file(LibFusion::getWorkingDir().absolutePath() + "/FVersion.txt");
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate))
        return;

    QDataStream out(&file);

    out << version.toLatin1();
    file.close();

    QFile filePath(LibFusion::getWorkingDir().absolutePath() + "/FPath.txt");
    if (!filePath.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate))
            return;

    QDataStream outP(&filePath);
    outP << currentPath.toLatin1();
    filePath.close();

}

QString FClientUpdater::readVersion(QString filePath)
{
    QString fileVersion;

    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);

    in >> fileVersion;
    fileVersion = file.readAll();
    file.close();
    return fileVersion;
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
