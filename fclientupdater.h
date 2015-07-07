#ifndef FCLIENTUPDATER_H
#define FCLIENTUPDATER_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QUrl>
#include <QString>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QFile>
#include <QDir>

#include <QDebug>

#include "ffiledownloader.h"

class FClientUpdater : public QObject
{
    Q_OBJECT
public:
    explicit FClientUpdater(QObject *parent = 0);
    QString getCRClientVersion();
    QString getDLClientVersion();
    bool isCurrentClient();
    void downloadClient();
    void updateClient();
    void restoreClient();
    bool clientExists();
    bool oldClientExists();

private:

    QNetworkAccessManager *manager;
    QDir *qd;
    QString clientDirectory = QDir::currentPath() + "/FusionClient.txt"; //Change this to client file name later.
    QString oldClientDirectory = QDir::currentPath() + "/OLDFusionClient.txt"; //Change this to client file name later.
    QString restoreClientDirectory = QDir::currentPath() + "/RESTOREFusionClient.txt"; //Change this to client file name later.
    QString versionFileDirectory = QDir::currentPath() + "/fversion.txt";
    QString fusionDirectory = QDir::currentPath();
    QString clientUrl = "http://pacific-citadel-1552.herokuapp.com/api/version/fusionClient";//Change this to URL of client later.

    int i = 1;


signals:

public slots:

    void clientReplyFinished(QNetworkReply *reply);

};

#endif // FCLIENTUPDATER_H
