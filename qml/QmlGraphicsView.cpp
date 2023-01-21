#include "QmlGraphicsView.hpp"
#include <QSGGeometryNode>
#include "BackgroundNode.h"
#include "gridnode.h"
#include "QmlBasicGraphicsScene.hpp"
#include "ConnectionGraphicsObject.hpp"
#include "NodeGraphicsObject.hpp"
#include "StyleCollection.hpp"
#include "UndoCommands.hpp"

#include <QQuickPaintedItem>

#include <QBrush>
#include <QPen>

#include <QMenu>

#include <QDebug>
#include <QPointF>
#include <QRectF>

#include <QtOpenGL>
#include <QtWidgets>

#include <cmath>
#include <iostream>

using QtNodes::QmlBasicGraphicsScene;
using QtNodes::QmlGraphicsView;

QmlGraphicsView::QmlGraphicsView(QQuickItem *parent)
    : QQuickItem(parent)
    , _clearSelectionAction(Q_NULLPTR)
    , _deleteSelectionAction(Q_NULLPTR)
    , _duplicateSelectionAction(Q_NULLPTR)
    , _copySelectionAction(Q_NULLPTR)
    , _pasteAction(Q_NULLPTR)
{
    setFlag(ItemHasContents, true);
    setAcceptedMouseButtons(Qt::MouseButton::LeftButton);
//    setDragMode(QQmlGraphicsView::ScrollHandDrag); -> solved
    setAcceptHoverEvents(true);

//    setRenderHint(QPainter::Antialiasing); -> solved
    setAntialiasing(true);


    auto const &flowViewStyle = StyleCollection::flowViewStyle();
    m_backgroundColor = flowViewStyle.BackgroundColor;
    m_coarseGridColor = flowViewStyle.CoarseGridColor;
    m_fineGridColor = flowViewStyle.FineGridColor;
//    setBackgroundBrush(flowViewStyle.BackgroundColor); -> solved

//    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); -> solved
//    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); -> solved

//    setTransformationAnchor(QQmlGraphicsView::AnchorUnderMouse);

//    setCacheMode(QQmlGraphicsView::CacheBackground);
//    setViewportUpdateMode(QQmlGraphicsView::BoundingRectViewportUpdate);

    setScaleRange(0.3, 2.0);

    // Sets the scene rect to its maximum possible ranges to avoid autu scene range
    // re-calculation when expanding the all QGraphicsItems common rect.
//    int maxSize = 32767;
//    setSceneRect(-maxSize, -maxSize, (maxSize * 2), (maxSize * 2));


}

QmlGraphicsView::QmlGraphicsView(QmlBasicGraphicsScene *scene, QQuickItem *parent)
    : QmlGraphicsView(parent)
{
    setScene(scene);
}

QAction *QmlGraphicsView::clearSelectionAction() const
{
    return _clearSelectionAction;
}

QAction *QmlGraphicsView::deleteSelectionAction() const
{
    return _deleteSelectionAction;
}

void QmlGraphicsView::setScene(QmlBasicGraphicsScene *scene)
{
//    QQmlGraphicsView::setScene(scene);

    {
        // setup actions
        delete _clearSelectionAction;
        _clearSelectionAction = new QAction(QStringLiteral("Clear Selection"), this);
        _clearSelectionAction->setShortcut(Qt::Key_Escape);

//        connect(_clearSelectionAction, &QAction::triggered, scene, &QGraphicsScene::clearSelection);

//        addAction(_clearSelectionAction);
    }

    {
        delete _deleteSelectionAction;
        _deleteSelectionAction = new QAction(QStringLiteral("Delete Selection"), this);
        _deleteSelectionAction->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
        _deleteSelectionAction->setShortcut(QKeySequence(QKeySequence::Delete));
        connect(_deleteSelectionAction,
                &QAction::triggered,
                this,
                &QmlGraphicsView::onDeleteSelectedObjects);

//        addAction(_deleteSelectionAction);
    }

    {
        delete _duplicateSelectionAction;
        _duplicateSelectionAction = new QAction(QStringLiteral("Duplicate Selection"), this);
        _duplicateSelectionAction->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
        _duplicateSelectionAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
        connect(_duplicateSelectionAction,
                &QAction::triggered,
                this,
                &QmlGraphicsView::onDuplicateSelectedObjects);

//        addAction(_duplicateSelectionAction);
    }

    {
        delete _copySelectionAction;
        _copySelectionAction = new QAction(QStringLiteral("Copy Selection"), this);
        _copySelectionAction->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
        _copySelectionAction->setShortcut(QKeySequence(QKeySequence::Copy));
        connect(_copySelectionAction,
                &QAction::triggered,
                this,
                &QmlGraphicsView::onCopySelectedObjects);

//        addAction(_copySelectionAction);
    }

    {
        delete _pasteAction;
        _pasteAction = new QAction(QStringLiteral("Copy Selection"), this);
        _pasteAction->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
        _pasteAction->setShortcut(QKeySequence(QKeySequence::Paste));
        connect(_pasteAction, &QAction::triggered, this, &QmlGraphicsView::onPasteObjects);

//        addAction(_pasteAction);
    }

    auto undoAction = scene->undoStack().createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);
