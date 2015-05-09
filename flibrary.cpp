#include "flibrary.h"

FLibrary::FLibrary(FDB db)
{
}

void FLibrary::addGame(FGame game)
{
    this->libraryGames.insert(0, game);
}

void FLibrary::removeGame(int index)
{
    this->libraryGames.removeAt(index);
}

FGame FLibrary::gameByIndex(int index)
{
    return FGame();
}

FGame FLibrary::gameByName(QString name)
{
    return FGame();
}

QList<FGame> FLibrary::getGames()
{
    return this->libraryGames;
}

