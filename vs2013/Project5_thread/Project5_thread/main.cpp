#include <thread>
#include<iostream>
#include <Windows.h>
#include <vector>

using namespace std;

struct Ret{
    int beg;
    int end;
    int count;
    int ret;
};

void fun( Ret * ret) {
    for( int i = ret->beg; i < ret->end; i++ ) {
        ret->ret += i;
    }
  return;
}



int main( ) {
    vector<thread*> threads;
    vector<Ret*> args;
    
    int beg , end;
    int sum = 0;
    int count;
    cin >> count;
    if( count < 1 ) {
        cout << "输入非法";
    }
    cin.clear( );
    cin.sync( );
    cout << "输入范围：";
    cin >> beg;
    cin >> end;
    if( end < beg ) {
        int temp = beg;
        beg = end;
        end = temp;
    }
    int n = (end-beg+1) / count;
    if( ( end - beg + 1 ) <= count ) {
        n = 1;
        count = end - beg + 1;
    }
    int last = 0;
    for(int i = beg; i+n <= end+1; i += n ) {
        Ret * ret = new Ret { i , i + n , n , 0 };
        args.push_back(ret);
        threads.push_back( new thread( fun , ret ) );
        last = i+n; 
    }
    //todo
    if( ( end - beg + 1 ) % count != 0 ) {
        Ret * ret = new Ret { last , end + 1 , n , 0 };
        args.push_back( ret );
        threads.push_back( new thread( fun , ret ) );
    }

    for( auto it = args.begin( ); it != args.end( ); it++ ) {
        sum += ( *it )->ret;
    }
    for( auto it = threads.begin( ); it != threads.end( ); it++ ) {
        ( *it )->join( );
    }
    cout << sum << endl;
    
    system( "pause" );
    return 0;
}