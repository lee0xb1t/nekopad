#ifndef NAMMGR_H
#define NAMMGR_H

#include <QtNetwork>

class NamManager
{
public:
    static NamManager *instance();

    QNetworkAccessManager *nam();

private:
    NamManager();

    QNetworkAccessManager m_nam;
};

#endif // NAMMGR_H
