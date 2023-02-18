#include "../include/GlobalDefine.h"

/*---------------------------------------------------------------------------------
-   函 数 名: HandleSEDPDiscoveredWriterDataMsg
-   功能描述: 处理SEDP发现写入器报文
-   输    入: 子报文消息头、Data、内存块、Participant
-   返    回:
-   全局变量: 
-   注    释: 处理SEDP发现写入器报文
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2019\8\05       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL HandleSEDPDiscoveredWriterDataMsg(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader*  pstSubMsgHeader, Data* pstDataMsg, MemoryBlock* pstMemBlock, DDS_DomainParticipant* pstParticipant)
{
    DDS_DiscoveredWriter* pstTempDiscWriter = NULL;
	BuiltinDiscWriter*  pstBuiltinDiscWriter = NULL;
	HistoryData* pstTempHistoryData = NULL;
	HistoryData* pstHistoryData = NULL;
	InlineQos stInlineQos;
	DDS_Topic* pstTopic = NULL;
	DDS_DiscoveredWriter*  pstDiscWriter = NULL;
	DDS_DiscoveredParticipant* pstDiscParticipant = NULL;
	DDS_Topic* pstTempTopic = NULL;

	BOOL bNeedExchangeBytes;
	bNeedExchangeBytes = FALSE;
	if (IS_MACHINE_LITTLE_ENDIAN != pstSubMsgHeader->flags[0])
		bNeedExchangeBytes = TRUE;

    if (!pstSubMsgHeader->flags[1] && !(pstSubMsgHeader->flags[2] || pstSubMsgHeader->flags[3]))
    {
        pstMemBlock->readIndex = pstMemBlock->totalSize;
        PrintLog(COMMLOG_WARN, "SEDPDiscoveredWriter: SubMsg header is abnormal!\n");
        return FALSE;
    }

    /* 内建发布者writer */
    pstBuiltinDiscWriter = GetBuiltinDiscWriterFromBuiltinDataReaderByPrefix(&pstParticipant->stBuiltinDataReader[1], &pstRTPSMsgHeader->guidPrefix);
    if (NULL == pstBuiltinDiscWriter)
    {
        pstMemBlock->readIndex = pstMemBlock->totalSize;
        PrintLog(COMMLOG_WARN, "SEDPDiscoveredWriter: Not found SUB BuiltinDiscWriter !.\n");
        return FALSE;
    }

    /* Nephalem 判断数据是否接收过，重复的数据不再接收 */
    pstTempHistoryData = pstBuiltinDiscWriter->stHistoryCache.pstHead;
    while (NULL != pstTempHistoryData)
    {
        if (ISEQUAL_SEQUENCENUMBER(pstTempHistoryData->seqNum, pstDataMsg->writerSN))
        {
            //printf("Nephalem : pstDataMsg->writerSN %d repeat \n", pstDataMsg->writerSN.low);
            return TRUE;
        }

        pstTempHistoryData = pstTempHistoryData->pNext;
    }

    pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC(sizeof(HistoryData));
    if (NULL == pstHistoryData)
    {
        PrintLog(COMMLOG_ERROR, "SEDPDiscoveredWriter: Failed to create HistoryCache, Insufficient memory space.\n");
        return FALSE;
    }

    /* 初始化历史数据 */
    InitHistoryData(pstHistoryData);

    /* 记录seqNum */
    pstHistoryData->seqNum = pstDataMsg->writerSN;

    /* 序列在线qos */
    if (pstSubMsgHeader->flags[1])
    {
		stInlineQos.guid = GUID_UNKNOWN;
		stInlineQos.disposed = 0;
        NetworkByteOrderConvertInlineQos(pstMemBlock, &stInlineQos, bNeedExchangeBytes);
        if (stInlineQos.disposed)
        {
            pstTopic = pstParticipant->pstTopic;
            while (NULL != pstTopic)
            {
                /* 远端写入器清除 */
                DDS_MUTEX_LOCK(pstParticipant->m_discWtMutex);
                DeleteDiscWriterFromTopicByGuid(pstTopic, &stInlineQos.guid);
                DDS_MUTEX_UNLOCK(pstParticipant->m_discWtMutex);
                FindOwnerWriter(pstTopic);
                pstTopic = pstTopic->pNext;
            }
        }
    }

    /* 处理发布报文 */
    if (pstSubMsgHeader->flags[2] || pstSubMsgHeader->flags[3])
    {
        /* 创建主题申请内存 */
        pstTopic = (DDS_Topic*)DDS_STATIC_MALLOC(sizeof(DDS_Topic));
        if (NULL == pstTopic)
        {
            DDS_STATIC_FREE(pstHistoryData);
            PrintLog(COMMLOG_ERROR, "SEDPDiscoveredWriter: Failed to create remote topic, Insufficient memory space.\n");
            return FALSE;
        }

        /* 初始化主题 */
        DDS_Topic_Init(pstTopic);

        /* 创建远端写入器申请内存 */
        pstDiscWriter = (DDS_DiscoveredWriter*)DDS_STATIC_MALLOC(sizeof(DDS_DiscoveredWriter));
        if (NULL == pstDiscWriter)
        {
            DDS_STATIC_FREE(pstHistoryData);
            DDS_STATIC_FREE(pstTopic);
            PrintLog(COMMLOG_ERROR, "SEDPDiscoveredWriter: Failed to create remote DiscoveredWriter, Insufficient memory space.\n");
            return FALSE;
        }

        /* 初始化写入器 */
        DDS_DiscoveredWriter_Init(pstDiscWriter);

        /* 序列化data present */
        DESERIALIZE_SERIALIZEDDATA(pstDataMsg->serializedPayload, (*pstMemBlock), bNeedExchangeBytes);
        NetworkByteOrderConvertDiscDataWriter(pstMemBlock, pstTopic, pstDiscWriter, bNeedExchangeBytes);

        /* Nephalem 远端阅读器自身相关的主题设置信息 */
        pstDiscWriter->pstRelateTopic = pstTopic;

        /* Participant地址挂到对应的阅读器上 */
        pstDiscParticipant = pstParticipant->pstDiscParticipant;
        while (NULL != pstDiscParticipant)
        {
            if (GuidPrefix_Is_Equal(&pstDiscParticipant->stRTPSMsgHeader.guidPrefix, &pstDiscWriter->guid.prefix))
            {
                pstDiscWriter->pstUnicastLocator = pstDiscParticipant->rtpsParticipantProxy.defaultUnicastLocator;
                break;
            }

            pstDiscParticipant = pstDiscParticipant->pNext;
        }

        /* 校验本地是否存在该主题 */
        pstTempTopic = pstParticipant->pstTopic;
        while (NULL != pstTempTopic)
        {
            /* 主题本地是否已存在 */
            if (!strcmp(pstTopic->topicName.value, pstTempTopic->topicName.value) && !strcmp(pstTopic->topicType.value, pstTempTopic->topicType.value))
            {
                /* 插入内建发布者reader,失败则清理 */
                if (!InsertHistoryCacheOrder(&pstBuiltinDiscWriter->stHistoryCache, pstHistoryData))
                {
                    DDS_STATIC_FREE(pstHistoryData);
                    pstHistoryData = NULL;
                }

                /* 若本地主题已存在该远端写入器，则释放内存直接放回 */
                pstTempDiscWriter = GetDiscoveredWriterFromTopicByGuid(pstTempTopic, &pstDiscWriter->guid);
                if (NULL != pstTempDiscWriter)
                {
                    PrintLog(COMMLOG_INFO, "SEDPDiscoveredWriter: The discovered writer is exist | topic name: %s.\n", pstTopic->topicName.value);

                    DDS_STATIC_FREE(pstDiscWriter);
                    pstDiscWriter = NULL;

                    /* Nephalem 更新TopicQoS */
                    if (pstTempTopic != pstTempDiscWriter->pstRelateTopic)
                    {
                        UnInitTopicPartitionQoS(&pstTempDiscWriter->pstRelateTopic->stTopicQos);
                        DDS_STATIC_FREE(pstTempDiscWriter->pstRelateTopic);
                        pstTempDiscWriter->pstRelateTopic = NULL;
                    }

                    /* Nephalem 远端阅读器自身相关的主题设置信息 */
                    pstTempDiscWriter->pstRelateTopic = pstTopic;

                    /* Nephalem partition 判断是否存在相同分区 */
                    pstTempDiscWriter->bSamePartiton = EqualPartitionName(&pstTopic->stTopicQos, &pstTempTopic->stTopicQos);

					/* Nephalem ownership 更新所有权强度最高的写入器 */
					FindOwnerWriter(pstTempTopic);

                    return TRUE;
                }

                ///* 释放新建的主题 */
                //DDS_STATIC_FREE(pstTopic);

				/* Nephalem partition 判断是否存在相同分区 */
                pstDiscWriter->bSamePartiton = EqualPartitionName(&pstTopic->stTopicQos, &pstTempTopic->stTopicQos);
   
                /* 插入父节点，方便访问上层数据结构 */
                pstDiscWriter->pstTopic = pstTempTopic;

                /* 远端写入器不存在，需插入 */
                LIST_INSERT_HEAD(pstTempTopic->pstDiscWriter, pstDiscWriter);
                
//            	printf("SEDP: Topic %s add remote DataWriter  %d.%d.%d.%d : %d \n",pstTopic->topicName.value,
//            			pstDiscWriter->guid.prefix.value[0],
//            			pstDiscWriter->guid.prefix.value[1],
//            			pstDiscWriter->guid.prefix.value[2],
//            			pstDiscWriter->guid.prefix.value[3],
//            			pstDataMsg->writerSN.low);

                /* Nephalem ownership 更新所有权强度最高的写入器 */
                FindOwnerWriter(pstTempTopic);
               
                return TRUE;
            }
            pstTempTopic = pstTempTopic->pNext;
        }

        /* 插入父节点，方便访问上层数据结构 */
        pstDiscWriter->pstTopic = pstTopic;

        /* Nephalem 新Topic恢复陈默认无partition状态 */
        UnInitTopicPartitionQoS(&pstTopic->stTopicQos);

        /* 远端写入器插入到新主题下*/
        LIST_INSERT_HEAD(pstTopic->pstDiscWriter, pstDiscWriter);

        /* 插入父节点，方便访问上层数据结构 */
        pstTopic->pstParticipant = pstParticipant;

        /* 该新主题插入到Participant下的主题链表 */
        LIST_INSERT_HEAD(pstParticipant->pstTopic, pstTopic);
        
    }
	
    /* 插入内建发布者reader,失败则清理 */
    if (!InsertHistoryCacheOrder(&pstBuiltinDiscWriter->stHistoryCache, pstHistoryData))
    {
        DDS_STATIC_FREE(pstHistoryData);
    }

    return TRUE;
}

