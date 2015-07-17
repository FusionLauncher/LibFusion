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
QString FClientUpdater::getDLClientLinuxVersion()
{
    if (clientLinuxExists())
    {

        qDebug() << "Downloaded linux client version: 0.0.1";
        return "0.0.1"; //Read downloaded version from a file.
    }
    else
    {

        qDebug() << "There is no downloaded linux client.";
        return "NA";
    }
}

//Gets downloaded windows client version from file.
QString FClientUpdater::getDLClientWindowsVersion()
{
    if (clientWindowsExists())
    {

        qDebug() << "Downloaded windows client version: 0.0.1";
        return "0.0.1"; //Read downloaded version from a file.
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
bool FClientUpdater::isCurrentLinuxClient()
{

    if (getDLClientLinuxVersion() == getCRClientVersion())
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

    else if (getDLClientLinuxVersion() == "NA")
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
bool FClientUpdater::isCurrentWindowsClient()
{

    if (getDLClientWindowsVersion() == getCRClientVersion())
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

    else if (getDLClientWindowsVersion() == "NA")
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

//Downloads current linux client.
void FClientUpdater::downloadLinuxClient()
{

    manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(clientLinuxUrl);

    QNetworkReply *reply = manager->get(request);

    QObject::connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateDownloadProgress(qint64,qint64)));

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(linuxFinished(QNetworkReply*)));

    /*qDebug() << "Attempting to download linux client.";
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(clientReplyFinishedLinux(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(clientLinuxUrl)));*/
}

void FClientUpdater::linuxFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    QByteArray ba = reply->readAll();
    QFile file(clientLinuxDirectory);

    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);

    out << ba;
}

void FClientUpdater::windowsFinished()
{

}

void FClientUpdater::updateDownloadProgress(qint64 current, qint64 total)
{

    this->totalProgress = total;
    this->currentProgress = current;
}

qint64 FClientUpdater::getTotalProgress()
{

    return this->totalProgress;
}

qint64 FClientUpdater::getCurrentProgress()
{

    return this->currentProgress;
}

//void FClientUpdater::clientReplyFinishedLinux(QNetworkReply *reply)
//{
    /*reply->deleteLater();
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
    }*/
//}

//Downloads current windows client.
void FClientUpdater::downloadWindowsClient()
{

    qDebug() << "Attempting to download windows client.";
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(clientReplyFinishedWindows(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(clientWindowsUrl)));
}

/*void FClientUpdater::clientReplyFinishedWindows(QNetworkReply *reply)
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
}*/

//Replaces downloaded client with current client.
void FClientUpdater::updateLinuxClient()
{

    qDebug() << "Attempting to update Linux client.";

    //Rename downloaded client.
    qd->rename(clientLinuxDirectory, oldClientLinuxDirectory);
    qDebug() << "Renamed CURRENT to OLD";

    //Download current client.
    downloadLinuxClient();
}

//Replaces downloaded client with current client.
void FClientUpdater::updateWindowsClient()
{

    qDebug() << "Attempting to update Windows client.";

    //Rename downloaded client.
    qd->rename(clientWindowsDirectory, oldClientWindowsDirectory);
    qDebug() << "Renamed CURRENT to OLD";

    //Download current client.
    downloadWindowsClient();
}

//Restores previous client.
void FClientUpdater::restoreLinuxClient()
{

        qDebug() << "Attempting to restore Linux client.";

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

//Restores previous client.
void FClientUpdater::restoreWindowsClient()
{

        qDebug() << "Attempting to restore Windows client.";

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

//Returns true if the linux client exists.
bool FClientUpdater::clientLinuxExists()
{

    qDebug() << "Linux Client exists: " << qd->exists(clientLinuxDirectory);
    return qd->exists(clientLinuxDirectory);
}

//Returns true if the windows client exists.
bool FClientUpdater::clientWindowsExists()
{

    qDebug() << "Windows Client exists: " << qd->exists(clientWindowsDirectory);
    return qd->exists(clientWindowsDirectory);
}

//Returns true if old linux client exists.
bool FClientUpdater::oldClientLinuxExists()
{

    qDebug() << "Old Linux Client exists: " << qd->exists(oldClientLinuxDirectory);
    return qd->exists(oldClientLinuxDirectory);
}

//Returns true if old windows client exists.
bool FClientUpdater::oldClientWindowsExists()
{

    qDebug() << "Old Windows Client exists: " << qd->exists(oldClientWindowsDirectory);
    return qd->exists(oldClientWindowsDirectory);
}
