#include <stdio.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <string>


#pragma  comment(lib,"Ws2_32.lib")

int main(){
    std::string s;
    s.c_str( );
    int ret;
    WSADATA wsaData;
    ret = WSAStartup( MAKEWORD( 2 , 2 ) , &wsaData );
    if( ret != 0 ) {
        printf( "wsastartup failed" );
        return 1;
    }

    SOCKET recvSocket = INVALID_SOCKET;
    sockaddr_in recvAddr;
    sockaddr_in sendAddr;
    int sendAddr_len;// = sizeof( sendAddr );
    unsigned short port = 2048;

    const int LEN_BUF = 256;
    char rbuf [LEN_BUF];
    recvSocket = socket( AF_INET , SOCK_DGRAM , IPPROTO_UDP );
    if( recvSocket == INVALID_SOCKET ) {
        WSACleanup( );
        return 1;
    }

    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = htons( port );
    inet_pton( AF_INET , "0.0.0.0" , &recvAddr.sin_addr );
    //recvAddr.sin_addr.S_un.S_addr = 0;//any ≤√¥“‚Àº£ø

    ret = bind( recvSocket , (SOCKADDR*) &recvAddr , sizeof( recvAddr ) );

   
    while (true)
    {
        ret = recvfrom( recvSocket , rbuf , LEN_BUF , 0 , (SOCKADDR*) &sendAddr , &sendAddr_len );
        printf( "%s" , rbuf );
    }
   

    closesocket( recvSocket );
    WSACleanup( );


    
	return 0;
}