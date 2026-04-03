#ifndef REQUESTTAG_H
#define REQUESTTAG_H

#include <QtNetwork>
#include <QVariant>

enum class RequestTag : int {
    Unknown = 0,

    login_GetCaptcha,
    login_Poll,

    user_FetchUserData,
    user_ReloadWbiKey,

    fav_FetchFavList,

    videoList_FetchVideoList,

    videoInfo_FetchVideoInfo,
    videoInfo_FetchPlayUrl,
};

Q_DECLARE_METATYPE(RequestTag)

#endif // REQUESTTAG_H
