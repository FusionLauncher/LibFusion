#ifndef FUPDATER_H
#define FUPDATER_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QUrl>
#include <QString>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>

class FUpdater : public QObject
{
    Q_OBJECT
public:
    explicit FUpdater(QObject *parent = 0);
    QString getLibraryVersion();
    QString getClientVersion();
    bool isCurrentLibrary(QString thisLibrary);
    bool isCurrentClient(QString thisClient);
    //TODO: Add functions to download things.

private:

    QNetworkAccessManager *manager;

signals:

public slots:

};

#endif // FUPDATER_H
