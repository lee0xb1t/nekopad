#include "videoinfo.h"
#include "biliwbi.h"
#include "nammgr.h"
#include "requesttag.h"

VideoInfo::VideoInfo(QObject *parent)
    : QObject{parent}
    , m_nam(NamManager::instance().nam())
    , m_playUrlInfo{new PlayUrlInfo(this)}
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
    if (maker != QVariant::fromValue(RequestTag::videoInfo_FetchVideoInfo)
        && maker != QVariant::fromValue(RequestTag::videoInfo_FetchPlayUrl)) {
        return;
    }

    QVariant maker2 = reply->request().attribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 1));
    if (!maker2.isValid()) {
        return;
    }

    if (maker2 != QVariant::fromValue(bvid())) {
        return;
    }

    QVariant maker3 = reply->request().attribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 2));
    if (!maker3.isValid()) {
        return;
    }

    if (maker3 != QVariant::fromValue(index())) {
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
            } else if (maker.isValid() && maker == QVariant::fromValue(RequestTag::videoInfo_FetchPlayUrl)) {
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
                            onPlayUrlReceived(dataObj);
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
    req.setAttribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 1), bvid());
    req.setAttribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 2), index());
    m_nam->get(req);
}

void VideoInfo::fetchPlayUrl() {
    BiliWbi bw(imgKey(), subKey());
    QMap<QString, QVariant> params;
    params["bvid"] = bvid();
    params["cid"] = firstCid();
    params["platform"] = "html5";
    params["high_quality"] = 1;
    params["qn"] = 80;  // 1080
    params["fnval"] = 16; // dash format
    params["fnver"] = 0;
    QString signedQuery = bw.signParams(params);
    QUrl url("https://api.bilibili.com/x/player/wbi/playurl?" + signedQuery);
    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::User, QVariant::fromValue(RequestTag::videoInfo_FetchPlayUrl));
    req.setAttribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 1), bvid());
    req.setAttribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 2), index());
    m_nam->get(req);
}

void VideoInfo::onVideoInfoReceived(const QJsonObject& jsonData) {
    setiIsValid(true);

    setCover(jsonData["pic"].toString());
    setTitle(jsonData["title"].toString());
    QJsonObject ownerObj = jsonData["owner"].toObject();
    setOwnerMid(QString::number(ownerObj["mid"].toInteger()));
    setOwnerName(ownerObj["name"].toString());

    setFirstCid(QString::number(jsonData["cid"].toInteger()));
    setPageCnt(jsonData["videos"].toInt());

    emit videoInfoLoaded();
}

void VideoInfo::onPlayUrlReceived(const QJsonObject& jsonData) {
    m_playUrlInfo->setQuality(jsonData["quality"].toInt());
    m_playUrlInfo->setFormat(jsonData["format"].toString());
    m_playUrlInfo->setTimelength(jsonData["timelength"].toInt()*1000);
    m_playUrlInfo->setAcceptFormat(jsonData["accept_format"].toString());
    m_playUrlInfo->setAcceptQuality(jsonData["accept_quality"].toString());

    QJsonValue dashVal = jsonData["dash"];
    if (dashVal.isNull()) {
        emit errorMsg("no-dash-info");
        return;
    }

    QJsonObject dashObj = dashVal.toObject();

    QJsonValue audioVal = dashObj["audio"];
    if (audioVal.isNull()) {
        emit errorMsg("no-audio-info");
        return;
    }
    if (!audioVal.isArray()) {
        emit errorMsg("audio-info-is-not-array");
        return;
    }

    int tempAudioCode = 0;
    QString tempAudioBaseUrl = "";
    QJsonArray audioArr = audioVal.toArray();
    for (int i = 0; i < audioArr.size(); i++) {
        QJsonValue audioItem = audioArr[i];
        int audioCode = audioItem["id"].toInt();
        if (audioCode > tempAudioCode) {
            tempAudioCode = audioCode;
            tempAudioBaseUrl = audioItem["baseUrl"].toString();
        }
    }

    m_playUrlInfo->setAudioCode(tempAudioCode);
    m_playUrlInfo->setAudioUrl(tempAudioBaseUrl);

    emit playUrlInfoChanged();
    emit playUrlLoaded();
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

PlayUrlInfo *VideoInfo::playUrlInfo() const
{
    return m_playUrlInfo;
}

QString VideoInfo::cover() const
{
    return m_cover;
}

void VideoInfo::setCover(const QString &newCover)
{
    if (m_cover == newCover)
        return;
    m_cover = newCover;
    emit coverChanged();
}

QString VideoInfo::title() const
{
    return m_title;
}

void VideoInfo::setTitle(const QString &newTitle)
{
    if (m_title == newTitle)
        return;
    m_title = newTitle;
    emit titleChanged();
}

QString VideoInfo::ownerMid() const
{
    return m_ownerMid;
}

void VideoInfo::setOwnerMid(const QString &newOwnerMid)
{
    if (m_ownerMid == newOwnerMid)
        return;
    m_ownerMid = newOwnerMid;
    emit ownerMidChanged();
}

QString VideoInfo::ownerName() const
{
    return m_ownerName;
}

void VideoInfo::setOwnerName(const QString &newOwnerName)
{
    if (m_ownerName == newOwnerName)
        return;
    m_ownerName = newOwnerName;
    emit ownerNameChanged();
}

int VideoInfo::index() const
{
    return m_index;
}

void VideoInfo::setIndex(int newIndex)
{
    if (m_index == newIndex)
        return;
    m_index = newIndex;
    emit indexChanged();
}
