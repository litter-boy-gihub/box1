#include <stdio.h>
#include <iostream>
#include <memory>
#include <WinSock2.h>
#include <WS2tcpip.h>
#define LEN_BUFF 256
#define WIN32_LEAN_AND_MEAN
#pragma  comment(lib,"Ws2_32.lib")

using  std::make_unique;
using std::unique_ptr;
unsigned short port = 2048;

int main( ) {
    int ret;
    unique_ptr<char> sbuf( new char [LEN_BUFF] );
    unique_ptr<char> rbuf( new char [LEN_BUFF] );

    //初始化
    WSADATA wsaData;
    ret = WSAStartup( MAKEWORD( 2 , 2 ) , &wsaData );
    if( ret != 0 ) {
        printf( "wsastartup failed" );
        return 1;
    }

    //创建socket
    struct addrinfo *result = nullptr ,
                    *ptr = nullptr ,
                    hints;
    ZeroMemory( &hints , sizeof( hints ) );
    hints.ai_family = AF_INET;//ipv4
    hints.ai_socktype = SOCK_DGRAM;//数据报
    hints.ai_protocol = IPPROTO_UDP;//udp协议

    //get addrinfo
    ret = getaddrinfo( "127.0.0.1" , "2048" , &hints,&result);
    if( ret != 0 ) {
        printf( "getaddrinfo failed" );
        WSACleanup( );
        return 2;
    }

    //create socket
    SOCKET connectSockt = INVALID_SOCKET;
    ptr = result;
    connectSockt = socket( ptr->ai_family , ptr->ai_socktype , ptr->ai_protocol );
    if( connectSockt == INVALID_SOCKET ) {
        printf( "socket failed" );
        freeaddrinfo( ptr );
        WSACleanup( );
        return 3;
    }

    freeaddrinfo( result );

    //发送目的sockaddr_in
    sockaddr_in remoteAddr;
    sockaddr_in localAddr;

    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(port);
    inet_pton( AF_INET , "127.0.0.1" , &remoteAddr.sin_addr );
    
    //localAddr.sin_family = AF_INET;
    //localAddr.sin_port = htons( port );
    //inet_pton( AF_INET , "127.0.0.1" , &localAddr.sin_addr );
    //
    ////bind
    //ret = bind( connectSockt , (SOCKADDR*) &localAddr , sizeof( localAddr ) );
    //if( ret != 0 ) {
    //    printf( "bind error" );
    //    closesocket( connectSockt );
    //    WSACleanup( );
    //    return 4;
    //}

    char *sbuf_p = sbuf.release( );
    char *rbuf_p = sbuf.release( );
    while(true) {
        std::cin.getline(sbuf_p,LEN_BUFF);
        //sendto
        ret = sendto( connectSockt , sbuf_p , LEN_BUFF , 0 , (SOCKADDR*) &remoteAddr , sizeof( remoteAddr ) );
        if( ret == SOCKET_ERROR ) {
            printf( "sendto error" );
            closesocket( connectSockt );
            WSACleanup( );
        }

        //ret = recvfrom( connectSockt , rbuf_p , LEN_BUFF , 0 , (SOCKADDR*) &remoteAddr , sizeof( remoteAddr ) );

        //recvfrom
    }
    
    closesocket( connectSockt );
    WSACleanup( );
    system("pause" );
    return 0;
}