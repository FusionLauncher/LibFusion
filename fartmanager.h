#ifndef FARTMANAGER_H
#define FARTMANAGER_H

#include <QXmlStreamReader>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include "fgame.h"
#include "libfusion_global.h"

class LIBFUSIONSHARED_EXPORT FArtManager: public QObject
{
Q_OBJECT
public:
    FArtManager();
    FArtManager(QObject *parent) : QObject(parent) { }

    void getGameData(FGame *g, QString platform);
private:
    QNetworkAccessManager* m_manager;

    void processData();
    QXmlStreamReader xml;
    QString baseImgUrl;

    QString clearartURL;
    QString boxartURL;
    QList<QString> fanartURLs;
    FGame *game;
private slots:
    void dataReady(QNetworkReply *pReply);


signals:
    void gotData(QString);
};

#endif // FARTMANAGER_H
