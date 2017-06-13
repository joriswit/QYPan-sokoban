import QtQuick 2.0

Item {
    id: topBarRoot
    property string titleString: ""
    property string defaultButtonImage: "../../../images/previous_item_green.png"
    property string pressedButtonImage: "../../../images/previous_item_light_green.png"
    property string defaultColor: "#00cc00"
    signal backTagButtonClicked()
    TagButton {
        id: backTagButton
        width: parent.height * 0.65
        height: width
        anchors.left: parent.left
        anchors.leftMargin: horizontalSpacing
        anchors.verticalCenter: parent.verticalCenter
        pressTagSource: pressedButtonImage
        releaseTagSource: defaultButtonImage
        onClicked: {
            topBarRoot.backTagButtonClicked();
        }
    }
    Text {
        id: pageTitle
        text: qsTr(titleString)
        font.pointSize: 20
        color: defaultColor
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
        color: defaultColor
    }
}
