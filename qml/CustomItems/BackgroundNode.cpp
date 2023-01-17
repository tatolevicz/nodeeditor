//
// Created by Arthur Abel Motelevicz on 16/01/23.
//

#include "BackgroundNode.h"

BackgroundNode::BackgroundNode(QColor fillColor)
    : m_geometry(QSGGeometry::defaultAttributes_Point2D(), 6),
    m_fillColor(fillColor)
{
    setGeometry(&m_geometry);
    m_geometry.setDrawingMode(QSGGeometry::DrawTriangleStrip);
    setMaterial(&m_material);
    m_material.setColor(m_fillColor);
}

/*
 * The function hardcodes a fixed set of grid lines and scales
 * those to the bounding rect.
 */
void BackgroundNode::setRect(const QRectF &rect)
{
    QSGGeometry *g = geometry();

    float x = rect.x();
    float y = rect.y();
    float w = rect.width();
    float h = rect.height();

    QSGGeometry::Point2D *v = g->vertexDataAsPoint2D();

    v[0].set(x, y);
    v[1].set(x + w, y);
    v[2].set(x + w, y + h);
    v[3].set(x, y);
    v[4].set(x, y + h);
    v[5].set(x + w, y + h);

    // Tell the scenegraph we updated the geometry..
    markDirty(QSGNode::DirtyGeometry);
}
