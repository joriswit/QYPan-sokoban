import QtQuick 2.0
import "compoent"

Rectangle {
    id: topPageRoot
    property string pageName: "topPage"
    color: "#cc99ff"
    Image {
        id: topPageLogo
        width: parent.width * 0.4
        height: width
        source: "../images/man_push_purple.png"
        fillMode: Image.PreserveAspectFit
        anchors.top: parent.top
        anchors.topMargin: parent.height * 0.25
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Column {
        id: topPageButtonColumn
        property real columnSpacing: parent.width * 0.08
        anchors.top: topPageLogo.bottom
        anchors.topMargin: columnSpacing
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: columnSpacing
        ButtonA {
            id: beginButton
            width: topPageLogo.width
            height: width * 0.35
            text: qsTr("开 始")
            defaultColor: "#660066"
            pressedColor: "#990099"
            onClicked: {
                onBeginButtonClicked();
            }
        }
        ButtonA {
            id: makeButton
            width: topPageLogo.width
            height: width * 0.35
            text: qsTr("制 作")
            defaultColor: "#660066"
            pressedColor: "#990099"
            onClicked: {
                onMakeButtonClicked();
            }
        }
    }
    function onBeginButtonClicked(){
        stackView.push(Qt.resolvedUrl("ChoseLevelPage.qml"));
    }
    function onMakeButtonClicked(){
    }
}
