#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "../include/mapmanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<MapManager>("MapManager", 1, 0, "MapManager");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("map_manager", new MapManager);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}
