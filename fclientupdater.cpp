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

//Gets downloaded client version from file.
QString FClientUpdater::getDLClientVersion()
{
    if ((clientExists(1)) || (clientExists)(2))
    {

        qDebug() << "Downloaded client version: 0.0.1";
        return "0.0.1"; //Read downloaded version from a file.
    }
    else
    {

        qDebug() << "There is no downloaded client.";
        return "NA";
    }
}

//Compare downloaded client version with current client version.
bool FClientUpdater::isCurrentClient()
{

    if (getDLClientVersion() == getCRClientVersion())
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

    else if (getDLClientVersion() == "NA")
    {

        //There is no downloaded client.
        qDebug() << "There is no downloaded client.";

        return true;
    }

    else
    {
        qDebug() << "Downloaded client version does not match current client version.";

        return false;
    }
}

//Downloads current linux client.
void FClientUpdater::downloadLinuxClient()
{

    qDebug() << "Attempting to download linux client.";
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(clientReplyFinishedLinux(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(clientLinuxUrl)));
}

void FClientUpdater::clientReplyFinishedLinux(QNetworkReply *reply)
{
    reply->deleteLater();
    reply->ignoreSslErrors();

    if(reply->error())
    {
        qDebug() << "[ERROR] Client download reply error.";
        qDebug() << reply->errorString();
    }
    else if(reply->url() != clientLinuxUrl)
    {

        qDebug() << "[ERROR] Client reply URL does not match real client URL.";
        return;
    }
    else
    {

        FFileDownloader *downloader = new FFileDownloader(clientLinuxUrl, clientLinuxDirectory); //Change this to client file name later.
        qDebug() << "Downloaded linux client.";
    }
}

//Downloads current windows client.
void FClientUpdater::downloadWindowsClient()
{

    qDebug() << "Attempting to download windows client.";
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(clientReplyFinishedWindows(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(clientWindowsUrl)));
}

void FClientUpdater::clientReplyFinishedWindows(QNetworkReply *reply)
{
    reply->deleteLater();
    reply->ignoreSslErrors();

    if(reply->error())
    {
        qDebug() << "[ERROR] Client download reply error.";
        qDebug() << reply->errorString();
    }
    else if(reply->url() != clientWindowsUrl)
    {

        qDebug() << "[ERROR] Client reply URL does not match real client URL.";
        return;
    }
    else
    {

        FFileDownloader *downloader = new FFileDownloader(clientWindowsUrl, clientWindowsDirectory);
        qDebug() << "Downloaded windows client.";
    }
}

//Replaces downloaded client with current client.
void FClientUpdater::updateClient(int i)
{

    if (i == 1)
    {

        qDebug() << "Attempting to update Linux client.";

        //Rename downloaded client.
        qd->rename(clientLinuxDirectory, oldClientLinuxDirectory);
        qDebug() << "Renamed CURRENT to OLD";

        //Download current client.
        downloadLinuxClient();
    }
    else if (i == 2)
    {

        qDebug() << "Attempting to update Windows client.";

        //Rename downloaded client.
        qd->rename(clientWindowsDirectory, oldClientWindowsDirectory);
        qDebug() << "Renamed CURRENT to OLD";

        //Download current client.
        downloadWindowsClient();
    }
}

//Restores previous client.
void FClientUpdater::restoreClient(int i)
{

    if (i == 1)
    {

        qDebug() << "Attempting to restore linux client.";

        //Rename previous client.
        qd->rename(oldClientLinuxDirectory, restoreClientLinuxDirectory);
        qDebug() << "Renamed OLD to RESTORE.";

        //Rename unwanted client.
        qd->rename(clientLinuxDirectory, oldClientLinuxDirectory);
        qDebug() << "Renamed CURRENT to OLD.";

        //Rename previous client again.
        qd->rename(restoreClientLinuxDirectory, clientLinuxDirectory);
        qDebug() << "Renamed RESTORE to CURRENT";
    }

    else if (i == 1)
    {

        qDebug() << "Attempting to restore windows client.";

        //Rename previous client.
        qd->rename(oldClientWindowsDirectory, restoreClientWindowsDirectory);
        qDebug() << "Renamed OLD to RESTORE.";

        //Rename unwanted client.
        qd->rename(clientWindowsDirectory, oldClientWindowsDirectory);
        qDebug() << "Renamed CURRENT to OLD.";

        //Rename previous client again.
        qd->rename(restoreClientWindowsDirectory, clientWindowsDirectory);
        qDebug() << "Renamed RESTORE to CURRENT";
    }

}

//Returns true if the client exists.
bool FClientUpdater::clientExists(int i)
{

    if (i == 1)
    {

        qDebug() << "Linux Client exists: " << qd->exists(clientLinuxDirectory);
        return qd->exists(clientLinuxDirectory);
    }
    else if (i == 2)
    {

        qDebug() << "Windows Client exists: " << qd->exists(clientWindowsDirectory);
        return qd->exists(clientWindowsDirectory);
    }
}

//Returns true if old client exists.
bool FClientUpdater::oldClientExists(int i)
{
    if (i == 1)
    {

        qDebug() << "Old Linux Client exists: " << qd->exists(oldClientLinuxDirectory);
        return qd->exists(oldClientLinuxDirectory);
    }
    else if (i == 2)
    {

        qDebug() << "Old Windows Client exists: " << qd->exists(oldClientWindowsDirectory);
        return qd->exists(oldClientWindowsDirectory);
    }
}
