#ifndef LIBFUSION_H
#define LIBFUSION_H

#include "libfusion_global.h"

#include <QDir>

class LIBFUSIONSHARED_EXPORT LibFusion
{

public:
    LibFusion();
    static QDir getWorkingDir();
};

#endif // LIBFUSION_H
