#include "../include/GlobalDefine.h"

#define DATA_SIZE                   (64 * 1024)       /* 分片负载量 */
#ifdef _FC
#define FRAG_SIZE                   (64 * 1024)   /* 用户数据负载量 */
#else
#define FRAG_SIZE                   (64 * 1024 - 1024)   /* 用户数据负载量 */
#endif


extern int iWriteLog;
/*---------------------------------------------------------------------------------
-   函 数 名: JointUSERDataMsgSendUnknownReader
-   功能描述: 组装用户数据报文发送未指定阅读器
-   输    入: 本地写入器、缓存
-   返    回:
-   全局变量:
-   注    释: 组装用户数据发送报文
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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

    /* 分片待修复 */
    /* 是否需要分片发送 */
    if (pstHistoryData->uiDataLen > FRAG_SIZE)
    {
        /* 网络字节序初始化，缓存发布报文 */
        INITIAL_MEMORYBLOCK(stMemBlock, cUserBuffer, DATA_SIZE);

        /* 第一步序列化RTPS报文头 */
        SERIALIZE_RTPS_MESSAGE_HEADER(pstParticipant->stRTPSMsgHeader, stMemBlock);

        /* 时间报文头设置 */
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
        /* 序列化时间报文头 */
        SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);
        /* 序列化时间子报文体 */
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
        /* 第二步序列化子报文头 */
        SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);
        /* 第三步序列化子报文消息体 */
        SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

        /* 用户数据报文头设置 */
        stSubMsgHeader.submessageId = DATA_FRAG;
		if (0 == IS_MACHINE_LITTLE_ENDIAN)
			stSubMsgHeader.flags[0] = FALSE;    //littleEndian
		else
			stSubMsgHeader.flags[0] = TRUE;    //littleEndian
        stSubMsgHeader.flags[1] = TRUE;    //inlineQosFlag
        stSubMsgHeader.flags[2] = TRUE;    //dataFlag
        stSubMsgHeader.flags[3] = FALSE;   //keyFlag

        /* 用户数据报文体设置 */
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

        /* 序列化用户数据报文头 */
        SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

        /* 序列化用户数据报文体 */
        SERIALIZE_DATAFRAG(stDataFrag, stMemBlock);

        /* 序列化参数列表 */
        stParamHead.paramType = PID_KEY_HASH;
        stParamHead.length = sizeof(GUID_t);
        SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);
        SERIALIZE_GUID(pstDataWriter->guid, stMemBlock);

        /* 序列化参数列表结尾 */
        stParamHead.paramType = PID_SENTINEL;
        stParamHead.length = 0;
        SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);

        /* 序列化的有效载荷 */
        SERIALIZE_SERIALIZEDDATA(stDataFrag.serializedPayload, stMemBlock);

        /*for (int i = 0; i < pstHistoryData->uiFragLen; i++)
        {
            printf("%c", pstHistoryData->pstFragData[i]);
        }
        printf("\n");*/
        /* 序列化用户数据内容 */
        PUT_BYTE_ARRAY(stMemBlock, pstHistoryData->pstFragData->data, pstHistoryData->pstFragData->usDataLen);

        /* 计算子报文消息体长度，不带消息头 */
        stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stDataFrag.uiStartIndex;
        
        //printf("seqNum: %u, submsgLength: %u\n", stDataFrag.writerSN.low, stSubMsgHeader.submessageLength);
        /* 序列化修改子报文长度 */
        INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

        PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

        while (NULL != pstDiscReader)
        {
            /* Nephalem partition 不是同一分区不发送 */
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

            /* 修改目的地GuidPrefix */
            INITIAL_MEMORYBLOCK(stMemBlockMod, stInfoDest.pcGuidPre, sizeof(GuidPrefix_t));
            SERIALIZE_GUID_PREFIX(pstDiscReader->guid.prefix, stMemBlockMod);

            ///* 修改目的地GuidEntityId */
            //INITIAL_MEMORYBLOCK(stMemBlockMod, stData.pcReaderId, sizeof(EntityId_t));
            //SERIALIZE_ENTITYID(pstDiscReader->guid.entityId, stMemBlockMod);

            /* 发送到对应的每个远端阅读器 */
            DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
            
            RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscReader->pstUnicastLocator);

            /* Nephalem 所有分片数据发送完，发送HeartbetFrag进行确认 */
            if (pstHistoryData->uiFragTotal == pstHistoryData->uiFragNum)
            {
#if !defined(NO_HEARTBEAT)
                /* Nephalem 此时pstHistoryData还未HistoryCache，所以在此将参数传入 */
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
    	
        /* 网络字节序初始化，缓存发布报文 */
        INITIAL_MEMORYBLOCK(stMemBlock, cUserBuffer, DATA_SIZE);
        
    	if(1 == iWriteLog)
    	{
    		printf("Nephalem : INITIAL_MEMORYBLOCK ");
    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
    		printf("totalSize = %d writeIndex = %d \n",stMemBlock.totalSize,stMemBlock.writeIndex);
    	}

        /* 第一步序列化RTPS报文头 */
        SERIALIZE_RTPS_MESSAGE_HEADER(pstParticipant->stRTPSMsgHeader, stMemBlock);
        
    	if(1 == iWriteLog)
    	{
    		printf("Nephalem : SERIALIZE_RTPS_MESSAGE_HEADER ");
    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
    		printf("totalSize = %d writeIndex = %d \n",stMemBlock.totalSize,stMemBlock.writeIndex);
    	}

        /* 时间报文头设置 */
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
        /* 序列化时间报文头 */
        SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);
        /* 序列化时间子报文体 */
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
    	
        /* 第二步序列化子报文头 */
        SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);
        /* 第三步序列化子报文消息体 */
        SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);
        
    	if(1 == iWriteLog)
    	{
    		printf("Nephalem : SERIALIZE_INFODESTINATION ");
    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
    		printf("totalSize = %d writeIndex = %d \n",stMemBlock.totalSize,stMemBlock.writeIndex);
    	}

        /* 用户数据报文头设置 */
        stSubMsgHeader.submessageId = DATA;
		if (0 == IS_MACHINE_LITTLE_ENDIAN)
			stSubMsgHeader.flags[0] = FALSE;    //littleEndian
		else
			stSubMsgHeader.flags[0] = TRUE;    //littleEndian
        stSubMsgHeader.flags[1] = FALSE;    //inlineQosFlag
        stSubMsgHeader.flags[2] = TRUE;    //dataFlag
        stSubMsgHeader.flags[3] = FALSE;   //keyFlag

        /* 用户数据报文体设置 */
        stData.extraFlags[0] = 0x00;
        stData.extraFlags[1] = 0x00;
        stData.octetsToInlineQos = 0x0010;
        stData.readerId = ENTITYID_UNKNOWN;
        stData.writerId = pstDataWriter->guid.entityId;
        stData.writerSN = pstHistoryData->seqNum;
		if (IS_MACHINE_LITTLE_ENDIAN)
			stData.serializedPayload.m_cdrType = CDR_LE;  //小端用户数据设置  
		else
			stData.serializedPayload.m_cdrType = CDR_BE;  //小端用户数据设置    
        stData.serializedPayload.m_option = 0x0000;

    	if(1 == iWriteLog)
    	{
    		printf("Nephalem : SERIALIZE_SUBMESSAGEHEADER ");
    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
    		printf("totalSize = %d writeIndex = %d \n",stMemBlock.totalSize,stMemBlock.writeIndex);
    	}
    	
        /* 序列化用户数据报文头 */
        SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);
        
    	if(1 == iWriteLog)
    	{
    		printf("Nephalem : SERIALIZE_SUBMESSAGEHEADER ");
    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
    	}

        /* 序列化用户数据报文体 */
        SERIALIZE_DATA(stData, stMemBlock);
        
