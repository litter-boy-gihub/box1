#include "../include/GlobalDefine.h"

#ifdef _FC
#define DataFragSize (64*1024)
#else
#define DataFragSize (63*1024)
#endif

int iWriteLog = 0;

int iSPDPFCSEND = 0;

int iUserFCSEND = 0;

int iDDSRECV = 0;

int iUSERRECV = 0;

void OpenWriteLog()
{
	iWriteLog = 1;
}

void CloseWriteLog()
{
	iWriteLog = 0;
}

 void Open_FC_SEND()
 {
	 iSPDPFCSEND = 1;
 }

 void Close_FC_SEND()
 {
	 iSPDPFCSEND = 0;
 }

 void Open_FC_SEND_U()
 {
	 iUserFCSEND = 1;
 }


 void Close_FC_SEND_U()
 {
	 iUserFCSEND = 0;
 }

  void Open_DDS_RECV()
  {
	  iDDSRECV = 1;
  }

  void Close_DDS_RECV()
  {
	  iDDSRECV = 0;
  }

  void Open_USER_RECV()
  {
	  iUSERRECV = 1;
  }

  void Close_USER_RECV()
  {
	  iUSERRECV = 0;
  }

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_DataWriter_Init
-   ��������: ��ʼ��д����
-   ��    ��: д����ָ��
-   ��    ��: 
-   ȫ�ֱ���:
-   ע    ��: ��ʼ��д����������Ϣ
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DDS_DataWriter_Init(DDS_DataWriter* pstDataWriter)
{
    if (NULL != pstDataWriter)
    {
        pstDataWriter->seqNum = SEQUENCENUMBER_ZERO;
        pstDataWriter->stHeartbeat.count = 0;
        InitHistoryCache(&pstDataWriter->stHistoryCache);
		pstDataWriter->lastWriteTime.sec = 0;
        pstDataWriter->lastWriteTime.nanosec = 0;
        pstDataWriter->destroyFlag = FALSE;
        pstDataWriter->writerCallBack = NULL;

		pstDataWriter->uiHistoryDataSize = 0;
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_DataWriter_Uninit
-   ��������: ����д����
-   ��    ��: д����ָ��
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ����д����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DDS_DataWriter_Uninit(DDS_DataWriter* pstDataWriter)
{
    if (NULL != pstDataWriter)
    {
        pstDataWriter->seqNum = SEQUENCENUMBER_ZERO;
        pstDataWriter->stHeartbeat.count = 0;
        pstDataWriter->destroyFlag = TRUE;
        UninitHistoryCache(&pstDataWriter->stHistoryCache);
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_DataWriter_delete
-   ��������: д����ɾ��
-   ��    ��: д����ָ��
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: д����ɾ��
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DDS_DataWriter_delete(DDS_DataWriter* pstDataWriter)
{
    DDS_DataWriter* pstPreDataWriter = NULL;
    DDS_Topic* pstTopic = pstDataWriter->pstTopic;

    DDS_DataWriter* pstCurDataWriter = pstTopic->pstLocalWriter;
    while (NULL != pstCurDataWriter)
    {
        if (pstCurDataWriter == pstDataWriter)
        {
            if (NULL == pstPreDataWriter)
            {
                pstTopic->pstLocalWriter = pstCurDataWriter->pNext;
                DDS_DataWriter_Uninit(pstCurDataWriter);
                pstCurDataWriter = pstTopic->pstLocalWriter;
            }
            else
            {
                pstPreDataWriter->pNext = pstCurDataWriter->pNext;
                DDS_DataWriter_Uninit(pstCurDataWriter);
                pstCurDataWriter = pstPreDataWriter->pNext;
            }
            break;
        }
        else
        {
            pstPreDataWriter = pstCurDataWriter;
            pstCurDataWriter = pstCurDataWriter->pNext;
        }
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: JointHeartbeatMsgSendUnknownReader
-   ��������: ��װ�������ķ���δָ���Ķ���
-   ��    ��: ����DomainParticipantָ��
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��װ�������ķ���ָ���Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\15       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID JointHeartbeatMsgSendUnknownReader(DDS_DataWriter* pstDataWriter)
{
    DDS_DiscoveredReader*  pstDiscReader;
    MemoryBlock stMemBlock;
    MemoryBlock stMemBlockMod;
    SubMessageHeader  stSubMsgHeader;
    InfoTimestamp     stInfoTS;
    CHAR buff[NETWORK_BYTE_MAX_SIZE];

    DDS_DomainParticipant* pstParticipant = pstDataWriter->pstTopic->pstParticipant;

    /* �����޻��棬���������� */
    if (NULL == pstDataWriter->stHistoryCache.pstHead)
    {
        return;
    }

    /* �����ֽ����ʼ�������淢������ */
    INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

    /* ��һ�����л�RTPS����ͷ */
    SERIALIZE_RTPS_MESSAGE_HEADER(pstParticipant->stRTPSMsgHeader, stMemBlock);

    /* ʱ�䱨��ͷ���� */
    stSubMsgHeader.submessageId = INFO_TS;
    stSubMsgHeader.submessageLength = sizeof(Time_t);
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
    stSubMsgHeader.flags[2] = FALSE;    //dataFlag
    stSubMsgHeader.flags[3] = FALSE;   //keyFlag
    stInfoTS.value = GetNowTime();

    /* ���л�ʱ�䱨��ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���л�ʱ���ӱ����� */
    SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

    /* ��������ͷ */
    stSubMsgHeader.submessageId = HEARTBEAT;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;

    /* ����������Ϣ�� */
    pstDataWriter->stHeartbeat.readerId = ENTITYID_UNKNOWN;
    pstDataWriter->stHeartbeat.writerId = pstDataWriter->guid.entityId;
    pstDataWriter->stHeartbeat.firstSN = pstDataWriter->stHistoryCache.pstHead->seqNum;
    pstDataWriter->stHeartbeat.lastSN = pstDataWriter->stHistoryCache.pstTail->seqNum;
    pstDataWriter->stHeartbeat.count++;

    /* ���л��û������ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���л��û������ӱ�����Ϣ�壬ͬʱ̽���¼readerIdλ�� */
    SERIALIZE_HEARTBEAT(pstDataWriter->stHeartbeat, stMemBlock);

    /* �����ӱ�����Ϣ�峤�ȣ�������Ϣͷ */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - pstDataWriter->stHeartbeat.uiStartIndex;

    /* ���л��޸��ӱ��ĳ��� */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

    pstDiscReader = pstDataWriter->pstTopic->pstDiscReader;
    while (NULL != pstDiscReader)
    {
        /* Nephalem parition ����ͬһ���������� */
        if (FALSE == pstDiscReader->bSamePartiton)
        {
            pstDiscReader = pstDiscReader->pNext;
            continue;
        }

        /* ֻ����״̬������ */
        if (RELIABLE_RELIABILITY_QOS == pstDiscReader->stDataReaderQos.reliability.kind)
        {
            /* ���л��޸����Ķ���id */
            INITIAL_MEMORYBLOCK(stMemBlockMod, pstDataWriter->stHeartbeat.pcReadId, sizeof(EntityId_t));

            SERIALIZE_ENTITYID(pstDiscReader->guid.entityId, stMemBlockMod);

            if (!pstParticipant->bLoopBack && GuidPrefix_Is_Equal(&pstParticipant->stRTPSMsgHeader.guidPrefix, &pstDiscReader->guid.prefix))
            {
                pstDiscReader = pstDiscReader->pNext;
                continue;
            }

            /* ���͵���Ӧ��ÿ��Զ���Ķ��� */
            RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscReader->pstUnicastLocator);
        }

        pstDiscReader = pstDiscReader->pNext;
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: JointHeartbeatMsgSendAssignReader
-   ��������: ��װ�������ķ���ָ���Ķ���
-   ��    ��: ����DomainParticipantָ��
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��װ�������ķ���ָ���Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\15       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID JointHeartbeatMsgSendAssignReader(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscdReader)
{
    MemoryBlock stMemBlock;
    MemoryBlock stMemBlockMod;
    SubMessageHeader  stSubMsgHeader;
    InfoTimestamp     stInfoTS;
    CHAR buff[NETWORK_BYTE_MAX_SIZE];

    DDS_DomainParticipant* pstParticipant = pstDataWriter->pstTopic->pstParticipant;

    /* �����޻��棬���������� */
    if (NULL == pstDataWriter->stHistoryCache.pstHead)
    {
        return;
    }

	/* Nephalem partition ����ͬһ���������� */
	if (FALSE == pstDiscdReader->bSamePartiton)
		return ;

    /* �����ֽ����ʼ�������淢������ */
    INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

    /* ��һ�����л�RTPS����ͷ */
    SERIALIZE_RTPS_MESSAGE_HEADER(pstParticipant->stRTPSMsgHeader, stMemBlock);

    /* ʱ�䱨��ͷ���� */
    stSubMsgHeader.submessageId = INFO_TS;
    stSubMsgHeader.submessageLength = sizeof(Time_t);
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
    stSubMsgHeader.flags[2] = FALSE;   //dataFlag
    stSubMsgHeader.flags[3] = FALSE;   //keyFlag
    stInfoTS.value = GetNowTime();

    /* ���л�ʱ�䱨��ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���л�ʱ���ӱ����� */
    SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

    /* ��������ͷ */
    stSubMsgHeader.submessageId = HEARTBEAT;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;

    /* ����������Ϣ�� */
    pstDataWriter->stHeartbeat.readerId = pstDiscdReader->guid.entityId;
    pstDataWriter->stHeartbeat.writerId = pstDataWriter->guid.entityId;
    pstDataWriter->stHeartbeat.firstSN = pstDataWriter->stHistoryCache.pstHead->seqNum;
    pstDataWriter->stHeartbeat.lastSN = pstDataWriter->stHistoryCache.pstTail->seqNum;
    pstDataWriter->stHeartbeat.count++;

    /* ���л��û������ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���л��û������ӱ�����Ϣ�壬ͬʱ̽���¼readerIdλ�� */
    SERIALIZE_HEARTBEAT(pstDataWriter->stHeartbeat, stMemBlock);

    /* �����ӱ�����Ϣ�峤�ȣ�������Ϣͷ */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - pstDataWriter->stHeartbeat.uiStartIndex;

    /* ���л��޸��ӱ��ĳ��� */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

    if (!pstParticipant->bLoopBack && GuidPrefix_Is_Equal(&pstParticipant->stRTPSMsgHeader.guidPrefix, &pstDiscdReader->guid.prefix))
    {
        return;
    }

    /* ���͵���Ӧ��Զ���Ķ��� */
    RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscdReader->pstUnicastLocator);
}

/*---------------------------------------------------------------------------------
-   �� �� ��: JointHeartbeatMsgSendAllReader
-   ��������: ��װ�������ķ��������Ķ���
-   ��    ��: ����DomainParticipantָ��
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��װ�������ķ��������Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\15       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID JointHeartbeatMsgSendAllReader(DDS_DataWriter* pstDataWriter)
{
    DDS_DiscoveredReader* pstDiscReader = pstDataWriter->pstTopic->pstDiscReader;

    while (NULL != pstDiscReader)
    {
        JointHeartbeatMsgSendAssignReader(pstDataWriter, pstDiscReader);
        pstDiscReader = pstDiscReader->pNext;
    }
}


/*---------------------------------------------------------------------------------
-   �� �� ��: JointHeartbeatFragMsgSendAssignReader
-   ��������: ��װ��Ƭ�������ķ���ָ���Ķ���
-   ��    ��: 
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��װ��Ƭ�������ķ���ָ���Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2021\4\12       1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
VOID JointHeartbeatFragMsgSendAssignReader(HistoryData* pstHistoryData,DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscdReader)
{
    MemoryBlock stMemBlock;
    MemoryBlock stMemBlockMod;
    SubMessageHeader  stSubMsgHeader;
    InfoTimestamp     stInfoTS;
    CHAR buff[NETWORK_BYTE_MAX_SIZE];
    HeartbeatFrag  stHeartbeatFrag;

    DDS_DomainParticipant* pstParticipant = pstDataWriter->pstTopic->pstParticipant;

    ///* �����޻��棬���������� */
    //if (NULL == pstDataWriter->stHistoryCache.pstHead)
    //{
    //    return;
    //}

    /* Nephalem partition ����ͬһ���������� */
    if (FALSE == pstDiscdReader->bSamePartiton)
        return;

    /* �����ֽ����ʼ�������淢������ */
    INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

    /* ��һ�����л�RTPS����ͷ */
    SERIALIZE_RTPS_MESSAGE_HEADER(pstParticipant->stRTPSMsgHeader, stMemBlock);

    /* ʱ�䱨��ͷ���� */
    stSubMsgHeader.submessageId = INFO_TS;
    stSubMsgHeader.submessageLength = sizeof(Time_t);
    stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
    stSubMsgHeader.flags[2] = FALSE;   //dataFlag
    stSubMsgHeader.flags[3] = FALSE;   //keyFlag
    stInfoTS.value = GetNowTime();

    /* ���л�ʱ�䱨��ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���л�ʱ���ӱ����� */
    SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

    /* ��������ͷ */
    stSubMsgHeader.submessageId = HEARTBEAT_FRAG;
    stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;

     
    /* ��Ƭ����������Ϣ�� */
    stHeartbeatFrag.readerId = pstDiscdReader->guid.entityId;
    stHeartbeatFrag.writerId = pstDataWriter->guid.entityId;
    stHeartbeatFrag.writerSN = pstHistoryData->seqNum;
    stHeartbeatFrag.lastFragmentNum = pstHistoryData->uiFragTotal;
    stHeartbeatFrag.count = ++pstDataWriter->stHeartbeat.count;


    /* ���л��û������ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���л��û������ӱ�����Ϣ�壬ͬʱ̽���¼readerIdλ�� */
    SERIALIZE_HEARTBEATFRAG(stHeartbeatFrag, stMemBlock);

    /* �����ӱ�����Ϣ�峤�ȣ�������Ϣͷ */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stHeartbeatFrag.uiStartIndex;

    /* ���л��޸��ӱ��ĳ��� */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

    if (!pstParticipant->bLoopBack && GuidPrefix_Is_Equal(&pstParticipant->stRTPSMsgHeader.guidPrefix, &pstDiscdReader->guid.prefix))
    {
        return;
    }

    /* ���͵���Ӧ��Զ���Ķ��� */
    RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscdReader->pstUnicastLocator);
}


/*---------------------------------------------------------------------------------
-   �� �� ��: RecoverMissSeqNum
-   ��������: ��ԭ��ʧ��Ƭ�����������ش�
-   ��    ��: ����д������Զ���Ķ�����ack����
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��ԭ��ʧ��Ƭ�����������ش�
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\14       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID RecoverWriterMissSeqNum(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, AckNack* pstAckNackMsg)
{
    SequenceNumber_t seqNumMiss = SEQUENCENUMBER_ZERO;
    SequenceNumber_t seqNumOffset = SEQUENCENUMBER_ZERO;
    DDS_ReturnCode_t returnCode = DDS_RETCODE_OK;
    const UINT32  bitStart = 0x80000000;
    UINT32 uiNumbit = 0;
	SequenceNumber_t nowNum = SEQUENCENUMBER_ZERO;

#if defined TG 
	SequenceNumber_t missSeqVec[257];
	memset(missSeqVec, 0, sizeof(missSeqVec));
	UINT32 missSeqVecPos = 0;
#endif

    for (uiNumbit = 0; uiNumbit < pstAckNackMsg->readerSNState.numBits; uiNumbit++)
    {
        /* λ�������bitMap��ÿһλֵ������0˵����λ��Ч��ͨ��ƫ���������ȱʧ */
        if (0 < ((bitStart >> (uiNumbit >> 5))&(pstAckNackMsg->readerSNState.bitMap[uiNumbit >> 5])))
        {
            seqNumOffset.low = uiNumbit;
            seqNumMiss = Add_SequenceNumber(pstAckNackMsg->readerSNState.base, seqNumOffset);

#if defined TG 
			missSeqVec[missSeqVecPos++] = seqNumMiss;
#endif

#ifndef TG 
            /* ȱʧ��Ƭ�ط� */
            returnCode = DataWriterSendCacheMsg(pstDataWriter, pstDiscReader, seqNumMiss);
			if (nowNum.low != 0 && seqNumMiss.low <= nowNum.low)
			{
				continue;
			}
            if (DDS_RETCODE_OK != returnCode)
            {
				//printf("resend DataR and Seq = %d \n", seqNumMiss.low);
                //DataWriterSendGapMsg(pstDataWriter, pstDiscReader, seqNumMiss, &nowNum);
//                 printf("%d gap send 339\n",seqNumMiss.low);
                //todo����gap����
            }
#endif 
        }
    }
    //printf("sleep1\n");
    //QY_Sleep(1);
#if defined TG 
	if (0 != missSeqVecPos)
		DataWriterSendCacheMsgTogether(pstDataWriter, pstDiscReader, missSeqVec);
#endif

}


/*---------------------------------------------------------------------------------
-   �� �� ��: RecoverWriterMissDataFrag
-   ��������: �ش�ָ��DataFrag����
-   ��    ��: ����д������Զ���Ķ�����Nack����
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �ش�ָ��DataFrag����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2021\4\14       1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
VOID RecoverWriterMissDataFrag(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, NackFrag* pstNackFragMsg)
{
    DDS_ReturnCode_t returnCode = DDS_RETCODE_OK;
    const UINT32  bitStart = 0x80000000;
    UINT32 uiNumbit = 0;
    HistoryData* pstHistoryData = NULL;
	NackFragReader* pstNackFragReader = NULL;

    /* Nephalem ����ָ��writerSN������ */
    pstHistoryData = pstDataWriter->stHistoryCache.pstHead;
    while (NULL != pstHistoryData)
    {
        if (ISEQUAL_SEQUENCENUMBER(pstNackFragMsg->writerSN, pstHistoryData->seqNum))
            break;
        pstHistoryData = pstHistoryData->pNext;
    }

    /* Nephalem writerSN�����ݲ����� ֱ�ӷ��� */
    if (NULL == pstHistoryData)
        return;
	/* JD 2021-11-29 ����ᵼ�´�����ݵ�Heartbeat ��AckNack�ı��ĵ��쳣�����յ�HEARTBEAT_FRAG�ظ�NACK_FRAG��
	����֮����յ��ı��ĳ�������,�������������ݶ��߻ظ�Nack_Frag���ĺ�������д�߶Ͽ����ӣ�֮��ᵼ������д��һֱ����������
	��������д�߲����ڷ���Nack_Frag���ģ��������ݶ��߻ᷢ��AcknackҪ�������Data���ģ�DWʼ���޷����·������Data���ģ�������ѭ�� */
    pstHistoryData->bUseNackFrag = TRUE;
    
    /* Nephalem �б�Ϊ��ʱ��ֱ�Ӳ��� */
    if (NULL == pstHistoryData->pstNackReader)
    {
        pstHistoryData->pstNackReader = (NackFragReader*)DDS_STATIC_MALLOC_T(sizeof(NackFragReader));
        memcpy(&pstHistoryData->pstNackReader->stReaderID, &pstDiscReader->guid, sizeof(GUID_t));
        pstHistoryData->pstNackReader->pNext = NULL;
    }
    else
    {
        /* Nephalem ���Ҹ�reader�Ƿ���ӵ�ʹ��NackFrag�ش��б��� */
        NackFragReader* pstTemp = pstHistoryData->pstNackReader;
        while (NULL != pstTemp)
        {
            if (TRUE == Guid_Is_Equal(&pstTemp->stReaderID, &pstDiscReader->guid))
                break;
            pstTemp = pstTemp->pNext;
        }

        if (NULL == pstTemp)
        {
            pstTemp = pstHistoryData->pstNackReader;
            while (NULL != pstTemp->pNext)
                pstTemp = pstTemp->pNext;

            pstTemp->pNext = (NackFragReader*)DDS_STATIC_MALLOC_T(sizeof(NackFragReader));
            memcpy(&pstTemp->pNext->stReaderID, &pstDiscReader->guid, sizeof(GUID_t));
            pstTemp->pNext->pNext = NULL;
        }
    }



    pstNackFragReader = (NackFragReader*)DDS_STATIC_MALLOC_T(sizeof(NackFragReader));
    if (NULL == pstNackFragReader)
        return;
    

    for (uiNumbit = 0; uiNumbit < pstNackFragMsg->fragmentNumberState.numBits; uiNumbit++)
    {
        /* λ�������bitMap��ÿһλֵ������0˵����λ��Ч��ͨ��ƫ���������ȱʧ */
        if (0 < ((bitStart >> (uiNumbit % 32))&(pstNackFragMsg->fragmentNumberState.bitMap[uiNumbit >> 5])))
        {
            /* ȱʧ��Ƭ�ط� */
            JointUSERDataFragMsgSendAssignReader(pstDataWriter, pstDiscReader, pstHistoryData, pstNackFragMsg->fragmentNumberState.base + uiNumbit);
        }
    }
#if !defined(NO_HEARTBEAT)
	if(pstNackFragMsg->fragmentNumberState.numBits != 0)
		JointHeartbeatFragMsgSendAssignReader(pstHistoryData, pstDataWriter, pstDiscReader);
#endif
}
/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_StringDataWriter_write
-   ��������: �û����ݷ��ͽӿ�
-   ��    ��: д�������û����ݡ����ݳ���
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �û����ݷ��ͽӿ�
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\12       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t DDS_StringDataWriter_write(DDS_DataWriter* pstDataWriter, const CHAR* buff, UINT32 length)
{
	DDS_ReturnCode_t retCode;
	BOOL StoreFlag = FALSE;
	BOOL FreeOldFlag = FALSE;
	HistoryData* pstHistoryData = NULL;
	HistoryData* tmpHead = NULL;
	int iTopicLen = 0;
	int i = 1;
    if (length <= 0)
    {
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to send message, length error.\n");
        return DDS_RETCODE_ERROR;
    }
    if (NULL == pstDataWriter)
    {
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to send message, dataWriter is not exist.\n");
        return DDS_RETCODE_ERROR;
    }
    
    if(iWriteLog == 1)
    {
		printf("DDS_StringDataWriter_write ");
		printf("Topic %s and len = %d \n",pstDataWriter->pstTopic->topicName.value,length);
    }
    
#ifdef QYDDS_MASSDATA
    /* Nephalem ��������ݽ��з�Ƭ����η��� */
    if (length > DataFragSize && RELIABLE_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
    {
        DDS_ReturnCode_t retCode;

        INT32 iNum = length / DataFragSize;
		/*TODO������ǰ����֧��VS2010�ľɱ�����*/
        HistoryData massDataInfo;
        int i = 0;
		if ((KEEP_LAST_HISTORY_QOS == pstDataWriter->stDataWriterQos.history.kind) && (pstDataWriter->stDataWriterQos.history.depth < iNum + 1))
            printf("Nephalem Warnning : history.depth is small \n");
        if (LENGTH_UNLIMITED != pstDataWriter->stDataWriterQos.resource_limits.max_samples)
        {
            if ((KEEP_ALL_HISTORY_QOS == pstDataWriter->stDataWriterQos.history.kind) && (pstDataWriter->stDataWriterQos.resource_limits.max_samples < iNum + 1))
                printf("Nephalem Warnning : resource_limits.max_samples is small \n");
        }

        /* Nephalem massdata ��������Ϣ����������*/
        /* Nephalem massdata�Ļ�����Ե��ڵ�ǰ��Ƭ���ݵ������ ��ͬ�����Ϊͳһϵ�з�Ƭ����*/
        massDataInfo.massDataBaseSeq = Add_SequenceNumber(pstDataWriter->seqNum, SEQUENCENUMBER_START);
        /* Nephalem massdata��Ƭ������ */
        massDataInfo.massDataTotal = iNum + 1;
        /* Nephalem massdata�����ܴ�С */
        massDataInfo.massDataTotolSize = length;
               
        for (i = 0; i < iNum; i++)
        {
            retCode = DDS_StringDataWriter_writeFragData(pstDataWriter, buff + DataFragSize * i, DataFragSize, &massDataInfo);
            if (DDS_RETCODE_OK != retCode)
                return retCode;       
        }

        retCode = DDS_StringDataWriter_writeFragData(pstDataWriter, buff + DataFragSize * iNum, length % DataFragSize, &massDataInfo);
        return retCode;
    }
#endif;
	/* ���뾲̬�ڴ洴��HistoryData����ڵ� */
	pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC_T(sizeof(HistoryData));
	if (NULL == pstHistoryData)
	{
		PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData, Insufficient memory space.\n");
		return DDS_RETCODE_ERROR;
	}
	InitHistoryData(pstHistoryData);

    /* Nephalem �����Ƿ��Ƭ������Ҫ�����ݱ��浽HistoryData�У��������ط�ʱʹ��*/
	{
		int topicLen = 0;
		int msgLen = 0;
#ifdef _TOPIC_INFO
		topicLen = 4 + pstDataWriter->pstTopic->topicName.length;
		topicLen += (topicLen & 3) ? (4 - (topicLen & 3)) : 0;
#endif
#ifdef QYDDS_RTI_STRINGHEADER
		msgLen = 4;
#endif
		/* ���뾲̬�ڴ洴����ʷ�������� */
		pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC_T(topicLen + msgLen + length);
		if (NULL == pstHistoryData->data)
		{
			/* ʧ���ͷ�ǰ��������ڴ� */
			DDS_STATIC_FREE(pstHistoryData);
	        pstHistoryData = NULL;
			PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData buff, Insufficient memory space.\n");
			return DDS_RETCODE_ERROR;
		}
		memset(pstHistoryData->data, 0, topicLen + msgLen + length);
#ifdef _TOPIC_INFO
		memcpy(pstHistoryData->data, &pstDataWriter->pstTopic->topicName.length, 4);
		memcpy(pstHistoryData->data + 4, pstDataWriter->pstTopic->topicName.value, pstDataWriter->pstTopic->topicName.length);
#endif
#ifdef QYDDS_RTI_STRINGHEADER
		memcpy(pstHistoryData->data + topicLen, &length, 4);
#endif
		/* ���ݿ����������� */
		memcpy(pstHistoryData->data + topicLen + msgLen, buff, length);
		length += topicLen + msgLen;
	}

    /* ���û������ݳ��� */
    pstHistoryData->uiDataLen = length;

    /* ÿд��һ������д������ǰseqNum�Լ�1 */
    pstDataWriter->seqNum = Add_SequenceNumber(pstDataWriter->seqNum, SEQUENCENUMBER_START);

    /* �������ݼ�¼��ǰseqNum����Ա������ش� */
    pstHistoryData->seqNum = pstDataWriter->seqNum;

    /* �������ʱ��� */
    if (pstDataWriter->writerCallBack)
    {
        /* �û��Զ���ʱ��ص� */
		/* Nephalem �ûص������ᱻdeadline�������˴������лص� */
        //pstDataWriter->writerCallBack(&pstHistoryData->timeInfo);
    }
    else
    {
        /* Ĭ��ʱ��ص� */
        pstHistoryData->timeInfo = GetNowTime();
    }
   
    if (pstHistoryData->uiDataLen > DataFragSize)
    {
        pstHistoryData->uiFragTotal = pstHistoryData->uiDataLen / DataFragSize + 1;

        /* Nephalem uiDataLen > DataFragSize ʱ����Ƭ�����ض�����0 */
        //if (pstHistoryData->uiDataLen % DataFragSize != 0)
        //{
        //    pstHistoryData->uiFragTotal += 1;
        //}

        pstHistoryData->pstFragData = (FragData*)DDS_STATIC_MALLOC_T(sizeof(FragData));
        if (NULL == pstHistoryData->pstFragData)
        {
            DDS_STATIC_FREE(pstHistoryData->data);
            pstHistoryData->data = NULL;
			DDS_STATIC_FREE(pstHistoryData);
            pstHistoryData->data = NULL;
            PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryDataFrag, Insufficient memory space.\n");
            return DDS_RETCODE_ERROR;
        }
        
        for (i = 1; i <= pstHistoryData->uiFragTotal; i++)
        {
            int pstLength = 0;
            if (i == pstHistoryData->uiFragTotal)
            {
                pstLength = pstHistoryData->uiDataLen % DataFragSize;
            }
            if (pstLength == 0)
            {
                pstLength = DataFragSize;
            }
			pstHistoryData->pstFragData->data = (CHAR*)DDS_STATIC_MALLOC_T(pstLength);
            if (NULL == pstHistoryData->pstFragData->data)
            {
                /* ʧ���ͷ�ǰ��������ڴ� */
                DDS_STATIC_FREE(pstHistoryData->data);
                pstHistoryData->data = NULL;
				DDS_STATIC_FREE(pstHistoryData);
                pstHistoryData = NULL; 
                PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData->pstFragData, Insufficient memory space.\n");
                return DDS_RETCODE_ERROR;
            }

            pstHistoryData->uiFragLen = DataFragSize;
            pstHistoryData->uiFragNum = i;
            pstHistoryData->pstFragData->usFragIndex = i;
            pstHistoryData->pstFragData->usDataLen = pstLength;
            /* ���ݿ����������� */
            memcpy(pstHistoryData->pstFragData->data, buff + (i - 1)*DataFragSize, pstLength);
            /* ��װ�������ķ��͸����ĵ������Ķ��� */
            DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);

            retCode = JointUSERDataMsgSendUnknownReader(pstDataWriter, pstHistoryData);

            DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
            DDS_STATIC_FREE(pstHistoryData->pstFragData->data);
            pstHistoryData->pstFragData->data = NULL;
            //QY_Sleep(5);

        }
    }
    else
    {
    	if(1 == iWriteLog)
    	{
    		printf("Nephalem : JointUSERDataMsgSendUnknownReader Start .. ");
    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
    	}
#if defined TG
		retCode = DDS_RETCODE_OK; //ֻ�з���ֵΪok���Żᱣ�浽cache
		/* Nephalem 28��DATA����ͷ�ĳ��� */
		if (pstDataWriter->uiHistoryDataSize + pstHistoryData->uiDataLen + 28 > 63 * 1024)
		{
			DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
			retCode = SendHistoryData(pstDataWriter);
			DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);		
		}

		//���ܵ�ǰ���ݰ���û�з��ͣ�����Ҫ����ʷ���ݷ���֮�󣬽���ǰ���ݲ��뵽��ʷ������
		pstDataWriter->uiHistoryDataSize += pstHistoryData->uiDataLen + 28;
		/* Nephalem ���������� */
		DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
		InsertHistoryCacheTail(&pstDataWriter->stHistoryCache, pstHistoryData);
		DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
		return retCode;//ֱ�ӷ��أ���Ϊ�·��ж��Ƿ񱣴���߼��е����
#else
        retCode = JointUSERDataMsgSendUnknownReader(pstDataWriter, pstHistoryData);
        DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
#endif
    }

    if(iWriteLog == 1)
    {
        if(DDS_RETCODE_OK != retCode)
        	printf("Topic %s write data err ret = %d \n",pstDataWriter->pstTopic->topicName.value,retCode);
    }
    
    
	if(1 == iWriteLog)
	{
		printf("Nephalem : JointUSERDataMsgSendUnknownReader End .. ");
		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
	}

    if (length > DataFragSize)
    {
        DDS_STATIC_FREE(pstHistoryData->pstFragData);
        pstHistoryData->pstFragData = NULL;
    }

    StoreFlag = FALSE;

    /* Nephalem �ɿ��������ж��Ƿ���Ҫ�������� */
    if (RELIABLE_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
    {
        /* Nephalem ���ͳɹ�����������*/
        if (retCode == DDS_RETCODE_OK)
        {
            StoreFlag = TRUE;
        }
        else
        {
            if (pstDataWriter->stDataWriterQos.durability.kind == VOLATILE_DURABILITY_QOS)
            {
            }
            else if (pstDataWriter->stDataWriterQos.durability.kind == TRANSIENT_LOCAL_DURABILITY_QOS)
            {
            }
            else if (pstDataWriter->stDataWriterQos.durability.kind == TRANSIENT_DURABILITY_QOS)
            {
                StoreFlag = TRUE;
            }
            else
            {
                //todo �����ݴ��浽�ļ�ϵͳ�У�Ӳ���ϣ�
            }
        }
    }
    /* Nephalem �ǿɿ��������ж��Ƿ���Ҫ�������� */
    else if (BEST_EFFORT_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
    {
        if (pstDataWriter->stDataWriterQos.durability.kind == VOLATILE_DURABILITY_QOS)
        {
        }
        else if (pstDataWriter->stDataWriterQos.durability.kind == TRANSIENT_LOCAL_DURABILITY_QOS)
        {
        }
        else if (pstDataWriter->stDataWriterQos.durability.kind == TRANSIENT_DURABILITY_QOS)
        {
            StoreFlag = TRUE;
        }
        else
        {
            //todo �����ݴ��浽�ļ�ϵͳ�У�Ӳ���ϣ�
        }
    }

	if(1 == iWriteLog)
	{
		printf("Nephalem : StoreFlag = %d ",StoreFlag);
		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
	}
	
    if (StoreFlag)
    {
        /* Nephalem �ж��Ƿ���Ҫ�ͷ���ɻ��� */
        FreeOldFlag = FALSE;

        /* Nephalem resource_limits  KEEP_LAST */
        if (KEEP_LAST_HISTORY_QOS == pstDataWriter->stDataWriterQos.history.kind)
        {
            /* ������ݲ��뵽���ػ����� */
            /* Nephalem resource_limits ����history���ޣ�������ɻ��� */
            if (pstDataWriter->stHistoryCache.uiHistoryDataNum >= pstDataWriter->stDataWriterQos.history.depth)
            {
                /* Nephalem resource_limits ֱ��ɾ�������� */
                    FreeOldFlag = TRUE;
            }
        }
        /* Nephalem resource_limits  KEEP_ALL */
        else if (KEEP_ALL_HISTORY_QOS == pstDataWriter->stDataWriterQos.history.kind)
        {
            /* ������ݲ��뵽���ػ����� */
            /* Nephalem resource_limits ����resource_limits���ޣ�������ɻ��� */
            if (LENGTH_UNLIMITED != pstDataWriter->stDataWriterQos.resource_limits.max_samples
                && pstDataWriter->stHistoryCache.uiHistoryDataNum >= pstDataWriter->stDataWriterQos.resource_limits.max_samples)
            {
                /* Nephalem resource_limits ��best_effort��ֱ��ɾ�������� */
                if (BEST_EFFORT_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
                    FreeOldFlag = TRUE;
                else if (RELIABLE_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
                {   /* Nephalem resource_limits ��reliable�¶����ȴ�stHistoryCache�ڳ��ռ� */
                    while (pstDataWriter->stHistoryCache.uiHistoryDataNum >= pstDataWriter->stDataWriterQos.resource_limits.max_samples)
                    {
                        if (pstDataWriter->stDataWriterQos.durability.kind == TRANSIENT_DURABILITY_QOS)
                            return DDS_RETCODE_OUT_OF_RESOURCES;
                        QY_Sleep(100);
                    }
                }
            }
        }
        /* Nephalem ɾ�������� */
        if (FreeOldFlag)
        {
            DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
            tmpHead = pstDataWriter->stHistoryCache.pstHead;
            pstDataWriter->stHistoryCache.pstHead = pstDataWriter->stHistoryCache.pstHead->pNext;
            DDS_STATIC_FREE(tmpHead->data);
            tmpHead->data = NULL;
            DDS_STATIC_FREE(tmpHead);
            tmpHead = NULL;
            pstDataWriter->stHistoryCache.uiHistoryDataNum--;
            DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
        }

        /* Nephalem ���������� */
        DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
        InsertHistoryCacheTail(&pstDataWriter->stHistoryCache, pstHistoryData);
        DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
    }
    else
    {
        /* Nephalem �����Ƿ��Ƭ����Ҫ�ͷ�data�ڴ� */
		DDS_STATIC_FREE(pstHistoryData->data);
        pstHistoryData->data = NULL;
        DDS_STATIC_FREE(pstHistoryData);
        pstHistoryData = NULL;
    }

    if (retCode == DDS_RETCODE_OK && (pstDataWriter->stDataWriterQos.deadline.period.sec != 0 || pstDataWriter->stDataWriterQos.deadline.period.nanosec != 0))
	{
		pstDataWriter->lastWriteTime = GetNowTime();
	}
    return retCode;
}


#if defined QYDDS_MASSDATA
/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_StringDataWriter_writeFragData
-   ��������: ���ͷ�Ƭ����
-   ��    ��: д�������û����ݡ����ݳ��ȣ���Ƭ��Ϣ����Ƭ���ݻ���ţ���Ƭ�����������ܴ�С��
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ���ͷ�Ƭ����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2021\01\06       1.0        Nephalem      Created
----------------------------------------------------------------------------------*/
DDS_ReturnCode_t DDS_StringDataWriter_writeFragData(DDS_DataWriter* pstDataWriter, const CHAR* buff, UINT32 length, HistoryData* pstMassDataInfo)
{
	DDS_ReturnCode_t retCode;
	HistoryData* pstHistoryData = NULL;
	BOOL FreeOldFlag = FALSE;
	BOOL StoreFlag = FALSE;
	HistoryData* tmpHead = NULL;

    if (length <= 0)
    {
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to send message, length error.\n");
        return DDS_RETCODE_ERROR;
    }
    if (NULL == pstDataWriter)
    {
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to send message, dataWriter is not exist.\n");
        return DDS_RETCODE_ERROR;
    }

    /* ���뾲̬�ڴ洴��HistoryData����ڵ� */
    pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC_T(sizeof(HistoryData));
    if (NULL == pstHistoryData)
    {
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData, Insufficient memory space.\n");
        return DDS_RETCODE_ERROR;
    }
    InitHistoryData(pstHistoryData);

    /* Nephalem massData�����Ϣ��ֵ*/
    pstHistoryData->massDataBaseSeq = pstMassDataInfo->massDataBaseSeq;
    pstHistoryData->massDataTotal = pstMassDataInfo->massDataTotal;
    pstHistoryData->massDataTotolSize = pstMassDataInfo->massDataTotolSize;

    pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC_T(length);

    if (NULL == pstHistoryData->data)
    {
        /* ʧ���ͷ�ǰ��������ڴ� */
        DDS_STATIC_FREE(pstHistoryData);
        pstHistoryData = NULL;
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData buff, Insufficient memory space.\n");
        return DDS_RETCODE_ERROR;
    }

    /* ���ݿ����������� */
    memcpy(pstHistoryData->data, buff, length);

    /* ���û������ݳ��� */
    pstHistoryData->uiDataLen = length;

    /* ÿд��һ������д������ǰseqNum�Լ�1 */
    pstDataWriter->seqNum = Add_SequenceNumber(pstDataWriter->seqNum, SEQUENCENUMBER_START);

    /* �������ݼ�¼��ǰseqNum����Ա������ش� */
    pstHistoryData->seqNum = pstDataWriter->seqNum;

    /* �������ʱ��� */
    if (pstDataWriter->writerCallBack)
    {
        /* �û��Զ���ʱ��ص� */
        pstDataWriter->writerCallBack(&pstHistoryData->timeInfo);
    }
    else
    {
        /* Ĭ��ʱ��ص� */
        pstHistoryData->timeInfo = GetNowTime();
    }
    

    /* ��װ�������ķ��͸����ĵ������Ķ��� */
    DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
    retCode = JointUSERDataMsgSendUnknownReader(pstDataWriter, pstHistoryData);
    DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);

    StoreFlag = FALSE;

    /* Nephalem �ɿ��������ж��Ƿ���Ҫ�������� */

    /* Nephalem ���ͳɹ�����������*/
    if (retCode == DDS_RETCODE_OK)
    {
        StoreFlag = TRUE;
    }
    else
    {
        if (pstDataWriter->stDataWriterQos.durability.kind == VOLATILE_DURABILITY_QOS)
        {
        }
        else if (pstDataWriter->stDataWriterQos.durability.kind == TRANSIENT_LOCAL_DURABILITY_QOS)
        {
        }
        else if (pstDataWriter->stDataWriterQos.durability.kind == TRANSIENT_DURABILITY_QOS)
        {
            StoreFlag = TRUE;
        }
        else
        {
            //todo �����ݴ��浽�ļ�ϵͳ�У�Ӳ���ϣ�
        }
    }

    if (StoreFlag)
    {
        /* Nephalem �ж��Ƿ���Ҫ�ͷ���ɻ��� */
        FreeOldFlag = FALSE;

        /* Nephalem resource_limits  KEEP_LAST */
        if (KEEP_LAST_HISTORY_QOS == pstDataWriter->stDataWriterQos.history.kind)
        {
            /* ������ݲ��뵽���ػ����� */
            /* Nephalem resource_limits ����history���ޣ�������ɻ��� */
            if (pstDataWriter->stHistoryCache.uiHistoryDataNum >= pstDataWriter->stDataWriterQos.history.depth)
            {
                /* Nephalem resource_limits ��best_effort��ֱ��ɾ�������� */
                if (BEST_EFFORT_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
                    FreeOldFlag = TRUE;
            }
        }
        /* Nephalem resource_limits  KEEP_ALL */
        else if (KEEP_ALL_HISTORY_QOS == pstDataWriter->stDataWriterQos.history.kind)
        {
            /* ������ݲ��뵽���ػ����� */
            /* Nephalem resource_limits ����resource_limits���ޣ�������ɻ��� */
            if (LENGTH_UNLIMITED != pstDataWriter->stDataWriterQos.resource_limits.max_samples
                && pstDataWriter->stHistoryCache.uiHistoryDataNum >= pstDataWriter->stDataWriterQos.resource_limits.max_samples)
            {
                /* Nephalem resource_limits ��best_effort��ֱ��ɾ�������� */
                if (BEST_EFFORT_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
                    FreeOldFlag = TRUE;
                else if (RELIABLE_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
                {   /* Nephalem resource_limits ��reliable�¶����ȴ�stHistoryCache�ڳ��ռ� */
                    while (pstDataWriter->stHistoryCache.uiHistoryDataNum >= pstDataWriter->stDataWriterQos.history.depth)
                    {
                        if (pstDataWriter->stDataWriterQos.durability.kind == TRANSIENT_DURABILITY_QOS)
                            return DDS_RETCODE_OUT_OF_RESOURCES;

                        QY_Sleep(100);
                    }
                }
            }
        }
        /* Nephalem ɾ�������� */
        if (FreeOldFlag)
        {
            DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
            tmpHead = pstDataWriter->stHistoryCache.pstHead;
            pstDataWriter->stHistoryCache.pstHead = pstDataWriter->stHistoryCache.pstHead->pNext;
            DDS_STATIC_FREE(tmpHead->data);
            tmpHead->data = NULL;
            DDS_STATIC_FREE(tmpHead);
            tmpHead = NULL;
            pstDataWriter->stHistoryCache.uiHistoryDataNum--;
            DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
        }

        /* Nephalem ���������� */
        DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
        InsertHistoryCacheTail(&pstDataWriter->stHistoryCache, pstHistoryData);
        DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
    }
    else
    {
        /* Nephalem �ͷ�data�ڴ� */
        DDS_STATIC_FREE(pstHistoryData->data);
        pstHistoryData->data = NULL;
        DDS_STATIC_FREE(pstHistoryData);
        pstHistoryData = NULL;
    }
#ifdef QYDDS_DEADLINE_QOS
    if (retCode == DDS_RETCODE_OK && pstDataWriter->stDataWriterQos.deadline.period.sec != 0 || pstDataWriter->stDataWriterQos.deadline.period.nanosec != 0)
	{
		pstDataWriter->lastWriteTime = GetNowTime();
	}
#endif
    return retCode;
}

#endif 
/*---------------------------------------------------------------------------------
-   �� �� ��: DataWriterSendCacheMsg
-   ��������: ���������ش�
-   ��    ��: д������Զ���Ķ�����ȱʧseqNum
-   ��    ��: DDS_RETCODE_OK��DDS_RETCODE_ERROR
-   ȫ�ֱ���:
-   ע    ��: ���������ش�
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\16       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_ReturnCode_t DataWriterSendCacheMsg(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, SequenceNumber_t seqNum)
{
    SequenceNumber_t   seqNumOffset;
    UINT32             uiOffset;
	HistoryData* pstHistoryData = NULL;

    /* �����޻������ݻ���ack�����޶�ʧ��ֱ�ӷ��ز����� */
    if (NULL == pstDataWriter->stHistoryCache.pstHead)
    {
        //printf("1\n");
        return DDS_RETCODE_ERROR;
    }

    /* ȱʧ���Ĳ��ڻ������䣬ֱ�ӷ��ز����� */
    if (ISLESSTHEN_SEQUENCENUMBER(pstDataWriter->stHistoryCache.pstTail->seqNum, seqNum) || ISLESSTHEN_SEQUENCENUMBER(seqNum, pstDataWriter->stHistoryCache.pstHead->seqNum))
    {
        //printf("2\n");
		//printf("cache missed, seqNum: %d, headNum: %d, tailNum: %d.\n", seqNum.low, pstDataWriter->stHistoryCache.pstHead->seqNum.low, pstDataWriter->stHistoryCache.pstTail->seqNum.low);
        return DDS_RETCODE_ERROR;
    }

    /* ȱʧ���汨��ƫ���� */
    seqNumOffset = Sub_SequenceNumber(seqNum, pstDataWriter->stHistoryCache.pstHead->seqNum);

    pstHistoryData = pstDataWriter->stHistoryCache.pstHead;

    /* ͨ��ƫ��������ȱʧ���ģ���Ϊ�����������ģ�����һ�����Ƚϣ�ֱ���ҵ���Ӧλ�� */
    for (uiOffset = 0; uiOffset < seqNumOffset.low; uiOffset++)
    {
        if (NULL != pstHistoryData)
        {
            pstHistoryData = pstHistoryData->pNext;
        }
        else
        {
            printf("DataWriterSendCacheMsg: There is a problem with the history cache linked list.\n");
            return DDS_RETCODE_ERROR;
        }
    }

    if (pstHistoryData == NULL)
    {
        return DDS_RETCODE_OK;
    }

    /* ƫ�������ҵ��Ļ��汨�ĶԲ��ϣ��������߼����� */
    if (!ISEQUAL_SEQUENCENUMBER(pstHistoryData->seqNum, seqNum))
    {
        printf("DataWriterSendCacheMsg: There is a problem with the history cache linked list, inconsistency of data, expect: %d, actually: %d.\n",seqNum.low, pstHistoryData->seqNum.low);
        return DDS_RETCODE_ERROR;
    }

    /* Nephalem ͨ��NackFrag���ж�ʧ��Ƭ���ش����������������ݰ� */
    if (TRUE == pstHistoryData->bUseNackFrag)
    {
        /* Nephalem ���Ҹ�reader�Ƿ���ӵ�ʹ��NackFrag�ش��б��� */
        NackFragReader* pstTemp = pstHistoryData->pstNackReader;
        while (NULL != pstTemp)
        {
            if (TRUE == Guid_Is_Equal(&pstTemp->stReaderID, &pstDiscReader->guid))
                break;
            pstTemp = pstTemp->pNext;
        }
        if (NULL != pstTemp)
            return DDS_RETCODE_OK;
    }

    /* ��ʼ���ͱ��� */
    JointUSERDataMsgSendAssignReader(pstDataWriter, pstDiscReader, pstHistoryData);

    return DDS_RETCODE_OK;
}

DDS_ReturnCode_t DataWriterSendCacheMsgTogether(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, SequenceNumber_t* seqNum)
{
	MemoryBlock       stMemBlock;
	MemoryBlock       stMemBlockMod;
	SubMessageHeader  stSubMsgHeader;
	ParameterHead     stParamHead;
	InfoTimestamp     stInfoTS;
	InfoDestination   stInfoDest;
	Data              stData;
	CHAR              buff[NETWORK_BYTE_MAX_SIZE];
	UINT32 Flags = 0;
	UINT32 uiSeqNum = 0;
	HistoryData* pstHistoryData = NULL;
	UINT32 uiOffset = 0;


	while (1)
	{
		/* Nephalem seqNumΪ0 û��������Ҫ�ش� */
		if (0 == seqNum[uiSeqNum].high && 0 == seqNum[uiSeqNum].low)
			return DDS_RETCODE_OK;

		/* �����ֽ����ʼ�������淢������ */
		INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

		/* ��һ�����л�RTPS����ͷ */
		SERIALIZE_RTPS_MESSAGE_HEADER(pstDataWriter->pstTopic->pstParticipant->stRTPSMsgHeader, stMemBlock);

		/* ʱ�䱨��ͷ���� */
		stSubMsgHeader.submessageId = INFO_TS;
		stSubMsgHeader.submessageLength = sizeof(Time_t);
		if (0 == IS_MACHINE_LITTLE_ENDIAN)
			stSubMsgHeader.flags[0] = FALSE;    //littleEndian
		else
			stSubMsgHeader.flags[0] = TRUE;    //littleEndian
		stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
		stSubMsgHeader.flags[2] = FALSE;   //dataFlag
		stSubMsgHeader.flags[3] = FALSE;   //keyFlag
		stInfoTS.value = GetNowTime();

		/* ���л�ʱ�䱨��ͷ */
		SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

		/* ���л�ʱ���ӱ����� */
		SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

		stSubMsgHeader.submessageId = INFO_DST;
		stSubMsgHeader.submessageLength = 12;
		if (0 == IS_MACHINE_LITTLE_ENDIAN)
			stSubMsgHeader.flags[0] = FALSE;    //littleEndian
		else
			stSubMsgHeader.flags[0] = TRUE;    //littleEndian
		stSubMsgHeader.flags[1] = FALSE;
		stSubMsgHeader.flags[2] = FALSE;
		stSubMsgHeader.flags[3] = FALSE;
		stInfoDest.guidPrefix = pstDiscReader->guid.prefix;

		/* �ڶ������л��ӱ���ͷ */
		SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

		/* ���������л��ӱ�����Ϣ�� */
		SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

		while (uiSeqNum < 256)
		{
			if (0 == seqNum[uiSeqNum].high && 0 == seqNum[uiSeqNum].low)
				break;

			pstHistoryData = pstDataWriter->stHistoryCache.pstHead;
			while (NULL != pstHistoryData)
			{
				if (ISEQUAL_SEQUENCENUMBER(pstHistoryData->seqNum, seqNum[uiSeqNum]))
					break;

				pstHistoryData = pstHistoryData->pNext;
			}

			if (NULL == pstHistoryData)
			{
				uiSeqNum++;
				continue;
			}

			/* Nephalem ͨ��NackFrag���ж�ʧ��Ƭ���ش����������������ݰ� */
			if (TRUE == pstHistoryData->bUseNackFrag)
			{
				/* Nephalem ���Ҹ�reader�Ƿ���ӵ�ʹ��NackFrag�ش��б��� */
				NackFragReader* pstTemp = pstHistoryData->pstNackReader;
				while (NULL != pstTemp)
				{
					if (TRUE == Guid_Is_Equal(&pstTemp->stReaderID, &pstDiscReader->guid))
						break;
					pstTemp = pstTemp->pNext;
				}
				if (NULL != pstTemp)
				{
					uiSeqNum++;
					continue;
				}
			}

			/* data����ͷ���� */
			stSubMsgHeader.submessageId = DATA;

			/* Nephlaem ����Data���ĳ��� */
			stSubMsgHeader.submessageLength =
				sizeof(stData.extraFlags) +
				sizeof(stData.octetsToInlineQos) +
				sizeof(stData.readerId) +
				sizeof(stData.writerId) +
				sizeof(stData.writerSN);

			stSubMsgHeader.submessageLength += sizeof(unsigned short) * 2 + pstHistoryData->uiDataLen;

			uiOffset = ((stSubMsgHeader.submessageLength & 1) == 0) ? 0 :
				SHORT_SIZE - (stSubMsgHeader.submessageLength & 1);

			stSubMsgHeader.submessageLength += uiOffset;

			//�ռ䲻����װ���� 
			if (stMemBlock.writeIndex + stSubMsgHeader.submessageLength + 4 >= NETWORK_BYTE_MAX_SIZE)
				break;

			if (0 == IS_MACHINE_LITTLE_ENDIAN)
				stSubMsgHeader.flags[0] = FALSE;    //littleEndian
			else
				stSubMsgHeader.flags[0] = TRUE;    //littleEndian
			stSubMsgHeader.flags[1] = FALSE;    //inlineQosFlag
			stSubMsgHeader.flags[2] = (pstHistoryData->data) ? TRUE : FALSE;  //dataPresent
			stSubMsgHeader.flags[3] = FALSE;   //keyFlag

			/* data���������� */
			stData.extraFlags[0] = 0x00;
			stData.extraFlags[1] = 0x00;
			stData.octetsToInlineQos = 0x0010;
			stData.readerId = pstDiscReader->guid.entityId;
			stData.writerId = pstDataWriter->guid.entityId;
			stData.writerSN = pstHistoryData->seqNum;
			if (IS_MACHINE_LITTLE_ENDIAN)
				stData.serializedPayload.m_cdrType = CDR_LE;   //littleEndian
			else
				stData.serializedPayload.m_cdrType = CDR_BE;    //littleEndian
			stData.serializedPayload.m_option = 0x0000;

			/* �ڶ������л��ӱ���ͷ */
			SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

			/* ���������л��ӱ�����Ϣ�� */
			SERIALIZE_DATA(stData, stMemBlock);

			/* ���л�����Ч�غ� */
			SERIALIZE_SERIALIZEDDATA(stData.serializedPayload, stMemBlock);

			/* ���л�д������Ϣ */
			PUT_BYTE_ARRAY(stMemBlock, pstHistoryData->data, pstHistoryData->uiDataLen);

			stMemBlock.writeIndex += uiOffset;
			stMemBlock.wrPtr += uiOffset;
			

			uiSeqNum++;

		}

		RTPSSendMsg(&stMemBlock, &pstDataWriter->pstTopic->pstParticipant->socketList.sedp_data_sender, pstDiscReader->pstUnicastLocator);

	}

	return DDS_RETCODE_OK;
}

extern DDS_ReturnCode_t DataWriterSendGapMsg(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, SequenceNumber_t seqNum, SequenceNumber_t *nowNum)
{
    MemoryBlock           stMemBlock;
//    MemoryBlock           stMemBlockMod;
    SubMessageHeader      stSubMsgHeader;
    InfoTimestamp         stInfoTS;
//    ParameterHead         stParamHead;
    InfoDestination       stInfoDest;
    Gap                   stGapMsg;
    CHAR                  cUserBuffer[1024];
	int i = 0;

    DDS_DomainParticipant* pstParticipant = pstDataWriter->pstTopic->pstParticipant;

    /* �����ֽ����ʼ�������淢������ */
    INITIAL_MEMORYBLOCK(stMemBlock, cUserBuffer, 1024);

    /* ��һ�����л�RTPS����ͷ */
    SERIALIZE_RTPS_MESSAGE_HEADER(pstParticipant->stRTPSMsgHeader, stMemBlock);

    /* ʱ�䱨��ͷ���� */
    stSubMsgHeader.submessageId = INFO_TS;
    stSubMsgHeader.submessageLength = sizeof(Time_t);
    stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
    stSubMsgHeader.flags[2] = FALSE;   //dataFlag
    stSubMsgHeader.flags[3] = FALSE;   //keyFlag
    stInfoTS.value = GetNowTime();

    /* ���л�ʱ�䱨��ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���л�ʱ���ӱ����� */
    SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

    /* InfoDT���� */
    stSubMsgHeader.submessageId = INFO_DST;
    stSubMsgHeader.submessageLength = 12;
    stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;
    stSubMsgHeader.flags[3] = FALSE;
    stInfoDest.guidPrefix = pstDiscReader->guid.prefix;

    /* �ڶ������л��ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���������л��ӱ�����Ϣ�� */
    SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

    stSubMsgHeader.submessageId = GAP;
    stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
    stSubMsgHeader.flags[2] = FALSE;
    stSubMsgHeader.flags[3] = FALSE;

    /* �û����ݱ��������� */
	stGapMsg.subMsgHeader = stSubMsgHeader;
    /*memset(stGapMsg.gapList.bitMap, 0, SEQUENCE_BIT_NUMBER);*/
	for (i = 0; i < 8; i++)
	{
		stGapMsg.gapList.bitMap[i] = 0;
	}
    stGapMsg.gapStart = seqNum;
    stGapMsg.readerId = pstDiscReader->guid.entityId;
    stGapMsg.writerId = pstDataWriter->guid.entityId;
    stGapMsg.gapList.base = seqNum;
    stGapMsg.gapList.numBits = pstDataWriter->stHistoryCache.pstHead->seqNum.low - seqNum.low;
	nowNum->low = pstDataWriter->stHistoryCache.pstHead->seqNum.low;
    /* ���л�Gap����ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);
    /* ���л�Gap������ */
    SERIALIZE_GAP(stGapMsg, stMemBlock);

    RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscReader->pstUnicastLocator);
    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DelDataWriterHistoryCache
-   ��������: ���ڻ������
-   ��    ��: ����
-   ��    ��: 
-   ȫ�ֱ���:
-   ע    ��: ���ڻ������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\16       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DelDataWriterHistoryCache(DDS_DataWriter* pstDataWriter)
{
	HistoryData* psHistoryData = NULL;
    HistoryData* psHistoryDataDel = NULL;
    DDS_DiscoveredReader*  pstDiscReader = NULL;
    DDS_LocalWriter* pstLocalWriter = NULL;
	SequenceNumber_t seqNumMin = SEQUENCENUMBER_ZERO;
	DDS_DomainParticipant* pstParticipant = NULL;

	if (pstDataWriter == NULL)
	{
		return;
	}

    pstParticipant = pstDataWriter->pstTopic->pstParticipant;

    if (NULL != pstDataWriter)
    {
        if (RELIABLE_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
        {
            if (NULL == pstDataWriter->stHistoryCache.pstTail)
            {
                return;
            }
            /* ������С����seqNum���ȱ���д������󻺴��1����Զ���Ķ�������seqNum�����ڵ���������򻺴�ȫ����� */
            seqNumMin = Add_SequenceNumber(pstDataWriter->stHistoryCache.pstTail->seqNum, SEQUENCENUMBER_START);

            /* ��ȡÿ��������Զ���Ķ�����С������seqNum */
            pstDiscReader = pstDataWriter->pstTopic->pstDiscReader;
            while (NULL != pstDiscReader)
            {
				if (FALSE == pstDiscReader->bSamePartiton)
				{
					pstDiscReader = pstDiscReader->pNext;
					continue;
				}

                if (!pstParticipant->bLoopBack && GuidPrefix_Is_Equal(&pstParticipant->stRTPSMsgHeader.guidPrefix, &pstDiscReader->guid.prefix))
                {
                    pstDiscReader = pstDiscReader->pNext;
                    continue;
                }

                if (RELIABLE_RELIABILITY_QOS == pstDiscReader->stDataReaderQos.reliability.kind)
                {
                    pstLocalWriter = GetDiscoveredReaderLocalWriterByGuid(pstDiscReader, &pstDataWriter->guid);
					if (pstLocalWriter != NULL)
					{
						if (ISLESSTHEN_SEQUENCENUMBER(pstLocalWriter->stAckNack.readerSNState.base, seqNumMin))
						{
							seqNumMin = pstLocalWriter->stAckNack.readerSNState.base;
						}
					}
                }

                pstDiscReader = pstDiscReader->pNext;
            }
            /* ����д����������ɾ����С����seqNum֮ǰ�������� */
            psHistoryData = pstDataWriter->stHistoryCache.pstHead;
            while (NULL != psHistoryData)
            {
                if (ISLESSTHEN_SEQUENCENUMBER(psHistoryData->seqNum, seqNumMin))
                {
                    psHistoryDataDel = psHistoryData;
                }
                /* ���ڵ�����СseqNumֵ������ */
                else
                {
                    break;
                }
                psHistoryData = psHistoryData->pNext;

                /* �����ͷ� */
                DDS_STATIC_FREE(psHistoryDataDel->data);
                psHistoryDataDel->data = NULL;
                DDS_STATIC_FREE(psHistoryDataDel);
                psHistoryDataDel = NULL;

                /* ͷ�ڵ�ƫ�� */
                pstDataWriter->stHistoryCache.pstHead = psHistoryData;
                pstDataWriter->stHistoryCache.uiHistoryDataNum--;
            }
			//printf("headSeqNum: %d\n", pstDataWriter->stHistoryCache.pstHead->seqNum.low);
            /* ͷ�ڵ�Ϊ�գ���Ϊ������β�ڵ�Ҳ���ÿ� */
            if (NULL == pstDataWriter->stHistoryCache.pstHead)
            {
                pstDataWriter->stHistoryCache.pstTail = NULL;
            }
        }
    }
}

DDS_DLL DDS_ReturnCode_t DDS_DeadLine_msg_write(DDS_DataWriter* pstDataWriter)
{
	DDS_ReturnCode_t retCode;
	HistoryData* pstHistoryData = NULL;
	char* buf = "DeadLine MSG.";
	int len = strlen(buf) + 1;
	//strcpy(buf, "DeadLine MSG.", len);
	if (NULL == pstDataWriter)
	{
		PrintLog(COMMLOG_ERROR, "DataWriter: Failed to send message, dataWriter is not exist.\n");
		return DDS_RETCODE_ERROR;
	}

	pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC_T(sizeof(HistoryData));
	if (NULL == pstHistoryData)
	{
		PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData, Insufficient memory space.\n");
		return DDS_RETCODE_ERROR;
	}
	InitHistoryData(pstHistoryData);
	/* ���뾲̬�ڴ洴����ʷ�������� */
	pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC_T(len);

	if (NULL == pstHistoryData->data)
	{
		/* ʧ���ͷ�ǰ��������ڴ� */
		DDS_STATIC_FREE(pstHistoryData);
		PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData buff, Insufficient memory space.\n");
		return DDS_RETCODE_ERROR;
	}

	/* ���ݿ����������� */
	memcpy(pstHistoryData->data, buf, len);

	/* ���û������ݳ��� */
	pstHistoryData->uiDataLen = len;

	/* �������ʱ��� */
	if (pstDataWriter->writerCallBack)
	{
		/* �û��Զ���ʱ��ص� */
		pstDataWriter->writerCallBack(&pstHistoryData->timeInfo);
	}
	else
	{
		/* Ĭ��ʱ��ص� */
		pstHistoryData->timeInfo = GetNowTime();
	}
	
	/* ��װ�������ķ��͸����ĵ������Ķ��� */
	DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
	retCode = JointUSERDataMsgSendUnknownReader(pstDataWriter, pstHistoryData);
	DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
	DDS_STATIC_FREE(pstHistoryData->data);
	DDS_STATIC_FREE(pstHistoryData);
	pstDataWriter->lastWriteTime = GetNowTime();
	return retCode;
}

VOID DeadLineTask(DDS_DataWriter* pstDataWriter)
{
	Duration_t nowTime;
	Duration_t restTime;
	Duration_t curTime;
	int tmpSleepTime;
	int	sleepTime = pstDataWriter->stDataWriterQos.deadline.period.sec * 1000;
	sleepTime += pstDataWriter->stDataWriterQos.deadline.period.nanosec / 1000 / 1000;
	
	while (pstDataWriter->destroyFlag == FALSE)
	{
		nowTime = GetNowTime();
		curTime = nowTime;
		//printf("now time %d:%d  ", nowTime.sec, nowTime.nanosec);
		nowTime = Sub_Duration(nowTime, pstDataWriter->lastWriteTime);
		//printf("last time %d:%d  now time  %d:%d \n", pstDataWriter->lastWriteTime.sec, pstDataWriter->lastWriteTime.nanosec,nowTime.sec, nowTime.nanosec);
        if (Compare_Duration(nowTime, pstDataWriter->stDataWriterQos.deadline.period) || Duration_IsEqual(nowTime, pstDataWriter->stDataWriterQos.deadline.period))
		{
			pstDataWriter->writerCallBack(&curTime);
			DDS_DeadLine_msg_write(pstDataWriter);
			pstDataWriter->lastWriteTime = GetNowTime();
            QY_Sleep(sleepTime);
		}
        else
        {
            restTime = Sub_Duration(pstDataWriter->stDataWriterQos.deadline.period, nowTime);
            tmpSleepTime = restTime.sec * 1000;
            tmpSleepTime += restTime.nanosec / 1000 / 1000;
            QY_Sleep(tmpSleepTime);
        }
	}
}

VOID UserDataHeartTask(DDS_DataWriter* pstDataWriter)
{
	DDS_DiscoveredReader* pstDiscReader = NULL;
	Time_t nowTime;
	INT now_time;
    UINT sleepTime = pstDataWriter->stDataWriterQos.controller.controller.period_per_heart;
    QY_Sleep(sleepTime);
    while (pstDataWriter->destroyFlag == FALSE)
    {
        if (pstDataWriter->stHistoryCache.uiHistoryDataNum == 0)
        {
            QY_Sleep(sleepTime);
            continue;
        }
		DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
        pstDiscReader = pstDataWriter->pstTopic->pstDiscReader;

        while (pstDiscReader != NULL)
        {
            nowTime = GetNowTime();
            now_time = nowTime.sec * 1000 + nowTime.nanosec / 1000 / 1000;
            now_time -= ((pstDiscReader->lastHeartTime.sec * 1000) + pstDiscReader->lastHeartTime.nanosec / 1000 / 1000);
            if (now_time > sleepTime)
            {

                JointHeartbeatMsgSendAssignReader(pstDataWriter, pstDiscReader);
                pstDiscReader->lastHeartTime = GetNowTime();
                pstDiscReader->heartMsgNum = 0;

            }
            pstDiscReader = pstDiscReader->pNext;
        }
		DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
        QY_Sleep(sleepTime);
    }
}

VOID DeadLineSendTask(DDS_DataWriter* pstDataWriter)
{
#if defined(_WIN32)
	pstDataWriter->hThread[0] = CreateThread(NULL, 0, (VOID*)&DeadLineTask, (VOID*)pstDataWriter, 0, NULL);
#elif defined(__linux__)
    pthread_create(&pstDataWriter->hThread[0], 0, DeadLineTask, pstDataWriter);
#elif defined (_VXWORKS)
#if defined _TM1 || defined _TM3

#else
    pstDataWriter->hThread[0] = taskSpawn("DeadLineTask",115,VX_FP_TASK,1024 * 1024 * 4,DeadLineTask,(int)pstDataWriter,0,0,0,0,0,0,0,0,0);
#endif
#endif
}

VOID UserDataHeartSendTask(DDS_DataWriter* pstDataWriter)
{
#if defined(_WIN32)
    pstDataWriter->hThread[1] = CreateThread(NULL, 0, (VOID*)&UserDataHeartTask, (VOID*)pstDataWriter, 0, NULL);
#elif defined(__linux__)
    pthread_create(&pstDataWriter->hThread[1], 0, UserDataHeartTask, pstDataWriter);
#elif defined (_VXWORKS)
#if defined _TM1 || defined _TM3

#else
    pstDataWriter->hThread[1] = taskSpawn("UserDataHeartTask",115,VX_FP_TASK,1024 * 1024 * 4,UserDataHeartTask,(int)pstDataWriter,0,0,0,0,0,0,0,0,0);
#endif
#endif
}

void SERIALIZE_SUBMESSAGEHEADER_L(SubMessageHeader subMsgHeader, MemoryBlock* memBlock)
{
	do
	{
		UINT32 uiIndex;
		CHAR cFlag = 0x00;
		PUT_BYTE((*memBlock), subMsgHeader.submessageId)
			for (uiIndex = 0; uiIndex < 4; uiIndex++)
			{
				cFlag |= ((subMsgHeader.flags[uiIndex] & 0x01) << uiIndex);
			}
		PUT_BYTE((*memBlock), cFlag)
			subMsgHeader.pcLenFlag = memBlock->wrPtr;
		//PUT_UNSIGNED_SHORT((*memBlock), subMsgHeader.submessageLength)
			do                                                                                         
			{                                                                                          
				unsigned int cdr_i;                                                                    
				unsigned int cdr_offset;                                                               
				/* handle align*/                                                                      
				cdr_offset = (((unsigned int)(*memBlock).writeIndex & 1) == 0) ? 0 :
				SHORT_SIZE - ((unsigned int)(*memBlock).writeIndex & 1);
				if (((*memBlock).writeIndex + SHORT_SIZE + cdr_offset) <= (*memBlock).totalSize)
				{                                                                                      
					memset((*memBlock).wrPtr, 0, cdr_offset);
					(*memBlock).wrPtr += cdr_offset;
					(*memBlock).writeIndex += cdr_offset + SHORT_SIZE;
					/* put data into memoryblock */                                                    
					memcpy((*memBlock).wrPtr, (unsigned char *)(&subMsgHeader.submessageLength), SHORT_SIZE);
					(*memBlock).wrPtr += SHORT_SIZE;
				}                                                                                      
				else                                                                                   
				{                                                                                      
					/* memory out of range */                                                          
					/* TODO: handle error */                                                           
				}                                                                                      
			} while (0);
	} while (0);
}

DDS_ReturnCode_t SendHistoryData(DDS_DataWriter* pstDataWriter)
{
	DDS_ReturnCode_t  retCode;
	MemoryBlock       stMemBlock;
	MemoryBlock       stMemBlockMod;
	SubMessageHeader  stSubMsgHeader;
	ParameterHead     stParamHead;
	InfoTimestamp     stInfoTS;
	InfoDestination   stInfoDest;
	Data              stData;
	CHAR              buff[NETWORK_BYTE_MAX_SIZE];

	DDS_DiscoveredReader* pstDiscReader;
	HistoryData* pstHistoryData = NULL;
	HistoryData* pstDelHistoryData = NULL;

	UINT32  uiOffset = 0;

	/* �ϰ��ĸ�ʽ��INFO_TS + INFO_DST + n��Data�ӱ��ģ��ȹ���ǰ��ı��ģ���ͨ��ѭ������Ҫ���͵����ݰ���װ��һ�� */
	INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);/* �̶����ĳ���Ϊ64KB�������ֽ����ʼ�������淢������ */

	/* ��һ�����л�RTPS����ͷ */
	SERIALIZE_RTPS_MESSAGE_HEADER(pstDataWriter->pstTopic->pstParticipant->stRTPSMsgHeader, stMemBlock);

	/* ʱ�䱨��ͷ���� */
	stSubMsgHeader.submessageId = INFO_TS;
	stSubMsgHeader.submessageLength = sizeof(Time_t);
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
	stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
	stSubMsgHeader.flags[2] = FALSE;   //dataFlag
	stSubMsgHeader.flags[3] = FALSE;   //keyFlag
	stInfoTS.value = GetNowTime();

	/* ���л�ʱ�䱨��ͷ */
	SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

	/* ���л�ʱ���ӱ����� */
	SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

	stSubMsgHeader.submessageId = INFO_DST;
	stSubMsgHeader.submessageLength = 12;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
	stSubMsgHeader.flags[1] = FALSE;
	stSubMsgHeader.flags[2] = FALSE;
	stSubMsgHeader.flags[3] = FALSE;
	stInfoDest.guidPrefix = pstDataWriter->guid.prefix;

	/* �ڶ������л��ӱ���ͷ */
	SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

	/* ���������л��ӱ�����Ϣ�� */
	SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

	pstHistoryData = pstDataWriter->stHistoryCache.pstHead;

	/* ���û�����ݣ�����Ҫ��������Ĳ���*/
	if (pstHistoryData == NULL || pstDataWriter->uiHistoryDataSize == 0)
	{
		return DDS_RETCODE_NO_DATA;
	}
	while (NULL != pstHistoryData)/* ͨ��ѭ��������ƴ��ͬһ������ */
	{
		if (FALSE == pstHistoryData->bSend)
		{
			pstHistoryData = pstHistoryData->pNext;
			continue;
		}

		pstHistoryData->bSend = FALSE;

		/* data����ͷ���� */
		stSubMsgHeader.submessageId = DATA;

		/* Nephlaem ����Data���ĳ��� */
		stSubMsgHeader.submessageLength = sizeof(stData.extraFlags) +
			sizeof(stData.octetsToInlineQos) + sizeof(stData.readerId) +
			sizeof(stData.writerId) + sizeof(stData.writerSN);

		stSubMsgHeader.submessageLength += sizeof(unsigned short) * 2 + pstHistoryData->uiDataLen;

		/* Nephalem �ֽڶ���*/	
		uiOffset = ((stSubMsgHeader.submessageLength & 1) == 0) ? 0 :
		SHORT_SIZE - (stSubMsgHeader.submessageLength & 1);

		stSubMsgHeader.submessageLength += uiOffset;

		if (0 == IS_MACHINE_LITTLE_ENDIAN)
			stSubMsgHeader.flags[0] = FALSE;    //littleEndian
		else
			stSubMsgHeader.flags[0] = TRUE;    //littleEndian
		stSubMsgHeader.flags[1] = FALSE;    //inlineQosFlag
		stSubMsgHeader.flags[2] = (pstHistoryData->data) ? TRUE : FALSE;  //dataPresent
		stSubMsgHeader.flags[3] = FALSE;   //keyFlag

		/* data���������� */
		stData.extraFlags[0] = 0x00;
		stData.extraFlags[1] = 0x00;
		stData.octetsToInlineQos = 0x0010;
		stData.readerId = ENTITYID_UNKNOWN;
		stData.writerId = pstDataWriter->guid.entityId;
		stData.writerSN = pstHistoryData->seqNum;
		if (IS_MACHINE_LITTLE_ENDIAN)
			stData.serializedPayload.m_cdrType = CDR_LE;   //littleEndian
		else
			stData.serializedPayload.m_cdrType = CDR_BE;    //littleEndian
		stData.serializedPayload.m_option = 0x0000;

		/* �ڶ������л��ӱ���ͷ */
		SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

		/* ���������л��ӱ�����Ϣ�� */
		SERIALIZE_DATA(stData, stMemBlock);

		/* ���л�����Ч�غ� */
		SERIALIZE_SERIALIZEDDATA(stData.serializedPayload, stMemBlock);

		/* ���л�д������Ϣ */
		PUT_BYTE_ARRAY(stMemBlock, pstHistoryData->data, pstHistoryData->uiDataLen);

		stMemBlock.writeIndex += uiOffset;
		stMemBlock.wrPtr += uiOffset;

		pstHistoryData = pstHistoryData->pNext;
	}

	pstDiscReader = pstDataWriter->pstTopic->pstDiscReader;
	if (pstDiscReader == NULL) return DDS_RETCODE_NO_AVAILABLE_READER;

	while (NULL != pstDiscReader)
	{
		/* Nephalem partition ����ͬһ���������� */
		if (FALSE == pstDiscReader->bSamePartiton)
		{
			pstDiscReader = pstDiscReader->pNext;
			continue;
		}

		if (BEST_EFFORT_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind && RELIABLE_RELIABILITY_QOS == pstDiscReader->stDataReaderQos.reliability.kind)
		{
			PrintLog(COMMLOG_WARN, "Reader and writer RELIABILITY_QOS do not match.\n");
			pstDiscReader = pstDiscReader->pNext;
			continue;
		}

		if (TRUE == RTPSSendMsg(&stMemBlock, &pstDataWriter->pstTopic->pstParticipant->socketList.user_data_sender, pstDiscReader->pstUnicastLocator))
		{
			retCode = DDS_RETCODE_OK;
		}
		else
		{
			retCode = DDS_RETCODE_ERROR;
		}

		pstDiscReader = pstDiscReader->pNext;

	}

	pstDataWriter->uiHistoryDataSize = 0; //���꽫��ʷ���ݳ��ȼ�¼����
	return retCode;
}

