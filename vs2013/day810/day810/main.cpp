#include <iostream>
#include<thread>
#include <vector>
using namespace std;

int arr [20]{};

void sum(int b,int e,int arr_i ) {
     for (int i = b; i < e ; i++)
     {
         arr [arr_i] += i;
     }
}

int main(){
    vector<thread*> ts;
    int beg , end , count;
    cout << "·Ö±ğÊäÈëbeg end count:" << endl;
    cin >> beg;
    cin >> end;
    cin >> count;
    int n = ( end - beg +1) / count;
    int last = n*( count - 1 )+beg;
    for( int i = 0; i< count-1 ; i ++ ) {
        ts.push_back( new thread(sum,i*n+beg,(i+1)*n+beg,i ) );
    }
    ts.push_back( new thread( sum , last , end + 1 , count - 1 ));
    int ret = 0;
    for (int i = 0; i <ts.size()  ; i++)
    {
        ret += arr [i];
    }
    cout << ret << endl;
    getchar( );
    getchar( );
	return 0;
}