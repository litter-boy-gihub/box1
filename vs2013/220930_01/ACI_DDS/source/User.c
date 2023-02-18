#include "../include/GlobalDefine.h"

#define DATA_SIZE                   (64 * 1024)       /* ��Ƭ������ */
#ifdef _FC
#define FRAG_SIZE                   (64 * 1024)   /* �û����ݸ����� */
#else
#define FRAG_SIZE                   (64 * 1024 - 1024)   /* �û����ݸ����� */
#endif


extern int iWriteLog;
/*---------------------------------------------------------------------------------
-   �� �� ��: JointUSERDataMsgSendUnknownReader
-   ��������: ��װ�û����ݱ��ķ���δָ���Ķ���
-   ��    ��: ����д����������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��װ�û����ݷ��ͱ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_ReturnCode_t JointUSERDataMsgSendUnknownReader(DDS_DataWriter* pstDataWriter, HistoryData* pstHistoryData)
{
    DDS_DiscoveredReader* pstDiscReader;
    MemoryBlock           stMemBlock;
    MemoryBlock           stMemBlockMod;
    SubMessageHeader      stSubMsgHeader;
    ParameterHead         stParamHead;
    InfoTimestamp         stInfoTS;
    InfoDestination       stInfoDest;
    DataFrag              stDataFrag;
    Data                  stData;
    CHAR                  cUserBuffer[DATA_SIZE];

    DDS_DomainParticipant* pstParticipant = pstDataWriter->pstTopic->pstParticipant;

	if(1 == iWriteLog)
	{
		printf("Nephalem : JointUSERDataMsgSendUnknownReader ");
		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
	}
	
	pstDiscReader = pstDataWriter->pstTopic->pstDiscReader;
	if (pstDiscReader == NULL)
	{
		if(1 == iWriteLog)
		{
			printf("Nephalem : DDS_RETCODE_NO_AVAILABLE_READER ");
			printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
		}
		
		return DDS_RETCODE_NO_AVAILABLE_READER;
	}

    /* ��Ƭ���޸� */
    /* �Ƿ���Ҫ��Ƭ���� */
    if (pstHistoryData->uiDataLen > FRAG_SIZE)
    {
        /* �����ֽ����ʼ�������淢������ */
        INITIAL_MEMORYBLOCK(stMemBlock, cUserBuffer, DATA_SIZE);

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
        stInfoTS.value = pstHistoryData->timeInfo;
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
        stInfoDest.guidPrefix = GUIDPREFIX_UNKNOWN;
        /* �ڶ������л��ӱ���ͷ */
        SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);
        /* ���������л��ӱ�����Ϣ�� */
        SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

        /* �û����ݱ���ͷ���� */
        stSubMsgHeader.submessageId = DATA_FRAG;
		if (0 == IS_MACHINE_LITTLE_ENDIAN)
			stSubMsgHeader.flags[0] = FALSE;    //littleEndian
		else
			stSubMsgHeader.flags[0] = TRUE;    //littleEndian
        stSubMsgHeader.flags[1] = TRUE;    //inlineQosFlag
        stSubMsgHeader.flags[2] = TRUE;    //dataFlag
        stSubMsgHeader.flags[3] = FALSE;   //keyFlag

        /* �û����ݱ��������� */
        stDataFrag.extraFlags[0] = 0x00;
        stDataFrag.extraFlags[1] = 0x00;
        stDataFrag.octetsToInlineQos = 0x0010;
        stDataFrag.readerId = ENTITYID_UNKNOWN;
        stDataFrag.writerId = pstDataWriter->guid.entityId;
        stDataFrag.writerSN = pstHistoryData->seqNum;
        
		if (IS_MACHINE_LITTLE_ENDIAN)
			stDataFrag.serializedPayload.m_cdrType = CDR_LE;    //littleEndian
		else
			stDataFrag.serializedPayload.m_cdrType = CDR_BE;    //littleEndian
        stDataFrag.serializedPayload.m_option = 0x0000;
        stDataFrag.fragmentStartingNum = pstHistoryData->pstFragData->usFragIndex;
        stDataFrag.fragmentsInSubmessage = 1;
        stDataFrag.dataSize = pstHistoryData->uiDataLen;
        stDataFrag.fragmentSize = pstHistoryData->uiFragLen;

        /* ���л��û����ݱ���ͷ */
        SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

        /* ���л��û����ݱ����� */
        SERIALIZE_DATAFRAG(stDataFrag, stMemBlock);

        /* ���л������б� */
        stParamHead.paramType = PID_KEY_HASH;
        stParamHead.length = sizeof(GUID_t);
        SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);
        SERIALIZE_GUID(pstDataWriter->guid, stMemBlock);

        /* ���л������б��β */
        stParamHead.paramType = PID_SENTINEL;
        stParamHead.length = 0;
        SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);

        /* ���л�����Ч�غ� */
        SERIALIZE_SERIALIZEDDATA(stDataFrag.serializedPayload, stMemBlock);

        /*for (int i = 0; i < pstHistoryData->uiFragLen; i++)
        {
            printf("%c", pstHistoryData->pstFragData[i]);
        }
        printf("\n");*/
        /* ���л��û��������� */
        PUT_BYTE_ARRAY(stMemBlock, pstHistoryData->pstFragData->data, pstHistoryData->pstFragData->usDataLen);

        /* �����ӱ�����Ϣ�峤�ȣ�������Ϣͷ */
        stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stDataFrag.uiStartIndex;
        
        //printf("seqNum: %u, submsgLength: %u\n", stDataFrag.writerSN.low, stSubMsgHeader.submessageLength);
        /* ���л��޸��ӱ��ĳ��� */
        INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

        PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

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

            if (!pstParticipant->bLoopBack && GuidPrefix_Is_Equal(&pstParticipant->stRTPSMsgHeader.guidPrefix, &pstDiscReader->guid.prefix))
            {
                pstDiscReader = pstDiscReader->pNext;
                continue;
            }

            /* �޸�Ŀ�ĵ�GuidPrefix */
            INITIAL_MEMORYBLOCK(stMemBlockMod, stInfoDest.pcGuidPre, sizeof(GuidPrefix_t));
            SERIALIZE_GUID_PREFIX(pstDiscReader->guid.prefix, stMemBlockMod);

            ///* �޸�Ŀ�ĵ�GuidEntityId */
            //INITIAL_MEMORYBLOCK(stMemBlockMod, stData.pcReaderId, sizeof(EntityId_t));
            //SERIALIZE_ENTITYID(pstDiscReader->guid.entityId, stMemBlockMod);

            /* ���͵���Ӧ��ÿ��Զ���Ķ��� */
            DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
            
            RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscReader->pstUnicastLocator);

            /* Nephalem ���з�Ƭ���ݷ����꣬����HeartbetFrag����ȷ�� */
            if (pstHistoryData->uiFragTotal == pstHistoryData->uiFragNum)
            {
#if !defined(NO_HEARTBEAT)
                /* Nephalem ��ʱpstHistoryData��δHistoryCache�������ڴ˽��������� */
                JointHeartbeatFragMsgSendAssignReader(pstHistoryData, pstDataWriter, pstDiscReader);
#endif
            }

            if (pstDiscReader->heartMsgNum >= pstDataWriter->stDataWriterQos.controller.controller.pack_per_herat)
            {
#if !defined(NO_HEARTBEAT)
                JointHeartbeatMsgSendAssignReader(pstDataWriter, pstDiscReader);
#endif
                pstDiscReader->heartMsgNum = 0;
                pstDiscReader->lastHeartTime = GetNowTime();
            }
            pstDiscReader->heartMsgNum++;
            DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
            pstDiscReader = pstDiscReader->pNext;
        }
        return DDS_RETCODE_OK;
    }
    else
    {
    	if(1 == iWriteLog)
    	{
    		printf("Nephalem : SERIALIZE_RTPS_MESSAGE ");
    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
    	}
    	
        /* �����ֽ����ʼ�������淢������ */
        INITIAL_MEMORYBLOCK(stMemBlock, cUserBuffer, DATA_SIZE);
        
    	if(1 == iWriteLog)
    	{
    		printf("Nephalem : INITIAL_MEMORYBLOCK ");
    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
    		printf("totalSize = %d writeIndex = %d \n",stMemBlock.totalSize,stMemBlock.writeIndex);
    	}

        /* ��һ�����л�RTPS����ͷ */
        SERIALIZE_RTPS_MESSAGE_HEADER(pstParticipant->stRTPSMsgHeader, stMemBlock);
        
    	if(1 == iWriteLog)
    	{
    		printf("Nephalem : SERIALIZE_RTPS_MESSAGE_HEADER ");
    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
    		printf("totalSize = %d writeIndex = %d \n",stMemBlock.totalSize,stMemBlock.writeIndex);
    	}

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
        stInfoTS.value = pstHistoryData->timeInfo;
        
    	if(1 == iWriteLog)
    	{
    		printf("Nephalem : SERIALIZE_SUBMESSAGEHEADER ");
    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
    		printf("totalSize = %d writeIndex = %d \n",stMemBlock.totalSize,stMemBlock.writeIndex);
    	}
        /* ���л�ʱ�䱨��ͷ */
        SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);
        /* ���л�ʱ���ӱ����� */
        SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);
        
    	if(1 == iWriteLog)
    	{
    		printf("Nephalem : SERIALIZE_INFOTIMESTAMP ");
    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
    		printf("totalSize = %d writeIndex = %d \n",stMemBlock.totalSize,stMemBlock.writeIndex);
    	}

        stSubMsgHeader.submessageId = INFO_DST;
        stSubMsgHeader.submessageLength = 12;
		if (0 == IS_MACHINE_LITTLE_ENDIAN)
			stSubMsgHeader.flags[0] = FALSE;    //littleEndian
		else
			stSubMsgHeader.flags[0] = TRUE;    //littleEndian
        stSubMsgHeader.flags[1] = FALSE;
        stSubMsgHeader.flags[2] = FALSE;
        stSubMsgHeader.flags[3] = FALSE;
        stInfoDest.guidPrefix = GUIDPREFIX_UNKNOWN;
        
    	if(1 == iWriteLog)
    	{
    		printf("Nephalem : SERIALIZE_SUBMESSAGEHEADER ");
    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
    		printf("totalSize = %d writeIndex = %d \n",stMemBlock.totalSize,stMemBlock.writeIndex);
    	}
    	
        /* �ڶ������л��ӱ���ͷ */
        SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);
        /* ���������л��ӱ�����Ϣ�� */
        SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);
        
    	if(1 == iWriteLog)
    	{
    		printf("Nephalem : SERIALIZE_INFODESTINATION ");
    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
    		printf("totalSize = %d writeIndex = %d \n",stMemBlock.totalSize,stMemBlock.writeIndex);
    	}

        /* �û����ݱ���ͷ���� */
        stSubMsgHeader.submessageId = DATA;
		if (0 == IS_MACHINE_LITTLE_ENDIAN)
			stSubMsgHeader.flags[0] = FALSE;    //littleEndian
		else
			stSubMsgHeader.flags[0] = TRUE;    //littleEndian
        stSubMsgHeader.flags[1] = FALSE;    //inlineQosFlag
        stSubMsgHeader.flags[2] = TRUE;    //dataFlag
        stSubMsgHeader.flags[3] = FALSE;   //keyFlag

        /* �û����ݱ��������� */
        stData.extraFlags[0] = 0x00;
        stData.extraFlags[1] = 0x00;
        stData.octetsToInlineQos = 0x0010;
        stData.readerId = ENTITYID_UNKNOWN;
        stData.writerId = pstDataWriter->guid.entityId;
        stData.writerSN = pstHistoryData->seqNum;
		if (IS_MACHINE_LITTLE_ENDIAN)
			stData.serializedPayload.m_cdrType = CDR_LE;  //С���û���������  
		else
			stData.serializedPayload.m_cdrType = CDR_BE;  //С���û���������    
        stData.serializedPayload.m_option = 0x0000;

    	if(1 == iWriteLog)
    	{
    		printf("Nephalem : SERIALIZE_SUBMESSAGEHEADER ");
    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
    		printf("totalSize = %d writeIndex = %d \n",stMemBlock.totalSize,stMemBlock.writeIndex);
    	}
    	
        /* ���л��û����ݱ���ͷ */
        SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);
        
    	if(1 == iWriteLog)
    	{
    		printf("Nephalem : SERIALIZE_SUBMESSAGEHEADER ");
    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
    	}

        /* ���л��û����ݱ����� */
        SERIALIZE_DATA(stData, stMemBlock);
        
