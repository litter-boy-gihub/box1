/************************************************************************/
/*
命令行参数解析
 -help 打印命令
 -inline 发送端既包含Writer也包含一个Reader
 -send 发送端
 -recv 接收端
 -id   域id
 -reliability 1/2    1可靠  2不可靠
 

 -topic_list    11 一个主题（topic1 type1)
                1122 即为两个主题类型相同的主题  

 -topic_match   当收发多个主题时，按顺序
                DataWriter/DataWriter 发布订阅的topic
                例如订阅方（指定-recv）：
                121  指DataReader1 订阅topoic1 R2 订阅 topic2 R3 订阅 topic1
                
*/
/************************************************************************/

#include"GlobalDefine.h" 
#include<vector>
#include<map>
#include<thread>
#include<iostream>
#include<string>


int g_numpkg = 0;
bool is_sender = true;


void STACALL callback( struct _DataReader* pstDataReader ) {
    UINT32 dataNum = 0;
    DataSeq stDataSeq;
    DataLen dataLen;
    static unsigned int seqNum = 0;

    /* 收数据 */
    DDS_ReturnCode_t retCode = DDS_StringDataReader_take( pstDataReader , &stDataSeq );
    if( DDS_RETCODE_NO_DATA != retCode ) {
        for( dataNum = 0; dataNum < stDataSeq.length; dataNum++ ) {
            char * userData = DDS_StringSeq_get( &stDataSeq , dataNum , &dataLen );
            g_numpkg++;
            printf( "********** g_numpkg = %d, data = %d  len = %d \n" , g_numpkg , userData [0] , dataLen);
        }
        /* 释放缓存 */
        DDS_StringDataReader_return_loan( pstDataReader , &stDataSeq );
    }
}

void readTask( DDS_DomainParticipant* participant ) {
    DDS_Topic* topic = participant->pstTopic;
    DDS_DataReader* dataReader = DDS_Topic_create_datareader( topic , nullptr , (CALLBACKFUN) callback );
    while( 1 ) { Sleep( 1000 ); };
}

