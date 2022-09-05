#include "graphwidget.h"

#include "edge.h"
#include "nodes.h"
#include "textedge.h"

#include <math.h>

#include <QKeyEvent>
#include <QRandomGenerator>

const int Random_num=150,Min_siz=400;
double Z_top=0.9,Z_bottom=0.2;
const QString Empty="";
const char RazdSig='/', RazdLine=':';

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(Min_siz, Min_siz);


    //setings
    this->setStyleSheet("background-color:white");
    from=to=nullptr;
    flag_edge=flag_node=flagD_edge=flagD_node=false;

}

void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

//Клавиши бинды
void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        shuffle();
        break;

    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QList<Node *> nodes;
    const QList<QGraphicsItem *> items = scene->items();
    for (QGraphicsItem *item : items) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    for (Node *node : qAsConst(nodes))
        node->calculateForces();

    bool itemsMoved = false;
    for (Node *node : qAsConst(nodes)) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}


#if QT_CONFIG(wheelevent)

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow(2., -event->angleDelta().y() / 240.0));
}

#endif


void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphWidget::shuffle()
{
    const QList<QGraphicsItem *> items = scene->items();
    for (QGraphicsItem *item : items) {
        if (qgraphicsitem_cast<Node *>(item))
            item->setPos(-Random_num + QRandomGenerator::global()->bounded(Random_num*2),
                         -Random_num + QRandomGenerator::global()->bounded(Random_num*2));
    }
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}

void GraphWidget::mousePressEvent(QMouseEvent * mouseEvent){
    if (flag_node){

        Node *nd=new Node(this);
        nd->set_name(value);

        //check for first node
        QList <QGraphicsItem*> items=scene->items();
        QGraphicsItem *item;
        int i=0,flag=0;
        while(i<items.count()){
            item=items[i];
            if(qgraphicsitem_cast<Node *>(item)){
                i=items.count();
                flag=1;
            }
            ++i;
        }

        //node to add
        scene->addItem(nd);
        QPoint po=mouseEvent->pos();
        nd->setPos(mapToScene(po));
        nd->setZValue(Z_top);

        //start node
        if (!flag){
            Node *node1=new Node(this);
            node1->setPos(0,0);
            node1->set_name("0");
            node1->setZValue(Z_top);

            //edge from start
            scene->addItem(node1);
            scene->addItem(new Edge(node1,nd));
        }

        flag_node=false;
        update();

    }
    else if (flag_edge){
        if (from){
            QPoint po=mouseEvent->pos();
            QGraphicsItem *item=scene->itemAt(mapToScene(po),QTransform());

            if((to=qgraphicsitem_cast<Node *>(item))){
                bool f=1;
                for(Edge* edge:from->edges()){
                    if(edge->sourceNode()==from&&edge->destNode()==to){
                        f=0;
                    }
                }
                if(f){
                    Edge *ed =new Edge(from,to);
                    TextEdge *text= new TextEdge(value);
                    text->set_edge(ed);
                    text->setZValue(Z_bottom);
                    ed->setZValue(0);

                    scene->addItem(ed);
                    scene->addItem(text);

                    flag_edge=false;
                }
                from=to=nullptr;
            }
        }
        else{
            QGraphicsItem *item=QGraphicsView::itemAt((mouseEvent->pos()));
            from=qgraphicsitem_cast<Node *>(item);
        }
    }
    else if(flagD_node){
        QPoint po=mouseEvent->pos();
        TextEdge *line;
        QList <TextEdge *> text;
        Node *nd=qgraphicsitem_cast<Node *>(scene->itemAt(mapToScene(po),QTransform()));
        for(QGraphicsItem *it:scene->items()){
            if((line=qgraphicsitem_cast<TextEdge *>(it))){
                text.append(line);
            };
        }

        for(Edge * ed:nd->edges()){
            for(TextEdge *ln:text){
                if(ln->get_edge()==ed){
                    scene->removeItem(ln);
                }
            }
            scene->removeItem(ed);
        }

        scene->removeItem(nd);
        flagD_node=false;
    }
    else if(flagD_edge){
        QPoint po=mouseEvent->pos();
        Edge *ed;
        QList <Edge*> edges;

        for (QGraphicsItem *item:scene->items()){
            if((ed=qgraphicsitem_cast<Edge *>(item))){
                edges.append(ed);
            };
        }

        for(Edge *ed:edges){
            ed->setZValue(Z_top);
        }

        ed=qgraphicsitem_cast<Edge *>(scene->itemAt(mapToScene(po),QTransform()));

        TextEdge *line;
        QList <TextEdge *> text;

        for(QGraphicsItem *it:scene->items()){
            if((line=qgraphicsitem_cast<TextEdge *>(it))){
                text.append(line);
            };
            if(Node *nod=qgraphicsitem_cast<Node *>(it)){
               if(nod->edges().indexOf(ed)!=-1){
                   nod->removeEdge(ed);
               };
            };

        }

        for(TextEdge *ln:text){
            if(ln->get_edge()==ed){
                scene->removeItem(ln);
            }
        }

        scene->removeItem( ed);

        for(Edge *ed:edges){
            ed->setZValue(Z_bottom);
        }
        flagD_edge=false;
    }
    else
    {
        QGraphicsView::mousePressEvent(mouseEvent);
    }

}

