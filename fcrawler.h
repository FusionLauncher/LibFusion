#ifndef FCRAWLER_H
#define FCRAWLER_H

#include <fdb.h>
#include <QDir>


#include "libfusion_global.h"

class LIBFUSIONSHARED_EXPORT FCrawler
{
public:
    FCrawler();
    ~FCrawler();
    void scanAllFolders();
private:
    FDB db;
    FGameType getType(QDir folder);
    void getSteamGames(QDir folder);
    void getOriginGames();
};

#endif // FCRAWLER_H
