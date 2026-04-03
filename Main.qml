import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QCC
import Qt5Compat.GraphicalEffects
import Qt.labs.platform
import nekopad
import netutils 1.0
import "components"

Window {
    id: rootWinId
    width: 800
    height: 580
    visible: true
    color: "transparent"

    minimumWidth: 800
    maximumWidth: 800
    minimumHeight: 580
    maximumHeight: 580

    flags: Qt.FramelessWindowHint | Qt.Window & ~Qt.WindowStaysOnTopHint

    property string audioBvid
    property string audioSource
    property int audioDuration
    property int audioDurationReal : 0
    property string audioCookies

    property int audioVolumn: 50

    MouseArea {
        anchors.fill: parent
        onPressed: {
            rootWinId.startSystemMove()
        }
    }

    Rectangle {
        id: rootRectId

        width: parent.width
        height: parent.height
        color: "#fafafa"
        border.width: 1
        border.color: "#e4e4e7"
        radius: 4

        Rectangle {
            width: parent.width
            height: parent.height
            color: "#fafafa"
            border.width: 1
            border.color: "#e4e4e7"

            ColumnLayout {
                width: parent.width-2
                height: parent.height-2
                anchors.centerIn: parent

                spacing: 0

                RowLayout {
                    spacing: 0

                    Rectangle {
                        width: 200
                        height: 518
                        color: "transparent"

                        Rectangle {
                            width: parent.width-1
                            height: parent.height
                            color: "transparent"

                            ColumnLayout {
                                anchors.fill: parent
                                spacing: 0

                                Logo {
                                }

                                Fav {
                                    favList: favMgr.items

                                    onFavClicked: function(favid) {
                                        console.log("onFavClicked:", favid)
                                        videoListModelId.favId = favid
                                    }
                                }

                                User {
                                    id: user
                                    // uid: userMgr.uid
                                    // name: userMgr.uname
                                    // avatar: userMgr.avatarUrl
                                    // isLogin: userMgr.isLogin

                                    onUserClicked: function(isLogin, uid) {
                                        if (!isLogin) {
                                            loginMgr.startLogin()
                                            dialog.open()
                                        } else {

                                        }
                                    }
                                }
                            }
                        }
                    }

                    Rectangle {
                        id: videoRectId
                        width: 598
                        height: 518
                        color: "#fff"
                        radius: 8

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 0

                            Head {
                                width: parent.width
                                height: 50

                                onMinimized: {
                                    rootWinId.visibility = Window.Minimized
                                }

                                onClosed: {
                                    rootWinId.close()
                                }
                            }

                            VList {
                                id: videoListId

                                width: parent.width
                                height: 470
                                videoListModel: videoListModelId

                                imgKey: userMgr.imgKey
                                subKey: userMgr.subKey

                                onChoiceVideo: function(bvid, url, duration) {
                                    rootWinId.audioBvid = bvid
                                    rootWinId.audioSource = url
                                    rootWinId.audioDuration = duration
                                    rootWinId.audioCookies = NamMgr.cookies()
                                    console.log("rootWinId.audioCookies:", rootWinId.audioCookies)

                                    nplayerId.prepare()
                                    playControlId.startPlay()
                                }
                            }
                        }
                    }
                }

                PlayControls {
                    id: playControlId

                    width: 798
                    height: 60

                    bvid: audioBvid
                    imgKey: userMgr.imgKey
                    subKey: userMgr.subKey

                    endTime: audioDurationReal > 0 ?audioDurationReal : audioDuration

                    from: 0
                    to: playControlId.endTime
                    value: 0

                    audioVolumn: rootWinId.audioVolumn

                    onAutoPlay: {
                        nplayerId.isEnd = false
                        nplayerId.play()
                    }

                    onPrevClicked: {
                        videoListId.selectPrev()
                    }

                    onNextClicked: {
                        videoListId.selectNext()
                    }

                    onPlayClicked: function() {
                        if (nplayerId.isPlaying()) {
                            nplayerId.pause()
                        } else {
                            nplayerId.isEnd = false
                            nplayerId.play()
                        }
                    }

                    onNeedPause: {
                        nplayerId.pause()
                    }

                    onNeedPlay: function(position) {
                        nplayerId.isEnd = false
                        nplayerId.seek(position)
                        nplayerId.play()
                    }

                    onVolumnChanged: function(volumn) {
                        rootWinId.audioVolumn = volumn
                    }
                }

            }

            layer.enabled: true
            layer.smooth: true
            layer.samples: 16
            layer.effect: OpacityMask {
                maskSource: Item {
                    width: rootRectId.width
                    height: rootRectId.height
                    Rectangle {
                        anchors.centerIn: parent
                        width: parent.width
                        height: parent.height
                        radius: rootRectId.radius+2
                    }
                }
            }
        }
    }

    QCC.Dialog {
        id: dialog
        anchors.centerIn: parent
        title: "扫描二维码"
        modal: true
        width: 150+25
        height: 150+65

        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: console.log("Ok clicked")
        onRejected: console.log("Cancel clicked")

        Image {
            width: 150
            height: 150
            fillMode: Image.Stretch
            source: loginMgr.qrcodeImg
        }
    }

    Connections {
        target: loginMgr
        function onIsLoggingInChanged() {
            console.log("loginMgr.isLoggingIn:", loginMgr.isLoggingIn)
        }
        function onStatusTextChanged() {
            console.log("loginMgr.statusText:", loginMgr.statusText)
        }
        function onLoginSuccess(refresh_token) {
            console.log("refresh_token:", refresh_token)
            userMgr.fetchUserData();
        }
        function onQrcodeInvalid() {
            console.log("onQrcodeInvalid")
        }
    }

    Connections {
        target: userMgr
        function onUserDataLoaded() {
            favMgr.fetchFavList(userMgr.uid);
            user.uid = userMgr.uid
            user.name = userMgr.uname
            user.avatar = userMgr.avatarUrl
            user.isLogin = userMgr.isLogin
            user.coins = userMgr.coins
            user.isVip = userMgr.vipStatus
            dialog.close()

            console.log(NamMgr.cookies())
        }
    }

    Connections {
        target: favMgr
        function onFavListLoaded() {
            console.log("onFavListLoaded")
            for (let i = 0; i < favMgr.items.length; i++) {
                console.log(favMgr.items[i].title)
            }
        }
        function onFavMgrError(msg) {
            console.log("onFavMgrError:", msg)
        }
    }

    VideoListModel {
        id: videoListModelId
        favId: ""

        onFavListLoaded: function() {
            console.log("onFavListLoaded:", videoListModelId.favId)
        }
    }

    NPlayer {
        id: nplayerId

        bvid: audioBvid
        cookies: audioCookies
        source: audioSource

        volumn: audioVolumn

        property bool isEnd: false

        onMediaParsed: {
        }

        onVideoStart: {
            rootWinId.audioDurationReal = nplayerId.realDurationMs
            console.log("onVideoStart,nplayerId.realDurationMs:", nplayerId.realDurationMs);
        }

        onVideoEnd: {
            console.log("onVideoEnd");
            isEnd = true
        }

        onVideoUpdate: function(time) {
            playControlId.value = time
        }

        onVideoStopped: function() {
            if (isEnd) {
                console.log("onVideoStop");
                isEnd = false
                playControlId.value = playControlId.endTime
                videoListId.selectNext()
            }
        }
    }

    SystemTrayIcon {
        id: trayIcon
        visible: true
        icon.source: "qrc:/resources/logo_32x32.ico"
        tooltip: qsTr("NEKOPAD")

        onActivated: {
            if (reason === SystemTrayIcon.Trigger) {
                if (rootWinId.visible) {
                    rootWinId.hide()
                } else {
                    rootWinId.show()
                    rootWinId.raise()
                    rootWinId.requestActivate()
                }
            }
        }

        menu: Menu {
            MenuItem {
                text: rootWinId.visible ? "隐藏窗口" : "显示窗口"
                onTriggered: {
                    if (rootWinId.visible) {
                        rootWinId.hide()
                    } else {
                        rootWinId.show()
                        rootWinId.raise()
                    }
                }
            }
            MenuSeparator {}
            MenuItem {
                text: qsTr("退出")
                onTriggered: Qt.quit()
            }
        }
    }

}
