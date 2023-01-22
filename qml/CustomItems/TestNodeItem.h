//
// Created by Arthur Abel Motelevicz on 21/01/23.
//
#pragma once

#include <QQuickItem>
#include "Export.hpp"
//#include <QSGTransformNode>

class BackgroundNode;

//class GraphItemNode : public QSGTransformNode
//{
//public:
//    BackgroundNode * test;
//};

namespace QtNodes {
/**
 * @brief A central view able to render objects from `QmlBasicGraphicsScene`.
 */
class NODE_EDITOR_PUBLIC TestNodeItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)

public:
    struct ScaleRange
    {
        double minimum = 0;
        double maximum = 0;
    };

    const QColor &backgroundColor() const;
    void setBackgroundColor(const QColor &newColor);


public:
    TestNodeItem(QQuickItem *parent = Q_NULLPTR);
    BackgroundNode* getNode();
Q_SIGNALS:
    void backgroundColorChanged();
protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void hoverEnterEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;

    void onFirstShow();
    bool _firstShowed = false;

protected:

private:
    QPointF _difference;
    QPointF _clickPos;
    BackgroundNode * _node{nullptr};
    bool _geometryChanged;
    QColor m_backgroundColor;
};
} // namespace QtNodes
