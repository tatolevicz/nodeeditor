#pragma once

//#include <QtWidgets/QQmlGraphicsView>
#include <QQuickItem>

#include "Export.hpp"
#include <QSGTransformNode>

class GridNode;
class BackgroundNode;

class GraphNode : public QSGTransformNode
{
public:
    BackgroundNode * background;
    GridNode *fineGrid;
    GridNode *coarseGrid;
};

namespace QtNodes {

class QmlBasicGraphicsScene;

/**
 * @brief A central view able to render objects from `QmlBasicGraphicsScene`.
 */
class NODE_EDITOR_PUBLIC QmlGraphicsView : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor fineGridColor READ fineGridColor WRITE setFineGridColor NOTIFY fineGridColorChanged)
    Q_PROPERTY(QColor coarseGridColor READ coarseGridColor WRITE setCoarseGridColor NOTIFY coarseGridColorChanged)

public:
    struct ScaleRange
    {
        double minimum = 0;
        double maximum = 0;
    };

    const QColor &backgroundColor() const;
    const QColor &fineGridColor() const;
    const QColor &coarseGridColor() const;

    void setBackgroundColor(const QColor &newColor);
    void setFineGridColor(const QColor &newColor);
    void setCoarseGridColor(const QColor &newColor);


public:
    QmlGraphicsView(QQuickItem *parent = Q_NULLPTR);
    QmlGraphicsView(QmlBasicGraphicsScene *scene, QQuickItem *parent = Q_NULLPTR);

    QmlGraphicsView(const QmlGraphicsView &) = delete;
//    QmlGraphicsView operator=(const QmlGraphicsView &) = delete;

    QAction *clearSelectionAction() const;

    QAction *deleteSelectionAction() const;

    void setScene(QmlBasicGraphicsScene *scene);

    void centerScene();

    /// @brief max=0/min=0 indicates infinite zoom in/out
    void setScaleRange(double minimum = 0, double maximum = 0);

    void setScaleRange(ScaleRange range);

    double getScale() const;

public Q_SLOTS:
    void scaleUp();

    void scaleDown();

    void setupScale(double scale);

    void onDeleteSelectedObjects();

    void onDuplicateSelectedObjects();

    void onCopySelectedObjects();

    void onPasteObjects();

Q_SIGNALS:
    void backgroundColorChanged();
    void fineGridColorChanged();
    void coarseGridColorChanged();
    void scaleChanged(double scale);

protected:
protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

//    void contextMenuEvent(QContextMenuEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void hoverEnterEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;
//    void drawBackground(QPainter *painter, const QRectF &r);

    void showEvent(QShowEvent *event);

protected:
    QmlBasicGraphicsScene *_scene;
    QmlBasicGraphicsScene *nodeScene();

    /// Computes scene position for pasting the copied/duplicated node groups.
    QPointF scenePastePosition();

private:
    QAction *_clearSelectionAction = nullptr;
    QAction *_deleteSelectionAction = nullptr;
    QAction *_duplicateSelectionAction = nullptr;
    QAction *_copySelectionAction = nullptr;
    QAction *_pasteAction = nullptr;

    QPointF _clickPos;
    ScaleRange _scaleRange;
    GraphNode *_transformNode{nullptr};
    bool _geometryChanged;

    QColor m_coarseGridColor;
    QColor m_fineGridColor;
    QColor m_backgroundColor;

    int _maxSize = 10000;
    QPointF getCurrentPosition();
};
} // namespace QtNodes
