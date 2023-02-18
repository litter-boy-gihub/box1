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
//�ṹ�嶨��
/****************************************************************************************************************************************/

typedef void(STACALL *CALLBACKTIME)(Time_t* pstTimeInfo);

/* ����д���� */
typedef struct _DataWriter
{
    //BOOL                bVaild;           /* �Ƿ���Ч */
    GUID_t                guid;           /* ��Writer��Guid */
    SequenceNumber_t    seqNum;           /* ��¼�û����ݱ��,ÿ����һ���µ������Լ�1 */
    Heartbeat           stHeartbeat;      /* ����д��������Զ��д�������� */
    CALLBACKTIME        writerCallBack;   /* ʱ������ɻص� */
    DDS_DataWriterQos   stDataWriterQos;  /* д����qos */
    HistoryCache        stHistoryCache;   /* ��writer����ʷ���� */
	Duration_t			lastWriteTime;

#if defined(_WIN32)
    HANDLE hThread[2];      //�շ������߳�
#elif defined(__linux__) || defined(_VXWORKS)
    pthread_t hThread[2];
#endif

    BOOL                destroyFlag;
    struct _Topic*      pstTopic;
    struct _DataWriter* pNext;

}DDS_DataWriter;

/****************************************************************************************************************************************/
//��������
/****************************************************************************************************************************************/

/******************************************/
/**************** �û��ӿ� ****************/
/* �û�����д�� */
extern DDS_DLL DDS_ReturnCode_t DDS_StringDataWriter_write(DDS_DataWriter* pstDataWriter, const CHAR* buff, UINT32 lenght);


/* д������ʼ�� */
extern VOID DDS_DataWriter_Init(DDS_DataWriter* pstDataWriter);

/* д�������� */
extern VOID DDS_DataWriter_Uninit(DDS_DataWriter* pstDataWriter);

/* ɾ��д�� */
extern VOID DDS_DataWriter_delete(DDS_DataWriter* pstDataWriter);

/* ��װ�������ķ�������δָ���Ķ��� */
extern VOID JointHeartbeatMsgSendUnknownReader(DDS_DataWriter* pstDataWriter);

/* ��װ�������ķ���ָ���Ķ��� */
extern VOID JointHeartbeatMsgSendAssignReader(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscdReader);

/* ��װ�������ķ��������Ķ��� */
extern VOID JointHeartbeatMsgSendAllReader(DDS_DataWriter* pstDataWriter);

/* Nephalem ��װ��Ƭ�������ķ���ָ���Ķ��� */
extern VOID JointHeartbeatFragMsgSendAssignReader(HistoryData* pstHistoryData,DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscdReader);

/* ��ԭ��ʧ��Ƭ�����������ش� */
extern VOID RecoverWriterMissSeqNum(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, AckNack* pstAckNackMsg);

extern VOID RecoverWriterMissDataFrag(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, NackFrag* pstNackFragMsg);



#if defined QYDDS_MASSDATA
/* Nephalem massdata ��Ƭ���ݷ���*/
DDS_ReturnCode_t DDS_StringDataWriter_writeFragData(DDS_DataWriter* pstDataWriter, const CHAR* buff, UINT32 length, HistoryData* pstMassDataInfo);
#endif 

/* ���������ش� */
extern DDS_ReturnCode_t DataWriterSendCacheMsg(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, SequenceNumber_t seqNum);

/*���߶�ʧ��Ƭ�ڷ����Ѿ�����������gap����*/
extern DDS_ReturnCode_t DataWriterSendGapMsg(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, SequenceNumber_t seqNum, SequenceNumber_t *nowNum);

/* ���ڻ������ */
extern VOID DelDataWriterHistoryCache(DDS_DataWriter* pstDataWriter);

/*DeadLine���ݷ����߳�*/
extern VOID DeadLineSendTask(DDS_DataWriter* pstDataWriter);

/*�û��������������߳�*/
extern VOID UserDataHeartSendTask(DDS_DataWriter* pstDataWriter);

#endif
