
#ifndef _TS_LIB_H_
#define _TS_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*#include "apexType.h"*/
//#include <mipcHv/mipc.h>
//#include <mipcHv/mtlLib.h>
//#include <mipcHv/msl_consts.h>
//#include <safeipc/safeipc.h>
//#include "apex/apexLib.h"
//#include <taskLibCommon.h>
#include <string.h>
#include <stdio.h>

#ifndef _VXWORKS_653
#ifndef _TM3
typedef enum RETURN_CODE_VALUE_TYPE        /* return code type */
    {
#ifndef _WIN32
    NO_ERROR       = 0, /* request valid and operation performed       */
#endif
    NO_ACTION      = 1, /* status of system unaffected by request      */
    NOT_AVAILABLE  = 2, /* resource required by request is unavailable */
    INVALID_PARAM  = 3, /* invalid parameter specified in request      */
    INVALID_CONFIG = 4, /* parameter incompatible with configuration   */
    INVALID_MODE   = 5, /* request incompatible with current mode      */
    TIMED_OUT      = 6  /* timeout tied-up with request has expired    */
    } RETURN_CODE_TYPE;

    typedef unsigned int MESSAGE_SIZE_TYPE;
#endif
#endif
/*connection_id定义*/
#define TSS_A_B_RECV_1 0
#define TSS_A_B_SEND_1 1
#define TSS_B_A_RECV_1 2
#define TSS_B_A_SEND_1 3
#define Topic_1 4

typedef unsigned int Uint32;
typedef unsigned short Uint16;
typedef unsigned char Uchar;


#define MAX_TOPIC_NAME_LEN		64	/*主题名最大长度*/

/*UDP类型*/
#define UDP_TYPE_RECV  0xA5A5A5A5 /*接收UDP, 利用UDP_Recv()进行接收*/
#define UDP_TYPE_SEND  0x7E7E7E7E /*发送UDP, 利用UDP_Send()进行发送*/
#define MAX_TSS_NUM	1000		  /*允许TSS通道最大个数*/
#define MAX_TRANS_NUM	100		  /*数据分发表内容上限*/

/*MIPC表格内容是否有效*/
#define MSG_VALID		1
#define MSG_INVALID		0

/*MIPC表格收发方向*/
#define MSG_SEND		0			
#define MSG_RECV		1			

/*通道连接名类型*/
typedef char CONNECTION_NAME_TYPE[64];
/*事物标识类型*/
typedef unsigned int TRANSACTION_ID_TYPE;
/*消息类型ID类型*/
typedef unsigned int MESSAGE_TYPE_GUID;
/*通道ID类型*/
typedef unsigned int CONNECTION_ID_TYPE;
/*数据最大长度类型*/
/*typedef unsigned int MESSAGE_SIZE_TYPE;*/   /*FWQ 2020-12-7 重复定义冲突*/
/*数据最大长度类型*/
typedef unsigned int CONNECTION_DIRECTION_TYPE;

typedef long long TRANS_INFO;
/*数据地址类型*/
/*typedef unsigned char* MESSAGE_ADDR_TYPE;*/	/*FWQ 2020-12-7 重复定义冲突*/
/*Partition PCT type*/
typedef struct {
	char connection_name[64];
	char connection_type;
	unsigned int connection_direction;
	char source_addresss[24];
	char dest_address[24];
	unsigned int max_message_size;
	unsigned int source_port;	/*发送方UDP端口号(输出UDP时为本方发送UDP端口号，输入UDP时为接收模式: 0-阻塞, 1-非阻塞)*/
	unsigned int dest_port;		/*接收方UDP端口号(输出UDP时为对方接收UDP端口号, 输入UDP时为本方接收UDP端口号)*/
	unsigned int QOS;
} PART_PCT_TYPE;


///*Partition PCT type 用于653 APEX端口的定义*/
//typedef struct {
//	Uint32		MSG_ID;				/*Message ID defined by system*/
//	Uint32 		PORT_TYPE;			/*0: QUEUING 1: SAMPLING*/
//	NAME_TYPE 	PORT_NAME;
//	Uint32 		PORT_DIRECTION;		/*0：SOURCE 1：DESTINATION*/
//	Uint32 		MSG_SIZE;
//	Uint32 		QUEUE_LEN;
//	Uint32 		QUEUE_DISCIPLINE;	/*0:FIFO, 1: PRIORITY; Only for QUEUING PORT*/
//	SYSTEM_TIME_TYPE		REFRESH_RATE;		/*Only for SAMPLING PORT*/
//	Uint32		PARTITION_NUM;      /*The partition number which the port belongs to*/
//	Uint32		MODULE_NUM;			/*The module number which the port belongs to*/
//} PART_PCT_TYPE_653;

