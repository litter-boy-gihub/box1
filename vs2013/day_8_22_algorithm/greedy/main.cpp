//贪心：最大整数
#include<cstdlib>
#include <string>
#include <iostream>
#include <algorithm>

bool need_exchange( int a , int b ) {
    char * t = (char*) malloc( 16 * sizeof( char ) );
    t = (char*) realloc( t , 32 * sizeof( char* ) );
    char* pch = (char*) calloc( 32 , sizeof( char ) );
    sprintf( pch , "%d",a );
    std::string str1( pch );
    memset( pch , 0 , 32 );
    sprintf( pch , "%d" , b );
    std::string str2( pch );
    std::string str12 = str1 + str2;
    std::string str21 = str2 + str1;
    if( atoi( str21.c_str( ) ) > atoi( str12.c_str( ) ) ) return true;
    return false;
}

/*冒泡排序*/
void sort_to_max(int arr[],int s) {
    int max = 0;
    for( int i = s-1; i > 0 ; i-- )
        for( int j = 0; j < i; j++ ) {
            if( need_exchange( arr [j] , arr [j + 1] ) ) {
                std::swap(arr [j] ,arr [j + 1] );
            }
        }
}

int main(){
    int arr[] {7 , 13 , 4 , 246 }; //7 4 246 13
    int arr2[] {13 , 312 , 343 };  //343 312 13
    sort_to_max( arr ,4);
    sort_to_max( arr2 , 3 );
    for (int i = 0; i < 4 ; i++)
    {
        std::cout << arr [i];
    }
    std::cout << std::endl;
    for( int i = 0; i < 3; i++ ) {
        std::cout << arr2 [i];
    }
    std::cout << std::endl;

    system( "pause" );
	return 0;
}