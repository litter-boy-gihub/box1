#include <iostream>
#include <mutex>
#include <Windows.h>
using  namespace std;


#pragma data_seg("myShare")
mutex mtx;
int share_i = 0;
#pragma data_seg()

#pragma comment(linker,"/section:myShare,rws")

int main(){

    for (int i = 0; i < 100 ; i++)
    {
        mtx.lock( );
        Sleep( 1000 );
        share_i++;
        cout << share_i << endl;
        mtx.unlock( );
    }
    
    getchar( ); 
	return 0;
}