void GraphWidget::set_edge_flag()
{
    flag_edge=true;
}

void GraphWidget::set_node_flag()
{
    flag_node=true;
}

void GraphWidget::clear_area()
{
    for(QGraphicsItem *item: scene->items()){
        scene->removeItem(item);
    }
}

void GraphWidget::set_value(QString val){
    value=val;
}

void GraphWidget::add_existing(QList<QString> list){
    int cur_pos=0;
    QList <Node*> nodes;
    Node *to,*from;
    while(cur_pos<list.length()){
        QString val=list[cur_pos],val2=list[cur_pos+1];
        from=to=nullptr;
        for(Node* node:nodes){
            if(node->get_name()==val){
                from=node;
                val=Empty;
            }

            if(node->get_name()==val2){
                to=node;
                val2=Empty;
            }
        }

        if(val!=Empty){
            Node* nd=new Node(this);
            nd->setZValue(Z_top);
            nd->set_name(list[cur_pos]);
            nodes.append(nd);

            scene->addItem(nd);
            nd->setPos(QRandomGenerator::global()->bounded(Random_num+100),
                       QRandomGenerator::global()->bounded(Random_num+100));
            from=nd;
        }

        if(val2!=Empty){
            Node* nd=new Node(this);
            nd->setZValue(Z_top);
            nd->set_name(list[cur_pos+1]);
            nodes.append(nd);

            scene->addItem(nd);
            nd->setPos(QRandomGenerator::global()->bounded(Random_num+100),
                       QRandomGenerator::global()->bounded(Random_num+100));
            to=nd;
        }

        Edge*ed=new Edge(from,to);
        TextEdge *text=new TextEdge(list[cur_pos+2]+RazdSig+list[cur_pos+3]);
        text->set_edge(ed);

        scene->addItem(text);
        scene->addItem(ed);
        ed->setZValue(Z_bottom);
        cur_pos+=4;
    }
}

QList <QString> GraphWidget::save(){
    QList <Node*> nodes;
    QList <TextEdge*> texts;
    QList <QString> list;


    for(QGraphicsItem* item:scene->items()){
       Node* node;
       TextEdge *text;
       if((node=qgraphicsitem_cast<Node *>(item))){
           nodes.append(node);
       }
       if((text=qgraphicsitem_cast<TextEdge*>(item))){
           texts.append(text);
       }
    }

    for(Node* node:nodes){
        for(Edge *edge:node->edges()){
            if(edge->sourceNode()==node){
               int cur_pos=0;
               TextEdge *text;
               QString val;
               while(cur_pos<texts.length()){
                   text=texts[cur_pos];
                   if(text->get_edge()==edge){
                       val=text->get_value();
                       cur_pos=texts.length();
                   }
                   else{
                       cur_pos+=1;
                   }

               }
               int index=val.indexOf(RazdSig);
               list.append(node->get_name()+RazdLine);
               list.append(edge->destNode()->get_name()+RazdLine);
               list.append(val.left(index)+RazdLine);
               list.append(val.mid(index+1)+RazdLine);
            }
        }
    }
    return list;

}

void GraphWidget::set_Dedge_flag(){
    flagD_edge=true;
}

void GraphWidget::set_Dnode_flag(){
    flagD_node=true;
}

void GraphWidget::clear_flags(){
    flag_edge=flag_node=flagD_edge=flagD_node=false;
}
