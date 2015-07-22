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

class FClientUpdater : public QObject
{
    Q_OBJECT
public:
    explicit FClientUpdater(QObject *parent = 0);
    QString getCRClientVersion();

    //Get downloaded client version.
    QString getDLClientLinuxVersion(QString path);
    QString getDLClientWindowsVersion(QString path); //TODO: Read version from file will only need one function.

    //Check if current client
    bool isCurrentLinuxClient(QString path);
    bool isCurrentWindowsClient(QString path);

    //Check for clients
    bool clientLinuxExists(QString path);
    bool clientWindowsExists(QString path);

    //Check for old clients
    bool oldClientLinuxExists(QString path);
    bool oldClientWindowsExists(QString path);

private:

    QNetworkAccessManager *manager;
    QDir *qd;

signals:

public slots:

};

#endif // FCLIENTUPDATER_H
