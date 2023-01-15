#include "graphview.h"
#include <QPainter>

GraphView::GraphView(QQuickItem *p): QQuickPaintedItem(p)
{
    setAcceptedMouseButtons(Qt::LeftButton);

    setAntialiasing(true);
}

void GraphView::paint(QPainter *painter)
{
    painter->save();

    const QRectF paintRect = contentsBoundingRect();

    //draw fill with no pen
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0,0,0));
    painter->drawRect(paintRect);

    painter->restore();
}

void GraphView::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
}

void GraphView::wheelEvent(QWheelEvent *ev)
{


    for(auto c : childItems()){
        if(ev->angleDelta().y() > 0){
            c->setWidth(c->width() + 10);
            c->setHeight(c->height() + 10);
        }
        else{
            c->setWidth(c->width() - 10);
            c->setHeight(c->height() - 10);
        }
    }


    qDebug() << "Content scale: " << contentsScale()  ;
}
