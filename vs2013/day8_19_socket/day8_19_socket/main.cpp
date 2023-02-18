#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#include<cstdlib>
#include<time.h>
#include <iostream>
using namespace std;

#pragma comment(lib,"Ws2_32.lib")

struct Msg {
    char    m_name [32];
    int     m_name_len;
    char    m_msg [256];
    int     m_msg_len;
};


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
    bindAddr.sin_port = htons( unsigned short( 51234 ) );
    inet_pton( AF_INET , "0.0.0.0" , &bindAddr.sin_addr );

    SOCKET bindSocket = INVALID_SOCKET;
    bindSocket = socket( AF_INET , SOCK_DGRAM , IPPROTO_UDP );
    if( bindSocket == INVALID_SOCKET ) {
        closesocket( bindSocket );
        WSACleanup( );
        return 1;
    }

    //���õ�ַ����,���Զ������˽��յ�����
    bool addreuse = true;
    ret = setsockopt( bindSocket , SOL_SOCKET , SO_REUSEADDR , (char*) &addreuse , sizeof( addreuse ) );

    ret = bind( bindSocket , (SOCKADDR*) &bindAddr , sizeof( bindAddr ) );
    if( ret != 0 ) {
        closesocket( bindSocket );
        WSACleanup( );
        return 1;
    }

    //�����鲥��ַ
    ip_mreq multicast;
    inet_pton( AF_INET , "127.0.0.1" , &multicast.imr_interface );
    inet_pton( AF_INET , "230.230.230.230" , &multicast.imr_multiaddr );
    ret = setsockopt( bindSocket , IPPROTO_IP , IP_ADD_MEMBERSHIP , (char*) &multicast , sizeof( multicast ) );
    if( ret != -0 ) {
        closesocket( bindSocket );
        WSACleanup( );
        return 1;
    }

    sockaddr_in sendAddr;
    int sendAddr_len = sizeof( sendAddr );
    char *recvBuff = new char [sizeof(Msg)];
    vector<char*> names;

    //1.����Ϣ
    sockaddr_in addr_to;
    addr_to.sin_family = AF_INET;
    addr_to.sin_port = htons( unsigned short( 51234 ) );
    inet_pton( AF_INET , "230.230.230.230" , &addr_to.sin_addr );

    Msg msg;
    memset( msg.m_name , 0 , 32 );
    /*DWORD id = GetCurrentThreadId( );
    sprintf_s( msg.m_name , "%d" , id );*/
    cout << "�������֣�";
    cin.getline( msg.m_name , 32 );


    msg.m_msg_len = strlen( msg.m_name );
    sprintf_s( msg.m_msg , "hello %d" , time( 0 ) );
    msg.m_msg_len = strlen( msg.m_msg );

    sendto( bindSocket , (char*) &msg , sizeof( Msg ) , 0 , (SOCKADDR*) &addr_to , sizeof( addr_to ) );

    while( true ) {
        Sleep( 1000 );
        
        //����Ϣ
        recvfrom( bindSocket , recvBuff , sizeof(Msg) , 0 , (SOCKADDR*) &sendAddr , &sendAddr_len );
        Msg *pmsg = (Msg*) recvBuff;
        printf( "���ͷ���%s ������Ϣ�� %s",pmsg->m_name,pmsg->m_msg );
        
        //2.������µķ��ͷ����򱣴淢�ͷ����Ƶ�����
        bool find = false;
        for( auto it = names.begin( ); it != names.end( ); it++ ) {
           if (!strcmp((*it),pmsg->m_name)){
               find = true;
           }
        }
        char* str = new char [32];
        strcpy( str , pmsg->m_name );
        if(!find)names.push_back( str );

        //3.��˳���ӡ���з��ͷ������б�
        for( auto it = names.begin( ); it != names.end( ); it++ ) {
            printf( "�����б�%s\n" , ( *it ) );
        }
        //4.��˳���ÿ�����ͷ�����һ����Ϣ������Ϊ���Լ�����+�������
        int i = 1;
        for( auto it = names.begin( ); it != names.end( );it++ )
        {
            Msg reply;
            sprintf_s( reply.m_name , "%s",msg.m_name);
            reply.m_name_len = strlen( reply.m_name );
            sprintf_s( reply.m_msg , "%s  %d" , reply.m_name,i );
            reply.m_msg_len = strlen( reply.m_msg );
            sendto( bindSocket , (char*)&reply , sizeof( Msg ),0, (SOCKADDR*)&addr_to , sizeof( addr_to ) );
        } 
    }

    return 0;
}