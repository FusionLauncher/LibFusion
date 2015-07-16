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

    bool oldClientExists();

    //Get progress
    qint64 getCurrentProgress();
    qint64 getTotalProgress();

private:

    QNetworkAccessManager *manager;
    QDir *qd;
    QString clientLinuxDirectory = QDir::currentPath() + "/FusionClient";
    QString clientWindowsDirectory = QDir::currentPath() + "/FusionClient.exe";
    QString oldClientDirectory = QDir::currentPath() + "/FusionClient.OLD";
    QString restoreClientDirectory = QDir::currentPath() + "/FusionClient.RESTORE";
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
