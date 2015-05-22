#ifndef FLIBRARY_H
#define FLIBRARY_H

#include <QObject>
#include "fdb.h"
#include "fgame.h"

class FLibrary : public QObject
{
    Q_OBJECT
public:
    explicit FLibrary(FDB db, QObject *parent = 0);
    void addGame(FGame game);
    void removeGameByIndex(int index);
    FGame getGameByIndex(int index);
    QList<FGame> getGameList();

private:
    QList<FGame> gameList;
    FDB db;

signals:

public slots:
};

#endif // FLIBRARY_H
