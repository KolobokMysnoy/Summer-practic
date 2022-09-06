#ifndef NODES_H
#define NODES_H

#include <QGraphicsItem>
#include <QList>
//определённые в другом месте классы
class GraphWidget;
class Edge;

class Node : public QGraphicsItem
{
public:
    Node(GraphWidget *graphWidget);
    ~Node();

    //add or remove edge
    //and get all edges connected to
    //node
    void addEdge(Edge *edge);
    void removeEdge(Edge *edge);
    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    //set and get value
    void set_name(QString name);
    QString get_name();

    //repaint item to new position
    bool advance();
    void calculateForces();

protected:
    //shape of node and paint this node
    QPainterPath shape() const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //item change position
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    //when transfer node with mouse
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QList<Edge *> edgeList;
    QPointF newPos;
    GraphWidget *graph;

    QString name_of_node;
};

#endif
