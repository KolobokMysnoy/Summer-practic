#ifndef TEXTEDGE_H
#define TEXTEDGE_H
#include <QGraphicsItem>
#include "edge.h"

class TextEdge: public QGraphicsItem
{
public:
    TextEdge(QString val);

    //value to print
    void set_value(QString text);
    QString get_value();

    //where to place
    void set_edge(Edge * edge);
    Edge* get_edge();

    enum { Type = UserType + 3 };
    int type() const override { return Type; }

protected:
    //paint
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    Edge *where;
    QString value;
};


#endif // TEXTEDGE_H
