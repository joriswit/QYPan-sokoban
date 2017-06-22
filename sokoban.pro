TEMPLATE = app

QT += qml quick
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += src/cpp/main.cpp \
    src/cpp/mapmanager.cpp \
    src/cpp/navigatealgorithm.cpp

RESOURCES += resource.qrc

#LIBS += -L../sokoban/lib -llua52

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    include/mapmanager.h \
    include/lua/lauxlib.h \
    include/lua/lua.h \
    include/lua/lualib.h \
    include/navigatealgorithm.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS =
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
