import QtQuick 2.15
import QtQuick.Layouts

Item {
    property string currentVid

    id: rootId

    Rectangle {
        id: logoRectId
        anchors.fill: parent
        color: "transparent"
        border.color: "#fafafa"
        border.width: 1

        Rectangle {
            anchors.bottom: parent.bottom
            width: parent.width
            height: parent.height-1
            color: "#fff"

            Rectangle {
                id: listContainerId
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                anchors.topMargin: 4
                anchors.bottomMargin: 4

                // ColumnLayout {
                //     width: parent.width
                //     spacing: 4


                // }
                ListView {
                    anchors.fill: parent
                    spacing: 4
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds

                    model: mmodel
                    delegate: VItem {
                        id: vitemId
                        width: parent.width
                        height: 66
                        antialiasing: true

                        currentVid: rootId.currentVid

                        onChoiceVid: function (v) {
                            console.log(v)
                            rootId.currentVid = v
                        }
                    }
                }
            }
        }
    }

    ListModel {
        id: mmodel
        ListElement {
            vid: "a"
            title: "AA"
            second: 12
            thumburl: ""
            author: "L"
        }
        ListElement {
            vid: "b"
            title: "AA"
            second: 12
            thumburl: ""
            author: "L"
        }
        ListElement {
            vid: "c"
            title: "AA"
            second: 12
            thumburl: ""
            author: "L"
        }
        ListElement {
            vid: "d"
            title: "AA"
            second: 12
            thumburl: ""
            author: "L"
        }
        ListElement {
            vid: "e"
            title: "AA"
            second: 12
            thumburl: ""
            author: "L"
        }
        ListElement {
            vid: "f"
            title: "AA"
            second: 12
            thumburl: ""
            author: "L"
        }
        ListElement {
            vid: "g"
            title: "AA"
            second: 12
            thumburl: ""
            author: "L"
        }
    }
}
