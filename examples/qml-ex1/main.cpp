#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "ellipseitem.h"
#include "graphview.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<EllipseItem>("GraphEditor", 1, 0, "Ellipse");
    qmlRegisterType<GraphView>("GraphEditor", 1, 0, "GraphView");


    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qml-ex1/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
