#ifndef FLOGGER_H
#define FLOGGER_H

#include <QString>

#include "libfusion_global.h"

class LIBFUSIONSHARED_EXPORT FLogger
{
public:
    static void append(QString fileName, QString message);
    static void clear(QString fileName);
};

#endif // FLOGGER_H
