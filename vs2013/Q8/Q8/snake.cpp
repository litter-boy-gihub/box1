#include"snake.h"
#include <QPen>
#include<QBrush>
#include <QPainter>
Snake::Snake( QWidget *parent /*= 0 */ ) :QWidget( parent ) {
    init( );
}

void Snake::init( ) {
    //初始时，蛇身长为2，位置如下
    if( body.empty( ) ) {
        this->body.push_back( new QPoint( 17 , 20 ) );
        this->body.push_back( new QPoint( 17 , 21 ) );
    }
}
//前进一步（前提是刚插入到头)
void Snake::step( ) {
    //画出来每一个尾巴和头来，尾巴根据前一个坐标获得方向，头根据后一个获得方向，尾巴用空白颜色画，头用身体颜色画。
    QPoint back = ( *body.back( ) );
    //更新坐标数据下一步
    for( auto it = body.rbegin( ); it+1 != body.rend( ); it++ ) {
        auto ptr = it;
        ptr++;
        int x = ( *ptr )->x( );
        int y = ( *ptr )->y( );
        ( *it )->setX( x );
        ( *it )->setY( y );
    }
    auto it = body.begin( );
    int temp = 0;
    //上-1 下1 左2 右-2
    QPoint off(0,0);
    switch( this->direction_not_fron_key ) {
    case -1:
        temp = ( ( *it )->y( ) - 1 ) % 25;
        if( temp < 0 ) temp += 25;
        ( *it )->setY( temp ); 
        off.setY( -1 );
        break;
    case  1:
        ( *it )->setY( ( ( *it )->y( ) + 1 ) % 25 ); 
        off.setY( 1 );
        break;
    case  2:
        temp = ( ( *it )->x( ) - 1 ) % 35;
        if( temp < 0 ) temp += 35;
        ( *it )->setX( temp ); 
        off.setX( -1 );
        break;
    case -2:
        ( *it )->setX( ( ( *it )->x( ) + 1 ) % 35 ); 
        off.setX( 1 );
        break;
    default:
        break;
    }

    //绘制
    draw( );
    
}
//需保证下一个位置不越界，需要外界判断
void Snake::eat( ) {
    QPoint *add = new QPoint( 0 , 0 );
    auto it = body.rbegin();
    auto it2 = it;
    it2++;
    int x = ( *it )->rx( ) - ( *it2 )->rx( );
    int y = ( *it )->ry( ) - ( *it2 )->ry( );
    //TODO 可以不处理越界step自动更新下一个
    add->setX( ( *it )->rx( )+ x );
    add->setY( ( *it )->ry( )+ y );
    body.push_back( add );
}

bool Snake::find_body( int x , int y ) {
    for( auto it = body.begin( ); it != body.end( );it++ )
    {
        if( ( *it )->rx( ) == x && ( *it )->ry( ) == y ) return true;
    }
    return false;
}

void Snake::draw( ) {
    //设置画笔
    QPainter paint( qobject_cast<QWidget*>( this->parent( ) ) ); //todo: 设置device（父指针）
    QPen pen;
    pen.setWidth( 1 );
    pen.setColor( Qt::yellow );
    QBrush brush;
    brush.setColor( Qt::yellow );
    brush.setStyle( Qt::BrushStyle::SolidPattern );
    paint.setPen( pen );
    paint.setBrush( brush );

    //绘制身体
    for( auto it = body.begin( ); it != body.end( ); it++ ) {
        int x = ( *it )->x( );
        int y = ( *it )->y( );
        QRect rect( 21 * x + 51 , 21 * y + 11 , 19 , 19 );
        paint.drawRect( rect );
    }
}
