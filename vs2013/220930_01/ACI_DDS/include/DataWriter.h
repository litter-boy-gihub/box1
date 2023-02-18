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
*  @file     DataWriter.h
*  @brief    DataWriter
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

#ifndef HIPERD_DATAWRITER_H
#define HIPERD_DATAWRITER_H

# define HEARTBEAT_DISCONNECT 3

struct _Topic;

/****************************************************************************************************************************************/
//结构体定义
/****************************************************************************************************************************************/

typedef void(STACALL *CALLBACKTIME)(Time_t* pstTimeInfo);

/* 数据写入器 */
typedef struct _DataWriter
{
	/************************ 基础信息 ****************************/
	GUID_t              guid;				/* 本Writer的Guid */
	DDS_DataWriterQos   stDataWriterQos;	/* 写入器qos */
	HistoryCache        stHistoryCache;		/* 该writer的历史数据 */
	CALLBACKTIME        writerCallBack;		/* 时间戳生成回调 */
	/*********************** 行为信息 *****************************/
	BOOL                destroyFlag;
	//BOOL              bVaild;				/* 是否有效 */
	SequenceNumber_t    seqNum;				/* 记录用户数据编号,每插入一个新的数据自加1 */
	Heartbeat           stHeartbeat;		/* 本地写入器发送远端写入器心跳 */
	Duration_t			lastWriteTime;
	unsigned int		uiHistoryDataSize;	/* 当前缓存的所有报文总长度 */
#if defined(_WIN32)
    HANDLE hThread[2];      //收发任务线程
#elif defined(__linux__) || defined(_VXWORKS)
    pthread_t hThread[2];
#endif

	struct _Topic*      pstTopic;			/* 关联的父主题节点*/
    struct _DataWriter* pNext;
}DDS_DataWriter;

/*********************************************************************************************/
/********************************** 写入器用户接口定义 ***************************************/
/* 用户数据写入 */
extern DDS_DLL DDS_ReturnCode_t DDS_StringDataWriter_write(DDS_DataWriter* pstDataWriter, const CHAR* buff, UINT32 lenght);

/*********************************************************************************************/
/**************************************** 写入器信息操作 *************************************/
/* 写入器初始化 */
extern VOID DDS_DataWriter_Init(DDS_DataWriter* pstDataWriter);
/* 写入器析构 */
extern VOID DDS_DataWriter_Uninit(DDS_DataWriter* pstDataWriter);
/* 删除写入器 */
extern VOID DDS_DataWriter_delete(DDS_DataWriter* pstDataWriter);//LJM，接口放在这里不合理吧

/*********************************************************************************************/
/**************************************** 写入器报文操作 *************************************/
/* 组装心跳报文发送所有未指定阅读器 */
extern VOID JointHeartbeatMsgSendUnknownReader(DDS_DataWriter* pstDataWriter);
/* 组装心跳报文发送指定阅读器 */
extern VOID JointHeartbeatMsgSendAssignReader(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscdReader);
/* 组装心跳报文发送所有阅读器 */
extern VOID JointHeartbeatMsgSendAllReader(DDS_DataWriter* pstDataWriter);
/* Nephalem 组装分片心跳报文发送指定阅读器 */
extern VOID JointHeartbeatFragMsgSendAssignReader(HistoryData* pstHistoryData, DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscdReader);

/* 【组包发送历史数据】，在write接口中如果超过64KB时调用，以及participant的周期数据检查线程调用。
  实际上应该区分两种场景，第一种为64KB用满，第二种有可能没有数据 */
extern DDS_ReturnCode_t SendHistoryData(DDS_DataWriter* pstDataWriter);
/* 收到Ack时将缺失的数据组包发送 */
DDS_ReturnCode_t DataWriterSendCacheMsgTogether(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, SequenceNumber_t* seqNum);

/* 复原丢失分片，进行网络重传；在接收到ack报文时调用 */
extern VOID RecoverWriterMissSeqNum(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, AckNack* pstAckNackMsg);
/*读者丢失分片在发端已经丢弃，发送gap报文；在RecoverWriterMissSeqNum重传接口中调用*/
extern DDS_ReturnCode_t DataWriterSendGapMsg(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, SequenceNumber_t seqNum, SequenceNumber_t *nowNum);
extern VOID RecoverWriterMissDataFrag(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, NackFrag* pstNackFragMsg);

#if defined QYDDS_MASSDATA
/* Nephalem massdata 分片数据发送*/
DDS_ReturnCode_t DDS_StringDataWriter_writeFragData(DDS_DataWriter* pstDataWriter, const CHAR* buff, UINT32 length, HistoryData* pstMassDataInfo);
#endif 

/* 缓存数据重传 */
extern DDS_ReturnCode_t DataWriterSendCacheMsg(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, SequenceNumber_t seqNum);
/* 过期缓存清除 */
extern VOID DelDataWriterHistoryCache(DDS_DataWriter* pstDataWriter);

/***************************************************************/
/************************ 写入器检测线程 ***********************/
/*DeadLine数据发送线程*/
extern VOID DeadLineSendTask(DDS_DataWriter* pstDataWriter);
/*用户数据心跳发送线程*/
extern VOID UserDataHeartSendTask(DDS_DataWriter* pstDataWriter);


extern void OpenWriteLog();

extern void CloseWriteLog();

extern void Open_FC_SEND();

extern void Close_FC_SEND();

extern void Open_FC_SEND_U();

extern void Close_FC_SEND_U();

extern void Open_DDS_RECV();

extern void Close_DDS_RECV();

extern void Open_USER_RECV();

extern void Close_USER_RECV();


#endif
