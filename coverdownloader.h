#ifndef COVERDOWNLOADER_H
#define COVERDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class CoverDownloader : public QObject
{
    Q_OBJECT
public:
    explicit CoverDownloader(QUrl imageUrl, QObject *parent = 0);
    virtual ~CoverDownloader();
    QByteArray downloadedData() const;

signals:
    void downloaded();

private slots:
    void fileDownloaded(QNetworkReply* pReply);
private:
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;
};

#endif
