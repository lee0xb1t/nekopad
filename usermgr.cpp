#include "usermgr.h"
#include "cookiemgr.h"

UserManager::UserManager(QObject *parent)
    : QObject{parent}
    , m_nam(new QNetworkAccessManager(this))
{
    m_nam->setCookieJar(CookieManager::instance()->cookieJar());

    connect(m_nam, &QNetworkAccessManager::finished, this, &UserManager::onNetworkFinished);
}

void UserManager::fetchUserData() {
    QNetworkRequest request(QUrl("https://api.bilibili.com/x/web-interface/nav"));
    request.setAttribute(QNetworkRequest::User, UserManager::FetchUserData);
    m_nam->get(request);
}

void UserManager::onNetworkFinished(QNetworkReply *reply) {
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QVariant maker = reply->request().attribute(QNetworkRequest::User);
        if (maker.isValid()) {
            QByteArray jsonData = reply->readAll();

            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);

            if (error.error != QJsonParseError::NoError) {
                emit userError(error.errorString());
                reply->deleteLater();
                return;
            }

            if (maker.isValid() && maker == UserManager::FetchUserData) {
                if (doc.isObject()) {
                    QJsonObject jsonObj = doc.object();
                    QJsonValue codeVal = jsonObj["code"];
                    QJsonValue msgVal = jsonObj["message"];

                    if (codeVal.toInt(-1) != 0) {
                        emit userError(msgVal.toString(""));
                    } else {
                        QJsonValue dataVal = jsonObj["data"];
                        if (dataVal.isObject()) {
                            QJsonObject dataObj = dataVal.toObject();
                            onUserDataReceived(dataObj);
                        } else {
                            emit userError("url or qrcode_key is not found");
                        }
                    }
                }
            }
        }

    } else {
        emit userError(reply->errorString());
    }

    reply->deleteLater();
}

void UserManager::onUserDataReceived(const QJsonObject& jsonData) {
    setIsLogin(jsonData["isLogin"].toBool());
    setAvatarUrl(jsonData["face"].toString());

    QJsonValue levelInfo = jsonData["level_info"];
    if (levelInfo.isObject()) {
        QJsonObject levelInfoObj = levelInfo.toObject();
        setCurrentLevel(levelInfoObj["current_level"].toInt());
    }

    setUid(QString::number(jsonData["mid"].toInteger()));
    setUname(jsonData["uname"].toString());
    setVipStatus(jsonData["vipStatus"].toBool());
    setCoins(jsonData["money"].toInt());

    emit userDataLoaded();
}

bool UserManager::isLogin() const
{
    return m_isLogin;
}

void UserManager::setIsLogin(bool newIsLogin)
{
    if (m_isLogin == newIsLogin)
        return;
    m_isLogin = newIsLogin;
    emit isLoginChanged();
}

QUrl UserManager::avatarUrl() const
{
    return m_avatarUrl;
}

void UserManager::setAvatarUrl(const QUrl &newAvatarUrl)
{
    if (m_avatarUrl == newAvatarUrl)
        return;
    m_avatarUrl = newAvatarUrl;
    emit avatarUrlChanged();
}

int UserManager::currentLevel() const
{
    return m_currentLevel;
}

void UserManager::setCurrentLevel(int newCurrentLevel)
{
    if (m_currentLevel == newCurrentLevel)
        return;
    m_currentLevel = newCurrentLevel;
    emit currentLevelChanged();
}

QString UserManager::uid() const
{
    return m_uid;
}

void UserManager::setUid(const QString &newUid)
{
    if (m_uid == newUid)
        return;
    m_uid = newUid;
    emit uidChanged();
}

QString UserManager::uname() const
{
    return m_uname;
}

void UserManager::setUname(const QString &newUname)
{
    if (m_uname == newUname)
        return;
    m_uname = newUname;
    emit unameChanged();
}

bool UserManager::vipStatus() const
{
    return m_vipStatus;
}

void UserManager::setVipStatus(bool newVipStatus)
{
    if (m_vipStatus == newVipStatus)
        return;
    m_vipStatus = newVipStatus;
    emit vipStatusChanged();
}

int UserManager::coins() const
{
    return m_coins;
}

void UserManager::setCoins(int newCoins)
{
    if (m_coins == newCoins)
        return;
    m_coins = newCoins;
    emit coinsChanged();
}
