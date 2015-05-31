#include "ffiledownloader.h"

#include <QDir>
#include <QFileInfo>


FFileDownloader::FFileDownloader(QUrl fileUrl, QObject *parent) :
 QObject(parent)
{
 connect(
  &m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
  this, SLOT (fileDownloaded(QNetworkReply*))
  );

 QNetworkRequest request(fileUrl);
 m_WebCtrl.get(request);
}


FFileDownloader::FFileDownloader(QUrl fileUrl, QString t, QObject *parent) :
 QObject(parent)
{
 connect(
  &m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
  this, SLOT (fileDownloaded(QNetworkReply*))
  );
 target = t;
 QNetworkRequest request(fileUrl);
 m_WebCtrl.get(request);
}

FFileDownloader::~FFileDownloader() { }

void FFileDownloader::fileDownloaded(QNetworkReply* pReply) {
 m_DownloadedData = pReply->readAll();
 //emit a signal
 pReply->deleteLater();

 if(target!= NULL)
 {
     QFile file(target);
     QFileInfo fileInfo(file);

     if(!fileInfo.absoluteDir().exists()) {
         fileInfo.absoluteDir().mkpath(fileInfo.absoluteDir().absolutePath());
     }

     file.open(QIODevice::WriteOnly);
     file.write(m_DownloadedData);
     file.close();

 }
 emit downloaded();
}

QByteArray FFileDownloader::downloadedData() const {
 return m_DownloadedData;
}
