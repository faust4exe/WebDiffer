import QtQuick 2.2

Rectangle {
    id: root
    color: "transparent"
    border.color: "black"
    border.width: 1

    Rectangle {
        anchors.fill: parent
        anchors.margins: 1
        color: "transparent"
        border.color: "white"
        border.width: 1
    }

    ScanRectHandle {
        anchors.centerIn: parent
        source: "qrc:/images/move.svg"
        onPositionChanged: function(mouse) {
            theWindow.scanRect.x += mouse.x
            theWindow.scanRect.y += mouse.y
        }
    }

    ScanRectHandle {
        anchors.verticalCenter: parent.top
        anchors.horizontalCenter: parent.left
        source: "qrc:/images/resizeF.svg"
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
        source: "qrc:/images/resizeB.svg"
        onPositionChanged: function(mouse) {
            theWindow.scanRect.width += mouse.x
            theWindow.scanRect.y += mouse.y
            theWindow.scanRect.height -= mouse.y
        }
    }

    ScanRectHandle {
        anchors.verticalCenter: parent.bottom
        anchors.horizontalCenter: parent.left
        source: "qrc:/images/resizeB.svg"
        onPositionChanged: function(mouse) {
            theWindow.scanRect.x += mouse.x
            theWindow.scanRect.width -= mouse.x
            theWindow.scanRect.height += mouse.y
        }
    }

    ScanRectHandle {
        anchors.verticalCenter: parent.bottom
        anchors.horizontalCenter: parent.right
        source: "qrc:/images/resizeF.svg"
        onPositionChanged: function(mouse) {
            theWindow.scanRect.width += mouse.x
            theWindow.scanRect.height += mouse.y
        }
    }
}
