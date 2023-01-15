#include "QmlGraphicsView.hpp"

#include "QmlBasicGraphicsScene.hpp"
#include "ConnectionGraphicsObject.hpp"
#include "NodeGraphicsObject.hpp"
#include "StyleCollection.hpp"
#include "UndoCommands.hpp"

#include <QQuickPaintedItem>

#include <QtGui/QBrush>
#include <QtGui/QPen>

#include <QtWidgets/QMenu>

#include <QtCore/QDebug>
#include <QtCore/QPointF>
#include <QtCore/QRectF>

#include <QtOpenGL>
#include <QtWidgets>

#include <cmath>
#include <iostream>

using QtNodes::QmlBasicGraphicsScene;
using QtNodes::QmlGraphicsView;

QmlGraphicsView::QmlGraphicsView(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , _clearSelectionAction(Q_NULLPTR)
    , _deleteSelectionAction(Q_NULLPTR)
    , _duplicateSelectionAction(Q_NULLPTR)
    , _copySelectionAction(Q_NULLPTR)
    , _pasteAction(Q_NULLPTR)
{
//    setDragMode(QQmlGraphicsView::ScrollHandDrag);
//    setRenderHint(QPainter::Antialiasing);

//    auto const &flowViewStyle = StyleCollection::flowViewStyle();

//    setBackgroundBrush(flowViewStyle.BackgroundColor);

//    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

//    setTransformationAnchor(QQmlGraphicsView::AnchorUnderMouse);

//    setCacheMode(QQmlGraphicsView::CacheBackground);
//    setViewportUpdateMode(QQmlGraphicsView::BoundingRectViewportUpdate);

    setScaleRange(0.3, 2);

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
}

void QmlGraphicsView::setupScale(double scale)
{
    scale = std::max(_scaleRange.minimum, std::min(_scaleRange.maximum, scale));

    if (scale <= 0)
        return;

//    if (scale == transform().m11())
//        return;
//
//    QTransform matrix;
//    matrix.scale(scale, scale);
//    setTransform(matrix, false);
//
//    Q_EMIT scaleChanged(scale);
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
    if (event->button() == Qt::LeftButton) {
        _clickPos = mapToScene(event->pos());
    }
}

void QmlGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "Event: " << event->pos().y();
//    QQmlGraphicsView::mouseMoveEvent(event);
//    if (scene()->mouseGrabberItem() == nullptr && event->buttons() == Qt::LeftButton) {
//        // Make sure shift is not being pressed
//        if ((event->modifiers() & Qt::ShiftModifier) == 0) {
//            QPointF difference = _clickPos - mapToScene(event->pos());
//            setSceneRect(sceneRect().translated(difference.x(), difference.y()));
//        }
//    }
}
void QmlGraphicsView::paint(QPainter *painter){
    drawBackground(painter, contentsBoundingRect());
}


void QmlGraphicsView::drawBackground(QPainter *painter, const QRectF &r)
{
//    QQmlGraphicsView::drawBackground(painter, r);
    qDebug() << "DrawBg: " << r.x();
    auto drawGrid = [&](double gridStep) {
//        QRect windowRect = rect();
        QRectF windowRect = contentsBoundingRect();
        QPointF tl = mapToScene(windowRect.topLeft());
        QPointF br = mapToScene(windowRect.bottomRight());

        double left = std::floor(tl.x() / gridStep - 0.5);
        double right = std::floor(br.x() / gridStep + 1.0);
        double bottom = std::floor(tl.y() / gridStep - 0.5);
        double top = std::floor(br.y() / gridStep + 1.0);

        // vertical lines
        for (int xi = int(left); xi <= int(right); ++xi) {
            QLineF line(xi * gridStep, bottom * gridStep, xi * gridStep, top * gridStep);

            painter->drawLine(line);
        }

        // horizontal lines
        for (int yi = int(bottom); yi <= int(top); ++yi) {
            QLineF line(left * gridStep, yi * gridStep, right * gridStep, yi * gridStep);
            painter->drawLine(line);
        }
    };

    auto const &flowViewStyle = StyleCollection::flowViewStyle();

    QPen pfine(flowViewStyle.FineGridColor, 1.0);

    painter->setPen(pfine);
    drawGrid(15);

    QPen p(flowViewStyle.CoarseGridColor, 1.0);

    painter->setPen(p);
    drawGrid(150);
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
    QRectF const viewRect = contentsBoundingRect();
    if (!viewRect.contains(origin))
        origin = viewRect.center();

    return mapToScene(origin);
}
