/*********************************************************************************
*  OpenST Basic tool library
*  Copyright (C) 2019 QiYuan
*
*
*  This program is security software
*  without permission you cannot use or distribute the software
*  otherwise, legal liability will be investigated
*
*  If you want to use the software, please contact the authorities to obtain a license
*  < http://www.iamqy.com.cn >.
*
*  @file     DomainParticipant.h
*  @brief    Participant
*  Details.
*
*  @author   Hunter
*  @email    1028251654@qq.com
*  @version  1.0
*  @date     2019/08/08
*  @license  
*
*--------------------------------------------------------------------------------
*  Remark         : Description
*--------------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>  | <operation> | <Description>
*--------------------------------------------------------------------------------
*  2019/08/08 | 1.0       | Hunter    | Create file |
*--------------------------------------------------------------------------------
*
*********************************************************************************/
#ifndef HIPERD_PARTICIPANT_H
#define HIPERD_PARTICIPANT_H

#if defined _FC
#define NETWORK_BYTE_MAX_SIZE (1024* 64)
#define FC_NETWORK_BYTE_MAX_SIZE (1024* 64)
#else
#define NETWORK_BYTE_MAX_SIZE (1024*64)
#endif

/* Nephalem SEDP一般会不会超过512字节 */
#define SEDP_BYTE_MAX_SIZE  512
//#define DomainId unsigned int

extern BOOL g_RapdIOValid;
/****************************************************************************************************************************************/
//结构体定义
/****************************************************************************************************************************************/

extern DDS_MUTEX g_sendMutex;  //线程加锁v

typedef struct _SocketList
{
    SocketInfo spdp_data_sender;
    SocketInfo spdp_data_receiver;
    SocketInfo sedp_data_sender;
    SocketInfo sedp_data_receiver;
    SocketInfo user_data_sender;
    SocketInfo user_data_receiver;

}SocketList;

typedef struct _DomainParticipant
{
	/************************ 基础信息 ****************************/
    RTPSMessageHeader             stRTPSMsgHeader;
    ParticipantData               dcpsParticipantData;
	DDS_DomainParticipantQos	  pstParticipantQos;
    ParticipantProxy              rtpsParticipantProxy;

	SocketList                    socketList;             /* 参与者与外部通信的所有socket，组播发送与接收、单播发送与接收、用户单播发送与接收共6个 */
	DDS_Topic*                    pstTopic;				  /* 创建的主题信息 */
	DDS_DiscoveredParticipant*    pstDiscParticipant;     /* 网络上远端参与者信息 */
	/************************ 行为信息 ****************************/
	BuiltinDataWriter             stBuiltinDataWriter[3]; /* 1、参与者；2发布者；3订阅者，分别用来发送Data(p)、Data(w)、Data(r)报文 */
	BuiltinDataReader             stBuiltinDataReader[3]; /* 1、参与者；2发布者；3订阅者，分别用来处理Data(p)、Data(w)、Data(r)报文 */
	Duration_t                    livelinessDuration;	  //自身存活性心跳时间，0为未设置存活性
	Duration_t					  discMinDuration;		  //所有远端participant中最小的心跳间隔，作为存活检测线程周期
    HighPrecisionTimeBlock        heartBeatMark;
    
    DDS_Monitor*                  pstMonitor;
    BOOL                          bLoopBack;
    BOOL                          destroyFlag;

	int							  sleepTime;			//livelinessQoS下检测心跳线程休眠时间
	int 						  livelinessNum;		//远端配置了存活性的participant现存数量
#ifdef  STATIC_DISCOVERY
	BOOL						  bOnlyStaticMathc;
#endif
	/************************ 调试辅助信息 ****************************/
	int							  processId;		//进程ID
	EX_UINT32					  ipAddr;			//IP地址
//    EX_UINT32                   netMask;			//子网掩码
#if defined(_WIN32)
    HANDLE hThread[6];			  //收发任务线程
#elif defined(__linux__) || defined(_VXWORKS)
    pthread_t hThread[6];
#endif
	/************************ 锁和信号量 ****************************/
    BOOL      threadSignal;		//信号量
	DDS_MUTEX sleepMutex;	
    DDS_MUTEX threadMutex;		//线程加锁
    DDS_MUTEX m_discWtMutex;    //线程加锁，远端writer的删除与处理远端writer报文互斥
    DDS_MUTEX m_discRdMutex;    //线程加锁，远端reader的删除与处理远端reader报文互斥
    DDS_MUTEX monMutex;			//监控线程加锁
}DDS_DomainParticipant;

