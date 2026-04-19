#ifndef NAMMGR_H
#define NAMMGR_H

#include <QtNetwork>
#include <QObject>

class NamManager : public QObject
{
    Q_OBJECT
public:
    static NamManager& instance();

    QNetworkAccessManager *nam();

    Q_INVOKABLE QString cookies();

private:
    explicit NamManager(QObject *parent = nullptr);

    QNetworkAccessManager m_nam;
};

#endif // NAMMGR_H
