#include "graphwindow.h"
#include "styles.h"

#include <QFile>
#include <QTextStream>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QLabel>

#include <QRegion>

const int Max=1, Min=0;
const QString Filter="Text file(*.txt)", FileTitle="Save file",FileTitleLo="Open file",
    StandartPlace="C://",FirstLine="example program v 0.2",
    Enter="\n",Empty="";
const char RazdSig='/', RazdLine=':';

Graphwindow::Graphwindow() : QWidget()
{
    //graphwidget
    graph=new GraphWidget(this);

    //label
    QLabel *label_signla=new QLabel("Signal(0/1)",this);
    QLabel *label_name=new QLabel("Name of node or signal",this);

    //line
    line=new QLineEdit(this);
    line_signal=new QSpinBox(this);
    line_signal->setMaximum(Max);
    line_signal->setMinimum(Min);

    //Buttons
    QPushButton *button_add_edge=new QPushButton("Add edge",this);
    QPushButton *button_add_node=new QPushButton("Add node",this);

    QPushButton *button_Dedge=new QPushButton("Delete edge",this);
    QPushButton *button_Dnode=new QPushButton("Delete node",this);

    QPushButton *button_add_existing=new QPushButton("Open file",this);
    QPushButton *button_save=new QPushButton("Save file",this);
    QPushButton *button_clear=new QPushButton("Clear area",this);
    QPushButton *button_exit=new QPushButton("Back",this);


    //layouts
    QVBoxLayout* mainlay=new QVBoxLayout(this);
    QHBoxLayout* buttons_user=new QHBoxLayout();
    QHBoxLayout* buttons_sys=new QHBoxLayout();
    QHBoxLayout* buttons_delete=new QHBoxLayout();

    QHBoxLayout* input_lay=new QHBoxLayout();
    QVBoxLayout* input_left=new QVBoxLayout();
    QVBoxLayout* input_right=new QVBoxLayout();

    //Add buttons to layouts
    mainlay->addWidget(graph);
    mainlay->addLayout(input_lay);
    mainlay->addLayout(buttons_user);
    mainlay->addLayout(buttons_delete);
    mainlay->addLayout(buttons_sys);

    input_left->addWidget(label_name);
    input_left->addWidget(line);
    input_right->addWidget(label_signla);
    input_right->addWidget(line_signal);

    input_lay->addLayout(input_left);
    input_lay->addLayout(input_right);

    buttons_user->addWidget(button_add_node);
    buttons_user->addWidget(button_add_edge);

    buttons_delete->addWidget(button_Dedge);
    buttons_delete->addWidget(button_Dnode);

    buttons_sys->addWidget(button_add_existing);
    buttons_sys->addWidget(button_save);
    buttons_sys->addWidget(button_clear);

    mainlay->addWidget(button_exit);

    //Connects
    QObject::connect(button_add_existing,&QPushButton::clicked,[this]{graph->clear_area();this->add_existing();});
    QObject::connect(button_save,&QPushButton::clicked,[this]{this->save();});
    QObject::connect(button_clear,&QPushButton::clicked,[this]{graph->clear_area();});

    QObject::connect(button_Dedge,&QPushButton::clicked,[this]{graph->clear_flags() ;this->graph->set_Dedge_flag();});
    QObject::connect(button_Dnode,&QPushButton::clicked,[this]{graph->clear_flags() ;this->graph->set_Dnode_flag();});

    QObject::connect(button_add_edge,&QPushButton::clicked,[this]{graph->clear_flags() ;this->add_edge();});
    QObject::connect(button_add_node,&QPushButton::clicked,[this]{graph->clear_flags() ;this->add_node();});
    QObject::connect(button_exit,&QPushButton::clicked,[this]{this->close();});

    //buttons style
    button_add_edge->setStyleSheet(styles::get_button_style());
    button_add_existing->setStyleSheet(styles::get_button_style());
    button_add_node->setStyleSheet(styles::get_button_style());
    button_clear->setStyleSheet(styles::get_button_style());
    button_save->setStyleSheet(styles::get_button_style());
    button_exit->setStyleSheet(styles::get_button_style());
    button_Dedge->setStyleSheet(styles::get_button_style());
    button_Dnode->setStyleSheet(styles::get_button_style());

    //settings
    this->setStyleSheet(styles::get_main_background());
    this->setWindowTitle("Graph");
    line->setStyleSheet(styles::get_line());
    line_signal->setStyleSheet(styles::get_line());
}

void Graphwindow::save()
{
    QList<QString> list= graph->save();

    QFile file;
    QString file_name=QFileDialog::getSaveFileName(this,FileTitle,StandartPlace,Filter);
    file.setFileName(file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    int cur_pos=0;

    out<<FirstLine<<Enter;
    while(list.length()>cur_pos){
        out<<list[cur_pos]<<list[cur_pos+1]<<list[cur_pos+2]
          <<list[cur_pos+3]<<Enter;
        cur_pos+=4;
    }

    file.close();
}

void Graphwindow::add_existing()
{
    //Struct in file
    //Node_source:node_destination:signal_letter:signal_number
    QFile file;
    QString file_name=QFileDialog::getOpenFileName(this,FileTitleLo,StandartPlace,Filter);
    file.setFileName(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QList <QString> strings;
    if(in.readLine()!=FirstLine){
        return;
    }
    while (!in.atEnd()) {
        QString line = in.readLine();

        int x=0,y=0;
        y=line.indexOf(RazdLine,x+1);
        while(y!=-1){
            // from 2:3:4 --> (2)(3)(4)

            if(x==0){
                strings.append(line.mid(x,y));
            }
            else
            {
                strings.append(line.mid(x+1,y-x-1));
            }

            x=y;
            y=line.indexOf(RazdLine,x+1);
        }
    }

    graph->clear_area();
    graph->add_existing(strings);

    file.close();
}

void Graphwindow::add_edge(){
    line->show();
    if(line->text()!=Empty){
        graph->set_value(line->text()+RazdSig+line_signal->text());
        graph->set_edge_flag();
    }
}

void Graphwindow::add_node(){
    line->show();
    if(line->text()!=Empty){
        graph->set_value(line->text());
         graph->set_node_flag();
    }
}

Graphwindow::~Graphwindow()
{
    for(QLayout *lay : this->findChildren<QLayout*>()){
        delete lay;
    };
};
