#include "textedge.h"
#include <QPainter>

const int FontSize=14,ShiftX=7,ShiftY=10;
const qreal extra=14,BoundRect=30;

void TextEdge::set_value(QString text)
{
    //value of edge
    value=text;
}

void TextEdge::set_edge(Edge *edge)
{
    //add edge
    where=edge;
}

void TextEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //settings for pen
    QFont font = painter->font();
    font.setPointSize(FontSize);
    painter->setFont(font);
    painter->setPen(Qt::black);

    //position to draw
    QPointF po=where->middle();
    po.setX(po.x()-ShiftX);
    po.setY(po.y()+ShiftY);
    painter->drawText(po,value);
}

TextEdge::TextEdge(QString val)
{
    set_value(val);
}

QRectF TextEdge::boundingRect() const
{
    return QRectF(QPointF(where->middle().x()-4,where->middle().y()-5),
                  QSizeF(BoundRect,BoundRect))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

Edge* TextEdge::get_edge(){
    return where;
}

QString TextEdge::get_value(){
    return value;
}
