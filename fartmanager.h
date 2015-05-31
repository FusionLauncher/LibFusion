#ifndef FARTMANAGER_H
#define FARTMANAGER_H

#include <QXmlStreamReader>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include "fgame.h"
#include "libfusion_global.h"
#include "thegamedbstorage.h"

class LIBFUSIONSHARED_EXPORT FArtManager: public QObject
{
Q_OBJECT
public:
    FArtManager();
    FArtManager(QObject *parent) : QObject(parent) { }

    void getGameData(FGame *g, QString platform);
private:
    QNetworkAccessManager* m_manager;

    void processGame();
    QXmlStreamReader *xml;
    QList<TheGameDBStorage*> Games;
    FGame *game;
private slots:
    void dataReady(QNetworkReply *pReply);


    void on_downloadFinished();
signals:
    void gotData(QString);
    void startedDownload();
    void finishedDownload();
};

#endif // FARTMANAGER_H