//    	if(1 == iWriteLog)
//    	{
//    		printf("Nephalem : SERIALIZE_DATA ");
//    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
//    	}

//        /* ���л������б� */
//        stParamHead.paramType = PID_KEY_HASH;
//        stParamHead.length = sizeof(GUID_t);
//        SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);
//        SERIALIZE_GUID(pstDataWriter->guid, stMemBlock);
//
//        /* ���л������б��β */
//        stParamHead.paramType = PID_SENTINEL;
//        stParamHead.length = 0;
//        SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);

        /* ���л�����Ч�غ� */
        SERIALIZE_SERIALIZEDDATA(stData.serializedPayload, stMemBlock);
        
//    	if(1 == iWriteLog)
//    	{
//    		printf("Nephalem : SERIALIZE_SERIALIZEDDATA ");
//    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
//    	}

#if defined QYDDS_MASSDATA
        /* Nephalem massdata���л���Ƭ������Ϣ*/
        if (pstHistoryData->massDataTotal != 0)
        {
            /* Nephalem massdata���л���Ƭ���ݱ�־ */
            PUT_BYTE_ARRAY(stMemBlock, "MASSDATA", 9);
            /* Nephalem massdata���л���Ƭ���ݻ����*/
            SERIALIZE_SEQUENCENUMBER(pstHistoryData->massDataBaseSeq, stMemBlock);
            /* Nephalem massdata���л���Ƭ����*/
            PUT_UNSIGNED_INT(stMemBlock, pstHistoryData->massDataTotal);
            /* Nephalem massdata���л���Ƭ�����ܴ�С*/
            PUT_UNSIGNED_INT(stMemBlock, pstHistoryData->massDataTotolSize);
        }
