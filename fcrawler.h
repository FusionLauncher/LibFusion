#ifndef FCRAWLER_H
#define FCRAWLER_H

#include "fdb.h"
#include <QDir>


#include "libfusion_global.h"

class LIBFUSIONSHARED_EXPORT FCrawler
{
public:
    FCrawler();
    ~FCrawler();

    /**
     * @brief scanAllFolders for new Games
     *
     * Start Scans in Folders based on their detected Type.
     * Folders are defined in the DB, Table: watchedFolders
     */
    void scanAllFolders();
private:
    FDB db;

    /**
     * @brief getType
     * @param folder Folder to Check
     * @return Type of Folder
     *
     * Tries to get the Folder-Type, based on its content. Actually, only Steam is implemented
     */
    FGameType getType(FWatchedFolder folder);

    /**
     * @brief getSteamGames Scans a Steam-Library for Games
     * @param folder Folder to Check
     *
     * Scans a SteamApps-Folder for Games. Adds all Missing games to the Lib.
     */
    void getSteamGames(FWatchedFolder folder);

    /**
     * @brief getOriginGames  Scans the Origin Logfile for new Games and adds them
     *
     * Reads the Logfile, located in "C:\\ProgramData\\Origin\\Logs\\Client_Log.txt"; for new Games
     * Adds them, if they are not found in the Library.
     *
     * Succeeds for 80% of games.
     *
     */
    void getOriginGames();
    void getGalaxyGames(FWatchedFolder folder);
    void scanforLauncher(FWatchedFolder folder);
    void updateSteamDirs();
};

#endif // FCRAWLER_H