//    	if(1 == iWriteLog)
//    	{
//    		printf("Nephalem : SERIALIZE_DATA ");
//    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
//    	}

//        /* 序列化参数列表 */
//        stParamHead.paramType = PID_KEY_HASH;
//        stParamHead.length = sizeof(GUID_t);
//        SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);
//        SERIALIZE_GUID(pstDataWriter->guid, stMemBlock);
//
//        /* 序列化参数列表结尾 */
//        stParamHead.paramType = PID_SENTINEL;
//        stParamHead.length = 0;
//        SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);

        /* 序列化的有效载荷 */
        SERIALIZE_SERIALIZEDDATA(stData.serializedPayload, stMemBlock);
        
//    	if(1 == iWriteLog)
//    	{
//    		printf("Nephalem : SERIALIZE_SERIALIZEDDATA ");
//    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
//    	}

#if defined QYDDS_MASSDATA
        /* Nephalem massdata序列化分片数据信息*/
        if (pstHistoryData->massDataTotal != 0)
        {
            /* Nephalem massdata序列化分片数据标志 */
            PUT_BYTE_ARRAY(stMemBlock, "MASSDATA", 9);
            /* Nephalem massdata序列化分片数据基序号*/
            SERIALIZE_SEQUENCENUMBER(pstHistoryData->massDataBaseSeq, stMemBlock);
            /* Nephalem massdata序列化分片总数*/
            PUT_UNSIGNED_INT(stMemBlock, pstHistoryData->massDataTotal);
            /* Nephalem massdata序列化分片数据总大小*/
            PUT_UNSIGNED_INT(stMemBlock, pstHistoryData->massDataTotolSize);
        }
