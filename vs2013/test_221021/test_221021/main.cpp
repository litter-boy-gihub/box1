#include<stdlib.h>
#include<stdio.h>

typedef struct _Data Data;

int main( ) {

    int *i = (int*)malloc( sizeof( int ) );
    void * p = i;
    free( p );

    Data* data = (Data*)malloc( 8 );
    char* temp = (char*)data;
    temp += 4;
    Data* data2 = (Data*) temp;
    free( data );
    //free(data2);  这样中断，所以free是知道要释放的长度的，例如有一个长度头在前面？？？
                    // | header | user_data_aear    |
                    //          ^
                    //          |  <-user_length->  |
                    //          ptr
    int a [5] = { 1 , 2 , 3 , 4 , 5 };
    for( auto i : a ) { printf( "%d  " , i ); }
    const int* ptr = a;
    
    int * ptr1 = const_cast<int*>(ptr);
    int c { 2} , b {5 };
    c++ + b;

    c = *( ptr1++ );
    c = *ptr1++;
    c = *++ptr1;








    return 0;
}