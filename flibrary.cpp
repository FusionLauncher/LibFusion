#include "flibrary.h"

FLibrary::FLibrary(FDB db, QObject *parent) : QObject(parent)
{

}

void FLibrary::addGame(FGame game)
{
    gameList.append(game);
}

void FLibrary::removeGameByIndex(int index)
{
    gameList.removeAt(index);
}

FGame FLibrary::getGameByIndex(int index)
{
    return gameList.at(index);
}

QList<FGame> FLibrary::getGameList()
{
    return gameList;
}
