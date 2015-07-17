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
#include <QFile>
#include <QDataStream>

#include <QDebug>

#include "ffiledownloader.h"

class FClientUpdater : public QObject
{
    Q_OBJECT
public:
    explicit FClientUpdater(QObject *parent = 0);
    QString getCRClientVersion();

    //Get downloaded client version.
    QString getDLClientLinuxVersion();
    QString getDLClientWindowsVersion();

    //Check if current client
    bool isCurrentLinuxClient();
    bool isCurrentWindowsClient();

    //Download clients
    void downloadLinuxClient();
    void downloadWindowsClient();

    //Update clients
    void updateLinuxClient();
    void updateWindowsClient();

    //Restore clients
    void restoreLinuxClient();
    void restoreWindowsClient();

    //Check for clients
    bool clientLinuxExists();
    bool clientWindowsExists();

    //Check for old clients
    bool oldClientLinuxExists();
    bool oldClientWindowsExists();

private:

    QNetworkAccessManager *manager;
    QDir *qd;
    QString clientLinuxDirectory = QDir::currentPath() + "/FusionClient";
    QString clientWindowsDirectory = QDir::currentPath() + "/FusionClient.exe";
    QString oldClientLinuxDirectory = QDir::currentPath() + "/FusionClientL.OLD";
    QString oldClientWindowsDirectory = QDir::currentPath() + "/FusionClientW.OLD";
    QString restoreClientLinuxDirectory = QDir::currentPath() + "/FusionClientL.RESTORE";
    QString restoreClientWindowsDirectory = QDir::currentPath() + "/FusionClientW.RESTORE";
    QString fusionDirectory = QDir::currentPath();
    QString clientLinuxUrl = "http://70.72.248.199/Resources/FusionClient";
    QString clientWindowsUrl = "http://70.72.248.199/Resources/FusionClient.exe";

signals:

public slots:
    void clientReplyFinishedLinux(QNetworkReply *reply);
    void clientReplyFinishedWindows(QNetworkReply *reply);

};

#endif // FCLIENTUPDATER_H
