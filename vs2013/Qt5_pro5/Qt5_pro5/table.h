#ifndef TABLE_H
#define TABLE_H

#include <QtWidgets/QWidget>
#include "ui_table.h"
#include <QTableWidgetSelectionRange>

class Table : public QWidget
{
    Q_OBJECT

public:
    Table(QWidget *parent = 0);
    ~Table();

public slots:
    void add( );
    void del( );
    void change( );
    void update( );
private:
    Ui::TableClass ui;
    int row;
    QList<QTableWidgetSelectionRange> ranges;
    QSet<int> set; //代表选中了哪些行
};

#endif // TABLE_H
