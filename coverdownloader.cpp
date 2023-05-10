#include "coverdownloader.h"

CoverDownloader::CoverDownloader(QUrl imageUrl, QObject *parent) :
    QObject(parent)
{
    connect(
        &m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
        this, SLOT (fileDownloaded(QNetworkReply*))
        );

    QNetworkRequest request(imageUrl);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);
    m_WebCtrl.get(request);
}

CoverDownloader::~CoverDownloader() { }

void CoverDownloader::fileDownloaded(QNetworkReply* pReply) {
    m_DownloadedData = pReply->readAll();
    //emit a signal
    pReply->deleteLater();
    emit downloaded();
}

QByteArray CoverDownloader::downloadedData() const {
    return m_DownloadedData;
}
