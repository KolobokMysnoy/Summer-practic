#include "edge.h"
#include "nodes.h"

#include <QPainter>
#include <QtMath>
#include <QPainterPath>

const int Shift_y=40,Size_y=40, Size_x=130,Shift_x_arrow=10;

Edge::Edge(Node *sourceNode, Node *destNode)
    : source(sourceNode), dest(destNode)
{
    setAcceptedMouseButtons(Qt::NoButton);
    if(source==dest){
        //if we need circle to one node
        source->addEdge(this);
    }
    else
    {
        source->addEdge(this);
        dest->addEdge(this);
    }
    adjust();
}

Node *Edge::sourceNode() const
{
    return source;
}

Node *Edge::destNode() const
{
    return dest;
}

void Edge::adjust()
{
    if (!source || !dest)
            return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }
}

QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0+10;

    if(source==dest){
        //if we need circle
        // we shift our point of rect
        return QRectF(QPointF(sourcePoint.x(),sourcePoint.y()-Shift_y),
                      QSizeF(Size_x,Size_y))

            .normalized()
            .adjusted(-extra, -extra, extra, extra);
    }
    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.))&&!(source==dest))
        return;

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    //Arc
    //point to draw arc
    int k=10+line.length()/10;
    QPointF c1,c2;

    c1=line.center();
    c2=dest->pos();
    c1=centre(c1,k);

    //draw arc
    QPainterPath *path=new QPainterPath;


    if(source==dest){
        //if we need circle
        path->moveTo(QPointF(sourcePoint.x(),sourcePoint.y()-10));
        painter->drawPoint(sourcePoint);
        path->cubicTo(QPointF(sourcePoint.x()+Size_x-10,sourcePoint.y()-Size_y),
                      QPointF(destPoint.x()+Shift_x_arrow,destPoint.y()),QPointF(destPoint.x()+Shift_x_arrow,destPoint.y()));

        painter->drawPath(*path);
        double angle=std::atan2(0,-15);
        QPointF destArrowP1 = destPoint + QPointF(sin(angle - M_PI / 3) * arrowSize+Shift_x_arrow,
                                                  cos(angle - M_PI / 3) * arrowSize);
        QPointF destArrowP2 = destPoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize+Shift_x_arrow,
                                                  cos(angle - M_PI + M_PI / 3) * arrowSize);

        painter->setBrush(Qt::black);

        painter->drawPolygon(QPolygonF() << QPointF(line.p2().x()+Shift_x_arrow,line.p2().y()) <<
                             destArrowP1 << destArrowP2);

        delete path;
        return;
    }
    path->moveTo(sourcePoint);
    path->cubicTo(c1,destPoint,destPoint);


    painter->setPen(Qt::black);
    painter->drawPath(*path);

    // Draw the arrows

    QLineF line2(destPoint, c1);
    double angle=std::atan2(line2.dy(),-line2.dx());

    QPointF destArrowP1 = destPoint + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                              cos(angle - M_PI / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                              cos(angle - M_PI + M_PI / 3) * arrowSize);


    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);

    delete path;
}

QPointF Edge::middle(){
    //point to draw TextEdge
    QLineF line(sourcePoint, destPoint);
    QPointF c2=boundingRect().center();
    QPointF c1= centre(line.center(),line.length()/10+10);
    QLineF line2(c1,c2);

    if(c1.y()<c2.y()){
        //bottom text
        return QPointF(line2.center().x(),line2.center().y());
    }
    else{
        //top text
        return QPointF(line2.center().x(),line2.center().y());
    }
}

QPointF Edge::centre(QPointF c1,int k){
    //where to place our c1 for cubicto
    if(source->x()==dest->x()){
        if(source->y()>dest->y()){
            //left arc
            c1.setX(c1.x()-k);
        }
        else{
            //right arc
            c1.setX(c1.x()+k);
        }
    }

    //y=y
    if(source->y()==dest->y()){
        if(source->x()>dest->x()){
            //bottom arc
            c1.setY(c1.y()+k);
        }
        else{
            //upper arc
            c1.setY(c1.y()-k);
        }
    }

    //one node right top

    if(source->x()>dest->x()&&source->y()<dest->y()){
        //right arc
        c1.setY(c1.y()+k);
        c1.setX(c1.x()+k);
    }

    if(source->x()<dest->x()&&source->y()>dest->y()){
        //left arc
        c1.setY(c1.y()-k);
        c1.setX(c1.x()-k);
    }

    //one node left top
    if(source->x()<dest->x()&&source->y()<dest->y()){
        //right arc
        c1.setY(c1.y()-k);
        c1.setX(c1.x()+k);
    }

    if(source->x()>dest->x()&&source->y()>dest->y()){
        //left arc
        c1.setY(c1.y()+k);
        c1.setX(c1.x()-k);
    }

    //one node left top

    //point-end
    //don't go beyoung bounding rectangle
    if(c1.x()<boundingRect().topLeft().x()){
        c1.setX(boundingRect().topLeft().x());
    }

    if(c1.x()>boundingRect().topRight().x()){
        c1.setX(boundingRect().topRight().x());
    }


    if(c1.y()<boundingRect().topRight().y()){
        c1.setY(boundingRect().topRight().y());
    }


    if(c1.y()>boundingRect().bottomLeft().y()){
        c1.setY(boundingRect().bottomLeft().y());
    }
    return c1;
}
