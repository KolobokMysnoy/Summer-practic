#include "table.h"

Line::Line(int number_node, int node_connect, int node_signal)
{
    //create a line
    number_of_node=number_node;
    number_of_connect=node_connect;
    this->node_signal=node_signal;
    next=nullptr;
}

void Line::edit(int number_node, int node_connect, int node_signal)
{
    number_of_node=number_node;
    number_of_connect=node_connect;
    this->node_signal=node_signal;
}

void Line::edit_next_line(Line * line_to_add)
{
    next=line_to_add;
}

int Line::get_number_of_node()
{
    return number_of_node;
}

int Line::get_number_of_connect()
{
    return number_of_connect;
}

int Line::get_node_signal()
{
    return node_signal;
}

Line * Line::get_next_line(){
    return next;
}

Line::~Line()
{
    delete next;
}

//----- Table

Table::Table()
{
    first_line=last_line=nullptr;
}

void Table::add_line(Line *line_to_add)
{
    if(!first_line)
    {
        first_line=last_line=line_to_add;
        return;
    }
    last_line->edit_next_line(line_to_add);
    last_line=line_to_add;
}

Line *Table::get_first_line()
{
    return first_line;
}

Line *Table::get_last_line()
{
    return last_line;
}

Table::~Table()
{
    delete first_line;
}
