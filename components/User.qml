import QtQuick 2.15

Item {
    property alias color: userRectId.color

    id: userRootid
    width: 199
    height: 70
    Rectangle {
        id: userRectId
        anchors.fill: parent
        color: "green"
    }
}
