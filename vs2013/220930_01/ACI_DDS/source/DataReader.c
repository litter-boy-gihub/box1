#include "../include/GlobalDefine.h"
/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_DataReader_Init
-   ��������: �Ķ�����ʼ��
-   ��    ��: �Ķ���ָ��
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �Ķ�����ʼ��
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DDS_DataReader_Init(DDS_DataReader* pstDataReader)
{
    if (NULL != pstDataReader)
    {
        pstDataReader->lastRecvTime.sec = 0;
        pstDataReader->lastRecvTime.nanosec = 0;
        pstDataReader->totHistoryNum = 0;
    //    InitHistoryCache(&pstDataReader->stUserData);
        /* Nephalem ��ʼ��*/
        pstDataReader->pstOwnerDiscWriter = NULL;
        pstDataReader->deadlineCallBack = NULL;
        pstDataReader->lastDataTime.sec = 0;
        pstDataReader->lastDataTime.nanosec = 0;
        pstDataReader->recvData = NULL;
        pstDataReader->recvCallBack = NULL;
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_DataReader_Uninit
-   ��������: д������ʼ��
-   ��    ��: д����ָ��
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: д������ʼ��
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DDS_DataReader_Uninit(DDS_DataReader* pstDataReader)
{
    if (NULL != pstDataReader)
    {
    //    UninitHistoryCache(&pstDataReader->stUserData);
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_DataReader_delete
-   ��������: �Ķ���ɾ��
-   ��    ��: �Ķ���ָ��
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �Ķ���ɾ��
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DDS_DataReader_delete(DDS_DataReader* pstDataReader)
{
    DDS_DataReader* pstPreDataReader = NULL;
    DDS_Topic* pstTopic = pstDataReader->pstTopic;

    DDS_DataReader* pstCurDataReader = pstTopic->pstLocalReader;
    while (NULL != pstCurDataReader)
    {
        if (pstCurDataReader == pstDataReader)
        {
            if (NULL == pstPreDataReader)
            {
                if (pstCurDataReader->pNext != NULL)
                {
                    pstTopic->pstLocalReader = pstCurDataReader->pNext;
                }
                DDS_DataReader_Uninit(pstCurDataReader);
                pstCurDataReader = pstTopic->pstLocalReader;
            }
            else
            {
                pstPreDataReader->pNext = pstCurDataReader->pNext;
                DDS_DataReader_Uninit(pstCurDataReader);
                pstCurDataReader = pstPreDataReader->pNext;
            }
            break;
        }
        else
        {
            pstPreDataReader = pstCurDataReader;
            pstCurDataReader = pstCurDataReader->pNext;
        }
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: JointAckNackMsgSendAssignWriter
-   ��������: ��Զ��writer�ظ�ack����
-   ��    ��: ����д����ָ��
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��Զ��writer�ظ�ack����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID JointAckNackMsgSendAssignWriter(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter)
{
    MemoryBlock stMemBlock;
    MemoryBlock stMemBlockMod;
    SubMessageHeader  stSubMsgHeader;
    InfoDestination   stInfoDest;
    CHAR buff[NETWORK_BYTE_MAX_SIZE];

    /* �����ֽ����ʼ�� */
    INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

    /* ��һ�����л�RTPS����ͷ */
    SERIALIZE_RTPS_MESSAGE_HEADER(pstDiscWriter->pstTopic->pstParticipant->stRTPSMsgHeader, stMemBlock);

    stSubMsgHeader.submessageId = INFO_DST;
    stSubMsgHeader.submessageLength = 12;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;
    stSubMsgHeader.flags[3] = FALSE;
    stInfoDest.guidPrefix = pstDiscWriter->guid.prefix;

    /* �ڶ������л��ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���������л��ӱ�����Ϣ�� */
    SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

    /* ����ACK����ͷ */
    stSubMsgHeader.submessageId = ACKNACK;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //finalFlag
    if (pstDiscWriter->stAckNack.readerSNState.numBits)
    {
        stSubMsgHeader.flags[1] = FALSE;
    }
    else
    {
        stSubMsgHeader.flags[1] = TRUE;
    }

    /* ����ACK������Ϣ�� */
    pstDiscWriter->stAckNack.readerId = pstDataReader->guid.entityId;
    pstDiscWriter->stAckNack.writerId = pstDiscWriter->guid.entityId;
    pstDiscWriter->stAckNack.count++;

    /* �ڶ������л��ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���������л��ӱ�����Ϣ�� */
    SERIALIZE_ACKNACK(pstDiscWriter->stAckNack, stMemBlock);
    

    /* �ӱ�����Ϣ�峤�ȣ�������Ϣͷ */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - pstDiscWriter->stAckNack.uiStartIndex;

    /* ���л��޸��ӱ��ĳ��� */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

    /* ����ack���� */
    RTPSSendMsg(&stMemBlock, &pstDataReader->pstTopic->pstParticipant->socketList.user_data_sender, pstDiscWriter->pstUnicastLocator);
}


/*---------------------------------------------------------------------------------
-   �� �� ��: JointNackFragMsgSendAssignWriter
-   ��������: ��Զ��writer�ظ�Nackfrag����
-   ��    ��: ����д����ָ��
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��Զ��writer�ظ�Nackfrag����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2021\4\13       1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
VOID JointNackFragMsgSendAssignWriter(SequenceNumber_t writerSN, DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter)
{
    MemoryBlock stMemBlock;
    MemoryBlock stMemBlockMod;
    SubMessageHeader  stSubMsgHeader;
    InfoDestination   stInfoDest;
    CHAR buff[NETWORK_BYTE_MAX_SIZE];
    NackFrag stNackFrag;
    HistoryData* pstHistoryData = NULL;
    UINT32 uiNumbit = 0;

    memset(stNackFrag.fragmentNumberState.bitMap, 0, FRAGMENT_BIT_NUMBER);
    /* �����ֽ����ʼ�� */
    INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

    /* ��һ�����л�RTPS����ͷ */
    SERIALIZE_RTPS_MESSAGE_HEADER(pstDiscWriter->pstTopic->pstParticipant->stRTPSMsgHeader, stMemBlock);

    stSubMsgHeader.submessageId = INFO_DST;
    stSubMsgHeader.submessageLength = 12;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;
    stSubMsgHeader.flags[3] = FALSE;
    stInfoDest.guidPrefix = pstDiscWriter->guid.prefix;

    /* �ڶ������л��ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���������л��ӱ�����Ϣ�� */
    SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

    /* ����ACK����ͷ */
    stSubMsgHeader.submessageId = NACK_FRAG;
    stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //finalFlag


    /* ����ACK������Ϣ�� */
    stNackFrag.readerId = pstDataReader->guid.entityId;
    stNackFrag.writerId = pstDiscWriter->guid.entityId;
    stNackFrag.count = ++pstDiscWriter->stAckNack.count;

    /* Nephalem ����ָ��writerSN������ */
    pstHistoryData = pstDiscWriter->stHistoryCache.pstHead;
    while (NULL != pstHistoryData)
    {
        if (ISEQUAL_SEQUENCENUMBER(writerSN, pstHistoryData->seqNum))
            break;
        pstHistoryData = pstHistoryData->pNext;
    }

    /* Nephalem writerSN�����ݲ����� ֱ�ӷ��� */
    if (NULL == pstHistoryData)
        return;

    /* Nephalem ��Ƭ����ȫ�����յ������ظ� */
    if (pstHistoryData->uiFragTotal  == pstHistoryData->uiFragNum)
        return;

    /* Nephalem �Ƿ�Ƭ */
    stNackFrag.writerSN = writerSN;
    stNackFrag.fragmentNumberState.base = pstHistoryData->uiFragNum;
    stNackFrag.fragmentNumberState.numBits = 0;

    while ((pstHistoryData->uiFragNum + uiNumbit <= pstHistoryData->uiFragTotal) && uiNumbit != 256)
    {
        /* Nephalem ��Ƭ���ݶ�ʧ������fragmentNumberState */
        if (pstHistoryData->FragDataStatus[pstHistoryData->uiFragNum + uiNumbit] == 1)
        {    
            stNackFrag.fragmentNumberState.bitMap[uiNumbit >> 0x05] |= (0x80000000 >> (uiNumbit & 0x1f));
        }
        uiNumbit++;
        stNackFrag.fragmentNumberState.numBits++;
    }

    
    
   
    /* �ڶ������л��ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���������л��ӱ�����Ϣ�� */
    SERIALIZE_NACKFRAG(stNackFrag, stMemBlock);


    /* �ӱ�����Ϣ�峤�ȣ�������Ϣͷ */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stNackFrag.uiStartIndex;

    /* ���л��޸��ӱ��ĳ��� */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

    /* ����ack���� */
    RTPSSendMsg(&stMemBlock, &pstDataReader->pstTopic->pstParticipant->socketList.user_data_sender, pstDiscWriter->pstUnicastLocator);
}

/*---------------------------------------------------------------------------------
-   �� �� ��: GetMissSeqNum
-   ��������: ��ȡ��ʧƬ��
-   ��    ��: �����Ķ�����Զ��д�������������seqNum
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��ȡ��ʧƬ��
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\14       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID GetDataReaderMissSeqNum(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter, SequenceNumber_t lastSN)
{
	SequenceNumber_t tempSN = SEQUENCENUMBER_ZERO;
    /* HistoryCache��seqNum����ɢ�������У�expectSeqNum��lastSN����ɢ�����������У�������һ���ԣ��鲢�Ƚ�һ�μ����ҳ�����ȱʧƬ�Σ������ݵݹ顢�����˱����ɣ�ɧ�� �м��м� */
    HistoryData* pstHistoryData = pstDiscWriter->stHistoryCache.pstHead;
    UINT32 uiNumbit = 0;

    /* ������գ�ÿһλ����ʾ��ǰ����Ƿ�ȱʧ */
    memset(pstDiscWriter->stAckNack.readerSNState.bitMap, 0, SEQUENCE_BIT_NUMBER);
   
    tempSN = pstDiscWriter->stAckNack.readerSNState.base;

    /* ���ֵ������ֵС��˵��δ��ʧ */
    if (ISLESSTHEN_SEQUENCENUMBER(lastSN, tempSN))
    {
        pstDiscWriter->stAckNack.readerSNState.numBits = 0;
        return;
    }

    while (!ISEQUAL_SEQUENCENUMBER(tempSN, Add_SequenceNumber(lastSN, SEQUENCENUMBER_START)))
    {
        if (NULL != pstHistoryData)
        {
            /* ���������������ݣ����� */
            
            //printf("Nephalem %d - %d uiFragNum : %d , uiFragTotal = %d \n", tempSN.low, pstHistoryData->seqNum.low, pstHistoryData->uiFragNum, pstHistoryData->uiFragTotal);
            if (ISEQUAL_SEQUENCENUMBER(pstHistoryData->seqNum, tempSN))
            {
                /* Nephalem ��Ƭ����δ��װȫʱҲ��¼Ϊȱʧ */
                if ((pstHistoryData->uiFragTotal > 0) && (pstHistoryData->uiFragNum != pstHistoryData->uiFragTotal))
                {
                    pstDiscWriter->stAckNack.readerSNState.bitMap[uiNumbit >> 0x05] |= (0x80000000 >> (uiNumbit & 0x1f));
                    
                    /* Nephalem ����Ack����֮ǰ �ȷ���NackFrag */
                    JointNackFragMsgSendAssignWriter(pstHistoryData->seqNum, pstDataReader, pstDiscWriter);
                }

                pstHistoryData = pstHistoryData->pNext;
            }
            /* ������Ҳû���������ݣ���¼ȱʧ */
            else
            {
                /* ÿһλ��¼�Ƿ���ȱʧ����base��ʼ�������¼����256���Ƿ���ȱʧ, λ���������ͳ�ĳ�����ȡģ���� */
                pstDiscWriter->stAckNack.readerSNState.bitMap[uiNumbit >> 0x05] |= (0x80000000 >> (uiNumbit & 0x1f));
            }
        }
        else
        {
            /* ��������꣬ʣ��ȫ��ȱʧ����¼ȱʧ */
            pstDiscWriter->stAckNack.readerSNState.bitMap[uiNumbit >> 0x05] |= (0x80000000 >> (uiNumbit & 0x1f));
        }
        pstDiscWriter->stAckNack.readerSNState.numBits = uiNumbit + 1;

        tempSN = Add_SequenceNumber(tempSN, SEQUENCENUMBER_START);

        /* ������������256�����������ȱʧ���ȴ��´��������� */
        uiNumbit++;
        if (256 == uiNumbit)
        {
            return;
        }
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: CorrectAllHistoryCache
-   ��������: reader�ܻ���ﵽdepth���޺���������writer����ɻ���
-   ��    ��: �����Ķ���
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: 
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\12\08       1.0        Frank       Created
----------------------------------------------------------------------------------*/
VOID CorrectOldHistoryCache(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter)
{
	HistoryData* pstDelHistoryData = NULL;
    HistoryData* pstTempHistoryData = pstDiscWriter->stHistoryCache.pstHead;
	HistoryData* pstNextHistoryData = NULL;
    while (NULL != pstTempHistoryData)
    {
		pstNextHistoryData = pstTempHistoryData->pNext;
        /*���������ٵ���depth�������£������ύ*/
        if (pstDiscWriter->stHistoryCache.uiHistoryDataNum <= pstDataReader->stDataReaderQos.history.depth)
        {
            return;
        }
        else
        {
            /* ���������޳� */
			pstDiscWriter->stHistoryCache.pstHead = pstTempHistoryData->pNext;
            /* ���������Լ� */
            pstDiscWriter->stHistoryCache.uiHistoryDataNum--;

            pstDiscWriter->stAckNack.readerSNState.base = Add_SequenceNumber(pstTempHistoryData->seqNum, SEQUENCENUMBER_START);

            /* Nephalme ��Ƭ����δ��װ��ȫʱ ֱ��ɾ����HistoryData ���ύ */
            if ((pstTempHistoryData->uiFragTotal > 0) && (pstTempHistoryData->uiFragNum != pstTempHistoryData->uiFragTotal))
            {
                pstDelHistoryData = pstTempHistoryData;

                pstTempHistoryData = pstTempHistoryData->pNext;
                /* �ͷŻ������е������� */
                UninitHistoryData(pstDelHistoryData);
                /* �ͷŻ����� */
                DDS_STATIC_FREE(pstDelHistoryData);        
                pstDelHistoryData = NULL;

                continue;
            }

#if defined QYDDS_MASSDATA
            /* Nephalem massdata��Ƭ���ݽ������ ��ŵ�MassDataHistoryCache��������ֱ���ύ */
            if (pstTempHistoryData->massDataTotolSize > 0)
            {
                ProcessMassData(pstTempHistoryData, pstDataReader, pstDiscWriter);

                /* Nephalem massdata ���ݴ����������ͷ� */
                pstDelHistoryData = pstTempHistoryData;
                pstTempHistoryData = pstTempHistoryData->pNext;
                DDS_STATIC_FREE(pstDelHistoryData->data);
                pstDelHistoryData->data = NULL;
                DDS_STATIC_FREE(pstDelHistoryData);
                pstDelHistoryData = NULL;
                continue;
            }
#endif 
            /* �ύ���û� */
            DOUBLE_INSERT_TAIL(pstDataReader->pstTopic->stUserData.pstHead, pstDataReader->pstTopic->stUserData.pstTail, pstTempHistoryData);
                /* ÿ�ύһ�Σ��û����ݸ������� */
            pstDataReader->pstTopic->stUserData.uiHistoryDataNum++;

        }

        pstTempHistoryData = pstNextHistoryData;
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: CorrectDiscoveredWriterHistoryCache
-   ��������: ����Զ��д�����������ύ
-   ��    ��: �����Ķ�����Զ��д����
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ����Զ��д�����������ύ
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\14       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID CorrectDiscoveredWriterHistoryCache(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter)
{
	HistoryData* pstDelHistoryData = NULL;
	HistoryData* pstNextHistoryData = NULL;
    /* Զ��д��������Ϊ��ɢ������������һ���Կ����ύ�����Լ��������� */
    HistoryData* pstTempHistoryData = pstDiscWriter->stHistoryCache.pstHead;
	HistoryData* pstPreHistoryData = pstDiscWriter->stHistoryCache.pstHead; /* JD 2021-12-01 ǰ�ýڵ� */
	SequenceNumber_t baseSeqNum = pstDiscWriter->stAckNack.readerSNState.base;

    while (NULL != pstTempHistoryData)
    {
		pstNextHistoryData = pstTempHistoryData->pNext;
        /* ��������С������ֵ������˳��ȫ���ύ */
		if (ISLESSTHEN_SEQUENCENUMBER(pstTempHistoryData->seqNum, baseSeqNum))
        {
			/* JD 2021-12-01 �����ύ���ݣ��Ƚ������ύ�����ݴ�pDisDataWriter��ɾ�� */
			pstDiscWriter->stHistoryCache.uiHistoryDataNum--;
			if (pstTempHistoryData == pstDiscWriter->stHistoryCache.pstHead)
			{ /* ��ǰ�ύ����Ϊ������ */
				if (NULL == pstDiscWriter->stHistoryCache.pstHead->pNext)
				{
					pstDiscWriter->stHistoryCache.pstHead = NULL;
					pstDiscWriter->stHistoryCache.pstTail = NULL;
				}
				else {
					pstDiscWriter->stHistoryCache.pstHead = pstDiscWriter->stHistoryCache.pstHead->pNext;
					pstTempHistoryData->pNext = NULL;
				}
			}
			else if (pstTempHistoryData == pstDiscWriter->stHistoryCache.pstTail)
			{/* ��ǰ�ύ����Ϊβ����*/
				pstDiscWriter->stHistoryCache.pstTail = pstPreHistoryData;
				pstPreHistoryData->pNext = NULL;
				pstTempHistoryData->pNext = NULL;
			}
			else
			{ /* ��ǰ����Ϊ�м����� */
				pstPreHistoryData->pNext = pstTempHistoryData->pNext;
				pstTempHistoryData->pNext = NULL;
			}

            /* Nephalme ��Ƭ����δ��װ��ȫʱ ֱ��ɾ����HistoryData ���ύ */
            if ((pstTempHistoryData->uiFragTotal > 0) && (pstTempHistoryData->uiRecvFragNum != pstTempHistoryData->uiFragTotal))
            {
                pstDelHistoryData = pstTempHistoryData;
                pstTempHistoryData = pstNextHistoryData;

                /* �ͷŻ������е������� */
                UninitHistoryData(pstDelHistoryData);
                /* �ͷŻ����� */
                DDS_STATIC_FREE(pstDelHistoryData);
                pstDelHistoryData = NULL;
                continue;
            }

#if defined QYDDS_MASSDATA
            /* Nephalem massdata��Ƭ���ݽ������ ��ŵ�MassDataHistoryCache��������ֱ���ύ */
            if (pstTempHistoryData->massDataTotolSize > 0)
            {
                ProcessMassData(pstTempHistoryData, pstDataReader, pstDiscWriter);

                /* Nephalem massdata ���ݴ����������ͷ� */
                pstDelHistoryData = pstTempHistoryData;
                pstTempHistoryData = pstTempHistoryData->pNext;
                DDS_STATIC_FREE(pstDelHistoryData->data);
                pstDelHistoryData->data = NULL;
                DDS_STATIC_FREE(pstDelHistoryData);
                pstDelHistoryData = NULL;
                continue;
            }
#endif 

			DOUBLE_INSERT_TAIL(pstDataReader->pstTopic->stUserData.pstHead, pstDataReader->pstTopic->stUserData.pstTail, pstTempHistoryData);
            /* ÿ�ύһ�Σ��û����ݸ������� */
            pstDataReader->pstTopic->stUserData.uiHistoryDataNum++;
 
        }
        /* �������ݵ�������ֵ����ɢ�������������ύ */
		else if (ISEQUAL_SEQUENCENUMBER(pstTempHistoryData->seqNum, baseSeqNum))
        {
            /* Nephalme ��Ƭ����δ��װ��ȫʱ ���ύ */
            if ((pstTempHistoryData->uiFragTotal > 0) && pstTempHistoryData->uiRecvFragNum != pstTempHistoryData->uiFragTotal)
            {
                pstTempHistoryData = pstTempHistoryData->pNext;
                break;
            }
			/* JD 2021-12-01 �����ύ���ݣ��Ƚ������ύ�����ݴ�pDisDataWriter��ɾ�� */
			pstDiscWriter->stHistoryCache.uiHistoryDataNum--;
			if (pstTempHistoryData == pstDiscWriter->stHistoryCache.pstHead)
			{ /* ��ǰ�ύ����Ϊ������ */
				if (NULL == pstDiscWriter->stHistoryCache.pstHead->pNext)
				{
					pstDiscWriter->stHistoryCache.pstHead = NULL;
					pstDiscWriter->stHistoryCache.pstTail = NULL;
				}
				else {
					pstDiscWriter->stHistoryCache.pstHead = pstDiscWriter->stHistoryCache.pstHead->pNext;
					pstTempHistoryData->pNext = NULL;
				}
			}
			else if(pstTempHistoryData == pstDiscWriter->stHistoryCache.pstTail)
			{/* ��ǰ�ύ����Ϊβ����*/
				pstDiscWriter->stHistoryCache.pstTail = pstPreHistoryData;
				pstPreHistoryData->pNext = NULL;
				pstTempHistoryData->pNext = NULL;
			}
			else
			{ /* ��ǰ����Ϊ�м����� */
				pstPreHistoryData->pNext = pstTempHistoryData->pNext;
				pstTempHistoryData->pNext = NULL;
			}
			
			/* ����reader���������������1 */
			pstDiscWriter->stAckNack.readerSNState.base = Add_SequenceNumber(pstDiscWriter->stAckNack.readerSNState.base, SEQUENCENUMBER_START);

#if defined QYDDS_MASSDATA
            /* Nephalem massdata��Ƭ���ݽ������ ��ŵ�MassDataHistoryCache��������ֱ���ύ */
            if (pstTempHistoryData->massDataTotolSize > 0)
            {
                ProcessMassData(pstTempHistoryData, pstDataReader, pstDiscWriter);

                /* Nephalem massdata ���ݴ����������ͷ� */
                pstDelHistoryData = pstTempHistoryData;
                pstTempHistoryData = pstTempHistoryData->pNext;
                DDS_STATIC_FREE(pstDelHistoryData->data);
                pstDelHistoryData->data = NULL;
                DDS_STATIC_FREE(pstDelHistoryData);
                pstDelHistoryData = NULL;
                continue;
            }
#endif 

			/* �ύ���û� */
		    DOUBLE_INSERT_TAIL(pstDataReader->pstTopic->stUserData.pstHead, pstDataReader->pstTopic->stUserData.pstTail, pstTempHistoryData);
            /* ÿ�ύһ�Σ��û����ݸ������� */
            pstDataReader->pstTopic->stUserData.uiHistoryDataNum++;
			baseSeqNum = Add_SequenceNumber(baseSeqNum, SEQUENCENUMBER_START);
        }
        /* �������ݴ�������ֵ*/
        else
        {
            printf("Nephalem :  loss Data Seq %d recv seq %d\n", pstDiscWriter->stAckNack.readerSNState.base.low
				,pstTempHistoryData->seqNum.low);
            return;
        }
        //pstTempHistoryData = pstDiscWriter->stHistoryCache.pstHead;
        //pstTempHistoryData = pstTempHistoryData->pNext;

		pstTempHistoryData = pstNextHistoryData;
    }
}

VOID CorrectDiscoveredWriterGapMsg(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter, Gap* pstGapMsg)
{
    HistoryData* pstTempHistoryData = pstDiscWriter->stHistoryCache.pstHead;
    SequenceNumber_t gapEnd = pstGapMsg->gapStart;
    gapEnd.low += pstGapMsg->gapList.numBits;
    while (NULL != pstTempHistoryData)
    {
        /*��Gap���������ʧ����SeqNumС�ģ�ȫ���ύ*/
        if (ISLESSTHEN_SEQUENCENUMBER(pstTempHistoryData->seqNum, gapEnd))
        {
            /* ���ʱע��β�ڵ� */
            if (pstTempHistoryData == pstDiscWriter->stHistoryCache.pstTail)
            {
                pstDiscWriter->stHistoryCache.pstTail = NULL;
            }

            /* ���������޳� */
            pstDiscWriter->stHistoryCache.pstHead = pstTempHistoryData->pNext;

            /* �ύ���û� */
            DOUBLE_INSERT_TAIL(pstDataReader->pstTopic->stUserData.pstHead, pstDataReader->pstTopic->stUserData.pstTail, pstTempHistoryData);

            /* ÿ�ύһ�Σ��û����ݸ������� */
            pstDataReader->pstTopic->stUserData.uiHistoryDataNum++;

            /* ���������Լ� */
            pstDiscWriter->stHistoryCache.uiHistoryDataNum--;
            pstTempHistoryData = pstDiscWriter->stHistoryCache.pstHead;
            continue;
        }
        else
        {
            pstDiscWriter->stAckNack.readerSNState.base = gapEnd;
            break;
        }
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DeilverStatuslessUserMsg
-   ��������: �ύ��״̬�Ķ�������
-   ��    ��: �Ķ�����Զ��д�������û���������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �ύ��״̬�Ķ�������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\14       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DeilverStatuslessUserMsg(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter, HistoryData* pstHistoryData)
{
	HistoryData* pstTempHistoryData = NULL;
	HistoryData* pstDelHistoryData = NULL;
	HistoryData* pstNextHistoryData = NULL;
	BOOL bDeilverFragDataFlag = FALSE;
    if (pstDiscWriter == NULL)
    {
        return;
    }
	
	if(pstDiscWriter->stDataWriterQos.liveliness.kind == MANUAL_BY_TOPIC_LIVELINESS_QOS)
		pstDiscWriter->lastLivelinessTime = GetNowTime();

    if (NULL != pstDataReader->pstTopic->stUserData.pstHead && pstHistoryData->uiFragTotal == 0)
    {
//         printf("After reading the data, Please clean up the memory");
//         exit(EXIT_FAILURE);
    }

	if (pstDataReader == pstDataReader->pstTopic->pstLocalReader)
	{
		if (pstHistoryData->uiFragTotal > 0)
		{
			/* JD 2021-11-26 ����Ƿ�Ƭ���ģ������������HistoryData ֵ����һ�ݺ���в��� */
			HistoryData* pstFragHistoryData = (HistoryData*)malloc(sizeof(HistoryData));
			InitHistoryData(pstFragHistoryData);
			CopyHistoryData(pstFragHistoryData, pstHistoryData);


			pstTempHistoryData = pstDiscWriter->stHistoryCache.pstHead;
			/* Nephalem ���ҷ�Ƭ���ݶ�ӦseqNum��HistoryData */
			while (NULL != pstTempHistoryData)
			{
				if (ISEQUAL_SEQUENCENUMBER(pstTempHistoryData->seqNum, pstFragHistoryData->seqNum))
					break;
				pstTempHistoryData = pstTempHistoryData->pNext;
			}

			//printf("RecvData SN = %d and FragNum = %d \n", pstHistoryData->seqNum.low, pstHistoryData->uiFragNum);
			//if (pstHistoryData->uiFragNum == 1) //JD �˷����ж��ڶ����ͬ��DataReader�����������
			if (NULL == pstTempHistoryData)
			{
				do {
					pstFragHistoryData->data = (CHAR*)DDS_STATIC_MALLOC_T(pstFragHistoryData->uiDataLen);

					if (NULL == pstFragHistoryData->data)
					{
						/* �û����������ڴ�ʧ�����ͷ�֮ǰ���� */
						/* Nephalem �ͷ�pstHistoryData�������ڴ�й© */
						DDS_STATIC_FREE(pstFragHistoryData->pstFragData->data);
						pstFragHistoryData->pstFragData->data = NULL;
						DDS_STATIC_FREE(pstFragHistoryData->pstFragData);
						pstFragHistoryData->pstFragData = NULL;
						DDS_STATIC_FREE(pstFragHistoryData);
						pstFragHistoryData = NULL;
						return;
					}
					pstFragHistoryData->uiRecvFragNum += 1;
					memcpy(pstFragHistoryData->data, pstFragHistoryData->pstFragData->data, pstFragHistoryData->pstFragData->usDataLen);

					DDS_STATIC_FREE(pstFragHistoryData->pstFragData->data);
					pstFragHistoryData->pstFragData->data = NULL;
					DDS_STATIC_FREE(pstFragHistoryData->pstFragData);
					pstFragHistoryData->pstFragData = NULL;

					/* ������������ֱ��ɾ����û����� */
					if (!InsertHistoryCacheOrder(&pstDiscWriter->stHistoryCache, pstFragHistoryData))
					{
						PrintLog(COMMLOG_INFO, "DeilverStatuslessUserMsg:User data was not inserted, Because user data already exists.\n");
						DDS_STATIC_FREE(pstFragHistoryData->data);
						pstFragHistoryData->data = NULL;
						DDS_STATIC_FREE(pstFragHistoryData);
						pstFragHistoryData = NULL;;
						return;
					}
				} while (0);
			}
			else
			{
				//printf("Nephalem : Recv %d.%d \n", pstHistoryData->seqNum.low, pstHistoryData->uiFragNum);

				/* Nephalem �÷�Ƭ���ݵķ�Ƭ������ѱ���ķ�Ƭ��Ų��������� ��*/
				if (pstTempHistoryData->uiFragNum == (pstFragHistoryData->uiFragNum - 1))
				{

					pstTempHistoryData->uiRecvFragNum += 1;
					/* Nephalem ���Ʒ�Ƭ���ݵ�HistoryDfata�� */
					memcpy(pstTempHistoryData->data + pstFragHistoryData->uiFragLen * pstTempHistoryData->uiFragNum, pstFragHistoryData->pstFragData->data, pstFragHistoryData->pstFragData->usDataLen);
					pstTempHistoryData->uiFragNum += 1;

					if (pstFragHistoryData->uiFragNum == pstFragHistoryData->uiFragTotal)
						bDeilverFragDataFlag = TRUE;
				}

				//printf("Nephalem : Err %d.%d -  %d.%d\n", pstTempHistoryData->seqNum.low, pstTempHistoryData->uiFragNum, pstHistoryData->seqNum.low, pstHistoryData->uiFragNum);
				UninitHistoryData(pstFragHistoryData);
				DDS_STATIC_FREE(pstFragHistoryData);
				pstFragHistoryData = NULL;

				if (pstTempHistoryData->uiRecvFragNum == pstTempHistoryData->uiFragTotal)
					bDeilverFragDataFlag = TRUE;

			}
		}
		else
		{
			//pstDataReader->totHistoryNum++;
			DOUBLE_INSERT_TAIL(pstDataReader->pstTopic->stUserData.pstHead, pstDataReader->pstTopic->stUserData.pstTail, pstHistoryData);
			// ÿ�ύһ�Σ��û����ݸ������� 
			pstDataReader->pstTopic->stUserData.uiHistoryDataNum++;
		}
	}

    if (bDeilverFragDataFlag)
    {
        pstTempHistoryData = pstDiscWriter->stHistoryCache.pstHead;
        
        while (NULL != pstTempHistoryData)
        {
			pstNextHistoryData = pstTempHistoryData->pNext;


			/* JD 2021-12-01 �����ύ���ݣ��Ƚ������ύ�����ݴ�pDisDataWriter��ɾ��pstDiscWriter->stHistoryCache */
			/* JD 2021-12-01 BEST_EFFORTʱ pstDiscWriter->stHistoryCache���ǿ��ύ���ݣ������ύ���ݶ�ɾ���� */
			pstDiscWriter->stHistoryCache.uiHistoryDataNum--;
			if (pstTempHistoryData == pstDiscWriter->stHistoryCache.pstHead)
			{ /* ��ǰ�ύ����Ϊ������ */
				if (NULL == pstDiscWriter->stHistoryCache.pstHead->pNext)
				{
					pstDiscWriter->stHistoryCache.pstHead = NULL;
					pstDiscWriter->stHistoryCache.pstTail = NULL;
				}
				else {
					pstDiscWriter->stHistoryCache.pstHead = pstDiscWriter->stHistoryCache.pstHead->pNext;
					pstTempHistoryData->pNext = NULL;
				}
			}

            /*��������*/
            pstDiscWriter->stAckNack.readerSNState.base = Add_SequenceNumber(pstTempHistoryData->seqNum, SEQUENCENUMBER_START);
           

            /* Nephalme ��Ƭ����δ��װ��ȫʱ ֱ��ɾ����HistoryData ���ύ */
            if ((pstTempHistoryData->uiFragTotal > 0) && (pstTempHistoryData->uiRecvFragNum != pstTempHistoryData->uiFragTotal))
            {
                pstDelHistoryData = pstTempHistoryData;

                pstTempHistoryData = pstTempHistoryData->pNext;
                /* �ͷŻ������е������� */
                DDS_STATIC_FREE(pstDelHistoryData->data);
                pstDelHistoryData->data = NULL;
                /* �ͷŻ����� */
                DDS_STATIC_FREE(pstDelHistoryData);
                pstDelHistoryData = NULL;
  
                continue;
            }

			/* �ύ���û� */
			DOUBLE_INSERT_TAIL(pstDataReader->pstTopic->stUserData.pstHead, pstDataReader->pstTopic->stUserData.pstTail, pstTempHistoryData);
            /* ÿ�ύһ�Σ��û����ݸ������� */
            pstDataReader->pstTopic->stUserData.uiHistoryDataNum++;

            pstTempHistoryData = pstNextHistoryData;
        }
    }

    /* �ص������������û����� */
    if (pstDataReader->pstTopic->stUserData.pstHead)
    {
        if (NULL != pstDataReader->recvCallBack)
        {
        #ifdef QYDDS_TIME_BASED_FILTER_QOS
            if (pstDataReader->stDataReaderQos.time_based_filter.minimum_separation.sec != 0 || pstDataReader->stDataReaderQos.time_based_filter.minimum_separation.nanosec != 0)
            {
                Time_t nowTime = GetNowTime();
                Duration_t timeUsed = Sub_Duration(nowTime, pstDataReader->lastRecvTime);
                if (Compare_Duration(timeUsed, pstDataReader->stDataReaderQos.time_based_filter.minimum_separation))
                {
                    (pstDataReader->recvCallBack)(pstDataReader);
                    pstDataReader->lastRecvTime = nowTime;
                }
            }
            else
            {
                (pstDataReader->recvCallBack)(pstDataReader);
            }

        #else
            (pstDataReader->recvCallBack)(pstDataReader);
        #endif
        }
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DeilverStatusFulUserMsg
-   ��������: �ύ��״̬�Ķ�������
-   ��    ��: �Ķ�����Զ��д�������û���������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �ύ��״̬�Ķ�������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\14       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DeilverStatusFulUserMsg(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter, HistoryData* pstHistoryData)
{
	SequenceNumber_t maxSeq = SEQUENCENUMBER_ZERO;
	HistoryData* pstTempHistoryData = NULL;
	CHAR* pstTemp = NULL;
	INT32 i = 0;
	if (pstDiscWriter == NULL)
	{
		UninitHistoryData(pstHistoryData);
		DDS_STATIC_FREE(pstHistoryData);
		pstHistoryData = NULL;
		return;
	}
	//    if (NULL != pstDataReader->pstTopic->stUserData.pstHead)
	//    {
	////         printf("After reading the data, Please clean up the memory");
	////         exit(EXIT_FAILURE);
	//    }

		/* Nephalem resource_limits KEEP_ALL��ֻ����һ����Χ����ŵ����� */
	if (KEEP_ALL_HISTORY_QOS == pstDataReader->stDataReaderQos.history.kind
		&& LENGTH_UNLIMITED != pstDataReader->stDataReaderQos.resource_limits.max_samples)
	{
		/* Nephalem ���յ�������ݱ��Ϊ readerSNState.base + resource_limits.max_samples */
		maxSeq = SEQUENCENUMBER_ZERO;
		maxSeq.low = pstDataReader->stDataReaderQos.resource_limits.max_samples;
		maxSeq = Add_SequenceNumber(pstDiscWriter->stAckNack.readerSNState.base, maxSeq);

		/* Nephalem ���ݱ�Ŵ��ڿɽ������ݵ�����ţ��ܾ����� */
		if (ISLESSTHEN_SEQUENCENUMBER(maxSeq, pstHistoryData->seqNum))
		{
			UninitHistoryData(pstHistoryData);
			DDS_STATIC_FREE(pstHistoryData);
			pstHistoryData = NULL;
			return;
		}
	}

	//ֻ��λ��ͷ���ı���Reader�Żᱣ�����ݵ�HistoryCache�У������ύ����ʱ�����ݵ�ַ����Topic��HistoryCache�У�����ʼ�ձ�����ͷ������Reader�Ļ���
	if (pstDataReader == pstDataReader->pstTopic->pstLocalReader) 
	{
		/* Nephalem �ְ����ݴ��� */
		if (pstHistoryData->uiFragTotal > 0)
		{
			/* JD 2021-11-25 ����Ƿ�Ƭ���ģ������������HistoryData ֵ����һ�ݺ���в��� */
			HistoryData* pstFragHistoryData = (HistoryData*)malloc(sizeof(HistoryData));
			InitHistoryData(pstFragHistoryData);
			CopyHistoryData(pstFragHistoryData, pstHistoryData);

			//printf("Nephalem : Recv %d.%d \n", pstHistoryData->seqNum.low, pstHistoryData->uiFragNum);
			pstTempHistoryData = pstDiscWriter->stHistoryCache.pstHead;
			/* Nephalem ���ҷ�Ƭ���ݶ�ӦseqNum��HistoryData */
			while (NULL != pstTempHistoryData)
			{
				if (ISEQUAL_SEQUENCENUMBER(pstTempHistoryData->seqNum, pstHistoryData->seqNum))
					break;
				pstTempHistoryData = pstTempHistoryData->pNext;
			}
			/* Nephalem �޷��ҵ��÷�Ƭ���ݵ�HistoryData */
			if (NULL == pstTempHistoryData)
			{
				/* JD 2021-11-26 ���յ���Ƭ���ݽ���+1*/
				pstFragHistoryData->uiRecvFragNum += 1;
				/* Nephalem �������飬������ݽ���״̬ */
				pstFragHistoryData->FragDataStatus = (CHAR*)DDS_STATIC_MALLOC_T(pstFragHistoryData->uiFragTotal + 1);
				/* Nephalem ��0λ�����壬���з�Ƭ����״̬����Ϊ1����ʾδ�յ� */
				memset(pstFragHistoryData->FragDataStatus, 1, pstFragHistoryData->uiFragTotal + 1);

				pstFragHistoryData->data = (CHAR*)DDS_STATIC_MALLOC_T(pstFragHistoryData->uiDataLen);

				if (NULL == pstFragHistoryData->data)
				{
					/* �û����������ڴ�ʧ�����ͷ�֮ǰ���� */
					/* Nephalem �ͷ�pstFragHistoryData�������ڴ�й© */
					UninitHistoryData(pstFragHistoryData);
					DDS_STATIC_FREE(pstFragHistoryData);
					pstFragHistoryData = NULL;
					return;
				}

				/* Nephalem �򻺴���������� */
				InsertHistoryCacheOrder(&pstDiscWriter->stHistoryCache, pstFragHistoryData);

				/* Nephalem ��ǰ��Ƭ��������Ϊ�ѽ��� */
				pstFragHistoryData->FragDataStatus[pstFragHistoryData->uiFragNum] = 0;

				memcpy(pstFragHistoryData->data + pstFragHistoryData->uiFragLen * (pstFragHistoryData->uiFragNum - 1), pstFragHistoryData->pstFragData->data, pstFragHistoryData->pstFragData->usDataLen);

				DDS_STATIC_FREE(pstFragHistoryData->pstFragData->data);
				pstFragHistoryData->pstFragData->data = NULL;

				DDS_STATIC_FREE(pstFragHistoryData->pstFragData);
				pstFragHistoryData->pstFragData = NULL;

				pstFragHistoryData->uiFragNum = pstFragHistoryData->uiFragTotal + 1;
				for (i = 1; i <= pstFragHistoryData->uiFragTotal; i++)
				{
					if (pstFragHistoryData->FragDataStatus[i] == 1)
					{
						pstFragHistoryData->uiFragNum = i;
						break;
					}
				}
			}
			else
			{
				/* JD 2021-11-26 �ж������Ƿ��Ѿ����գ�����Ѿ������򲻽��д��� */
				if (pstTempHistoryData->FragDataStatus[pstFragHistoryData->uiFragNum] != 0)
				{
					pstTempHistoryData->uiRecvFragNum += 1;
					/* Nephalem ��ǰ��Ƭ��������Ϊ�ѽ��� */
					pstTempHistoryData->FragDataStatus[pstFragHistoryData->uiFragNum] = 0;

					memcpy(pstTempHistoryData->data + pstFragHistoryData->uiFragLen * (pstFragHistoryData->uiFragNum - 1), pstFragHistoryData->pstFragData->data, pstFragHistoryData->pstFragData->usDataLen);

					UninitHistoryData(pstFragHistoryData);

					DDS_STATIC_FREE(pstFragHistoryData);
					pstFragHistoryData = NULL;

					/* Nephalem pstHistoryData->uiFragTotal + 1��ʾ���з�Ƭ�����յ� */
					pstTempHistoryData->uiFragNum = pstTempHistoryData->uiFragTotal + 1;
					for (i = 1; i <= pstTempHistoryData->uiFragTotal; i++)
					{
						if (pstTempHistoryData->FragDataStatus[i] == 1)
						{
							pstTempHistoryData->uiFragNum = i;
							break;
						}
					}

				}

				UninitHistoryData(pstFragHistoryData);
				DDS_STATIC_FREE(pstFragHistoryData);
				pstFragHistoryData = NULL;

			}

		}
		else
		{
			/* Nephalem ��HistoryData���뵽������ */
			/*DeadLine����ֱ���ύ*/
			if (strcmp(pstHistoryData->data, "DeadLine MSG.") == 0)
			{
				/* �ύ���û� */
				DOUBLE_INSERT_TAIL(pstDataReader->pstTopic->stUserData.pstHead, pstDataReader->pstTopic->stUserData.pstTail, pstHistoryData);
				/* �ص������������û����� */
				if (NULL != pstDataReader->recvCallBack)
				{
#ifdef QYDDS_TIME_BASED_FILTER_QOS
					if (pstDataReader->stDataReaderQos.time_based_filter.minimum_separation.sec != 0 || pstDataReader->stDataReaderQos.time_based_filter.minimum_separation.nanosec != 0)
					{
						Time_t nowTime = GetNowTime();
						Duration_t timeUsed = Sub_Duration(nowTime, pstDataReader->lastRecvTime);
						if (Compare_Duration(timeUsed, pstDataReader->stDataReaderQos.time_based_filter.minimum_separation))
						{
							(pstDataReader->recvCallBack)(pstDataReader);
							pstDataReader->lastRecvTime = nowTime;
						}
					}
					else
					{
						(pstDataReader->recvCallBack)(pstDataReader);
					}
#else
					(pstDataReader->recvCallBack)(pstDataReader);
#endif
				}
				return;
			}

			/* ������������ֱ��ɾ����û����� */
			if (!InsertHistoryCacheOrder(&pstDiscWriter->stHistoryCache, pstHistoryData))
			{
				PrintLog(COMMLOG_INFO, "HandleUSERWriterDataMsg:User data was not inserted, Because user data already exists.\n");
				UninitHistoryData(pstHistoryData);
				DDS_STATIC_FREE(pstHistoryData);
				pstHistoryData = NULL;
				return;
			}
		}

		/* Nephalem resource_limits  KEEP_LAST */
		if (KEEP_LAST_HISTORY_QOS == pstDataReader->stDataReaderQos.history.kind)
		{
			/* ����history���ޣ�������ɻ��� */
			if (pstDiscWriter->stHistoryCache.uiHistoryDataNum > pstDataReader->stDataReaderQos.history.depth)
				CorrectOldHistoryCache(pstDataReader, pstDiscWriter);
			else/* �������� */
				CorrectDiscoveredWriterHistoryCache(pstDataReader, pstDiscWriter);
		}
		/* Nephalem resource_limits  KEEP_ALL ��ɾ���κ����� */
		else if (KEEP_ALL_HISTORY_QOS == pstDataReader->stDataReaderQos.history.kind)
		{
			/* Nephalem KeepAll �²����ܳ���history���ޣ��������� */
			CorrectDiscoveredWriterHistoryCache(pstDataReader, pstDiscWriter);
		}

	}

    if (pstDataReader->pstTopic->stUserData.pstHead)
    {
            /* �ص������������û����� */
            if (NULL != pstDataReader->recvCallBack)
            {
				/* ������Զ�˶�Ϊ��״̬�ύ���� */
				//pstDataReader->totHistoryNum++;
#ifdef QYDDS_TIME_BASED_FILTER_QOS
                if (pstDataReader->stDataReaderQos.time_based_filter.minimum_separation.sec != 0 || pstDataReader->stDataReaderQos.time_based_filter.minimum_separation.nanosec != 0)
                {
                    Time_t nowTime = GetNowTime();
                    Duration_t timeUsed = Sub_Duration(nowTime, pstDataReader->lastRecvTime);
                    if (Compare_Duration(timeUsed, pstDataReader->stDataReaderQos.time_based_filter.minimum_separation))
                    {
                        (pstDataReader->recvCallBack)(pstDataReader);
                        pstDataReader->lastRecvTime = nowTime;
                    }
                }
                else
                {
                    (pstDataReader->recvCallBack)(pstDataReader);
                }
#else
            (pstDataReader->recvCallBack)(pstDataReader);
#endif
            }
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DeilverStatusFulUserMsgWhenExpectSeqNumUnderFirstSN
-   ��������: �ύ��״̬�Ķ������ݣ�������seqNum������firstSN��С
-   ��    ��: �Ķ�����Զ��д�������û���������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �ύ��״̬�Ķ������ݣ�������seqNum������firstSN��С
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\15       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DeilverStatusFulUserMsgWhenExpectSeqNumUnderFirstSN(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter, SequenceNumber_t firstSN)
{
    if (pstDiscWriter == NULL)
    {
        return;
    }
    if (NULL != pstDataReader->pstTopic->stUserData.pstHead)
    {
        printf("After reading the data, Please clean up the memory");
//        exit(EXIT_FAILURE);
    }

    /* ��������ֵ��С��Ϊ������Сֵ */
    pstDiscWriter->stAckNack.readerSNState.base = firstSN;

    /* ��������*/
    if (pstDataReader->pstTopic->stUserData.uiHistoryDataNum > 0)
    {
        CorrectDiscoveredWriterHistoryCache(pstDataReader, pstDiscWriter);
    }
    if (pstDataReader->pstTopic->stUserData.pstHead)
    {
        /* �ص������������û����� */
        if (NULL != pstDataReader->recvCallBack)
        {
            (pstDataReader->recvCallBack)(pstDataReader);
        }
        UninitHistoryCache(&pstDiscWriter->pstTopic->stUserData);
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_DataReader_take
-   ��������: ��ȡ�û�����
-   ��    ��: �Ķ�����DataSeq
-   ��    ��: DDS_RETCODE_OK��DDS_RETCODE_NO_DATA
-   ȫ�ֱ���:
-   ע    ��: ��ȡ�û�����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\16       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t DDS_StringDataReader_take(DDS_DataReader* pstDataReader, DataSeq* pstDataSeq)
{
	HistoryData* tmpHistoryData = NULL;
    if (NULL == pstDataReader->pstTopic->stUserData.pstHead)
    {
        return DDS_RETCODE_NO_DATA;
    }

    tmpHistoryData = pstDataReader->pstTopic->stUserData.pstHead;
    while ((tmpHistoryData->uiFragTotal > 0) && (tmpHistoryData->uiRecvFragNum != tmpHistoryData->uiFragTotal))
    {
        if (tmpHistoryData->pNext != NULL)
        {
            tmpHistoryData = tmpHistoryData->pNext;
        }
    }
    /* �������ݸ�����ֵ */
    if (strcmp(tmpHistoryData->data, "DeadLine MSG.") == 0)
    {
        pstDataSeq->length = 1;
    }
    else
    {
        pstDataSeq->length = pstDataReader->pstTopic->stUserData.uiHistoryDataNum;
    }
    /* ���������׵�ַ */
    pstDataSeq->pstHead = tmpHistoryData;

    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_DataReader_take
-   ��������: ��ȡ�û�����
-   ��    ��: �Ķ�����DataSeq
-   ��    ��: DDS_RETCODE_OK��DDS_RETCODE_NO_DATA
-   ȫ�ֱ���:
-   ע    ��: ��ȡ�û�����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\16       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DataSeq*  DDS_StringDataReader_take_cs(DDS_DataReader* pstDataReader)
{
	DataSeq* pstDataSeq = NULL;
    if (NULL == pstDataReader->pstTopic->stUserData.pstHead)
    {
        return NULL;
    }

    pstDataSeq = (DataSeq*)DDS_STATIC_MALLOC(sizeof(DataSeq));
    if (NULL == pstDataSeq)
    {
        return NULL;
    }

    /* �������ݸ�����ֵ */
    pstDataSeq->length = pstDataReader->pstTopic->stUserData.uiHistoryDataNum;

    /* ���������׵�ַ */
    pstDataSeq->pstHead = pstDataReader->pstTopic->stUserData.pstHead;

    return pstDataSeq;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_StringSeq_length
-   ��������: �����û�����
-   ��    ��: DataSeq�����ݱ��
-   ��    ��: �û�����ָ��
-   ȫ�ֱ���:
-   ע    ��: �����û�����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\16       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL UINT32 DDS_StringSeq_length(DataSeq* pstDataSeq)
{
    if (NULL == pstDataSeq)
    {
        return 0;
    }

    return pstDataSeq->length;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_StringSeq_get
-   ��������: �����û�����
-   ��    ��: DataSeq�����ݱ��
-   ��    ��: �û�����ָ��
-   ȫ�ֱ���:
-   ע    ��: �����û�����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\16       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL CHAR* DDS_StringSeq_get(DataSeq* pstDataSeq, UINT32 num, DataLen* pDataLen)
{
    UINT32 uiNum;

    HistoryData* pstHistoryData = pstDataSeq->pstHead;

    for (uiNum = 0; uiNum < pstDataSeq->length; uiNum++)
    {
        if (uiNum == num)
        {
			*pDataLen = pstHistoryData->uiDataLen;
			return pstHistoryData->data;
        }

        pstHistoryData = pstHistoryData->pNext;
    }
    return NULL;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_StringSeq_get
-   ��������: �����û�����
-   ��    ��: DataSeq�����ݱ��
-   ��    ��: �û�����ָ��
-   ȫ�ֱ���:
-   ע    ��: �����û�����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\16       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL BOOL DDS_TimeSeq_get(DataSeq* pstDataSeq, UINT32 num, Time_t* pstTimeInfo)
{
    UINT32 uiNum;

    HistoryData* pstHistoryData = pstDataSeq->pstHead;

    for (uiNum = 0; uiNum < pstDataSeq->length; uiNum++)
    {
        if (uiNum == num)
        {
            *pstTimeInfo = pstHistoryData->timeInfo;
            return TRUE;
        }

        pstHistoryData = pstHistoryData->pNext;
    }
    return FALSE;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_StringSeq_get
-   ��������: �����û�����
-   ��    ��: DataSeq�����ݱ��
-   ��    ��: �û�����ָ��
-   ȫ�ֱ���:
-   ע    ��: �����û�����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\16       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL VOID DDS_StringSeq_get_cs(DataSeq* pstDataSeq, UINT32 num,  CHAR* msg)
{
    UINT32 uiNum;

    HistoryData* pstHistoryData = pstDataSeq->pstHead;

    for (uiNum = 0; uiNum < pstDataSeq->length; uiNum++)
    {
        if (uiNum == num)
        {
            memcpy(msg, pstHistoryData->data, pstHistoryData->uiDataLen);

            return;
        }

        pstHistoryData = pstHistoryData->pNext;
    }

    return ;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_StringSeq_get
-   ��������: �����û�����
-   ��    ��: DataSeq�����ݱ��
-   ��    ��: �û�����ָ��
-   ȫ�ֱ���:
-   ע    ��: �����û�����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\16       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL UINT32 DDS_StringSeq_len_cs(DataSeq* pstDataSeq, UINT32 num)
{
    UINT32 uiNum;

    HistoryData* pstHistoryData = pstDataSeq->pstHead;

    for (uiNum = 0; uiNum < pstDataSeq->length; uiNum++)
    {
        if (uiNum == num)
        {
            return  pstHistoryData->uiDataLen;

        }

        pstHistoryData = pstHistoryData->pNext;
    }

    return 0;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_StringDataReader_return_loan
-   ��������: �û�������Դ�ͷ�
-   ��    ��: �����Ķ���
-   ��    ��: DDS_RETCODE_OK��DDS_RETCODE_NO_DATA
-   ȫ�ֱ���:
-   ע    ��: �û�������Դ�ͷ�, ��ȡ�û����ݺ��������ͷţ��������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\16       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t DDS_StringDataReader_return_loan(DDS_DataReader* pstDataReader, DataSeq* pstDataSeq)
{
	UINT32 uiNum;
	HistoryData* pstDelHistoryData = NULL;
	HistoryData* pstHistoryData = NULL;
	DDS_DataReader* tmpReader = NULL;
	/* �ύ�������ݵ�DataReader ��������Ŀ��Ϊ0 */
	pstDataReader->totHistoryNum = 0;

    tmpReader = pstDataReader->pstTopic->pstLocalReader;
	if(NULL != pstDataReader->pNext) //�����������reader��ֱ�ӷ��ز��ͷ�����
		return DDS_RETCODE_OK;
  //  while (tmpReader!=NULL)
  //  {
		///* JD 2021-12-01 �޸��ж��Ƿ����е����ݶ��ύ���ˣ���DataReader������û�н����ύ�����򷵻� */
		////if (tmpReader->totHistoryNum == 0)
  //      if (tmpReader->totHistoryNum > 0)
  //      {
  //          return DDS_RETCODE_OK;
  //      }
  //      tmpReader = tmpReader->pNext;
  //  }
	/* JD 2021-12-01 ע�� */
    //tmpReader = pstDataReader->pstTopic->pstLocalReader;
    //while (tmpReader != NULL)
    //{
    //    tmpReader->totHistoryNum--;
    //    tmpReader = tmpReader->pNext;
    //}
	/* JD 2021-12-01 ����DataReader���ύ�ˣ���ʼɾ������ */
    pstHistoryData = pstDataSeq->pstHead;

    for (uiNum = 0; uiNum < pstDataSeq->length; uiNum++)
    {
        pstDelHistoryData = pstHistoryData;

        pstHistoryData = pstHistoryData->pNext;

        /* �ͷŻ������е������� */
        UninitHistoryData(pstDelHistoryData);

        /* �ͷŻ����� */
        DDS_STATIC_FREE(pstDelHistoryData);
		pstDelHistoryData = NULL;
    }

    /* �����ʼ�� */
    InitHistoryCache(&pstDataReader->pstTopic->stUserData);

    return DDS_RETCODE_OK;
}

#if defined QYDDS_MASSDATA
/*---------------------------------------------------------------------------------
-   �� �� ��: ProcessMassData
-   ��������: ��MassData ��Ƭ���ݽ������
-   ��    ��: �����Ķ���
-   ��    ��: 
-   ȫ�ֱ���:
-   ע    ��: 
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2021\1\07       1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
void ProcessMassData(HistoryData* pstTempHistoryData, DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter)
{
	HistoryData* pstHistoryData = NULL;
	HistoryData* pstTempMassDataHistoryData = NULL;
	SequenceNumber_t RecvSeqNum = SEQUENCENUMBER_ZERO;
	SequenceNumber_t nextSeqNum = SEQUENCENUMBER_ZERO;
	HistoryData* pstDelHistoryData = NULL;
    //printf("Nephalem : RecvData SeqNum = %d \n", pstTempHistoryData->seqNum.low);
    /* Nephalem massdata ��Ƭ����Ϊ�װ�����*/
    if (ISEQUAL_SEQUENCENUMBER(pstTempHistoryData->massDataBaseSeq, pstTempHistoryData->seqNum))
    {
        /* Nephalem massdata pstTempHistoryData�����ⲿͳһ�ͷţ���������ռ�*/
        pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC_T(sizeof(HistoryData));
        if (NULL == pstHistoryData)
        {
            PrintLog(COMMLOG_ERROR, "HandleUSERWriterDataMsg:Failed to create HistoryData, Insufficient memory space.\n");
            return;
        }

        /* Nephalem ��ʼ�� */
        InitHistoryData(pstHistoryData);

        *pstHistoryData = *pstTempHistoryData;

        /* Nephalem ָ�벻���и�ֵ */
        pstHistoryData->pstFragData = NULL;
        pstHistoryData->pNext = NULL;

        pstHistoryData->massDataRecvNum ++;
        pstHistoryData->uiDataLen = pstHistoryData->massDataTotolSize;

        /* Nephalme massdata ���·�Ƭ�����ڴ�Ϊ�����ܴ�С */
        pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC_T(pstHistoryData->massDataTotolSize);
        if (NULL == pstHistoryData->data)
        {
            PrintLog(COMMLOG_ERROR, "HandleUSERWriterDataMsg:Failed to create HistoryData->data, Insufficient memory space.\n");
            return;
        }

        /* Nephalem massdata �����ݸ��Ƶ��¿ռ��� */
        memcpy(pstHistoryData->data, pstTempHistoryData->data, pstTempHistoryData->uiDataLen);

        /* Nephalem ����historyQos�������ޣ���������ʱ�ͷž����� */
        if (KEEP_LAST_HISTORY_QOS == pstDataReader->stDataReaderQos.history.kind
            && pstDiscWriter->stMassDataHistoryCache.uiHistoryDataNum >= pstDataReader->stDataReaderQos.history.depth)
        {
            pstDelHistoryData = pstDiscWriter->stMassDataHistoryCache.pstHead;
            pstDiscWriter->stMassDataHistoryCache.pstHead = pstDiscWriter->stMassDataHistoryCache.pstHead->pNext;

            DDS_STATIC_FREE(pstDelHistoryData->data);
            pstDelHistoryData->data = NULL;
            DDS_STATIC_FREE(pstDelHistoryData);
            pstDelHistoryData = NULL;
            pstDiscWriter->stMassDataHistoryCache.uiHistoryDataNum--;
        }

       //printf("Nephalem : Insert SeqNum = %d \n", pstTempHistoryData->seqNum.low);
        /* Nephalem massdata��HistoryData �����б�  */
        if (!InsertHistoryCacheOrder(&pstDiscWriter->stMassDataHistoryCache, pstHistoryData))
        {
            PrintLog(COMMLOG_INFO, "HandleUSERWriterDataMsg:User data was not inserted, Because user data already exists.\n");
        }

    }
    /* Nephalem massdata ���װ���Ƭ����*/
    else
    {
        pstTempMassDataHistoryData = pstDiscWriter->stMassDataHistoryCache.pstHead;
        while (NULL != pstTempMassDataHistoryData)
        {
            /* Nephalem massdata ���ݱ��С�ڻ���ַ���϶���Ϊ�������ݵķ�Ƭ����*/
            if (ISLESSTHEN_SEQUENCENUMBER(pstTempHistoryData->seqNum, pstTempMassDataHistoryData->seqNum))
            {
                break;
            }

            /* Nephalem massdata ��ǰ�����ڴ�����һ����Ƭ���ݵı�� */
            RecvSeqNum = SEQUENCENUMBER_ZERO;
            RecvSeqNum.low = pstTempMassDataHistoryData->massDataRecvNum;
            nextSeqNum = Add_SequenceNumber(pstTempMassDataHistoryData->massDataBaseSeq, RecvSeqNum);

            /* Nephalem massdata ��װ���� */
            if (ISEQUAL_SEQUENCENUMBER(nextSeqNum, pstTempHistoryData->seqNum))
            {
                //printf("Nephalem : Memcpy SeqNum = %d \n", pstTempHistoryData->seqNum.low);

                memcpy(pstTempMassDataHistoryData->data + (63 * 1024) * pstTempMassDataHistoryData->massDataRecvNum, pstTempHistoryData->data, pstTempHistoryData->uiDataLen);

                /* Nephalem massdata�ѽ�����������һ */
                pstTempMassDataHistoryData->massDataRecvNum++;
                break;
            }
            pstTempMassDataHistoryData = pstTempMassDataHistoryData->pNext;
        }

    }

    /* Nepalem massdata ����������ݣ���������Ƿ������� */
    if (0 < pstDiscWriter->stMassDataHistoryCache.uiHistoryDataNum &&
        pstDiscWriter->stMassDataHistoryCache.pstHead->massDataTotal == pstDiscWriter->stMassDataHistoryCache.pstHead->massDataRecvNum)
    {      
        //printf("Nephalem : notify SeqNum = %d \n", pstDiscWriter->stMassDataHistoryCache.pstHead->seqNum.low);

        pstTempMassDataHistoryData = pstDiscWriter->stMassDataHistoryCache.pstHead;
        /* Nephalem massdata �û����ݸ������� */
        pstDiscWriter->pstTopic->stUserData.uiHistoryDataNum++;
        /* Nephalme massdata �������ݴ�MassDataHistoryCache�޳� */
        pstDiscWriter->stMassDataHistoryCache.pstHead = pstDiscWriter->stMassDataHistoryCache.pstHead->pNext;
        /* Nephalem massdata MassDataHistoryCache������������1 */
        pstDiscWriter->stMassDataHistoryCache.uiHistoryDataNum--;
        /* Nephalem massdata DOUBLE_INSERT_TAIL���ƻ�ԭ�������ϵ */
        /* Nephalem massdata �ύ�����ɵ����� */
        DOUBLE_INSERT_TAIL(pstDiscWriter->pstTopic->stUserData.pstHead, pstDiscWriter->pstTopic->stUserData.pstTail, pstTempMassDataHistoryData);
    }
}

#endif
