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
    return text;
}

//Gets downloaded client version from file.
QString FClientUpdater::getDLClientVersion()
{
    if (clientExists())
    {

        return "0.0.1"; //Read downloaded version from a file.
    }
    else
    {

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

//Downloads current client.
void FClientUpdater::downloadClient()
{

    qDebug() << "Attempting to download client.";
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(clientReplyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl(clientUrl)));
}

void FClientUpdater::clientReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    reply->ignoreSslErrors();

    if(reply->error())
    {
        qDebug() << "[ERROR] Client download reply error.";
        qDebug() << reply->errorString();
    }
    else if(reply->url() != clientUrl)
    {

        qDebug() << "[ERROR] Client reply URL does not match real client URL.";
        return;
    }
    else
    {

        FFileDownloader *downloader = new FFileDownloader(clientUrl, clientDirectory); //Change this to client file name later.
        qDebug() << "Client downloaded.";
    }
}

//Replaces downloaded client with current client.
void FClientUpdater::updateClient()
{

    //Rename downloaded client.
    qd->rename(clientDirectory, oldClientDirectory);
    qDebug() << "Renamed CURRENT to OLD";

    //Download current client.
    downloadClient();
}

//Restores previous client.
void FClientUpdater::restoreClient()
{

    //Rename previous client.
    qd->rename(oldClientDirectory, restoreClientDirectory);
    qDebug() << "Renamed OLD to RESTORE.";

    //Rename unwanted client.
    qd->rename(clientDirectory, oldClientDirectory);
    qDebug() << "Renamed CURRENT to OLD.";

    //Rename previous client again.
    qd->rename(restoreClientDirectory, clientDirectory);
    qDebug() << "Renamed RESTORE to CURRENT";
}

//Returns true if the client exists.
bool FClientUpdater::clientExists()
{

    qDebug() << "Client Exists: " << qd->exists(clientDirectory);
    return qd->exists(clientDirectory);
}

//Returns true if old client exists.
bool FClientUpdater::oldClientExists()
{

    qDebug() << "Old Client Exists: " << qd->exists(oldClientDirectory);
    return qd->exists(oldClientDirectory);
}
