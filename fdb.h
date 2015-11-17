#ifndef FDB_H
#define FDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "flauncher.h"
#include "fgame.h"
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
    QList<FGame *> getGameList();
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
    bool watchedFolderExists(FWatchedFolder *wf);
    bool addWatchedFolder(FWatchedFolder wf);


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
    bool updateLaunchers(QList<FLauncher> launchers);
    FLauncher getLauncher(int id);
    QList<FLauncher> getLaunchers();
    bool updateLauncher(FLauncher launcher);
    bool launcherExists(FLauncher launcher);


    bool updateGame(FGame *g);
    bool updateLastLaunched(FGame *g);
    QList<FGame *> getLatestLaunchedGames(int limit);

    QDir getSavegameDir();

private:
    QSqlDatabase db;
    QSqlQuery query;

    FGame *createGameFromQuery(QSqlQuery query);
    bool tryExecute(QSqlQuery *q);
    bool createDatabase();
    bool connect(QSqlDatabase *database, QFileInfo dbFile);
signals:

public slots:
};

#endif // FDB_H