//    addAction(undoAction);

    auto redoAction = scene->undoStack().createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(QKeySequence::Redo);
//    addAction(redoAction);
}

void QmlGraphicsView::centerScene()
{
//    if (scene()) {
//        scene()->setSceneRect(QRectF());
//
//        QRectF sceneRect = scene()->sceneRect();
//
//        if (sceneRect.width() > this->rect().width() || sceneRect.height() > this->rect().height()) {
//            fitInView(sceneRect, Qt::KeepAspectRatio);
//        }
//
//        centerOn(sceneRect.center());
//    }
}
//
//void QmlGraphicsView::contextMenuEvent(QContextMenuEvent *event)
//{
//    if (itemAt(event->pos())) {
//        QQmlGraphicsView::contextMenuEvent(event);
//        return;
//    }
//
//    auto const scenePos = mapToScene(event->pos());
//
//    QMenu *menu = nodeScene()->createSceneMenu(scenePos);
//
//    if (menu) {
//        menu->exec(event->globalPos());
//    }
//}

void QmlGraphicsView::wheelEvent(QWheelEvent *event)
{
    QPoint delta = event->angleDelta();

    if (delta.y() == 0) {
        event->ignore();
        return;
    }

    double const d = delta.y() / std::abs(delta.y());

    if (d > 0.0)
        scaleUp();
    else
        scaleDown();
}

double QmlGraphicsView::getScale() const
{
//    return transform().m11();
    return 1.0;
}

void QmlGraphicsView::setScaleRange(double minimum, double maximum)
{
    if (maximum < minimum)
        std::swap(minimum, maximum);
    minimum = std::max(0.0, minimum);
    maximum = std::max(0.0, maximum);

    _scaleRange = {minimum, maximum};

//    setupScale(transform().m11());
}

void QmlGraphicsView::setScaleRange(ScaleRange range)
{
    setScaleRange(range.minimum, range.maximum);
}

void QmlGraphicsView::scaleUp()
{
//    double const step = 1.2;
//    double const factor = std::pow(step, 1.0);
//
//    if (_scaleRange.maximum > 0) {
//        QTransform t = transform();
//        t.scale(factor, factor);
//        if (t.m11() >= _scaleRange.maximum) {
//            setupScale(t.m11());
//            return;
//        }
//    }
//
//    scale(factor, factor);
//    Q_EMIT scaleChanged(transform().m11());

    double const step = 1.1;
//    double const factor = std::pow(step, 1.1);
    QTransform t = _transformNode->matrix().toTransform();

    if (_scaleRange.maximum > 0) {
        t.scale(step, step);
        if (t.m11() >= _scaleRange.maximum) {
            setupScale(t.m11());
            return;
        }
    }

    _transformNode->setMatrix(t);
    update();

}