int main(int argc,char *argv[] ) {  

    /*保存命令行参数*/
    std::map<std::string , std::string> args;
    for( int i = 1; i < argc; i++ ) {
        if( *argv [i] == '-' ) {
                std::string str( argv [i] );
            if( i + 1 < argc && *argv[i+1] != '-') {  
                args.insert( make_pair( str , std::string( argv [i+1] ) ) );
                i++;
            }
            args.insert( make_pair( str , "" ) );
        }
    }

    /*命令行信息*/
    for( auto it = args.begin( ); it != args.end( ); it++ ) {
        std::cout<<'['<< it->first<<':'<<it->second<<']'<<'\n';
    }

    /*打印帮助信息*/
    if( args.find( std::string( "-help" ) ) != args.end( ) ) {
        printf( "\
            /************************************************************************/\n\
            命令行参数解析 \n\
            -help 打印命令  \
            -send [ i ] 发送方个数 \n\
            -recv [ i ] 接收方个数  \n\
            -id   域id \n\
            -reliability 1/2    1可靠  2不可靠 \n\
            -send_model     发送模式（单发单收，多发单收，单发多收，多发多收）\n\
                            目前多即2个，后续需要更改 \n\n\
            -topic_list     11 一个主题（topic1 type1) \n\
                            1122 即为两个主题类型相同的主题  \n\
            -topic_match    当收发多个主题时，按顺序 \n\n\
                            DataWriter/DataWriter 发布订阅的topic \n\
                            例如订阅方（指定-recv）：\n\
                            121  指DataReader1 订阅topoic[1-1] R2 订阅 topic[2-1] R3 订阅 topic[1-1] \n\
                            例如发布方（指定-send）：\n\
                            121  指DataWriter1 发布topoic[1-1] W2 发布 topic[2-1] W3 发布 topic[1-1] \n\
            /************************************************************************/\n");
        return 0;
    }

    if( args.find( std::string( "-send" ) ) == args.end( ) ) is_sender = false;

    


    const int buff_len = 4 * 1024;
    DDS_DomainParticipant* participant;
    DDS_DomainParticipantQos* stDomainQos = nullptr;
    DDS_DataWriterQos writerQos;  //为什么不能用指针分配的方式，在  DDS_Topic_get_default_datawriter_qos( topic , &writerQos );失败

    DDS_ParticipantFactory_init( );
    DDS_DomainParticipantFactory_get_defaul_participant_qos( stDomainQos );

    int id =  atoi( (args.find( std::string( "-id" ) ))->second.c_str( ) );
  
    participant = DDS_DomainParticipantFactory_create_participant( id , stDomainQos );

    
    /*解析topic 数量*/
    std::vector<std::pair<char , char>> topic_lists;
    DDS_Topic** topic = nullptr;
    auto it = args.begin( );
    if( ( it = args.find( std::string( "-topic_list" ) ) ) != args.end( ) ) {
        int list = atoi( it->second.c_str( ) );
        while( list > 0 ) {
            int temp1 = list % 10; //低位
            temp1 /= 10;
            int temp2 = list % 10; //高位
            temp2 /= 10;
            topic_lists.push_back( std::make_pair( temp2 , temp1 ) );//高位：topic 低位:type
        }
        reverse( topic_lists.begin( ) , topic_lists.end( ) );    //翻转顺序
        topic = new DDS_Topic* [topic_lists.size()];
        char topic_prefix [8] = "topic"; // 为了安全可以判断一下num占据的位数是否溢出
        char type_prefix [8] = "type";
        char topic_num [4] = "";
        char type_num [4] = "";
        for( int i = 0; i < topic_lists.size(); i++ ) {
            sscanf( topic_num , "%d" , topic_lists.at( i ).first );
            sscanf( type_num , "%d" , topic_lists.at( i ).second );
            strcpy( topic_prefix + 5 , topic_num );
            strcpy( type_prefix + 4 , type_num );
            topic[i] = DDS_DomainParticipant_create_topic( participant ,topic_prefix , type_prefix , nullptr );
        }
    }
    else {
        topic = new DDS_Topic*[1];
        topic[0] = DDS_DomainParticipant_create_topic( participant , "topic1" , "type1" , nullptr );
    }

    /*解析writer 和 reader 数量 */
    DDS_Topic_get_default_datawriter_qos( topic[0] , &writerQos );  //为什么获取默认qos参数要有topic?   实现里没有用到，这里类似c++里的this指针，表达对象调用关系。
    writerQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
    std::vector<char> topic_matchs;
    DDS_DataWriter** dataWriter = nullptr;
    DDS_DataReader** dataReader = nullptr;
    if( ( it = args.find( std::string( "-topic_list" ) ) ) != args.end( ) ) {
        int matchs = atoi( it->second.c_str( ) );
        int temp;
        while( matchs > 0 ) {
            temp = matchs % 10;
            temp /= 10;
            topic_matchs.push_back( temp );
        }
        reverse( topic_matchs.begin( ) , topic_matchs.end( ) ); //翻转顺序
        if( is_sender ) {
            dataWriter = new DDS_DataWriter*[topic_matchs.size()];
            for( int i = 0; i < topic_matchs.size( ); i++ ) {
                DDS_Topic_create_datawriter( topic [topic_matchs [i]] , &writerQos , nullptr );
            }
        }
        else {
            dataReader = new DDS_DataReader*[topic_matchs.size()];
            for( int i = 0; i < topic_matchs.size( ); i++ ) {
                dataReader[i] =  DDS_Topic_create_datareader( topic [topic_matchs [i]] , nullptr , callback );
            }
        }
    }
    else {
        if( is_sender ) { 
            dataWriter = new DDS_DataWriter* [1]; 
            dataWriter [0] = DDS_Topic_create_datawriter( topic [0] , NULL , NULL );
        }
        if( !is_sender ) {
            dataReader = new DDS_DataReader* [1];
            dataReader [0] = DDS_Topic_create_datareader( topic [0] , NULL , callback );
        }
    }
    
    
    std::thread t1;
    if( ( it = args.find( std::string( "-inline" ) ) ) != args.end( ) )
    t1 = std::thread{ readTask , participant };

    /*sender*/
    if(is_sender ) {
        int step = 1;
        char sendbuf [buff_len];
        if( topic_matchs.size( ) != 0 ) {
            for( int i = 0; i < topic_matchs.size( ); i++ ) {  //这里应该用多线程替代。或者线程池
                memset( sendbuf , 49 + step++ , buff_len );
                printf( "sleep for 5s...\n" );
                Sleep( 6000 );
                printf( "start to send...\n" );
                for( int i = 0; i < 32; i++ ) {
                    sendbuf [i] = 50;
                    DDS_StringDataWriter_write( dataWriter[i] , sendbuf , buff_len );
                }
                printf( "end to send...\n" );
            }
        }
        else {
            memset( sendbuf , 49 + step++ , buff_len );
            printf( "sleep for 5s...\n" );
            Sleep( 6000 );
            printf( "start to send...\n" );
            for( int i = 0; i < 32; i++ ) {
                sendbuf [i] = 50;
                DDS_StringDataWriter_write( dataWriter[0] , sendbuf , buff_len );
            }
            printf( "end to send...\n" );
        }
    }

    /*recver*/
  /*  if( !is_sender ) {
        dataReader = DDS_Topic_create_datareader( topic , NULL , callback );
    }*/

    while( 1 );

    if( ( it = args.find( std::string( "-inline" ) ) ) != args.end( ) )
    if( t1.joinable( ) ) t1.join( );

    /**/
    //DDS_Topic_delete_datawriter( topic );
    /* 清除主题 */
    //DDS_DomainParticipant_delete_topic( participant , topic );
    /* 清除参与者 */
    DDS_DomainParticipantFactory_delete_participant( participant );

    return 0;
}