#endif
        /* ���л��û��������� */
        PUT_BYTE_ARRAY(stMemBlock, pstHistoryData->data, pstHistoryData->uiDataLen);
        
        if (pstHistoryData->uiDataLen & 3)
        {
        	MEMORYBLOCK_SKIP_WRITE(stMemBlock, (4 - (pstHistoryData->uiDataLen & 3)));
        }

//    	if(1 == iWriteLog)
//    	{
//    		printf("Nephalem : PUT_BYTE_ARRAY ");
//    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
//    	}

        /* �����ӱ�����Ϣ�峤�ȣ�������Ϣͷ */
        stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stData.uiStartIndex;

//    	if(1 == iWriteLog)
//    	{
//    		printf("Nephalem : INITIAL_MEMORYBLOCK ");
//    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
//    	}
    	
        /* ���л��޸��ӱ��ĳ��� */
        INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));
 
        PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);
        
//    	if(1 == iWriteLog)
//    	{
//    		printf("Nephalem : PUT_UNSIGNED_SHORT ");
//    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
//    	}

        while (NULL != pstDiscReader)
        {

//        	if(1 == iWriteLog)
//        	{
//        		printf("Nephalem : NULL != pstDiscReader ");
//        		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
//        	}
            /* Nephalem partition ����ͬһ���������� */
            if (FALSE == pstDiscReader->bSamePartiton)
            {
//            	if(1 == iWriteLog)
//            	{
//            		printf("Nephalem : not Same partition ");
//            		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
//            	}
            	
                pstDiscReader = pstDiscReader->pNext;
                continue;
            }

            if (BEST_EFFORT_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind && RELIABLE_RELIABILITY_QOS == pstDiscReader->stDataReaderQos.reliability.kind)
            {
//            	if(1 == iWriteLog)
//            	{
//            		printf("Reader and writer RELIABILITY_QOS do not match. ");
//            		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
//            	}
                PrintLog(COMMLOG_WARN, "Reader and writer RELIABILITY_QOS do not match.\n");
                pstDiscReader = pstDiscReader->pNext;
                continue;
            }

            if (!pstParticipant->bLoopBack && GuidPrefix_Is_Equal(&pstParticipant->stRTPSMsgHeader.guidPrefix, &pstDiscReader->guid.prefix))
            {
//            	if(1 == iWriteLog)
//            	{
//            		printf("pstParticipant->bLoopBack ");
//            		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
//            	}
                pstDiscReader = pstDiscReader->pNext;
                continue;
            }

            /* �޸�Ŀ�ĵ�GuidPrefix */
            INITIAL_MEMORYBLOCK(stMemBlockMod, stInfoDest.pcGuidPre, sizeof(GuidPrefix_t));
            SERIALIZE_GUID_PREFIX(pstDiscReader->guid.prefix, stMemBlockMod);

            /* �޸�Ŀ�ĵ�GuidEntityId */
            INITIAL_MEMORYBLOCK(stMemBlockMod, stData.pcReaderId, sizeof(EntityId_t));
            SERIALIZE_ENTITYID(pstDiscReader->guid.entityId, stMemBlockMod);

//			PrintBinData(stMemBlock.base, stMemBlock.writeIndex, "Send User Data");

            /* ���͵���Ӧ��ÿ��Զ���Ķ��� */
            DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
            
			RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscReader->pstUnicastLocator);
            pstDiscReader->heartMsgNum++;
            if (pstDiscReader->heartMsgNum >= pstDataWriter->stDataWriterQos.controller.controller.pack_per_herat)
            {
#if !defined(NO_HEARTBEAT)
                JointHeartbeatMsgSendAssignReader(pstDataWriter, pstDiscReader);
#endif
                pstDiscReader->heartMsgNum = 0;
                pstDiscReader->lastHeartTime = GetNowTime();
            }
            DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
            pstDiscReader = pstDiscReader->pNext;
        }
        return DDS_RETCODE_OK;
#ifdef DDS_MONITOR
        /* ���ͼ����Ϣ */
        DDS_MUTEX_LOCK(pstParticipant->monMutex);
        UserMsgSendMonitor(pstParticipant, &pstDataWriter->pstTopic->tpGuid, &stMemBlock, 0x00);
        DDS_MUTEX_UNLOCK(pstParticipant->monMutex);
#endif

    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: JointUSERDataMsgSendAssignReader
