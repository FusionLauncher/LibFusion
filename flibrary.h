#ifndef FGAME_H
#include "fgame.h"
#endif

#include "fdb.h"

#ifndef FLIBRARY_H
#define FLIBRARY_H

class FLibrary
{
public:
    FLibrary(FDB db);
    void addGame(FGame game);
    void removeGame(int index);
    FGame gameByIndex(int index);
    FGame gameByName(QString name);
    QList<FGame> getGames();
private:
    QList<FGame> libraryGames;
    FDB db;
};

#endif // FLIBRARY_H
