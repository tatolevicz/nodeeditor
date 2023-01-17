//
// Created by Arthur Abel Motelevicz on 16/01/23.
//

#include "gridnode.h"
#include "qmath.h"

GridNode::GridNode(QColor gridColor, int gridSize)
    : m_geometry(QSGGeometry::defaultAttributes_Point2D(), 0),
    m_gridColor(gridColor),
    _gridSize(gridSize)
{
    setGeometry(&m_geometry);
    m_geometry.setDrawingMode(QSGGeometry::DrawLines);

    setMaterial(&m_material);
    m_material.setColor(m_gridColor);
}

/*
 * The function hardcodes a fixed set of grid lines and scales
 * those to the bounding rect.
 */
void GridNode::setRect(const QRectF &rect)
{
    int vCount = int((rect.width() - 1) / _gridSize);
    int hCount = int((rect.height() - 1) / _gridSize);

    int lineCount = vCount + hCount;

    QSGGeometry *g = geometry();

    g->allocate(lineCount * 2);

    float x = rect.x();
    float y = rect.y();
    float w = rect.width();
    float h = rect.height();

    QSGGeometry::Point2D *v = g->vertexDataAsPoint2D();

    // Then write the vertical lines
    for (int i=0; i<vCount; ++i)  {
        float dx = (i + 1) * _gridSize;
        v[i*2].set(rect.x() + dx, y);
        v[i*2+1].set(rect.x() + dx, y + h);
    }
    v += vCount * 2;
    // Then write the horizontal lines
    for (int i=0; i<hCount; ++i)  {
        float dy = (i + 1) * _gridSize;
        v[i*2].set( x, rect.y() + dy);
        v[i*2+1].set(x + w, rect.y() + dy);
    }

    // Tell the scenegraph we updated the geometry..
    markDirty(QSGNode::DirtyGeometry);
}
