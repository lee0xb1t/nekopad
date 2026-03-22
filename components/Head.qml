import QtQuick 2.15
import QtQuick.Layouts

Item {
    id: rootId

    Rectangle {
        id: logoRectId
        anchors.fill: parent
        color: "transparent"

        Rectangle {
            anchors.right: logoRectId.right
            width: layoutId.implicitWidth
            height: layoutId.implicitHeight
            color: "transparent"

            RowLayout {
                id: layoutId
                spacing: 0

                Rectangle {
                    width: 40
                    height: 32
                    Text {
                        anchors.centerIn: parent
                        text: "_"
                    }
                }

                Rectangle {
                    width: 40
                    height: 32
                    Text {
                        anchors.centerIn: parent
                        text: "口"
                    }
                }

                Rectangle {
                    width: 40
                    height: 32
                    Text {
                        anchors.centerIn: parent
                        text: "×"
                    }
                }
            }
        }
    }
}
