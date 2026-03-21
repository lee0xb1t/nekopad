import QtQuick
import QtQuick.Layouts
import "components"

Window {
    id: rootWinId
    width: 800
    height: 580
    visible: true
    color: "#fafafa"

    Rectangle {
        width: 200
        height: 520
        border.color: "#e4e4e7"
        border.width: 1
        color: "#fafafa"

        ColumnLayout {
            width: 200
            spacing: 0

            Logo {
            }

            Fav {
            }

            User {
            }
        }
    }
}
