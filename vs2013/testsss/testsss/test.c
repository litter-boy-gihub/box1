#include<stddef.h>
#include<stdio.h>

char* p = NULL;

int fun() {
    return 1;
}
         //c语言没有函数重载









int fun( char* p ) {
    return 2;
}

int main( ) {

    printf( "%d" , fun(NULL) );
    getchar( );
    return 0;
}