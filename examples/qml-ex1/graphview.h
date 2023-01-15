#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QQuickPaintedItem>

class GraphView : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit GraphView(QQuickItem *p = nullptr);
    void paint(QPainter *painter) override;

protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void wheelEvent(QWheelEvent *ev) override;

signals:


private:


};



#endif // GRAPHVIEW_H
