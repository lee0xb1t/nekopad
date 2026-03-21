import QtQuick 2.15
import QtQuick.Layouts

Item {
    id: rootId

    property string currentFavId: ""
    signal favChoice(string id)

    Rectangle {
        id: favListId
        // anchors.fill: parent
        width: parent.width
        height: parent.height

        anchors.leftMargin: 15
        anchors.rightMargin: 15
        anchors.topMargin: 5
        anchors.bottomMargin: 5

        color: "#fafafa"

        Rectangle {
            id: favItemWrapId
            width: 159
            height: parent.height
            anchors.centerIn: parent

            color: "#fafafa"

            ColumnLayout {
                width: parent.width
                height: parent.height
                spacing: 1

                ListView {
                    width: parent.width
                    height: parent.height
                    clip: true

                    model: ListModel {
                        ListElement {
                            favid: "1"
                            title: "First Favorate Music | Top 100 Hachimi Music"
                        }
                        ListElement {
                            favid: "2"
                            title: "First Favorate Music | Top 100 Hachimi Music"
                        }
                        ListElement {
                            favid: "3"
                            title: "First Favorate Music | Top 100 Hachimi Music"
                        }
                        ListElement {
                            favid: "4"
                            title: "First Favorate Music | Top 100 Hachimi Music"
                        }
                        ListElement {
                            favid: "5"
                            title: "First Favorate Music | Top 100 Hachimi Music"
                        }
                    }

                    delegate: Rectangle {
                        required property string favid
                        required property string title

                        id: favItemId
                        width: 159
                        height: 32
                        anchors.topMargin: 18
                        anchors.bottomMargin: 18
                        anchors.leftMargin: 15
                        anchors.rightMargin: 15

                        border.color: rootId.currentFavId === favid ? "#e4e4e7" : ""
                        border.width: rootId.currentFavId === favid ? 1.3 : 0
                        radius: 8

                        color: favMAId.containsMouse ? "#efeff1" : "#fafafa"

                        Text {
                            id: favTextId
                            anchors.centerIn: parent
                            width: parent.width-30
                            clip: true

                            text: title
                            font.pixelSize: 12
                            color: rootId.currentFavId === favid ? "#fb7185" : (favMAId.containsMouse ? "#18181b" : "#71717a")
                        }

                        MouseArea {
                            id: favMAId
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor

                            onClicked: {
                                rootId.currentFavId = favid
                                rootId.favChoice(favid)
                            }
                        }
                    }
                }
            }
        }
    }

}
