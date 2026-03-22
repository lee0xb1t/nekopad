import QtQuick 2.15
import QtQuick.Layouts

Item {
    id: rootId

    required property string currentVid

    required property int index
    required property string vid
    required property string title

    signal choiceVid(string vid)

    width: 500
    height: 300

    Rectangle {
        id: containerParentRectId
        anchors.fill: parent
        radius: 8
        color: currentVid === vid ? "#f4f4f5" : (videoItemMAId.containsMouse ? "#fafafa" : "transparent")
        border.color: currentVid === vid ? "#e4e4e7" : (videoItemMAId.containsMouse ? "#f4f4f5" : "transparent")
        border.width: 1

        Rectangle {
            id: containerRectId
            width: parent.width-2
            height: parent.height-2
            color: "transparent"
            anchors.centerIn: parent

            RowLayout {
                width: parent.width
                height: parent.height
                spacing: 25

                Rectangle {
                    width: 20
                    height: playTextId.implicitHeight
                    // anchors.verticalCenter: parent.verticalCenter
                    color: "transparent"

                    Text {
                        id: playTextId
                        color: videoItemMAId.containsMouse ? "#52525b" : "#a1a1aa"
                        anchors.right: parent.right
                        text: index+1
                    }
                }

                Rectangle {
                    width: 40
                    height: 40
                    // anchors.verticalCenter: parent.verticalCenter
                    color: "transparent"

                    Image {
                        anchors.fill: parent
                        source: "https://picsum.photos/seed/m83/300/300"
                        smooth: true
                        mipmap: true
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 60
                    // anchors.verticalCenter: parent.verticalCenter
                    color: "transparent"

                    Text {
                        anchors.centerIn: parent
                        text: rootId.title
                    }
                }

                Rectangle {
                    width: 56
                    height: timeTextId.implicitHeight
                    color: "transparent"

                    Text {
                        id: timeTextId
                        color: videoItemMAId.containsMouse ? "#52525b" : "#a1a1aa"
                        text: "05:20"
                    }
                }
            }

            MouseArea {
                id: videoItemMAId
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                onClicked: {
                    choiceVid(rootId.vid)
                }
            }
        }
    }
}
