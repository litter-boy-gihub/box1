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

    /* 测试创建删除是否出现错误是否有内存泄漏*/
    //while( 1 ) {
    //    participant = DDS_DomainParticipantFactory_create_participant( 1 , NULL );
    //    //DDS_DomainParticipantFactory_delete_participant( participant );             //成员指针moniter没有释放内存
    //}
    
    




    topic = DDS_DomainParticipant_create_topic( participant , "data" , "type" , NULL );



    //data_writer 的内存关联了缓存
    while( 1 ) {
        dataWriter = DDS_Topic_create_datawriter( topic , NULL , NULL );
        //DDS_DataWriter_delete_from_topic( dataWriter );
        DDS_Topic_delete_datawriter( topic );
        //UninitHistoryCache( &participant->stBuiltinDataWriter [1].stHistoryCache );  没有导出，不是用户接口 ，只有再删除参与者时才是否内存。
    }


        

    DDS_DomainParticipant_delete_topic_by_name(participant,"data","type" );


    topic = DDS_DomainParticipant_create_topic( participant , "data" , "type" , NULL );
    DDS_DomainParticipant_delete_topic( participant , topic );


    DDS_DomainParticipantFactory_delete_participant( participant );


    ///* 清除写入器 */
    //DDS_Topic_delete_datawriter( pstTopic );

    ///* 清除主题 */
    //DDS_DomainParticipant_delete_topic( pstParticipant , pstTopic );

    ///* 清除参与者 */
    //DDS_DomainParticipantFactory_delete_participant( pstParticipant );

    return 0;
}                                                                                 