import QtQuick 2.0

Item {
    id: cellRoot
    property int mapType: 0
    property string path: ""
    property int pathLength: 0
    property int pathCount: 0
    property bool isKillMove: false
    property bool isPushBox: false
    property real newX: 0
    property real newY: 0
    property string cellType: '-'
    property var man: ["../../../images/green_man.png", "../../../images/purple_man.png"]
    property var box: ["../../../images/green_box.png", "../../../images/purple_box.png"]
    property var wall: ["../../../images/green_wall.png", "../../../images/purple_wall.png"]
    property var goal: ["../../../images/green_goal.png", "../../../images/purple_goal.png"]
    property var floor: ["../../../images/green_floor.png", "../../../images/purple_floor.png"]
    property var mark_border: ["../../../images/green_mark.png", "../../../images/purple_mark.png"]
    property var mark_box: ["../../../images/green_mark_box.png", "../../../images/purple_mark_box.png"]
    signal completedManMove(int row, int column, bool push_box)
    signal killManMove(int row, int column)
    signal moveBox(string directory)
    Image {
        id: cell
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        source: {
            if(cellType == '@') return man[type];
            else if(cellType == '$') return box[type];
            else if(cellType == '#') return wall[type];
            else if(cellType == '.') return goal[type];
            else if(cellType == 'm') return mark_border[type];
            else if(cellType == 'b') return mark_box[type];
            else return floor[type];
        }
    }
    NumberAnimation on x {
        id: animationX
        to: newX
        duration: 100
        running: false
        alwaysRunToEnd: true
        onStopped: {
            cellMove();
        }
    }
    NumberAnimation on y {
        id: animationY
        to: newY
        duration: 100
        running: false
        alwaysRunToEnd: true
        onStopped: {
            cellMove();
        }
    }

    function killMove(){
        isKillMove = true;
    }

    function init(){
        isKillMove = false;
        isPushBox = false;
        pathCount = 0;
    }

    function isMoving(){
        return animationX.running || animationY.running
    }

    function cellMove(){
        //console.log("count[" + pathCount + "]");
        //console.log("length[" + pathLength + "]");
        //console.log("is kill[" + isKillMove + "]");
        var row = Math.floor((cellRoot.y+0.5*cellRoot.width)/cellRoot.width);
        var column = Math.floor((cellRoot.x+0.5*cellRoot.width)/cellRoot.width);
        if(pathCount < pathLength && isKillMove == false){
            var direction = path[pathCount];
            var lower_direction = direction.toLowerCase();
            if(lower_direction == 'u'){
                if(direction == 'U'){
                    isPushBox = true;
                    moveBox("u");
                }
                newY = cellRoot.y - cellRoot.width;
                animationY.start();
            }else if(lower_direction == 'd'){
                if(direction == 'D'){
                    isPushBox = true;
                    moveBox("d");
                }
                newY = cellRoot.y + cellRoot.width;
                animationY.start();
            }else if(lower_direction == 'l'){
                if(direction == 'L'){
                    isPushBox = true;
                    moveBox("l");
                }
                newX = cellRoot.x - cellRoot.width;
                animationX.start();
            }else if(lower_direction == 'r'){
                if(direction == 'R'){
                    isPushBox = true;
                    moveBox("r");
                }
                newX = cellRoot.x + cellRoot.width;
                animationX.start();
            }
            pathCount += 1;
        }else{
            if(cellType == '@'){
                if(isKillMove == false){
                    completedManMove(row, column, isPushBox);
                }else{
                    killManMove(row, column);
                }
            }
        }
    }
}
