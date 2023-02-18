#include<iostream>
#include <thread>
#include<Windows.h>
#include <mutex>
#include <condition_variable>

using std::thread;
using std::cout;

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

//���� 
void myPrint( int i ) {
    std::unique_lock<std::mutex> lck( mtx );
    while( !ready ) {
        cv.wait( lck );//�����ɹ�����lck.unlock ��notify ����lck.lock
    }
    std::cout << i << " thread_id: " << std::this_thread::get_id( ) << std::endl;
}

void go( ) {
    std::unique_lock<std::mutex> lck( mtx );
    ready = true;
    cv.notify_all( );
}

int main( ) {
    //ͬ���ͻ��⣬ͨ����������ʵ�ֻ������
    thread * ps [10];
    for( int i = 0; i < 10; i++ ) {
        ps [i] = new thread( myPrint , i );
    }

    go( );
    
    for( auto &t : ps ) {
        t->join( );
    }

    getchar( );
    return 0;
}