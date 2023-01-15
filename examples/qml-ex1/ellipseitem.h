#ifndef ELLIPSEITEM_H
#define ELLIPSEITEM_H

#include <QQuickPaintedItem>

class EllipseItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)

public:
    explicit EllipseItem(QQuickItem *p = nullptr);
    void paint(QPainter *painter) override;

    const QColor &borderColor() const;
    void setBorderColor(const QColor &newBorderColor);

    const QColor &fillColor() const;
    void setFillColor(const QColor &newFillColor);

    int borderWidth() const;
    void setBorderWidth(int newBorderWidth);

protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void wheelEvent(QWheelEvent *ev) override;


signals:
    void borderColorChanged();
    void fillColorChanged();
    void borderWidthChanged();

private:
    static QColor randomColor();
    QColor m_borderColor;
    QColor m_fillColor;
    int m_borderWidth;
};

#endif // ELLIPSEITEM_H
