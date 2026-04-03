import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import QtQuick.Shapes
import nekopad

Item {
    property string bvid: ""
    property string imgKey
    property string subKey

    property string cover: ""
    property string vtitle
    property string vauthorid: ""
    property string vauthor: ""

    property int endTime: 0

    property alias from: progressBarId.from
    property alias to: progressBarId.to
    property alias value: progressBarId.value

    property int audioVolumn: 50

    property bool isPlaying: true

    signal prevClicked();
    signal nextClicked();
    signal playClicked();

    signal needPause();
    signal needPlay(double val);

    signal autoPlay();

    signal volumnChanged(int volumn)

    id: rootId
    width: 798
    height: 60

    Rectangle {
        width: parent.width
        height: parent.height
        border.width: 1
        border.color: "#f4f4f5"

        Rectangle {
            width: parent.width
            height: parent.height-1
            color: "#fff"

            anchors.bottom: parent.bottom

            RowLayout {
                anchors.fill: parent
                spacing: 5

                Rectangle {
                    Layout.leftMargin: 20

                    width: 40
                    height: 40

                    color: "#F1F2F3"

                    Image {
                        id: imgId
                        anchors.fill: parent
                        source: rootId.cover
                        smooth: true
                        mipmap: true
                        fillMode: Image.PreserveAspectCrop

                        layer.enabled: true
                        layer.effect: OpacityMask {
                            maskSource: Item {
                                width:  imgId.width
                                height: imgId.height
                                Rectangle {
                                    anchors.centerIn: parent
                                    width: imgId.width
                                    height: imgId.height
                                    radius: 10
                                }
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                        onClicked: {
                            goBvid(rootId.bvid)
                        }
                    }
                }

                Rectangle {
                    width: 100
                    height: 40

                    MouseArea {
                        id: musicTextMAId
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                        onClicked: {
                            goBvid(rootId.bvid)
                        }
                    }

                    ColumnLayout {
                        anchors.verticalCenter: parent.verticalCenter

                        spacing: 0

                        Rectangle {
                            width: 110
                            height: musicTextId.implicitHeight
                            clip: true

                            Text {
                                id: musicTextId
                                font.bold: true
                                font.pixelSize: 12
                                color: musicTitleMAId.containsMouse ? "#fb7185" : "#18181b"
                                text: rootId.vtitle ? rootId.vtitle : ""
                                clip: true

                                Behavior on color {
                                    ColorAnimation {
                                        duration: 100
                                    }
                                }
                            }

                            MouseArea {
                                id: musicTitleMAId
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                                propagateComposedEvents: true
                                onClicked: function(mouse) {
                                    mouse.accepted = false;
                                }
                            }
                        }

                        Rectangle {
                            width: authorTextId.implicitWidth
                            height: authorTextId.implicitHeight

                            Text {
                                id: authorTextId
                                color: "#71717a"
                                text: rootId.vauthor
                                clip: true
                            }

                            MouseArea {
                                id: authorMAId
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                                onClicked: function(mouse) {
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    id: progressRectId
                    width: 320
                    height: 50
                    // border.width: 1
                    // border.color: "#000"

                    ColumnLayout {
                        anchors.right: parent.right
                        spacing: 0

                        Rectangle {
                            width: 310
                            height: 35
                            // border.width: 1
                            // border.color: "#000"

                            RowLayout {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter

                                Rectangle {
                                    width: 16
                                    height: 16

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
                                            strokeColor: prevControlId.containsMouse? "#27272a" : "#a1a1aa"
                                            fillColor: prevControlId.containsMouse? "#27272a" : "#a1a1aa"

                                            capStyle: ShapePath.RoundCap
                                            joinStyle: ShapePath.RoundJoin

                                            pathElements: prevSvgs

                                            Behavior on strokeColor {
                                                ColorAnimation {
                                                    duration: 100
                                                }
                                            }

                                            Behavior on fillColor {
                                                ColorAnimation {
                                                    duration: 100
                                                }
                                            }
                                        }
                                    }

                                    MouseArea {
                                        id: prevControlId
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                                        onClicked: function(mouse) {
                                            isPlaying = false
                                            prevClicked()
                                        }
                                    }
                                }
                                Rectangle {
                                    width: 30
                                    height: 30

                                    radius: 30
                                    color: "#fb7185"

                                    Shape {
                                        fillMode: Shape.PreserveAspectFit
                                        width: 12
                                        height: 12
                                        anchors.centerIn: parent
                                        asynchronous: true

                                        layer.enabled: true
                                        layer.smooth: true
                                        layer.samples: 16

                                        ShapePath {
                                            strokeWidth: 2.0
                                            strokeColor: playControlId.containsMouse? "#fff" : "#fff"
                                            fillColor: playControlId.containsMouse? "#fff" : "#fff"

                                            capStyle: ShapePath.RoundCap
                                            joinStyle: ShapePath.RoundJoin

                                            pathElements: isPlaying ? pauseSvgs : playSvgs

                                            Behavior on strokeColor {
                                                ColorAnimation {
                                                    duration: 100
                                                }
                                            }

                                            Behavior on fillColor {
                                                ColorAnimation {
                                                    duration: 100
                                                }
                                            }
                                        }
                                    }

                                    MouseArea {
                                        id: playControlId
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                                        onClicked: function(mouse) {
                                            if (videoInfoId.isValid) {
                                                isPlaying = !isPlaying
                                            }

                                            playClicked()
                                        }
                                    }
                                }
                                Rectangle {
                                    width: 16
                                    height: 16

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
                                            strokeColor: nextControlId.containsMouse? "#27272a" : "#a1a1aa"
                                            fillColor: nextControlId.containsMouse? "#27272a" : "#a1a1aa"

                                            capStyle: ShapePath.RoundCap
                                            joinStyle: ShapePath.RoundJoin

                                            pathElements: nextSvgs

                                            Behavior on strokeColor {
                                                ColorAnimation {
                                                    duration: 100
                                                }
                                            }

                                            Behavior on fillColor {
                                                ColorAnimation {
                                                    duration: 100
                                                }
                                            }
                                        }
                                    }

                                    MouseArea {
                                        id: nextControlId
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                                        onClicked: function(mouse) {
                                            isPlaying = false
                                            nextClicked()
                                        }
                                    }
                                }
                            }
                        }

                        Rectangle {
                            width: 300
                            height: 15
                            // border.width: 1
                            // border.color: "#000"

                            Layout.alignment: Qt.AlignHCenter

                            RowLayout {
                                spacing: 0
                                anchors.centerIn: parent
                                width: parent.width
                                height: parent.height

                                Rectangle {
                                    width: curTimeTextId.width
                                    height: parent.height

                                    Layout.alignment: Qt.AlignVCenter

                                    Text {
                                        id: curTimeTextId
                                        font.pixelSize: 11
                                        text: msToMinSec(rootId.value)
                                    }
                                }

                                NProgressBar {
                                    id: progressBarId
                                    width: 240
                                    height: 13

                                    // anchors.centerIn: parent
                                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

                                    from: 0
                                    to: rootId.endTime
                                    value: 0

                                    onPressed: function(val) {
                                        console.log("onPressed:", val);
                                        needPause();
                                        isPlaying = false
                                    }

                                    onReleased: function(val) {
                                        console.log("onReleased:", val);
                                        needPlay(val)
                                        isPlaying = true
                                    }
                                }

                                Rectangle {
                                    width: curTimeTextId.width
                                    height: parent.height

                                    Layout.alignment: Qt.AlignVCenter

                                    Text {
                                        id: endTimeTextId
                                        font.pixelSize: 11
                                        text: msToMinSec(endTime)
                                    }
                                }
                            }

                        }
                    }
                }

                Rectangle {
                    width: 190
                    height: 40
                    // border.width: 1
                    // border.color: "#000"

                    PlaySetting {
                        width: parent.width
                        height: parent.height

                        currentVolumn: audioVolumn

                        onVolumnChanged: function(volumn) {
                            rootId.volumnChanged(volumn)
                        }
                    }
                }
            }
        }
    }

    VideoInfo {
        id: videoInfoId
        index: 1
        imgKey: rootId.imgKey
        subKey: rootId.subKey
        bvid: rootId.bvid

        onVideoInfoLoaded: function() {
            rootId.cover = videoInfoId.cover
            rootId.vtitle = videoInfoId.title
            rootId.vauthorid = videoInfoId.ownerMid
            rootId.vauthor = videoInfoId.ownerName

            console.log("onVideoInfoLoaded|videoInfoId.cover:", videoInfoId.cover)

            autoPlay()
            isPlaying = true
        }

        onPlayUrlLoaded: function() {
        }

        onErrorMsg: function(msg) {
            console.log("PlayControls|onErrorMsg:", msg)
        }
    }

    function startPlay() {
        videoInfoId.fetchVideoInfo()
    }


    function goBvid(bvid) {

    }

    function msToMinSec(ms) {
        const totalSeconds = Math.floor(ms / 1000);
        const minutes = Math.floor(totalSeconds / 60);
        const seconds = totalSeconds % 60;
        return `${minutes}:${seconds.toString().padStart(2, '0')}`;
    }


    //
    property list<PathSvg> prevSvgs : [
        PathSvg {
            path: "M17.971 4.285A2 2 0 0 1 21 6v12a2 2 0 0 1-3.029 1.715l-9.997-5.998a2 2 0 0 1-.003-3.432z"
        },
        PathSvg {
            path: "M3 20V4"
        }
    ]

    property list<PathSvg> nextSvgs : [
        PathSvg {
            path: "M21 4v16"
        },
        PathSvg {
            path: "M6.029 4.285A2 2 0 0 0 3 6v12a2 2 0 0 0 3.029 1.715l9.997-5.998a2 2 0 0 0 .003-3.432z"
        }
    ]

    property list<PathSvg> playSvgs : [
        PathSvg {
            path: "M5 5a2 2 0 0 1 3.008-1.728l11.997 6.998a2 2 0 0 1 .003 3.458l-12 7A2 2 0 0 1 5 19z"
        }
    ]

    property list<PathSvg> pauseSvgs : [
        PathSvg {
            path: "M6 3h3a1 1 0 0 1 1 1v16a1 1 0 0 1-1 1H6a1 1 0 0 1-1-1V4a1 1 0 0 1 1-1z"
        },
        PathSvg {
            path: "M15 3h3a1 1 0 0 1 1 1v16a1 1 0 0 1-1 1h-3a1 1 0 0 1-1-1V4a1 1 0 0 1 1-1z"
        }
    ]
}
