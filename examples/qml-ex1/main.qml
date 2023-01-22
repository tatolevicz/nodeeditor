import QtQuick
import GraphEditor 1.0

Window {
    width: 1000
    height: 1000
    visible: true
    title: qsTr("Hello World")
    Rectangle{
        id: parentBg
        anchors.fill: parent
        color: "black"
/*
       Rectangle{
            id: container
            anchors.fill: parent
            anchors.margins: 20
            color: "grey"
            //radius: 60
            clip: true  // there is an issue with the render and clip parent is not working
                        //uncomment the radius to see that
*/
            GraphView{
                id: flowView
//                fineGridColor: "blue"
//                coarseGridColor: "red"
//                backgroundColor: "yellow"
                width: parent.width
                height: parent.height
                TestNodeItem
                {
                    id: testRect
                    width: 100
                    height: 100
                    backgroundColor: "blue"
                    x: 0
                    y: 0
                     Rectangle
                    {
                        id: summy
                        width: 100
                        height: 100
                        color: "pink"
                    }
                }

            }
        }
//    }
}
