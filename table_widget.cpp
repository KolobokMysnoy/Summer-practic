#include "table_widget.h"
#include "styles.h"

#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QRegularExpression>
#include <QFileDialog>

//digit or not
QRegularExpression re("\\d*");

const int Max=1, Min=0;
const QString Filter="Text file(*.txt)", FileTitle="Save file",FileTitleLo="Open file",
    StandartPlace="C://",FirstLine="example program v 0.2",
    Enter="\n",Empty="";
const char RazdLine=':';

Table_widget::Table_widget()
{
    //buttons
    QPushButton *button_add=new QPushButton("Add line",this);
    QPushButton *button_save=new QPushButton("Save",this);
    QPushButton *button_add_table=new QPushButton("Add existing",this);
    QPushButton *button_exit=new QPushButton("Back",this);
    QPushButton *button_delete=new QPushButton("Delete",this);

    //buttons style
    button_add->setStyleSheet(styles::get_button_style());
    button_save->setStyleSheet(styles::get_button_style());
    button_add_table->setStyleSheet(styles::get_button_style());
    button_exit->setStyleSheet(styles::get_button_style());
    button_delete->setStyleSheet(styles::get_button_style());

    //labels
    QLabel * lab1=new QLabel(this);
    QLabel * lab2=new QLabel(this);
    QLabel * lab3=new QLabel(this);
    QLabel * lab4=new QLabel(this);

    lab1->setText("Source");
    lab2->setText("Destination");
    lab3->setText("Signal B");
    lab4->setText("Signal");


    //layouts
    QHBoxLayout *lab_lay=new QHBoxLayout();
    lay = new QVBoxLayout();
    where_to_add=new QVBoxLayout();

    lab_lay->addWidget(lab1);
    lab_lay->addWidget(lab2);
    lab_lay->addWidget(lab3);
    lab_lay->addWidget(lab4);

    lay->addLayout(lab_lay);
    lay->addLayout(where_to_add);

    lay->addWidget(button_add);
    lay->addWidget(button_save);
    lay->addWidget(button_add_table);
    lay->addWidget(button_delete);
    lay->addWidget(button_exit);


    //spacer
    lay->addStretch(0);

    //connects
    QObject::connect(button_add,&QPushButton::clicked,[this]{this->form_stork(where_to_add);});
    QObject::connect(button_save,&QPushButton::clicked,[this]{this->save();});
    QObject::connect(button_add_table,&QPushButton::clicked,[this]{this->add_table();});
    QObject::connect(button_exit,&QPushButton::clicked,[this]{this->close();});
    QObject::connect(button_delete,&QPushButton::clicked,[this]{this->delete_line();});

    //settings
    this->setStyleSheet(styles::get_main_background());
    this->setWindowTitle("Table");
    this->setLayout(lay);
}

void Table_widget::add_table()
{
    refresh();

    //open file
    QString file_name=QFileDialog::getOpenFileName(this,FileTitleLo,StandartPlace,Filter);
    file.setFileName(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    //if not our files
    if(in.readLine()!=FirstLine){
        return;
    }

    //input from file
    QList <QString> strings;
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

    //form strok to window
    int cur_pos=0;
    while(cur_pos<strings.length())
    {
        form_stork(where_to_add,strings[cur_pos],strings[cur_pos+1],
                strings[cur_pos+2],strings[cur_pos+3]);
        cur_pos+=4;
    }

    //close file
    file.close();
}

void Table_widget::refresh()
{
//    clear all fields
    QList <QLineEdit *> lines= this->findChildren<QLineEdit*>();
    for(QLineEdit * line:lines){
        delete line;
    }

    QList <QSpinBox *> spins= this->findChildren<QSpinBox*>();
    for(QSpinBox * spin:spins){
        delete spin;
    }

    QList <QHBoxLayout*> boxes=lay->findChildren<QHBoxLayout*>();
    int x=0;
    for (QHBoxLayout* item : boxes) {
        if(x!=0){
            delete item;
        }
        x+=1;
    }
}

void Table_widget::form_stork(QVBoxLayout *layout_where_add,QString node_from,
                              QString dest_node,QString signal_letter,QString signal_to)
{
    //form line
    QLineEdit *from = new QLineEdit(node_from,this);
    QLineEdit *to_node= new QLineEdit(dest_node,this);
    QLineEdit *signal=new QLineEdit(signal_letter,this);
    QSpinBox *signal_digit=new QSpinBox(this);

    //settings
    //spin
    signal_digit->setMaximum(Max);
    signal_digit->setMinimum(Min);

    signal_digit->setStyleSheet(styles::get_line());
    from->setStyleSheet(styles::get_line());
    to_node->setStyleSheet(styles::get_line());
    signal->setStyleSheet(styles::get_line());

    if(re.match(signal_to).hasMatch()){
        signal_digit->setValue(signal_to.toInt());
    }

    //add line
    QHBoxLayout *hbox=new QHBoxLayout();

    hbox->addWidget(from);
    hbox->addWidget(to_node);
    hbox->addWidget(signal);
    hbox->addWidget(signal_digit);

    layout_where_add->addLayout(hbox);
}

void Table_widget::save()
{
    //location to save
    QString file_name=QFileDialog::getSaveFileName(this,FileTitle,StandartPlace,Filter);
    file.setFileName(file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    //header of file
    out<<FirstLine<<Enter;

    int i=0;
    QList <QString> text;
    for(QLineEdit *tmp_lay:this->findChildren<QLineEdit*>()){
        if (i!=0 && i%4==0){
            if(text.count()==4){
                out<<text[0]<<RazdLine<<text[1]<<RazdLine<<text[2]
                           <<RazdLine<<text[3]<<RazdLine;
                out<<Enter;
            }
            text.clear();
        }

        //check if line is empty
        QString line=tmp_lay->text();
        if (!line.isEmpty()&&text.count()<4){
             if(i%4==1&&text[0]=="0"){
                text.append(line);
                text.append("");
                text.append("");
             }
             else
             {
                 text.append(line);
             }
         }

        ++i;
    }
    if(text.count()==4){
        out<<text[0]<<RazdLine<<text[1]<<RazdLine<<text[2]
                   <<RazdLine<<text[3]<<RazdLine;
        out<<Enter;
    }
    text.clear();

    file.close();

}

void Table_widget::delete_line()
{
    QList <QLineEdit*> lines=this->findChildren<QLineEdit*>();
    QList <QSpinBox*> spins=findChildren<QSpinBox*>();

    int i=lines.count()-2;
    delete lines[i];
    delete lines[i-1];
    delete lines[i-2];
    delete spins[spins.count()-1];

}

Table_widget::~Table_widget(){
    this->refresh();
}
