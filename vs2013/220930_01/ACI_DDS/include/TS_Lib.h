
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
/*connection_id����*/
#define TSS_A_B_RECV_1 0
#define TSS_A_B_SEND_1 1
#define TSS_B_A_RECV_1 2
#define TSS_B_A_SEND_1 3
#define Topic_1 4

typedef unsigned int Uint32;
typedef unsigned short Uint16;
typedef unsigned char Uchar;


#define MAX_TOPIC_NAME_LEN		64	/*��������󳤶�*/

/*UDP����*/
#define UDP_TYPE_RECV  0xA5A5A5A5 /*����UDP, ����UDP_Recv()���н���*/
#define UDP_TYPE_SEND  0x7E7E7E7E /*����UDP, ����UDP_Send()���з���*/
#define MAX_TSS_NUM	1000		  /*����TSSͨ��������*/
#define MAX_TRANS_NUM	100		  /*���ݷַ�����������*/

/*MIPC��������Ƿ���Ч*/
#define MSG_VALID		1
#define MSG_INVALID		0

/*MIPC����շ�����*/
#define MSG_SEND		0			
#define MSG_RECV		1			

/*ͨ������������*/
typedef char CONNECTION_NAME_TYPE[64];
/*�����ʶ����*/
typedef unsigned int TRANSACTION_ID_TYPE;
/*��Ϣ����ID����*/
typedef unsigned int MESSAGE_TYPE_GUID;
/*ͨ��ID����*/
typedef unsigned int CONNECTION_ID_TYPE;
/*������󳤶�����*/
/*typedef unsigned int MESSAGE_SIZE_TYPE;*/   /*FWQ 2020-12-7 �ظ������ͻ*/
/*������󳤶�����*/
typedef unsigned int CONNECTION_DIRECTION_TYPE;

typedef long long TRANS_INFO;
/*���ݵ�ַ����*/
/*typedef unsigned char* MESSAGE_ADDR_TYPE;*/	/*FWQ 2020-12-7 �ظ������ͻ*/
/*Partition PCT type*/
typedef struct {
	char connection_name[64];
	char connection_type;
	unsigned int connection_direction;
	char source_addresss[24];
	char dest_address[24];
	unsigned int max_message_size;
	unsigned int source_port;	/*���ͷ�UDP�˿ں�(���UDPʱΪ��������UDP�˿ںţ�����UDPʱΪ����ģʽ: 0-����, 1-������)*/
	unsigned int dest_port;		/*���շ�UDP�˿ں�(���UDPʱΪ�Է�����UDP�˿ں�, ����UDPʱΪ��������UDP�˿ں�)*/
	unsigned int QOS;
} PART_PCT_TYPE;


///*Partition PCT type ����653 APEX�˿ڵĶ���*/
//typedef struct {
//	Uint32		MSG_ID;				/*Message ID defined by system*/
//	Uint32 		PORT_TYPE;			/*0: QUEUING 1: SAMPLING*/
//	NAME_TYPE 	PORT_NAME;
//	Uint32 		PORT_DIRECTION;		/*0��SOURCE 1��DESTINATION*/
//	Uint32 		MSG_SIZE;
//	Uint32 		QUEUE_LEN;
//	Uint32 		QUEUE_DISCIPLINE;	/*0:FIFO, 1: PRIORITY; Only for QUEUING PORT*/
//	SYSTEM_TIME_TYPE		REFRESH_RATE;		/*Only for SAMPLING PORT*/
//	Uint32		PARTITION_NUM;      /*The partition number which the port belongs to*/
//	Uint32		MODULE_NUM;			/*The module number which the port belongs to*/
//} PART_PCT_TYPE_653;

/*Ӧ��ʹ�ö˿���Ϣ*/
//typedef struct {
//	NAME_TYPE				PORT_NAME;
//	QUEUING_PORT_ID_TYPE	PORT_ID;
//}APP_PORT_INFO;

