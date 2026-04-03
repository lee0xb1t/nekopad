import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material

Rectangle {
    property alias hovered: controlMAId.containsMouse

    signal pressed(double position)
    signal released(double position)
    signal posChanged(double position)

    property alias from: control.from
    property alias to: control.to
    property alias value: control.value

    id: rootId
    width: 300
    height: 15

    ProgressBar {
        id: control

        anchors.verticalCenter: parent.verticalCenter

        width: parent.width
        height: 4

        Behavior on value {
            NumberAnimation { duration: 100; easing.type: Easing.OutQuad }
        }

        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 4
            color: "#e4e4e7"
            radius: 2
            border.color: "#e4e4e7"
        }

        contentItem: Item {
            implicitWidth: 200
            implicitHeight: 4
            clip: true

            Rectangle {
                id: progressRect
                width: control.visualPosition * parent.width
                height: parent.height
                radius: 2
                color: "#fb7185"

                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#fb7185" }
                    GradientStop { position: 1.0; color: "#ff9eb5" }
                }
            }
        }
    }

    MouseArea {
        id: controlMAId
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onPressed: function(mouse) {
            updateValue(mouse.x)
            rootId.pressed(Math.round(control.value))
        }

        onPositionChanged: function(mouse) {
            if (pressed) {
                updateValue(mouse.x)
                posChanged(Math.round(control.value))
            }
        }

        onReleased: function(mouse) {
            updateValue(mouse.x)
            rootId.released(Math.round(control.value))
        }

        function updateValue(mouseX) {
            var ratio = mouseX / width;
            ratio = Math.max(0, Math.min(1, ratio));
            var actualValue = control.from + (ratio * (control.to - control.from));
            control.value = actualValue;
        }
    }
}
