import QtQuick 2.15
import QtQuick.Layouts

Item {
    property string currentVid
    property var videoListModel

    property string imgKey
    property string subKey

    property string selectDirection: "next"

    signal choiceVideo(string bvid, string url, int duration)

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
                    id: listViewId

                    anchors.fill: parent
                    spacing: 4
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds

                    model: videoListModel
                    delegate: VItem {
                        id: vitemId
                        width: listContainerId.width
                        height: 66
                        antialiasing: true

                        currentVid: rootId.currentVid
                        imgKey: rootId.imgKey
                        subKey: rootId.subKey

                        onChoiceVid: function (bvid) {
                            listViewId.currentIndex = index
                            rootId.currentVid = bvid
                            console.log("onChoiceVid, bvid:", bvid)
                        }

                        onChoiceVideo: function(url, duration) {
                            console.log("onChoiceVideo:", url)
                            rootId.choiceVideo(rootId.currentVid, url, duration)
                        }

                        onVideoInvalid: {
                            if (selectDirection === "next") {
                                selectNext()
                            } else {
                                selectPrev()
                            }
                        }
                    }
                }
            }
        }
    }

    function selectPrev() {
        selectDirection = "prev"

        var prevIndex = listViewId.currentIndex-1
        if (prevIndex >= 0) {
            console.log("current select index:", prevIndex)
        } else {
            console.log("the last index")
            prevIndex = 0
        }

        selectIndex(prevIndex)
    }

    function selectNext() {
        selectDirection = "next"

        var nextIndex = listViewId.currentIndex+1
        if (nextIndex < videoListModel.rowCount()) {
            console.log("current select index:", nextIndex)
        } else {
            console.log("the last index")
            nextIndex = 0
        }

        selectIndex(nextIndex)
    }

    function selectIndex(index) {
        let itemModel = videoListModel.items[index]
        var delegateItem = listViewId.itemAtIndex(index)

        listViewId.positionViewAtIndex(index, ListView.Center)

        delegateItem.select(itemModel.bvid)
    }

    ListModel {
        id: mmodel
        ListElement {
            mediaId: "a"
            type: 1
            cover: "https://i0.hdslb.com/bfs/archive/53626a003127a20c68b73f89cb431891b046ba72.jpg"
            duration: 1
            attr: 1
            bvid: "BV19z421q7cH"
            upId: "a"
            upName: "a"
            title: "test title"
        }
        ListElement {
            mediaId: "a"
            type: 1
            cover: "https://i1.hdslb.com/bfs/archive/c7a9bae86be26c0a707843f1073ca3327bf2331f.jpg"
            duration: 1
            attr: 1
            bvid: "BV1BdiMBCEke"
            upId: "a"
            upName: "a"
            title: "“test title 1"
        }
        ListElement {
            mediaId: "a"
            type: 1
            cover: "https://i1.hdslb.com/bfs/archive/c7a9bae86be26c0a707843f1073ca3327bf2331f.jpg"
            duration: 1
            attr: 1
            bvid: "a"
            upId: "a"
            upName: "a"
            title: "test title 2"
        }
    }
}
