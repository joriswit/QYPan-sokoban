var map_info = null;
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
    var dynamic_cells = map_info.cells.replace(/[+*]/g, dynamicReplace);

    var i, cell_type, row, column;
    for(i = 0; i < static_cells.length; i++){
        cell_type = static_cells[i];
        row = Math.floor(i / map_info.column);
        column = i % map_info.column;
        static_map_objects[i] = createCell(row, column, cell_type);
    }

    for(i = 0; i < dynamic_cells.length; i++){
        cell_type = dynamic_cells[i];
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
        return obj;
    }else{
        console.log("error loading cell component");
        console.log(component.errorString());
        return undefined;
    }
}

function touchPosition(x, y){
    var row = Math.floor(y / map_info.cell_size);
    var column = Math.floor(x / map_info.cell_size);
    console.log("touch row[" + row + "]");
    console.log("touch column[" + column + "]");
    var string_result = map_manager.touchPosition(row, column);
    var result = JSON.parse(string_result);
    if(result.type == "MAN_MOVE"){
        console.log("type[" + result.type + "]");
        console.log("path[" + result.path + "]");
        manMove(result.path);
    }
}

function manMove(path){
    var man_position = map_info.cells.indexOf("@");
    var i, row, column;
    var current_postion = man_position;
    for(i = 0; i < path.length; i++){
        row = Math.floor(current_postion / map_info.column);
        column = current_postion % map_info.column;
        dynamic_map_objects[current_postion] = undefined;
        if(path[i] == 'u'){
            row = row - 1;
        }else if(path[i] == 'd'){
            row = row + 1;
        }else if(path[i] == 'l'){
            column = column - 1;
        }else if(path[i] == 'r'){
            column = column + 1;
        }
        current_postion = row * map_info.column + column;
        dynamic_map_objects[i] = createCell(row, column, '@');
    }
}