void QmlGraphicsView::scaleDown()
{
//    double const step = 1.2;
//    double const factor = std::pow(step, -1.0);

//    if (_scaleRange.minimum > 0) {
//        QTransform t = transform();
//        t.scale(factor, factor);
//        if (t.m11() <= _scaleRange.minimum) {
//            setupScale(t.m11());
//            return;
//        }
//    }
//
//    scale(factor, factor);
//    Q_EMIT scaleChanged(transform().m11());

    double const step = 0.9;
//    double const factor = std::pow(step, 0.9);
    QTransform t = _transformNode->matrix().toTransform();

    if (_scaleRange.minimum > 0) {
        t.scale(step, step);
        if (t.m11() <= _scaleRange.minimum) {
            setupScale(t.m11());
            return;
        }
    }

    _transformNode->setMatrix(t);
    update();
}

void QmlGraphicsView::setupScale(double scale)
{
    scale = std::max(_scaleRange.minimum, std::min(_scaleRange.maximum, scale));

    if (scale <= 0)
        return;

    QTransform t = _transformNode->matrix().toTransform();
    if (scale == t.m11())
        return;

    QTransform matrix;
    matrix.translate(t.dx(), t.dy());
    matrix.scale(scale, scale);
    _transformNode->setMatrix(matrix);
    update();

    Q_EMIT scaleChanged(scale);
}

void QmlGraphicsView::onDeleteSelectedObjects()
{
//    nodeScene()->undoStack().push(new DeleteCommand(nodeScene()));
}

void QmlGraphicsView::onDuplicateSelectedObjects()
{
//    QPointF const pastePosition = scenePastePosition();

//    nodeScene()->undoStack().push(new CopyCommand(nodeScene()));
//    nodeScene()->undoStack().push(new PasteCommand(nodeScene(), pastePosition));
}

void QmlGraphicsView::onCopySelectedObjects()
{
//    nodeScene()->undoStack().push(new CopyCommand(nodeScene()));
}

void QmlGraphicsView::onPasteObjects()
{
//    QPointF const pastePosition = scenePastePosition();
//    nodeScene()->undoStack().push(new PasteCommand(nodeScene(), pastePosition));
}

void QmlGraphicsView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Shift:
//        setDragMode(QQmlGraphicsView::RubberBandDrag);
        break;

    default:
        break;
    }

//    QQmlGraphicsView::keyPressEvent(event);
}

void QmlGraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Shift:
//        setDragMode(QQmlGraphicsView::ScrollHandDrag);
        break;

    default:
        break;
    }
//    QQmlGraphicsView::keyReleaseEvent(event);
}

void QmlGraphicsView::mousePressEvent(QMouseEvent *event)
{
//    QQmlGraphicsView::mousePressEvent(event);
//    if (event->button() == Qt::LeftButton) {
//        _clickPos = mapToScene(event->pos());
//    }

    if (event->button() == Qt::LeftButton) {
        _clickPos = event->pos();
        QGuiApplication::setOverrideCursor(QCursor(Qt::ClosedHandCursor));
    }
}
void QmlGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton) {
        QGuiApplication::restoreOverrideCursor();
    }
}
void QmlGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug() << "Event X: " << event->pos().rx();
//    qDebug() << "Event Y: " << event->pos().ry();
    if (event->buttons() == Qt::LeftButton) {
        // Make sure shift is not being pressed
        if ((event->modifiers() & Qt::ShiftModifier) == 0) {
            auto t = _transformNode->matrix().toTransform();

            QPointF difference = (_clickPos - event->pos())*(-1)/t.m11();
            _clickPos = event->pos();

            auto cp = getCurrentPosition();
            qDebug() << "CurrentPosition X: " << cp.x();
//            if(cp.x() > _maxSize/4){
//                qDebug() << "O loco X!";
//                setCurrentPosition(QPointF(_maxSize, cp.y()));
//                return;
//            }
//
//            if(cp.x() < -_maxSize/4){
//                qDebug() << "O loco X!";
//                setCurrentPosition(QPointF(-_maxSize, cp.y()));
//                return;
//            }
//
//
//
//            if(cp.y() > _maxSize/4 || cp.y() < -_maxSize/4){
//                qDebug() << "O loco Y!";
//                return;
//            }

//            t.translate(difference.x(), difference.y());
//            _transformNode->setMatrix(t);
//            update();

            setCurrentPosition(cp + difference);

        }
    }

