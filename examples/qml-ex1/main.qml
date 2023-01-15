import QtQuick
import GraphEditor 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    GraphView{
        id: flowView
        width: 400
        height: 400
        anchors.margins: 20
        Ellipse {
               id: ellipse
               width: parent.width/3
               height: parent.height/3

               fillColor: "blue"
               borderColor: "red"
               borderWidth: 2
               anchors.margins: 50
               Text {
                  anchors.centerIn: parent
                  text: parent.fillColor
                  font.pixelSize: parent.width*0.2
               }
           }
    }

//    Ellipse {
//        id: ellipse
//        anchors.fill: parent

//        fillColor: "blue"
//        borderColor: "red"
//        borderWidth: 2
//        anchors.margins: 50
//        Text {
//           anchors.centerIn: parent
//           text: parent.fillColor
//           font.pixelSize: 36
//        }
//    }
}
