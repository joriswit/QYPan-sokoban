var map_info = null;
var man_object = null;
var start_man_position = null;
var start_box_position = null;
var start_box_position = null;
var end_box_position = null;
var touch_position = null;
var box_border_object = null;
var mark_box_objects = null;
var component = null;
var static_map_objects = null;
var dynamic_map_objects = null;

function alertObj(obj){
    var output = "";
    for(var i in obj){
        var property=obj[i];
        output+=i+" = "+property+"\n";
    }
    console.log(output);
}

function staticReplace(){
    var cell = arguments[0];
    if(cell == '@') return '-';
    else if(cell == '+') return '.';
    else if(cell == '$') return '-';
    else if(cell == '*') return '.';
    else return cell;
}

function dynamicReplace(){
    var cell = arguments[0];
    if(cell == '@') return '-';
    if(cell == '+') return '-';
    else if(cell == '*') return '$';
    else if(cell == '.') return '-';
    else return cell;
}

function createManObject(){
    var man_position = map_info.cells.indexOf("@");
    if(man_position == -1){
        man_position = map_info.cells.indexOf("+");
    }
    start_man_position = man_position;
    var row = Math.floor(man_position / map_info.column);
    var column = man_position % map_info.column;
    man_object = createCell(row, column, '@');
}

function openMap(type, level) {
    console.log("open map type[" + type + "], level[" + level + "]");
    var string_map_info = map_manager.openMap(type, level);
    console.log(string_map_info);
    map_info = JSON.parse(string_map_info);
    map_info.type = type;

    adjustMapSize();

    if(static_map_objects != null) static_map_objects = null;
    if(dynamic_map_objects != null) dynamic_map_objects = null;
    static_map_objects = new Array(map_info.row * map_info.column);
    dynamic_map_objects = new Array(map_info.row * map_info.column);

    var static_cells = map_info.cells.replace(/[@+$*]/g, staticReplace);
    var dynamic_cells = map_info.cells.replace(/[@+*.]/g, dynamicReplace);

    var i, cell_type, row, column;
    for(i = 0; i < static_cells.length; i++){
        cell_type = static_cells[i];
        row = Math.floor(i / map_info.column);
        column = i % map_info.column;
        static_map_objects[i] = createCell(row, column, cell_type);
    }

    for(i = 0; i < dynamic_cells.length; i++){
        cell_type = dynamic_cells[i];
        if(cell_type == '-'){
            continue;
        }
        row = Math.floor(i / map_info.column);
        column = i % map_info.column;
        dynamic_map_objects[i] = createCell(row, column, cell_type);
    }
    createManObject();
}

function adjustMapSize(){
    var map_height = centerArea.height;
    var cell_size = map_height / map_info.row;
    var map_width = cell_size * map_info.column;
    if(map_width > centerArea.width){
        map_width = centerArea.width
        cell_size = map_width / map_info.column;
        map_height = cell_size * map_info.row;
    }
    map_info.cell_size = cell_size;
    mapArea.width = map_width;
    mapArea.height = map_height;
}

function createCell(row, column, cell_type){
    if(component == null){
        component = Qt.createComponent("../qml/compoent/Cell.qml");
    }

    if(component.status == Component.Ready){
        var obj = component.createObject(mapArea);
        if (obj == null) {
            console.log("error creating cell");
            console.log(component.errorString());
            return undefined;
        }
        obj.width = map_info.cell_size;
        obj.height = obj.width;
        obj.x = map_info.cell_size * column;
        obj.y = map_info.cell_size * row;
        obj.mapType = map_info.type;
        obj.cellType = cell_type;
        if(cell_type == "@"){
            obj.completedManMove.connect(onCompletedManMove);
            obj.killManMove.connect(onKillManMove);
            obj.moveBox.connect(onMoveBox);
        }

        return obj;
    }else{
        console.log("error loading cell component");
        console.log(component.errorString());
        return undefined;
    }
}

function onMoveBox(directory){
    cellMove(start_box_position, directory);
}

function onCompletedManMove(row, column, push_box){
    if(push_box == true){
        var box_row = Math.floor(end_box_position / map_info.column);
        var box_column = end_box_position % map_info.column;
        console.log("end box row[" + box_row + "],column[" + box_column + "]");
        completedMove('$', box_row, box_column);
    }
    completedMove('@', row, column);
}

function onKillManMove(row, column){
    completedMove('@', row, column);
    if(touch_position != null){
        var next_row = Math.floor(touch_position / map_info.column);
        var next_column = touch_position % map_info.column;
        console.log("next row[" + next_row + "],column[" + next_column + "]");
        if(next_row != row || next_column != column){
            var cell = map_manager.getCell(next_row, next_column);
            if(cell == "-"){
                responseTouch(next_row, next_column);
            }
        }
    }
}

