#include "videoinfo.h"
#include "biliwbi.h"
#include "nammgr.h"
#include "requesttag.h"

VideoInfo::VideoInfo(QObject *parent)
    : QObject{parent}
    , m_nam(NamManager::instance()->nam())
{
    connect(m_nam, &QNetworkAccessManager::finished, this, &VideoInfo::onNetworkFinished);
}

VideoInfo::~VideoInfo() {
    // if (m_nam) {
    //     delete m_nam;
    // }
}

void VideoInfo::onNetworkFinished(QNetworkReply *reply) {
    if (!reply) return;

    QVariant maker = reply->request().attribute(QNetworkRequest::User);
    if (!maker.isValid()) {
        return;
    }
    if (maker != QVariant::fromValue(RequestTag::videoInfo_FetchVideoInfo)) {
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        QVariant maker = reply->request().attribute(QNetworkRequest::User);
        if (maker.isValid()) {
            QByteArray jsonData = reply->readAll();

            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);

            if (error.error != QJsonParseError::NoError) {
                emit errorMsg(error.errorString());
                reply->deleteLater();
                return;
            }

            if (maker.isValid() && maker == QVariant::fromValue(RequestTag::videoInfo_FetchVideoInfo)) {
                if (doc.isObject()) {
                    QJsonObject jsonObj = doc.object();
                    QJsonValue codeVal = jsonObj["code"];
                    QJsonValue msgVal = jsonObj["message"];

                    if (codeVal.toInt(-1) != 0) {
                        setiIsValid(false);
                        emit errorMsg(msgVal.toString(""));
                    } else {
                        QJsonValue dataVal = jsonObj["data"];
                        if (dataVal.isObject()) {
                            QJsonObject dataObj = dataVal.toObject();
                            onVideoInfoReceived(dataObj);
                        } else {
                            emit errorMsg("url or qrcode_key is not found");
                        }
                    }
                }
            }
        }

    } else {
        qCritical() << reply->errorString();
        emit errorMsg(reply->errorString());
    }

    reply->deleteLater();
}

void VideoInfo::fetchVideoInfo() {
    BiliWbi bw(imgKey(), subKey());
    QMap<QString, QVariant> params;
    params["bvid"] = bvid();
    QString signedQuery = bw.signParams(params);
    QUrl url("https://api.bilibili.com/x/web-interface/wbi/view?" + signedQuery);
    // QUrl url("https://api.bilibili.com/x/web-interface/wbi/view/detail");
    // QUrlQuery urlQuery;
    // urlQuery.addQueryItem("bvid", bvid());
    // url.setQuery(urlQuery);
    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::User, QVariant::fromValue(RequestTag::videoInfo_FetchVideoInfo));
    m_nam->get(req);
}

void VideoInfo::onVideoInfoReceived(const QJsonObject& jsonData) {
    setiIsValid(true);
    setFirstCid(QString::number(jsonData["cid"].toInteger()));
    setPageCnt(jsonData["videos"].toInt());

    emit videoInfoLoaded();
}

bool VideoInfo::isValid() const
{
    return m_isValid;
}

void VideoInfo::setiIsValid(bool newIsValid)
{
    if (m_isValid == newIsValid)
        return;
    m_isValid = newIsValid;
    emit isValidChanged();
}

QString VideoInfo::bvid() const
{
    return m_bvid;
}

void VideoInfo::setBvid(const QString &newBvid)
{
    if (m_bvid == newBvid)
        return;
    m_bvid = newBvid;
    emit bvidChanged();
}

int VideoInfo::pageCnt() const
{
    return m_pageCnt;
}

void VideoInfo::setPageCnt(int newPageCnt)
{
    if (m_pageCnt == newPageCnt)
        return;
    m_pageCnt = newPageCnt;
    emit pageCntChanged();
}

QString VideoInfo::imgKey() const
{
    return m_imgKey;
}

void VideoInfo::setImgKey(const QString &newImgKey)
{
    if (m_imgKey == newImgKey)
        return;
    m_imgKey = newImgKey;
    emit imgKeyChanged();
}

QString VideoInfo::subKey() const
{
    return m_subKey;
}

void VideoInfo::setSubKey(const QString &newSubKey)
{
    if (m_subKey == newSubKey)
        return;
    m_subKey = newSubKey;
    emit subKeyChanged();
}

QString VideoInfo::firstCid() const
{
    return m_firstCid;
}

void VideoInfo::setFirstCid(const QString &newFirstCid)
{
    if (m_firstCid == newFirstCid)
        return;
    m_firstCid = newFirstCid;
    emit firstCidChanged();
}
