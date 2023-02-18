#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QtWidgets/QWidget>
#include "ui_treewidget.h"

class treeWidget : public QWidget
{
    Q_OBJECT

public:
    treeWidget(QWidget *parent = 0);
    ~treeWidget();
    bool is_imput_empty( );
public slots:
    void set_item( QTreeWidgetItem *item , int col );
    void add( );
    void del( );
    void change( );

private:
    QTreeWidgetItem *rootItem;
    Ui::treeWidgetClass ui;
};

#endif // TREEWIDGET_H