#endif
        /* 序列化用户数据内容 */
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

        /* 计算子报文消息体长度，不带消息头 */
        stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stData.uiStartIndex;

//    	if(1 == iWriteLog)
//    	{
//    		printf("Nephalem : INITIAL_MEMORYBLOCK ");
//    		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
//    	}
    	
        /* 序列化修改子报文长度 */
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
            /* Nephalem partition 不是同一分区不发送 */
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

            /* 修改目的地GuidPrefix */
            INITIAL_MEMORYBLOCK(stMemBlockMod, stInfoDest.pcGuidPre, sizeof(GuidPrefix_t));
            SERIALIZE_GUID_PREFIX(pstDiscReader->guid.prefix, stMemBlockMod);

            /* 修改目的地GuidEntityId */
            INITIAL_MEMORYBLOCK(stMemBlockMod, stData.pcReaderId, sizeof(EntityId_t));
            SERIALIZE_ENTITYID(pstDiscReader->guid.entityId, stMemBlockMod);

//			PrintBinData(stMemBlock.base, stMemBlock.writeIndex, "Send User Data");

            /* 发送到对应的每个远端阅读器 */
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
        /* 发送监控消息 */
        DDS_MUTEX_LOCK(pstParticipant->monMutex);
        UserMsgSendMonitor(pstParticipant, &pstDataWriter->pstTopic->tpGuid, &stMemBlock, 0x00);
        DDS_MUTEX_UNLOCK(pstParticipant->monMutex);
#endif

    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: JointUSERDataMsgSendAssignReader
