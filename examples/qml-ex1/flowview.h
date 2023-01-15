//#ifndef FLOWVIEW_H
//#define FLOWVIEW_H
//
//#include <QQuickPaintedItem>
//
//class FlowView : public QQuickPaintedItem
//{
//    Q_OBJECT
//public:
//    struct ScaleRange
//    {
//        double minimum = 0;
//        double maximum = 0;
//    };
//
//public:
//    explicit FlowView(QQuickItem *p = nullptr);
////    FlowView(BasicGraphicsScene *scene, QWidget *parent = Q_NULLPTR);
//
////    FlowView(const FlowView &) = delete;
////    FlowView operator=(const FlowView &) = delete;
//
//    QAction *clearSelectionAction() const;
//
//    QAction *deleteSelectionAction() const;
//
////    void setScene(BasicGraphicsScene *scene);
//
//    void centerScene();
//
//    /// @brief max=0/min=0 indicates infinite zoom in/out
//    void setScaleRange(double minimum = 0, double maximum = 0);
//
//    void setScaleRange(ScaleRange range);
//
//    double getScale() const;
//
//public Q_SLOTS:
//    void scaleUp();
//
//    void scaleDown();
//
//    void setupScale(double scale);
//
//    void onDeleteSelectedObjects();
//
//    void onDuplicateSelectedObjects();
//
//    void onCopySelectedObjects();
//
//    void onPasteObjects();
//
////Q_SIGNALS:
////    void scaleChanged(double scale);
//
//protected:
////    void contextMenuEvent(QContextMenuEvent *event) override;
//
//    void wheelEvent(QWheelEvent *event) override;
//
//    void keyPressEvent(QKeyEvent *event) override;
//
//    void keyReleaseEvent(QKeyEvent *event) override;
//
//    void mousePressEvent(QMouseEvent *event) override;
//
//    void mouseMoveEvent(QMouseEvent *event) override;
//
//    void drawBackground(QPainter *painter, const QRectF &r);
//
////    void showEvent(QShowEvent *event) override;
//
//protected:
////    BasicGraphicsScene *nodeScene();
//
//    /// Computes scene position for pasting the copied/duplicated node groups.
//    QPointF scenePastePosition();
//
//private:
//    QAction *_clearSelectionAction = nullptr;
//    QAction *_deleteSelectionAction = nullptr;
//    QAction *_duplicateSelectionAction = nullptr;
//    QAction *_copySelectionAction = nullptr;
//    QAction *_pasteAction = nullptr;
//
//    QPointF _clickPos;
//    ScaleRange _scaleRange;
//};
//
//
//#endif // FLOWVIEW_H
