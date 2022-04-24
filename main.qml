import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Connections {
        target: ViewData

        function onXCordChanged(x) {
                textX.text = "X = " + x
            }

        function onYCordChanged(y) {
                textY.text = "Y = " + y
            }
    }


    Rectangle {
        id: textCord
        width: 300
        height: width/4
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        color: "white"
        Text {
            id: textX
            text: qsTr("None")
            color: "orange"
            font.pointSize: 18
            anchors.top: parent.top

        }
        Text {
            id: textY
            text: qsTr("None")
            color: "orange"
            font.pointSize: 18
            anchors.bottom: parent.bottom

        }

    }

}