-   功能描述: 组装用户数据报文发送指定阅读器
-   输    入: 本地写入器、缓存
-   返    回:
-   全局变量:
-   注    释: 组装用户数据报文发送指定阅读器
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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

    /* 是否需要分片发送 */
    if (pstHistoryData->uiDataLen > FRAG_SIZE)
    {
        /* 计算分片个数 */
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
            /* 计算当前分片长度 */
            if (uiFragTotal - 1 == uiFragNum)
            {
                usFragSize = pstHistoryData->uiDataLen - (uiFragNum * FRAG_SIZE);
            }
            else
            {
                usFragSize = FRAG_SIZE;
            }
            /* 网络字节序初始化，缓存发布报文 */
            INITIAL_MEMORYBLOCK(stMemBlock, cUserBuffer, DATA_SIZE);

            /* 第一步序列化RTPS报文头 */
            SERIALIZE_RTPS_MESSAGE_HEADER(pstParticipant->stRTPSMsgHeader, stMemBlock);

			if (0 == IS_MACHINE_LITTLE_ENDIAN)
				stSubMsgHeader.flags[0] = FALSE;    //littleEndian
			else
				stSubMsgHeader.flags[0] = TRUE;    //littleEndian
            stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
            stSubMsgHeader.flags[2] = TRUE;    //dataFlag
            stSubMsgHeader.flags[3] = FALSE;   //keyFlag

            /* 序列化时间 */
            stSubMsgHeader.submessageId = INFO_TS;
            stSubMsgHeader.submessageLength = sizeof(Time_t);
            stInfoTS.value = pstHistoryData->timeInfo;

            /* 序列化时间子报文头 */
            SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

            /* 序列化时间子报文消息体 */
            SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

            /* 序列化用户数据 */
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

            /* 序列化用户数据子报文头 */
            SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

            /* 序列化用户数据子报文消息体，同时探针记录readerId位置 */
            SERIALIZE_DATAFRAG(stDataFrag, stMemBlock);

            /* dataFlag或者keyFlag为TRUE，加上这个 */
            SERIALIZE_SERIALIZEDDATA(stDataFrag.serializedPayload, stMemBlock);

            /* 序列化用户数据内容 */
            PUT_BYTE_ARRAY(stMemBlock, pstHistoryData->data + (uiFragNum * FRAG_SIZE), usFragSize);

            /* 计算子报文消息体长度，不带消息头 */
            stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stDataFrag.uiStartIndex;

            /* 序列化修改子报文长度 */
            INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

            PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);
            //printf("Nepahlem : Resend fragmentStartingNum = %d writerSN = %d \n", stDataFrag.fragmentStartingNum, stDataFrag.writerSN.low);
            /* 发送到对应的每个远端阅读器 */
            RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscReader->pstUnicastLocator);
        }
    }
    else
    {
        /* 网络字节序初始化，缓存发布报文 */
        INITIAL_MEMORYBLOCK(stMemBlock, cUserBuffer, DATA_SIZE);

        /* 第一步序列化RTPS报文头 */
        SERIALIZE_RTPS_MESSAGE_HEADER(pstParticipant->stRTPSMsgHeader, stMemBlock);

        /* 时间报文头设置 */
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

        /* 序列化时间报文头 */
        SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

        /* 序列化时间子报文体 */
        SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

        /* InfoDT报文 */
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

        /* 第二步序列化子报文头 */
        SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

        /* 第三步序列化子报文消息体 */
        SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

        /* 用户数据报文头设置 */
        stSubMsgHeader.submessageId = DATA;
		if (0 == IS_MACHINE_LITTLE_ENDIAN)
			stSubMsgHeader.flags[0] = FALSE;    //littleEndian
		else
			stSubMsgHeader.flags[0] = TRUE;    //littleEndian
        stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
        stSubMsgHeader.flags[2] = TRUE;    //dataFlag
        stSubMsgHeader.flags[3] = FALSE;   //keyFlag

        /* 用户数据报文体设置 */
        stData.extraFlags[0] = 0x00;
        stData.extraFlags[1] = 0x00;
        stData.octetsToInlineQos = 0x0010;
        stData.readerId = pstDiscReader->guid.entityId;
        stData.writerId = pstDataWriter->guid.entityId;
        stData.writerSN = pstHistoryData->seqNum;
		if (0 == IS_MACHINE_LITTLE_ENDIAN)
			stData.serializedPayload.m_cdrType = CDR_LE;  //小端用户数据设置
		else
			stData.serializedPayload.m_cdrType = CDR_BE;  //小端用户数据设置
        stData.serializedPayload.m_option = 0x0000;

        /* 序列化用户数据报文头 */
        SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

        /* 序列化用户数据报文体 */
        SERIALIZE_DATA(stData, stMemBlock);

//        /* 序列化参数列表 */
//        stParamHead.paramType = PID_KEY_HASH;
//        stParamHead.length = sizeof(GUID_t);
//        SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);
//        SERIALIZE_GUID(pstDataWriter->guid, stMemBlock);
//
//        /* 序列化参数列表结尾 */
//        stParamHead.paramType = PID_SENTINEL;
//        stParamHead.length = 0;
//        SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);

        /* 序列化的有效载荷 */
        SERIALIZE_SERIALIZEDDATA(stData.serializedPayload, stMemBlock);

#if defined QYDDS_MASSDATA
        /* Nephalem massdata序列化分片数据信息*/
        if (pstHistoryData->massDataTotal != 0)
        {
            /* Nephalem massdata序列化分片数据标志 */
            PUT_BYTE_ARRAY(stMemBlock, "MASSDATA", 9);
            /* Nephalem massdata序列化分片数据基序号*/
            SERIALIZE_SEQUENCENUMBER(pstHistoryData->massDataBaseSeq, stMemBlock);
            /* Nephalem massdata序列化分片总数*/
            PUT_UNSIGNED_INT(stMemBlock, pstHistoryData->massDataTotal);
            /* Nephalem massdata序列化分片数据总大小*/
            PUT_UNSIGNED_INT(stMemBlock, pstHistoryData->massDataTotolSize);
        }
#endif
        /* 序列化用户数据内容 */
        PUT_BYTE_ARRAY(stMemBlock, pstHistoryData->data, pstHistoryData->uiDataLen);

        /* 计算子报文消息体长度，不带消息头 */
        stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stData.uiStartIndex;

        /* 序列化修改子报文长度 */
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

        /* 发送到对应的每个远端阅读器 */
        RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscReader->pstUnicastLocator);
    }
}


