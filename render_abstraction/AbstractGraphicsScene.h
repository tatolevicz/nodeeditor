//
// Created by Arthur Abel Motelevicz on 15/01/23.
//

#pragma once

#include <QtCore/QUuid>
//#include <QtWidgets/QGraphicsScene>
//#include <QtWidgets/QMenu>

#include <functional>
#include <memory>
#include <tuple>
#include <unordered_map>

#include "AbstractGraphModel.hpp"
#include "AbstractNodeGeometry.hpp"
#include "ConnectionIdHash.hpp"
#include "Definitions.hpp"
#include "Export.hpp"

#include "QUuidStdHash.hpp"

class QUndoStack;

namespace QtNodes {

class AbstractGraphModel;
class AbstractNodePainter;
class ConnectionGraphicsObject;
class NodeGraphicsObject;
class NodeStyle;

/// An naive abstraction of an instance of QGraphicsScene.
class NODE_EDITOR_PUBLIC AbstractGraphicsScene
{
public:
    // Scenes without models are not supported
//    AbstractGraphicsScene() = delete;

public:
    /// @returns associated AbstractGraphModel.
    virtual AbstractGraphModel const &graphModel() const = 0;

    virtual AbstractGraphModel &graphModel() = 0;

    virtual AbstractNodeGeometry &nodeGeometry() = 0;

    virtual AbstractNodePainter &nodePainter() = 0;

    virtual void setNodePainter(std::unique_ptr<AbstractNodePainter> newPainter) = 0;

    virtual QUndoStack &undoStack() = 0;

    /// Creates a "draft" instance of ConnectionGraphicsObject.
    /**
   * The scene caches a "draft" connection which has one loose end.
   * After attachment the "draft" instance is deleted and instead a
   * normal "full" connection is created.
   * Function @returns the "draft" instance for further geometry
   * manipulations.
   */
    virtual std::unique_ptr<ConnectionGraphicsObject> const &makeDraftConnection(
        ConnectionId const newConnectionId) = 0;

    /// Deletes "draft" connection.
    /**
   * The function is called when user releases the mouse button during
   * the construction of the new connection without attaching it to any
   * node.
   */
    virtual void resetDraftConnection() = 0;

    /// Deletes all the nodes. Connections are removed automatically.
    virtual void clearScene() = 0;

    /// @returns NodeGraphicsObject associated with the given nodeId.
    /**
   * @returns nullptr when the object is not found.
   */
    virtual NodeGraphicsObject *nodeGraphicsObject(NodeId nodeId) = 0;

    /// @returns ConnectionGraphicsObject corresponding to `connectionId`.
    /**
   * @returns `nullptr` when the object is not found.
   */
    virtual ConnectionGraphicsObject *connectionGraphicsObject(ConnectionId connectionId) = 0;

    virtual Qt::Orientation orientation() const = 0;

    virtual void setOrientation(Qt::Orientation const orientation) = 0;

public:
    /// Can @return an instance of the scene context menu in subclass.
    /**
   * Default implementation returns `nullptr`.
   */
    virtual QObject *createSceneMenu(QPointF const scenePos) = 0;

//Q_SIGNALS:
//    void nodeMoved(NodeId const nodeId, QPointF const &newLocation);
//
//    void nodeClicked(NodeId const nodeId);
//
//    void nodeSelected(NodeId const nodeId);
//
//    void nodeDoubleClicked(NodeId const nodeId);
//
//    void nodeHovered(NodeId const nodeId, QPoint const screenPos);
//
//    void nodeHoverLeft(NodeId const nodeId);
//
//    void connectionHovered(ConnectionId const connectionId, QPoint const screenPos);
//
//    void connectionHoverLeft(ConnectionId const connectionId);
//
//    /// Signal allows showing custom context menu upon clicking a node.
//    void nodeContextMenu(NodeId const nodeId, QPointF const pos);
//
//public Q_SLOTS:
//    /// Slot called when the `connectionId` is erased form the AbstractGraphModel.
//    void onConnectionDeleted(ConnectionId const connectionId);
//
//    /// Slot called when the `connectionId` is created in the AbstractGraphModel.
//    void onConnectionCreated(ConnectionId const connectionId);
//
//    void onNodeDeleted(NodeId const nodeId);
//
//    void onNodeCreated(NodeId const nodeId);
//
//    void onNodePositionUpdated(NodeId const nodeId);
//
//    void onNodeUpdated(NodeId const nodeId);

//    void onModelReset() = 0;


protected:
    /// @brief Creates Node and Connection graphics objects.
    /**
   * Function is used to populate an empty scene in the constructor. We
   * perform depth-first AbstractGraphModel traversal. The connections are
   * created by checking non-empty node `Out` ports.
   */
    virtual void traverseGraphAndPopulateGraphicsObjects() = 0 ;

    /// Redraws adjacent nodes for given `connectionId`
    virtual void updateAttachedNodes(ConnectionId const connectionId, PortType const portType)  = 0;

};

} // namespace QtNodes
