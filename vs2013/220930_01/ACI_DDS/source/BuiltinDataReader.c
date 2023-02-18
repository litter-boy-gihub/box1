#include "../include/GlobalDefine.h"
#include <string.h>
extern int iDDSRECV;

extern int iUSERRECV;
/*---------------------------------------------------------------------------------
-   �� �� ��: InitBuiltinDataReader
-   ��������: �ڽ��Ķ�����ʼ��
-   ��    ��: �ڽ�д����
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �ڽ��Ķ�����ʼ��
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID InitBuiltinDataReader(BuiltinDataReader* pstBuiltinDataReader)
{
    if (NULL != pstBuiltinDataReader)
    {
        pstBuiltinDataReader->pstBuiltinDataWriter = NULL;
        pstBuiltinDataReader->seqNum = SEQUENCENUMBER_ZERO;
        pstBuiltinDataReader->pstParticipant = NULL;
		pstBuiltinDataReader->guid = GUID_UNKNOWN;
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: InitBuiltinDataReader
-   ��������: �ڽ��Ķ�����ʼ��
-   ��    ��: �ڽ�д����
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �ڽ��Ķ�����ʼ��
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID UnInitBuiltinDiscWriter(BuiltinDiscWriter* pstBuiltinDiscWriter)
{
    if (NULL != pstBuiltinDiscWriter)
    {
        UninitHistoryCache(&pstBuiltinDiscWriter->stHistoryCache);

        //Locator_t* pstTempNode = NULL;
        ///* ����������ַ */
        //LIST_DELETE_ALL(pstBuiltinDiscWriter->pstUnicastLocator, pstTempNode);

        ///* �����鲥��ַ */
        //LIST_DELETE_ALL(pstBuiltinDiscWriter->pstMulticastLocator, pstTempNode);
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: UnInitBuiltinDataReader
-   ��������: �ڽ��Ķ�����ʼ��
-   ��    ��: �ڽ�д����
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �ڽ��Ķ�����ʼ��
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID UnInitBuiltinDataReader(BuiltinDataReader* pstBuiltinDataReader)
{
	BuiltinDiscWriter* pstBuiltinDiscWriter = NULL;
    if (NULL != pstBuiltinDataReader)
    {
        pstBuiltinDiscWriter = pstBuiltinDataReader->pstBuiltinDataWriter;
        while (NULL != pstBuiltinDiscWriter)
        {
            pstBuiltinDataReader->pstBuiltinDataWriter = pstBuiltinDiscWriter->pNext;
            UnInitBuiltinDiscWriter(pstBuiltinDiscWriter);
            DDS_STATIC_FREE(pstBuiltinDiscWriter);
            pstBuiltinDiscWriter = pstBuiltinDataReader->pstBuiltinDataWriter;
        }
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: JointAckNackMsgSendAssignBuiltinWriter
-   ��������: ��Զ���ڽ�writer�ظ�ack����
-   ��    ��: �����ڽ��Ķ�����Զ���ڽ�д����
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��Զ���ڽ�д�����ظ�ack����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID JointAckNackMsgSendAssignBuiltinWriter(BuiltinDataReader* pstBuiltinDataReader, BuiltinDiscWriter* pstBuiltinDiscWriter)
{
    MemoryBlock stMemBlock;
    MemoryBlock stMemBlockMod;
    SubMessageHeader  stSubMsgHeader;
    InfoDestination   stInfoDest;
    CHAR buff[NETWORK_BYTE_MAX_SIZE];

    /* �����ֽ����ʼ�� */
    INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

    /* ��һ�����л�RTPS����ͷ */
    SERIALIZE_RTPS_MESSAGE_HEADER(pstBuiltinDataReader->pstParticipant->stRTPSMsgHeader, stMemBlock);

    stSubMsgHeader.submessageId = INFO_DST;
    stSubMsgHeader.submessageLength = 12;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   
    stSubMsgHeader.flags[2] = FALSE;   
    stSubMsgHeader.flags[3] = FALSE;   
    stInfoDest.guidPrefix = pstBuiltinDiscWriter->guid.prefix;

    /* �ڶ������л��ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���������л��ӱ�����Ϣ�� */
    SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

    stSubMsgHeader.submessageId = ACKNACK;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[2] = FALSE;
    stSubMsgHeader.flags[3] = FALSE;
    if (pstBuiltinDiscWriter->stAckNack.readerSNState.numBits)
    {
        stSubMsgHeader.flags[1] = FALSE;
    }
    else
    {
        stSubMsgHeader.flags[1] = TRUE;
    }

    pstBuiltinDiscWriter->stAckNack.readerId = pstBuiltinDataReader->guid.entityId;
    pstBuiltinDiscWriter->stAckNack.writerId = pstBuiltinDiscWriter->guid.entityId;
    pstBuiltinDiscWriter->stAckNack.count++;

    /* �ڶ������л��ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);
    
    if(iDDSRECV == 1)
    	printf("Nephalem Send AckMsg : base = %d,numbits = %d\n",pstBuiltinDiscWriter->stAckNack.readerSNState.base.low,pstBuiltinDiscWriter->stAckNack.readerSNState.numBits);
#if defined _DEBUG_LOG
    printf("Nephalem Send AckMsg : base = %d,numbits = %d\n",pstBuiltinDiscWriter->stAckNack.readerSNState.base.low,pstBuiltinDiscWriter->stAckNack.readerSNState.numBits);
#endif
    
//    if(pstBuiltinDiscWriter->stAckNack.readerSNState.numBits == 0)
//    	return ;
    /* ���������л��ӱ�����Ϣ�� */
    SERIALIZE_ACKNACK(pstBuiltinDiscWriter->stAckNack, stMemBlock);

    /* �ӱ�����Ϣ�峤�ȣ�������Ϣͷ */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - pstBuiltinDiscWriter->stAckNack.uiStartIndex;

    /* ���л��޸��ӱ��ĳ��� */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

    /* ����ack���� */
    //PrintBinData(stMemBlock.base,stMemBlock.writeIndex, "AckMsgSend");
    RTPSSendMsg(&stMemBlock, &pstBuiltinDataReader->pstParticipant->socketList.sedp_data_sender, pstBuiltinDiscWriter->pstUnicastLocator);
}

/*---------------------------------------------------------------------------------
-   �� �� ��: GetBuiltinReaderMissSeqNum
-   ��������: ��ȡ��ʧ�ڽ����ݰ�
-   ��    ��: �����ڽ��Ķ�����Զ���ڽ�д��������������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��ȡ��ʧ�ڽ����ݰ�
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\14       1.0         Hunter      Created
----------------------------------------------------------------------------------*/

VOID GetBuiltinReaderMissSeqNum(BuiltinDataReader* pstBuiltinDataReader, BuiltinDiscWriter* pstBuiltinDiscWriter, Heartbeat* pstHeartbeatMsg)
{
    /* HistoryCache��seqNum����ɢ�������У�expectSeqNum��lastSN����ɢ�����������У�������һ���ԣ��鲢�Ƚ�һ�μ����ҳ�����ȱʧƬ�� */
    HistoryData* pstDelHistoryData = NULL;
	SequenceNumber_t tempSN = SEQUENCENUMBER_ZERO;
	HistoryData* pstHistoryData =NULL;
    const UINT32  UINT32_BIT = 32;
    UINT32 uiNumbit = 0;

    pstBuiltinDiscWriter->stAckNack.readerSNState.numBits = 0;

    /* ���seqNUmС������ֵ��������δȱʧ */
    if (ISLESSTHEN_SEQUENCENUMBER(pstHeartbeatMsg->lastSN, pstBuiltinDiscWriter->stAckNack.readerSNState.base))
    {
        return;
    }

    /* ����С�Ļ�С����������ֵ */
    if (ISLESSTHEN_SEQUENCENUMBER(pstBuiltinDiscWriter->stAckNack.readerSNState.base, pstHeartbeatMsg->firstSN))
    {
        pstBuiltinDiscWriter->stAckNack.readerSNState.base = pstHeartbeatMsg->firstSN;
    }

    /* Զ��д��������Ϊ��ɢ������������һ���Կ����ύ�����Լ��������� */
    pstHistoryData = pstBuiltinDiscWriter->stHistoryCache.pstHead;
    while (NULL != pstHistoryData)
    {
        /* ��������С������ֵ��ɾ������ */
        if (ISLESSTHEN_SEQUENCENUMBER(pstHistoryData->seqNum, pstBuiltinDiscWriter->stAckNack.readerSNState.base))
        {
            pstBuiltinDiscWriter->stHistoryCache.pstHead = pstHistoryData->pNext;

            /* ���ʱע��β�ڵ� */
            if (pstHistoryData == pstBuiltinDiscWriter->stHistoryCache.pstTail)
            {
                pstBuiltinDiscWriter->stHistoryCache.pstTail = NULL;
            }
            pstDelHistoryData = pstHistoryData;
        }
        /* �������ݵ�������ֵ��ɾ�����沢�����´�������seqNum */
        else if (ISEQUAL_SEQUENCENUMBER(pstHistoryData->seqNum, pstBuiltinDiscWriter->stAckNack.readerSNState.base))
        {
            pstBuiltinDiscWriter->stHistoryCache.pstHead = pstHistoryData->pNext;

            /* ���ʱע��β�ڵ� */
            if (pstHistoryData == pstBuiltinDiscWriter->stHistoryCache.pstTail)
            {
                pstBuiltinDiscWriter->stHistoryCache.pstTail = NULL;
            }
            pstDelHistoryData = pstHistoryData;
            pstBuiltinDiscWriter->stAckNack.readerSNState.base = Add_SequenceNumber(pstBuiltinDiscWriter->stAckNack.readerSNState.base, SEQUENCENUMBER_START);
        }
        /* �������ݴ�������ֵ����ɢ��������������ֱ������ */
        else
        {
            break;
        }
        pstHistoryData = pstHistoryData->pNext;

        /* �����¸��ڵ���ͷ��ϸ��ڵ��ڴ� */
        DDS_STATIC_FREE(pstDelHistoryData);
    }

    /* ���seqNUmС������ֵ��������δȱʧ */
    if (ISLESSTHEN_SEQUENCENUMBER(pstHeartbeatMsg->lastSN, pstBuiltinDiscWriter->stAckNack.readerSNState.base))
    {
        return;
    }

    /* ������գ�ÿһλ����ʾ��ǰ����Ƿ�ȱʧ */
    //memset(pstBuiltinDiscWriter->stAckNack.readerSNState.bitMap, 0xFFFFFFFF, SEQUENCE_BIT_NUMBER);
    
    memset(pstBuiltinDiscWriter->stAckNack.readerSNState.bitMap, 0, SEQUENCE_BIT_NUMBER);

    pstHistoryData = pstBuiltinDiscWriter->stHistoryCache.pstHead;

    tempSN = pstBuiltinDiscWriter->stAckNack.readerSNState.base;
    
    while (!ISEQUAL_SEQUENCENUMBER(tempSN, Add_SequenceNumber(pstHeartbeatMsg->lastSN, SEQUENCENUMBER_START)))
    {
        if (NULL != pstHistoryData)
        {
            /* ���������������ݣ����δȱʧ */
            if (ISEQUAL_SEQUENCENUMBER(pstHistoryData->seqNum, tempSN))
            {
                pstHistoryData = pstHistoryData->pNext;
            }
            /* ������Ҳû���������ݣ���¼ȱʧ */
            else
            {
                /* ÿһλ��¼�Ƿ���ȱʧ����base��ʼ�������¼����256���Ƿ���ȱʧ */
                pstBuiltinDiscWriter->stAckNack.readerSNState.bitMap[uiNumbit >> 0x05] |= (0x80000000 >> (uiNumbit & 0x1f));
            }
        }
        else
        {
            /* ��������꣬ʣ��ȫ��ȱʧ����¼ȱʧ */
            pstBuiltinDiscWriter->stAckNack.readerSNState.bitMap[uiNumbit >> 0x05] |= (0x80000000 >> (uiNumbit & 0x1f));
        }

        pstBuiltinDiscWriter->stAckNack.readerSNState.numBits = uiNumbit + 1;
        
        if(pstBuiltinDiscWriter->stAckNack.readerSNState.numBits >= 3)
        	pstBuiltinDiscWriter->stAckNack.readerSNState.numBits = 3;
        
        //printf("readerSNState.base = %d numBits = %d \n",pstBuiltinDiscWriter->stAckNack.readerSNState.base.low,pstBuiltinDiscWriter->stAckNack.readerSNState.numBits);

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
-   �� �� ��: BuiltinDataReaderInsertDiscDataWriter
-   ��������: �ڽ��Ķ���������Զ��д����
-   ��    ��: �ڽ��Ķ�����Զ��guid����ַ
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �ڽ��Ķ���������Զ��д����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL BuiltinDataReaderInsertDiscDataWriter(BuiltinDataReader* pstBuiltinDataReader, GUID_t* pstGuid, Locator_t* pstUnicastLocator)
{
    UINT32 uiHeartNum = 0;
	BuiltinDiscWriter*  pstBuiltinDiscWriter = NULL;
    if (NULL != pstBuiltinDataReader)
    {
        /* ���뾲̬�ڴ洴���ڽ�Զ���Ķ��� */
        pstBuiltinDiscWriter = (BuiltinDiscWriter*)DDS_STATIC_MALLOC(sizeof(BuiltinDiscWriter));
        if (NULL == pstBuiltinDiscWriter)
        {
            PrintLog(COMMLOG_ERROR, "BuiltinDiscReader: Failed to create BuiltinDiscWriter, Insufficient memory space.\n");
            return FALSE;
        }

        pstBuiltinDiscWriter->guid = *pstGuid;
        pstBuiltinDiscWriter->stHeartbeat.count = 0;
        pstBuiltinDiscWriter->stAckNack.count = 0;
        pstBuiltinDiscWriter->stAckNack.readerSNState.base = SEQUENCENUMBER_ZERO;

        /*  numBits�����㣬��ʹflags[1]Ϊfalse��ǿ�ƶԶ˷����������� */
        pstBuiltinDiscWriter->stAckNack.readerSNState.numBits = 1;
        pstBuiltinDiscWriter->pstUnicastLocator = pstUnicastLocator;

        /* ��ʷ�����ʼ�� */
        InitHistoryCache(&pstBuiltinDiscWriter->stHistoryCache);

        /* ����ͷ�� */
        LIST_INSERT_HEAD(pstBuiltinDataReader->pstBuiltinDataWriter, pstBuiltinDiscWriter);

        /* �ظ�ack�öԷ�ǿ�Ʒ�������������У��ȱʧ */
        memset(pstBuiltinDiscWriter->stAckNack.readerSNState.bitMap, 0, 4);

		/* Nephalem ����֤���Է�����ǿ�Ʒ����������� */
       JointAckNackMsgSendAssignBuiltinWriter(pstBuiltinDataReader, pstBuiltinDiscWriter);
    }

    return TRUE;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: GetBuiltinDiscWriterFromBuiltinDataReaderByPrefix
-   ��������: ����Զ���ڽ�д����
-   ��    ��: �ڽ�д������GuidPrefix_t
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ����Զ���ڽ�д����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BuiltinDiscWriter* GetBuiltinDiscWriterFromBuiltinDataReaderByPrefix(BuiltinDataReader* pstBuiltinDataReader, GuidPrefix_t* pstPrefix)
{
    BuiltinDiscWriter* pstBuiltinDiscWriter = pstBuiltinDataReader->pstBuiltinDataWriter;

    while (NULL != pstBuiltinDiscWriter)
    {
        if (GuidPrefix_Is_Equal(&pstBuiltinDiscWriter->guid.prefix, pstPrefix))
        {
            return pstBuiltinDiscWriter;
        }
        pstBuiltinDiscWriter = pstBuiltinDiscWriter->pNext;
    }

    return NULL;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DeleteBuiltinDiscWriterFromBuiltinReaderByGuidPrefix
-   ��������: ɾ��Զ���ڽ�д����
-   ��    ��: ���⡢Guid
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ɾ��Զ���ڽ�д����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DeleteBuiltinDiscWriterFromBuiltinReaderByGuidPrefix(BuiltinDataReader* pstBuiltinDataReader, GuidPrefix_t*  pstPrefix)
{
    BuiltinDiscWriter*  pstPrevBuiltinDataWriter = NULL;
    BuiltinDiscWriter*  pstBuiltinDataWriter = pstBuiltinDataReader->pstBuiltinDataWriter;

    while (NULL != pstBuiltinDataWriter)
    {
        if (GuidPrefix_Is_Equal(&pstBuiltinDataWriter->guid.prefix, pstPrefix))
        {
            if (!pstPrevBuiltinDataWriter)
            {
                pstBuiltinDataWriter = pstBuiltinDataWriter->pNext;
                DDS_STATIC_FREE(pstBuiltinDataReader->pstBuiltinDataWriter);
                pstBuiltinDataReader->pstBuiltinDataWriter = pstBuiltinDataWriter;
            }
            else
            {
                pstPrevBuiltinDataWriter->pNext = pstBuiltinDataWriter->pNext;
                DDS_STATIC_FREE(pstBuiltinDataWriter);
                pstBuiltinDataWriter = pstPrevBuiltinDataWriter->pNext;
            }
        }
        else
        {
            pstPrevBuiltinDataWriter = pstBuiltinDataWriter;
            pstBuiltinDataWriter = pstBuiltinDataWriter->pNext;
        }
    }
}
