import QtQuick 2.0
import "compoent"

Rectangle {
    id: choseLevelRoot
    property string pageName: "choseLevelPage"
    property real horizontalSpacing: width * 0.03
    color: "#ccffcc"
    Item {
        id: topBar
        anchors.top: parent.top
        width: parent.width
        height: parent.width * 0.15
        TagButton {
            id: backTagButton
            width: parent.height * 0.65
            height: width
            anchors.left: parent.left
            anchors.leftMargin: horizontalSpacing
            anchors.verticalCenter: parent.verticalCenter
            pressTagSource: "../../images/previous_item_light_green.png"
            releaseTagSource: "../../images/previous_item_green.png"
            onClicked: {
                onBackTagButtonClicked();
            }
        }
        Text {
            id: pageTitle
            text: qsTr("关卡选择")
            font.pointSize: 20
            color: "#00cc00"
            anchors.left: backTagButton.right
            anchors.verticalCenter: parent.verticalCenter
        }
        Rectangle {
            id: dividingLine
            anchors.left: parent.left
            anchors.leftMargin: horizontalSpacing
            anchors.right: parent.right
            anchors.rightMargin: horizontalSpacing
            anchors.bottom: parent.bottom
            height: 2
            color: "#00cc00"
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
                color: isLocked ? lockedColor : defaultColor
                Text {
                    text: index+1
                    color: "white"
                    font.pointSize: 18
                    anchors.centerIn: parent
                }
                MouseArea {
                    id: cellArea
                    //enabled: levelState[index] ? true : false
                    enabled: !isLocked
                    anchors.fill: parent
                    onPressed: parent.color = pressedColor
                    onReleased: parent.color = defaultColor
                    onClicked: {
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
        var i
        classicLevelGridView.model.insert(0, {"isLocked" : false,
                                       "lockedColor" : "#99cc66",
                                       "pressedColor" : "#66cc66",
                                       "defaultColor" : "#66cc33"});
        for(i = 1; i < 100; i++){
            classicLevelGridView.model.insert(i, {"isLocked" : true,
                                       "lockedColor" : "#99cc66",
                                       "pressedColor" : "#66cc66",
                                       "defaultColor" : "#66cc33"});
        }
        for(i = 0; i < 33; i++){
            selfMakeLevelGridView.model.insert(i, {"isLocked" : false,
                                           "lockedColor" : "#9999cc",
                                           "pressedColor" : "#789900ff",
                                           "defaultColor" : "#9900cc"});
        }
    }
    function onCellAreaClicked(index){
        var level = index + 1;
        console.log("level[" + level + "]")
        classicLevelGridView.model.setProperty(index+1, "isLocked", false);
    }
    function onBackTagButtonClicked(){
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

            backTagButton.pressTagSource = "../../images/previous_item_light_green.png";
            backTagButton.releaseTagSource = "../../images/previous_item_green.png";

            dividingLine.color = "#00cc00";
            pageTitle.color = "#00cc00";
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

            backTagButton.pressTagSource = "../../images/previous_item_light_purple.png";
            backTagButton.releaseTagSource = "../../images/previous_item_purple.png";

            dividingLine.color = "#660066";
            pageTitle.color = "#660066";
            choseLevelRoot.color = "#cc99ff";

            selfMakeLevelGridViewItem.visible = true;
            classicLevelGridViewItem.visible = false;
        }
    }
}