#if defined _FC
/* FC通信配置 */
//typedef struct TagSYCCommFCConfig
//{
//	UINT32							m_fcNum;				/* FC设备最大数量（可包含当前未上线，但未来可能上线的设备） */
//	UINT32							m_fcId;					/* 当前FC设备ID */
//	UINT32							m_deviceFCId[24];		/* 各FC设备ID（需包含当前设备，以确定当前设备的序号，并且所有设备上此数据应统一） */
//	UINT32							m_fcSeq;
//	UINT32                          m_role;
//} YCCommFCConfig;

#define FC_MAX_NODE_NUM 32
#define FC_MAX_STREAM_DATA_SIZE 2096

typedef struct TagSYCCommFCOneConfig
{
	UINT32						m_FCPort;
	UINT32 						m_localDDSSendMsgID;
	UINT32						m_localDDSRecvMsgID;
	UINT32 						m_localSEPDSendMsgID;
	UINT32						m_localSEPDRecvMsgID;
	UINT32						m_localUserSendMsgID;
	UINT32						m_localUserRecvMsgID;	
}YCCommFCOneConfig;

typedef struct TagSYCCommFCConfig
{
	int							m_nodeNum;
	int							m_thisNodeFCPort;
	int *						m_FCdev;
	YCCommFCOneConfig			m_configs[FC_MAX_NODE_NUM];	
}YCCommFCConfig;

#ifdef _FC
 extern YCCommFCConfig g_commFCConfig;
 extern int *						FCdev;
#elif defined _653_PORT
extern Port653Config g_PortConfig;

#endif

#elif defined _653_PORT

#define NODE_MAX_NUM 16U

typedef struct PORTSET
{
	UINT32 portSeq;
	char* portName;
	QUEUING_PORT_ID_TYPE portID;
}PortSet;

typedef struct POTConfig
{
	UINT32	m_localPartitionId;
	
	UINT32 SendPortNum;
	UINT32 recvPortNum;
	PortSet SendPort[NODE_MAX_NUM];
	PortSet recvPort[NODE_MAX_NUM];
		
}Port653Config;

#endif

