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
    QString getDLClientVersion();
    bool isCurrentClient();

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

    //Get progress
    qint64 getCurrentProgress();
    qint64 getTotalProgress();

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

    qint64 currentProgress;
    qint64 totalProgress;

signals:

public slots:

    void updateDownloadProgress(qint64 current, qint64 total);

    void linuxFinished(QNetworkReply *reply);
    void windowsFinished();
};

#endif // FCLIENTUPDATER_H
