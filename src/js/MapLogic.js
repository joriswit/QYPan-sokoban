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
        static_map_objects[i] = createCell(row, column, type, cell_type);
    }

    for(i = 0; i < dynamic_cells.length; i++){
        cell_type = dynamic_cells[i];
        row = Math.floor(i / map_info.column);
        column = i % map_info.column;
        dynamic_map_objects[i] = createCell(row, column, type, cell_type);
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

function createCell(row, column, map_type, cell_type){
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
        obj.mapType = map_type;
        obj.cellType = cell_type;
        return obj;
    }else{
        console.log("error loading cell component");
        console.log(component.errorString());
        return undefined;
    }
}































