import QtQuick 2.15
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Item {
    id: userRootId

    property string uid
    property string name
    property string avatar
    property int coins
    property bool isVip

    property bool isLogin: false
    property string defaultAvatar: "https://i0.hdslb.com/bfs/face/member/noface.jpg"

    signal userClicked(bool isLogin, string uid)

    width: 197
    height: 69

    Rectangle {
        id: userRectId
        anchors.fill: parent

        border.width: 1
        border.color: "#e4e4e7"

        color: "transparent"

        Rectangle {
            width: parent.width
            height: parent.height-1
            color: parent.color
            anchors.bottom: parent.bottom

            Rectangle {
                width: parent.width
                height: parent.height
                anchors.centerIn: parent
                color: "#fafafa"

                Rectangle {
                    width: parent.width/1.3
                    height: parent.height-2
                    anchors.centerIn: parent
                    color: parent.color
                    // anchors.leftMargin: 20

                    RowLayout {
                        id: layoutId
                        spacing: 10
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            width: 40
                            height: 40
                            color: "transparent"
                            // radius: 40
                            // border.width: 1
                            // border.color: "black"

                            Image {
                                id: avatarImgId
                                anchors.fill: parent
                                source: isLogin ? avatar : defaultAvatar

                                layer.enabled: true
                                layer.effect: OpacityMask {
                                    maskSource: Item {
                                        width: avatarImgId.width
                                        height: avatarImgId.height
                                        Rectangle {
                                            anchors.centerIn: parent
                                            width: 40
                                            height: 40
                                            radius: 40
                                        }
                                    }
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                                onClicked: {
                                    userRootId.userClicked(userRootId.isLogin, userRootId.uid)
                                }
                            }
                        }

                        Rectangle {
                            width: nameTextId.implicitWidth
                            height: 40
                            color: userRectId.color

                            Text {
                                anchors.centerIn: parent
                                id: nameTextId
                                text: isLogin ? name : "Login"
                            }

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                                onClicked: {
                                    userRootId.userClicked(userRootId.isLogin, userRootId.uid)
                                }
                            }
                        }
                    }
                }
            }
        }
    }

}
