import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.Material

T.ToolTip {
    id: control

    property int duration: 100
    property string textColor: "#000"
    property string backgroundColor: "#fff"

    property alias border: bgRectId.border

    x: parent ? (parent.width - implicitWidth) / 2 : 0
    y: -implicitHeight - 24

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    margins: 12
    padding: 8
    horizontalPadding: padding + 8

    closePolicy: T.Popup.CloseOnEscape | T.Popup.CloseOnPressOutsideParent | T.Popup.CloseOnReleaseOutsideParent

    Material.theme: Material.Light

    enter: Transition {
        // toast_enter
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; easing.type: Easing.OutQuad; duration: control.duration }
    }

    exit: Transition {
        // toast_exit
        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; easing.type: Easing.InQuad; duration: control.duration }
    }

    contentItem: Text {
        text: control.text
        font: control.font
        wrapMode: Text.Wrap
        color: textColor
    }

    background: Rectangle {
        id: bgRectId
        implicitHeight: control.Material.tooltipHeight
        color: backgroundColor
        opacity: 0.9
        radius: 2
    }
}
