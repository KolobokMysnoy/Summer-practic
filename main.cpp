#include "graphwidget.h"
#include "table_widget.h"
#include "graphwindow.h"
#include "styles.h"

#include <QApplication>
#include <QTime>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QMainWindow mainWindow;

    QPushButton *button_to_pic= new QPushButton("Picture",&mainWindow);
    QPushButton *button_to_block= new QPushButton("Block",&mainWindow);
    QPushButton *button_to_exit= new QPushButton("Exit",&mainWindow);

    button_to_block->setStyleSheet(styles::get_button_style());
    button_to_pic->setStyleSheet(styles::get_button_style());
    button_to_exit->setStyleSheet(styles::get_button_style());

    QVBoxLayout *main_lay=new QVBoxLayout;
    QWidget *wid=new QWidget;

    Graphwindow * grap=new Graphwindow;
    Table_widget *tb1=new Table_widget;

    //connect button
    QObject::connect(button_to_pic,&QPushButton::clicked,[grap]{grap->show();});
    QObject::connect(button_to_block,&QPushButton::clicked,[tb1]{tb1->show();});
    QObject::connect(button_to_exit,&QPushButton::clicked,[]{qApp->closeAllWindows();});

    //layouts
    main_lay->addWidget(button_to_block);
    main_lay->addWidget(button_to_pic);
    main_lay->addWidget(button_to_exit);
    wid->setLayout(main_lay);

    mainWindow.setCentralWidget(wid);

    //view
    mainWindow.setWindowTitle("Choose");
    mainWindow.setStyleSheet(styles::get_main_background());

    mainWindow.show();
    return app.exec();
}
