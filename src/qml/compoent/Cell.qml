import QtQuick 2.0

Item {
    id: cellRoot
    property int mapType: 0
    property string cellType: '-'
    property var man: ["../../../images/green_man.png", "../../../images/purple_man.png"]
    property var box: ["../../../images/green_box.png", "../../../images/purple_box.png"]
    property var wall: ["../../../images/green_wall.png", "../../../images/purple_wall.png"]
    property var goal: ["../../../images/green_goal.png", "../../../images/purple_goal.png"]
    property var floor: ["../../../images/green_floor.png", "../../../images/purple_floor.png"]
    Image {
        id: cell
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        source: {
            if(cellType == '@') return man[type];
            else if(cellType == '$') return box[type];
            else if(cellType == '#') return wall[type];
            else if(cellType == '.') return goal[type];
            else return floor[type];
        }
    }
}
