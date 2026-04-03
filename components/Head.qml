import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Shapes

Item {
    id: rootId

    signal minimized()
    signal closed()

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
                    color: minimumMAId.containsMouse ? "#f4f4f5" : "#fff"
                    // border.width: 1
                    // border.color: "#000"

                    Shape {
                        width: 13
                        height: 13
                        asynchronous: true
                        fillMode: Shape.PreserveAspectFit
                        anchors.centerIn: parent

                        layer.enabled: true
                        layer.smooth: true
                        layer.samples: 16

                        ShapePath {
                            strokeWidth: 2.0
                            strokeColor: minimumMAId.containsMouse ? "#411c1b" : "#71717a"
                            fillColor: "transparent"

                            capStyle: ShapePath.RoundCap
                            joinStyle: ShapePath.RoundJoin

                            startX: 5; startY: 12
                            PathLine { x: 19; y: 12 }
                        }
                    }

                    MouseArea {
                        id: minimumMAId
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                        onClicked: {
                            minimized()
                        }
                    }

                    Behavior on color {
                        ColorAnimation {
                            duration: 100
                        }
                    }
                }

                Rectangle {
                    width: 40
                    height: 32
                    color: closeMAId.containsMouse ? "#ef4444" : "#fff"

                    Shape {
                        width: 13
                        height: 13
                        asynchronous: true
                        fillMode: Shape.PreserveAspectFit
                        anchors.centerIn: parent

                        layer.enabled: true
                        layer.smooth: true
                        layer.samples: 16

                        ShapePath {
                            strokeWidth: 2.0
                            strokeColor: closeMAId.containsMouse ? "#fff" : "#71717a"
                            fillColor: "transparent"

                            capStyle: ShapePath.RoundCap
                            joinStyle: ShapePath.RoundJoin

                            pathElements: closeSvgs
                        }
                    }

                    MouseArea {
                        id: closeMAId
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                        onClicked: {
                            closed()
                        }
                    }

                    Behavior on color {
                        ColorAnimation {
                            duration: 90
                        }
                    }
                }
            }
        }
    }


    property list<PathSvg> closeSvgs : [
        PathSvg {
            path: "M18 6 6 18"
        },
        PathSvg {
            path: "m6 6 12 12"
        }
    ]
}