/***************************************************************************************************************************/
/**************************************** 对外用户接口函数声明 *************************************************************/
/***************************************************************************************************************************/
/* dp工厂资源初始化：1、日志初始化；2、任务列表初始化；3、静态内存分配；4、Rapidio初始化 */
extern DDS_DLL VOID DDS_ParticipantFactory_init();
/* 通过participant工厂创建participant */
extern DDS_DLL DDS_DomainParticipant* DDS_DomainParticipantFactory_create_participant(USHORT domainId, DDS_DomainParticipantQos* pstDomPartQos);
/* 不忽略本地回环 */
extern DDS_DLL VOID DDS_DomainParticipant_not_ignore_participant(DDS_DomainParticipant* pstDomainParticipant);
/* 忽略本地回环 */
extern DDS_DLL VOID DDS_DomainParticipant_ignore_participant(DDS_DomainParticipant* pstDomainParticipant);
/* 通过participant工厂删除participant */
extern DDS_DLL DDS_ReturnCode_t DDS_DomainParticipantFactory_delete_participant(DDS_DomainParticipant* pstDomainParticipan);
/* 获取默认的dp_QoS */
extern DDS_DLL DDS_ReturnCode_t DDS_DomainParticipantFactory_get_defaul_participant_qos(DDS_DomainParticipantQos* pstDomPartQos);
/* c#类型接口 */
extern DDS_DLL DDS_DomainParticipantQos* DDS_DomainParticipantFactory_get_defaul_participant_qos_cs();
/* dp绑定网卡Ip */ //？？在QoS上可以配置，为什么要加一个接口
extern DDS_DLL VOID DDS_Participant_bind_network(DDS_DomainParticipantQos* pstDomPartQos, const CHAR* pcIpAddr);
/************************************* 创建、查询、删除Topic *******************************************/
/* 通过participant创建主题 */
extern DDS_DLL DDS_Topic*  DDS_DomainParticipant_create_topic(DDS_DomainParticipant* pstParticipant, const char* pcTopicName, const char* pcTopicType, DDS_TopicQos* pstTopicQos);
/* 通过participant和topic_name查询主题 */
extern DDS_DLL DDS_Topic*  DDS_DomainParticipant_get_topic_by_name(DDS_DomainParticipant* pstParticipant, const CHAR* pcTopicName, const CHAR* pcTopicType);
/* 获取主题个数 */
extern DDS_DLL UINT32  DDS_DomainParticipant_get_topic_num(DDS_DomainParticipant* pstParticipant);
/* 通过participant删除主题 */
extern DDS_DLL DDS_ReturnCode_t  DDS_DomainParticipant_delete_topic(DDS_DomainParticipant* pstParticipant, DDS_Topic* pstTopic);
/* 通过topicName删除主题 */
extern DDS_DLL DDS_ReturnCode_t  DDS_DomainParticipant_delete_topic_by_name(DDS_DomainParticipant* pstParticipant, const CHAR* pcTopicName, const CHAR* pcTopicType);
/* 通过participant获取默认的主题qos */
extern DDS_DLL DDS_ReturnCode_t DDS_DomainParticipant_get_default_topic_qos(DDS_DomainParticipant* pstParticipant, DDS_TopicQos* pstTopicQos);
/* 通过participant获取默认的主题qos */
extern DDS_DLL DDS_TopicQos* DDS_DomainParticipant_get_default_topic_qos_cs(DDS_DomainParticipant* pstParticipant);

/* Nephalem Partition 主题加入分区 */
extern DDS_DLL DDS_ReturnCode_t AddPartition(DDS_Topic* pstTopic, const CHAR* pstPartitionname);
/* Nephalem Partition 主题删除分区 */
extern DDS_DLL DDS_ReturnCode_t DeletePartition(DDS_Topic* pstTopic, const CHAR* pstPartitionname);
/* Nephalem Partition 使修改Partition后的QoS生效 */ //？？LJM
extern DDS_DLL DDS_ReturnCode_t SetTopicQoS(DDS_Topic* pstTopic);

/**********************************************************************************************************************/
/******************************************* 内部函数声明 *************************************************************/
/**********************************************************************************************************************/
/**************************** 信息操作 ********************************/
/* 初始化participant成员 */
extern VOID InitDomainParticipant(DDS_DomainParticipant* pstDomainParticipant);
#ifdef _FC
int RecvAndParseFAllNetworkOrder(EX_INT32 kind,CHAR* buffer,DDS_DomainParticipant* pstParticipant);
//extern DDS_DLL void FCInit();
//extern int FCIsReady();
#elif defined _653_PORT
extern DDS_DLL void PortInit();
#endif
/* 通过EntityId查找相应内建写入器 */
extern BuiltinDataWriter* GetBuiltinWriterFromParticipantByEntityId(DDS_DomainParticipant* pstDomainParticipant, EntityId_t* pstEntityId);
/* 通过EntityId查找相应内建阅读器 */
extern BuiltinDataReader* GetBuiltinReaderFromParticipantByEntityId(DDS_DomainParticipant* pstDomainParticipant, EntityId_t* pstEntityId);
/* 通过EntityId查找相应写入器 */
extern DDS_DataWriter* GetLocalWriterFromParticipantByEntityId(DDS_DomainParticipant* pstDomainParticipant, EntityId_t* pstEntityId);
/* 通过EntityId查找相应写入器 */
extern DDS_DataReader* GetLocalReaderFromParticipantByEntityId(DDS_DomainParticipant* pstDomainParticipant, EntityId_t* pstEntityId);
/* 通过Guid查找相应远端写入器 */
extern DDS_DiscoveredWriter* GetDiscoveredWriterFromParticipantByGuid(DDS_DomainParticipant* pstDomainParticipant, GUID_t* pstGuid);
/* 通过Guid查找相应远端阅读器 */
extern DDS_DiscoveredReader* GetDiscoveredReaderFromParticipantByGuid(DDS_DomainParticipant* pstDomainParticipant, GUID_t* pstGuid);
/* 主题中删除指定远端内建参与者 */
extern VOID DeleteBuiltinDiscParticipantFromParticipantByGuidPrefix(DDS_DomainParticipant* pstParticipant, GuidPrefix_t*  pstPrefix);
/* 删除远端所有连接信息 */
extern VOID DeleteALLDiscoveredFromByGuidPrefix(DDS_DomainParticipant* pstParticipant, GuidPrefix_t stPrefix);