-   ��������: ��װ�û����ݱ��ķ���ָ���Ķ���
-   ��    ��: ����д����������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��װ�û����ݱ��ķ���ָ���Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID JointUSERDataMsgSendAssignReader(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader*  pstDiscReader, HistoryData* pstHistoryData)
{
    MemoryBlock           stMemBlock;
    MemoryBlock           stMemBlockMod;
    SubMessageHeader      stSubMsgHeader;
    InfoTimestamp         stInfoTS;
    ParameterHead         stParamHead;
    DataFrag              stDataFrag;
    InfoDestination       stInfoDest;
    Data                  stData;
    USHORT                usFragSize;
    UINT32                uiFragNum;
    CHAR                  cUserBuffer[DATA_SIZE];
    UINT32                uiFragTotal;

    DDS_DomainParticipant* pstParticipant = pstDataWriter->pstTopic->pstParticipant;

    /* �Ƿ���Ҫ��Ƭ���� */
    if (pstHistoryData->uiDataLen > FRAG_SIZE)
    {
        /* �����Ƭ���� */
        if (0 < pstHistoryData->uiDataLen % FRAG_SIZE)
        {
            uiFragTotal = (pstHistoryData->uiDataLen / FRAG_SIZE) + 1;
        }
        else
        {
            uiFragTotal = (pstHistoryData->uiDataLen / FRAG_SIZE);
        }

        for (uiFragNum = 0; uiFragNum < uiFragTotal; uiFragNum++)
        {
            /* ���㵱ǰ��Ƭ���� */
            if (uiFragTotal - 1 == uiFragNum)
            {
                usFragSize = pstHistoryData->uiDataLen - (uiFragNum * FRAG_SIZE);
            }
            else
            {
                usFragSize = FRAG_SIZE;
            }
            /* �����ֽ����ʼ�������淢������ */
            INITIAL_MEMORYBLOCK(stMemBlock, cUserBuffer, DATA_SIZE);

            /* ��һ�����л�RTPS����ͷ */
            SERIALIZE_RTPS_MESSAGE_HEADER(pstParticipant->stRTPSMsgHeader, stMemBlock);

			if (0 == IS_MACHINE_LITTLE_ENDIAN)
				stSubMsgHeader.flags[0] = FALSE;    //littleEndian
			else
				stSubMsgHeader.flags[0] = TRUE;    //littleEndian
            stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
            stSubMsgHeader.flags[2] = TRUE;    //dataFlag
            stSubMsgHeader.flags[3] = FALSE;   //keyFlag

            /* ���л�ʱ�� */
            stSubMsgHeader.submessageId = INFO_TS;
            stSubMsgHeader.submessageLength = sizeof(Time_t);
            stInfoTS.value = pstHistoryData->timeInfo;

            /* ���л�ʱ���ӱ���ͷ */
            SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

            /* ���л�ʱ���ӱ�����Ϣ�� */
            SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

            /* ���л��û����� */
            stSubMsgHeader.submessageId = DATA_FRAG;
            stDataFrag.extraFlags[0] = 0x00;
            stDataFrag.extraFlags[1] = 0x00;
            stDataFrag.octetsToInlineQos = 0x0010;
            stDataFrag.readerId = pstDiscReader->guid.entityId;;
            stDataFrag.writerId = pstDataWriter->guid.entityId;
            stDataFrag.writerSN = pstHistoryData->seqNum;
            stDataFrag.fragmentStartingNum = uiFragNum + 1;
            stDataFrag.fragmentsInSubmessage = 1;
            stDataFrag.dataSize = pstHistoryData->uiDataLen;
            stDataFrag.fragmentSize = FRAG_SIZE;

            /* ���л��û������ӱ���ͷ */
            SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

            /* ���л��û������ӱ�����Ϣ�壬ͬʱ̽���¼readerIdλ�� */
            SERIALIZE_DATAFRAG(stDataFrag, stMemBlock);

            /* dataFlag����keyFlagΪTRUE��������� */
            SERIALIZE_SERIALIZEDDATA(stDataFrag.serializedPayload, stMemBlock);

            /* ���л��û��������� */
            PUT_BYTE_ARRAY(stMemBlock, pstHistoryData->data + (uiFragNum * FRAG_SIZE), usFragSize);

            /* �����ӱ�����Ϣ�峤�ȣ�������Ϣͷ */
            stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stDataFrag.uiStartIndex;

            /* ���л��޸��ӱ��ĳ��� */
            INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

            PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);
            //printf("Nepahlem : Resend fragmentStartingNum = %d writerSN = %d \n", stDataFrag.fragmentStartingNum, stDataFrag.writerSN.low);
            /* ���͵���Ӧ��ÿ��Զ���Ķ��� */
            RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscReader->pstUnicastLocator);
        }
    }
    else
    {
        /* �����ֽ����ʼ�������淢������ */
        INITIAL_MEMORYBLOCK(stMemBlock, cUserBuffer, DATA_SIZE);

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
        stInfoTS.value = pstHistoryData->timeInfo;

        /* ���л�ʱ�䱨��ͷ */
        SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

        /* ���л�ʱ���ӱ����� */
        SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

        /* InfoDT���� */
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

        /* �û����ݱ���ͷ���� */
        stSubMsgHeader.submessageId = DATA;
		if (0 == IS_MACHINE_LITTLE_ENDIAN)
			stSubMsgHeader.flags[0] = FALSE;    //littleEndian
		else
			stSubMsgHeader.flags[0] = TRUE;    //littleEndian
        stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
        stSubMsgHeader.flags[2] = TRUE;    //dataFlag
        stSubMsgHeader.flags[3] = FALSE;   //keyFlag

        /* �û����ݱ��������� */
        stData.extraFlags[0] = 0x00;
        stData.extraFlags[1] = 0x00;
        stData.octetsToInlineQos = 0x0010;
        stData.readerId = pstDiscReader->guid.entityId;
        stData.writerId = pstDataWriter->guid.entityId;
        stData.writerSN = pstHistoryData->seqNum;
		if (0 == IS_MACHINE_LITTLE_ENDIAN)
			stData.serializedPayload.m_cdrType = CDR_LE;  //С���û���������
		else
			stData.serializedPayload.m_cdrType = CDR_BE;  //С���û���������
        stData.serializedPayload.m_option = 0x0000;

        /* ���л��û����ݱ���ͷ */
        SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

        /* ���л��û����ݱ����� */
        SERIALIZE_DATA(stData, stMemBlock);

//        /* ���л������б� */
//        stParamHead.paramType = PID_KEY_HASH;
//        stParamHead.length = sizeof(GUID_t);
//        SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);
//        SERIALIZE_GUID(pstDataWriter->guid, stMemBlock);
//
//        /* ���л������б��β */
//        stParamHead.paramType = PID_SENTINEL;
//        stParamHead.length = 0;
//        SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);

        /* ���л�����Ч�غ� */
        SERIALIZE_SERIALIZEDDATA(stData.serializedPayload, stMemBlock);

#if defined QYDDS_MASSDATA
        /* Nephalem massdata���л���Ƭ������Ϣ*/
        if (pstHistoryData->massDataTotal != 0)
        {
            /* Nephalem massdata���л���Ƭ���ݱ�־ */
            PUT_BYTE_ARRAY(stMemBlock, "MASSDATA", 9);
            /* Nephalem massdata���л���Ƭ���ݻ����*/
            SERIALIZE_SEQUENCENUMBER(pstHistoryData->massDataBaseSeq, stMemBlock);
            /* Nephalem massdata���л���Ƭ����*/
            PUT_UNSIGNED_INT(stMemBlock, pstHistoryData->massDataTotal);
            /* Nephalem massdata���л���Ƭ�����ܴ�С*/
            PUT_UNSIGNED_INT(stMemBlock, pstHistoryData->massDataTotolSize);
        }
#endif
        /* ���л��û��������� */
        PUT_BYTE_ARRAY(stMemBlock, pstHistoryData->data, pstHistoryData->uiDataLen);

        /* �����ӱ�����Ϣ�峤�ȣ�������Ϣͷ */
        stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stData.uiStartIndex;

        /* ���л��޸��ӱ��ĳ��� */
        INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

        PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

        if (BEST_EFFORT_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind && RELIABLE_RELIABILITY_QOS == pstDiscReader->stDataReaderQos.reliability.kind)
        {
            PrintLog(COMMLOG_WARN, "Reader and writer RELIABILITY_QOS do not match.\n");
            return;
        }

        if (!pstParticipant->bLoopBack && GuidPrefix_Is_Equal(&pstParticipant->stRTPSMsgHeader.guidPrefix, &pstDiscReader->guid.prefix))
        {
            return;
        }
        
        PrintBinData(stMemBlock.base,stMemBlock.writeIndex, "ReSend DataMsg");

        /* ���͵���Ӧ��ÿ��Զ���Ķ��� */
        RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscReader->pstUnicastLocator);
    }
}


