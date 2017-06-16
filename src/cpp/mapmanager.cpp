#include "../../include/mapmanager.h"

#include <QFile>
#include <QChar>
#include <QQueue>
#include <QRegExp>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <iostream>
using namespace std;

extern "C"
{
#include "../../include/lua/lua.h"
#include "../../include/lua/lauxlib.h"
#include "../../include/lua/lualib.h"
}

MapManager::MapManager(QObject *parent)
    : QObject(parent),
      max_classic_level_(0),
      max_self_make_level_(0)
{
    //classic_maps_directory_ = QDir::currentPath() + "/config/levels/classic/";
    //self_make_maps_directory_ = QDir::currentPath() + "/config/levels/self_make/";
    classic_maps_directory_ = "../sokoban/config/levels/classic/";
    self_make_maps_directory_ = "../sokoban/config/levels/self_make/";
}

bool MapManager::loadMap(MapType type){
    lua_State *L = luaL_newstate();
    if(!L){
        qDebug() << "lua: new state fault";
        return false;
    }

    //luaL_openlibs(L);

    int error = luaL_dofile(L, "../sokoban/config/mapinformation.lua");
    if(error){
        qDebug() << "lua: do file fault";
        return false;
    }

    lua_getglobal(L, "classic_levels");
    lua_getglobal(L, "self_make_levels");

    int t1 = lua_type(L, -1);
    int t2 = lua_type(L, -2);
    qDebug() << "lua type[" << t1 << "]";
    qDebug() << "lua type[" << t2 << "]";

    if(!lua_isnumber(L, -1) || !lua_isnumber(L, -2)){
        qDebug() << "lua: bad data type";
        return false;
    }

    max_classic_level_ = lua_tointeger(L, -2);
    max_self_make_level_= lua_tointeger(L, -1);

    qDebug() << "classic_levels[" << max_classic_level_ << "]";
    qDebug() << "self_make_levels[" << max_self_make_level_ << "]";

    if(type == CLASSIC){
        for(int i = 0; i < max_classic_level_; i++) loadMap(CLASSIC, i+1);
    }else if(type == SELF_MAKE){
        for(int i = 0; i < max_self_make_level_; i++) loadMap(SELF_MAKE, i+1);
    }else{
        lua_close(L);
        return false;
    }

    lua_close(L);

    return true;
}

QString MapManager::openMap(MapType type, int level){
    qDebug() << "open map in c++, type[" << type << "], level[" << level << "]";
    opened_map_info_ = (type == CLASSIC ? classic_maps_.at(level-1) : self_make_maps_.at(level-1));

    static_map_info_ = opened_map_info_;
    dynamic_map_info_ = opened_map_info_;
    static_map_info_.cells.replace(QRegExp("[@$]"), "-");
    static_map_info_.cells.replace(QRegExp("[+*]"), ".");
    dynamic_map_info_.cells.replace("+", "@");
    dynamic_map_info_.cells.replace("*", "$");
    dynamic_map_info_.cells.replace(".", "-");

    qDebug() << "c++ static map";
    qDebug() << static_map_info_.cells;
    qDebug() << "c++ dynamic map";
    qDebug() << dynamic_map_info_.cells;

    QJsonObject obj;
    obj.insert("row", opened_map_info_.row);
    obj.insert("column", opened_map_info_.column);
    obj.insert("cells", opened_map_info_.cells);
    QJsonDocument document;
    document.setObject(obj);
    QByteArray bytes = document.toJson(QJsonDocument::Compact);
    return QString(bytes);
}

bool MapManager::loadMap(MapType type, int level){
    if(type != CLASSIC && type != SELF_MAKE){
        return false;
    }else{
        char cstr_level[10] = {0};
        sprintf(cstr_level, "%d", level);
        QString qstr_level(cstr_level);
        QString suffix = ".txt";

        if(type == CLASSIC){
            if(level > max_classic_level_){
                return false;
            }else{
                return loadMap(type, classic_maps_directory_ + qstr_level + suffix);
            }
        }else if(type == SELF_MAKE){
            if(level > max_self_make_level_){
                return false;
            }else{
                return loadMap(type, self_make_maps_directory_ + qstr_level + suffix);
            }
        }else{
            return false;
        }
    }
}

