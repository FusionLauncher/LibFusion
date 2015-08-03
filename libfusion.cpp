#include "libfusion.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QStandardPaths>
#endif

LibFusion::LibFusion()
{

}

QDir LibFusion::getWorkingDir()
{
    #if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        return QDir(QCoreApplication::applicationDirPath());
    #elif (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
        return QDir(QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, QString(), QStandardPaths::LocateDirectory));
    #else
        return QDir(QStandardPaths::locate(QStandardPaths::DataLocation, QString(), QStandardPaths::LocateDirectory));
    #endif
}

