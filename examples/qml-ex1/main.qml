import QtQuick
import GraphEditor 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    GraphView{
        id: flowView
        width: parent.width
        height: parent.height
        anchors.margins: 20

    }
}
