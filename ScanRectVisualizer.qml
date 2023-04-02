import QtQuick 2.2

Rectangle {
    id: root
    color: "transparent"
    border.color: "red"
    border.width: 1

    ScanRectHandle {
        anchors.centerIn: parent
        cursorShape: Qt.SizeAllCursor
        onPositionChanged: function(mouse) {
            theWindow.scanRect.x += mouse.x
            theWindow.scanRect.y += mouse.y
        }
    }

    ScanRectHandle {
        anchors.verticalCenter: parent.top
        anchors.horizontalCenter: parent.left
        cursorShape: Qt.SizeFDiagCursor
        onPositionChanged: function(mouse) {
            theWindow.scanRect.x += mouse.x
            theWindow.scanRect.width -= mouse.x
            theWindow.scanRect.y += mouse.y
            theWindow.scanRect.height -= mouse.y
        }
    }

    ScanRectHandle {
        anchors.verticalCenter: parent.top
        anchors.horizontalCenter: parent.right
        cursorShape: Qt.SizeBDiagCursor
        onPositionChanged: function(mouse) {
            theWindow.scanRect.width += mouse.x
            theWindow.scanRect.y += mouse.y
            theWindow.scanRect.height -= mouse.y
        }
    }

    ScanRectHandle {
        anchors.verticalCenter: parent.bottom
        anchors.horizontalCenter: parent.left
        cursorShape: Qt.SizeBDiagCursor
        onPositionChanged: function(mouse) {
            theWindow.scanRect.x += mouse.x
            theWindow.scanRect.width -= mouse.x
            theWindow.scanRect.height += mouse.y
        }
    }

    ScanRectHandle {
        anchors.verticalCenter: parent.bottom
        anchors.horizontalCenter: parent.right
        cursorShape: Qt.SizeFDiagCursor
        onPositionChanged: function(mouse) {
            theWindow.scanRect.width += mouse.x
            theWindow.scanRect.height += mouse.y
        }
    }
}
