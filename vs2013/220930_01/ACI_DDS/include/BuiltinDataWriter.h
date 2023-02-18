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
*  @file     BuiltinDataWriter.h
*  @brief    
*  Details.
*
*  @author   Hunter
*  @email    1028251654@qq.com
*  @version  1.0
*  @date     2019/08/20
*  @license
*
*--------------------------------------------------------------------------------
*  Remark         : Description
*--------------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>  | <operation> | <Description>
*--------------------------------------------------------------------------------
*  2019/08/20 | 1.0       | Hunter    | Create file |
*--------------------------------------------------------------------------------
*
*********************************************************************************/

#ifndef HIPERD_BUILTINDATAWRITER_H
#define HIPERD_BUILTINDATAWRITER_H

struct _DomainParticipant;
/****************************************************************************************************************************************/
//结构体定义
/****************************************************************************************************************************************/

/* 内建远端阅读器 */
typedef struct _BuiltinDiscReader
{
    GUID_t       guid; 
    AckNack      stAckNack; /* 远端阅读器回复本地写入器Ack报文 */
    Locator_t*   pstUnicastLocator;
    UINT32       uiSendHeartBeatNum;    /* 最后回复ack截至后发送心跳个数，超过规定值，失去连接 */
	Duration_t	 livelinessDuration;
    struct _BuiltinDiscReader* pNext;

}BuiltinDiscReader;

/* 内建本地写入器 */
typedef struct _BuiltinDataWriter
{
    GUID_t                guid; /* 该内建写入器的的Guid */
    SequenceNumber_t    seqNum; /* 写入器编号 */
    Heartbeat           stHeartbeat; /* 本地写入器发送远端写入器心跳 */
    BuiltinDiscReader*  pstBuiltinDiscReader; /* 远端内建阅读器 */
    HistoryCache        stHistoryCache; /* 记录本地多个发布或订阅 */
    struct _DomainParticipant* pstParticipant; /* 父节点 */

}BuiltinDataWriter;

/************************************** 数据结构操作 *******************************************/
/* 内建写入器初始化 */
extern VOID InitBuiltinDataWriter(BuiltinDataWriter* pstBuiltinDataWriter);
/* 内建远端阅读器初始化 */
extern VOID UnInitBuiltinDiscReader(BuiltinDiscReader*  pstBuiltinDiscReader);
/* 内建写入器初始化 */
extern VOID UnInitBuiltinDataWriter(BuiltinDataWriter* pstBuiltinDataWriter);
/* 内建写入器初插入远端阅读器 */
extern BOOL BuiltinDataWriterInsertDiscDataReader(BuiltinDataWriter* pstBuiltinDataWriter, GUID_t* pstGuid, Locator_t* pstUnicastLocator);
/* 主题中删除指定远端内建阅读器 */
extern VOID DeleteBuiltinDiscReaderFromBuiltinWriterByGuidPrefix(BuiltinDataWriter* pstBuiltinDataWriter, GuidPrefix_t*  pstPrefix);
/* 查找远端内建阅读器 */
extern BuiltinDiscReader* GetBuiltinDiscReaderFromBuiltinDataWriterByPrefix(BuiltinDataWriter* pstBuiltinDataWriter, GuidPrefix_t* pstPrefix);

/*************************************** 报文操作 **********************************************/
/* 组装SEDP报文发送所有远端内建阅读器 */
extern VOID JointSEDPDataMsgSendALLBuiltinReader(RTPSMessageHeader* pstRTPSMsgHeader, HistoryData* pstHistoryData, BuiltinDataWriter* pstBuiltinDataWriter);
/* 组装SEDP报文发送给指定远端内建阅读器 */
extern VOID JointSEDPDataMsgSendBuiltinReader(RTPSMessageHeader* pstRTPSMsgHeader, HistoryData* pstHistoryData, BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader*  pstBuiltinDiscReader);
/* 组装心跳报文发送所有内建阅读器 */
extern VOID JointHeartbeatMsgSendAllBuiltinReader(RTPSMessageHeader* pstRTPSMsgHeader, BuiltinDataWriter* pstBuiltinDataWriter);
/* 组装心跳报文发送指定内建阅读器 */
extern VOID JointHeartbeatMsgSendAssignBuiltinReader(RTPSMessageHeader* pstRTPSMsgHeader, BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader* pstBuiltinDiscReader);
/* 复用组包，将历史数据打包一次性发送给新节点 */
DDS_ReturnCode_t BuiltinDataWriterSendCacheMsgTogether(BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader* pstBuiltinDiscReader, SequenceNumber_t* seqNum);

/* 复原内建丢失分片，进行网络重传，在处理Ack报文时调用 */
extern VOID RecoverBuiltinWriterMissSeqNum(BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader* pstBuiltinDiscReader, AckNack* pstAckNackMsg);
/* 内建缓存数据重传，在处理Ack报文时调用 */
extern DDS_ReturnCode_t BuiltinDataWriterSendCacheMsg(BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader* pstBuiltinDiscReader, SequenceNumber_t seqNum);
/****************************************** 其他接口，目前未见使用 ************************************************/
/* 组装SEDP报文发送指定远端内建阅读器 */ //重复的接口
extern VOID JointSEDPDataMsgSendAssignBuiltinReader(RTPSMessageHeader* pstRTPSMsgHeader, HistoryData* pstHistoryData, BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader*  pstBuiltinDiscReader);
/* 修改心跳机制 */
extern VOID ModifySendDiscReaderHeartbeat(BuiltinDataWriter* pstBuiltinDataWriter, BOOL bFlag);

#endif
