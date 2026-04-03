import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: rootId
    property var favList
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
                    spacing: 2
                    boundsBehavior: Flickable.StopAtBounds
                    // ScrollBar.vertical: ScrollBar {
                    //     policy: ScrollBar.AsNeeded // 仅在需要时显示
                    //     active: true // 始终显示或在交互时显示
                    //     background: Rectangle {
                    //         implicitWidth: 4
                    //         height: parent.height
                    //     }
                    //     contentItem: Rectangle {
                    //         color: "pink"
                    //     }
                    //     Rectangle {
                    //         anchors.fill: parent
                    //         color: "#fafafa"
                    //     }
                    // }

                    model: favList

                    delegate: Rectangle {
                        required property string favId
                        required property string title

                        id: favItemId
                        width: 159
                        height: 33
                        anchors.topMargin: 18
                        anchors.bottomMargin: 18
                        anchors.leftMargin: 15
                        anchors.rightMargin: 15

                        border.color: rootId.currentFavId === favId ? "#e4e4e7" : ""
                        border.width: rootId.currentFavId === favId ? 1.3 : 0
                        radius: 8

                        color: favMAId.containsMouse ? "#efeff1" : "#fafafa"

                        Text {
                            id: favTextId
                            anchors.centerIn: parent
                            width: parent.width-30
                            clip: true

                            text: title
                            font.pixelSize: 12
                            color: rootId.currentFavId === favId ? "#fb7185" : (favMAId.containsMouse ? "#18181b" : "#71717a")
                        }

                        MouseArea {
                            id: favMAId
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor

                            onClicked: {
                                rootId.currentFavId = favId
                                rootId.favChoice(favId)
                            }
                        }
                    }
                }
            }
        }
    }

    ListModel {
        id: listModel
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
        ListElement {
            favid: "6"
            title: "First Favorate Music | Top 100 Hachimi Music"
        }
        ListElement {
            favid: "7"
            title: "First Favorate Music | Top 100 Hachimi Music"
        }
        ListElement {
            favid: "8"
            title: "First Favorate Music | Top 100 Hachimi Music"
        }
        ListElement {
            favid: "9"
            title: "First Favorate Music | Top 100 Hachimi Music"
        }
        ListElement {
            favid: "11"
            title: "First Favorate Music | Top 100 Hachimi Music"
        }
        ListElement {
            favid: "12"
            title: "First Favorate Music | Top 100 Hachimi Music"
        }
    }
}
