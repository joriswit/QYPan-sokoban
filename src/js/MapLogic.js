var map_info = null;
var man_position = null;
var touch_position = null;
var mark_box_object = null;
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
    if(cell == '+') return '@';
    else if(cell == '*') return '$';
    else if(cell == '.') return '-';
    else return cell;
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
    var dynamic_cells = map_info.cells.replace(/[+*.]/g, dynamicReplace);
    man_position = dynamic_cells.indexOf("@");

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
            obj.completedMove.connect(onCompletedMove);
        }

        return obj;
    }else{
        console.log("error loading cell component");
        console.log(component.errorString());
        return undefined;
    }
}

function onCompletedMove(type, row, column){
    var current_man_position = row * map_info.column + column;
    var man_cell = dynamic_map_objects[man_position];
    man_cell.init();

    dynamic_map_objects[current_man_position] = man_cell;
    dynamic_map_objects[man_position] = null;

    var old_man_row = Math.floor(man_position / map_info.column);
    var old_man_column = Math.floor(man_position % map_info.column);
    map_manager.setCell(old_man_row, old_man_column, "-");
    map_manager.setCell(row, column, "@");
    man_position = current_man_position;
    console.log("completed");
    console.log("old row[" + old_man_row + "],column[" + old_man_column + "]");
    console.log("current row[" + row + "],column[" + column + "]");
    if(touch_position != null){
        var next_row = Math.floor(touch_position / map_info.column);
        var next_column = Math.floor(touch_position % map_info.column);
        console.log("next row[" + next_row + "],column[" + next_column + "]");
        if(next_row != row || next_column != column){
            responseTouch(next_row, next_column);
        }
    }
}

function touchPosition(x, y){
    var row = Math.floor(y / map_info.cell_size);
    var column = Math.floor(x / map_info.cell_size);
    console.log("touch row[" + row + "]");
    console.log("touch column[" + column + "]");

    touch_position = row * map_info.column + column;
    var touch_cell = dynamic_map_objects[touch_position];
    if(!(touch_cell != undefined && (touch_cell.cellType == "#" || touch_cell.cellType == "$"))){
        touchFloor(row, column);
    }else if(touch_cell != undefined && touch_cell.cellType == "$"){
        touchBox(row, column);
    }
}

function touchBox(row, column){
    console.log("touch box");
    if(mark_box_object != null){
        mark_box_object.destroy();
    }
    mark_box_object = createCell(row, column, 'm');
    responseTouch(row, column);
}

function touchFloor(row, column){
    if(mark_box_object != null){
        mark_box_object.destroy();
        mark_box_object = null;
        return;
    }
    var man_cell = dynamic_map_objects[man_position];
    if(man_cell.isMoving()){
        console.log("is moving");
        man_cell.killMove();
    }else{
        touch_position = row * map_info.column + column;
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
                manMove(result.man_position, result.end_position, result.path);
            }
        }else if(result.type == "TOUCH_BOX"){
        }
    }
}

function manMove(start_position, end_position, path){
    var cell = dynamic_map_objects[start_position];
    cell.path = path;
    cell.pathLength = path.length;
    cell.pathCount = 0;
    cell.cellMove();
}































