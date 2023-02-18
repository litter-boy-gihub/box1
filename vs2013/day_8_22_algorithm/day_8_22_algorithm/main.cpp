#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>


using namespace std;

struct LargerIntager {
    char data[32];
    LargerIntager( ) {
        memset( data , '0' , 32 );
    }
};

int get_len(const char * data) {
    int i = 31,zero_num = 0;
    while( data [i] == '0' && i-- >= 0 ) zero_num++;
    return 32 - zero_num;
}

LargerIntager operator+( const LargerIntager& left , const LargerIntager& right ) {
    int i = 0, len;    
    int l_len = 0 , r_len = 0;
    l_len = get_len( left.data );
    r_len = get_len( right.data );

    len = l_len > r_len ? l_len : r_len;
    
    LargerIntager ret;
    int sum,carray_intager = 0;
    for (int i = 0; i <= len ; i++)
    {
        l_len = int( left.data [i] )-int('0');
        r_len = int( right.data [i] )-int('0');
        sum = ( ( l_len + r_len + carray_intager ) % 10 );
        carray_intager = floor( ( l_len + r_len + carray_intager ) / 10.0 );
        ret.data [i] = char( sum + int( '0' ) );
    }
    return ret;
}

LargerIntager operator*(const LargerIntager left , const LargerIntager right ) {
    int l_len = 0 , r_len = 0,off = 0,carry_intager = 0,temp,l_int,r_int;
    l_len = get_len( left.data );
    r_len = get_len( right.data );
    vector<LargerIntager*> results;
    for (int i = 0; i < l_len ; i++)
    {
        off = i;
        auto ptr = new LargerIntager( );
        results.push_back(ptr);
        carry_intager = 0;
        l_int = int( left.data [i] ) - int( '0' );
    	for (int j = 0; j <=r_len ; j++)
    	{
            r_int = int( right.data [j] ) - int( '0' );
            temp = l_int*r_int+carry_intager;//乘积+进位
            carry_intager = floor( temp / 10.0 );//新进位
            temp = temp % 10;//当前数字
            ptr->data [j + off] = char( temp + int( '0' ) );
    	}
    }

    LargerIntager ret;
    for_each( results.begin( ) , results.end( ) , [&](LargerIntager* item ) {  
        ret = ret + *item;
    } );
    for_each( results.begin( ) , results.end( ) , [ &]( LargerIntager* item ) {delete( item );  } );//释放
    return ret;
}


ostream& operator<<( ostream& o , const LargerIntager& data ) {
    int i = get_len( data.data );
    while (i-- >= 0) o << data.data [i];
    o <<'\n';
    return o;
}

LargerIntager get_low( LargerIntager* data) {
    int half_len = floor((get_len( data->data )+1)/2.0);//长度的一半，高一半的位置起点
    LargerIntager ret;
    memcpy( ret.data , data->data , half_len );
    return ret;
}

LargerIntager get_hight( LargerIntager* data ) {
    int half_len = floor( ( get_len( data->data ) + 1 ) / 2.0 );//长度的一半，高一半的位置起点
    LargerIntager ret;
    memcpy( ret.data , data->data+half_len , half_len );
    return ret;
}

LargerIntager get_10_pow_n(int n ) {
    char temp [32];
    memset( temp , '0' , 32 );
    temp [n] = '1';
    LargerIntager ret;
    memcpy( ret.data , temp , 32 );
    return ret;
}

LargerIntager recursive(LargerIntager left,LargerIntager right) {
    int l_len , r_len,l_half_len,r_half_len;
    l_len = get_len( left.data );
    r_len = get_len( right.data );
    l_half_len = floor( ( l_len + 1 ) / 2.0 );
    r_half_len = floor( ( r_len + 1 ) / 2.0 );
    if( l_len <= 9 && r_len <= 9 ) { 
         
        LargerIntager ret = left*right;
        return ret;
    }      //长度都小于9，归
    else if( l_len <= 9 && r_len > 9 ) {    //长度right大于9
        return recursive( left , get_hight( &right ) ) * get_10_pow_n( r_half_len ) + recursive( left,get_low( &right ) );
    }
    else if( l_len > 9 && r_len <= 9 ) {    //长度left大于9
        return recursive(right, get_hight( &left ))*get_10_pow_n(l_half_len)+recursive(right,get_low(&left));
    }
    else {                                  //长度都大于9
        return recursive( get_hight( &left ) , get_hight( &right ) ) * get_10_pow_n( l_half_len + r_half_len ) +
            recursive( get_hight( &left ) , get_low( &right ) )*get_10_pow_n( l_half_len ) +
            recursive( get_low( &left ) , get_hight( &right ) )*get_10_pow_n( r_half_len ) +
            recursive( get_low( &left ) , get_low( &right ) );
    }
}

int main(){
    LargerIntager first , second;
    memcpy( first.data , "12345" , strlen( "12345" ) );
    memcpy( second.data , "55" , strlen( "55" ) );
    cout << first;
    cout << second;
    cout << recursive(first,second);

    system( "pause" );
    //abort( );
    //_exit(1 );退出不清理
    //exit( 0);
    //atexit(fun);
	return 0;
}