function completedMove(type, row, column){
    console.log(type + " completed move on row[" + row + "],column[" + column + "]");
    var new_position = row * map_info.column + column;
    var old_position;
    if(type == '@'){
        man_object.init();
        old_position = start_man_position;
        start_man_position = new_position;
        map_manager.setManPosition(new_position);
    }else if(type == '$'){
        var cell = dynamic_map_objects[start_box_position];
        old_position = start_box_position;
        dynamic_map_objects[new_position] = cell;
        dynamic_map_objects[old_position] = undefined;

        var old_row = Math.floor(old_position / map_info.column);
        var old_column = old_position % map_info.column;
        console.log(type + " old row[" + old_row + "],column[" + old_column + "]");
        map_manager.setCell(old_row, old_column, "-");
        map_manager.setCell(row, column, type);
    }
}

function touchPosition(x, y){
    var row = Math.floor(y / map_info.cell_size);
    var column = Math.floor(x / map_info.cell_size);
    console.log("touch row[" + row + "]");
    console.log("touch column[" + column + "]");

    touch_position = row * map_info.column + column;
    var touch_cell = dynamic_map_objects[touch_position];
    if(touch_cell != undefined){
        console.log("touch type[" + touch_cell.cellType + "]");
    }
    var t = map_manager.getCell(row, column);
    console.log("c++ touch type[" + t + "]");

    if(hasBorder() == true){
        console.log("is marked");
        deleteBorder();
        if(mark_box_objects != null){
            var mark_cell = mark_box_objects[touch_position];
            var kill_touch = false;
            if(mark_cell == undefined){ // 除了标记的可达点以外的所有触点
                map_manager.freeMark();
                if(!(touch_cell != undefined && touch_cell.cellType == "$")){
                    kill_touch = true;
                }
            }else{ // 标记的可达点
                kill_touch = true;
                if(!(touch_cell != undefined && touch_cell.cellType == "$")){
                    pushBox(touch_position);
                }else{
                    map_manager.freeMark();
                }
            }
            clearMarkBox();
            if(kill_touch == true){
                return;
            }
        }
    }

    if(!(touch_cell != undefined && (touch_cell.cellType == "#" || touch_cell.cellType == "$"))){
        touchFloor(row, column);
    }else if(touch_cell != undefined && touch_cell.cellType == "$"){
        touchBox(row, column);
    }
}

function pushBox(position){
    console.log("push box");
    end_box_position = position;
    var row = Math.floor(position / map_info.column);
    var column = position % map_info.column;
    var path = map_manager.getPushPath(row, column);
    console.log("path[" + path + "]");
    manMove(path);
}

function touchBox(row, column){
    console.log("touch box");
    if(man_object.isMoving()){
        console.log("is moving");
        return;
    }

    deleteBorder();
    makeBorder(row, column);
    start_box_position = row * map_info.column + column;
    responseTouch(row, column);
}

function hasBorder(){
    if(box_border_object != null){
        return true;
    }
    return false;
}

function makeBorder(row, column){
    box_border_object = createCell(row, column, 'm');
}

function deleteBorder(){
    if(box_border_object != null){
        box_border_object.destroy();
    }
    box_border_object = null;
}

function touchFloor(row, column){
    if(man_object.isMoving()){
        console.log("is moving");
        man_object.killMove();
    }else{
        responseTouch(row, column);
    }
}

function responseTouch(row, column){
    var string_result = map_manager.touchPosition(row, column);
    if(string_result != ""){
        var result = JSON.parse(string_result);
        if(result.type == "TOUCH_FLOOR"){
            console.log("type[" + result.type + "]");
            console.log("path[" + result.path + "]");
            if(result.path.length > 0){
                manMove(result.path);
            }
        }else if(result.type == "TOUCH_BOX"){
            console.log("type[" + result.type + "]");
            markBox(result.mark_cells);
        }
    }
}

function clearMarkBox(){
    if(mark_box_objects == null){
        return;
    }
    var i;
    for(i = 0; i < mark_box_objects.length; i++){
        if(mark_box_objects[i] != undefined){
            mark_box_objects[i].destroy();
        }
    }
    mark_box_objects = null;
}

function markBox(mark_cells){
    mark_box_objects = new Array(mark_cells.length);
    var i;
    for(i = 0; i < mark_cells.length; i++){
        if(mark_cells[i] == '1'){
            var row = Math.floor(i / map_info.column);
            var column = i % map_info.column;
            mark_box_objects[i] = createCell(row, column, 'b');
        }
    }
}

function manMove(path){
    man_object.path = path;
    man_object.pathLength = path.length;
    man_object.pathCount = 0;
    man_object.cellMove();
}

function cellMove(start_position, path){
    var cell = dynamic_map_objects[start_position];
    cell.path = path;
    cell.pathLength = path.length;
    cell.pathCount = 0;
    cell.cellMove();
}































