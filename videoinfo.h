#ifndef VIDEOINFO_H
#define VIDEOINFO_H

#include <QObject>
#include <QQmlEngine>
#include <QtNetwork>

class VideoInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString bvid READ bvid WRITE setBvid NOTIFY bvidChanged FINAL)
    Q_PROPERTY(QString imgKey READ imgKey WRITE setImgKey NOTIFY imgKeyChanged FINAL)
    Q_PROPERTY(QString subKey READ subKey WRITE setSubKey NOTIFY subKeyChanged FINAL)
    Q_PROPERTY(bool isValid READ isValid WRITE setiIsValid NOTIFY isValidChanged FINAL)
    Q_PROPERTY(QString firstCid READ firstCid WRITE setFirstCid NOTIFY firstCidChanged FINAL)
    Q_PROPERTY(int pageCnt READ pageCnt WRITE setPageCnt NOTIFY pageCntChanged FINAL)
    QML_ELEMENT
public:

    explicit VideoInfo(QObject *parent = nullptr);
    ~VideoInfo();

    Q_INVOKABLE void fetchVideoInfo();

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

public slots:
    void onNetworkFinished(QNetworkReply *reply);

signals:
    void errorMsg(const QString& msg);
    void videoInfoLoaded();

    void isValidChanged();
    void bvidChanged();

    void pageCntChanged();

    void firstCidChanged();

    void imgKeyChanged();

    void subKeyChanged();

private:
    void onVideoInfoReceived(const QJsonObject& jsonData);

    QNetworkAccessManager *m_nam;

    bool m_isValid;
    QString m_bvid;
    int m_pageCnt;
    QString m_imgKey;
    QString m_subKey;
    QString m_firstCid;
};

#endif // VIDEOINFO_H
