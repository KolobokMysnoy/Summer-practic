#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H


#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
class Node;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = nullptr);        

    void itemMoved();

    //flags
    void set_edge_flag();
    void set_node_flag();
    void set_Dnode_flag();
    void set_Dedge_flag();
    void clear_flags();

    void set_value(QString val);

    //files
    void add_existing(QList <QString> list);
    QList<QString> save();
    void clear_area();

public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();
    void mousePressEvent(QMouseEvent * mouseEvent);


protected:

    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
#endif

    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void scaleView(qreal scaleFactor);

private:
    bool flag_node,flag_edge,flagD_node,flagD_edge;
    int timerId = 0;
    QString value;

    Node* from,* to;
    QGraphicsScene *scene;
};

#endif // GRAPHWIDGET_H