/*---------------------------------------------------------------------------------
-   �� �� ��: JointUSERDataFragMsgSendAssignReader
-   ��������: ��װָ���û�����DataFrag���ķ���ָ���Ķ���
-   ��    ��: ����д����������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: 
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2021\4\14       1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
VOID JointUSERDataFragMsgSendAssignReader(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader*  pstDiscReader, HistoryData* pstHistoryData, UINT32 uiFragNum)
{
    MemoryBlock           stMemBlock;
    MemoryBlock           stMemBlockMod;
    SubMessageHeader      stSubMsgHeader;
    InfoTimestamp         stInfoTS;
    DataFrag              stDataFrag;
    USHORT                usFragSize;
    CHAR                  cUserBuffer[DATA_SIZE];
    UINT32                uiFragTotal;

    DDS_DomainParticipant* pstParticipant = pstDataWriter->pstTopic->pstParticipant;


    /* �����Ƭ���� */
    if (0 < pstHistoryData->uiDataLen % FRAG_SIZE)
    {
        uiFragTotal = (pstHistoryData->uiDataLen / FRAG_SIZE) + 1;
    }
    else
    {
        uiFragTotal = (pstHistoryData->uiDataLen / FRAG_SIZE);
    }

    /* ���㵱ǰ��Ƭ���� */
    if (uiFragTotal == uiFragNum)
    {
        usFragSize = pstHistoryData->uiDataLen % FRAG_SIZE;

        usFragSize = (0 == usFragSize) ? FRAG_SIZE : usFragSize;
    }
    else
    {
        usFragSize = FRAG_SIZE;
    }


    /* �����ֽ����ʼ�������淢������ */
    INITIAL_MEMORYBLOCK(stMemBlock, cUserBuffer, DATA_SIZE);

    /* ��һ�����л�RTPS����ͷ */
    SERIALIZE_RTPS_MESSAGE_HEADER(pstParticipant->stRTPSMsgHeader, stMemBlock);

	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
    stSubMsgHeader.flags[2] = TRUE;    //dataFlag
    stSubMsgHeader.flags[3] = FALSE;   //keyFlag

    /* ���л�ʱ�� */
    stSubMsgHeader.submessageId = INFO_TS;
    stSubMsgHeader.submessageLength = sizeof(Time_t);
    stInfoTS.value = pstHistoryData->timeInfo;

    /* ���л�ʱ���ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���л�ʱ���ӱ�����Ϣ�� */
    SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

    /* ���л��û����� */
    stSubMsgHeader.submessageId = DATA_FRAG;
    stDataFrag.extraFlags[0] = 0x00;
    stDataFrag.extraFlags[1] = 0x00;
    stDataFrag.octetsToInlineQos = 0x0010;
    stDataFrag.readerId = pstDiscReader->guid.entityId;;
    stDataFrag.writerId = pstDataWriter->guid.entityId;
    stDataFrag.writerSN = pstHistoryData->seqNum;
    stDataFrag.fragmentStartingNum = uiFragNum;
    stDataFrag.fragmentsInSubmessage = 1;
    stDataFrag.dataSize = pstHistoryData->uiDataLen;
    stDataFrag.fragmentSize = FRAG_SIZE;
	stDataFrag.serializedPayload.m_cdrType = CDR_LE;  //С���û���������
	stDataFrag.serializedPayload.m_option = 0x0000;

    /* ���л��û������ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���л��û������ӱ�����Ϣ�壬ͬʱ̽���¼readerIdλ�� */
    SERIALIZE_DATAFRAG(stDataFrag, stMemBlock);

    /* dataFlag����keyFlagΪTRUE��������� */
    SERIALIZE_SERIALIZEDDATA(stDataFrag.serializedPayload, stMemBlock);

    /* ���л��û��������� */
    PUT_BYTE_ARRAY(stMemBlock, pstHistoryData->data + ((uiFragNum - 1) * FRAG_SIZE), usFragSize);

    /* �����ӱ�����Ϣ�峤�ȣ�������Ϣͷ */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stDataFrag.uiStartIndex;

    /* ���л��޸��ӱ��ĳ��� */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);
    //printf("Nepahlem : Resend fragmentStartingNum = %d writerSN = %d \n", stDataFrag.fragmentStartingNum, stDataFrag.writerSN.low);
    /* ���͵���Ӧ��ÿ��Զ���Ķ��� */
    RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscReader->pstUnicastLocator);

}

/*---------------------------------------------------------------------------------
-   �� �� ��: HandleUSERWriterDataMsg
-   ��������: �����û����ݱ���
-   ��    ��: ����DomainParticipantָ��
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �����û����ݱ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\13       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL HandleUSERWriterDataMsg(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader*  pstSubMsgHeader, InfoTimestamp* pstTimestamp, Data* pstDataMsg, MemoryBlock* pstMemBlock, DDS_DomainParticipant* pstParticipant)
{
    GUID_t stDiscWtGuid;
    DDS_DataReader* pstDataReader = NULL;
	DDS_DiscoveredWriter* pstDiscWriter = NULL;
	InlineQos stInlineQos;
	HistoryData* pstHistoryData = NULL;
	int inlineQosSize = 0;
	//CHAR temp[10];
	BOOL bNeedExchangeBytes;
	bNeedExchangeBytes = FALSE;
	if (IS_MACHINE_LITTLE_ENDIAN != pstSubMsgHeader->flags[0])
		bNeedExchangeBytes = TRUE;

    /* ��ȡԶ��д����guid */
    stDiscWtGuid.prefix = pstRTPSMsgHeader->guidPrefix;
    stDiscWtGuid.entityId = pstDataMsg->writerId;

    /* ����qos��ʱ������ֱ�ӹ���*/
	if (pstSubMsgHeader->flags[1])	
    {
		stInlineQos.guid = GUID_UNKNOWN;
		stInlineQos.disposed = 0;
		inlineQosSize = pstMemBlock->readIndex;
        NetworkByteOrderConvertInlineQos(pstMemBlock, &stInlineQos, bNeedExchangeBytes);
    	inlineQosSize = pstMemBlock->readIndex - inlineQosSize;
    }

    /* �ж��Ƿ�Ϊ�û���Ϣ*/
    if (!(pstSubMsgHeader->flags[2] || pstSubMsgHeader->flags[3]))
    {
		pstMemBlock->readIndex = pstMemBlock->totalSize;
        PrintLog(COMMLOG_WARN, "HandleUSERWriterDataMsg: There are no UserData!\n");
        return FALSE;
    }
    
    /* ���Ҹ��û����ݶ�Ӧ��Զ��д���� */
    pstDiscWriter = GetDiscoveredWriterFromParticipantByGuid(pstParticipant, &stDiscWtGuid);
    if (NULL == pstDiscWriter)
    {
		pstMemBlock->readIndex = pstMemBlock->totalSize;
//        printf("Topic %s HandleUSERWriterDataMsg: Cannot discovered remote dataWrite.\n","Nephalem");
		PrintLog(COMMLOG_WARN, "HandleUSERWriterDataMsg: Cannot discovered remote dataWriter.\n");
        return FALSE;
    }

    /* Nephalem partition ����ͬһ���������� */
    if (FALSE == pstDiscWriter->bSamePartiton)
    {
        return FALSE;
    }

    pstDataReader = pstDiscWriter->pstTopic->pstLocalReader;
    if (NULL == pstDataReader)
    {
		pstMemBlock->readIndex = pstMemBlock->totalSize;
        PrintLog(COMMLOG_WARN, "HandleUSERWriterDataMsg: Cannot discovered dataReader.\n");
//        printf("Topic %s HandleUSERWriterDataMsg: Cannot discovered dataReader.\n",pstDiscWriter->pstTopic->topicName.value);
        return FALSE;
    }
#ifdef DDS_MONITOR
    /* ���ͼ����Ϣ */
    DDS_MUTEX_LOCK(pstParticipant->monMutex);
    UserMsgSendMonitor(pstParticipant, &pstDiscWriter->pstTopic->tpGuid, pstMemBlock, 0x01);
    DDS_MUTEX_UNLOCK(pstParticipant->monMutex);
