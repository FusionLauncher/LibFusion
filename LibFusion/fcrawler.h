/**
 * @class FCrawler
 *
 * @brief FCrawler is responsible for searching games in defined Folders
 *
 * This class searches given Folders for Games.
 * It is also capable of determining the Type of an folder.
 * FCrawler can also search for all Steam-directories.
 *
 */


#ifndef FCRAWLER_H
#define FCRAWLER_H

#include <QDir>

#include "fdb.h"
#include "libfusion_global.h"



class LIBFUSIONSHARED_EXPORT FCrawler
{
public:
    FCrawler();
    ~FCrawler();

    /**
     * @fn scanAllFolders
     * @brief Will Scan  all Folders for new Games
     *
     * Start Scans in Folders based on their detected Type.
     * Folders are defined in the DB, Table: watchedFolders
     */
    void scanAllFolders();

    /**
     * @fn getType
     * @brief Tries to Detect the Type of given Directory
     *
     * If it is from Steam, Origin, GOG or something else
     *
     * @param folder Folder to Check
     * @return Type of Folder
     *
     * Tries to get the Folder-Type, based on its content. Actually, only Steam is implemented
     */
    FGameType getType(FWatchedFolder folder);
private:
    FDB db;

    /**
     * @fn getSteamGames
     * @brief Scans a Steam-Library for Games
     *
     *
     * @param folder Folder to Check
     *
     * Scans a SteamApps-Folder for Games. Adds all Missing games to the Lib.
     */
    void getSteamGames(FWatchedFolder folder);

    /**
     * @fn getOriginGames
     * @brief Scans the Origin Logfile for new Games and adds them
     *
     * Reads the Logfile, located in "C:\\ProgramData\\Origin\\Logs\\Client_Log.txt"; for new Games
     * Adds them, if they are not found, in the Library.
     *
     * Succeeds for like 80% of games.
     *
     */
    void getOriginGames();

    /**
     * @fn getGalaxyGames
     * @brief  Scans the given Folder for GOG-Galaxy Games
     *
     * This is done by Reading all the "goggame-*.info"-Files from the Subdirectories.
     * All found Games are inserted into the DB.
     *
     * @param folder Folder to Check
     *
     */
    void getGalaxyGames(FWatchedFolder folder);

    /**
     * @fn scanforLauncher
     * @brief Scans the given Folder for Files with specific File-Endings
     *
     * The File-Endings are determined by Reading the Launcher from the DB witht the folder as the Key.
     * All found Games are inserted into the DB.
     * @param folder Folder to Check
     *
     */
    void scanforLauncher(FWatchedFolder folder);

    /**
     * @fn updateSteamDirs
     * @brief Get all the Steam-Gamedirectories
     *
     * The directiries are read from the libraryfolders.vdf in the Steam-Dir.
     * The found directories are added to the WatchedFolder list.
     *
     */
    void updateSteamDirs();
};

#endif // FCRAWLER_H
