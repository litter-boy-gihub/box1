#include"GlobalDefine.h" 
#include<memory.h>
#include <stdlib.h>
#include<stdint.h>

#ifdef WIN32
#define alignof(a) _alignof(a)
#endif

struct myData {
    char a;
    long l;
};

extern VOID UninitHistoryCache( HistoryCache* pstHistoryCache );

int main( ) {
    
    short b;
    struct myData data;
    printf( "%d\n" , alignof( data ) );
    printf( "%x\n" , &data );
    printf( "%x" , &b );
    int len = sizeof( struct myData );
    DDS_DomainParticipant* participant;
    DDS_Topic* topic;
    DDS_DataWriter* dataWriter;

    DDS_ParticipantFactory_init( );
    participant = DDS_DomainParticipantFactory_create_participant( 1 , NULL );

    /* ���Դ���ɾ���Ƿ���ִ����Ƿ����ڴ�й©*/
    //while( 1 ) {
    //    participant = DDS_DomainParticipantFactory_create_participant( 1 , NULL );
    //    //DDS_DomainParticipantFactory_delete_participant( participant );             //��Աָ��moniterû���ͷ��ڴ�
    //}
    
    




    topic = DDS_DomainParticipant_create_topic( participant , "data" , "type" , NULL );



    //data_writer ���ڴ�����˻���
    while( 1 ) {
        dataWriter = DDS_Topic_create_datawriter( topic , NULL , NULL );
        //DDS_DataWriter_delete_from_topic( dataWriter );
        DDS_Topic_delete_datawriter( topic );
        //UninitHistoryCache( &participant->stBuiltinDataWriter [1].stHistoryCache );  û�е����������û��ӿ� ��ֻ����ɾ��������ʱ���Ƿ��ڴ档
    }


        

    DDS_DomainParticipant_delete_topic_by_name(participant,"data","type" );


    topic = DDS_DomainParticipant_create_topic( participant , "data" , "type" , NULL );
    DDS_DomainParticipant_delete_topic( participant , topic );


    DDS_DomainParticipantFactory_delete_participant( participant );


    ///* ���д���� */
    //DDS_Topic_delete_datawriter( pstTopic );

    ///* ������� */
    //DDS_DomainParticipant_delete_topic( pstParticipant , pstTopic );

    ///* ��������� */
    //DDS_DomainParticipantFactory_delete_participant( pstParticipant );

    return 0;
}                                                                                 