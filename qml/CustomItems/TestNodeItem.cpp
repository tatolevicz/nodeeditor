//
// Created by Arthur Abel Motelevicz on 21/01/23.
//

#include "TestNodeItem.h"
#include "ConnectionGraphicsObject.hpp"
#include "CustomNodes/BackgroundNode.h"
#include "NodeGraphicsObject.hpp"
#include "StyleCollection.hpp"
#include "UndoCommands.hpp"
#include <QSGGeometryNode>
#include <QQuickPaintedItem>
#include <QBrush>
#include <QPointF>
#include <QRectF>
#include <QtOpenGL>
#include <QQuickWindow>
#include "BackgroundNode.h"

using QtNodes::TestNodeItem;

TestNodeItem::TestNodeItem(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
    setAcceptedMouseButtons(Qt::MouseButton::LeftButton);
    setAcceptHoverEvents(true);
    setAntialiasing(true);
//    setFlags(ItemAcceptsInputMethod);

    QTimer *timer = new QTimer(this);

    connect(timer, &QTimer::timeout,[&](){
        tick();
    });

    timer->setInterval(16); // 60 FPS

    timer->start();

}

void TestNodeItem::keyPressEvent(QKeyEvent *event)
{
    QQuickItem::keyPressEvent(event);

    if(event->modifiers() & Qt::ShiftModifier)
        _shiftPressed = true;
    else
        _shiftPressed = false;

    switch (event->key()) {
        case Qt::Key::Key_Up:
            _upPressed = true;
            _downPressed = false;
            break;
        case Qt::Key::Key_Down:
            _downPressed = true;
            _upPressed = false;
            break;
        case Qt::Key::Key_Left:
            _leftPressed = true;
            _rightPressed = false;
            break;
        case Qt::Key::Key_Right:
            _rightPressed = true;
            _leftPressed = false;
            break;
        default: {
            return;
        }
    }
}

void TestNodeItem::keyReleaseEvent(QKeyEvent *event)
{
    QQuickItem::keyReleaseEvent(event);

    switch (event->key()) {
        case Qt::Key::Key_Up:
            _upPressed = false;
            break;
        case Qt::Key::Key_Down:
            _downPressed = false;
            break;
        case Qt::Key::Key_Left:
            _leftPressed = false;
            break;
        case Qt::Key::Key_Right:
            _rightPressed = false;
            break;
        default: {
            return;
        }
    }
}

void TestNodeItem::mousePressEvent(QMouseEvent *event)
{
    //    QTestNodeItem::mousePressEvent(event);
    //    if (event->button() == Qt::LeftButton) {
    //        _clickPos = mapToScene(event->pos());
    //    }

    if (event->button() == Qt::LeftButton) {
        _clickPos = event->pos();
        QGuiApplication::setOverrideCursor(QCursor(Qt::ClosedHandCursor));
    }
}
void TestNodeItem::mouseReleaseEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton) {
        QGuiApplication::restoreOverrideCursor();
    }
}
void TestNodeItem::mouseMoveEvent(QMouseEvent *event)
{

    if (event->buttons() == Qt::LeftButton) {
        // Make sure shift is not being pressed
        if ((event->modifiers() & Qt::ShiftModifier) == 0) {
            bool dummy;
            auto t = itemTransform(this, &dummy);
            Q_UNUSED(t)
            _difference += (_clickPos - event->pos());///t.m11();
            _clickPos = event->pos();

            setX(x() - _difference.x());
            setY(y() - _difference.y());
        }
    }
}
void TestNodeItem::hoverEnterEvent(QHoverEvent *event){
    Q_UNUSED(event);
    QGuiApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
}
void TestNodeItem::hoverLeaveEvent(QHoverEvent *event){
    Q_UNUSED(event);
    QGuiApplication::restoreOverrideCursor();
}


void TestNodeItem::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    _geometryChanged = true;
    update();
    QQuickItem::geometryChange(newGeometry, oldGeometry);
}


QSGNode* TestNodeItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    if(!_firstShowed) {
        onFirstShow();
        _firstShowed = true;
    }
    _node = static_cast<BackgroundNode *>(oldNode);
    Q_UNUSED(oldNode);
    QRectF rect = boundingRect();

    if (rect.isEmpty()) {
        delete _node;
        return nullptr;
    }

    if (!_node) {
        _node = new BackgroundNode(QColor(m_backgroundColor));
        _node->setRect(rect);
    }

    if (_geometryChanged) {

    }

    _geometryChanged = false;

    return _node;

}
void TestNodeItem::onFirstShow(){

}


const QColor &TestNodeItem::backgroundColor() const
{
    return m_backgroundColor;
}

void TestNodeItem::setBackgroundColor(const QColor &newColor)
{
    if (m_backgroundColor == newColor)
        return;
    m_backgroundColor = newColor;
    update();
    Q_EMIT backgroundColorChanged();
}

BackgroundNode* TestNodeItem::getNode(){
    return _node;
}

void TestNodeItem::tick() {

    int step = _slowStep;

    if(_shiftPressed)
        step = _fastStep;

    int dx = 0, dy = 0;
    if (_leftPressed) {
        dx -= step;
    }
    if (_rightPressed) {
        dx += step;
    }
    if (_upPressed) {
        dy -= step;
    }
    if (_downPressed) {
        dy += step;
    }
    setX(x() + dx);
    setY(y() + dy);
    update();
}
