#include "fupdater.h"
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QUrl>
#include <QEventLoop>
#include <QObject>

FUpdater::FUpdater()
{

}

//Get FusionLib version from api.
QString FUpdater::getLibraryVersion()
{

    QWebView *webview = new QWebView();

    webview->load(QUrl("http://pacific-citadel-1552.herokuapp.com/api/version/fusionLib"));
    QEventLoop loop;
    QObject::connect(webview,SIGNAL(loadFinished(bool)),&loop,SLOT(quit()));
    loop.exec();

    QString libVer = webview->page()->mainFrame()->toPlainText();
    libVer.remove('"');

    if ((libVer.isEmpty()) || (libVer.isNull())) { return "NA"; }

    return libVer;
}

//Get FusionClient version from api.
QString FUpdater::getClientVersion()
{

    QWebView *webview = new QWebView();

    webview->load(QUrl("http://pacific-citadel-1552.herokuapp.com/api/version/fusionClient"));
    QEventLoop loop;
    QObject::connect(webview,SIGNAL(loadFinished(bool)),&loop,SLOT(quit()));
    loop.exec();

    QString clientVer = webview->page()->mainFrame()->toPlainText();
    clientVer.remove('"');

    if ((clientVer.isEmpty()) || (clientVer.isNull())) { return "NA"; }

    return clientVer;
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
