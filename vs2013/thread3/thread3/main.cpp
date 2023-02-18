#include<iostream>
#include <thread>
#include<Windows.h>
#include <mutex>

using std::thread;
using std::cout;

std::mutex mtx;
int count = 0;

//²»×èÈû
void add_100() {
     for (int i = 0; i <10000  ; i++)
     {
         if( try_lock( mtx ) ) {
             count++;
             mtx.unlock( );
         }
     }
    
}
//×èÈû
void add_100_( ) {
    for( int i = 0; i < 10000; i++ ) { 
        mtx.lock( );
        count++;
        mtx.unlock( );
    }
}


int main( ) {

    thread * ps [10];
    for( int i = 0; i < 10; i++ ) {
        ps [i] = new thread(add_100);
    }
    for( auto &t : ps ) {
        t->join( );
    }
    cout <<"try_lock:" <<count << std::endl;
    for( auto &t : ps ) {
        delete t;
    }
    count = 0;
    for( int i = 0; i < 10; i++ ) {
        ps [i] = new thread( add_100_ );
    }
    for( auto &t : ps ) {
        t->join( );
    }
    cout << "lock:" << count << std::endl;

    getchar( );
    return 0;
}