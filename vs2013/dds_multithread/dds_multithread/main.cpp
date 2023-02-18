#include "../src/QYdds.h"
#include <vector>
#include <mutex>
#include <thread>
#include <vector>
#include<CommLog.h>


struct TestMsg {
    char    m_senderName [32];
    int     m_senderNameLength;
    char    m_msg [256];
    int     m_msgLength;
};

//�鲥��ַ��230.230.230.230
//�鲥�˿ڣ�51234
//
//ÿ�밴˳��ִ��1����������
//1.�����������ݣ�����ӡ�������������ͷ����ƺ���Ϣ����
//2.������µķ��ͷ����򱣴淢�ͷ����Ƶ�����
//3.��˳���ӡ���з��ͷ������б�
//4.��˳���ÿ�����ͷ�����һ����Ϣ������Ϊ���Լ�����+�������

vector<DWORD> runing_threads;
std::mutex mtx;
std::mutex mtx_io;

once_flag once_f;
struct MyData {
    DWORD pid;
    char* str;
};


class myreader :public QYTopicReceiver {
    QYMessageResult myreader::onReceiveTopic( const QYTopicParam & param ) { 
        QYMessageResult msgRet = { true , 0 , NULL };
        DWORD *pid = (DWORD *)param.m_param;
        char buff [64] = { 0 }; 
        QYTopicSender moniter_sender("moniter",NULL);//�����л����ݲ���������߳�
        int len = param.m_paramLength;//�������ݵĳ���
        char* str = new char [len];//��ȥһ��DWORD�ĳ���
        char* val_str = (char*) ( param.m_param );
        val_str += 4;
        strcpy_s( str , len - 4 ,val_str);//    val_str������\0��β��Ȼ��Խ�硪������������������������������������
        sprintf_s( buff , "thread %d to thread %d recved:%s" , *pid , GetCurrentThreadId( ) ,str); 
        delete[] str;
        moniter_sender.sendData(buff,strlen(buff)+1);//���͵��ַ�����\0��β        
        printf( "msg:%s \n" , param.m_param+4 );
        return msgRet;
    };
};

class Moniter_receiver :public QYTopicReceiver {
    QYMessageResult Moniter_receiver::onReceiveTopic( const QYTopicParam & param ) override {
        char* data = (char*)param.m_param;
        char str [62]{0};
        strcpy_s( str , param.m_paramLength , data );
        mtx_io.lock( );
        PrintLog( 0 ,"%s\n",str);//��ӡ��־
        mtx_io.unlock( );
        return QYMessageResult( );
    }
};

void worker( ) {
    YddsInit( 1 );//�����̶߳�����1��

    //����
    myreader my_reader;
    char str [64]{0};
    DWORD t_id = GetCurrentThreadId( );
    mtx.lock( );
    runing_threads.push_back( t_id );//���̼߳��뵽ȫ�ִ洢�߳�id��vec��
    mtx.unlock( );
    
    char buf [32]{0};
    sprintf_s( buf , "%d" , t_id );
    my_reader.registTopic( buf , NULL );//ע��




    Sleep( 4000 );//�ȴ����������̶߳���ʼ�����
    //���Ͳ�֪ͨ����߳�
    
    MyData data;
    memset( buf , 0 , 32 );
    srand(GetCurrentThreadId() );//���߳�id��Ϊ��������ӣ��ڶ��߳���ʹ��srand��time(0))�ᵼ���ظ�����ʱ���ڲ��ı�����
    int i = rand( ) % runing_threads.size( );
    t_id = runing_threads [i];//������һ���Ѿ����е��̵߳�id��Ϊtype_name ����
    sprintf_s( buf , "%d" , t_id );
    QYTopicSender mySender( buf , NULL );
    data.pid = GetCurrentThreadId( );
    data.str = str;
    sprintf_s((data.str)+4,58, "iam %d\0" , GetCurrentThreadId() );
    DWORD* ptr_pid = (DWORD*)data.str;
    *ptr_pid = data.pid;
    mySender.sendData(str, strlen((data.str)+4)+5 );//�ַ�����\0��β
    
    //moniter_info
    QYTopicSender moniter_sender( "moniter" , NULL );
    char buf_moniter_info [64] = { 0 };
    sprintf_s( buf_moniter_info , "thread %d to thread %d sended:%s\0" , GetCurrentThreadId( ),t_id , str+4 );
    moniter_sender.sendData( buf_moniter_info , strlen( buf_moniter_info ) + 1 );//���͵��ַ�����\0��β 
    while (true)
    {
    }
    ;//��
}

void moniter( ) {
    YddsInit( 1 );
    Moniter_receiver moniter_receiver;
    moniter_receiver.registTopic( "moniter" , NULL );
    while (1) {}
    
}

int main(){
    thread t1 { moniter };
    vector<thread*> workers;
    for (int i = 0; i < 8 ; i++)
    {
        workers.push_back( new thread( worker ) );//�߳̿յȴ�	
    }


    for( auto it = workers.begin( ); it != workers.end( ); it++ ) {
        if( ( *it )->joinable( ) ) ( *it )->join( );
    }
    for( auto it = workers.begin( ); it != workers.end( );it++ )
    {
        delete( *it );
    }
    if( t1.joinable( ) ) t1.join( );
	return 0;
}