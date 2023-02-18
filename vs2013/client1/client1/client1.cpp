#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstdio>

#pragma warning(disable:4996)
#pragma comment(lib,"Ws2_32.lib")

int main(){
    int ret;
    WSADATA wsaData;
    ret = WSAStartup( MAKEWORD( 2 , 2 ) , &wsaData );
    if( ret != 0 ) {
        WSACleanup( );
        return 1;
    }

    sockaddr_in sendAddr;
    sendAddr.sin_family = AF_INET;
    sendAddr.sin_port = htons(unsigned short( 6666 ));
    inet_pton( AF_INET , "239.0.0.0" , &sendAddr.sin_addr );

    SOCKET sendSocket = INVALID_SOCKET;
    sendSocket = socket( AF_INET, SOCK_DGRAM , IPPROTO_UDP );
    if( sendSocket == INVALID_SOCKET ) {
        closesocket( sendSocket );
        WSACleanup( );
        return 1;
    }

    char sbuf[100] = "hello,are you ok?";
    ret = sendto( sendSocket ,sbuf,sizeof(sbuf) , 0 , (SOCKADDR*) &sendAddr , sizeof( sendAddr ) );
    if( ret == SOCKET_ERROR ) {
        closesocket( sendSocket );
        WSACleanup( );
        return 1;
    }

    char hostname [32];
    ret = gethostname( hostname , 32 );
    if (ret != 0){
        return 1;
    }
    printf( "%s\n" , hostname );
    hostent *host;
    host = gethostbyname( hostname );
    if (host ==nullptr){
        return 1;
    }

    for (int i = 0;; i++)
    {
        if( host->h_addr_list [i] ) {
            inet_ntop( host->h_addrtype , host->h_addr_list [i] , hostname , sizeof( hostname ) );
            printf( "%s\n" ,hostname );
        }
        else break;
    }
    

	return 0;
}