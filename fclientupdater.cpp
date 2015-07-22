#include "fclientupdater.h"

FClientUpdater::FClientUpdater(QObject *parent) : QObject(parent)
{

}

//Gets current client version from API.
QString FClientUpdater::getCRClientVersion()
{
    manager = new QNetworkAccessManager(this);
    QEventLoop loop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkRequest request(QUrl("https://pacific-citadel-1552.herokuapp.com/api/version/fusionClient"));
    QNetworkReply *reply = manager->get(request);
    reply->ignoreSslErrors();

    loop.exec();

    QString text = reply->readAll();
    reply->deleteLater();
    text.remove('"');

    if ((text.isEmpty()) || (text.isNull())) { qDebug() << "[ERROR] Client version from API is empty or null. There may be no connection to the API."; return "NA"; }
    qDebug() << "Current client version: " << text;
    return text;
}

//Gets downloaded linux client version from file.
QString FClientUpdater::getDLClientLinuxVersion(QString path)
{
    if (clientLinuxExists(path))
    {

        qDebug() << "Downloaded linux client version: 0.0.2";
        return "0.0.2"; //Read downloaded version from a file.
    }
    else
    {

        qDebug() << "There is no downloaded linux client.";
        return "NA";
    }
}

//Gets downloaded windows client version from file.
QString FClientUpdater::getDLClientWindowsVersion(QString path)
{
    if (clientWindowsExists(path))
    {

        qDebug() << "Downloaded windows client version: 0.0.2";
        return "0.0.2"; //Read downloaded version from a file.
    }
    else
    {

        qDebug() << "There is no downloaded windows client.";
        return "NA";
    }
}

/* Client will write its version to a file by the OS name.
 * Ex: if (os == win) { write.tofile("winversion=0.0.1");  }
 *     if (chosenOs = win) { read.fromfile("winversion")
 */

//Compare downloaded client version with current client version.
bool FClientUpdater::isCurrentLinuxClient(QString path)
{

    if (getDLClientLinuxVersion(path) == getCRClientVersion())
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

    else if (getDLClientLinuxVersion(path) == "NA")
    {

        //There is no downloaded client.
        qDebug() << "There is no downloaded linux client.";

        return true;
    }

    else
    {
        qDebug() << "Downloaded client version does not match current client version.";

        return false;
    }
}

//Compare downloaded client version with current client version.
bool FClientUpdater::isCurrentWindowsClient(QString path)
{

    if (getDLClientWindowsVersion(path) == getCRClientVersion())
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

    else if (getDLClientWindowsVersion(path) == "NA")
    {

        //There is no downloaded client.
        qDebug() << "There is no downloaded windows4 client.";

        return true;
    }

    else
    {
        qDebug() << "Downloaded client version does not match current client version.";

        return false;
    }
}

//Returns true if the linux client exists.
bool FClientUpdater::clientLinuxExists(QString path)
{

    qDebug() << "Linux Client exists: " << qd->exists(path);
    return qd->exists(path);
}

//Returns true if the windows client exists.
bool FClientUpdater::clientWindowsExists(QString path)
{

    qDebug() << "Windows Client exists: " << qd->exists(path);
    return qd->exists(path);
}

//Returns true if old linux client exists.
bool FClientUpdater::oldClientLinuxExists(QString path)
{

    qDebug() << "Old Linux Client exists: " << qd->exists(path);
    return qd->exists(path);
}

//Returns true if old windows client exists.
bool FClientUpdater::oldClientWindowsExists(QString path)
{

    qDebug() << "Old Windows Client exists: " << qd->exists(path);
    return qd->exists(path);
}
