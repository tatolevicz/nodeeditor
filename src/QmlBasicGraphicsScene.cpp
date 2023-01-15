#include "QmlBasicGraphicsScene.hpp"

#include "AbstractNodeGeometry.hpp"
#include "ConnectionGraphicsObject.hpp"
#include "ConnectionIdUtils.hpp"
#include "DefaultHorizontalNodeGeometry.hpp"
#include "DefaultNodePainter.hpp"
#include "DefaultVerticalNodeGeometry.hpp"
#include "GraphicsView.hpp"
#include "NodeGraphicsObject.hpp"

#include <QUndoStack>

//#include <QtWidgets/QFileDialog>
//#include <QtWidgets/QGraphicsSceneMoveEvent>

#include <QtCore/QBuffer>
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QtGlobal>

#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include <utility>
#include <queue>

namespace QtNodes {

QmlBasicGraphicsScene::QmlBasicGraphicsScene(AbstractGraphModel &graphModel, QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , _graphModel(graphModel)
    , _nodeGeometry(std::make_unique<DefaultHorizontalNodeGeometry>(_graphModel))
    , _nodePainter(std::make_unique<DefaultNodePainter>())
    , _undoStack(new QUndoStack(this))
    , _orientation(Qt::Horizontal)
{
//    setItemIndexMethod(QGraphicsScene::NoIndex);

    connect(&_graphModel,
            &AbstractGraphModel::connectionCreated,
            this,
            &QmlBasicGraphicsScene::onConnectionCreated);

    connect(&_graphModel,
            &AbstractGraphModel::connectionDeleted,
            this,
            &QmlBasicGraphicsScene::onConnectionDeleted);

    connect(&_graphModel,
            &AbstractGraphModel::nodeCreated,
            this,
            &QmlBasicGraphicsScene::onNodeCreated);

    connect(&_graphModel,
            &AbstractGraphModel::nodeDeleted,
            this,
            &QmlBasicGraphicsScene::onNodeDeleted);

    connect(&_graphModel,
            &AbstractGraphModel::nodePositionUpdated,
            this,
            &QmlBasicGraphicsScene::onNodePositionUpdated);

    connect(&_graphModel,
            &AbstractGraphModel::nodeUpdated,
            this,
            &QmlBasicGraphicsScene::onNodeUpdated);

    connect(&_graphModel, &AbstractGraphModel::modelReset, this, &QmlBasicGraphicsScene::onModelReset);

    traverseGraphAndPopulateGraphicsObjects();
}

QmlBasicGraphicsScene::~QmlBasicGraphicsScene() = default;

AbstractGraphModel const &QmlBasicGraphicsScene::graphModel() const
{
    return _graphModel;
}

AbstractGraphModel &QmlBasicGraphicsScene::graphModel()
{
    return _graphModel;
}

AbstractNodeGeometry &QmlBasicGraphicsScene::nodeGeometry()
{
    return *_nodeGeometry;
}

AbstractNodePainter &QmlBasicGraphicsScene::nodePainter()
{
    return *_nodePainter;
}

void QmlBasicGraphicsScene::setNodePainter(std::unique_ptr<AbstractNodePainter> newPainter)
{
    _nodePainter = std::move(newPainter);
}

QUndoStack &QmlBasicGraphicsScene::undoStack()
{
    return *_undoStack;
}

std::unique_ptr<ConnectionGraphicsObject> const &QmlBasicGraphicsScene::makeDraftConnection(
    ConnectionId const incompleteConnectionId)
{
//    _draftConnection = std::make_unique<ConnectionGraphicsObject>(*this, incompleteConnectionId);

    _draftConnection->grabMouse();

    return _draftConnection;
}

void QmlBasicGraphicsScene::resetDraftConnection()
{
    _draftConnection.reset();
}

void QmlBasicGraphicsScene::clearScene()
{
    auto const &allNodeIds = graphModel().allNodeIds();

    for (auto nodeId : allNodeIds) {
        graphModel().deleteNode(nodeId);
    }
}

NodeGraphicsObject *QmlBasicGraphicsScene::nodeGraphicsObject(NodeId nodeId)
{
    NodeGraphicsObject *ngo = nullptr;
    auto it = _nodeGraphicsObjects.find(nodeId);
    if (it != _nodeGraphicsObjects.end()) {
        ngo = it->second.get();
    }

    return ngo;
}

ConnectionGraphicsObject *QmlBasicGraphicsScene::connectionGraphicsObject(ConnectionId connectionId)
{
    ConnectionGraphicsObject *cgo = nullptr;
    auto it = _connectionGraphicsObjects.find(connectionId);
    if (it != _connectionGraphicsObjects.end()) {
        cgo = it->second.get();
    }

    return cgo;
}

void QmlBasicGraphicsScene::setOrientation(Qt::Orientation const orientation)
{
    if (_orientation != orientation) {
        _orientation = orientation;

        switch (_orientation) {
        case Qt::Horizontal:
            _nodeGeometry = std::make_unique<DefaultHorizontalNodeGeometry>(_graphModel);
            break;

        case Qt::Vertical:
            _nodeGeometry = std::make_unique<DefaultVerticalNodeGeometry>(_graphModel);
            break;
        }

        onModelReset();
    }
}

//QMenu *QmlBasicGraphicsScene::createSceneMenu(QPointF const scenePos)
//{
//    Q_UNUSED(scenePos);
//    return nullptr;
//}

void QmlBasicGraphicsScene::traverseGraphAndPopulateGraphicsObjects()
{
    auto allNodeIds = _graphModel.allNodeIds();

    // First create all the nodes.
    for (NodeId const nodeId : allNodeIds) {
//        _nodeGraphicsObjects[nodeId] = std::make_unique<NodeGraphicsObject>(*this, nodeId);
    }

    // Then for each node check output connections and insert them.
    for (NodeId const nodeId : allNodeIds) {
        unsigned int nOutPorts = _graphModel.nodeData<PortCount>(nodeId, NodeRole::OutPortCount);

        for (PortIndex index = 0; index < nOutPorts; ++index) {
            auto const &outConnectionIds = _graphModel.connections(nodeId, PortType::Out, index);

            for (auto cid : outConnectionIds) {
//                _connectionGraphicsObjects[cid] = std::make_unique<ConnectionGraphicsObject>(*this, cid);
            }
        }
    }
}

void QmlBasicGraphicsScene::updateAttachedNodes(ConnectionId const connectionId,
                                             PortType const portType)
{
    auto node = nodeGraphicsObject(getNodeId(portType, connectionId));

    if (node) {
        node->update();
    }
}

void QmlBasicGraphicsScene::onConnectionDeleted(ConnectionId const connectionId)
{
    auto it = _connectionGraphicsObjects.find(connectionId);
    if (it != _connectionGraphicsObjects.end()) {
        _connectionGraphicsObjects.erase(it);
    }

    // TODO: do we need it?
    if (_draftConnection && _draftConnection->connectionId() == connectionId) {
        _draftConnection.reset();
    }

    updateAttachedNodes(connectionId, PortType::Out);
    updateAttachedNodes(connectionId, PortType::In);
}

void QmlBasicGraphicsScene::onConnectionCreated(ConnectionId const connectionId)
{
//    _connectionGraphicsObjects[connectionId]
//        = std::make_unique<ConnectionGraphicsObject>(*this, connectionId);

    updateAttachedNodes(connectionId, PortType::Out);
    updateAttachedNodes(connectionId, PortType::In);
}

void QmlBasicGraphicsScene::onNodeDeleted(NodeId const nodeId)
{
    auto it = _nodeGraphicsObjects.find(nodeId);
    if (it != _nodeGraphicsObjects.end()) {
        _nodeGraphicsObjects.erase(it);
    }
}

void QmlBasicGraphicsScene::onNodeCreated(NodeId const nodeId)
{
//    _nodeGraphicsObjects[nodeId] = std::make_unique<NodeGraphicsObject>(*this, nodeId);
}

void QmlBasicGraphicsScene::onNodePositionUpdated(NodeId const nodeId)
{
    auto node = nodeGraphicsObject(nodeId);
    if (node) {
        node->setPos(_graphModel.nodeData(nodeId, NodeRole::Position).value<QPointF>());
        node->update();
    }
}

void QmlBasicGraphicsScene::onNodeUpdated(NodeId const nodeId)
{
    auto node = nodeGraphicsObject(nodeId);

    if (node) {
        node->setGeometryChanged();

        _nodeGeometry->recomputeSize(nodeId);

        node->update();
        node->moveConnections();
    }
}

void QmlBasicGraphicsScene::onModelReset()
{
    _connectionGraphicsObjects.clear();
    _nodeGraphicsObjects.clear();

//    clear();

    traverseGraphAndPopulateGraphicsObjects();
}

} // namespace QtNodes
