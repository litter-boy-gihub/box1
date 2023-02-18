#include <algorithm>
#include<thread>
#include <mutex>
#include<windows.h>


#pragma  comment(lib,"user32.lib")

//��������
template<typename T>
bool quick_sort( T* arr , int beg , int end ) {
    if( arr == nullptr ) return false;
    if( beg >= end ) return true;
    int left = beg + 1 , right = end;
    while( left <= right ) {
        if( arr [left] <= arr [beg] ) { //��arr[beg]��Ϊpivot
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

//��·�鲢����
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
    //merge ��p1 �� p2 ���뵽 beg��end��
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
    Myintager(int i) {      //c++ primer ���ᵽ�������õ����βε��õĹ��캯�������˴��βε������͵�һ����ʽ����ת����//�����ж���βε��ǳ�һ���β�����������Ĭ��ֵ��
        m_int = i;          //����ʹ��explicit ��ֹ�ӵ�������������ʽ����ת����
        cout_mtx.lock( );
        printf( "���캯����ʽת��" );
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