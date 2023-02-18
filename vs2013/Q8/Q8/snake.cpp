#include"snake.h"
#include <QPen>
#include<QBrush>
#include <QPainter>
Snake::Snake( QWidget *parent /*= 0 */ ) :QWidget( parent ) {
    init( );
}

void Snake::init( ) {
    //��ʼʱ������Ϊ2��λ������
    if( body.empty( ) ) {
        this->body.push_back( new QPoint( 17 , 20 ) );
        this->body.push_back( new QPoint( 17 , 21 ) );
    }
}
//ǰ��һ����ǰ���Ǹղ��뵽ͷ)
void Snake::step( ) {
    //������ÿһ��β�ͺ�ͷ����β�͸���ǰһ�������÷���ͷ���ݺ�һ����÷���β���ÿհ���ɫ����ͷ��������ɫ����
    QPoint back = ( *body.back( ) );
    //��������������һ��
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
    //��-1 ��1 ��2 ��-2
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

    //����
    draw( );
    
}
//�豣֤��һ��λ�ò�Խ�磬��Ҫ����ж�
void Snake::eat( ) {
    QPoint *add = new QPoint( 0 , 0 );
    auto it = body.rbegin();
    auto it2 = it;
    it2++;
    int x = ( *it )->rx( ) - ( *it2 )->rx( );
    int y = ( *it )->ry( ) - ( *it2 )->ry( );
    //TODO ���Բ�����Խ��step�Զ�������һ��
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
    //���û���
    QPainter paint( qobject_cast<QWidget*>( this->parent( ) ) ); //todo: ����device����ָ�룩
    QPen pen;
    pen.setWidth( 1 );
    pen.setColor( Qt::yellow );
    QBrush brush;
    brush.setColor( Qt::yellow );
    brush.setStyle( Qt::BrushStyle::SolidPattern );
    paint.setPen( pen );
    paint.setBrush( brush );

    //��������
    for( auto it = body.begin( ); it != body.end( ); it++ ) {
        int x = ( *it )->x( );
        int y = ( *it )->y( );
        QRect rect( 21 * x + 51 , 21 * y + 11 , 19 , 19 );
        paint.drawRect( rect );
    }
}
