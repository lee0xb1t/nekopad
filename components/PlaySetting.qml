import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Shapes
import QtQuick.Controls.Material

Item {
    property int currentVolumn: 50
    property int toggleVolumn: 0

    signal volumnChanged(int volumn)

    id: rootId

    width: 300
    height: 30

    Rectangle {
        width: parent.width
        height: parent.height

        RowLayout {
            spacing: 10
            height: parent.height
            anchors.right: parent.right

            Rectangle {
                width: 14
                height: 14

                Layout.alignment: Qt.AlignVCenter

                Shape {
                    fillMode: Shape.Stretch
                    width: 14
                    height: 14
                    anchors.centerIn: parent
                    asynchronous: true

                    layer.enabled: true
                    layer.smooth: true
                    layer.samples: 16

                    ShapePath {
                        strokeWidth: 2.0
                        strokeColor: "#939399"
                        fillColor: "transparent"

                        capStyle: ShapePath.RoundCap
                        joinStyle: ShapePath.RoundJoin

                        pathElements: [
                            PathSvg {
                                path: rootId.settingSvgPath
                            },
                            PathSvg {
                                path: rootId.settingSvgPath1
                            }
                        ]
                    }
                }

            }

            Rectangle {
                width: 14
                height: 14

                Layout.alignment: Qt.AlignVCenter

                Shape {
                    fillMode: Shape.Stretch
                    width: 13
                    height: 13
                    anchors.centerIn: parent
                    asynchronous: true

                    layer.enabled: true
                    layer.smooth: true
                    layer.samples: 16

                    ShapePath {
                        strokeWidth: 2.0
                        strokeColor: "#939399"
                        fillColor: "transparent"

                        capStyle: ShapePath.RoundCap
                        joinStyle: ShapePath.RoundJoin

                        pathElements: [
                            PathSvg {
                                path: rootId.orderSvgPath1
                            },
                            PathSvg {
                                path: rootId.orderSvgPath2
                            },
                            PathSvg {
                                path: rootId.orderSvgPath3
                            },
                            PathSvg {
                                path: rootId.orderSvgPath4
                            }
                        ]
                    }
                }
            }

            Rectangle {
                width: 1
                height: 14
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                color: "#e4e4e7"
            }

            Rectangle {
                width: 100
                height: 14

                Layout.alignment: Qt.AlignVCenter

                // border.width: 1
                // border.color: "black"

                RowLayout {
                    anchors.fill: parent
                    spacing: 0

                    Rectangle {
                        width: 12
                        height: 12

                        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

                        Shape {
                            fillMode: Shape.Stretch
                            width: parent.width
                            height: parent.height
                            anchors.centerIn: parent
                            asynchronous: true

                            layer.enabled: true
                            layer.smooth: true
                            layer.samples: 16

                            ShapePath {
                                strokeWidth: 2.0
                                strokeColor: "#939399"
                                fillColor: "transparent"

                                capStyle: ShapePath.RoundCap
                                joinStyle: ShapePath.RoundJoin

                                pathElements: getVolumnSvgByVolumn(volumnBarId.value)
                            }
                        }

                        MouseArea {
                            id: volumnIconMAId
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                            onClicked: {
                                let tempVolumn = currentVolumn
                                currentVolumn = toggleVolumn
                                toggleVolumn = tempVolumn

                                volumnBarId.value = currentVolumn
                                volumnChanged(currentVolumn)
                            }
                        }

                        NToolTip {
                            visible: volumnIconMAId.containsMouse
                            text: currentVolumn
                            font.pixelSize: 13
                            textColor: "#000"
                            backgroundColor: "#fff"
                            border.width: 1
                            border.color: "#000"
                            delay: 3000
                        }
                    }

                    NProgressBar {
                        id: volumnBarId
                        width: parent.width-30
                        height: 13

                        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

                        from: 0
                        to: 120
                        value: currentVolumn

                        onPressed: function(val) {
                        }

                        onReleased: function(val) {
                            toggleVolumn = 0
                            currentVolumn = val

                            volumnChanged(val)
                        }

                        NToolTip {
                            visible: volumnBarId.hovered
                            text: currentVolumn
                            font.pixelSize: 13
                            textColor: "#000"
                            backgroundColor: "#fff"
                            border.width: 1
                            border.color: "#000"
                            delay: 3000
                        }
                    }
                }
            }

            Rectangle {
                width: 40
                height: 19

                border.width: 1
                border.color: qualityMAId.containsMouse ? "#fb7185" : "#e4e4e7"
                radius: 4

                Rectangle {
                    width: qualityTextId.implicitWidth
                    height: qualityTextId.implicitHeight
                    anchors.centerIn: parent

                    Text {
                        id: qualityTextId
                        text: "1080P"
                        color: qualityMAId.containsMouse ? "#fb7185" : "#71717a"
                        font.pixelSize: 10

                        Behavior on color {
                            ColorAnimation {
                                duration: 100
                            }
                        }
                    }
                }

                MouseArea {
                    id: qualityMAId
                    anchors.fill: parent
                    hoverEnabled: true
                    // cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                    onClicked: {
                    }
                }

                Behavior on border.color {
                    ColorAnimation {
                        duration: 100
                    }
                }
            }
        }
    }


    //
    property string settingSvgPath: "M9.671 4.136a2.34 2.34 0 0 1 4.659 0 2.34 2.34 0 0 0 3.319 1.915 2.34 2.34 0 0 1 2.33 4.033 2.34 2.34 0 0 0 0 3.831 2.34 2.34 0 0 1-2.33 4.033 2.34 2.34 0 0 0-3.319 1.915 2.34 2.34 0 0 1-4.659 0 2.34 2.34 0 0 0-3.32-1.915 2.34 2.34 0 0 1-2.33-4.033 2.34 2.34 0 0 0 0-3.831A2.34 2.34 0 0 1 6.35 6.051a2.34 2.34 0 0 0 3.319-1.915"
    property string settingSvgPath1: "M12 15a3 3 0 1 0 0-6 3 3 0 0 0 0 6z"

    property string orderSvgPath1: "M17 2l4 4-4 4"
    property string orderSvgPath2: "M3 11v-1a4 4 0 0 1 4-4h14"
    property string orderSvgPath3: "M7 22l-4-4 4-4"
    property string orderSvgPath4: "M21 13v1a4 4 0 0 1-4 4H3"

    property list<PathSvg> volumn2Svgs : [
        PathSvg {
            path: "M11 4.702a.705.705 0 0 0-1.203-.498L6.413 7.587A1.4 1.4 0 0 1 5.416 8H3a1 1 0 0 0-1 1v6a1 1 0 0 0 1 1h2.416a1.4 1.4 0 0 1 .997.413l3.383 3.384A.705.705 0 0 0 11 19.298z"
        },
        PathSvg {
            path: "M16 9a5 5 0 0 1 0 6"
        },
        PathSvg {
            path: "M19.364 18.364a9 9 0 0 0 0-12.728"
        }
    ]

    property list<PathSvg> volumn1Svgs : [
        PathSvg {
            path: "M11 4.702a.705.705 0 0 0-1.203-.498L6.413 7.587A1.4 1.4 0 0 1 5.416 8H3a1 1 0 0 0-1 1v6a1 1 0 0 0 1 1h2.416a1.4 1.4 0 0 1 .997.413l3.383 3.384A.705.705 0 0 0 11 19.298z"
        },
        PathSvg {
            path: "M16 9a5 5 0 0 1 0 6"
        }
    ]

    property list<PathSvg> volumnXSvgs : [
        PathSvg {
            path: "M11 4.702a.705.705 0 0 0-1.203-.498L6.413 7.587A1.4 1.4 0 0 1 5.416 8H3a1 1 0 0 0-1 1v6a1 1 0 0 0 1 1h2.416a1.4 1.4 0 0 1 .997.413l3.383 3.384A.705.705 0 0 0 11 19.298z"
        },
        PathSvg {
            path: "M22 9L16 15"
        },
        PathSvg {
            path: "M16 9L22 15"
        }
    ]

    function getVolumnSvgByVolumn(volumn) {
        if (volumn >= 40) {
            return volumn2Svgs
        } else if (volumn >= 1 && volumn < 40) {
            return volumn1Svgs
        } else if (volumn < 1) {
            return volumnXSvgs
        } else {
            return volumn2Svgs
        }
    }
}
