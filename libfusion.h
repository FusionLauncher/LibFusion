#ifndef LIBFUSION_H
#define LIBFUSION_H

#include "libfusion_global.h"

#ifndef QTCORE
#include <QtCore>
#endif

#include <QDir>

class LIBFUSIONSHARED_EXPORT LibFusion
{

public:
    LibFusion();
    static QDir getWorkingDir();
};

#endif // LIBFUSION_H
