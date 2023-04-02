import QtQuick 2.2

Rectangle {
    id: root

    property alias cursorShape: mouseArea.cursorShape

    signal positionChanged(var mouse)

    border {
        color: "green"
        width: 1
    }
    color: "transparent"
    height: width
    width: 20

    MouseArea {
        id: mouseArea

        anchors.fill: parent
//        cursorShape: pressed ? Qt.ClosedHandCursor : Qt.OpenHandCursor

        onPositionChanged: function(mouse) {
            root.positionChanged(mouse)
        }
    }
}
