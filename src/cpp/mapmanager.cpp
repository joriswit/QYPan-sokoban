#include "../../include/mapmanager.h"
#include "../../include/navigatealgorithm.h"

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
      box_state_root_(NULL),
      max_classic_level_(0),
      max_self_make_level_(0),
      man_position_(0)
{
    //classic_maps_directory_ = QDir::currentPath() + "/config/levels/classic/";
    //self_make_maps_directory_ = QDir::currentPath() + "/config/levels/self_make/";
    classic_maps_directory_ = "assets:/config/levels/classic/";
    self_make_maps_directory_ = "assets:/config/levels/self_make/";
    //classic_maps_directory_ = "../sokoban/config/levels/classic/";
    //self_make_maps_directory_ = "../sokoban/config/levels/self_make/";
}

bool MapManager::loadMap(MapType type){
#if 0
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
#endif
    QFile file("assets:/config/mapinformation.txt");
    //QFile file("../sokoban/config/mapinformation.txt");
    if(!file.exists()){
        qDebug() << "file not exists";
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }
    QTextStream in(&file);
    QString line[5];
    int line_count = 0;
    while (!in.atEnd()) {
        line[line_count++] = in.readLine();
    }
    max_classic_level_ = line[0].toInt();
    max_self_make_level_ = line[1].toInt();

    qDebug() << "classic_levels[" << max_classic_level_ << "]";
    qDebug() << "self_make_levels[" << max_self_make_level_ << "]";

    if(type == CLASSIC){
        for(int i = 0; i < max_classic_level_; i++) loadMap(CLASSIC, i+1);
    }else if(type == SELF_MAKE){
        for(int i = 0; i < max_self_make_level_; i++) loadMap(SELF_MAKE, i+1);
    }else{
        //lua_close(L);
        return false;
    }

    //lua_close(L);
    file.close();

    return true;
}

QString MapManager::openMap(MapType type, int level){
    qDebug() << "open map in c++, type[" << type << "], level[" << level << "]";
    opened_map_info_ = (type == CLASSIC ? classic_maps_.at(level-1) : self_make_maps_.at(level-1));

    man_position_ = opened_map_info_.cells.indexOf(QChar('@'));
    if(man_position_ == -1){
        man_position_ = opened_map_info_.cells.indexOf(QChar('+'));
    }
    static_map_info_ = opened_map_info_;
    dynamic_map_info_ = opened_map_info_;
    static_map_info_.cells.replace(QRegExp("[@$]"), "-");
    static_map_info_.cells.replace(QRegExp("[+*]"), ".");
    dynamic_map_info_.cells.replace(QRegExp("[+@.]"), "-");
    dynamic_map_info_.cells.replace("*", "$");

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
    qDebug() << "set cell row[" << row << "],column[" << column << "],cell[" << type << "]";
}

void MapManager::clearAll(){
    qDebug() << "start clear in c++";
    freeMark();
    qDebug() << "end clear in c++";
}

void MapManager::freeMark(){
    if(box_state_root_ == NULL){
        qDebug() << "root is null";
    }else{
        qDebug() << "root is not null";
    }
    NavigateAlgorithm::freeBoxStateNodes(box_state_root_);
    box_state_root_ = NULL;
}

void MapManager::setManPosition(int position){
    man_position_ = position;
}

QString MapManager::getCell(int row, int column){
    int position = row * dynamic_map_info_.column + column;
    return QString(dynamic_map_info_.cells[position]);
}

QString MapManager::getPushPath(int row, int column){
    int position = row * dynamic_map_info_.column + column;
    QString push_path = NavigateAlgorithm::pushPath(box_state_root_, position);
    return push_path;
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
    QString mark_cells = NavigateAlgorithm::reachableCells(dynamic_map_info_, box_state_root_, man_position_, position);
    QJsonObject obj;
    obj.insert("type", "TOUCH_BOX");
    obj.insert("mark_cells", mark_cells);
    QJsonDocument document;
    document.setObject(obj);
    QByteArray bytes = document.toJson(QJsonDocument::Compact);
    return QString(bytes);
}

QString MapManager::touchFloor(int position){
    //qDebug() << "man position[" << man_position_ << "]";
    //qDebug() << "dynamic column[" << dynamic_map_info_.column << "]";
    qDebug() << "man row[" << man_position_ / dynamic_map_info_.column << "]";
    qDebug() << "man column[" << man_position_ % dynamic_map_info_.column << "]";
    //QString path = manPath(man_position, position);
    bool reachable = true;
    QString path = NavigateAlgorithm::manPath(dynamic_map_info_, man_position_, position, reachable);
    QJsonObject obj;
    obj.insert("type", "TOUCH_FLOOR");
    obj.insert("path", path);
    QJsonDocument document;
    document.setObject(obj);
    QByteArray bytes = document.toJson(QJsonDocument::Compact);
    return QString(bytes);
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
