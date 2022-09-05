#ifndef TABLE_H
#define TABLE_H

class Node;

class Line
{
public:

    Line(int number_node, int node_connect, int node_signal);
    void edit(int number_node, int node_connect, int node_signal);
    void edit_next_line(Line * line_to_add);

    int get_number_of_node();
    int get_number_of_connect();
    int get_node_signal();
    Line * get_next_line();

    ~Line();

private:

    Line* next;
    int number_of_node;
    int number_of_connect;
    int node_signal;
};

class Table
{
public:

    Table();

    void add_line(Line *line_to_add);
    Line* get_first_line();
    Line* get_last_line();

    ~Table();

private:

    Line *first_line;
    Line *last_line;
};

#endif // TABLE_H