#endif
    /* �����л�װ����Ϣ */
    DESERIALIZE_SERIALIZEDDATA(pstDataMsg->serializedPayload, (*pstMemBlock), bNeedExchangeBytes);

    if (pstMemBlock->totalSize < pstMemBlock->readIndex)
    {
        return FALSE;
    }

    /* �������ݽṹ�ڴ����� */
    pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC_T(sizeof(HistoryData));
    if (NULL == pstHistoryData)
    {
        PrintLog(COMMLOG_ERROR, "HandleUSERWriterDataMsg:Failed to create HistoryData, Insufficient memory space.\n");
        return FALSE;
    }
    /* Nephalem ��ʼ�� */
    InitHistoryData(pstHistoryData);
    
#if defined QYDDS_MASSDATA
    /* Nephalem massdata �ȴ����Ƭ��Ϣ����ֹ�û����ݳ��ȼ������ */
    /* Nephalem massdata��Ƭ������ȡ��ط�Ƭ������Ϣ */
    if (strcmp(pstMemBlock->rdPtr, "MASSDATA") == 0)
    {

        GET_CHAR_ARRAY((*pstMemBlock), temp, 9);

        /* Nephalem massdata�����л���Ƭ���ݻ���� */
        DESERIALIZE_SEQUENCENUMBER(pstHistoryData->massDataBaseSeq, (*pstMemBlock));
        /* Nephalem massdata�����л���Ƭ����*/
        GET_UNSIGNED_INT((*pstMemBlock), pstHistoryData->massDataTotal);
        /* Nephalem massdata�����л���Ƭ�����ܴ�С*/
        GET_UNSIGNED_INT((*pstMemBlock), pstHistoryData->massDataTotolSize);
    }
#endif
    /* �û�����ʱ��� */
    pstHistoryData->timeInfo = pstTimestamp->value;
    pstHistoryData->uiFragTotal = 0;
    pstHistoryData->uiFragNum = 0;

	/* �������seqNumС����������Ϊ�������ݣ�ֱ������ */
	if (ISLESSTHEN_SEQUENCENUMBER(pstDataMsg->writerSN, pstDiscWriter->stAckNack.readerSNState.base))
	{
		if (strcmp(pstHistoryData->data, "DeadLine MSG.") == 0)
		{
			//pstDataReader->totHistoryNum++;
			DeilverStatusFulUserMsg(pstDataReader, pstDiscWriter, pstHistoryData);
		}
		else
		{
			/* Nephalem �ͷ�pstHistoryData�������ڴ�й© */
			DDS_STATIC_FREE(pstHistoryData->data);
			pstHistoryData->data = NULL;
			DDS_STATIC_FREE(pstHistoryData);
			pstHistoryData = NULL;
		}
		return TRUE;
	}

    /* ��¼��ǰ����seqNum */
    pstHistoryData->seqNum = pstDataMsg->writerSN;

    {
    	int dataLen = pstSubMsgHeader->submessageLength - inlineQosSize - 24;
		int topicLen = 0;
#ifdef _TOPIC_INFO
		topicLen = *(int *)pstMemBlock->rdPtr;
		if (bNeedExchangeBytes)
		{
			topicLen = ((topicLen >> 24) & 0xFF) | ((topicLen >> 8) & 0xFF00) | ((topicLen << 8) & 0xFF0000) | ((topicLen << 24) & 0xFF000000);
		}
		topicLen += (topicLen & 3) ? (4 - (topicLen & 3)) : 0;
		MEMORYBLOCK_SKIP_READER((*pstMemBlock), 4 + topicLen);
		dataLen -= 4 + topicLen;
#endif
#ifdef QYDDS_RTI_STRINGHEADER
		dataLen = *(int *)pstMemBlock->rdPtr;
		if (bNeedExchangeBytes)
		{
			dataLen = ((dataLen >> 24) & 0xFF) | ((dataLen >> 8) & 0xFF00) | ((dataLen << 8) & 0xFF0000) | ((dataLen << 24) & 0xFF000000);
		}
		MEMORYBLOCK_SKIP_READER((*pstMemBlock), 4);
#endif
	    /* �����û����ݳ��� */
	    pstHistoryData->uiDataLen = dataLen;
	    /*
		pstHistoryData->uiDataLen = pstSubMsgHeader->submessageLength - sizeof(pstDataMsg->extraFlags) -
			sizeof(pstDataMsg->octetsToInlineQos) - sizeof(pstDataMsg->readerId) -
			sizeof(pstDataMsg->writerId) - sizeof(pstDataMsg->writerSN) - sizeof(unsigned short) * 2;
		*/
		//printf("Nephalem totalSize = %d readIndex = %d  uiDataLen = %d \n ",pstMemBlock->totalSize , pstMemBlock->readIndex,pstHistoryData->uiDataLen);

	    /* �û������ڴ����� */
	    pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC_T(pstHistoryData->uiDataLen);
	    if (NULL == pstHistoryData->data)
	    {
	        /* �û����������ڴ�ʧ�����ͷ�֮ǰ���� */
	        DDS_STATIC_FREE(pstHistoryData);
	        PrintLog(COMMLOG_ERROR, "HandleUSERWriterDataMsg:Failed to create HistoryData->data, Insufficient memory space.\n");
	        return FALSE;
	    }
	    /* �����л��û����� */
	    GET_CHAR_ARRAY((*pstMemBlock), pstHistoryData->data, (INT)pstHistoryData->uiDataLen);

	    if (dataLen & 3)
	    {
			MEMORYBLOCK_SKIP_READER((*pstMemBlock), (4 - (dataLen & 3)));
	    }
    }
	
	//PrintBinData(pstHistoryData->data,pstHistoryData->uiDataLen, "History Data");

    while (pstDataReader != NULL)
    {
    	
		/* Nephalem ownership ֵ��һ������ƥ�䣬ֱ������ */
		if (pstDataReader->stDataReaderQos.ownership.kind != pstDiscWriter->stDataWriterQos.ownership.kind)
		{
		    return TRUE;
		}

		/* Nephalem �����ݵ�д������������Ȩֵ��ߵ�д������ֱ������*/
		if (EXCLUSIVE_OWNERSHIP_QOS == pstDataReader->stDataReaderQos.ownership.kind &&
		    pstDiscWriter != pstDataReader->pstOwnerDiscWriter)
		{
		    return TRUE;
		}
        
        /* ����Ϊ��״̬�Ķ���������Զ��д����״̬��ֱ�������ύ */
        if (BEST_EFFORT_RELIABILITY_QOS == pstDataReader->stDataReaderQos.reliability.kind)
        {
        	pstDataReader->lastDataTime = GetNowTime();
            //pstDataReader->totHistoryNum++;
            DeilverStatuslessUserMsg(pstDataReader, pstDiscWriter, pstHistoryData);
            pstDataReader = pstDataReader->pNext;
            continue;
        }

        /* �����Ķ���Ϊ��״̬��Զ��д������״̬����ƥ�䣬ֱ������ */
        if (BEST_EFFORT_RELIABILITY_QOS == pstDiscWriter->stDataWriterQos.reliability.kind)
        {
            /* Nephalem �ͷ�pstHistoryData�������ڴ�й© */
            DDS_STATIC_FREE(pstHistoryData->data);
            pstHistoryData->data = NULL;
            DDS_STATIC_FREE(pstHistoryData);
            pstHistoryData = NULL;

            PrintLog(COMMLOG_WARN, "HandleUSERWriterDataMsg:Reader and writer states do not match.\n");
            return TRUE;
        }
        pstDiscWriter->heartMsgNum++;
        pstDataReader->lastDataTime = GetNowTime();
        DeilverStatusFulUserMsg(pstDataReader, pstDiscWriter, pstHistoryData);
        pstDataReader = pstDataReader->pNext;
    }
	/* JD Question 2021-11-25 �������߼�����take���ݺ��Ѿ�����ˣ�������û��������*/
    UninitHistoryCache(&pstDiscWriter->pstTopic->stUserData);
    return TRUE;
}

