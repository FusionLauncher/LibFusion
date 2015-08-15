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


#include "libfusion_global.h"

class LIBFUSIONSHARED_EXPORT FClientUpdater : public QObject
{
    Q_OBJECT
public:
    explicit FClientUpdater(QObject *parent = 0);
    QString getCRClientVersion();

    //Get downloaded client version.
    QString getDLClientVersion(QString filePath);

    //Check if current client
    bool isCurrentClient(QString filePath);

    bool fileExists(QString filePath);

    //Read/Write version info
    void writeVersion(QString version, QString filePath);
    QString readVersion(QString filePath);

private:

    QNetworkAccessManager *manager;
    QDir *qd = new QDir();

signals:

public slots:

};

#endif // FCLIENTUPDATER_H
