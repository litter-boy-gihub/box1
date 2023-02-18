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
    //BOOL                bVaild;           /* 是否有效 */
    GUID_t                guid;           /* 本Writer的Guid */
    SequenceNumber_t    seqNum;           /* 记录用户数据编号,每插入一个新的数据自加1 */
    Heartbeat           stHeartbeat;      /* 本地写入器发送远端写入器心跳 */
    CALLBACKTIME        writerCallBack;   /* 时间戳生成回调 */
    DDS_DataWriterQos   stDataWriterQos;  /* 写入器qos */
    HistoryCache        stHistoryCache;   /* 该writer的历史数据 */
	Duration_t			lastWriteTime;

#if defined(_WIN32)
    HANDLE hThread[2];      //收发任务线程
#elif defined(__linux__) || defined(_VXWORKS)
    pthread_t hThread[2];
#endif

    BOOL                destroyFlag;
    struct _Topic*      pstTopic;
    struct _DataWriter* pNext;

}DDS_DataWriter;

/****************************************************************************************************************************************/
//函数声明
/****************************************************************************************************************************************/

/******************************************/
/**************** 用户接口 ****************/
/* 用户数据写入 */
extern DDS_DLL DDS_ReturnCode_t DDS_StringDataWriter_write(DDS_DataWriter* pstDataWriter, const CHAR* buff, UINT32 lenght);


/* 写入器初始化 */
extern VOID DDS_DataWriter_Init(DDS_DataWriter* pstDataWriter);

/* 写入器析构 */
extern VOID DDS_DataWriter_Uninit(DDS_DataWriter* pstDataWriter);

/* 删除写入 */
extern VOID DDS_DataWriter_delete(DDS_DataWriter* pstDataWriter);

/* 组装心跳报文发送所有未指定阅读器 */
extern VOID JointHeartbeatMsgSendUnknownReader(DDS_DataWriter* pstDataWriter);

/* 组装心跳报文发送指定阅读器 */
extern VOID JointHeartbeatMsgSendAssignReader(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscdReader);

/* 组装心跳报文发送所有阅读器 */
extern VOID JointHeartbeatMsgSendAllReader(DDS_DataWriter* pstDataWriter);

/* Nephalem 组装分片心跳报文发送指定阅读器 */
extern VOID JointHeartbeatFragMsgSendAssignReader(HistoryData* pstHistoryData,DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscdReader);

/* 复原丢失分片，进行网络重传 */
extern VOID RecoverWriterMissSeqNum(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, AckNack* pstAckNackMsg);

extern VOID RecoverWriterMissDataFrag(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, NackFrag* pstNackFragMsg);



#if defined QYDDS_MASSDATA
/* Nephalem massdata 分片数据发送*/
DDS_ReturnCode_t DDS_StringDataWriter_writeFragData(DDS_DataWriter* pstDataWriter, const CHAR* buff, UINT32 length, HistoryData* pstMassDataInfo);
#endif 

/* 缓存数据重传 */
extern DDS_ReturnCode_t DataWriterSendCacheMsg(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, SequenceNumber_t seqNum);

/*读者丢失分片在发端已经丢弃，发送gap报文*/
extern DDS_ReturnCode_t DataWriterSendGapMsg(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, SequenceNumber_t seqNum, SequenceNumber_t *nowNum);

/* 过期缓存清除 */
extern VOID DelDataWriterHistoryCache(DDS_DataWriter* pstDataWriter);

/*DeadLine数据发送线程*/
extern VOID DeadLineSendTask(DDS_DataWriter* pstDataWriter);

/*用户数据心跳发送线程*/
extern VOID UserDataHeartSendTask(DDS_DataWriter* pstDataWriter);

#endif