BOOL HandleUSERWriterDataFrag(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader*  pstSubMsgHeader, InfoTimestamp* pstTimestamp, DataFrag* pstDataFrag, MemoryBlock* pstMemBlock, DDS_DomainParticipant* pstParticipant)
{
    GUID_t stDiscWtGuid;
    DDS_DataReader* pstDataReader = NULL;
	InlineQos stInlineQos;
	DDS_DiscoveredWriter* pstDiscWriter = NULL;
	HistoryData* pstHistoryData = NULL;

	BOOL bNeedExchangeBytes;
	bNeedExchangeBytes = FALSE;
	if (IS_MACHINE_LITTLE_ENDIAN != pstSubMsgHeader->flags[0])
		bNeedExchangeBytes = TRUE;

    /* ��ȡԶ��д����guid */
    stDiscWtGuid.prefix = pstRTPSMsgHeader->guidPrefix;
    stDiscWtGuid.entityId = pstDataFrag->writerId;

    /* ����qos��ʱ������ֱ�ӹ���*/
    if (pstSubMsgHeader->flags[1])
    {
		stInlineQos.guid = GUID_UNKNOWN;
		stInlineQos.disposed = 0;
        NetworkByteOrderConvertInlineQos(pstMemBlock, &stInlineQos, bNeedExchangeBytes);
    }

    /* �ж��Ƿ�Ϊ�û���Ϣ*/
    if (!(pstSubMsgHeader->flags[2] || pstSubMsgHeader->flags[3]))
    {
        PrintLog(COMMLOG_WARN, "HandleUSERWriterDataMsg: There are no UserData!\n");
        return FALSE;
    }

    /* ���Ҹ��û����ݶ�Ӧ��Զ��д���� */
    pstDiscWriter = GetDiscoveredWriterFromParticipantByGuid(pstParticipant, &stDiscWtGuid);
    if (NULL == pstDiscWriter)
    {
        PrintLog(COMMLOG_WARN, "HandleUSERWriterDataMsg: Cannot discovered remote dataWriter.\n");
        return FALSE;
    }

    /* Nephalem partition ����ͬһ���������� */
    if (FALSE == pstDiscWriter->bSamePartiton)
    {
        return FALSE;
    }

    pstDataReader = pstDiscWriter->pstTopic->pstLocalReader;
    if (NULL == pstDataReader)
    {
        PrintLog(COMMLOG_WARN, "HandleUSERWriterDataMsg: Cannot discovered dataReader.\n");
        return FALSE;
    }

    /* û��ָ���Ķ���Id��������Ϊͬһ�����µ��Ķ��� */
    //if (EntityId_Is_Equal(&ENTITYID_UNKNOWN, &pstDataMsg->readerId))
    //{
    //    pstDataReader = pstDiscWriter->pstTopic->pstLocalReader;
    //    if (NULL == pstDataReader)
    //    {
    //        PrintLog(COMMLOG_ERROR, "HandleUSERWriterDataMsg: Cannot discovered local dataReader.\n");
    //        return FALSE;
    //    }
    //}
    //else
    //{
    //    /* ָ�����Ķ���id����ҪУ���Ƿ�һ�� */
    //    pstDataReader = pstDiscWriter->pstTopic->pstLocalReader;
    //    while (NULL != pstDataReader)
    //    {
    //        if (EntityId_Is_Equal(&pstDataReader->guid.entityId, &pstDataMsg->readerId))
    //        {
    //            break;
    //        }
    //        pstDataReader = pstDataReader->pNext;
    //    }

    //    if (NULL == pstDataReader)
    //    {
    //        PrintLog(COMMLOG_ERROR, "HandleUSERWriterDataMsg: Cannot discovered local dataReader.\n");
    //        return FALSE;
    //    }
    //}

   
#ifdef DDS_MONITOR
    /* ���ͼ����Ϣ */
    DDS_MUTEX_LOCK(pstParticipant->monMutex);
    UserMsgSendMonitor(pstParticipant, &pstDiscWriter->pstTopic->tpGuid, pstMemBlock, 0x01);
    DDS_MUTEX_UNLOCK(pstParticipant->monMutex);
#endif

    /* Nephalem ownership ֵ��һ������ƥ�䣬ֱ������ */
    if (pstDataReader->stDataReaderQos.ownership.kind != pstDiscWriter->stDataWriterQos.ownership.kind)
    {
        return TRUE;
    }

    /* Nephalem �����ݵ�д������������Ȩֵ��ߵ�д������ֱ������*/
    if (EXCLUSIVE_OWNERSHIP_QOS == pstDataReader->stDataReaderQos.ownership.kind &&
        pstDiscWriter != pstDataReader->pstOwnerDiscWriter)
    {
        return TRUE;
    }

    /* �����л�װ����Ϣ */
    DESERIALIZE_SERIALIZEDDATA(pstDataFrag->serializedPayload, (*pstMemBlock), bNeedExchangeBytes);

    if (pstMemBlock->totalSize < pstMemBlock->readIndex)
    {
        return FALSE;
    }

    /* �������ݽṹ�ڴ����� */
    pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC_T(sizeof(HistoryData));
    if (NULL == pstHistoryData)
    {
        PrintLog(COMMLOG_ERROR, "HandleUSERWriterDataMsg:Failed to create HistoryData, Insufficient memory space.\n");
        return FALSE;
    }

    /* Nephalem ��ʼ�� */
    InitHistoryData(pstHistoryData);

    pstHistoryData->pstFragData = (FragData*)DDS_STATIC_MALLOC_T(sizeof(FragData));

    if (NULL == pstHistoryData->pstFragData)
    {
        DDS_STATIC_FREE(pstHistoryData);
        pstHistoryData->data = NULL;
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryDataFrag, Insufficient memory space.\n");
        return DDS_RETCODE_ERROR;
    }


    /* �û�����ʱ��� */
    pstHistoryData->timeInfo = pstTimestamp->value;

    /* �����û����ݳ��� */
    pstHistoryData->uiDataLen = pstDataFrag->dataSize;//pstMemBlock->totalSize - pstMemBlock->readIndex;

    pstHistoryData->uiFragLen = pstDataFrag->fragmentSize;

    /* �û������ڴ����� */
    //pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC_T(pstHistoryData->uiDataLen);

    //if (NULL == pstHistoryData->data)
    //{
    //    /* �û����������ڴ�ʧ�����ͷ�֮ǰ���� */
    //    DDS_STATIC_FREE(pstHistoryData);
    //    pstHistoryData = NULL;
    //    PrintLog(COMMLOG_ERROR, "HandleUSERWriterDataMsg:Failed to create HistoryData->data, Insufficient memory space.\n");
    //    return FALSE;
    //}

    /* ��¼��ǰ����seqNum */
    pstHistoryData->seqNum = pstDataFrag->writerSN;

    /* Nephalem �����ܷ�Ƭ���� */
    pstHistoryData->uiFragTotal = pstDataFrag->dataSize / FRAG_SIZE + 1;

    pstHistoryData->uiFragNum = pstDataFrag->fragmentStartingNum;

    pstHistoryData->uiFragLen = pstDataFrag->fragmentSize;

    /* Nephalem �����Ƭ����ʵ���� */
    if (pstHistoryData->uiFragTotal == pstHistoryData->uiFragNum)
    {
        pstHistoryData->pstFragData->usDataLen = pstHistoryData->uiDataLen % pstHistoryData->uiFragLen;

        if (0 == pstHistoryData->pstFragData->usDataLen)
            pstHistoryData->pstFragData->usDataLen = pstHistoryData->uiFragLen;
    }
    else
        pstHistoryData->pstFragData->usDataLen = pstHistoryData->uiFragLen;

    pstHistoryData->pstFragData->data = (CHAR*)DDS_STATIC_MALLOC_T(pstHistoryData->pstFragData->usDataLen);

    if (NULL == pstHistoryData->pstFragData->data)
    {
        /* �û����������ڴ�ʧ�����ͷ�֮ǰ���� */

        DDS_STATIC_FREE(pstHistoryData->pstFragData);
        pstHistoryData->pstFragData = NULL;
        DDS_STATIC_FREE(pstHistoryData);
        pstHistoryData = NULL;
        PrintLog(COMMLOG_ERROR, "HandleUSERWriterDataMsg:Failed to create HistoryData->data, Insufficient memory space.\n");
        return FALSE;
    }

    /* �����л��û����� */
    GET_CHAR_ARRAY((*pstMemBlock), pstHistoryData->pstFragData->data, pstHistoryData->pstFragData->usDataLen);

	/* �������seqNumС����������Ϊ�������ݣ�ֱ������ */
	if (ISLESSTHEN_SEQUENCENUMBER(pstDataFrag->writerSN, pstDiscWriter->stAckNack.readerSNState.base))
	{
		/* Nephalem �ͷ�pstHistoryData�������ڴ�й© */
		UninitHistoryData(pstHistoryData);
		DDS_STATIC_FREE(pstHistoryData);
		pstHistoryData = NULL;
		return TRUE;
	}

    while (pstDataReader != NULL)
    {
        /* ����Ϊ��״̬�Ķ���������Զ��д����״̬��ֱ�������ύ */
        if (BEST_EFFORT_RELIABILITY_QOS == pstDataReader->stDataReaderQos.reliability.kind)
        {          
            DeilverStatuslessUserMsg(pstDataReader, pstDiscWriter, pstHistoryData);

			///* JD Question 2021-11-25 BEST_EFFORT �����е�DataReader�ύ���� */
			pstDataReader = pstDataReader->pNext;
			continue;
			/* JD ��ô��return ��*/
            return TRUE;
        }

        /* �����Ķ���Ϊ��״̬��Զ��д������״̬����ƥ�䣬ֱ������ */
        if (BEST_EFFORT_RELIABILITY_QOS == pstDiscWriter->stDataWriterQos.reliability.kind)
        {
            /* Nephalem �ͷ�pstHistoryData�������ڴ�й© */
            DDS_STATIC_FREE(pstHistoryData->pstFragData->data);
            pstHistoryData->pstFragData->data = NULL;
            DDS_STATIC_FREE(pstHistoryData->pstFragData);
            pstHistoryData->pstFragData = NULL;
            DDS_STATIC_FREE(pstHistoryData);
            pstHistoryData = NULL;

            PrintLog(COMMLOG_WARN, "HandleUSERWriterDataMsg:Reader and writer states do not match.\n");
            return TRUE;
        }

        /* JD 2021-11-26 ��Ƭ���Ĳ����ڴ˴� ��totHistoryNum����++���� */
        //pstDataReader->totHistoryNum++;
        /* ������Զ�˶�Ϊ��״̬�ύ���� */
        DeilverStatusFulUserMsg(pstDataReader, pstDiscWriter, pstHistoryData);

        pstDataReader = pstDataReader->pNext;
    }
	/* JD DataFrag�����ύʱ���ὫpstHistoryData ���ݿ���һ���ڽ��в�����������Ҫ�������ͷ�һ���ڴ� */
	/* JD 2021-11-24 �ͷ� pstHistoryData ���ڴ� */
	if (NULL != pstHistoryData)
	{
		UninitHistoryData(pstHistoryData);
		DDS_STATIC_FREE(pstHistoryData);
		pstHistoryData = NULL;
	}



    return FALSE;
}

