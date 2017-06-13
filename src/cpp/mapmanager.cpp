#include "../../include/mapmanager.h"

#include <QFile>
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
    QJsonObject obj;
    MapInfo map_info = (type == CLASSIC ? classic_maps_.at(level-1) : self_make_maps_.at(level-1));
    obj.insert("row", map_info.row);
    obj.insert("column", map_info.column);
    obj.insert("cells", map_info.cells);
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
