import QtQuick 2.0
import "compoent"
import "../js/MapLogic.js" as MAP_LOGIC

Rectangle {
    id: levelMapRoot
    property string pageName: "levelMapPage"
    property real horizontalSpacing: width * 0.03
    property int level: 1
    property int type: 0
    property var backgroundColor: ["#ccffcc", "#99cc99ff"]
    property var topBarDefaultColor: ["#00cc00", "#660066"]
    property var topBarDefaultBackButtonImage: ["../../../images/previous_item_green.png", "../../../images/previous_item_purple.png"]
    property var topBarPressedBackButtonImage: ["../../../images/previous_item_light_green.png", "../../../images/previous_item_light_purple.png"]
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
        id: options
        anchors.bottom: parent.bottom
        width: parent.width
        height: width * 0.15
        Rectangle {
            id: dividingLine
            anchors.left: parent.left
            anchors.leftMargin: horizontalSpacing
            anchors.right: parent.right
            anchors.rightMargin: horizontalSpacing
            anchors.top: parent.top
            height: 2
            color: topBarDefaultColor[type]
        }
        ButtonA {
            id: solveButton
            width: height * 2
            height: parent.height
            text: qsTr("启动求解器")
            defaultColor: "#660066"
            pressedColor: "#990099"
            visible: solver_intent.isIntentAvailable()
            onClicked: {
                onSolverButtonClicked();
            }
        }
    }

    Item {
        id: centerArea
        anchors.top: topBar.bottom
        anchors.topMargin: topBar.height * 0.4
        anchors.left: parent.left
        anchors.leftMargin: horizontalSpacing
        anchors.right: parent.right
        anchors.rightMargin: horizontalSpacing
        anchors.bottom: options.top
        anchors.bottomMargin: topBar.height * 0.4
        Item {
            id: mapArea
            anchors.centerIn: parent
            MouseArea {
                id: touchArea
                anchors.fill: parent
                onClicked: {
                    onTouchArea(mouse.x, mouse.y);
                }
            }
        }
    }

    Component.onCompleted: {
        solver_intent.solutionFound.connect(onSolutionFound);
    }

    function onTouchArea(x, y){
        MAP_LOGIC.touchPosition(x, y);
    }

    function openMap(type, level){
        MAP_LOGIC.openMap(type, level);
    }

    function back(){
        MAP_LOGIC.clearAll();
        stackView.pop();
    }

    function onSolverButtonClicked(){
        solver_intent.callSolver(MAP_LOGIC.mapXsb());
    }

    function onSolutionFound(solution){
        // TODO handle box moves in solution
        MAP_LOGIC.manMove(solution);
    }
}
