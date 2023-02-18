#ifndef SNAKE_H
#define SNAKE_H

#include <QWidget>
class Snake :public QWidget {
public:
    Snake( QWidget *parent = 0 );
    ~Snake( ) {
        qDeleteAll( body );
    };
    void eat( );
    void step();
    void init( );
    void draw( );
    int get_direction( ) {
        return now_direction;
    }
    void set_direction(int dirction ) {
        now_direction = dirction;
    }
    bool find_body( int x, int y);
    QPoint get_head( ) {
        return *body.front( );
    }
    QPoint get_head_off( ) {
        
        auto it = body.begin( );
        auto it2 = it;
        it2++;
        int x = ( *it )->rx( )  - ( *it2 )->rx( );
        int y = ( *it )->ry( )  - ( *it2 )->ry( );
        QPoint off( x  , y );
        return off;
    }
    int direction_not_fron_key = -1;
private:
    QList<QPoint*> body;
    const static int max_x = 35 - 1;//const成员只能够在声明时候（c11）和初始化列表中定义
    const static int max_y = 25 - 1;
    int frame;
    int now_direction = -1;
};

#endif