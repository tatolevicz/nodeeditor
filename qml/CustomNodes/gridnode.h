//
// Created by Arthur Abel Motelevicz on 16/01/23.
//
#ifndef GRIDNODE_H
#define GRIDNODE_H

#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>

class GridNode : public QSGGeometryNode
{
public:
    GridNode(QColor gridColor, int gridSize);
    void setRect(const QRectF &rect);
    QRectF getRect();
private:
    QSGFlatColorMaterial m_material;
    QSGGeometry m_geometry;
    QColor m_gridColor;
    int _gridSize;
    QRectF _rect;
};

#endif // GRIDNODE_H
