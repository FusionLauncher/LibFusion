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

#include "fdb.h"
#include "libfusion_global.h"

enum FUpdaterResult { UpToDate, ErrorOnChecking, StableAvailable, NightlyAvailable };
enum FusionVersions { Stable, Beta, Nightly };
enum FusionSources { srcStable, srcStable_Alt, srcNightly, srcNightly_Alt };


struct FusionVersion {
    int Major = 0;
    int Minor = 0;
    int Build = 0;
    QString Name = "";
    bool initialized = false;

    bool operator==(FusionVersion a) {
       return a.Build == Build && a.Minor == Minor && a.Major == Major;
    }


    bool operator>(FusionVersion a) {
        if (Major > a.Major)
            return true;
         else
         {
            if(Minor > a.Minor )
                return true;
            else
            {
                if(Build > a.Build)
                    return true;
            }
        }
        return false;
    }

    bool operator<(FusionVersion a) {
        if (Major < a.Major)
            return true;
         else
         {
            if(Minor < a.Minor )
                return true;
            else
            {
                if(Build < a.Build)
                    return true;
            }
        }
        return false;
    }
};


struct VersionCheckResult {
    FusionVersion version;
    FusionSources source;
    FUpdaterResult Status;
    QString error;
};


class LIBFUSIONSHARED_EXPORT FClientUpdater : public QObject
{
    Q_OBJECT
public:
    explicit FClientUpdater(QObject *parent = 0);
    VersionCheckResult getLatestVersion(FusionVersions version);

    //Get downloaded client version.
    FusionVersion getInstalledVersion();

    bool fileExists(QString filePath);

    //Read/Write version info
    void writeVersion(QString version, QString currentPath);
    QString readVersion(QString filePath);
    QString readPath();

    FusionVersion strToVersion(QString VStr);
    QString VersionToStr(FusionVersion v);

    VersionCheckResult checkForUpdate();
private:

    QNetworkAccessManager *manager;
    QDir *qd = new QDir();
    FDB db;
    VersionCheckResult readOnlineVersionFile(QString URL);
signals:

public slots:

};

#endif // FCLIENTUPDATER_H
