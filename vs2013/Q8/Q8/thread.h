#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <QTime>
#include <QTimer>
class MyThread : public QThread{
    Q_OBJECT
public:
    MyThread( QObject *parent = 0 ) :QThread( parent ) {
        time = new QTime( );
    };
    ~MyThread( ) { };
    QTimer *timer;
signals:
    void timeOut( );
public slots:
    void stop_or_continue( );
    void emit_timeOut( );
protected:
    void run() override;
private:
    QTime *time = nullptr;
    

};
#endif // Q8_H