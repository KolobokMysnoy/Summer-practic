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

    //create a timer to transfer edge
    void itemMoved();

    //set flags to delete or add node or edge
    // and clear all flags
    void set_edge_flag();
    void set_node_flag();
    void set_Dnode_flag();
    void set_Dedge_flag();
    void clear_flags();

    //set a value to give it to new edge or
    // node
    void set_value(QString val);

    //add existing graph from list of strings or
    //give our graph in list of strings
    void add_existing(QList <QString> list);
    QList<QString> save();

    //clear all graphic items
    void clear_area();

public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();
    void mousePressEvent(QMouseEvent * mouseEvent) override;

protected:

    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
#endif

    void scaleView(qreal scaleFactor);

private:
    bool flag_node,flag_edge,flagD_node,flagD_edge;
    int timerId = 0;
    QString value;

    QGraphicsScene *scene;

    //from which node to which node
    //add edge
    Node* from,* to;
};

#endif // GRAPHWIDGET_H
