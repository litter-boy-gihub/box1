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

//组播地址：230.230.230.230
//组播端口：51234
//
//每秒按顺序执行1次以下任务：
//1.接收网络数据，并打印出来：包括发送方名称和消息内容
//2.如果有新的发送方，则保存发送方名称到本地
//3.按顺序打印所有发送方名称列表
//4.按顺序给每个发送方发送一条消息，内容为：自己名称+递增序号

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
        QYTopicSender moniter_sender("moniter",NULL);//反序列化数据并发给监控线程
        int len = param.m_paramLength;//整个数据的长度
        char* str = new char [len];//减去一个DWORD的长度
        char* val_str = (char*) ( param.m_param );
        val_str += 4;
        strcpy_s( str , len - 4 ,val_str);//    val_str必须以\0结尾不然会越界————————————————有问题
        sprintf_s( buff , "thread %d to thread %d recved:%s" , *pid , GetCurrentThreadId( ) ,str); 
        delete[] str;
        moniter_sender.sendData(buff,strlen(buff)+1);//发送的字符串以\0结尾        
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
        PrintLog( 0 ,"%s\n",str);//打印日志
        mtx_io.unlock( );
        return QYMessageResult( );
    }
};

void worker( ) {
    YddsInit( 1 );//所有线程都在域1内

    //接收
    myreader my_reader;
    char str [64]{0};
    DWORD t_id = GetCurrentThreadId( );
    mtx.lock( );
    runing_threads.push_back( t_id );//将线程加入到全局存储线程id的vec中
    mtx.unlock( );
    
    char buf [32]{0};
    sprintf_s( buf , "%d" , t_id );
    my_reader.registTopic( buf , NULL );//注册




    Sleep( 4000 );//等待其他所有线程都初始化完毕
    //发送并通知监控线程
    
    MyData data;
    memset( buf , 0 , 32 );
    srand(GetCurrentThreadId() );//用线程id作为随机数种子，在多线程中使用srand（time(0))会导致重复，短时间内不改变的情况
    int i = rand( ) % runing_threads.size( );
    t_id = runing_threads [i];//随机获得一个已经运行的线程的id作为type_name 发送
    sprintf_s( buf , "%d" , t_id );
    QYTopicSender mySender( buf , NULL );
    data.pid = GetCurrentThreadId( );
    data.str = str;
    sprintf_s((data.str)+4,58, "iam %d\0" , GetCurrentThreadId() );
    DWORD* ptr_pid = (DWORD*)data.str;
    *ptr_pid = data.pid;
    mySender.sendData(str, strlen((data.str)+4)+5 );//字符串含\0结尾
    
    //moniter_info
    QYTopicSender moniter_sender( "moniter" , NULL );
    char buf_moniter_info [64] = { 0 };
    sprintf_s( buf_moniter_info , "thread %d to thread %d sended:%s\0" , GetCurrentThreadId( ),t_id , str+4 );
    moniter_sender.sendData( buf_moniter_info , strlen( buf_moniter_info ) + 1 );//发送的字符串以\0结尾 
    while (true)
    {
    }
    ;//等
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
        workers.push_back( new thread( worker ) );//线程空等待	
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