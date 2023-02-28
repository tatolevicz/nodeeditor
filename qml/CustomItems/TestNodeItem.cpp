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
}

void TestNodeItem::keyPressEvent(QKeyEvent *event)
{
    QQuickItem::keyPressEvent(event);
    //testing node movent



    switch (event->key()) {
        case Qt::Key::Key_Up:
        {
            setY(y() + _step);
            break;
        }
        case Qt::Key::Key_Down:
        {
            setY(y() - _step);
            break;
        }
        case Qt::Key::Key_Left:
        {
            setX(x() - _step);
            break;
        }
        case Qt::Key::Key_Right:
        {
            setX(x() + _step);
            break;
        }
        case Qt::Key::Key_Shift:
        {
            _step = _step*5;
            break;
        }
        default: {
            break;
        }
    }

    return;//############

    switch (event->key()) {
    case Qt::Key_Shift:
        //        setDragMode(QTestNodeItem::RubberBandDrag);
        break;

    default:
        break;
    }

    //    QTestNodeItem::keyPressEvent(event);
}

void TestNodeItem::keyReleaseEvent(QKeyEvent *event)
{
    QQuickItem::keyPressEvent(event);
    //testing node movent

    switch (event->key()) {
        case Qt::Key::Key_Shift:
        {
            _step = _step/5;
            break;
        }
        default: {
            break;
        }
    }

    return;//############
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