import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

import DrawTrack 1.0

Window {
    id: window
    width: 800
    height: 480
    visible: true
    title: qsTr("AgroSlave_V0")

    Connections {
        // описываем реакцию на испущенные сигналы из слоя ViewData
        target: ViewData

        // обновляем координаты на textview
        function onXCordChanged(x) {
            textX.text = "X = " + x
        }
        function onYCordChanged(y) {
            textY.text = "Y = " + y
        }

        //получаем обновленную траекторию для отображения в QML
        function onPathToQML(path) {
            drawtrack.updatePath(path)
        }

        //ролучаем список ключевых точек для отображения в QML
        function onKeyPointsToQML(keypoints) {
            drawtrack.updateKeyPoint(keypoints)
        }

    }

    Connections {
        // описываем реакцию на испущенные сигналы из итема drawtrack
        target: drawtrack

        function onReleaseCoordinate(point) {

            // передаем координаты ключевой точки в слой ViewData
            // который потом передаст их в автопилот, а тот
            // в свою очередь обработает их и добавит в конец списка
            // ключевых точек, и испустит сигнал с новым
            // списком ключевых точек, которые получит слой ViewData
            // и передаст их для отображения в QML drawtrack
            ViewData.addKeyPointFromQML(point)
        }
    }

    DrawTrack {
        id: drawtrack
        anchors.fill: parent
        anchors.centerIn:  parent

    }

    Button {//кнопка переключения центрирования
        id: swapbutton
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 0
        text: "Centered"
        onClicked: drawtrack.swapCentered()
    }

    Rectangle {//итем изменения масштаба
        id: zoomItem
        anchors.right:  parent.right
        anchors.top:  parent.top
        anchors.rightMargin: 10
        anchors.topMargin: 20
        color: "#ddffdd"
        width: 80
        height: 240

        Text {//текущая величина масштаба
            id: currentZoom
            anchors.centerIn: parent
            text: qsTr(drawtrack.getZoom())
        }

        Button {//приближение
            id: zoomIn
            anchors.top:  parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: parent.height/3
            text: "+"
            onClicked: {
                drawtrack.zoomIn()
                currentZoom.text = qsTr(drawtrack.getZoom())
            }
        }

        Button {//отдаление
            id: zoomOut
            anchors.bottom:  parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: parent.height/3
            text: "-"
            onClicked: {
                drawtrack.zoomOut()
                currentZoom.text = qsTr(drawtrack.getZoom())
            }
        }


    }

    Rectangle {//отображение текущих координат по X и Y
        id: textCord
        width: 250
        height: width/4
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        color: "#eeeeff"
        radius: 10
        property real leftMarg: 10
        Text {
            id: textX
            text: qsTr("None")
            color: "blue"
            font.pointSize: 14
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: parent.leftMarg
        }
        Text {
            id: textY
            text: qsTr("None")
            color: "blue"
            font.pointSize: 14
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: parent.leftMarg
        }

    }

    Button {// переключение реле
        id: btRestateRelay
        text: "ON|OFF Relay"
        width: textCord.width/2
        height: width/4
        anchors.top: textCord.bottom //btCreateListPoint.bottom
        anchors.topMargin: 20
        anchors.left: textCord.left //parent.horizontalCenter
        highlighted: true
        onClicked: {
            ViewData.slotCommandToSlave14(120)
        }
    }

}
