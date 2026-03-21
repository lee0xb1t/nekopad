import QtQuick
import QtQuick.Layouts
import "components"

Window {
    id: rootWinId
    width: 800
    height: 580
    visible: true
    color: "#fff"

    Rectangle {
        width: 200
        height: 520
        border.color: "black"
        border.width: 1

        ColumnLayout {
            width: 200
            spacing: 0

            Logo {
                color: "#fafafa"
            }

            Fav {
                color: "#fafafa"
            }

            User {
                color: "#fafafa"
            }
        }
    }
}
