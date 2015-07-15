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
    if (clientExists())
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

//Downloads current client.
void FClientUpdater::downloadClient(int i)
{

    qDebug() << "Attempting to download client.";
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(clientReplyFinished(QNetworkReply*, i)));

    if (i == 1)
    {

        manager->get(QNetworkRequest(QUrl(clientLinuxUrl)));
    }
    else if (i == 2)
    {

        manager->get(QNetworkRequest(QUrl(clientWindowsUrl)));
    }
    else
    {

        qDebug() << "Error choosing os.";
    }
}

void FClientUpdater::clientReplyFinished(QNetworkReply *reply, int i)
{
    reply->deleteLater();
    reply->ignoreSslErrors();

    if(reply->error())
    {
        qDebug() << "[ERROR] Client download reply error.";
        qDebug() << reply->errorString();
    }
    else if((reply->url() != clientLinuxUrl) || (reply->url() != clientWindowsUrl))
    {

        qDebug() << "[ERROR] Client reply URL does not match real client URL.";
        return;
    }
    else
    {

        if (i == 1)
        {

            FFileDownloader *downloader = new FFileDownloader(clientLinuxUrl, clientDirectory); //Change this to client file name later.
            qDebug() << "Client downloaded.";
        }

        else if (i == 2)
        {

            FFileDownloader *downloader = new FFileDownloader(clientWindowsUrl, clientDirectory); //Change this to client file name later.
            qDebug() << "Client downloaded.";
        }

        else
        {

            qDebug() << "Error downloading client.";
        }
    }
}

//Replaces downloaded client with current client.
void FClientUpdater::updateClient(int i)
{

    qDebug() << "Attempting to update client.";

    //Rename downloaded client.
    qd->rename(clientDirectory, oldClientDirectory);
    qDebug() << "Renamed CURRENT to OLD";

    //Download current client.
    downloadClient(i);
}

//Restores previous client.
void FClientUpdater::restoreClient()
{

    qDebug() << "Attempting to restore client.";

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

    qDebug() << "Client exists: " << qd->exists(clientDirectory);
    return qd->exists(clientDirectory);
}

//Returns true if old client exists.
bool FClientUpdater::oldClientExists()
{

    qDebug() << "Old Client exists: " << qd->exists(oldClientDirectory);
    return qd->exists(oldClientDirectory);
}
