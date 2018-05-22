#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <shphandler.h>
#include <snowboywatcher.h>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<SHPHandler>("io.qt.shphandler", 1, 0, "SHPHandler");
    qmlRegisterType<SnowboyWatcher>("io.qt.snowboywatcher", 1, 0, "SnowboyWatcher");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