//    QQmlGraphicsView::mouseMoveEvent(event);
//    if (scene()->mouseGrabberItem() == nullptr && event->buttons() == Qt::LeftButton) {
//        // Make sure shift is not being pressed
//        if ((event->modifiers() & Qt::ShiftModifier) == 0) {
//            QPointF difference = _clickPos - mapToScene(event->pos());
//            setSceneRect(sceneRect().translated(difference.x(), difference.y()));
//        }
//    }
}
void QmlGraphicsView::hoverEnterEvent(QHoverEvent *event){
    Q_UNUSED(event);
    QGuiApplication::setOverrideCursor(QCursor(Qt::OpenHandCursor));
}
void QmlGraphicsView::hoverLeaveEvent(QHoverEvent *event){
    Q_UNUSED(event);
    QGuiApplication::restoreOverrideCursor();
}


void QmlGraphicsView::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    _geometryChanged = true;
    update();
    QQuickItem::geometryChange(newGeometry, oldGeometry);
}


QSGNode* QmlGraphicsView::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    if(!_firstShowed) {
        onFirstShow();
        _firstShowed = true;
    }
    _transformNode = static_cast<GraphNode *>(oldNode);

    QRectF rect = boundingRect();

    if (rect.isEmpty()) {
        delete _transformNode;
        return nullptr;
    }

    if (!_transformNode) {
        _transformNode = new GraphNode();
        _transformNode->background = new BackgroundNode(m_backgroundColor);
        _transformNode->fineGrid = new GridNode(m_fineGridColor, 16);
        _transformNode->coarseGrid = new GridNode(m_coarseGridColor, 16*10);


        _transformNode->background->setRect(QRectF(-_maxSize/2,-_maxSize/2, _maxSize, _maxSize));
        _transformNode->fineGrid->setRect(QRectF(-_maxSize/2,-_maxSize/2, _maxSize, _maxSize));
        _transformNode->coarseGrid->setRect(QRectF(-_maxSize/2,-_maxSize/2, _maxSize, _maxSize));
        _transformNode->appendChildNode(_transformNode->background);
        _transformNode->appendChildNode(_transformNode->fineGrid);
        _transformNode->appendChildNode(_transformNode->coarseGrid);

//        auto t = _transformNode->matrix().toTransform();
//        t.translate( boundingRect().width()/2, boundingRect().height()/2);
//        _transformNode->setMatrix(t);
        setCurrentPosition(QPointF(0,0));
    }

    if (_geometryChanged) {
//        _transformNode->grid->setRect(rect);
    }

    _geometryChanged = false;

    return _transformNode;
//    QSGGeometryNode *node = nullptr;
////    QSGGeometry *geometry = nullptr;
//
//    if (!oldNode) {
//        auto gridNode = new GridNode;
//        gridNode->setRect(boundingRect());
//        node = gridNode;
//    }
//    else{
//        auto gridNode = static_cast<GridNode *>(oldNode);
//        gridNode->setRect(boundingRect());
//        node = gridNode;
//    }
//
//
//    node->markDirty(QSGNode::DirtyGeometry);
//
//    qDebug() << "Paint called" ;
//    if(_transformNode == nullptr) {
//        _transformNode = new QSGTransformNode;
//        _transformNode->appendChildNode(node);
//    }
//
//    return _transformNode;
}
//void QmlGraphicsView::paint(QPainter *painter){
//    drawBackground(painter, contentsBoundingRect());
//}

//
//void QmlGraphicsView::drawBackground(QPainter *painter, const QRectF &r)
//{
////    QQmlGraphicsView::drawBackground(painter, r);
//    qDebug() << "DrawBg: " << r.x();
//    auto drawGrid = [&](double gridStep) {
////        QRect windowRect = rect();
//        QRectF windowRect = contentsBoundingRect();
//        QPointF tl = mapToScene(windowRect.topLeft());
//        QPointF br = mapToScene(windowRect.bottomRight());
//
//        double left = std::floor(tl.x() / gridStep - 0.5);
//        double right = std::floor(br.x() / gridStep + 1.0);
//        double bottom = std::floor(tl.y() / gridStep - 0.5);
//        double top = std::floor(br.y() / gridStep + 1.0);
//
//        // vertical lines
//        for (int xi = int(left); xi <= int(right); ++xi) {
//            QLineF line(xi * gridStep, bottom * gridStep, xi * gridStep, top * gridStep);
//
//            painter->drawLine(line);
//        }
//
//        // horizontal lines
//        for (int yi = int(bottom); yi <= int(top); ++yi) {
//            QLineF line(left * gridStep, yi * gridStep, right * gridStep, yi * gridStep);
//            painter->drawLine(line);
//        }
//    };
//
//    auto const &flowViewStyle = StyleCollection::flowViewStyle();
//
//    QPen pfine(flowViewStyle.FineGridColor, 1.0);
//
//    painter->setPen(pfine);
//    drawGrid(15);
//
//    QPen p(flowViewStyle.CoarseGridColor, 1.0);
//
//    painter->setPen(p);
//    drawGrid(150);
//}

