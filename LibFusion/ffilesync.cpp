#include <QDateTime>

#include "ffilesync.h"


enum SyncDirection {SrcToTrgt, TrgtToSrc};


bool FFileSync::sync(QDir local, int numBckps)
{
    QDateTime latestSync_local;
    QDateTime latestSync_synced;
    bool syncPossible = true;

    QDir synced; // = db->getSavegameDir();

    if (!local.exists())
        throw FException("Source does not Exist!\n" + local.absolutePath());

    if (!synced.exists())
        throw FException("Target does not Exist!\n" + synced.absolutePath());

    if (numBckps < 0)
        throw FException("Invalid Backup-Number!\nBackups: " + synced.absolutePath());

    QFile info_local(local.absoluteFilePath("f_sync.info"));
    QFile info_synced(synced.absoluteFilePath("f_sync.info"));

    if (!info_local.exists())
    {
        //qDebug() << "Never synced Source.";
        syncPossible = false;
    }
    else
    {
        latestSync_local = QDateTime::fromString(info_local.readAll());
    }


    if (!info_synced.exists())
    {
   //     qDebug() << "Never synced Target.";
        syncPossible = false;
    }
    else
    {
        latestSync_synced = QDateTime::fromString(info_synced.readAll());
    }

    if(syncPossible)
    {




    }

    return true;
}


