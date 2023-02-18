#include <iostream>
#include <iostream>
#include <typeinfo>

using namespace std;

template<typename T>
void type( T data ) {
    cout << typeid( data ).name() << endl;
}


template<typename T>
class Data{
public:
    Data( T data ) :data( data ) { };
    ~Data( ) { };
    T getData( ) {
        return this->data;
    };
    int comp( const T& data , int( *fun )( const T &a , const T &b ) );
private:
    T data;
};

template<typename T>
int Data<T>::comp( const T& data , int( *fun )(const T &a ,const T &b ) ) {
    return int( fun( this->data , data ) );
}

int comp(const int &a ,const int &b ) {
    if( a == b ) return 0;
    else if( a < b ) return -1;
    else return 1;
}

int main ( ) {
    int a { 3 };
    Data<int> data( a );
    cout << "get: " << data.getData( ) << endl;
    cout<<"comp : "<<data.comp( a , comp )<<endl;


    char c { };
    float f { };
    type( a );
    type( c );
    type( f );
    system( "pause" );
    return 0;
}
