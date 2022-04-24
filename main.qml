import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")


    Rectangle {
        id: rect
        width: 200
        height: width/2
        anchors.centerIn: parent
        color: "blue"
        Text {
            id: textRect
            text: qsTr("Custom text")
            color: "white"
            anchors.centerIn: parent
        }

    }
}
