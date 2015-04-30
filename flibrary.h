#ifndef FGAME_H
#include "fgame.h"
#endif

#ifndef FLIBRARY_H
#define FLIBRARY_H

class FLibrary
{
public:
    FLibrary();
    void addGame(FGame game);
    void removeGame(int index);
    FGame gameByIndex(int index);
    FGame gameByName(QString name);
    QList<FGame> getGames();
private:
    QList<FGame> libraryGames;
};

#endif // FLIBRARY_H
