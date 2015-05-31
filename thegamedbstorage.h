#ifndef THEGAMEDBSTORAGE
#define THEGAMEDBSTORAGE

#include "libfusion_global.h"

class LIBFUSIONSHARED_EXPORT TheGameDBStorage {
    public:
        QString baseImgUrl;
        QString gameID;
        QString gameName;

        //Images
        QString clearartURL;
        QString boxartURL;
        QString bannerURL;

        QList<QString> fanartURLs;


};


#endif // THEGAMEDBSTORAGE

