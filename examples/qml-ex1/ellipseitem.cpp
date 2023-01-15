#include <QPainter>
#include "ellipseitem.h"
#include <QRandomGenerator>

EllipseItem::EllipseItem(QQuickItem *p): QQuickPaintedItem(p)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setAntialiasing(true);

}

void EllipseItem::paint(QPainter *painter)
{

    painter->save();

    const QRectF paintRect = QRectF({}, QSizeF{width(), height()}).adjusted(m_borderWidth/2, m_borderWidth/2, -m_borderWidth/2, -m_borderWidth/2);

    //draw fill with no pen
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_fillColor);
    painter->drawEllipse(paintRect);

    painter->restore();

    //draw stroke with the pen
    painter->save();

    QPen pen = painter->pen();
    pen.setWidth(m_borderWidth);
    pen.setBrush(m_borderColor);
    painter->setPen(pen);

    painter->drawEllipse(paintRect);



    painter->restore();
}

void EllipseItem::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
    setFillColor(randomColor());
}

void EllipseItem::wheelEvent(QWheelEvent *ev)
{
    if(ev->angleDelta().y() > 0){
        setFillColor(m_fillColor.lighter(110));
    }
    else{
        setFillColor(m_fillColor.darker(110));
    }
}

QColor EllipseItem::randomColor()
{
    return QColor(
                QRandomGenerator::global()->generate() & 0xff,
                QRandomGenerator::global()->generate() & 0xff,
                QRandomGenerator::global()->generate() & 0xff
    );
}

const QColor &EllipseItem::borderColor() const
{
    return m_borderColor;
}

void EllipseItem::setBorderColor(const QColor &newBorderColor)
{
    if (m_borderColor == newBorderColor)
        return;
    m_borderColor = newBorderColor;
    emit borderColorChanged();
}

const QColor &EllipseItem::fillColor() const
{
    return m_fillColor;
}

void EllipseItem::setFillColor(const QColor &newFillColor)
{
    if (m_fillColor == newFillColor)
        return;
    m_fillColor = newFillColor;
    update();
    emit fillColorChanged();
}

int EllipseItem::borderWidth() const
{
    return m_borderWidth;
}

void EllipseItem::setBorderWidth(int newBorderWidth)
{
    if (m_borderWidth == newBorderWidth)
        return;
    m_borderWidth = newBorderWidth;
    emit borderWidthChanged();
}
