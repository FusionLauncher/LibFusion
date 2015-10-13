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

struct FusionVersion {
    int Major = 0;
    int Minor = 0;
    int Build = 0;
    QString Name = "";
    bool initialized = false;

    bool operator==(FusionVersion a) {
       return a.Build==Build && a.Minor==Minor && a.Major==Major;
    }
};

class LIBFUSIONSHARED_EXPORT FClientUpdater : public QObject
{
    Q_OBJECT
public:
    explicit FClientUpdater(QObject *parent = 0);
    FusionVersion getCRClientVersion();

    //Get downloaded client version.
    FusionVersion getDLClientVersion(QString filePath);

    bool fileExists(QString filePath);

    //Read/Write version info
    void writeVersion(QString version, QString currentPath);
    QString readVersion(QString filePath);
    QString readPath();

    FusionVersion strToVersion(QString VStr);
    QString VersionToStr(FusionVersion v);
private:

    QNetworkAccessManager *manager;
    QDir *qd = new QDir();

signals:

public slots:

};

#endif // FCLIENTUPDATER_H
