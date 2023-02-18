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
*  @file     BuiltinDataReader.h
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

#ifndef HIPERD_BUILTINDATAREADER_H
#define HIPERD_BUILTINDATAREADER_H

struct _DomainParticipant;
/****************************************************************************************************************************************/
//�ṹ�嶨��
/****************************************************************************************************************************************/

/* �ڽ�Զ��д���� */
typedef struct _BuiltinDiscWriter
{
    GUID_t                        guid;
    AckNack                       stAckNack;   /* �����Ķ����ظ�Զ��д����Acknack���� */
    Heartbeat                     stHeartbeat; /* Զ��д�������ͱ����Ķ������� */
    Locator_t*                    pstUnicastLocator;
    //Locator_t*                    pstMulticastLocator;
    HistoryCache                  stHistoryCache; /* ��¼Զ�˶���������ģ����ڽ�д��������ʷ���ݣ�ֻ��¼seqNum��������ʵ�����ݣ�����Ѱ�Ҷ�ʧ��Ƭ */
    struct _BuiltinDiscWriter* pNext;

}BuiltinDiscWriter ;

/* �ڽ������Ķ��� */
typedef struct _BuiltinDataReader
{
    GUID_t                 guid; /* ��reader��Guid */
    SequenceNumber_t       seqNum; /* ��¼SequenceNumber_t,ÿ����һ���µ������Լ�1 */
    BuiltinDiscWriter*     pstBuiltinDataWriter; /* �ڽ�Զ��д���� */
    struct _DomainParticipant* pstParticipant;

}BuiltinDataReader;

/****************************************************************************************************************************************/
//��������
/****************************************************************************************************************************************/

/* �ڽ��Ķ�����ʼ�� */
extern VOID InitBuiltinDataReader(BuiltinDataReader* pstBuiltinDataReader);

/* �ڽ�Զ��д�������� */
extern VOID UnInitBuiltinDiscWriter(BuiltinDiscWriter* pstBuiltinDiscWriter);

/* �ڽ��Ķ������� */
extern VOID UnInitBuiltinDataReader(BuiltinDataReader* pstBuiltinDataReader);

/* ��Զ���ڽ�writer�ظ�ack���� */
extern VOID JointAckNackMsgSendAssignBuiltinWriter(BuiltinDataReader* pstBuiltinDataReader, BuiltinDiscWriter* pstBuiltinDiscWriter);

/* ��ȡ��ʧ�ڽ�Ƭ�� */
extern VOID GetBuiltinReaderMissSeqNum(BuiltinDataReader* pstBuiltinDataReader, BuiltinDiscWriter* pstBuiltinDiscWriter, Heartbeat* pstHeartbeatMsg);

/* �ڽ��Ķ���������Զ��д���� */
extern BOOL BuiltinDataReaderInsertDiscDataWriter(BuiltinDataReader* pstBuiltinDataReader, GUID_t* pstGuid, Locator_t* pstUnicastLocator);

/* ����Զ���ڽ�д���� */
extern BuiltinDiscWriter* GetBuiltinDiscWriterFromBuiltinDataReaderByPrefix(BuiltinDataReader* pstBuiltinDataReader, GuidPrefix_t* pstPrefix);

/* ɾ��Զ���ڽ�д���� */
extern VOID DeleteBuiltinDiscWriterFromBuiltinReaderByGuidPrefix(BuiltinDataReader* pstBuiltinDataReader, GuidPrefix_t*  pstPrefix);

#endif
