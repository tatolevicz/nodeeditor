#pragma once


#include <QtWidgets/QWidget>

#include "PortType.hpp"
#include "NodeData.hpp"
#include "Serializable.hpp"
#include "NodeGeometry.hpp"
#include "NodeStyle.hpp"
#include "NodePainterDelegate.hpp"
#include "Export.hpp"
#include "memory.hpp"

namespace QtNodes
{

enum class NodeValidationState
{
  Valid,
  Warning,
  Error
};

class Connection;

class StyleCollection;

class NODE_EDITOR_PUBLIC NodeDataModel
  : public QObject
  , public Serializable
{
  Q_OBJECT

public:

  NodeDataModel();

  virtual
  ~NodeDataModel() = default;

  /// Caption is used in GUI
  virtual QString
  caption() const = 0;

  /// It is possible to hide caption in GUI
  virtual bool
  captionVisible() const { return true; }

  /// Port caption is used in GUI to label individual ports
  virtual QString
  portCaption(PortType, PortIndex) const { return QString(); }

  /// It is possible to hide port caption in GUI
  virtual bool
  portCaptionVisible(PortType, PortIndex) const { return false; }

  /// Name makes this model unique
  virtual QString
  name() const = 0;

public:

  QJsonObject
  save() const override;

public:

  virtual
  unsigned int nPorts(PortType portType) const = 0;

  virtual
  NodeDataType dataType(PortType portType, PortIndex portIndex) const = 0;

public:

  enum class ConnectionPolicy
  {
    One,
    Many,
  };

  virtual
  ConnectionPolicy
  portOutConnectionPolicy(PortIndex) const
  {
    return ConnectionPolicy::Many;
  }

  virtual
  ConnectionPolicy
  portInConnectionPolicy(PortIndex) const
  {
    return ConnectionPolicy::One;
  }

  NodeStyle const&
  nodeStyle() const;

  void
  setNodeStyle(NodeStyle const& style);

public:

  /// Triggers the algorithm
  virtual
  void
  setInData(std::shared_ptr<NodeData> nodeData,
            PortIndex port) = 0;

  // Use this if portInConnectionPolicy returns ConnectionPolicy::Many
  virtual
  void
  setInData(std::shared_ptr<NodeData> nodeData,
            PortIndex port,
            const QUuid& connectionId)
  {
    Q_UNUSED(connectionId);
    setInData(nodeData, port);
  }

  virtual
  std::shared_ptr<NodeData>
  outData(PortIndex port) = 0;

  /**
   * It is recommented to preform a lazy initialization for the
   * embedded widget and create it inside this function, not in the
   * constructor of the current model.
   *
   * Our Model Registry is able to shortly instantiate models in order
   * to call the non-static `Model::name()`. If the embedded widget is
   * allocated in the constructor but not actually embedded into some
   * QGraphicsProxyWidget, we'll gonna have a dangling pointer.
   */
  virtual
  QWidget *
  embeddedWidget() = 0;

  virtual
  bool
  resizable() const { return false; }

  virtual
  NodeValidationState
  validationState() const { return NodeValidationState::Valid; }

  virtual
  QString
  validationMessage() const { return QString(""); }

  virtual
  NodePainterDelegate* painterDelegate() const { return nullptr; }

public Q_SLOTS:

  virtual void
  inputConnectionCreated(Connection const&)
  {
  }

  virtual void
  inputConnectionDeleted(Connection const&)
  {
  }

  virtual void
  outputConnectionCreated(Connection const&)
  {
  }

  virtual void
  outputConnectionDeleted(Connection const&)
  {
  }

Q_SIGNALS:

  /// Triggers the updates in the nodes downstream.
  void
  dataUpdated(PortIndex index);

  /// Triggers the propagation of the empty data downstream.
  void
  dataInvalidated(PortIndex index);

  void
  computingStarted();

  void
  computingFinished();

  void embeddedWidgetSizeUpdated();

private:

  NodeStyle _nodeStyle;
};
}
