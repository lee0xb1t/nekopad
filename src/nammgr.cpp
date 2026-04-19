#include "nammgr.h"

NamManager::NamManager(QObject *parent) : QObject(parent)
{
}

NamManager& NamManager::instance() {
    static NamManager inst;
    return inst;
}

QNetworkAccessManager *NamManager::nam() {
    return &this->m_nam;
}

QString NamManager::cookies() {
    QUrl url = QUrl("https://bilibili.com");
    QList<QNetworkCookie> cookies = nam()->cookieJar()->cookiesForUrl(url);
    QStringList cookieList;
    for (auto &cookie : cookies) {
        cookieList << QString("%1=%2").arg(QString(cookie.name()), QString(cookie.value()));
    }
    return cookieList.join("; ");
}