/*应用使用端口信息*/
//typedef struct {
//	NAME_TYPE				PORT_NAME;
//	QUEUING_PORT_ID_TYPE	PORT_ID;
//}APP_PORT_INFO;

/*数据分发表具体内容*/
//typedef struct {
//	char 					ConnectionName[MAX_TOPIC_NAME_LEN];		/*该ICD的名字*/
//	struct mipc_sockaddr	DDSSelfSocket;							/*本分区MIPC结构体*/
//	struct mipc_sockaddr	DDSDestSocket;							/*目标分区MIPC结构体*/
//	int						DDSsd;									/*本分区绑定的MIPC套接字*/
//	unsigned int			TransferMode;							/*代表传输类型，send/recv，从应用分区看*/
//	char 					TopicName[MAX_TOPIC_NAME_LEN];			/*DDS的初始化名称，由ConnectionName+“_Topic”得到*/
//	unsigned int			Length;									/*数据长度*/
//	int						DomainID;								/*DDS的初始化域*/
//	int						QOS;									/*DDS使用的QOS  0-走DDS 1-走MIPC自回环*/
//	int						DDSFlag;								/*DDS创建标识，用于数据分发实体区分是否创建过DDS初始化函数, 0无效， 1有效*/
//	int						MIPCFlag;								/*MIPC创建标识，用于数据分发实体区分是否绑定套接字, 0无效， 1有效*/
//	int						TransBack;								/*直接回传初始化标识   0无效  1有效 */
//} TRANS_INFO;

/*分区的数据分发表*/

typedef struct
{
	unsigned short			part_name;						/*分区标识*/
	unsigned int 			PartitionNum;					/*分区号*/
	unsigned int 			Info_Num;						/*分发表内容总数*/
	TRANS_INFO				trans_info[MAX_TRANS_NUM];		/*分发表具体内容*/
} PART_TRANS_TABLE;

/*传输通道类型*/
typedef enum {
	SAMPLING_PORT, 	/*ARINC 653 sampling port*/
	QUEUING_PORT,	/*ARINC 653 queuing port*/
	Socket,			/*POSIX socket*/
	MQ,				/*POSIX queue*/
	SHM,			/*POSIX shared memory*/
	CORBA,			/*CORBA*/
	DDS				/*Data Distribution Services*/
} CONNECTION_TYPE;
/*Support publish/subscribe and request/response.*/
typedef enum {
	PUB_SUB,
	CLIENT,
	SERVER
} MESSAGING_PATTERN_TYPE;
/*配置文件名类型*/
typedef char CONFIGURATION_RESOURCE[256];

/*超时类型*/
/*typedef char SYSTEM_TIME_TYPE;*/
typedef long long    SYSTEM_TIME_TYPE;
typedef SYSTEM_TIME_TYPE TIMEOUT_TYPE;

/*返回值类型*/
#if 0
typedef enum RETURN_CODE_VALUE_Type        
    {
    NO_ERROR       = 0, /* request valid and operation performed       */ 
    NO_ACTION      = 1, /* status of system unaffected by request      */ 
    NOT_AVAILABLE  = 2, /* resource required by request is unavailable */ 
    INVALID_PARAM  = 3, /* invalid parameter specified in request      */ 
    INVALID_CONFIG = 4, /* parameter incompatible with configuration   */
    INVALID_MODE   = 5, /* request incompatible with current mode      */ 
    TIMED_OUT      = 6,  /* timeout tied-up with request has expired    */
	ERROR=-1,
    } RETURN_CODE_Type;
#endif
    
DDS_DLL void commInit(int * fcHandle);

/*接口函数声明*/ 
DDS_DLL void Initialize (
	/* in  */ CONFIGURATION_RESOURCE configuration,
	/* out */ RETURN_CODE_TYPE 		*return_code
);

DDS_DLL void Create_Connection (
	/* in  */ CONNECTION_NAME_TYPE 		connection_name,
	/* in  */ MESSAGING_PATTERN_TYPE 	pattern,
	/* out */ CONNECTION_ID_TYPE 		*connection_id,
	/* out */ CONNECTION_DIRECTION_TYPE *connection_direction,
	/* out */ MESSAGE_SIZE_TYPE 		*max_message_size,
	/* in */  TIMEOUT_TYPE              timeout,
	/* out */ RETURN_CODE_TYPE 			*return_code
);

DDS_DLL void Receive_Message
( 
	/* in    */	 CONNECTION_ID_TYPE 	connection_id,
	/* in    */	 TIMEOUT_TYPE 			timeout,
	/* inout */  TRANSACTION_ID_TYPE 	*transaction_id,
	/* out   */  void 					*message,
	/* out   */	 MESSAGE_SIZE_TYPE 		*message_size,
	/* out   */  RETURN_CODE_TYPE 		*return_code
);