bool MapManager::loadMap(MapType type, const QString &path){
    qDebug() << path;
    if(path.isEmpty()){
        return false;
    }

    QFile file(path);
    if(!file.exists()){
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }
    QTextStream in(&file);
    MapInfo map_info;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if(map_info.column == 0){
            map_info.column = line.length();
        }
        map_info.row += 1;
        map_info.cells += line;
    }

    qDebug() << "type[" << type << "]";
    qDebug() << "row[" << map_info.row << "]";
    qDebug() << "column[" << map_info.column << "]";
    qDebug() << "cells[" << map_info.cells << "]";

    if(type == CLASSIC){
        classic_maps_.append(map_info);
    }else if(type == SELF_MAKE){
        self_make_maps_.append(map_info);
    }
    file.close();
    return true;
}

void MapManager::setCell(int row, int column, const QString &type){
    if(row < 0 || row >= dynamic_map_info_.row || column < 0 || column >= dynamic_map_info_.column){
        return;
    }
    if(type.isEmpty()){
        return;
    }
    QChar cell = type[0];
    dynamic_map_info_.cells[row*dynamic_map_info_.column+column] = cell;
}

QString MapManager::touchPosition(int row, int column){
    if(row < 0 || row >= dynamic_map_info_.row || column < 0 || column >= dynamic_map_info_.column){
        return "";
    }
    int position = row * dynamic_map_info_.column + column;
    QChar cell = dynamic_map_info_.cells[position];
    if(cell == QChar('-')){
        return touchFloor(position);
    }else if(cell == QChar('$')){
        return touchBox(position);
    }else{
        return "";
    }
}

QString MapManager::touchBox(int position){
    return "";
}

QString MapManager::touchFloor(int position){
    int man_position = dynamic_map_info_.cells.indexOf(QChar('@'));
    qDebug() << "man row[" << man_position / dynamic_map_info_.column << "]";
    qDebug() << "man column[" << man_position % dynamic_map_info_.column << "]";
    QString path = manPath(man_position, position);
    QJsonObject obj;
    obj.insert("type", "TOUCH_FLOOR");
    obj.insert("man_position", man_position);
    obj.insert("end_position", position);
    obj.insert("path", path);
    QJsonDocument document;
    document.setObject(obj);
    QByteArray bytes = document.toJson(QJsonDocument::Compact);
    return QString(bytes);
}

QString MapManager::manPath(int from, int to){
    int go_x[4] = {0, 0, -1, 1};
    int go_y[4] = {-1, 1, 0, 0};
    char direction[5] = {"udlr"};

    struct PositionState{
        PositionState() : row(0), column(0), path("") {}
        int row, column;
        QString path;
    };

    int max_row = dynamic_map_info_.row;
    int max_column = dynamic_map_info_.column;

    QQueue<PositionState> queue;
    QVector<bool> mark(dynamic_map_info_.cells.length(), false);
    mark[from] = true;
    PositionState from_state;
    from_state.row = from / dynamic_map_info_.column;
    from_state.column = from % dynamic_map_info_.column;
    queue.enqueue(from_state);
    while(!queue.isEmpty()){
        PositionState current_state = queue.dequeue();
        int position = current_state.row * max_column + current_state.column;
        if(position == to){
            return current_state.path;
        }
        for(int i = 0; i < 4; i++){
            PositionState new_state;
            new_state.row = current_state.row + go_y[i];
            new_state.column = current_state.column + go_x[i];
            //qDebug() << "new row[" << new_state.row << "]";
            //qDebug() << "new column[" << new_state.column << "]";
            int new_position = new_state.row * max_column + new_state.column;
            if(new_state.row < 0 || new_state.row >= max_row || new_state.column < 0 || new_state.column >= max_column){
                continue;
            }
            if(dynamic_map_info_.cells[new_position] == QChar('#') ||
                    dynamic_map_info_.cells[new_position] == QChar('$')){
                continue;
            }
            if(mark[new_position] == true){
                continue;
            }
            mark[new_position] = true;
            new_state.path = current_state.path + direction[i];
            queue.enqueue(new_state);
        }
    }
    return "";
}

int MapManager::maxClassicLevel() const{
    return max_classic_level_;
}

int MapManager::maxSelfMakeLevel() const{
    return max_self_make_level_;
}

void MapManager::setMaxClassicLevel(int level){
    max_classic_level_ = level;
}

void MapManager::setMaxSelfMakeLevel(int level){
    max_self_make_level_ = level;
}
