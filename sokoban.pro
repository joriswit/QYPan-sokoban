TEMPLATE = app

QT += qml quick
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += src/main.cpp \
    src/mapmanager.cpp

RESOURCES += resource.qrc

LIBS += -LG:\qt\qml\androidtest\sokoban\lib -llualib

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    include/mapmanager.h
