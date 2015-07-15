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
    void downloadClient(int i);
    void updateClient(int i);
    void restoreClient();
    bool clientExists();
    bool oldClientExists();

private:

    QNetworkAccessManager *manager;
    QDir *qd;
    QString clientDirectory = QDir::currentPath() + "/FusionClient.txt"; //Change this to client file name later.
    QString oldClientDirectory = QDir::currentPath() + "/OLDFusionClient.txt"; //Change this to client file name later.
    QString restoreClientDirectory = QDir::currentPath() + "/RESTOREFusionClient.txt"; //Change this to client file name later.
    QString fusionDirectory = QDir::currentPath();
    QString clientLinuxUrl = "http://70.72.248.199/Resources/FusionClient";
    QString clientWindowsUrl = "http://70.72.248.199/Resources/FusionClient.exe";

signals:

public slots:

    void clientReplyFinished(QNetworkReply *reply, int i);
};

#endif // FCLIENTUPDATER_H
