//
// Created by Arthur Abel Motelevicz on 16/01/23.
//

#ifndef QTNODESLIBRARY_BACKGROUNDNODE_H
#define QTNODESLIBRARY_BACKGROUNDNODE_H

#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>

class BackgroundNode : public QSGGeometryNode
{
public:
    BackgroundNode(QColor fillColor);
    void setRect(const QRectF &rect);
    QRectF getRect();
private:
    QSGFlatColorMaterial m_material;
    QSGGeometry m_geometry;
    QColor m_fillColor;
    QRectF _rect;

};

#endif //QTNODESLIBRARY_BACKGROUNDNODE_H
