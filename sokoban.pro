TEMPLATE = app

QT += qml quick
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += src/main.cpp \
    src/mapmanager.cpp

RESOURCES += resource.qrc

# 这里当前目录指的是输出目录，而输出目录默认跟工程目录在同一个文件夹里
LIBS += -L../sokoban/lib -llua52

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    include/mapmanager.h \
    include/lua/lauxlib.h \
    include/lua/lua.h \
    include/lua/lualib.h
