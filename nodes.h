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

    //edges
    void addEdge(Edge *edge);
    void removeEdge(Edge *edge);
    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    //set and get value
    void set_name(QString name);
    QString get_name();

    bool advance();
    void calculateForces();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    //добавление действий мышкой(нажатие и отпускание)
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QList<Edge *> edgeList;
    QPointF newPos;
    GraphWidget *graph;

    QString name_of_node;
};

#endif
