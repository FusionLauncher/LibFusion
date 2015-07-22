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
    QString getDLClientLinuxVersion(QString filePath);
    QString getDLClientWindowsVersion(QString filePath); //TODO: Read version from file will only need one function.

    //Check if current client
    bool isCurrentLinuxClient(QString filePath);
    bool isCurrentWindowsClient(QString filePath);

    //Check for clients
    bool clientLinuxExists(QString filePath);
    bool clientWindowsExists(QString filePath);

    //Check for old clients
    bool oldClientLinuxExists(QString filePath);
    bool oldClientWindowsExists(QString filePath);

private:

    QNetworkAccessManager *manager;
    QDir *qd = new QDir();

signals:

public slots:

};

#endif // FCLIENTUPDATER_H
