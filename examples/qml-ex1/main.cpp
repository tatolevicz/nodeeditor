//#include <QGuiApplication>
//#include <QQmlApplicationEngine>

//#include "ellipseitem.h"
//#include "graphview.h"


//int main(int argc, char *argv[])
//{
//    QGuiApplication app(argc, argv);

//    qmlRegisterType<EllipseItem>("GraphEditor", 1, 0, "Ellipse");
//    qmlRegisterType<GraphView>("GraphEditor", 1, 0, "GraphView");


//    QQmlApplicationEngine engine;
//    const QUrl url(u"qrc:/qml-ex1/main.qml"_qs);
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                     &app, [url](QObject *obj, const QUrl &objUrl) {
//        if (!obj && url == objUrl)
//            QCoreApplication::exit(-1);
//    }, Qt::QueuedConnection);
//    engine.load(url);

//    return app.exec();
//}
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtNodes/QmlBasicGraphicsScene>
#include <QtNodes/ConnectionStyle>
#include <QtNodes/QmlGraphicsView>
#include <QtNodes/StyleCollection>

#include <QAction>
#include <QScreen>
//#include <QtWidgets/QApplication>

#include "simplegraphmodel.h"

using QtNodes::QmlBasicGraphicsScene;
using QtNodes::ConnectionStyle;
using QtNodes::QmlGraphicsView;
using QtNodes::NodeRole;
using QtNodes::StyleCollection;

int main(int argc, char *argv[])
{
//    QApplication app(argc, argv);

    QGuiApplication app(argc, argv);

//    qmlRegisterType<QmlBasicGraphicsScene>("GraphEditor", 1, 0, "Ellipse");
    qmlRegisterType<QmlGraphicsView>("GraphEditor", 1, 0, "GraphView");


    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qml-ex1/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();

//    SimpleGraphModel graphModel;
//
//    // Initialize and connect two nodes.
//    {
//        NodeId id1 = graphModel.addNode();
//        graphModel.setNodeData(id1, NodeRole::Position, QPointF(0, 0));
//
//        NodeId id2 = graphModel.addNode();
//        graphModel.setNodeData(id2, NodeRole::Position, QPointF(300, 300));
//
//        graphModel.addConnection(ConnectionId{id1, 0, id2, 0});
//    }
//
//    auto scene = new BasicGraphicsScene(graphModel);
//
//    GraphicsView view(scene);
//
//    // Setup context menu for creating new nodes.
//    view.setContextMenuPolicy(Qt::ActionsContextMenu);
//    QAction createNodeAction(QStringLiteral("Create Node"), &view);
//    QObject::connect(&createNodeAction, &QAction::triggered, [&]() {
//        // Mouse position in scene coordinates.
//        QPointF posView = view.mapToScene(view.mapFromGlobal(QCursor::pos()));
//
//        NodeId const newId = graphModel.addNode();
//        graphModel.setNodeData(newId, NodeRole::Position, posView);
//    });
//    view.insertAction(view.actions().front(), &createNodeAction);
//
//    view.setWindowTitle("Simple Node Graph");
//    view.resize(800, 600);
//
//    // Center window.
//    view.move(QApplication::primaryScreen()->availableGeometry().center() - view.rect().center());
//    view.showNormal();
//
//    return app.exec();
}
