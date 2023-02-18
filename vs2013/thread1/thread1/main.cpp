#include<iostream>
#include <thread>

using std::thread;
using std::cout;

//���� 
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
    //���ִ�����ʽ ���� �������� lambda ��Ա����Ĭ�ϲ�����һ��Ϊthisָ�뼴�����ַ��
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