#ifndef THEGAMEDBSTORAGE
#define THEGAMEDBSTORAGE

#include "libfusion_global.h"
/**
 * @brief The TheGameDBStorage class Stores informations get from theGamesDB.net
 */
class LIBFUSIONSHARED_EXPORT TheGameDBStorage {
    public:
        QString baseImgUrl;
        QString gameID;
        QString gameName;

        QString Platform;

        //Images
        QString clearartURL;
        QString boxartURL;
        QString bannerURL;

        QList<QString> fanartURLs;


};


#endif // THEGAMEDBSTORAGE

