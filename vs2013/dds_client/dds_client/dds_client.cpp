#include "GlobalDefine.h"

VOID  STACALL GetWinTime( Time_t* timeinfo ) {
    timeinfo->sec = time( NULL );
}

int main(){
	
    UINT32 i = 0;
    CHAR* buff = NULL;
    UINT32 len = 32;
    DDS_ReturnCode_t ret;
    DDS_DataWriterQos w_qos;

    DDS_ParticipantFactory_init( );

    DDS_DomainParticipant *participant =
        DDS_DomainParticipantFactory_create_participant( 0 , NULL );
    if( NULL == participant ) {
        printf( "participant create failed" );
        return 1;
    }

    DDS_Topic*  topic = DDS_DomainParticipant_create_topic( participant ,
        "topic_name" , "topic_type" , NULL );
    if (NULL == participant){
        printf( "topic create failed" );
        return 1;
    }

    ret = DDS_Topic_get_default_datawriter_qos( topic , &w_qos );
    if (DDS_RETCODE_ERROR ==  ret){
        printf( "get_w_qos failed" );
        return 1;
    }

    w_qos.reliability.kind = RELIABLE_RELIABILITY_QOS;

    DDS_DataWriter *dataWriter = DDS_Topic_create_datawriter( topic , &w_qos , GetWinTime );
    if (NULL == dataWriter){
        printf( "datawriter create failed" );
        return 1;
    }

    buff = (CHAR*)DDS_STATIC_MALLOC( len );
    if (NULL == buff){
        printf( "buff malloc failed" );
        return 1;
    }
    Sleep( 5000 );
    printf( "start sending......\n" );

    for( int i = 0; i < 50;i++ )
    {
        sprintf_s( buff , len , "hello world from me(%d)" , i );
        ret = DDS_StringDataWriter_write( dataWriter , buff , len );
        if (ret == DDS_RETCODE_OK){
            printf( "send (%d)\n" , i );
        }
        else if( ret == DDS_RETCODE_ERROR ) {
            printf( "data write failed\n" );
        }
        else {
            printf( "other condition\n" );
        }
    }

   /* while( true ) {
        sprintf_s( buff ,len, "hello world from me(%d)" , ++i );
        ret = DDS_StringDataWriter_write( dataWriter , buff , len );
        if( ret == DDS_RETCODE_ERROR ) {
            printf( "data write failed" );
        }
        Sleep( 1000 );
    }*/

    while( true ) {

    }
    Sleep( 10000 );
	return 0;
}
