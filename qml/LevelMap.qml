import QtQuick 2.0
import "compoent"

Rectangle {
    id: levelMapRoot
    property string pageName: "levelMapPage"
    property real horizontalSpacing: width * 0.03
    property int level: 1
    property int type: 0
    property var backgroundColor: ["#ccffcc", "#cc99ff"]
    property var topBarDefaultColor: ["#00cc00", "#660066"]
    property var topBarDefaultBackButtonImage: ["../../images/previous_item_green.png", "../../images/previous_item_purple.png"]
    property var topBarPressedBackButtonImage: ["../../images/previous_item_light_green.png", "../../images/previous_item_light_purple.png"]
    color: backgroundColor[type]
    TopBar {
        id: topBar
        anchors.top: parent.top
        width: parent.width
        height: parent.width * 0.15
        titleString: qsTr("关卡 ") + level.toString()
        defaultColor: topBarDefaultColor[type]
        defaultButtonImage: topBarDefaultBackButtonImage[type]
        pressedButtonImage: topBarPressedBackButtonImage[type]
        onBackTagButtonClicked: {
            back();
        }
    }
    Item {
        id: selfMakeLevelGridViewItem
        anchors.top: topBar.bottom
        anchors.topMargin: topBar.height * 0.8
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.7
        height: parent.height * 0.6
        visible: false
    }

    function back(){
        stackView.pop();
    }
}
