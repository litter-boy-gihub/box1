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
//�ṹ�嶨��
/****************************************************************************************************************************************/

/* �ڽ�Զ���Ķ��� */
typedef struct _BuiltinDiscReader
{
    GUID_t       guid; 
    AckNack      stAckNack; /* Զ���Ķ����ظ�����д����Ack���� */
    Locator_t*   pstUnicastLocator;
    UINT32       uiSendHeartBeatNum;    /* ���ظ�ack�����������������������涨ֵ��ʧȥ���� */
	Duration_t	 livelinessDuration;
    struct _BuiltinDiscReader* pNext;

}BuiltinDiscReader;

/* �ڽ�����д���� */
typedef struct _BuiltinDataWriter
{
    GUID_t                guid; /* ���ڽ�д�����ĵ�Guid */
    SequenceNumber_t    seqNum; /* д������� */
    Heartbeat           stHeartbeat; /* ����д��������Զ��д�������� */
    BuiltinDiscReader*  pstBuiltinDiscReader; /* Զ���ڽ��Ķ��� */
    HistoryCache        stHistoryCache; /* ��¼���ض���������� */
    struct _DomainParticipant* pstParticipant; /* ���ڵ� */

}BuiltinDataWriter;

/************************************** ���ݽṹ���� *******************************************/
/* �ڽ�д������ʼ�� */
extern VOID InitBuiltinDataWriter(BuiltinDataWriter* pstBuiltinDataWriter);
/* �ڽ�Զ���Ķ�����ʼ�� */
extern VOID UnInitBuiltinDiscReader(BuiltinDiscReader*  pstBuiltinDiscReader);
/* �ڽ�д������ʼ�� */
extern VOID UnInitBuiltinDataWriter(BuiltinDataWriter* pstBuiltinDataWriter);
/* �ڽ�д����������Զ���Ķ��� */
extern BOOL BuiltinDataWriterInsertDiscDataReader(BuiltinDataWriter* pstBuiltinDataWriter, GUID_t* pstGuid, Locator_t* pstUnicastLocator);
/* ������ɾ��ָ��Զ���ڽ��Ķ��� */
extern VOID DeleteBuiltinDiscReaderFromBuiltinWriterByGuidPrefix(BuiltinDataWriter* pstBuiltinDataWriter, GuidPrefix_t*  pstPrefix);
/* ����Զ���ڽ��Ķ��� */
extern BuiltinDiscReader* GetBuiltinDiscReaderFromBuiltinDataWriterByPrefix(BuiltinDataWriter* pstBuiltinDataWriter, GuidPrefix_t* pstPrefix);

/*************************************** ���Ĳ��� **********************************************/
/* ��װSEDP���ķ�������Զ���ڽ��Ķ��� */
extern VOID JointSEDPDataMsgSendALLBuiltinReader(RTPSMessageHeader* pstRTPSMsgHeader, HistoryData* pstHistoryData, BuiltinDataWriter* pstBuiltinDataWriter);
/* ��װSEDP���ķ��͸�ָ��Զ���ڽ��Ķ��� */
extern VOID JointSEDPDataMsgSendBuiltinReader(RTPSMessageHeader* pstRTPSMsgHeader, HistoryData* pstHistoryData, BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader*  pstBuiltinDiscReader);
/* ��װ�������ķ��������ڽ��Ķ��� */
extern VOID JointHeartbeatMsgSendAllBuiltinReader(RTPSMessageHeader* pstRTPSMsgHeader, BuiltinDataWriter* pstBuiltinDataWriter);
/* ��װ�������ķ���ָ���ڽ��Ķ��� */
extern VOID JointHeartbeatMsgSendAssignBuiltinReader(RTPSMessageHeader* pstRTPSMsgHeader, BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader* pstBuiltinDiscReader);
/* �������������ʷ���ݴ��һ���Է��͸��½ڵ� */
DDS_ReturnCode_t BuiltinDataWriterSendCacheMsgTogether(BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader* pstBuiltinDiscReader, SequenceNumber_t* seqNum);

/* ��ԭ�ڽ���ʧ��Ƭ�����������ش����ڴ���Ack����ʱ���� */
extern VOID RecoverBuiltinWriterMissSeqNum(BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader* pstBuiltinDiscReader, AckNack* pstAckNackMsg);
/* �ڽ����������ش����ڴ���Ack����ʱ���� */
extern DDS_ReturnCode_t BuiltinDataWriterSendCacheMsg(BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader* pstBuiltinDiscReader, SequenceNumber_t seqNum);
/****************************************** �����ӿڣ�Ŀǰδ��ʹ�� ************************************************/
/* ��װSEDP���ķ���ָ��Զ���ڽ��Ķ��� */ //�ظ��Ľӿ�
extern VOID JointSEDPDataMsgSendAssignBuiltinReader(RTPSMessageHeader* pstRTPSMsgHeader, HistoryData* pstHistoryData, BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader*  pstBuiltinDiscReader);
/* �޸��������� */
extern VOID ModifySendDiscReaderHeartbeat(BuiltinDataWriter* pstBuiltinDataWriter, BOOL bFlag);

#endif
