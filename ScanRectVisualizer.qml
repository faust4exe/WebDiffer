import QtQuick 2.2
import QtQuick.Controls 2.15
import QtWebView 1.1
import QtQuick.Layouts 1.1

Rectangle {
    id: root
    color: "transparent"
    border.color: "red"
    border.width: 1

    Rectangle {
        color: "transparent"
        border.color: "green"
        border.width: 1
        anchors.fill: parent
        anchors.margins: 10
    }

    Rectangle {
        anchors.centerIn: parent
        color: "transparent"
        border.color: "green"
        border.width: 1
        height: width
        width: 20

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeAllCursor
            onPositionChanged: {
                theWindow.scanRect.x += mouse.x
                theWindow.scanRect.y += mouse.y
            }
        }
    }

    Rectangle {
        anchors.verticalCenter: parent.top
        anchors.horizontalCenter: parent.left
        color: "transparent"
        border.color: "green"
        border.width: 1
        height: width
        width: 20

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeFDiagCursor
            onPositionChanged: {
                theWindow.scanRect.x += mouse.x
                theWindow.scanRect.width -= mouse.x
                theWindow.scanRect.y += mouse.y
                theWindow.scanRect.height -= mouse.y
            }
        }
    }

    Rectangle {
        anchors.verticalCenter: parent.top
        anchors.horizontalCenter: parent.right
        color: "transparent"
        border.color: "green"
        border.width: 1
        height: width
        width: 20

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeBDiagCursor
            onPositionChanged: {
                theWindow.scanRect.width += mouse.x
                theWindow.scanRect.y += mouse.y
                theWindow.scanRect.height -= mouse.y
            }
        }
    }

    Rectangle {
        anchors.verticalCenter: parent.bottom
        anchors.horizontalCenter: parent.left
        color: "transparent"
        border.color: "green"
        border.width: 1
        height: width
        width: 20

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeBDiagCursor
            onPositionChanged: {
                theWindow.scanRect.x += mouse.x
                theWindow.scanRect.width -= mouse.x
                theWindow.scanRect.height += mouse.y
            }
        }
    }

    Rectangle {
        anchors.verticalCenter: parent.bottom
        anchors.horizontalCenter: parent.right
        color: "transparent"
        border.color: "green"
        border.width: 1
        height: width
        width: 20

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeFDiagCursor
            onPositionChanged: {
                theWindow.scanRect.width += mouse.x
                theWindow.scanRect.height += mouse.y
            }
        }
    }
}
