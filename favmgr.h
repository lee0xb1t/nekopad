#ifndef FAVMGR_H
#define FAVMGR_H

#include <QObject>
#include <QtNetwork>
#include "models/favitemmodel.h"

class FavManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<FavItemModel*> items READ items NOTIFY itemsChanged)
    Q_PROPERTY(qsizetype count READ count NOTIFY countChanged)
public:
    enum RequestType {
        FetchFavList
    };
    Q_ENUM(RequestType)

    explicit FavManager(QObject *parent = nullptr);

    Q_INVOKABLE void fetchFavList(QString uid);

    QList<FavItemModel *> items() const;

    qsizetype count() const;

public slots:
    void onNetworkFinished(QNetworkReply *reply);

signals:
    void favListLoaded();
    void favMgrError(const QString& errorMsg);

    void itemsChanged();

    void countChanged();

private:
    void onFavListReceived(const QJsonObject& jsonObj);

    void addItems(FavItemModel *itemModel);

    QNetworkAccessManager *m_nam;
    QList<FavItemModel *> m_items;
    qsizetype m_count;
};

#endif // FAVMGR_H
