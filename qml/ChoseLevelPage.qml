import QtQuick 2.0
import "compoent"

Rectangle {
    id: choseLevelRoot
    property string pageName: "choseLevelPage"
    property real horizontalSpacing: width * 0.03
    color: "#ccffcc"
    TopBar {
        id: topBar
        anchors.top: parent.top
        width: parent.width
        height: parent.width * 0.15
        onBackTagButtonClicked: {
            back();
        }
        titleString: qsTr("关卡选择")
    }
    Item {
        id: selfMakeLevelGridViewItem
        anchors.top: topBar.bottom
        anchors.topMargin: topBar.height * 0.8
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.7
        height: parent.height * 0.6
        visible: false
        GridView {
            id: selfMakeLevelGridView
            anchors.fill: parent
            clip: true
            cellWidth: width / 5
            cellHeight: cellWidth
            model: ListModel {}
            delegate: viewDelegate
        }
    }
    Item {
        id: classicLevelGridViewItem
        anchors.top: topBar.bottom
        anchors.topMargin: topBar.height * 0.8
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.7
        height: parent.height * 0.6
        visible: true
        GridView {
            id: classicLevelGridView
            anchors.fill: parent
            clip: true
            cellWidth: width / 5
            cellHeight: cellWidth
            model: ListModel {}
            delegate: viewDelegate
        }
    }
    Component {
        id: viewDelegate
        Item {
            width: classicLevelGridView.cellWidth
            height: width
            Rectangle {
                width: parent.width * 0.75
                height: width
                anchors.centerIn: parent
                //color: cellArea.pressed ? "#66cc66" : (levelState[index] ? "#66cc33" : "#99cc66")
                color: isLocked ? lockedColor : (cellArea.pressed ? pressedColor : defaultColor)
                Text {
                    text: index+1
                    color: "white"
                    font.pointSize: 18
                    anchors.centerIn: parent
                }
                MouseArea {
                    id: cellArea
                    enabled: !isLocked
                    anchors.fill: parent
                    onClicked: {
                        onCellAreaClicked(mapType, index);
                    }
                }
            }
        }
    }
    ButtonA {
        id: classicButton
        width: classicLevelGridViewItem.width * 0.35
        height: width * 0.45
        anchors.top: classicLevelGridViewItem.bottom
        anchors.topMargin: topBar.height * 0.8
        anchors.left: classicLevelGridViewItem.left
        anchors.leftMargin: horizontalSpacing * 1.5
        text: qsTr("经 典")
        textSize: 15
        defaultColor: "#00cc00"
        pressedColor: "#00cc00"
        Image {
            id: classicTriangleTag
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
            width: parent.height * 0.4
            height: width
            visible: true
            source: "../images/top_right_green_tag.png"
            fillMode: Image.PreserveAspectFit
        }
        onClicked: {
            onClassicButtonClicked();
        }
    }
    ButtonA {
        id: selfMakeButton
        width: classicLevelGridViewItem.width * 0.35
        height: width * 0.45
        anchors.top: classicLevelGridViewItem.bottom
        anchors.topMargin: topBar.height * 0.8
        anchors.right: classicLevelGridViewItem.right
        anchors.rightMargin: horizontalSpacing * 1.5
        text: qsTr("自 制")
        textSize: 15
        defaultColor: "#00cc00"
        pressedColor: "#00cc00"
        Image {
            id: selfMakeTriangleTag
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
            width: parent.height * 0.4
            height: width
            visible: false
            source: "../images/top_right_green_tag.png"
            fillMode: Image.PreserveAspectFit
        }
        onClicked: {
            onSelfMakeButtonClicked();
        }
    }
    Component.onCompleted: {
        init();
    }

    function init(){
        var max_classic_level = map_manager.max_classic_level;
        var max_self_make_level = map_manager.max_self_make_level;
        var i
        for(i = 0; i < max_classic_level; i++){
            classicLevelGridView.model.insert(i, {"isLocked" : false,
                                       "mapType" : 0,
                                       "lockedColor" : "#99cc66",
                                       "pressedColor" : "#66cc66",
                                       "defaultColor" : "#66cc33"});
        }
        for(i = 0; i < max_self_make_level; i++){
            selfMakeLevelGridView.model.insert(i, {"isLocked" : false,
                                           "mapType" : 1,
                                           "lockedColor" : "#9999cc",
                                           "pressedColor" : "#789900ff",
                                           "defaultColor" : "#9900cc"});
        }
    }

    function onCellAreaClicked(type, index){
        var level = index + 1;
        console.log("classic level[" + level + "]");
        stackView.push(Qt.resolvedUrl("LevelMap.qml"));
        var topPage = stackView.depth - 1;
        stackView.get(topPage).level = level;
        stackView.get(topPage).type = type;
        //classicLevelGridView.model.setProperty(index+1, "isLocked", false);
    }

    function back(){
        stackView.pop();
    }

    function onClassicButtonClicked(){
        if(classicTriangleTag.visible == false){
            selfMakeTriangleTag.visible = false;
            classicTriangleTag.visible = true;

            selfMakeButton.defaultColor = "#00cc00";
            selfMakeButton.pressedColor = "#00cc00";
            selfMakeTriangleTag.source = "../images/top_right_green_tag.png";

            classicButton.defaultColor = "#00cc00";
            classicButton.pressedColor = "#00cc00";
            classicTriangleTag.source = "../images/top_right_green_tag.png";

            topBar.pressedButtonImage = "../../images/previous_item_light_green.png";
            topBar.defaultButtonImage = "../../images/previous_item_green.png";

            topBar.defaultColor = "#00cc00";
            choseLevelRoot.color = "#ccffcc";

            classicLevelGridViewItem.visible = true;
            selfMakeLevelGridViewItem.visible = false;
        }
    }

    function onSelfMakeButtonClicked(){
        if(selfMakeTriangleTag.visible == false){
            classicTriangleTag.visible = false;
            selfMakeTriangleTag.visible = true;

            selfMakeButton.defaultColor = "#660066";
            selfMakeButton.pressedColor = "#660066";
            selfMakeTriangleTag.source = "../images/top_right_purple_tag.png";

            classicButton.defaultColor = "#660066";
            classicButton.pressedColor = "#660066";
            classicTriangleTag.source = "../images/top_right_purple_tag.png";

            topBar.pressedButtonImage = "../../images/previous_item_light_purple.png";
            topBar.defaultButtonImage = "../../images/previous_item_purple.png";

            topBar.defaultColor = "#660066";
            choseLevelRoot.color = "#cc99ff";

            selfMakeLevelGridViewItem.visible = true;
            classicLevelGridViewItem.visible = false;
        }
    }
}
