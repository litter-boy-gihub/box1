#include "view.h"
#include<QPainter>
#include<QPen>
#include<QBrush>
#include <QMessageBox>
#include <QTime>

void View::processTimeOut( ) {
    //QMessageBox::warning( this , "" , "timeout" );
    next_step = true;
    auto_update = false;
    //TODO ����
    QPoint &&head = snake->get_head( );
    if(food_exist && head == *food_position ) {
        snake->eat( );
        food_exist = false;
    }
    update( );
}

void View::paintEvent( QPaintEvent *event ) {
    //��ô������ÿ�ζ����Ʊ��������ñ��⣬Ĭ��ÿ�ζ�����ձ����ٻ���paintevent
    draw_background( );
    if(!food_exist ) {
        food_position = new  QPoint( create_food( ) );
        food_exist = true;
    }
    if(food_exist) draw_food( );
    //��������
    if( next_step == true ) {
        snake->direction_not_fron_key = snake->get_direction();//�ֶ����ò��ᱻ�����޸ĵķ���
        snake->step();
        //��������
        next_step = false;
    }
    else {
        //��ԭ�ػ�����
        snake->draw( );
    }
    return;
}


void View::setKey( int key ) {
    if( abs(snake->get_direction()) == abs( key ) | !food_exist) return;
    snake->set_direction(key);
}


void View::draw_background( ) {
    QPainter par( this );
    QPen pen;
    pen.setWidth( 1 );
    pen.setColor( qRgb( 0 , 0 , 0 ) );
    QBrush brush;
    brush.setColor( qRgb( 135 , 162 , 86 ) );
    brush.setStyle( Qt::BrushStyle::SolidPattern );
    QRect rect( 50 , 10 , 735 , 525 );
    par.setPen( pen );
    par.setBrush( brush );
    par.drawRect( rect );
    QPoint beg;
    QPoint end;
    for( int i = 50; i <= 785; i += 21 ) {
        beg.setX( i );
        beg.setY( 10 );
        end.setX( i );
        end.setY( 535 );
        par.drawLine( beg , end );
    }
    for( int i = 10; i <= 535; i += 21 ) {
        beg.setX( 50 );
        end.setX( 785 );
        beg.setY( i );
        end.setY( i );
        par.drawLine( beg , end );
    }
}

QPoint View::create_food( ) {
    bool f = true;
    int x=0,y=0;
    while( f ) {
        QTime time = QTime::currentTime( );
        qsrand( time.second( ) );
        x = qrand( ) % 35;
        y = qrand( ) % 25;
        f = snake->find_body( x , y );//ֱ���Ҳ���
    }
    QPoint ret( x , y );
    return ret;
}

void View::draw_food( ) {
    int x = food_position->rx( );
    int y = food_position->ry( );
    QRect rect( 21 * x + 51 , 21 * y + 11 , 19 , 19 );

    //���û���
    QPainter paint(this);
    QPen pen;
    pen.setWidth( 1 );
    pen.setColor( Qt::red );
    QBrush brush;
    brush.setColor( Qt::red );
    brush.setStyle( Qt::BrushStyle::SolidPattern );
    paint.setPen( pen );
    paint.setBrush( brush );
    paint.drawRect( rect );
}


