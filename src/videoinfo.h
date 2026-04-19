#ifndef VIDEOINFO_H
#define VIDEOINFO_H

#include <QObject>
#include <QQmlEngine>
#include <QtNetwork>
#include "models/playurlinfo.h"

class VideoInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged FINAL)
    Q_PROPERTY(QString bvid READ bvid WRITE setBvid NOTIFY bvidChanged FINAL)
    Q_PROPERTY(QString imgKey READ imgKey WRITE setImgKey NOTIFY imgKeyChanged FINAL)
    Q_PROPERTY(QString subKey READ subKey WRITE setSubKey NOTIFY subKeyChanged FINAL)
    Q_PROPERTY(bool isValid READ isValid WRITE setiIsValid NOTIFY isValidChanged FINAL)

    Q_PROPERTY(QString cover READ cover WRITE setCover NOTIFY coverChanged FINAL)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(QString ownerMid READ ownerMid WRITE setOwnerMid NOTIFY ownerMidChanged FINAL)
    Q_PROPERTY(QString ownerName READ ownerName WRITE setOwnerName NOTIFY ownerNameChanged FINAL)

    Q_PROPERTY(QString firstCid READ firstCid WRITE setFirstCid NOTIFY firstCidChanged FINAL)
    Q_PROPERTY(int pageCnt READ pageCnt WRITE setPageCnt NOTIFY pageCntChanged FINAL)
    Q_PROPERTY(PlayUrlInfo* playUrlInfo READ playUrlInfo NOTIFY playUrlInfoChanged FINAL)
    QML_ELEMENT
public:

    explicit VideoInfo(QObject *parent = nullptr);
    ~VideoInfo();

    Q_INVOKABLE void fetchVideoInfo();
    Q_INVOKABLE void fetchPlayUrl();

    bool isValid() const;
    void setiIsValid(bool newIsValid);

    QString bvid() const;
    void setBvid(const QString &newBvid);

    int pageCnt() const;
    void setPageCnt(int newPageCnt);

    QString imgKey() const;
    void setImgKey(const QString &newImgKey);

    QString subKey() const;
    void setSubKey(const QString &newSubKey);

    QString firstCid() const;
    void setFirstCid(const QString &newFirstCid);

    PlayUrlInfo *playUrlInfo() const;

    QString cover() const;
    void setCover(const QString &newCover);

    QString title() const;
    void setTitle(const QString &newTitle);

    QString ownerMid() const;
    void setOwnerMid(const QString &newOwnerMid);

    QString ownerName() const;
    void setOwnerName(const QString &newOwnerName);

    int index() const;
    void setIndex(int newIndex);

public slots:
    void onNetworkFinished(QNetworkReply *reply);

signals:
    void errorMsg(const QString& msg);
    void videoInfoLoaded();
    void playUrlLoaded();

    void isValidChanged();
    void bvidChanged();

    void pageCntChanged();

    void firstCidChanged();

    void imgKeyChanged();

    void subKeyChanged();

    void playUrlInfoChanged();

    void coverChanged();

    void titleChanged();

    void ownerMidChanged();

    void ownerNameChanged();

    void indexChanged();

private:
    void onVideoInfoReceived(const QJsonObject& jsonData);
    void onPlayUrlReceived(const QJsonObject& jsonData);

    QNetworkAccessManager *m_nam;

    bool m_isValid = false;
    QString m_bvid;
    int m_pageCnt = 0;
    QString m_imgKey;
    QString m_subKey;
    QString m_firstCid;
    PlayUrlInfo *m_playUrlInfo = nullptr;
    QString m_cover;
    QString m_title;
    QString m_ownerMid;
    QString m_ownerName;
    int m_index;
};

#endif // VIDEOINFO_H