/*---------------------------------------------------------------------------------
-   函 数 名: HandleSEDPDiscoveredReaderDataMsg
-   功能描述: 处理SEDP发现阅读器报文
-   输    入: 子报文消息头、Data、内存块、Participant
-   返    回:
-   全局变量:
-   注    释: 处理SEDP发现阅读器报文
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2019\8\05       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL HandleSEDPDiscoveredReaderDataMsg(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader*  pstSubMsgHeader, Data* pstDataMsg, MemoryBlock* pstMemBlock, DDS_DomainParticipant* pstParticipant)
{
    DDS_DiscoveredReader* pstTempDiscReader = NULL;
    DDS_DiscoveredReader* pstTwinDiscReader = NULL;
	BuiltinDiscWriter*  pstBuiltinDiscWriter = NULL; 
    DDS_DataWriter*  pstLocalWriter = NULL;
	HistoryData* pstTempHistoryData = NULL;
	HistoryData* pstHistoryData = NULL;
	DDS_Topic* pstTopic = NULL;
	InlineQos stInlineQos;
	DDS_DiscoveredReader*  pstDiscReader = NULL;
	DDS_DiscoveredParticipant* pstDiscParticipant = NULL;
	DDS_Topic* pstTempTopic = NULL;

	BOOL bNeedExchangeBytes;
	bNeedExchangeBytes = FALSE;
	if (IS_MACHINE_LITTLE_ENDIAN != pstSubMsgHeader->flags[0])
		bNeedExchangeBytes = TRUE;

    if (!pstSubMsgHeader->flags[1] && !(pstSubMsgHeader->flags[2] || pstSubMsgHeader->flags[3]))
    {
        pstMemBlock->readIndex = pstMemBlock->totalSize;
        PrintLog(COMMLOG_WARN, "SEDPDiscoveredReader:  SubMsg header is abnormal !\n");
        return FALSE;
    }

    /* 内建发布者reader */
    pstBuiltinDiscWriter = GetBuiltinDiscWriterFromBuiltinDataReaderByPrefix(&pstParticipant->stBuiltinDataReader[2], &pstRTPSMsgHeader->guidPrefix);
    if (NULL == pstBuiltinDiscWriter)
    {
        pstMemBlock->readIndex = pstMemBlock->totalSize;
        PrintLog(COMMLOG_WARN, "SEDPDiscoveredReader: Not found SUB BuiltinDiscWriter !\n");
        return FALSE;
    }

    /* Nephalem 判断数据是否接收过，重复的数据不再接收 */
    pstTempHistoryData = pstBuiltinDiscWriter->stHistoryCache.pstHead;
    while (NULL != pstTempHistoryData)
    {
        if (ISEQUAL_SEQUENCENUMBER(pstTempHistoryData->seqNum, pstDataMsg->writerSN))
        {
            //printf("Nephalem : pstDataMsg->writerSN %d repeat \n", pstDataMsg->writerSN.low);
            return TRUE;
        }

        pstTempHistoryData = pstTempHistoryData->pNext;
    }


    pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC(sizeof(HistoryData));
    if (NULL == pstHistoryData)
    {
        PrintLog(COMMLOG_ERROR, "SEDPDiscoveredReader: Failed to create HistoryCache, Insufficient memory space.\n");
        return FALSE;
    }

    /* 初始化历史数据 */
    InitHistoryData(pstHistoryData);

    /* 记录seqNum */
    pstHistoryData->seqNum = pstDataMsg->writerSN;

    /* 反序列化在线qos信息 */
    if (pstSubMsgHeader->flags[1])
    {
		stInlineQos.guid = GUID_UNKNOWN;
		stInlineQos.disposed = 0;
        NetworkByteOrderConvertInlineQos(pstMemBlock, &stInlineQos, bNeedExchangeBytes);

        if (stInlineQos.disposed)
        {
            pstTopic = pstParticipant->pstTopic;
            while (NULL != pstTopic)
            {
                /* 远端阅读器清除 */
                DDS_MUTEX_LOCK(pstParticipant->m_discRdMutex);
                DeleteDiscReaderFromTopicByGuid(pstTopic, &stInlineQos.guid);
                DDS_MUTEX_UNLOCK(pstParticipant->m_discRdMutex);
                FindOwnerWriter(pstTopic);
                pstTopic = pstTopic->pNext;
            }
        }
    }

    /* 处理订阅报文 */
    if (pstSubMsgHeader->flags[2] || pstSubMsgHeader->flags[3])
    {
        /* 创建主题申请内存 */
        pstTopic = (DDS_Topic*)DDS_STATIC_MALLOC(sizeof(DDS_Topic));
        if (NULL == pstTopic)
        {
            DDS_STATIC_FREE(pstHistoryData);
            PrintLog(COMMLOG_ERROR, "SEDPDiscoveredReader: Failed to create remote topic, Insufficient memory space.\n");
            return FALSE;
        }

        /* 初始化主题 */
        DDS_Topic_Init(pstTopic);

        /* 创建远端写入器申请内存 */
        pstDiscReader = (DDS_DiscoveredReader*)DDS_STATIC_MALLOC(sizeof(DDS_DiscoveredReader));
        if (NULL == pstDiscReader)
        {
            /* 失败需释放之前创建的主题并返回 */
            DDS_STATIC_FREE(pstHistoryData);
            DDS_STATIC_FREE(pstTopic);

            PrintLog(COMMLOG_ERROR, "SEDPDiscoveredReader: Failed to create remote DiscoveredWriter, Insufficient memory space.\n");
            return FALSE;
        }

        /* 初始化远端阅读器 */
        DDS_DiscoveredReader_Init(pstDiscReader);

        /* 反序列化data present */
        DESERIALIZE_SERIALIZEDDATA(pstDataMsg->serializedPayload, (*pstMemBlock), bNeedExchangeBytes);
        NetworkByteOrderConvertDiscDataReader(pstMemBlock, pstTopic, pstDiscReader, bNeedExchangeBytes);

        /* Nephalem 远端阅读器自身相关的主题设置信息 */
        pstDiscReader->pstRelateTopic = pstTopic;
    
        /* Participant地址挂到对应的阅读器上 */
        pstDiscParticipant = pstParticipant->pstDiscParticipant;
        while (NULL != pstDiscParticipant)
        {
            if (GuidPrefix_Is_Equal(&pstDiscParticipant->stRTPSMsgHeader.guidPrefix, &pstDiscReader->guid.prefix))
            {
                pstDiscReader->pstUnicastLocator = pstDiscParticipant->rtpsParticipantProxy.defaultUnicastLocator;
                break;
            }

            pstDiscParticipant = pstDiscParticipant->pNext;
        }

        /* 校验本地是否存在该主题 */
        pstTempTopic = pstParticipant->pstTopic;
        while (NULL != pstTempTopic)
        {
            /* 发现本地已创建该主题，校验主题类型是否一致*/
            if (!strcmp(pstTopic->topicName.value, pstTempTopic->topicName.value) && !strcmp(pstTopic->topicType.value, pstTempTopic->topicType.value))
            {
                /* 插入内建订阅者reader,已存在则清理 */
                if (!InsertHistoryCacheOrder(&pstBuiltinDiscWriter->stHistoryCache, pstHistoryData))
                {
                    DDS_STATIC_FREE(pstHistoryData);
                }

                /* 若本地主题已存在该远端写入器，则释放内存直接放回 */
                pstTempDiscReader = GetDiscoveredReaderFromTopicByGuid(pstTempTopic, &pstDiscReader->guid);
                if (NULL != pstTempDiscReader)
                {
                    PrintLog(COMMLOG_INFO, "SEDPDiscoveredReader: The discovered reader is exist | topic name: %s.\n", pstTopic->topicName.value);
                    DDS_STATIC_FREE(pstDiscReader);
                    pstDiscReader = NULL;

                    /* Nephalem 更新TopicQoS */
                    if (pstTempTopic != pstTempDiscReader->pstRelateTopic)
                    {
                        UnInitTopicPartitionQoS(&pstTempDiscReader->pstRelateTopic->stTopicQos);
                        DDS_STATIC_FREE(pstTempDiscReader->pstRelateTopic);
                        pstTempDiscReader->pstRelateTopic = NULL;
                    }

                    /* Nephalem 远端阅读器自身相关的主题设置信息 */
                    pstTempDiscReader->pstRelateTopic = pstTopic;

					/* Nephalem partition 判断是否存在相同分区 */
                    pstTempDiscReader->bSamePartiton = EqualPartitionName(&pstTopic->stTopicQos, &pstTempTopic->stTopicQos);

                    return TRUE;
                }

                ///* 释放新建的主题 */
                //DDS_STATIC_FREE(pstTopic);

				/* Nephalem partition 判断是否存在相同分区 */
                pstDiscReader->bSamePartiton = EqualPartitionName(&pstTopic->stTopicQos, &pstTempTopic->stTopicQos);
                       
                /* 孪生远端reader插入 */
                pstTempDiscReader =  GetDiscoveredReaderFromTopicByPrefix(pstTempTopic, &pstDiscReader->guid.prefix);
                if (NULL != pstTempDiscReader)
                {
                    LIST_INSERT_TAIL(pstTempDiscReader->pstTwinDiscReader, pstDiscReader, pstTwinDiscReader);
                    return TRUE;
                }

                /* 插入父节点，方便访问上层数据结构 */
                pstDiscReader->pstTopic = pstTempTopic;

                /* 远端写入器不存在，需插入 */
                LIST_INSERT_HEAD(pstTempTopic->pstDiscReader, pstDiscReader);
                
//            	printf("SEDP: Topic %s add remote DataReader  %d.%d.%d.%d : %d\n",pstTopic->topicName.value,
//            			pstDiscReader->guid.prefix.value[0],
//            			pstDiscReader->guid.prefix.value[1],
//            			pstDiscReader->guid.prefix.value[2],
//            			pstDiscReader->guid.prefix.value[3],
//            			pstDataMsg->writerSN.low);

                /* Nephalem ownership 更新所有权强度最高的写入器 */
                FindOwnerWriter(pstTempTopic);

                /* 远端reader添加本地writer */
                
                pstLocalWriter = pstTempTopic->pstLocalWriter;
                while (NULL != pstLocalWriter)
                {
                    DiscoveredReaderAddLocalWriter(pstDiscReader, &pstLocalWriter->guid);
                    pstLocalWriter = pstLocalWriter->pNext;
                }
                return TRUE;
            }
            pstTempTopic = pstTempTopic->pNext;
        }

        /* 插入父节点，方便访问上层数据结构 */
        pstDiscReader->pstTopic = pstTopic;

        /* Nephalem 新Topic恢复陈默认无partition状态 */
        UnInitTopicPartitionQoS(&pstTopic->stTopicQos);

        /* 远端写入器插入到新主题下*/
        LIST_INSERT_HEAD(pstTopic->pstDiscReader, pstDiscReader);

        /* 插入父节点，方便访问上层数据结构 */
        pstTopic->pstParticipant = pstParticipant;

        /* 该新主题插入到Participant下的主题链表 */
        LIST_INSERT_HEAD(pstParticipant->pstTopic, pstTopic);

    }

    /* 插入内建订阅者reader, 已存在则清理 */
    if (!InsertHistoryCacheOrder(&pstBuiltinDiscWriter->stHistoryCache, pstHistoryData))
    {
        DDS_STATIC_FREE(pstHistoryData);
    }

    return TRUE;
}

