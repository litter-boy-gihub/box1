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
*  @file     DataReader.h
*  @brief    DataReader
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

#ifndef HIPERD_DATAREADER_H
#define HIPERD_DATAREADER_H

struct _Topic;
struct _DataReader;

typedef void(STACALL *CALLBACKFUN)(struct _DataReader* pstDataReader);

typedef struct _DataSeq
{
    UINT32 length;
    HistoryData* pstHead;

}DataSeq;

typedef UINT32 DataLen;

typedef struct _InfoSeq
{
    UINT32 length;
}InfoSeq;

typedef struct _DataReader
{
    GUID_t              guid; /* ��reader��Guid */
    CALLBACKFUN         recvCallBack; /* ��Ӧ��recvListener,����֪ͨ����  */
    DDS_DataReaderQos   stDataReaderQos; /* д����qos*/
    struct _Topic*      pstTopic; /* ���ڵ����� */
    UINT64              params[4]; /* �û��Զ������� */
    Time_t				lastRecvTime;		//����TIME_BASED_FLITER�жϽ���ʱ����
    UINT64              totHistoryNum;

    /* Nephalem OwnerShip_Strength ����Ȩǿ����ߵ�Զ��д���� */
    DDS_DiscoveredWriter*  pstOwnerDiscWriter;

    struct _DataReader* pNext;

}DDS_DataReader;


/* д������ʼ�� */
extern VOID DDS_DataReader_Init(DDS_DataReader* pstDataReader);

/* д�������� */
extern VOID DDS_DataReader_Uninit(DDS_DataReader* pstDataReader);

/* ɾ���Ķ��� */
extern VOID DDS_DataReader_delete(DDS_DataReader* pstDataReader);

/* ��Զ��writer�ظ�ack���� */
extern VOID JointAckNackMsgSendAssignWriter(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter);

/* ��Զ��writer�ظ�NackFrag���� */
extern VOID JointNackFragMsgSendAssignWriter(SequenceNumber_t writerSN, DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter);

/* ��ȡ��ʧƬ�� */
extern VOID GetDataReaderMissSeqNum(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter, SequenceNumber_t lastSN);

/* ����Զ��д�����������ύ */
extern VOID CorrectDiscoveredWriterHistoryCache(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter);

/*�ﵽreader��History.depth���޺��ύ��ɱ���������*/
extern VOID CorrectOldHistoryCache(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter);

/*����Զ��д����Gap����*/
extern VOID CorrectDiscoveredWriterGapMsg(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter,Gap* pstGapMsg);

/* �ύ��״̬�Ķ������� */
extern VOID DeilverStatuslessUserMsg(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter, HistoryData* pstHistoryData);

/* �ύ��״̬�Ķ������� */
/* HistoryData ��ֵ��ѭ�� ������г�ʼ�����ڸú�����ʹ�ã�ֵ�ᷢ�ͱ仯���ڶ��ν������ֱ��� */
extern VOID DeilverStatusFulUserMsg(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter, HistoryData* pstHistoryData);

/* �ύ��״̬�Ķ������ݣ�������seqNum������firstSN��С */
extern VOID DeilverStatusFulUserMsgWhenExpectSeqNumUnderFirstSN(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter, SequenceNumber_t firstSN);

/* ��ȡ�û����� */
extern DDS_DLL DDS_ReturnCode_t DDS_StringDataReader_take(DDS_DataReader* pstDataReader, DataSeq* pstDataSeq);

/* ��ȡ�û����� */
extern DDS_DLL DataSeq*  DDS_StringDataReader_take_cs(DDS_DataReader* pstDataReader);

/* ��ȡ�û���Ϣ���� */
extern DDS_DLL UINT32 DDS_StringSeq_length(DataSeq* pstDataSeq);

/* �����û����� */
extern DDS_DLL CHAR* DDS_StringSeq_get(DataSeq* pstDataSeq, UINT32 num, DataLen* pDataLen);

/* �����û�����ʱ��� */
extern DDS_DLL BOOL DDS_TimeSeq_get(DataSeq* pstDataSeq, UINT32 num, Time_t* pstTimeInfo);

/* �����û����ݳ��� */
extern DDS_DLL UINT32 DDS_StringSeq_len_cs(DataSeq* pstDataSeq, UINT32 num);

/* �����û����� */
extern DDS_DLL VOID DDS_StringSeq_get_cs(DataSeq* pstDataSeq, UINT32 num,  CHAR* msg);

/* �û�������Դ�ͷ�, ��ȡ�û����ݺ��������ͷţ�������� */
extern DDS_DLL DDS_ReturnCode_t DDS_StringDataReader_return_loan(DDS_DataReader* pstDataReader, DataSeq* pstDataSeq);

#if defined QYDDS_MASSDATA
/* Nephalem MassData��Ƭ���ݴ��� */
void ProcessMassData(HistoryData* pstTempHistoryData, DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter);
#endif

#endif
