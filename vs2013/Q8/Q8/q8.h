#ifndef Q8_H
#define Q8_H

#include <QtWidgets/QMainWindow>
#include "ui_q8.h"
#include "snake.h"
#include "thread.h"


class Q8 : public QMainWindow
{
    Q_OBJECT

public:
    Q8(QWidget *parent = 0);
    ~Q8();
protected:
    void keyPressEvent( QKeyEvent *event );
signals:
    void setKeySignal( int key );
private:
    Ui::Q8Class ui;
    Snake *snake;
    MyThread *work;
    bool stop = false;
};

#endif // Q8_H
