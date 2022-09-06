#include "nodes.h"
#include "edge.h"
#include "graphwidget.h"

#include <QRectF>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

const int Shift=10, Elipse_diam=20,ShiftText=5,LetterSize=13;
qreal Adjust_bound_point=10;

Node::Node(GraphWidget *graphWidget)
     : graph(graphWidget)
 {
     setFlag(ItemIsMovable);
     setFlag(ItemSendsGeometryChanges);
     setCacheMode(DeviceCoordinateCache);
 }

void Node::addEdge(Edge *edge)
 {
    //add edge to the list of all edges
     edgeList << edge;
     edge->adjust();
 }

void Node::removeEdge(Edge *edge)
 {
    edgeList.remove(edgeList.indexOf(edge));
    edge->adjust();
 }

 QList<Edge *> Node::edges() const
 {
     //return list of edges
     return edgeList;
 }

 void Node::calculateForces()
 {
     if (!scene() || scene()->mouseGrabberItem() == this) {
         newPos = pos();
         return;
     }

     QRectF sceneRect = scene()->sceneRect();
     newPos = pos() ;
     newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + Shift), sceneRect.right() - Shift));
     newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + Shift), sceneRect.bottom() - Shift));
 }

 bool Node::advance()
 {
     if (newPos == pos())
         return false;

     setPos(newPos);
     return true;
 }

 QRectF Node::boundingRect() const
 {
     qreal adjust = 2;
     return QRectF( -Adjust_bound_point - adjust, -Adjust_bound_point - adjust,
                    Elipse_diam+3 + adjust, Elipse_diam+3+ adjust);
 }

 QPainterPath Node::shape() const
 {
     //node hitbox
     QPainterPath path;
     path.addEllipse(-Adjust_bound_point, -Adjust_bound_point,
                     Elipse_diam, Elipse_diam);
     return path;
 }

 void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
 {
     //draw node
     if(name_of_node!="0"){
        painter->setPen(QPen(Qt::black, 2));
     }
     else{
         painter->setPen(QPen(Qt::white,2));
     }
     painter->drawEllipse(-Adjust_bound_point, -Adjust_bound_point,
                          Elipse_diam, Elipse_diam);


     //text in node
     QFont font = painter->font();
     font.setBold(true);
     font.setPointSize(LetterSize);
     painter->setFont(font);
     painter->setPen(Qt::black);
     painter->drawText(-ShiftText,ShiftText,name_of_node);
 }

 QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
 {
     //redraw edges to new position
     switch (change) {
     case ItemPositionHasChanged:
         for (Edge *edge : qAsConst(edgeList))
             edge->adjust();
         graph->itemMoved();
         break;
     default:
         break;
     };

     return QGraphicsItem::itemChange(change, value);
 }

 void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mousePressEvent(event);
 }

 void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mouseReleaseEvent(event);
 }

 void Node::set_name(QString name){
     //set value of node
     name_of_node=name;
 }

 QString Node::get_name(){
     return name_of_node;
 }

 Node::~Node(){
     for(Edge *ed:edgeList){
         delete ed;
     }
 }