DDS_DLL void Send_Message
( 
	/* in    */	 CONNECTION_ID_TYPE 	connection_id,
	/* in    */	 TIMEOUT_TYPE 			timeout,
	/* inout */  TRANSACTION_ID_TYPE 	*transaction_id,
	/* inout */  void                	*message,
	/* in    */  MESSAGE_SIZE_TYPE 		message_size,
	/* out   */	 RETURN_CODE_TYPE 		*return_code
);

DDS_DLL void Get_Connection_Id(
	/* in  */ CONNECTION_NAME_TYPE connection_name, 
	/* out */ CONNECTION_ID_TYPE   *connection_id, 
	/* out */ RETURN_CODE_TYPE 	   *retCode
);

typedef void (*Read_Callback)(const void* buf, long length,int domain_id, const char* topic_name);

DDS_DLL void Register_Callback(
		/* in    */	 CONNECTION_ID_TYPE 	connection_id,
		/* in    */	 TIMEOUT_TYPE 			timeout,
		/* in    */  Read_Callback			Cback,
		/* in    */  MESSAGE_SIZE_TYPE 		message_size,
		/* out   */	 RETURN_CODE_TYPE 		*return_code
);

DDS_DLL char * codeToStr(RETURN_CODE_TYPE retCode);



#if 0  /*目前不支持*/
void TS_Destroy_Connection (
	/* in  */ 	CONNECTION_ID_TYPE 		connection_id,
	/* out */ 	RETURN_CODE_TYPE		*return_code
);
/*读回调句柄*/
typedef void (*Read_Callback) (
		/* in     */ TRANSACTION_ID_TYPE transaction_id,
		/* in out */ void  *message,
		/* in     */ MESSAGE_TYPE_GUID message_type_id,
		/* in     */ MESSAGE_SIZE_TYPE message_size,
		/* in     */ WAITSET_TYPE waitset,
		/* out    */ RETURN_CODE_TYPE *return_code
);

void TS_Get_Connection_Parameters (
	/* inout */ CONNECTION_NAME_TYPE 	*connection_name,
	/* inout */ CONNECTION_ID_TYPE 		*connection_id,
	/* out   */ TRANSPORT_CONNECTION_STATUS_TYPE 	*connection_status,
	/* out   */ RETURN_CODE_TYPE 		*return_code
);

void TS_Register_Callback( 
	/* in    */  CONNECTION_ID_TYPE 	connection_id,
	/* in    */  WAITSET_TYPE 			waitset,
	/* inout */  Read_Callback 			data_callback,
	/* in 	 */  MESSAGE_SIZE_TYPE 		max_message_size,
	/* out   */	 RETURN_CODE_TYPE 		*return_code
);

void TS_Unregister_Callback (
	/* in  */ CONNECTION_ID_TYPE connection_id,
	/* out */ RETURN_CODE_TYPE 	 *return_code
);


void TRANSPORT_MESSAGE_PACK(
	/* in    */	 CONNECTION_ID_TYPE 	connection_id,
	/* in    */	 MESSAGE_TYPE_GUID 		message_type_id, 	/*源消息类型ID*/
	/* in    */  MESSAGE_SIZE_TYPE 		message_size,
	/* in    */	 MESSAGE_ADDR_TYPE 		message,
	/* out   */	 TRANSPORT_MESSAGE_BLOCK	*message_block, /*数据传输消息块*/
	/* out   */	 RETURN_CODE_TYPE 		*return_code
);

void TRANSPORT_MESSAGE_UNPACK(
	/* in    */	 CONNECTION_ID_TYPE 	connection_id,
	/* out   */  MESSAGE_ADDR_TYPE 		message,
	/* in    */	 TRANSPORT_MESSAGE_BLOCK	*message_block, /*数据传输消息块*/
	/* out   */	 RETURN_CODE_TYPE 		*return_code
);
#endif

///*初始化传输表格的端口*/
//void create_app_table_init_ports();
//
///*增加本地传输表格内容*/
//void local_Trans_table_add(TRANS_INFO table_info);
//
///*根据PCT表格创建本地MIPC表格*/
//void MIPC_Table_Create(void);
//
///*将本地生成的数据转发表格传输至TSS分区*/
//void local_Trans_table_report(void);
//
///*使能本地收发表中的内容*/
//void local_Trans_table_enable(void);
//
///*MIPC端口初始化函数*/
//int MIPC_init(struct mipc_sockaddr *self,struct mipc_sockaddr *dest, int *sd, int TransMode, NAME_TYPE port_name);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*#ifndef _TS_LIB_H*/
