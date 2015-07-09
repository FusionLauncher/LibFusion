#ifndef FFILEDOWNLOADER_H
#define FFILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>


// THIS IS FROM https://wiki.qt.io/Download_Data_from_URL

class FFileDownloader : public QObject
{
 Q_OBJECT
 public:
  explicit FFileDownloader(QUrl fileUrl, QObject *parent = 0);
  FFileDownloader(QUrl fileUrl, QString t, QObject *parent = 0);
  virtual ~FFileDownloader();
  QByteArray downloadedData() const;

signals:
  void downloaded();
  void srcDownloaded(QString);

private slots:
  void fileDownloaded(QNetworkReply* pReply);

private:
  QString target;
  QString src;
  QNetworkAccessManager m_WebCtrl;
  QByteArray m_DownloadedData;
};

#endif // FFILEDOWNLOADER_H
