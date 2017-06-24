#include "../../include/mapmanager.h"
#include "../../include/solverintent.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<MapManager>("MapManager", 1, 0, "MapManager");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("map_manager", new MapManager);
    engine.rootContext()->setContextProperty("solver_intent", new SolverIntent);
    engine.load(QUrl(QStringLiteral("qrc:/src/qml/main.qml")));

    return app.exec();
}
