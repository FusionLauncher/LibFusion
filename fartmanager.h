#ifndef FARTMANAGER_H
#define FARTMANAGER_H

#include <QXmlStreamReader>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include "fgame.h"
#include "libfusion_global.h"
#include "thegamedbstorage.h"

class LIBFUSIONSHARED_EXPORT FArtManager: public QObject
{
Q_OBJECT
public:
    FArtManager();
    FArtManager(FGame *g);
    FArtManager(QObject *parent) : QObject(parent) { }

    /**
     * @brief getGameData Search for Game by Game-name
     * @param g Game we are searching
     * @param platform to get better results
     */
    void getGameData(FGame *g, QString platform);

    /**
     * @brief getGameData Search data by theGameDB-ID
     * @param g Game we are searching, used to determine Path for Artwork
     * @param gameDBEntry Used to get Data by stored Game-ID
     */
    void getGameData(FGame *g, TheGameDBStorage* gameDBEntry);



    void importArtwork(QFileInfo fi, QString destName);
private:
    QNetworkAccessManager* m_manager;

    /**
     * @brief processGame Reads the XML-Response and Pasre it into a TheGameDBStorage-Object.
     */
    void processGame();

    QXmlStreamReader *xml; /**< Reads the Response from theGamesDB.net */

    QList<TheGameDBStorage*> Games; /**< List of Mathing Games from theGamesdb.net */

    FGame *game; /**< Curret Game wa are searching */

    bool triedSearch; /**< Flag if search WITHOUT exactname happend. To perevnt loop */

private slots:
    void dataReady(QNetworkReply *pReply); /**< QNetworkAccessManager has finished Downloading */
    void downloadFinished(QString);
signals:
    /**
     * @brief startedDownload Emitted when a Download is started.
     *
     * use this to count started/finished Downloads
     */
    void startedDownload();

    /**
     * @brief finishedDownload Emitted when a Download is finished.
     *
     * use this to count started/finished Downloads
     */
    void finishedDownload();

    /**
     * @brief foundMultipleGames Emitted, when Muliple Games are found.
     *
     * Use the List to select correct game, and call
    void getGameData(FGame *g, TheGameDBStorage* gameDBEntry);
     */
    void foundMultipleGames(QList<TheGameDBStorage*>);
};

#endif // FARTMANAGER_H
