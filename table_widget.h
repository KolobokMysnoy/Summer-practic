        #ifndef TABLE_WIDGET_H
#define TABLE_WIDGET_H

#include <QString>

#include <QWidget>
#include <QFile>
#include <QVBoxLayout>

class Table_widget : public QWidget
{
    Q_OBJECT
public:
    Table_widget();
    ~Table_widget();

private:
    QVBoxLayout *lay;
    QVBoxLayout *where_to_add;
    QFile file;

    //delete all lines
    void refresh();

    //add existing or save
    void add_table();
    void save();

    //delete line
    void delete_line();

    //form new clear line
    void form_stork(QVBoxLayout *layout_where_add,QString node_from="",
                                  QString dest_node="",QString signal_letter="",QString signal_to="");
};

#endif // TABLE_WIDGET_H
