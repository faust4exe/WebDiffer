import QtQuick 2.2
import Qt5Compat.GraphicalEffects

Image {
    id: root

    signal positionChanged(var mouse)

    height: width
    width: 20

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        cursorShape: pressed ? Qt.ClosedHandCursor : Qt.OpenHandCursor

        onPositionChanged: function(mouse) {
            root.positionChanged(mouse)
        }
    }

    layer.enabled: true
    layer.effect: DropShadow {
        cached: true
        color: "white"
        spread: 0.8
    }
}