/*---------------------------------------------------------------------------------
-   函 数 名: JointUSERDataFragMsgSendAssignReader
-   功能描述: 组装指定用户数据DataFrag报文发送指定阅读器
-   输    入: 本地写入器、缓存
-   返    回:
-   全局变量:
-   注    释: 
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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


    /* 计算分片个数 */
    if (0 < pstHistoryData->uiDataLen % FRAG_SIZE)
    {
        uiFragTotal = (pstHistoryData->uiDataLen / FRAG_SIZE) + 1;
    }
    else
    {
        uiFragTotal = (pstHistoryData->uiDataLen / FRAG_SIZE);
    }

    /* 计算当前分片长度 */
    if (uiFragTotal == uiFragNum)
    {
        usFragSize = pstHistoryData->uiDataLen % FRAG_SIZE;

        usFragSize = (0 == usFragSize) ? FRAG_SIZE : usFragSize;
    }
    else
    {
        usFragSize = FRAG_SIZE;
    }


    /* 网络字节序初始化，缓存发布报文 */
    INITIAL_MEMORYBLOCK(stMemBlock, cUserBuffer, DATA_SIZE);

    /* 第一步序列化RTPS报文头 */
    SERIALIZE_RTPS_MESSAGE_HEADER(pstParticipant->stRTPSMsgHeader, stMemBlock);

	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
    stSubMsgHeader.flags[2] = TRUE;    //dataFlag
    stSubMsgHeader.flags[3] = FALSE;   //keyFlag

    /* 序列化时间 */
    stSubMsgHeader.submessageId = INFO_TS;
    stSubMsgHeader.submessageLength = sizeof(Time_t);
    stInfoTS.value = pstHistoryData->timeInfo;

    /* 序列化时间子报文头 */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* 序列化时间子报文消息体 */
    SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

    /* 序列化用户数据 */
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
	stDataFrag.serializedPayload.m_cdrType = CDR_LE;  //小端用户数据设置
	stDataFrag.serializedPayload.m_option = 0x0000;

    /* 序列化用户数据子报文头 */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* 序列化用户数据子报文消息体，同时探针记录readerId位置 */
    SERIALIZE_DATAFRAG(stDataFrag, stMemBlock);

    /* dataFlag或者keyFlag为TRUE，加上这个 */
    SERIALIZE_SERIALIZEDDATA(stDataFrag.serializedPayload, stMemBlock);

    /* 序列化用户数据内容 */
    PUT_BYTE_ARRAY(stMemBlock, pstHistoryData->data + ((uiFragNum - 1) * FRAG_SIZE), usFragSize);

    /* 计算子报文消息体长度，不带消息头 */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stDataFrag.uiStartIndex;

    /* 序列化修改子报文长度 */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);
    //printf("Nepahlem : Resend fragmentStartingNum = %d writerSN = %d \n", stDataFrag.fragmentStartingNum, stDataFrag.writerSN.low);
    /* 发送到对应的每个远端阅读器 */
    RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscReader->pstUnicastLocator);

}

/*---------------------------------------------------------------------------------
-   函 数 名: HandleUSERWriterDataMsg
-   功能描述: 处理用户数据报文
-   输    入: 本地DomainParticipant指针
-   返    回:
-   全局变量:
-   注    释: 处理用户数据报文
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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

    /* 获取远端写入器guid */
    stDiscWtGuid.prefix = pstRTPSMsgHeader->guidPrefix;
    stDiscWtGuid.entityId = pstDataMsg->writerId;

    /* 在线qos暂时不处理，直接过滤*/
	if (pstSubMsgHeader->flags[1])	
    {
		stInlineQos.guid = GUID_UNKNOWN;
		stInlineQos.disposed = 0;
		inlineQosSize = pstMemBlock->readIndex;
        NetworkByteOrderConvertInlineQos(pstMemBlock, &stInlineQos, bNeedExchangeBytes);
    	inlineQosSize = pstMemBlock->readIndex - inlineQosSize;
    }

    /* 判断是否为用户信息*/
    if (!(pstSubMsgHeader->flags[2] || pstSubMsgHeader->flags[3]))
    {
		pstMemBlock->readIndex = pstMemBlock->totalSize;
        PrintLog(COMMLOG_WARN, "HandleUSERWriterDataMsg: There are no UserData!\n");
        return FALSE;
    }
    
    /* 查找该用户数据对应的远端写入器 */
    pstDiscWriter = GetDiscoveredWriterFromParticipantByGuid(pstParticipant, &stDiscWtGuid);
    if (NULL == pstDiscWriter)
    {
		pstMemBlock->readIndex = pstMemBlock->totalSize;
//        printf("Topic %s HandleUSERWriterDataMsg: Cannot discovered remote dataWrite.\n","Nephalem");
		PrintLog(COMMLOG_WARN, "HandleUSERWriterDataMsg: Cannot discovered remote dataWriter.\n");
        return FALSE;
    }

    /* Nephalem partition 不是同一分区不处理 */
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
    /* 发送监控消息 */
    DDS_MUTEX_LOCK(pstParticipant->monMutex);
    UserMsgSendMonitor(pstParticipant, &pstDiscWriter->pstTopic->tpGuid, pstMemBlock, 0x01);
    DDS_MUTEX_UNLOCK(pstParticipant->monMutex);
#endif
    /* 反序列化装载信息 */
    DESERIALIZE_SERIALIZEDDATA(pstDataMsg->serializedPayload, (*pstMemBlock), bNeedExchangeBytes);

    if (pstMemBlock->totalSize < pstMemBlock->readIndex)
    {
        return FALSE;
    }

    /* 缓存数据结构内存申请 */
    pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC_T(sizeof(HistoryData));
    if (NULL == pstHistoryData)
    {
        PrintLog(COMMLOG_ERROR, "HandleUSERWriterDataMsg:Failed to create HistoryData, Insufficient memory space.\n");
        return FALSE;
    }
    /* Nephalem 初始化 */
    InitHistoryData(pstHistoryData);
    
