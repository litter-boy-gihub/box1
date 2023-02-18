#ifndef NODEPAD_H
#define NODEPAD_H

#include <QtWidgets/QMainWindow>
#include "ui_nodepad.h"

class Nodepad : public QMainWindow
{
    Q_OBJECT

public:
    Nodepad(QWidget *parent = 0);
    ~Nodepad();
public slots:
void open( bool );
void save( bool );
void exit( bool );
void selectall( bool );
void del( bool );
void help( bool );
void about( bool );


private:
    Ui::NodepadClass ui;
    QString fileName;
};

#endif // NODEPAD_H
