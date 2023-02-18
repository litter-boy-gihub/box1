#include<iostream>
#include <thread>

using std::thread;
using std::cout;

//函数 
void myPrint( ) {
    std::cout <<"myPrint thread_id: "<< std::this_thread::get_id()<< std::endl;
}

class Obj {
public:
    void operator()( ) {
        std::cout << "obj thread_id: " << std::this_thread::get_id( ) << std::endl;
    }
    void memberf( ) {
        std::cout << "memeberf thread_id: " << std::this_thread::get_id( ) << std::endl;
    }
};

int main(){
    //四种创建方式 函数 函数对象 lambda 成员函数默认参数第一个为this指针即对象地址。
    thread t1 {myPrint };
    Obj obj;
    thread t2 { obj };
    thread t3( [ ]( int i ) {std::cout << "lambada thread_id: " << std::this_thread::get_id( ) << std::endl; } , 3 );
    thread t4( &Obj::memberf , &obj );
    t1.join( );
    t2.join( );
    t3.join( );
    t4.join( );

    getchar( );
	return 0;
}