#if defined QYDDS_MASSDATA
    /* Nephalem massdata 先处理分片信息，防止用户数据长度计算错误 */
    /* Nephalem massdata分片数据提取相关分片数据信息 */
    if (strcmp(pstMemBlock->rdPtr, "MASSDATA") == 0)
    {

        GET_CHAR_ARRAY((*pstMemBlock), temp, 9);

        /* Nephalem massdata反序列化分片数据基序号 */
        DESERIALIZE_SEQUENCENUMBER(pstHistoryData->massDataBaseSeq, (*pstMemBlock));
        /* Nephalem massdata反序列化分片总数*/
        GET_UNSIGNED_INT((*pstMemBlock), pstHistoryData->massDataTotal);
        /* Nephalem massdata反序列化分片数据总大小*/
        GET_UNSIGNED_INT((*pstMemBlock), pstHistoryData->massDataTotolSize);
    }
#endif
    /* 用户数据时间戳 */
    pstHistoryData->timeInfo = pstTimestamp->value;
    pstHistoryData->uiFragTotal = 0;
    pstHistoryData->uiFragNum = 0;

	/* 如果数据seqNum小于期望，则为过期数据，直接跳过 */
	if (ISLESSTHEN_SEQUENCENUMBER(pstDataMsg->writerSN, pstDiscWriter->stAckNack.readerSNState.base))
	{
		if (strcmp(pstHistoryData->data, "DeadLine MSG.") == 0)
		{
			//pstDataReader->totHistoryNum++;
			DeilverStatusFulUserMsg(pstDataReader, pstDiscWriter, pstHistoryData);
		}
		else
		{
			/* Nephalem 释放pstHistoryData，否则内存泄漏 */
			DDS_STATIC_FREE(pstHistoryData->data);
			pstHistoryData->data = NULL;
			DDS_STATIC_FREE(pstHistoryData);
			pstHistoryData = NULL;
		}
		return TRUE;
	}

    /* 记录当前数据seqNum */
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
	    /* 计算用户数据长度 */
	    pstHistoryData->uiDataLen = dataLen;
	    /*
		pstHistoryData->uiDataLen = pstSubMsgHeader->submessageLength - sizeof(pstDataMsg->extraFlags) -
			sizeof(pstDataMsg->octetsToInlineQos) - sizeof(pstDataMsg->readerId) -
			sizeof(pstDataMsg->writerId) - sizeof(pstDataMsg->writerSN) - sizeof(unsigned short) * 2;
		*/
		//printf("Nephalem totalSize = %d readIndex = %d  uiDataLen = %d \n ",pstMemBlock->totalSize , pstMemBlock->readIndex,pstHistoryData->uiDataLen);

	    /* 用户数据内存申请 */
	    pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC_T(pstHistoryData->uiDataLen);
	    if (NULL == pstHistoryData->data)
	    {
	        /* 用户数据申请内存失败需释放之前申请 */
	        DDS_STATIC_FREE(pstHistoryData);
	        PrintLog(COMMLOG_ERROR, "HandleUSERWriterDataMsg:Failed to create HistoryData->data, Insufficient memory space.\n");
	        return FALSE;
	    }
	    /* 反序列化用户数据 */
	    GET_CHAR_ARRAY((*pstMemBlock), pstHistoryData->data, (INT)pstHistoryData->uiDataLen);

	    if (dataLen & 3)
	    {
			MEMORYBLOCK_SKIP_READER((*pstMemBlock), (4 - (dataLen & 3)));
	    }
    }
	
	//PrintBinData(pstHistoryData->data,pstHistoryData->uiDataLen, "History Data");

    while (pstDataReader != NULL)
    {
    	
		/* Nephalem ownership 值不一样，不匹配，直接跳过 */
		if (pstDataReader->stDataReaderQos.ownership.kind != pstDiscWriter->stDataWriterQos.ownership.kind)
		{
		    return TRUE;
		}

		/* Nephalem 本数据的写入器不是所有权值最高的写入器，直接跳过*/
		if (EXCLUSIVE_OWNERSHIP_QOS == pstDataReader->stDataReaderQos.ownership.kind &&
		    pstDiscWriter != pstDataReader->pstOwnerDiscWriter)
		{
		    return TRUE;
		}
        
        /* 本地为无状态阅读器，无论远端写入器状态，直接向上提交 */
        if (BEST_EFFORT_RELIABILITY_QOS == pstDataReader->stDataReaderQos.reliability.kind)
        {
        	pstDataReader->lastDataTime = GetNowTime();
            //pstDataReader->totHistoryNum++;
            DeilverStatuslessUserMsg(pstDataReader, pstDiscWriter, pstHistoryData);
            pstDataReader = pstDataReader->pNext;
            continue;
        }

        /* 本地阅读器为有状态，远端写入器无状态，不匹配，直接跳过 */
        if (BEST_EFFORT_RELIABILITY_QOS == pstDiscWriter->stDataWriterQos.reliability.kind)
        {
            /* Nephalem 释放pstHistoryData，否则内存泄漏 */
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
	/* JD Question 2021-11-25 正常的逻辑，在take数据后已经清空了，这句语句没有起到作用*/
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

    /* 获取远端写入器guid */
    stDiscWtGuid.prefix = pstRTPSMsgHeader->guidPrefix;
    stDiscWtGuid.entityId = pstDataFrag->writerId;

    /* 在线qos暂时不处理，直接过滤*/
    if (pstSubMsgHeader->flags[1])
    {
		stInlineQos.guid = GUID_UNKNOWN;
		stInlineQos.disposed = 0;
        NetworkByteOrderConvertInlineQos(pstMemBlock, &stInlineQos, bNeedExchangeBytes);
    }

    /* 判断是否为用户信息*/
    if (!(pstSubMsgHeader->flags[2] || pstSubMsgHeader->flags[3]))
    {
        PrintLog(COMMLOG_WARN, "HandleUSERWriterDataMsg: There are no UserData!\n");
        return FALSE;
    }

    /* 查找该用户数据对应的远端写入器 */
    pstDiscWriter = GetDiscoveredWriterFromParticipantByGuid(pstParticipant, &stDiscWtGuid);
    if (NULL == pstDiscWriter)
    {
        PrintLog(COMMLOG_WARN, "HandleUSERWriterDataMsg: Cannot discovered remote dataWriter.\n");
        return FALSE;
    }

    /* Nephalem partition 不是同一分区不处理 */
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

    /* 没有指定阅读器Id，即关联为同一主题下的阅读器 */
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
    //    /* 指定了阅读器id，需要校验是否一致 */
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
    /* 发送监控消息 */
    DDS_MUTEX_LOCK(pstParticipant->monMutex);
    UserMsgSendMonitor(pstParticipant, &pstDiscWriter->pstTopic->tpGuid, pstMemBlock, 0x01);
    DDS_MUTEX_UNLOCK(pstParticipant->monMutex);
#endif

    /* Nephalem ownership 值不一样，不匹配，直接跳过 */
    if (pstDataReader->stDataReaderQos.ownership.kind != pstDiscWriter->stDataWriterQos.ownership.kind)
    {
        return TRUE;
    }

    /* Nephalem 本数据的写入器不是所有权值最高的写入器，直接跳过*/
    if (EXCLUSIVE_OWNERSHIP_QOS == pstDataReader->stDataReaderQos.ownership.kind &&
        pstDiscWriter != pstDataReader->pstOwnerDiscWriter)
    {
        return TRUE;
    }

    /* 反序列化装载信息 */
    DESERIALIZE_SERIALIZEDDATA(pstDataFrag->serializedPayload, (*pstMemBlock), bNeedExchangeBytes);

    if (pstMemBlock->totalSize < pstMemBlock->readIndex)
    {
        return FALSE;
    }

    /* 缓存数据结构内存申请 */
    pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC_T(sizeof(HistoryData));
    if (NULL == pstHistoryData)
    {
        PrintLog(COMMLOG_ERROR, "HandleUSERWriterDataMsg:Failed to create HistoryData, Insufficient memory space.\n");
        return FALSE;
    }

    /* Nephalem 初始化 */
    InitHistoryData(pstHistoryData);

    pstHistoryData->pstFragData = (FragData*)DDS_STATIC_MALLOC_T(sizeof(FragData));

    if (NULL == pstHistoryData->pstFragData)
    {
        DDS_STATIC_FREE(pstHistoryData);
        pstHistoryData->data = NULL;
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryDataFrag, Insufficient memory space.\n");
        return DDS_RETCODE_ERROR;
    }


    /* 用户数据时间戳 */
    pstHistoryData->timeInfo = pstTimestamp->value;

    /* 计算用户数据长度 */
    pstHistoryData->uiDataLen = pstDataFrag->dataSize;//pstMemBlock->totalSize - pstMemBlock->readIndex;

    pstHistoryData->uiFragLen = pstDataFrag->fragmentSize;

    /* 用户数据内存申请 */
    //pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC_T(pstHistoryData->uiDataLen);

    //if (NULL == pstHistoryData->data)
    //{
    //    /* 用户数据申请内存失败需释放之前申请 */
    //    DDS_STATIC_FREE(pstHistoryData);
    //    pstHistoryData = NULL;
    //    PrintLog(COMMLOG_ERROR, "HandleUSERWriterDataMsg:Failed to create HistoryData->data, Insufficient memory space.\n");
    //    return FALSE;
    //}

    /* 记录当前数据seqNum */
    pstHistoryData->seqNum = pstDataFrag->writerSN;

    /* Nephalem 计算总分片数量 */
    pstHistoryData->uiFragTotal = pstDataFrag->dataSize / FRAG_SIZE + 1;

    pstHistoryData->uiFragNum = pstDataFrag->fragmentStartingNum;

    pstHistoryData->uiFragLen = pstDataFrag->fragmentSize;

    /* Nephalem 计算分片的真实长度 */
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
        /* 用户数据申请内存失败需释放之前申请 */

        DDS_STATIC_FREE(pstHistoryData->pstFragData);
        pstHistoryData->pstFragData = NULL;
        DDS_STATIC_FREE(pstHistoryData);
        pstHistoryData = NULL;
        PrintLog(COMMLOG_ERROR, "HandleUSERWriterDataMsg:Failed to create HistoryData->data, Insufficient memory space.\n");
        return FALSE;
    }

    /* 反序列化用户数据 */
    GET_CHAR_ARRAY((*pstMemBlock), pstHistoryData->pstFragData->data, pstHistoryData->pstFragData->usDataLen);

	/* 如果数据seqNum小于期望，则为过期数据，直接跳过 */
	if (ISLESSTHEN_SEQUENCENUMBER(pstDataFrag->writerSN, pstDiscWriter->stAckNack.readerSNState.base))
	{
		/* Nephalem 释放pstHistoryData，否则内存泄漏 */
		UninitHistoryData(pstHistoryData);
		DDS_STATIC_FREE(pstHistoryData);
		pstHistoryData = NULL;
		return TRUE;
	}

    while (pstDataReader != NULL)
    {
        /* 本地为无状态阅读器，无论远端写入器状态，直接向上提交 */
        if (BEST_EFFORT_RELIABILITY_QOS == pstDataReader->stDataReaderQos.reliability.kind)
        {          
            DeilverStatuslessUserMsg(pstDataReader, pstDiscWriter, pstHistoryData);

			///* JD Question 2021-11-25 BEST_EFFORT 给所有的DataReader提交数据 */
			pstDataReader = pstDataReader->pNext;
			continue;
			/* JD 怎么就return 了*/
            return TRUE;
        }

        /* 本地阅读器为有状态，远端写入器无状态，不匹配，直接跳过 */
        if (BEST_EFFORT_RELIABILITY_QOS == pstDiscWriter->stDataWriterQos.reliability.kind)
        {
            /* Nephalem 释放pstHistoryData，否则内存泄漏 */
            DDS_STATIC_FREE(pstHistoryData->pstFragData->data);
            pstHistoryData->pstFragData->data = NULL;
            DDS_STATIC_FREE(pstHistoryData->pstFragData);
            pstHistoryData->pstFragData = NULL;
            DDS_STATIC_FREE(pstHistoryData);
            pstHistoryData = NULL;

            PrintLog(COMMLOG_WARN, "HandleUSERWriterDataMsg:Reader and writer states do not match.\n");
            return TRUE;
        }

        /* JD 2021-11-26 分片报文不能在此处 对totHistoryNum进行++处理 */
        //pstDataReader->totHistoryNum++;
        /* 本地与远端都为有状态提交数据 */
        DeilverStatusFulUserMsg(pstDataReader, pstDiscWriter, pstHistoryData);

        pstDataReader = pstDataReader->pNext;
    }
	/* JD DataFrag数据提交时，会将pstHistoryData 数据拷贝一份在进行操作，所以需要在外面释放一下内存 */
	/* JD 2021-11-24 释放 pstHistoryData 的内存 */
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

    /* 获取远端写入器guid */
    stDiscWtGuid.prefix = pstRTPSMsgHeader->guidPrefix;
    stDiscWtGuid.entityId = pstGapMsg->writerId;

    /* 在线qos暂时不处理，直接过滤*/
    if (pstSubMsgHeader->flags[1])
    {
		stInlineQos.guid = GUID_UNKNOWN;
		stInlineQos.disposed = 0;
        NetworkByteOrderConvertInlineQos(pstMemBlock, &stInlineQos, bNeedExchangeBytes);
    }

    /* 查找该用户数据对应的远端写入器 */
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

            /* 回调函数，处理用户数据 */
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
