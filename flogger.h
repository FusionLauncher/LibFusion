#ifndef FLOGGER_H
#define FLOGGER_H

#include <QString>

#include "libfusion_global.h"

class LIBFUSIONSHARED_EXPORT FLogger
{
public:
    /**
     * @brief append Appends message to the log in fileName
     * @param message String that is getting appended
     * @param fileName File where the log is saved
     */
    static void append(QString fileName, QString message);
    /**
     * @brief clear Clears fileName
     * @param fileName File getting cleared
     */
     static void clear(QString fileName);
};

#endif // FLOGGER_H
