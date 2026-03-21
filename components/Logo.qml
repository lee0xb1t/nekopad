import QtQuick 2.15
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Item {
    id: logoRootId
    width: 199
    height: 85

    Rectangle {
        id: logoRectId
        anchors.fill: parent
        color: "transparent"

        RowLayout {
            anchors.centerIn: parent
            spacing: 10

            Rectangle {
                id: logoRect1Id
                width: 28
                height: 28
                radius: 8

                color: "#fb7185"

                Text {
                    anchors.centerIn: parent
                    text: "N"
                    color: "#fff"
                    font.bold: true
                    font.weight: 800
                    font.pixelSize: 14
                }

                layer.enabled: true
                layer.effect: DropShadow {
                    horizontalOffset: 0
                    verticalOffset: 3      // 阴影向下偏移
                    radius: 10             // 模糊半径，越大越柔和
                    color: "#fecdd3"       // 带透明度的黑色 (Alpha 通道很重要)
                    samples: 16            // 采样率，越高边缘越平滑 (性能消耗略增)
                    cached: true           // 【重要】缓存效果，如果 Rectangle 不动，大幅提升性能
                }
            }

            Rectangle {
                id: logoRect2Id
                width: text2Id.implicitWidth
                height: text2Id.implicitHeight
                color: logoRectId.color

                Text {
                    id: text2Id
                    anchors.centerIn: parent
                    text: "NICOPAD"
                    color: "#18181b"
                    font.bold: true
                    font.weight: 700
                    font.pixelSize: 16
                    font.letterSpacing: 1.5
                }
            }
        }
    }
}
