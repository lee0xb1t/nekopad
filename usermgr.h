#ifndef USERMGR_H
#define USERMGR_H

#include <QObject>
#include <QtNetwork>

class UserManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isLogin READ isLogin WRITE setIsLogin NOTIFY isLoginChanged FINAL)
    Q_PROPERTY(QUrl avatarUrl READ avatarUrl WRITE setAvatarUrl NOTIFY avatarUrlChanged FINAL)
    Q_PROPERTY(int currentLevel READ currentLevel WRITE setCurrentLevel NOTIFY currentLevelChanged FINAL)
    Q_PROPERTY(QString uid READ uid WRITE setUid NOTIFY uidChanged FINAL)
    Q_PROPERTY(QString uname READ uname WRITE setUname NOTIFY unameChanged FINAL)
    Q_PROPERTY(bool vipStatus READ vipStatus WRITE setVipStatus NOTIFY vipStatusChanged FINAL)
    Q_PROPERTY(int coins READ coins WRITE setCoins NOTIFY coinsChanged FINAL)
public:
    enum RequestType {
        FetchUserData,
    };
    Q_ENUM(RequestType)

    explicit UserManager(QObject *parent = nullptr);

    Q_INVOKABLE void fetchUserData();

    bool isLogin() const;
    void setIsLogin(bool newIsLogin);

    QUrl avatarUrl() const;
    void setAvatarUrl(const QUrl &newAvatarUrl);

    int currentLevel() const;
    void setCurrentLevel(int newCurrentLevel);

    QString uid() const;
    void setUid(const QString &newUid);

    QString uname() const;
    void setUname(const QString &newUname);

    bool vipStatus() const;
    void setVipStatus(bool newVipStatus);

    int coins() const;
    void setCoins(int newCoins);

public slots:
    void onNetworkFinished(QNetworkReply *reply);

signals:
    void userDataLoaded();
    void userError(const QString& error);

    void isLoginChanged();

    void avatarUrlChanged();

    void currentLevelChanged();

    void uidChanged();

    void unameChanged();

    void vipStatusChanged();

    void coinsChanged();

private:
    void onUserDataReceived(const QJsonObject& jsonData);

    QNetworkAccessManager *m_nam;
    bool m_isLogin;
    QUrl m_avatarUrl;
    int m_currentLevel;
    QString m_uid;
    QString m_uname;
    bool m_vipStatus;
    int m_coins;
};

#endif // USERMGR_H
