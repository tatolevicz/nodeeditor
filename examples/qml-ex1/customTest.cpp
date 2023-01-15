//
// Created by Arthur Abel Motelevicz on 15/01/23.
//

#include "customTest.h"
#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>

CustomTest::CustomTest(QQuickItem *parent)
    : QQuickItem(parent)
    , m_p1(0, 0)
    , m_p2(1, 0)
    , m_p3(0, 1)
    , m_p4(1, 1)
    , m_segmentCount(32)
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
        geometry->setLineWidth(2);
        geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
        auto *material = new QSGFlatColorMaterial;
        material->setColor(QColor(255, 0, 0));
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }
    else{
        node = static_cast<QSGGeometryNode *>(oldNode);
        geometry = node->geometry();
        geometry->allocate(m_segmentCount);
    }

    QSizeF itemSize = size();
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

    for (int i = 0; i < m_segmentCount; ++i) {
        qreal t = i / qreal(m_segmentCount - 1);
        qreal invt = 1 - t;

        QPointF pos = invt * invt * invt * m_p1
                      + 3 * invt * invt * t * m_p2
                      + 3 * invt * t * t * m_p3
                      + t * t * t * m_p4;

        float x = pos.x() * itemSize.width();
        float y = pos.y() * itemSize.height();

        vertices[i].set(x, y);
    }
    node->markDirty(QSGNode::DirtyGeometry);

    return node;
}
CustomTest::~CustomTest() {}
