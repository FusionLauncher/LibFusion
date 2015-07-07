#ifndef FDB_H
#define FDB_H

#ifndef FGAME_H
#include <fgame.h>
#endif

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

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
    void resetDatabase();
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

    bool updateWatchedFolders(QList<QDir> data);
    QList<QDir> getWatchedFoldersList();
    bool beginTransaction();
    bool endTransaction();
    bool gameExists(FGame game);

    bool updateBoolPref(QString pref, bool value);
    bool addBoolPref(QString pref, bool value);
    bool getBoolPref(QString pref);
    bool getBoolPref(QString pref, bool defaultValue);
private:
    QSqlDatabase db;
    QSqlQuery query;

signals:

public slots:
};

#endif // FDB_H
