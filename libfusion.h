#ifndef LIBFUSION_H
#define LIBFUSION_H

#include "libfusion_global.h"

class LIBFUSIONSHARED_EXPORT LibFusion
{

public:
    LibFusion();
    void getLibraryVersion();
    void getClientVersion();
    bool isCurrentLibrary(QString thisLibrary);
    bool isCurrentClient(QString thisClient);
    //TODO: Add function to download things.
};

#endif // LIBFUSION_H
