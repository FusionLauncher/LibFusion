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
    void downloadLinuxClient();
    void downloadWindowsClient();
    void updateClient(int i);
    void restoreClient(int i);
    bool clientExists(int i);
    bool oldClientExists(int i);

private:

    QNetworkAccessManager *manager;
    QDir *qd;
    QString clientLinuxDirectory = QDir::currentPath() + "/FusionClient";
    QString clientWindowsDirectory = QDir::currentPath() + "/FusionClient.exe";
    QString oldClientLinuxDirectory = QDir::currentPath() + "/OLDFusionClient";
    QString oldClientWindowsDirectory = QDir::currentPath() + "/OLDFusionClient.exe";
    QString restoreClientLinuxDirectory = QDir::currentPath() + "/RESTOREFusionClient";
    QString restoreClientWindowsDirectory = QDir::currentPath() + "/RESTOREFusionClient";
    QString fusionDirectory = QDir::currentPath();
    QString clientLinuxUrl = "http://70.72.248.199/Resources/FusionClient";
    QString clientWindowsUrl = "http://70.72.248.199/Resources/FusionClient.exe";

signals:

public slots:

    void clientReplyFinishedLinux(QNetworkReply *reply);

    void clientReplyFinishedWindows(QNetworkReply *reply);
};

#endif // FCLIENTUPDATER_H
