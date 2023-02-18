#define TEST02_C
#include "test02.h"
#include"ooo.h"
#include"GlobalDefine.h"
#include<memory.h>
#include <stdlib.h>
#include<stdint.h>
#include<string.h>

#ifdef WIN32
#define alignof(a) _alignof(a)
#endif

typedef void( *FUN )( );


void fun( );
void fun2( );
static struct This mainthis = { fun , fun2 };


///在类中添加这个成员
typedef struct _Obj {
    This* this;
}Obj;



Obj* Obj_new( ) {                                                               //需要区分用户是在堆栈还是堆上声明的对象。如果是堆栈上使用init 如果是 堆上使用new 分配后再初始化返回。
    Obj  *obj = (Obj*) malloc( sizeof( Obj ) );
    obj->this = (This*) malloc( sizeof( This ) );
    obj->this->con = mainthis.con;
    obj->this->des = mainthis.des;
    return obj;

    /*init->this->con = mainthis.con;
    init->this->des = mainthis.des;
    return init;*/
}

/////////////////////////////////
void fun( ) {
    printf( "con\n" );
    return;
}

void fun2( ) {
    printf( "des\n" );
    return;
}

struct myData {
    char a;
    long l;
};
 

int main( ) {

    
    Obj* obj = Obj_new( );
    obj->this->con( );
    obj->this->des( );
    donothing( ); //定义了不同的This 说明可以在不同的头文件中包含同名的struct定义，只要保证包含的头文件能够独立包含。  但重构量太大了，基本不可能。



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
    DDS_DomainParticipant_delete_topic( participant , topic );


    //data_writer 的内存关联了缓存
    while( 1 ) {
        dataWriter = DDS_Topic_create_datawriter( topic , NULL , NULL );
        //DDS_DataWriter_delete_from_topic( dataWriter );
        DDS_Topic_delete_datawriter( topic );
        //UninitHistoryCache( &participant->stBuiltinDataWriter [1].stHistoryCache );  没有导出，不是用户接口 ，只有再删除参与者时才是否内存。
    }




    DDS_DomainParticipant_delete_topic(participant, topic);
    DDS_DomainParticipant_delete_topic_by_name( participant , "name" , "type" );


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