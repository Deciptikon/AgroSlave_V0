import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("AgroSlave_V0")

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
        color: "#eeeeff"
        radius: 10
        property real leftMarg: 10
        Text {
            id: textX
            text: qsTr("None")
            color: "orange"
            font.pointSize: 18
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: parent.leftMarg
        }
        Text {
            id: textY
            text: qsTr("None")
            color: "orange"
            font.pointSize: 18
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: parent.leftMarg
        }

    }

    Button {
        id: btCreateListPoint
        text: "Create List Point"
        width: 300
        height: width/4
        anchors.top: textCord.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        highlighted: true
        onClicked: {
            ViewData.slotCreateListPoint()
        }
    }

    Button {
        id: btRestateRelay
        text: "ON|OFF Relay"
        width: 300
        height: width/4
        anchors.top: btCreateListPoint.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        highlighted: true
        onClicked: {
            ViewData.slotCommandToSlave14(120)
        }
    }

}
