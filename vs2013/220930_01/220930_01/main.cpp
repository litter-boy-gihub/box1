/************************************************************************/
/*
�����в�������
 -help ��ӡ����
 -inline ���Ͷ˼Ȱ���WriterҲ����һ��Reader
 -send ���Ͷ�
 -recv ���ն�
 -id   ��id
 -reliability 1/2    1�ɿ�  2���ɿ�
 

 -topic_list    11 һ�����⣨topic1 type1)
                1122 ��Ϊ��������������ͬ������  

 -topic_match   ���շ��������ʱ����˳��
                DataWriter/DataWriter �������ĵ�topic
                ���綩�ķ���ָ��-recv����
                121  ָDataReader1 ����topoic1 R2 ���� topic2 R3 ���� topic1
                
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

    /* ������ */
    DDS_ReturnCode_t retCode = DDS_StringDataReader_take( pstDataReader , &stDataSeq );
    if( DDS_RETCODE_NO_DATA != retCode ) {
        for( dataNum = 0; dataNum < stDataSeq.length; dataNum++ ) {
            char * userData = DDS_StringSeq_get( &stDataSeq , dataNum , &dataLen );
            g_numpkg++;
            printf( "********** g_numpkg = %d, data = %d  len = %d \n" , g_numpkg , userData [0] , dataLen);
        }
        /* �ͷŻ��� */
        DDS_StringDataReader_return_loan( pstDataReader , &stDataSeq );
    }
}

void readTask( DDS_DomainParticipant* participant ) {
    DDS_Topic* topic = participant->pstTopic;
    DDS_DataReader* dataReader = DDS_Topic_create_datareader( topic , nullptr , (CALLBACKFUN) callback );
    while( 1 ) { Sleep( 1000 ); };
}

int main(int argc,char *argv[] ) {  

    /*���������в���*/
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

    /*��������Ϣ*/
    for( auto it = args.begin( ); it != args.end( ); it++ ) {
        std::cout<<'['<< it->first<<':'<<it->second<<']'<<'\n';
    }

    /*��ӡ������Ϣ*/
    if( args.find( std::string( "-help" ) ) != args.end( ) ) {
        printf( "\
            /************************************************************************/\n\
            �����в������� \n\
            -help ��ӡ����  \
            -send [ i ] ���ͷ����� \n\
            -recv [ i ] ���շ�����  \n\
            -id   ��id \n\
            -reliability 1/2    1�ɿ�  2���ɿ� \n\
            -send_model     ����ģʽ���������գ��෢���գ��������գ��෢���գ�\n\
                            Ŀǰ�༴2����������Ҫ���� \n\n\
            -topic_list     11 һ�����⣨topic1 type1) \n\
                            1122 ��Ϊ��������������ͬ������  \n\
            -topic_match    ���շ��������ʱ����˳�� \n\n\
                            DataWriter/DataWriter �������ĵ�topic \n\
                            ���綩�ķ���ָ��-recv����\n\
                            121  ָDataReader1 ����topoic[1-1] R2 ���� topic[2-1] R3 ���� topic[1-1] \n\
                            ���緢������ָ��-send����\n\
                            121  ָDataWriter1 ����topoic[1-1] W2 ���� topic[2-1] W3 ���� topic[1-1] \n\
            /************************************************************************/\n");
        return 0;
    }

    if( args.find( std::string( "-send" ) ) == args.end( ) ) is_sender = false;

    


    const int buff_len = 4 * 1024;
    DDS_DomainParticipant* participant;
    DDS_DomainParticipantQos* stDomainQos = nullptr;
    DDS_DataWriterQos writerQos;  //Ϊʲô������ָ�����ķ�ʽ����  DDS_Topic_get_default_datawriter_qos( topic , &writerQos );ʧ��

    DDS_ParticipantFactory_init( );
    DDS_DomainParticipantFactory_get_defaul_participant_qos( stDomainQos );

    int id =  atoi( (args.find( std::string( "-id" ) ))->second.c_str( ) );
  
    participant = DDS_DomainParticipantFactory_create_participant( id , stDomainQos );

    
    /*����topic ����*/
    std::vector<std::pair<char , char>> topic_lists;
    DDS_Topic** topic = nullptr;
    auto it = args.begin( );
    if( ( it = args.find( std::string( "-topic_list" ) ) ) != args.end( ) ) {
        int list = atoi( it->second.c_str( ) );
        while( list > 0 ) {
            int temp1 = list % 10; //��λ
            temp1 /= 10;
            int temp2 = list % 10; //��λ
            temp2 /= 10;
            topic_lists.push_back( std::make_pair( temp2 , temp1 ) );//��λ��topic ��λ:type
        }
        reverse( topic_lists.begin( ) , topic_lists.end( ) );    //��ת˳��
        topic = new DDS_Topic* [topic_lists.size()];
        char topic_prefix [8] = "topic"; // Ϊ�˰�ȫ�����ж�һ��numռ�ݵ�λ���Ƿ����
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

    /*����writer �� reader ���� */
    DDS_Topic_get_default_datawriter_qos( topic[0] , &writerQos );  //Ϊʲô��ȡĬ��qos����Ҫ��topic?   ʵ����û���õ�����������c++���thisָ�룬��������ù�ϵ��
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
        reverse( topic_matchs.begin( ) , topic_matchs.end( ) ); //��ת˳��
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
            for( int i = 0; i < topic_matchs.size( ); i++ ) {  //����Ӧ���ö��߳�����������̳߳�
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
    /* ������� */
    //DDS_DomainParticipant_delete_topic( participant , topic );
    /* ��������� */
    DDS_DomainParticipantFactory_delete_participant( participant );

    return 0;
}



