#ifndef FDBUPDATER_H
#define FDBUPDATER_H

#include <QObject>
#include "fdb.h"

class FDBUpdater : public QObject
{
    Q_OBJECT
public:
    explicit FDBUpdater(QObject *parent = 0, FDB *db = 0);
    bool checkForDBUpdate();
    bool updateDB();
    bool versionExists();
    void initVersion();
private:
    FDB *db;
    bool foundVersion;
    QString latestVersion;

signals:

public slots:
};

#endif // FDBUPDATER_H
