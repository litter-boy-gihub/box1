#include<stddef.h>
#include<stdio.h>

char* p = NULL;

int fun() {
    return 1;
}
         //c����û�к�������









int fun( char* p ) {
    return 2;
}

int main( ) {

    printf( "%d" , fun(NULL) );
    getchar( );
    return 0;
}