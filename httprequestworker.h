#ifndef HTTPREQUESTWORKER
#define HTTPREQUESTWORKER

#include <QObject>
#include <QString>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>

enum HttpRequestVarLayout {NOT_SET, ADDRESS, URL_ENCODED, MULTIPART};

class HttpRequestInputFileElement {
 public:
    QString variable_name;
    QString local_filename;
    QString request_filename;
    QString mime_type;
};

class HttpRequestInput {
public:
    QString url_str;
    QString http_method;
    HttpRequestVarLayout var_layout;
    QMap<QString, QString> vars;
    QList<HttpRequestInputFileElement> files;

    HttpRequestInput();
    HttpRequestInput(QString v_url_str, QString v_http_method);
    void initialize();
    void add_var(QString key, QString value);
    void add_file(QString variable_name, QString local_filename, QString request_filename, Qstring mime_type);
};

class HttpRequestWorker : public QObject {
    Q_OBJECT

public:
    QByteArray response;
    QNetworkReply::NetworkError error_type;
    QString error_str;

    explicit HttpRequestWorker(QObject *parent = 0); //pointer uugh

    QString http_attribute_encode(QString attribute_name, QString input);
    void execute(HttpRequestInput *input); //pointer uugh

signals:
    void on_execution_finished(HttpRequestWorker *worker);

private:
    QNetworkAccessManager *manager;

private slots:
    void on_manager_finished(QNetworkReply *reply);
};

#endif // HTTPREQUESTWORKER
