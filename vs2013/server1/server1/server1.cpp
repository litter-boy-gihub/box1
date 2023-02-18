#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>


#pragma comment(lib,"Ws2_32.lib")

int main( ) {
    int ret;
    WSADATA wsaData;
    ret = WSAStartup( MAKEWORD( 2 , 2 ) , &wsaData );
    if( ret != 0 ) {
        WSACleanup( );
        return 1;
    }

    sockaddr_in bindAddr;
    bindAddr.sin_family = AF_INET;
    bindAddr.sin_port = htons(unsigned short( 6666 ));
    inet_pton( AF_INET , "0.0.0.0" , &bindAddr.sin_addr );

    SOCKET bindSocket = INVALID_SOCKET;
    bindSocket = socket( AF_INET , SOCK_DGRAM , IPPROTO_UDP );
    if( bindSocket == INVALID_SOCKET ) {
        closesocket( bindSocket );
        WSACleanup( );
        return 1;
    }
    //设置缓冲区大小，发送接收超时
    int bufsize = 32 * 1024;
    setsockopt( bindSocket , SOL_SOCKET , SO_RCVBUF , (char*) &bufsize , sizeof( bufsize ) );
    setsockopt( bindSocket , SOL_SOCKET , SO_SNDBUF , (char*) &bufsize , sizeof( bufsize ) );
    int time = 1000;
    setsockopt( bindSocket , SOL_SOCKET , SO_SNDTIMEO , (char*) &bufsize , sizeof( bufsize ) );
    setsockopt( bindSocket , SOL_SOCKET , SO_RCVTIMEO , (char*) &bufsize , sizeof( bufsize ) );

    //设置地址复用,可以多个服务端接收到数据
    bool addreuse = true;
    ret = setsockopt( bindSocket , SOL_SOCKET , SO_REUSEADDR , (char*) &addreuse , sizeof( addreuse ) );

    ret = bind( bindSocket , (SOCKADDR*) &bindAddr , sizeof( bindAddr ) );
    if( ret != 0 ) {
        closesocket( bindSocket );
        WSACleanup( );
        return 1;
    }

    //加入组播地址
    
    ip_mreq multicast;
    inet_pton( AF_INET , "127.0.0.1" , &multicast.imr_interface );
    inet_pton( AF_INET , "239.0.0.0" , &multicast.imr_multiaddr );
    ret = setsockopt( bindSocket , IPPROTO_IP , IP_ADD_MEMBERSHIP , (char*)&multicast , sizeof( multicast ) );
    if (ret !=-0){
        closesocket( bindSocket );
        WSACleanup( );
        return 1;
    }

    sockaddr_in sendAddr;
    int sendAddr_len = sizeof(sendAddr);
    char *recvBuff = new char [100];
    while (true)
    {

        recvfrom( bindSocket , recvBuff , 100 , 0 , (SOCKADDR*)&sendAddr , &sendAddr_len );
        std::cout << recvBuff << std::endl;
    }

    return 0;
}