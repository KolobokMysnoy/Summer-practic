#include "dialog.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>

dialog::dialog()
{
    QVBoxLayout *box=new QVBoxLayout(this);
    QPushButton *button=new QPushButton("Input",this);
    QLineEdit *line=new QLineEdit(this);

    box->addWidget(button);
    box->addWidget(line);

    this->setLayout(box);

    QObject::connect(&button_add_edge,&QPushButton::clicked,[this]{this->add_edge();});
}
