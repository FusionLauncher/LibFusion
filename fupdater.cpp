#include "fupdater.h"

FUpdater::FUpdater(QObject *parent) : QObject(parent)
{

}

//Compare downloaded library version with library version from api.
QString FUpdater::getLibraryVersion()
{
    manager = new QNetworkAccessManager(this);
    QEventLoop loop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkRequest request(QUrl("http://pacific-citadel-1552.herokuapp.com/api/version/fusionLib"));
    QNetworkReply *reply = manager->get(request);
    loop.exec();

    QString text = reply->readAll();
    reply->deleteLater();
    text.remove('"');

    if ((text.isEmpty()) || (text.isNull())) { return "NA"; }
    return text;
}

//Compare downloaded client version with client version from api.
QString FUpdater::getClientVersion()
{
    manager = new QNetworkAccessManager(this);
    QEventLoop loop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkRequest request(QUrl("http://pacific-citadel-1552.herokuapp.com/api/version/fusionClient"));
    QNetworkReply *reply = manager->get(request);
    loop.exec();

    QString text = reply->readAll();
    reply->deleteLater();
    text.remove('"');

    if ((text.isEmpty()) || (text.isNull())) { return "NA"; }
    return text;
}

//Compare downloaded library version with library version from api.
bool FUpdater::isCurrentLibrary(QString thisLibrary)
{

    if (thisLibrary == getLibraryVersion())
    {

        return true;
    }

    else if ( getLibraryVersion() == "NA" )
    {
        //There is no connection to the api.
        return true;
    }

    else
    {

        return false;
    }
}

//Compare downloaded client version with client version from api.
bool FUpdater::isCurrentClient(QString thisClient)
{

    if (thisClient == getClientVersion())
    {

        return true;
    }

    else if ( getClientVersion() == "NA" )
    {
        //There is no connection to the api.
        return true;
    }

    else
    {

        return false;
    }
}

//TODO: Add functions to download things.
