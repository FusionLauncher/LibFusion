#ifndef FDB_H
#define FDB_H

#ifndef FGAME_H
#include <fgame.h>
#endif

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "flauncher.h"

#include "fwatchedfolder.h"
#include "libfusion_global.h"

class LIBFUSIONSHARED_EXPORT FDB : public QObject
{
    //Q_OBJECT
public:
    explicit FDB(QObject *parent = 0);
    bool init();
    bool addGame(FGame game);
    bool removeGameById(int id);
    FGame* getGame(int id);
    QList<FGame> getGameList();
    int getGameCount();

    QString getTextPref(QString pref);
    QString getTextPref(QString pref, QString defaultValue);
    bool addTextPref(QString pref, QString value);
    bool updateTextPref(QString pref, QString value);
    bool deletePref(QString pref);

    int getIntPref(QString pref);
    int getIntPref(QString pref, int defaultValue);
    bool addIntPref(QString pref, int value);
    bool updateIntPref(QString pref, int value);

    bool updateWatchedFolders(QList<FWatchedFolder> data);
    QList<FWatchedFolder> getWatchedFoldersList();
    bool beginTransaction();
    bool endTransaction();
    bool rollbackTransaction();
    bool gameExists(FGame game);
    bool runQuery(QSqlQuery q);
    bool updateBoolPref(QString pref, bool value);
    bool addBoolPref(QString pref, bool value);
    bool getBoolPref(QString pref);
    bool getBoolPref(QString pref, bool defaultValue);
    int addLauncher(FLauncher launcher);
    FLauncher getLauncher(int id);
    QList<FLauncher> getLaunchers();


    bool updateGame(FGame *g);
    bool updateLauncher(FLauncher launcher);
    bool launcherExists(FLauncher launcher);

private:
    QSqlDatabase db;
    QSqlQuery query;

signals:

public slots:
};

#endif // FDB_H
