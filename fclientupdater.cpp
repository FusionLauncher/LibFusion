#include "fclientupdater.h"
#include "libfusion.h"

FClientUpdater::FClientUpdater(QObject *parent) : QObject(parent)
{

}

//Gets current client version from API.
QString FClientUpdater::getCRClientVersion()
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
        qDebug() << "[ERROR] Client version from API is empty or null. There may be no connection to the API."; return "NA";
    }
    qDebug() << "Current client version: " << text;
    return text;
}

//Gets downloaded client version from file.
QString FClientUpdater::getDLClientVersion(QString filePath)
{
    if (fileExists(filePath))
    {

        qDebug() << "Downloaded client version: " << FClientUpdater::readVersion(filePath);
        return FClientUpdater::readVersion(filePath);
    }
    else
    {

        qDebug() << "Unable to find version file.";
        return "NA";
    }
}

//Compare downloaded client version with current client version.
bool FClientUpdater::isCurrentClient(QString path)
{

    if (getDLClientVersion(path) == getCRClientVersion())
    {
        qDebug() << "Downloaded client version does match current client version.";

        return true;
    }

    else if (getCRClientVersion() == "NA")
    {
        //There is no connection to the api.
        qDebug() << "[ERROR] Client version from API is empty or null. There may be no connection to the API.";

        return true;
    }

    else if (getDLClientVersion(path) == "NA")
    {

        qDebug() << "There is no downloaded client.";

        return true;
    }

    else
    {
        qDebug() << "Downloaded client version does not match current client version.";

        return false;
    }
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
