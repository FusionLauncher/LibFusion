#ifndef FFILESYNC_H
#define FFILESYNC_H

#include <QDir>

#include "fexception.h"


class FFileSync
{
public:
    bool sync(QDir local , int numBckps);

private:
  //  FDB *db;
};

#endif // FFILESYNC_H
