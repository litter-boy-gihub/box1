#include <algorithm>
#include<thread>
#include <mutex>
#include<windows.h>


#pragma  comment(lib,"user32.lib")

//快速排序
template<typename T>
bool quick_sort( T* arr , int beg , int end ) {
    if( arr == nullptr ) return false;
    if( beg >= end ) return true;
    int left = beg + 1 , right = end;
    while( left <= right ) {
        if( arr [left] <= arr [beg] ) { //以arr[beg]作为pivot
            left++;
        }
        else {
            std::swap( arr [left] , arr [right] );
            right--;
        }
    }
    std::swap( arr [beg] , arr [right] );
    quick_sort( arr , beg , right - 1 );
    quick_sort( arr , right + 1 , end );
    return true;
}

//二路归并排序
template<typename T>
bool binary_merge_sort( T* arr , int beg , int end ) {
    if( arr == nullptr ) return false;
    if( beg == end ) return true;
    int mid = floor( ( beg + end ) / 2.0 );
    T * p1 = new T [mid - beg + 1];
    T * p2 = new T [end - mid];
    int len_left = mid - beg + 1;
    int len_right = end - mid;
    memcpy( p1 , arr , len_left*sizeof(T) );
    memcpy( p2 , arr + mid+1 , len_right*sizeof(T) );
    binary_merge_sort( p1 , beg , mid );
    binary_merge_sort( p2 , mid + 1 , end );
    //merge 把p1 和 p2 加入到 beg到end中
    for (int i = beg ; i <=end  ; i++)
    {
        if( len_left == 0 ) {
            while( len_right != 0 ) {
                arr [i++] = *p2++;
                len_right--;
            }
            break;
        }
        if( len_right == 0 ) {
            while( len_left != 0 ) {
                arr [i++] = *p2++;
                len_left--;
            }
            break;
        }

        if( *p1 <= *p2 ) {
            arr [i] = *p1++;
            len_left--;
        }
        else {
            arr [i] = *p2++;
            len_right--;
        }
    }

    delete[] p1;
    delete[] p2;
    return true;
}

void fun( char * str) {
    printf( "thread id: %d\n" , std::this_thread::get_id( ) );
    printf( "%s\n" , str );
    printf( "%p\n" , str );
}

std::mutex cout_mtx;
void exchange( int& a , int& b ) {
    if( a == b ) return;
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
    cout_mtx.lock( );
    printf( "a : %d b : %d\n" , a , b );
    cout_mtx.unlock( );
}

class Myintager
{
public:
    Myintager(int i) {      //c++ primer 中提到，可以用单个形参调用的构造函数定义了从形参到类类型的一个隐式类型转换。//可以有多个形参但是除一个形参外其他都有默认值。
        m_int = i;          //可以使用explicit 禁止从单个参数进行隐式类型转换。
        cout_mtx.lock( );
        printf( "构造函数隐式转换" );
        cout_mtx.unlock( );
    };
    int m_int;
};

struct Obj {
    void operator()(Myintager i) {
        cout_mtx.lock( );
        printf( "%d",i.m_int );
        cout_mtx.unlock( );
    };
};

int main(){
    char* str = "forevergarden";
    printf( "%p\n" , str );
    std::thread t( fun , str );
    t.join( );
    //int arr[] {9 , 7 , 4 , 2 , 1 };
    //bool b = quick_sort<int>( arr , 0 , 4 );
    //bool b = binary_merge_sort( arr , 0 , 4 );
    int a = 1 , b = 2;
    std::thread t1( exchange , std::ref( a ) , std::ref( b ) );
    cout_mtx.lock( );
    printf( "main:  a : %d b : %d\n" , a , b );
    cout_mtx.unlock( );
    t1.join( );

    Obj obj;
    std::thread t2( obj , 3 );
    t2.join( );

    POINT p;
    while( true ) {
        Sleep( 1000 );
        GetCursorPos( &p );
        printf( "%ld %ld\n",p.x,p.y );
    }
    return 0;
}