/*********************** 初始化、创建任务线程 *********************/
/* 多任务网络收发任务 */
extern VOID NetworkSendReceiveTask(DDS_DomainParticipant* pstParticipant);
/* 多任务网络收发任务关闭 */
extern VOID CloseNetworkSendReceiveTask(DDS_DomainParticipant* pstParticipant);
/* 单任务网络收发任务 */
extern DDS_DLL VOID NetworkTransceiverTask(DDS_DomainParticipant* pstParticipant);
/* 周期检查 */
extern DDS_ReturnCode_t DeadlineCheck(DDS_DomainParticipant* pstParticipant);
/* 主题存活性 */
extern DDS_ReturnCode_t TopicLiveLinessCheck(DDS_DomainParticipant* pstParticipant);
/* 用户数据周期 */
VOID UserDataPeriodTask(DDS_DomainParticipant* pstParticipant);

/************************ 报文处理函数 ******************************/
/* 处理内建Ack报文 */
extern BOOL HandleBuiltinAckNackMsg(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader* pstSubMsgHeader, AckNack* pstAckNackMsg, DDS_DomainParticipant* pstParticipant);
/* 处理Ack报文 */
extern BOOL HandleAckNackMsg(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader* pstSubMsgHeader, AckNack* pstAckNackMsg, DDS_DomainParticipant* pstParticipant);
/* 处理分片Ack报文 */
extern BOOL HandleNackFragMsg(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader* pstSubMsgHeader, NackFrag* pstNackFragMsg, DDS_DomainParticipant* pstParticipant);
/* 内建心跳处理.是否需要数据重传 */
extern BOOL HandleBuiltinHeartbeatMsg(RTPSMessageHeader* pstRTPSMsgHeader, Heartbeat* pstHeartbeatMsg, DDS_DomainParticipant* pstParticipant);
/* 心跳处理.是否需要数据重传 */
extern BOOL HandleHeartbeatMsg(RTPSMessageHeader* pstRTPSMsgHeader, Heartbeat* pstHeartbeatMsg, DDS_DomainParticipant* pstParticipant);
/* 分片心跳处理.是否需要数据重传 */
extern BOOL HandleHeartbeatFragMsg(RTPSMessageHeader* pstRTPSMsgHeader, HeartbeatFrag* pstHeartbeatFragMsg, DDS_DomainParticipant* pstParticipant);

/*********************** 报文发送函数 ******************************/
/* 发送定时心跳任务 */
extern VOID SendHeartbeatTasksOrder(DDS_DomainParticipant* pstParticipant);
/* 向监控节点发送用户消息 */
extern VOID UserMsgSendMonitor(DDS_DomainParticipant* pstParticipant, GUID_t* tpGuid, MemoryBlock* pstMemBlock, UINT8 type);

#endif