/*���ݷַ����������*/
//typedef struct {
//	char 					ConnectionName[MAX_TOPIC_NAME_LEN];		/*��ICD������*/
//	struct mipc_sockaddr	DDSSelfSocket;							/*������MIPC�ṹ��*/
//	struct mipc_sockaddr	DDSDestSocket;							/*Ŀ�����MIPC�ṹ��*/
//	int						DDSsd;									/*�������󶨵�MIPC�׽���*/
//	unsigned int			TransferMode;							/*���������ͣ�send/recv����Ӧ�÷�����*/
//	char 					TopicName[MAX_TOPIC_NAME_LEN];			/*DDS�ĳ�ʼ�����ƣ���ConnectionName+��_Topic���õ�*/
//	unsigned int			Length;									/*���ݳ���*/
//	int						DomainID;								/*DDS�ĳ�ʼ����*/
//	int						QOS;									/*DDSʹ�õ�QOS  0-��DDS 1-��MIPC�Իػ�*/
//	int						DDSFlag;								/*DDS������ʶ���������ݷַ�ʵ�������Ƿ񴴽���DDS��ʼ������, 0��Ч�� 1��Ч*/
//	int						MIPCFlag;								/*MIPC������ʶ���������ݷַ�ʵ�������Ƿ���׽���, 0��Ч�� 1��Ч*/
//	int						TransBack;								/*ֱ�ӻش���ʼ����ʶ   0��Ч  1��Ч */
//} TRANS_INFO;

/*���������ݷַ���*/

typedef struct
{
	unsigned short			part_name;						/*������ʶ*/
	unsigned int 			PartitionNum;					/*������*/
	unsigned int 			Info_Num;						/*�ַ�����������*/
	TRANS_INFO				trans_info[MAX_TRANS_NUM];		/*�ַ����������*/
} PART_TRANS_TABLE;

/*����ͨ������*/
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
/*�����ļ�������*/
typedef char CONFIGURATION_RESOURCE[256];

/*��ʱ����*/
/*typedef char SYSTEM_TIME_TYPE;*/
typedef long long    SYSTEM_TIME_TYPE;
typedef SYSTEM_TIME_TYPE TIMEOUT_TYPE;

/*����ֵ����*/
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

/*�ӿں�������*/ 
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



#if 0  /*Ŀǰ��֧��*/
void TS_Destroy_Connection (
	/* in  */ 	CONNECTION_ID_TYPE 		connection_id,
	/* out */ 	RETURN_CODE_TYPE		*return_code
);
/*���ص����*/
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
	/* in    */	 MESSAGE_TYPE_GUID 		message_type_id, 	/*Դ��Ϣ����ID*/
	/* in    */  MESSAGE_SIZE_TYPE 		message_size,
	/* in    */	 MESSAGE_ADDR_TYPE 		message,
	/* out   */	 TRANSPORT_MESSAGE_BLOCK	*message_block, /*���ݴ�����Ϣ��*/
	/* out   */	 RETURN_CODE_TYPE 		*return_code
);

void TRANSPORT_MESSAGE_UNPACK(
	/* in    */	 CONNECTION_ID_TYPE 	connection_id,
	/* out   */  MESSAGE_ADDR_TYPE 		message,
	/* in    */	 TRANSPORT_MESSAGE_BLOCK	*message_block, /*���ݴ�����Ϣ��*/
	/* out   */	 RETURN_CODE_TYPE 		*return_code
);
#endif

///*��ʼ��������Ķ˿�*/
//void create_app_table_init_ports();
//
///*���ӱ��ش���������*/
//void local_Trans_table_add(TRANS_INFO table_info);
//
///*����PCT��񴴽�����MIPC���*/
//void MIPC_Table_Create(void);
//
///*���������ɵ�����ת���������TSS����*/
//void local_Trans_table_report(void);
//
///*ʹ�ܱ����շ����е�����*/
//void local_Trans_table_enable(void);
//
///*MIPC�˿ڳ�ʼ������*/
//int MIPC_init(struct mipc_sockaddr *self,struct mipc_sockaddr *dest, int *sd, int TransMode, NAME_TYPE port_name);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*#ifndef _TS_LIB_H*/
