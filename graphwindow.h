#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QWidget>
#include "graphwidget.h"

#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>

class Graphwindow : public QWidget
{
    Q_OBJECT
public:
    Graphwindow();
    ~Graphwindow();

    //add graph or save
    void add_existing();
    void save();

    //add node and edge
    void add_edge();
    void add_node();

private:
    GraphWidget* graph;
    QLineEdit * line;
    QSpinBox *line_signal;

};

#endif // GRAPHWINDOW_H
