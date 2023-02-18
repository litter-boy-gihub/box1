#include "painter.h"
#include <QColor>
#include <QPainter>
#include <QMouseEvent>
#include <QPen>
#include<QtMath>
painter::painter( QWidget *parent )
    : QMainWindow( parent ) , pre_point( QPoint( 0 , 0 ) ) , cur_point( QPoint( 0 , 0 ) ) ,\
    letf_cur_point( QPoint( 0 , 0 ) ) , letf_pre_point( QPoint( 0 , 0 ) ) , off_point( 0,0),
    rotae( 0 ) , scale(1,1 )
{
    ui.setupUi(this);
    QColor white = qRgb( 255 , 255 , 255 );
    image =new QImage( width() , height() , QImage::Format_RGB32 );
    image->fill( white );
    update( );



}

painter::~painter()
{

}

void painter::mousePressEvent( QMouseEvent *event ) {
    if( ( QApplication::keyboardModifiers( ) == Qt::ControlModifier ) && ( event->button( ) == Qt::LeftButton ) ) {
        letf_pre_point = event->pos( );
        return;
    }
    pre_point = event->pos( );
    pre_point.setX( pre_point.x( ) - off_point.x( ) );
    pre_point.setY( pre_point.y( ) - off_point.y( ) );//减去坐标偏移

    //缩放
    pre_point.setX( pre_point.x( )/scale.first );
    pre_point.setY( pre_point.y( )/scale.second );

    //旋转
    const double PI = 3.1415926535897932384626433;
    pre_point.setX( pre_point.y( )*qSin( rotae*PI / 180 ) + pre_point.x( )*qCos( rotae*PI / 180 ) );
    pre_point.setY( pre_point.y( )*qCos( rotae*PI / 180 ) - pre_point.x( )*qSin( rotae*PI / 180 ) );
}


void painter::mouseMoveEvent( QMouseEvent *event ) {
    
    //ctr + letfbutton
    if(  QApplication::keyboardModifiers( ) == Qt::ControlModifier ) {
        letf_cur_point = event->pos( );
        off_point += letf_cur_point - letf_pre_point;
        update( );

        letf_pre_point = letf_cur_point;
        return;
    }
    
    QPainter painter(image );
    QPen pen;
    pen.setColor( Qt::black );
    pen.setWidth( 1 );
    painter.setPen( pen );
    cur_point = event->pos( );//屏幕坐标系
    
    //平移
    cur_point.setX( cur_point.x( ) - off_point.x( ) );
    cur_point.setY( cur_point.y( ) - off_point.y( ) );//减去坐标偏移

    //缩放
    cur_point.setX( cur_point.x( )/scale.first );
    cur_point.setY( cur_point.y( )/scale.second );

    //旋转
    const double PI = 3.1415926535897932384626433;
    cur_point.setX( cur_point.y( )*qSin( rotae*PI / 180 ) + cur_point.x( )*qCos( rotae*PI / 180 ) );
    cur_point.setY( cur_point.y( )*qCos( rotae*PI / 180 ) - cur_point.x( )*qSin( rotae*PI / 180 ) );

    painter.drawLine(pre_point,cur_point );
    update( );
    pre_point = cur_point;

    
}

void painter::paintEvent( QPaintEvent *event ) {
    QPainter *paint = new QPainter( this );
    paint->translate( off_point );
    paint->scale( scale.first , scale.second );
    paint->rotate( rotae );
    paint->drawImage( QPoint( 0, 0 ) , *image );
}

void painter::keyPressEvent( QKeyEvent *event ) {
    switch( event->modifiers() ) {
    case Qt::AltModifier:
        if( event->key( ) == Qt::Key_Up ) {
            rotae += 10;
            update( );
            break;
        }
        if( event->key( ) == Qt::Key_Down ) {
            rotae -= 10;
            update( );
            break;
        }
    case Qt::ShiftModifier:
        if( event->key( ) == Qt::Key_Up ) {
            scale.first += 0.2;
            scale.second += 0.2;
            update( );
            break;
        }
        if( event->key( ) == Qt::Key_Down ) {
            scale.first -= 0.2;
            scale.second -= 0.2;
            update( );
            break;
        }
    default:
        break;
    }
}
