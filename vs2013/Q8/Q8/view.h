#ifndef VIEW_H
#define VIEW_H
#include<QWidget>
#include"snake.h"

class View:public QWidget{
    Q_OBJECT
public:
    View( QWidget *parent = 0 ):QWidget( parent ) {
    };
    ~View( ) { };
    void set_snake(Snake * i_snake ) {
        this->snake = i_snake;
    }
    bool snake_isvalid( ) {
        if( snake == nullptr ) return false;
        return true;
    }
    QPoint create_food( );
public slots:
    void processTimeOut( );//处理事件循环，间隔时间走一步
    void setKey(int key );//接收gui线程事件输入
protected:
    void paintEvent( QPaintEvent *event ) override;
private:
    void draw_food( );
    void draw_background( );
    Snake *snake = nullptr;
    bool next_step = false;
    bool auto_update = true;
    bool food_exist = false;
    QPoint *food_position = nullptr;
};

#endif