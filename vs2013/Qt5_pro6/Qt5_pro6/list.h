#ifndef LIST_H
#define LIST_H

#include <QtWidgets/QMainWindow>
#include "ui_list.h"

class List : public QMainWindow
{
    Q_OBJECT

public:
    List(QWidget *parent = 0);
    ~List();

public slots:
    void add( );
    void del( );
    void change( );
    void update( );

private:
    Ui::ListClass ui;
    QListWidgetItem *cur;
};

#endif // LIST_H
