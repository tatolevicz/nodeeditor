//#include <QGuiApplication>
//#include <QQmlApplicationEngine>
//#include "customTest.h"
//
//int main(int argc, char *argv[])
//{
//    QGuiApplication app(argc, argv);
//
//    qmlRegisterType<CustomTest>("GraphEditor", 1, 0, "GraphView");
//
//    QQmlApplicationEngine engine;
//    const QUrl url(u"qrc:/qml-ex1/main.qml"_qs);
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                     &app, [url](QObject *obj, const QUrl &objUrl) {
//        if (!obj && url == objUrl)
//            QCoreApplication::exit(-1);
//    }, Qt::QueuedConnection);
//    engine.load(url);
//
//    return app.exec();
//}


#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtNodes/QmlBasicGraphicsScene>
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/GraphicsView>
#include <QtNodes/ConnectionStyle>
#include <QtNodes/QmlGraphicsView>
#include <QtNodes/StyleCollection>
#include "TestNodeItem.h"
#include <QAction>
#include <QScreen>
#include <QtWidgets/QApplication>

#include "simplegraphmodel.h"

using QtNodes::QmlBasicGraphicsScene;
using QtNodes::ConnectionStyle;
using QtNodes::QmlGraphicsView;
using QtNodes::TestNodeItem;

using QtNodes::GraphicsView;
using QtNodes::BasicGraphicsScene;
using QtNodes::NodeRole;
using QtNodes::StyleCollection;

#define RUN_QML 1

int main(int argc, char *argv[])
{

#if RUN_QML
    QGuiApplication app(argc, argv);

//    qmlRegisterType<QmlBasicGraphicsScene>("GraphEditor", 1, 0, "Ellipse");
    qmlRegisterType<QmlGraphicsView>("GraphEditor", 1, 0, "GraphView");
    qmlRegisterType<TestNodeItem>("GraphEditor", 1, 0, "TestNodeItem");


    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qml-ex1/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
#else
    QApplication app(argc, argv);

    SimpleGraphModel graphModel;

    // Initialize and connect two nodes.
    {
        NodeId id1 = graphModel.addNode();
        graphModel.setNodeData(id1, NodeRole::Position, QPointF(0, 0));

        NodeId id2 = graphModel.addNode();
        graphModel.setNodeData(id2, NodeRole::Position, QPointF(300, 300));

        graphModel.addConnection(ConnectionId{id1, 0, id2, 0});
    }

    auto scene = new BasicGraphicsScene(graphModel);

    GraphicsView view(scene);

    // Setup context menu for creating new nodes.
    view.setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction createNodeAction(QStringLiteral("Create Node"), &view);
    QObject::connect(&createNodeAction, &QAction::triggered, [&]() {
        // Mouse position in scene coordinates.
        QPointF posView = view.mapToScene(view.mapFromGlobal(QCursor::pos()));

        NodeId const newId = graphModel.addNode();
        graphModel.setNodeData(newId, NodeRole::Position, posView);
    });
    view.insertAction(view.actions().front(), &createNodeAction);

    view.setWindowTitle("Simple Node Graph");
    view.resize(800, 600);

    // Center window.
    view.move(QApplication::primaryScreen()->availableGeometry().center() - view.rect().center());
    view.showNormal();

    return app.exec();
#endif

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
