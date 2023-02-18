#include "GlobalDefine.h"

/* 阅读器共接收到的总包数 */
static int g_numpkg = 0;

/* 阅读器绑定的回调函数 */
VOID STACALL ReceiveData( DDS_DataReader* pstDataReader ) {
    UINT32 dataNum = 0;
    DataSeq stDataSeq;
    DataLen dataLen;
    static unsigned int seqNum = 0;

    /* 收数据 */
    DDS_ReturnCode_t retCode = DDS_StringDataReader_take( pstDataReader , &stDataSeq );//收不到数据，
    if( DDS_RETCODE_NO_DATA != retCode ) {
        for( dataNum = 0; dataNum < stDataSeq.length; dataNum++ ) {
            char * userData = DDS_StringSeq_get( &stDataSeq , dataNum , &dataLen );
            g_numpkg++;
            printf( "**********stDataSeq.length=%d g_numpkg = %d, data = %d  len = %d \n" , stDataSeq.length , g_numpkg , userData [0] , strlen( userData ) );
            for (int i = 0; i <dataLen  ; i++)
            {
                printf( "%c" , userData [i] );
            }
            printf( "\n" );
        }
        /* 释放缓存 */
        DDS_StringDataReader_return_loan( pstDataReader , &stDataSeq );
    }
}


int main(){
    UINT32 num = 0;
    DataSeq dataSeq;
    DataLen dataLen;
    DDS_ReturnCode_t ret;
    DDS_DataReaderQos r_qos;

    DDS_ParticipantFactory_init( );

    DDS_DomainParticipant * participant =
        DDS_DomainParticipantFactory_create_participant( 0 , NULL );
    if (NULL == participant){
        printf( "participant create failed" );
        return 1;
    }

    DDS_Topic *topic = DDS_DomainParticipant_create_topic( participant ,
        "topic_name" , "topic_type" , 0 );
    if (NULL == topic){
        printf( "topic create failed" );
        return 1;
    }

    ret = DDS_Topic_get_default_datareader_qos( topic , &r_qos );
    if( ret == DDS_RETCODE_ERROR ) {
        printf( "get r_qos failed" );
        return 1;
    }

    r_qos.reliability.kind = RELIABLE_RELIABILITY_QOS;

    DDS_DataReader *dataReader = DDS_Topic_create_datareader( topic , &r_qos , ReceiveData);
    if( NULL == dataReader ) {
        printf( "dataReader create failed" );
        return 1;
    }

 

    while (true)
    {
        continue;
    }

	return 0;
}