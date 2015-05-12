#ifndef FDB_H
#define FDB_H

#ifndef FGAME_H
#include <fgame.h>
#endif

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

class FDB : public QObject
{
    //Q_OBJECT
public:
    explicit FDB(QObject *parent = 0);
    bool init();
    bool addGame(FGame game);
    FGame* getGame(int id);
    QList<FGame> getGameList();
    void resetDatabase();
    int getGameCount();


private:
    QSqlDatabase db;
    QSqlQuery query;

signals:

public slots:
};

#endif // FDB_H