void QmlGraphicsView::onFirstShow(){
    if(_maxSize < boundingRect().width() || _maxSize < boundingRect().height())
        _maxSize = std::max(boundingRect().width(), boundingRect().height());
}

void QmlGraphicsView::showEvent(QShowEvent *event)
{
    qDebug() << "Ev: " << event->isAccepted();
//    QQmlGraphicsView::showEvent(event);

    centerScene();
}

QmlBasicGraphicsScene *QmlGraphicsView::nodeScene()
{
//    return dynamic_cast<QmlBasicGraphicsScene *>(scene());
    return _scene;
}

QPointF QmlGraphicsView::scenePastePosition()
{
//    QPoint origin = mapFromGlobal(QCursor::pos());
    QPointF origin = mapFromGlobal(QCursor::pos());

//    QRect const viewRect = rect();
    QRectF const viewRect = boundingRect();
    if (!viewRect.contains(origin))
        origin = viewRect.center();

    return mapToScene(origin);
}

const QColor &QmlGraphicsView::backgroundColor() const
{
    return m_backgroundColor;
}

void QmlGraphicsView::setBackgroundColor(const QColor &newColor)
{
    if (m_backgroundColor == newColor)
        return;
    m_backgroundColor = newColor;
    update();
    Q_EMIT backgroundColorChanged();
}

const QColor &QmlGraphicsView::fineGridColor() const
{
    return m_fineGridColor;
}

void QmlGraphicsView::setFineGridColor(const QColor &newGridColor)
{
    if (m_fineGridColor == newGridColor)
        return;
    m_fineGridColor = newGridColor;
    update();
    Q_EMIT fineGridColorChanged();
}

const QColor &QmlGraphicsView::coarseGridColor() const
{
    return m_coarseGridColor;
}

void QmlGraphicsView::setCoarseGridColor(const QColor &newGridColor)
{
    if (m_coarseGridColor == newGridColor)
        return;
    m_coarseGridColor = newGridColor;
    update();
    Q_EMIT coarseGridColorChanged();
}

QPointF QmlGraphicsView::getCurrentPosition(){
    QPointF out;
    auto t = _transformNode->matrix().toTransform();

    out.setX(t.dx() - boundingRect().width()/2);
    out.setY(t.dy() - boundingRect().height()/2);

    return  out;
}

void QmlGraphicsView::setCurrentPosition(const QPointF& newPos){
    auto t = QTransform();
    auto ct = _transformNode->matrix().toTransform();
    t.scale(ct.m11(), ct.m11());
    auto newX = newPos.x();
    auto newY = newPos.y();

    auto xLeftLimit =   _maxSize/2 - boundingRect().width()/2;
    auto yTopLimit =    _maxSize/2 - boundingRect().width()/2;
    auto xRightLimit =  -(_maxSize/2 - boundingRect().width()/2);
    auto yBottomLimit = -(_maxSize/2 - boundingRect().width()/2);

    if(newX > xLeftLimit ) {
        newX = xLeftLimit;
    }
    else if(newX < xRightLimit ) {
        newX = xRightLimit;
    }

    if(newY > yTopLimit ) {
        newY = yTopLimit;
    }
    else if(newY < yBottomLimit ) {
        newY = yBottomLimit;
    }

    newX += boundingRect().width()/2;
    newY += boundingRect().width()/2;

    t.translate(
        newX,
        newY
    );

    _transformNode->setMatrix(t);
    update();
}