BOOL HandleGapMsg(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader*  pstSubMsgHeader, InfoTimestamp* pstTimestamp, Gap* pstGapMsg, MemoryBlock* pstMemBlock, DDS_DomainParticipant* pstParticipant)
{
    GUID_t stDiscWtGuid;
    DDS_DataReader* pstDataReader = NULL;
	InlineQos stInlineQos;
	DDS_DiscoveredWriter* pstDiscWriter = NULL;

	BOOL bNeedExchangeBytes;
	bNeedExchangeBytes = FALSE;
	if (IS_MACHINE_LITTLE_ENDIAN != pstSubMsgHeader->flags[0])
		bNeedExchangeBytes = TRUE;

    /* ��ȡԶ��д����guid */
    stDiscWtGuid.prefix = pstRTPSMsgHeader->guidPrefix;
    stDiscWtGuid.entityId = pstGapMsg->writerId;

    /* ����qos��ʱ������ֱ�ӹ���*/
    if (pstSubMsgHeader->flags[1])
    {
		stInlineQos.guid = GUID_UNKNOWN;
		stInlineQos.disposed = 0;
        NetworkByteOrderConvertInlineQos(pstMemBlock, &stInlineQos, bNeedExchangeBytes);
    }

    /* ���Ҹ��û����ݶ�Ӧ��Զ��д���� */
    pstDiscWriter = GetDiscoveredWriterFromParticipantByGuid(pstParticipant, &stDiscWtGuid);
    if (NULL == pstDiscWriter)
    {
        PrintLog(COMMLOG_WARN, "HandleUSERWriterDataMsg: Cannot discovered remote dataWriter.\n");
        return FALSE;
    }

    pstDataReader = pstDiscWriter->pstTopic->pstLocalReader;
    if (NULL == pstDataReader)
    {
        PrintLog(COMMLOG_WARN, "HandleUSERWriterDataMsg: Cannot discovered dataReader.\n");
        return FALSE;
    }


    CorrectDiscoveredWriterGapMsg(pstDataReader, pstDiscWriter, pstGapMsg);

    if (pstDataReader->pstTopic->stUserData.pstHead)
    {
        while (NULL != pstDataReader)
        {

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
                pstDataReader = pstDataReader->pNext;

        }

            UninitHistoryCache(&pstDiscWriter->pstTopic->stUserData);
    }
    return TRUE;
}
