#include"thread.h"
#include <QTimer>
#include<QTimerEvent>
void MyThread::run() {
    timer = new QTimer();
    timer->start( 500 );
    connect( timer , &QTimer::timeout , this , &MyThread::emit_timeOut );
    QThread::exec( );//事件循环 接收信号
}

void MyThread::emit_timeOut( ) {
    emit timeOut( );
}

void MyThread::stop_or_continue( ) {
    int id = timer->timerId( );
    if( id == -1 ) {
        timer->stop( ); //todo 怎么释放timer的内存，delete报错、、、
        timer = new QTimer( );
        timer->start(500);
        connect( timer , &QTimer::timeout , this , &MyThread::emit_timeOut );
    }
    else {
        timer->stop( );
        
    }
}
