#ifndef FUPDATER_H
#define FUPDATER_H

#include <QObject>

class FUpdater
{
public:
    FUpdater();
    void getLibraryVersion();
    void getClientVersion();
    bool isCurrentLibrary(QString thisLibrary);
    bool isCurrentClient(QString thisClient);
    //TODO: Add functions to download things.
};

#endif // FUPDATER_H
