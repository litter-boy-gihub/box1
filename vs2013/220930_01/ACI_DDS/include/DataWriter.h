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
	/************************ ������Ϣ ****************************/
	GUID_t              guid;				/* ��Writer��Guid */
	DDS_DataWriterQos   stDataWriterQos;	/* д����qos */
	HistoryCache        stHistoryCache;		/* ��writer����ʷ���� */
	CALLBACKTIME        writerCallBack;		/* ʱ������ɻص� */
	/*********************** ��Ϊ��Ϣ *****************************/
	BOOL                destroyFlag;
	//BOOL              bVaild;				/* �Ƿ���Ч */
	SequenceNumber_t    seqNum;				/* ��¼�û����ݱ��,ÿ����һ���µ������Լ�1 */
	Heartbeat           stHeartbeat;		/* ����д��������Զ��д�������� */
	Duration_t			lastWriteTime;
	unsigned int		uiHistoryDataSize;	/* ��ǰ��������б����ܳ��� */
#if defined(_WIN32)
    HANDLE hThread[2];      //�շ������߳�
#elif defined(__linux__) || defined(_VXWORKS)
    pthread_t hThread[2];
#endif

	struct _Topic*      pstTopic;			/* �����ĸ�����ڵ�*/
    struct _DataWriter* pNext;
}DDS_DataWriter;

/*********************************************************************************************/
/********************************** д�����û��ӿڶ��� ***************************************/
/* �û�����д�� */
extern DDS_DLL DDS_ReturnCode_t DDS_StringDataWriter_write(DDS_DataWriter* pstDataWriter, const CHAR* buff, UINT32 lenght);

/*********************************************************************************************/
/**************************************** д������Ϣ���� *************************************/
/* д������ʼ�� */
extern VOID DDS_DataWriter_Init(DDS_DataWriter* pstDataWriter);
/* д�������� */
extern VOID DDS_DataWriter_Uninit(DDS_DataWriter* pstDataWriter);
/* ɾ��д���� */
extern VOID DDS_DataWriter_delete(DDS_DataWriter* pstDataWriter);//LJM���ӿڷ������ﲻ�����

/*********************************************************************************************/
/**************************************** д�������Ĳ��� *************************************/
/* ��װ�������ķ�������δָ���Ķ��� */
extern VOID JointHeartbeatMsgSendUnknownReader(DDS_DataWriter* pstDataWriter);
/* ��װ�������ķ���ָ���Ķ��� */
extern VOID JointHeartbeatMsgSendAssignReader(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscdReader);
/* ��װ�������ķ��������Ķ��� */
extern VOID JointHeartbeatMsgSendAllReader(DDS_DataWriter* pstDataWriter);
/* Nephalem ��װ��Ƭ�������ķ���ָ���Ķ��� */
extern VOID JointHeartbeatFragMsgSendAssignReader(HistoryData* pstHistoryData, DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscdReader);

/* �����������ʷ���ݡ�����write�ӿ����������64KBʱ���ã��Լ�participant���������ݼ���̵߳��á�
  ʵ����Ӧ���������ֳ�������һ��Ϊ64KB�������ڶ����п���û������ */
extern DDS_ReturnCode_t SendHistoryData(DDS_DataWriter* pstDataWriter);
/* �յ�Ackʱ��ȱʧ������������� */
DDS_ReturnCode_t DataWriterSendCacheMsgTogether(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, SequenceNumber_t* seqNum);

/* ��ԭ��ʧ��Ƭ�����������ش����ڽ��յ�ack����ʱ���� */
extern VOID RecoverWriterMissSeqNum(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, AckNack* pstAckNackMsg);
/*���߶�ʧ��Ƭ�ڷ����Ѿ�����������gap���ģ���RecoverWriterMissSeqNum�ش��ӿ��е���*/
extern DDS_ReturnCode_t DataWriterSendGapMsg(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, SequenceNumber_t seqNum, SequenceNumber_t *nowNum);
extern VOID RecoverWriterMissDataFrag(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, NackFrag* pstNackFragMsg);

#if defined QYDDS_MASSDATA
/* Nephalem massdata ��Ƭ���ݷ���*/
DDS_ReturnCode_t DDS_StringDataWriter_writeFragData(DDS_DataWriter* pstDataWriter, const CHAR* buff, UINT32 length, HistoryData* pstMassDataInfo);
#endif 

/* ���������ش� */
extern DDS_ReturnCode_t DataWriterSendCacheMsg(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, SequenceNumber_t seqNum);
/* ���ڻ������ */
extern VOID DelDataWriterHistoryCache(DDS_DataWriter* pstDataWriter);

/***************************************************************/
/************************ д��������߳� ***********************/
/*DeadLine���ݷ����߳�*/
extern VOID DeadLineSendTask(DDS_DataWriter* pstDataWriter);
/*�û��������������߳�*/
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
