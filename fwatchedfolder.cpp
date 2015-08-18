#include "fwatchedfolder.h"

FWatchedFolder::FWatchedFolder()
{

}

int FWatchedFolder::getLauncherID() const
{
    return LauncherID;
}

void FWatchedFolder::setLauncherID(int value)
{
    LauncherID = value;
}


int FWatchedFolder::getDbID() const
{
    return dbID;
}

void FWatchedFolder::setDbID(int value)
{
    dbID = value;
}

QDir FWatchedFolder::getDirectory() const
{
    return Directory;
}

void FWatchedFolder::setDirectory(const QDir &value)
{
    Directory = value;
}

