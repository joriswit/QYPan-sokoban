import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Window 2.2
import MapManager 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 376
    height: 670
    title: qsTr("sokoban")

    StackView {
        id: stackView // 实现翻页
        anchors.fill: parent
        focus: true
        Keys.onReleased: {
            if (event.key === Qt.Key_Back) {
                /*
                var top = stackView.depth - 1;
                var topPage = stackView.get(top);
                if(stackView.depth > 1){
                    if(topPage.pageName === "talkPage"){
                        signalManager.stackPop();
                    }else{
                        stackView.pop();
                    }
                    event.accepted = true;
                }else if(topPage.pageName === "mainTabPage"){
                    stackView.get(top).quit();
                    event.accepted = true;
                }
                */
            }
        }
    }

    Component.onCompleted: {
        stackView.push(Qt.resolvedUrl("/qml/TopPage.qml"));
        map_manager.loadMap(MapManager.CLASSIC);
        map_manager.loadMap(MapManager.SELF_MAKE);
    }
}
