import QtQuick 2.0

Rectangle {
    id: root
    antialiasing: true
    radius: 4
    color: "transparent"
    property string pressTagSource: ""
    property string releaseTagSource: ""
    signal clicked()
    Image {
        anchors.fill: parent
        source: tagMouse.pressed ? pressTagSource : releaseTagSource
        fillMode: Image.PreserveAspectFit
    }
    MouseArea {
        id: tagMouse
        anchors.fill: parent
        //anchors.margins: -10
        onClicked:{
            root.clicked();
        }
    }
}
