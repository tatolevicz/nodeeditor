//
// Created by Arthur Abel Motelevicz on 15/01/23.
//

#include "customTest.h"
#include <QSGFlatColorMaterial>
#include <QSGVertexColorMaterial>
#include <QSGGeometryNode>


CustomTest::CustomTest(QQuickItem *parent)
    : QQuickItem(parent)
    , m_p1(0, 0)
    , m_p2(1, 0)
    , m_p3(0, 1)
    , m_p4(1, 1)
    , m_segmentCount(6)
{
    setFlag(ItemHasContents, true);
}
void CustomTest::setP1(const QPointF &p) {

    if (p == m_p1)
        return;

    m_p1 = p;
    emit p1Changed(p);
    update();

}
void CustomTest::setP2(const QPointF &p) {
    if (p == m_p2)
        return;

    m_p2 = p;
    emit p2Changed(p);
    update();
}
void CustomTest::setP3(const QPointF &p) {
    if (p == m_p2)
        return;

    m_p2 = p;
    emit p2Changed(p);
    update();
}
void CustomTest::setP4(const QPointF &p) {
    if (p == m_p2)
        return;

    m_p2 = p;
    emit p2Changed(p);
    update();
}

void CustomTest::setSegmentCount(int count) {
    if(m_segmentCount == count)
        return ;

    m_segmentCount = count;
    emit segmentCountChanged(count);
    update();
}

QSGNode *CustomTest::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *node = nullptr;
    QSGGeometry *geometry = nullptr;

    if (!oldNode) {
        node = new QSGGeometryNode;
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), m_segmentCount);

//        geometry->setLineWidth(2);
//        geometry->setDrawingMode(QSGGeometry::DrawTriangles);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
        auto *material = new QSGFlatColorMaterial;
        material->setColor(QColor(255, 0, 0));
//
//        auto *material = new QSGVertexColorMaterial;
//        material->
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }
    else{
        node = static_cast<QSGGeometryNode *>(oldNode);
        geometry = node->geometry();
        geometry->allocate(m_segmentCount*2);
    }

    QSizeF itemSize = size();
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
    float x1 =  0;
    float y1 =  0;
    float x2 =  itemSize.width();
    float y2 =  itemSize.height();

    vertices[0].set(x1, y1);
    vertices[1].set(x2, y1);
    vertices[2].set(x2, y2);
    vertices[3].set(x1, y1);
    vertices[4].set(x1, y2);
    vertices[5].set(x2, y2);
//    for (int i = 0; i < m_segmentCount; ++i) {
//        qreal t = i / qreal(m_segmentCount - 1);
//        qreal invt = 1 - t;
//
//        QPointF pos = invt * invt * invt * m_p1
//                      + 3 * invt * invt * t * m_p2
//                      + 3 * invt * t * t * m_p3
//                      + t * t * t * m_p4;
//
//        float x = pos.x() * itemSize.width();
//        float y = pos.y() * itemSize.height();
//
//        vertices[i].set(x, y);
//    }
    node->markDirty(QSGNode::DirtyGeometry);

    qDebug() << "Paint called" ;
    return node;
}
CustomTest::~CustomTest() {}
