#include "q8.h"
#include <QSizePolicy>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include<QString>
#include <QPushButton>
#include"thread.h"
#include <QtWidgets/qwidget.h>
#include"snake.h"
#include"view.h"
#include <QTimer>
#include <QKeyEvent>

Q8::Q8(QWidget *parent)
    : QMainWindow( parent )
{
    ui.setupUi(this);
    this->setWindowTitle( QStringLiteral( "贪吃蛇" ) );
    work = new MyThread(this);
    View *t1 = new View(this);
    snake = new Snake( t1 );
    t1->set_snake( snake );
    connect( work , &MyThread::timeOut , t1 , &View::processTimeOut );
    connect( this , &Q8::setKeySignal , t1, &View::setKey);
    
    t1->setMinimumWidth( 800 );
    t1->setMinimumHeight( 600 );
    t1->setSizePolicy( QSizePolicy::Expanding , QSizePolicy::Expanding );
    setCentralWidget( t1 );

    QPushButton *button1 = new QPushButton( QStringLiteral("开始游戏") );
    connect( button1 , &QPushButton::clicked , this , [ = ]( ) {work->start( ); } );
    QPushButton *button2 = new QPushButton( QStringLiteral("暂停/继续") );
    //发射信号killtimer new Qtimer继续
    connect( button2 , &QPushButton::clicked , work , &MyThread::stop_or_continue );
    button1->setFocusPolicy( Qt::NoFocus );
    button2->setFocusPolicy( Qt::NoFocus );
    QVBoxLayout * vlayout = new QVBoxLayout(t1);
    QHBoxLayout *hlayout = new QHBoxLayout( );
    hlayout->addWidget( button1 );
    hlayout->addWidget( button2 );
    vlayout->addStretch( );
    vlayout->addLayout( hlayout );
    
}

Q8::~Q8( ) { }

void Q8::keyPressEvent( QKeyEvent *event ) {
    int key = event->key( );
    switch (key)
    {
    case Qt::Key_Up: setKeySignal(-1) ; break;
    case Qt::Key_Down:setKeySignal( 1 ); break;
    case Qt::Key_Left: setKeySignal( 2 ); break;
    case Qt::Key_Right: setKeySignal( -2 ); break;
    default:
    	break;
    }
}
