#include "../include/GlobalDefine.h"
/*---------------------------------------------------------------------------------
-   函 数 名: DDS_DataReader_Init
-   功能描述: 阅读器初始化
-   输    入: 阅读器指针
-   返    回:
-   全局变量:
-   注    释: 阅读器初始化
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
        /* Nephalem 初始化*/
        pstDataReader->pstOwnerDiscWriter = NULL;
        pstDataReader->deadlineCallBack = NULL;
        pstDataReader->lastDataTime.sec = 0;
        pstDataReader->lastDataTime.nanosec = 0;
        pstDataReader->recvData = NULL;
        pstDataReader->recvCallBack = NULL;
    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_DataReader_Uninit
-   功能描述: 写入器初始化
-   输    入: 写入器指针
-   返    回:
-   全局变量:
-   注    释: 写入器初始化
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
-   函 数 名: DDS_DataReader_delete
-   功能描述: 阅读器删除
-   输    入: 阅读器指针
-   返    回:
-   全局变量:
-   注    释: 阅读器删除
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
-   函 数 名: JointAckNackMsgSendAssignWriter
-   功能描述: 给远端writer回复ack报文
-   输    入: 本地写入器指针
-   返    回:
-   全局变量:
-   注    释: 给远端writer回复ack报文
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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

    /* 网络字节序初始化 */
    INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

    /* 第一步序列化RTPS报文头 */
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

    /* 第二步序列化子报文头 */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* 第三步序列化子报文消息体 */
    SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

    /* 设置ACK报文头 */
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

    /* 设置ACK报文消息体 */
    pstDiscWriter->stAckNack.readerId = pstDataReader->guid.entityId;
    pstDiscWriter->stAckNack.writerId = pstDiscWriter->guid.entityId;
    pstDiscWriter->stAckNack.count++;

    /* 第二步序列化子报文头 */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* 第三步序列化子报文消息体 */
    SERIALIZE_ACKNACK(pstDiscWriter->stAckNack, stMemBlock);
    

    /* 子报文消息体长度，不带消息头 */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - pstDiscWriter->stAckNack.uiStartIndex;

    /* 序列化修改子报文长度 */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

    /* 发送ack报文 */
    RTPSSendMsg(&stMemBlock, &pstDataReader->pstTopic->pstParticipant->socketList.user_data_sender, pstDiscWriter->pstUnicastLocator);
}


/*---------------------------------------------------------------------------------
-   函 数 名: JointNackFragMsgSendAssignWriter
-   功能描述: 给远端writer回复Nackfrag报文
-   输    入: 本地写入器指针
-   返    回:
-   全局变量:
-   注    释: 给远端writer回复Nackfrag报文
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
    /* 网络字节序初始化 */
    INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

    /* 第一步序列化RTPS报文头 */
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

    /* 第二步序列化子报文头 */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* 第三步序列化子报文消息体 */
    SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

    /* 设置ACK报文头 */
    stSubMsgHeader.submessageId = NACK_FRAG;
    stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //finalFlag


    /* 设置ACK报文消息体 */
    stNackFrag.readerId = pstDataReader->guid.entityId;
    stNackFrag.writerId = pstDiscWriter->guid.entityId;
    stNackFrag.count = ++pstDiscWriter->stAckNack.count;

    /* Nephalem 查找指定writerSN的数据 */
    pstHistoryData = pstDiscWriter->stHistoryCache.pstHead;
    while (NULL != pstHistoryData)
    {
        if (ISEQUAL_SEQUENCENUMBER(writerSN, pstHistoryData->seqNum))
            break;
        pstHistoryData = pstHistoryData->pNext;
    }

    /* Nephalem writerSN的数据不存在 直接返回 */
    if (NULL == pstHistoryData)
        return;

    /* Nephalem 分片数据全部接收到，不回复 */
    if (pstHistoryData->uiFragTotal  == pstHistoryData->uiFragNum)
        return;

    /* Nephalem 非分片 */
    stNackFrag.writerSN = writerSN;
    stNackFrag.fragmentNumberState.base = pstHistoryData->uiFragNum;
    stNackFrag.fragmentNumberState.numBits = 0;

    while ((pstHistoryData->uiFragNum + uiNumbit <= pstHistoryData->uiFragTotal) && uiNumbit != 256)
    {
        /* Nephalem 分片数据丢失，更新fragmentNumberState */
        if (pstHistoryData->FragDataStatus[pstHistoryData->uiFragNum + uiNumbit] == 1)
        {    
            stNackFrag.fragmentNumberState.bitMap[uiNumbit >> 0x05] |= (0x80000000 >> (uiNumbit & 0x1f));
        }
        uiNumbit++;
        stNackFrag.fragmentNumberState.numBits++;
    }

    
    
   
    /* 第二步序列化子报文头 */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* 第三步序列化子报文消息体 */
    SERIALIZE_NACKFRAG(stNackFrag, stMemBlock);


    /* 子报文消息体长度，不带消息头 */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stNackFrag.uiStartIndex;

    /* 序列化修改子报文长度 */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

    /* 发送ack报文 */
    RTPSSendMsg(&stMemBlock, &pstDataReader->pstTopic->pstParticipant->socketList.user_data_sender, pstDiscWriter->pstUnicastLocator);
}

/*---------------------------------------------------------------------------------
-   函 数 名: GetMissSeqNum
-   功能描述: 获取丢失片段
-   输    入: 本地阅读器，远端写入器，心跳最大seqNum
-   返    回:
-   全局变量:
-   注    释: 获取丢失片段
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\14       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID GetDataReaderMissSeqNum(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter, SequenceNumber_t lastSN)
{
	SequenceNumber_t tempSN = SEQUENCENUMBER_ZERO;
    /* HistoryCache中seqNum是离散递增排列，expectSeqNum到lastSN是离散连续递增排列，根据这一特性，归并比较一次即可找出所有缺失片段，大数据递归、遍历乃编码大忌，骚年 切记切记 */
    HistoryData* pstHistoryData = pstDiscWriter->stHistoryCache.pstHead;
    UINT32 uiNumbit = 0;

    /* 集合清空，每一位都表示当前序号是否缺失 */
    memset(pstDiscWriter->stAckNack.readerSNState.bitMap, 0, SEQUENCE_BIT_NUMBER);
   
    tempSN = pstDiscWriter->stAckNack.readerSNState.base;

    /* 最大值比期望值小，说明未丢失 */
    if (ISLESSTHEN_SEQUENCENUMBER(lastSN, tempSN))
    {
        pstDiscWriter->stAckNack.readerSNState.numBits = 0;
        return;
    }

    while (!ISEQUAL_SEQUENCENUMBER(tempSN, Add_SequenceNumber(lastSN, SEQUENCENUMBER_START)))
    {
        if (NULL != pstHistoryData)
        {
            /* 缓存里有这条数据，跳过 */
            
            //printf("Nephalem %d - %d uiFragNum : %d , uiFragTotal = %d \n", tempSN.low, pstHistoryData->seqNum.low, pstHistoryData->uiFragNum, pstHistoryData->uiFragTotal);
            if (ISEQUAL_SEQUENCENUMBER(pstHistoryData->seqNum, tempSN))
            {
                /* Nephalem 分片数据未组装全时也记录为缺失 */
                if ((pstHistoryData->uiFragTotal > 0) && (pstHistoryData->uiFragNum != pstHistoryData->uiFragTotal))
                {
                    pstDiscWriter->stAckNack.readerSNState.bitMap[uiNumbit >> 0x05] |= (0x80000000 >> (uiNumbit & 0x1f));
                    
                    /* Nephalem 发送Ack报文之前 先发送NackFrag */
                    JointNackFragMsgSendAssignWriter(pstHistoryData->seqNum, pstDataReader, pstDiscWriter);
                }

                pstHistoryData = pstHistoryData->pNext;
            }
            /* 缓存里也没有这条数据，记录缺失 */
            else
            {
                /* 每一位记录是否有缺失，从base开始，往后记录连续256条是否有缺失, 位运算替代传统的除法与取模运算 */
                pstDiscWriter->stAckNack.readerSNState.bitMap[uiNumbit >> 0x05] |= (0x80000000 >> (uiNumbit & 0x1f));
            }
        }
        else
        {
            /* 缓存遍历完，剩下全是缺失，记录缺失 */
            pstDiscWriter->stAckNack.readerSNState.bitMap[uiNumbit >> 0x05] |= (0x80000000 >> (uiNumbit & 0x1f));
        }
        pstDiscWriter->stAckNack.readerSNState.numBits = uiNumbit + 1;

        tempSN = Add_SequenceNumber(tempSN, SEQUENCENUMBER_START);

        /* 计数器，超过256，即不在添加缺失，等待下次心跳处理 */
        uiNumbit++;
        if (256 == uiNumbit)
        {
            return;
        }
    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: CorrectAllHistoryCache
-   功能描述: reader总缓存达到depth上限后，清理所有writer的最旧缓存
-   输    入: 本地阅读器
-   返    回:
-   全局变量:
-   注    释: 
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
        /*总数量减少到了depth上限以下，结束提交*/
        if (pstDiscWriter->stHistoryCache.uiHistoryDataNum <= pstDataReader->stDataReaderQos.history.depth)
        {
            return;
        }
        else
        {
            /* 缓存数据剔除 */
			pstDiscWriter->stHistoryCache.pstHead = pstTempHistoryData->pNext;
            /* 缓存数据自减 */
            pstDiscWriter->stHistoryCache.uiHistoryDataNum--;

            pstDiscWriter->stAckNack.readerSNState.base = Add_SequenceNumber(pstTempHistoryData->seqNum, SEQUENCENUMBER_START);

            /* Nephalme 分片数据未组装齐全时 直接删除该HistoryData 不提交 */
            if ((pstTempHistoryData->uiFragTotal > 0) && (pstTempHistoryData->uiFragNum != pstTempHistoryData->uiFragTotal))
            {
                pstDelHistoryData = pstTempHistoryData;

                pstTempHistoryData = pstTempHistoryData->pNext;
                /* 释放缓存器中的数据区 */
                UninitHistoryData(pstDelHistoryData);
                /* 释放缓存器 */
                DDS_STATIC_FREE(pstDelHistoryData);        
                pstDelHistoryData = NULL;

                continue;
            }

#if defined QYDDS_MASSDATA
            /* Nephalem massdata分片数据进行组包 存放到MassDataHistoryCache，不进行直接提交 */
            if (pstTempHistoryData->massDataTotolSize > 0)
            {
                ProcessMassData(pstTempHistoryData, pstDataReader, pstDiscWriter);

                /* Nephalem massdata 数据处理完后进行释放 */
                pstDelHistoryData = pstTempHistoryData;
                pstTempHistoryData = pstTempHistoryData->pNext;
                DDS_STATIC_FREE(pstDelHistoryData->data);
                pstDelHistoryData->data = NULL;
                DDS_STATIC_FREE(pstDelHistoryData);
                pstDelHistoryData = NULL;
                continue;
            }
#endif 
            /* 提交到用户 */
            DOUBLE_INSERT_TAIL(pstDataReader->pstTopic->stUserData.pstHead, pstDataReader->pstTopic->stUserData.pstTail, pstTempHistoryData);
                /* 每提交一次，用户数据个数自增 */
            pstDataReader->pstTopic->stUserData.uiHistoryDataNum++;

        }

        pstTempHistoryData = pstNextHistoryData;
    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: CorrectDiscoveredWriterHistoryCache
-   功能描述: 矫正远端写入器缓存与提交
-   输    入: 本地阅读器、远端写入器
-   返    回:
-   全局变量:
-   注    释: 矫正远端写入器缓存与提交
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\14       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID CorrectDiscoveredWriterHistoryCache(DDS_DataReader* pstDataReader, DDS_DiscoveredWriter* pstDiscWriter)
{
	HistoryData* pstDelHistoryData = NULL;
	HistoryData* pstNextHistoryData = NULL;
    /* 远端写入器缓存为离散递增，根据这一特性快速提交数据以及更新期望 */
    HistoryData* pstTempHistoryData = pstDiscWriter->stHistoryCache.pstHead;
	HistoryData* pstPreHistoryData = pstDiscWriter->stHistoryCache.pstHead; /* JD 2021-12-01 前置节点 */
	SequenceNumber_t baseSeqNum = pstDiscWriter->stAckNack.readerSNState.base;

    while (NULL != pstTempHistoryData)
    {
		pstNextHistoryData = pstTempHistoryData->pNext;
        /* 缓存数据小于期望值，无论顺序全部提交 */
		if (ISLESSTHEN_SEQUENCENUMBER(pstTempHistoryData->seqNum, baseSeqNum))
        {
			/* JD 2021-12-01 可以提交数据，先将可以提交的数据从pDisDataWriter中删除 */
			pstDiscWriter->stHistoryCache.uiHistoryDataNum--;
			if (pstTempHistoryData == pstDiscWriter->stHistoryCache.pstHead)
			{ /* 当前提交数据为首数据 */
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
			{/* 当前提交数据为尾数据*/
				pstDiscWriter->stHistoryCache.pstTail = pstPreHistoryData;
				pstPreHistoryData->pNext = NULL;
				pstTempHistoryData->pNext = NULL;
			}
			else
			{ /* 当前数据为中间数据 */
				pstPreHistoryData->pNext = pstTempHistoryData->pNext;
				pstTempHistoryData->pNext = NULL;
			}

            /* Nephalme 分片数据未组装齐全时 直接删除该HistoryData 不提交 */
            if ((pstTempHistoryData->uiFragTotal > 0) && (pstTempHistoryData->uiRecvFragNum != pstTempHistoryData->uiFragTotal))
            {
                pstDelHistoryData = pstTempHistoryData;
                pstTempHistoryData = pstNextHistoryData;

                /* 释放缓存器中的数据区 */
                UninitHistoryData(pstDelHistoryData);
                /* 释放缓存器 */
                DDS_STATIC_FREE(pstDelHistoryData);
                pstDelHistoryData = NULL;
                continue;
            }

#if defined QYDDS_MASSDATA
            /* Nephalem massdata分片数据进行组包 存放到MassDataHistoryCache，不进行直接提交 */
            if (pstTempHistoryData->massDataTotolSize > 0)
            {
                ProcessMassData(pstTempHistoryData, pstDataReader, pstDiscWriter);

                /* Nephalem massdata 数据处理完后进行释放 */
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
            /* 每提交一次，用户数据个数自增 */
            pstDataReader->pstTopic->stUserData.uiHistoryDataNum++;
 
        }
        /* 缓存数据等于期望值，离散连续递增数据提交 */
		else if (ISEQUAL_SEQUENCENUMBER(pstTempHistoryData->seqNum, baseSeqNum))
        {
            /* Nephalme 分片数据未组装齐全时 不提交 */
            if ((pstTempHistoryData->uiFragTotal > 0) && pstTempHistoryData->uiRecvFragNum != pstTempHistoryData->uiFragTotal)
            {
                pstTempHistoryData = pstTempHistoryData->pNext;
                break;
            }
			/* JD 2021-12-01 可以提交数据，先将可以提交的数据从pDisDataWriter中删除 */
			pstDiscWriter->stHistoryCache.uiHistoryDataNum--;
			if (pstTempHistoryData == pstDiscWriter->stHistoryCache.pstHead)
			{ /* 当前提交数据为首数据 */
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
			{/* 当前提交数据为尾数据*/
				pstDiscWriter->stHistoryCache.pstTail = pstPreHistoryData;
				pstPreHistoryData->pNext = NULL;
				pstTempHistoryData->pNext = NULL;
			}
			else
			{ /* 当前数据为中间数据 */
				pstPreHistoryData->pNext = pstTempHistoryData->pNext;
				pstTempHistoryData->pNext = NULL;
			}
			
			/* 本地reader的期望读序号增加1 */
			pstDiscWriter->stAckNack.readerSNState.base = Add_SequenceNumber(pstDiscWriter->stAckNack.readerSNState.base, SEQUENCENUMBER_START);

#if defined QYDDS_MASSDATA
            /* Nephalem massdata分片数据进行组包 存放到MassDataHistoryCache，不进行直接提交 */
            if (pstTempHistoryData->massDataTotolSize > 0)
            {
                ProcessMassData(pstTempHistoryData, pstDataReader, pstDiscWriter);

                /* Nephalem massdata 数据处理完后进行释放 */
                pstDelHistoryData = pstTempHistoryData;
                pstTempHistoryData = pstTempHistoryData->pNext;
                DDS_STATIC_FREE(pstDelHistoryData->data);
                pstDelHistoryData->data = NULL;
                DDS_STATIC_FREE(pstDelHistoryData);
                pstDelHistoryData = NULL;
                continue;
            }
#endif 

			/* 提交到用户 */
		    DOUBLE_INSERT_TAIL(pstDataReader->pstTopic->stUserData.pstHead, pstDataReader->pstTopic->stUserData.pstTail, pstTempHistoryData);
            /* 每提交一次，用户数据个数自增 */
            pstDataReader->pstTopic->stUserData.uiHistoryDataNum++;
			baseSeqNum = Add_SequenceNumber(baseSeqNum, SEQUENCENUMBER_START);
        }
        /* 缓存数据大于期望值*/
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
        /*比Gap报文中最大丢失数据SeqNum小的，全部提交*/
        if (ISLESSTHEN_SEQUENCENUMBER(pstTempHistoryData->seqNum, gapEnd))
        {
            /* 清除时注意尾节点 */
            if (pstTempHistoryData == pstDiscWriter->stHistoryCache.pstTail)
            {
                pstDiscWriter->stHistoryCache.pstTail = NULL;
            }

            /* 缓存数据剔除 */
            pstDiscWriter->stHistoryCache.pstHead = pstTempHistoryData->pNext;

            /* 提交到用户 */
            DOUBLE_INSERT_TAIL(pstDataReader->pstTopic->stUserData.pstHead, pstDataReader->pstTopic->stUserData.pstTail, pstTempHistoryData);

            /* 每提交一次，用户数据个数自增 */
            pstDataReader->pstTopic->stUserData.uiHistoryDataNum++;

            /* 缓存数据自减 */
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
-   函 数 名: DeilverStatuslessUserMsg
-   功能描述: 提交无状态阅读器数据
-   输    入: 阅读器、远端写入器、用户缓存数据
-   返    回:
-   全局变量:
-   注    释: 提交无状态阅读器数据
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
			/* JD 2021-11-26 如果是分片报文，将传入参数的HistoryData 值拷贝一份后进行操作 */
			HistoryData* pstFragHistoryData = (HistoryData*)malloc(sizeof(HistoryData));
			InitHistoryData(pstFragHistoryData);
			CopyHistoryData(pstFragHistoryData, pstHistoryData);


			pstTempHistoryData = pstDiscWriter->stHistoryCache.pstHead;
			/* Nephalem 查找分片数据对应seqNum的HistoryData */
			while (NULL != pstTempHistoryData)
			{
				if (ISEQUAL_SEQUENCENUMBER(pstTempHistoryData->seqNum, pstFragHistoryData->seqNum))
					break;
				pstTempHistoryData = pstTempHistoryData->pNext;
			}

			//printf("RecvData SN = %d and FragNum = %d \n", pstHistoryData->seqNum.low, pstHistoryData->uiFragNum);
			//if (pstHistoryData->uiFragNum == 1) //JD 此方法判断在多个相同的DataReader情况下有问题
			if (NULL == pstTempHistoryData)
			{
				do {
					pstFragHistoryData->data = (CHAR*)DDS_STATIC_MALLOC_T(pstFragHistoryData->uiDataLen);

					if (NULL == pstFragHistoryData->data)
					{
						/* 用户数据申请内存失败需释放之前申请 */
						/* Nephalem 释放pstHistoryData，否则内存泄漏 */
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

					/* 缓存里有数据直接删除，没有添加 */
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

				/* Nephalem 该分片数据的分片序号与已保存的分片序号不是连续的 ）*/
				if (pstTempHistoryData->uiFragNum == (pstFragHistoryData->uiFragNum - 1))
				{

					pstTempHistoryData->uiRecvFragNum += 1;
					/* Nephalem 复制分片数据到HistoryDfata中 */
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
			// 每提交一次，用户数据个数自增 
			pstDataReader->pstTopic->stUserData.uiHistoryDataNum++;
		}
	}

    if (bDeilverFragDataFlag)
    {
        pstTempHistoryData = pstDiscWriter->stHistoryCache.pstHead;
        
        while (NULL != pstTempHistoryData)
        {
			pstNextHistoryData = pstTempHistoryData->pNext;


			/* JD 2021-12-01 可以提交数据，先将可以提交的数据从pDisDataWriter中删除pstDiscWriter->stHistoryCache */
			/* JD 2021-12-01 BEST_EFFORT时 pstDiscWriter->stHistoryCache总是可提交数据，不可提交数据都删除了 */
			pstDiscWriter->stHistoryCache.uiHistoryDataNum--;
			if (pstTempHistoryData == pstDiscWriter->stHistoryCache.pstHead)
			{ /* 当前提交数据为首数据 */
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

            /*更新期望*/
            pstDiscWriter->stAckNack.readerSNState.base = Add_SequenceNumber(pstTempHistoryData->seqNum, SEQUENCENUMBER_START);
           

            /* Nephalme 分片数据未组装齐全时 直接删除该HistoryData 不提交 */
            if ((pstTempHistoryData->uiFragTotal > 0) && (pstTempHistoryData->uiRecvFragNum != pstTempHistoryData->uiFragTotal))
            {
                pstDelHistoryData = pstTempHistoryData;

                pstTempHistoryData = pstTempHistoryData->pNext;
                /* 释放缓存器中的数据区 */
                DDS_STATIC_FREE(pstDelHistoryData->data);
                pstDelHistoryData->data = NULL;
                /* 释放缓存器 */
                DDS_STATIC_FREE(pstDelHistoryData);
                pstDelHistoryData = NULL;
  
                continue;
            }

			/* 提交到用户 */
			DOUBLE_INSERT_TAIL(pstDataReader->pstTopic->stUserData.pstHead, pstDataReader->pstTopic->stUserData.pstTail, pstTempHistoryData);
            /* 每提交一次，用户数据个数自增 */
            pstDataReader->pstTopic->stUserData.uiHistoryDataNum++;

            pstTempHistoryData = pstNextHistoryData;
        }
    }

    /* 回调函数，处理用户数据 */
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
-   函 数 名: DeilverStatusFulUserMsg
-   功能描述: 提交有状态阅读器数据
-   输    入: 阅读器、远端写入器、用户缓存数据
-   返    回:
-   全局变量:
-   注    释: 提交有状态阅读器数据
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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

		/* Nephalem resource_limits KEEP_ALL下只接收一定范围内序号的数据 */
	if (KEEP_ALL_HISTORY_QOS == pstDataReader->stDataReaderQos.history.kind
		&& LENGTH_UNLIMITED != pstDataReader->stDataReaderQos.resource_limits.max_samples)
	{
		/* Nephalem 接收的最大数据编号为 readerSNState.base + resource_limits.max_samples */
		maxSeq = SEQUENCENUMBER_ZERO;
		maxSeq.low = pstDataReader->stDataReaderQos.resource_limits.max_samples;
		maxSeq = Add_SequenceNumber(pstDiscWriter->stAckNack.readerSNState.base, maxSeq);

		/* Nephalem 数据编号大于可接收数据的最大编号，拒绝接收 */
		if (ISLESSTHEN_SEQUENCENUMBER(maxSeq, pstHistoryData->seqNum))
		{
			UninitHistoryData(pstHistoryData);
			DDS_STATIC_FREE(pstHistoryData);
			pstHistoryData = NULL;
			return;
		}
	}

	//只有位于头部的本地Reader才会保存数据到HistoryCache中，满足提交条件时将数据地址传到Topic的HistoryCache中，数据始终保存在头部本地Reader的缓存
	if (pstDataReader == pstDataReader->pstTopic->pstLocalReader) 
	{
		/* Nephalem 分包数据处理 */
		if (pstHistoryData->uiFragTotal > 0)
		{
			/* JD 2021-11-25 如果是分片报文，将传入参数的HistoryData 值拷贝一份后进行操作 */
			HistoryData* pstFragHistoryData = (HistoryData*)malloc(sizeof(HistoryData));
			InitHistoryData(pstFragHistoryData);
			CopyHistoryData(pstFragHistoryData, pstHistoryData);

			//printf("Nephalem : Recv %d.%d \n", pstHistoryData->seqNum.low, pstHistoryData->uiFragNum);
			pstTempHistoryData = pstDiscWriter->stHistoryCache.pstHead;
			/* Nephalem 查找分片数据对应seqNum的HistoryData */
			while (NULL != pstTempHistoryData)
			{
				if (ISEQUAL_SEQUENCENUMBER(pstTempHistoryData->seqNum, pstHistoryData->seqNum))
					break;
				pstTempHistoryData = pstTempHistoryData->pNext;
			}
			/* Nephalem 无法找到该分片数据的HistoryData */
			if (NULL == pstTempHistoryData)
			{
				/* JD 2021-11-26 接收到分片数据进行+1*/
				pstFragHistoryData->uiRecvFragNum += 1;
				/* Nephalem 申请数组，存放数据接收状态 */
				pstFragHistoryData->FragDataStatus = (CHAR*)DDS_STATIC_MALLOC_T(pstFragHistoryData->uiFragTotal + 1);
				/* Nephalem 第0位无意义，所有分片数据状态设置为1，表示未收到 */
				memset(pstFragHistoryData->FragDataStatus, 1, pstFragHistoryData->uiFragTotal + 1);

				pstFragHistoryData->data = (CHAR*)DDS_STATIC_MALLOC_T(pstFragHistoryData->uiDataLen);

				if (NULL == pstFragHistoryData->data)
				{
					/* 用户数据申请内存失败需释放之前申请 */
					/* Nephalem 释放pstFragHistoryData，否则内存泄漏 */
					UninitHistoryData(pstFragHistoryData);
					DDS_STATIC_FREE(pstFragHistoryData);
					pstFragHistoryData = NULL;
					return;
				}

				/* Nephalem 向缓存里添加数据 */
				InsertHistoryCacheOrder(&pstDiscWriter->stHistoryCache, pstFragHistoryData);

				/* Nephalem 当前分片数据设置为已接收 */
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
				/* JD 2021-11-26 判断数据是否已经接收，如果已经接收则不进行处理 */
				if (pstTempHistoryData->FragDataStatus[pstFragHistoryData->uiFragNum] != 0)
				{
					pstTempHistoryData->uiRecvFragNum += 1;
					/* Nephalem 当前分片数据设置为已接收 */
					pstTempHistoryData->FragDataStatus[pstFragHistoryData->uiFragNum] = 0;

					memcpy(pstTempHistoryData->data + pstFragHistoryData->uiFragLen * (pstFragHistoryData->uiFragNum - 1), pstFragHistoryData->pstFragData->data, pstFragHistoryData->pstFragData->usDataLen);

					UninitHistoryData(pstFragHistoryData);

					DDS_STATIC_FREE(pstFragHistoryData);
					pstFragHistoryData = NULL;

					/* Nephalem pstHistoryData->uiFragTotal + 1表示所有分片都已收到 */
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
			/* Nephalem 将HistoryData插入到链表中 */
			/*DeadLine数据直接提交*/
			if (strcmp(pstHistoryData->data, "DeadLine MSG.") == 0)
			{
				/* 提交到用户 */
				DOUBLE_INSERT_TAIL(pstDataReader->pstTopic->stUserData.pstHead, pstDataReader->pstTopic->stUserData.pstTail, pstHistoryData);
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
				return;
			}

			/* 缓存里有数据直接删除，没有添加 */
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
			/* 超出history上限，清理最旧缓存 */
			if (pstDiscWriter->stHistoryCache.uiHistoryDataNum > pstDataReader->stDataReaderQos.history.depth)
				CorrectOldHistoryCache(pstDataReader, pstDiscWriter);
			else/* 缓存上送 */
				CorrectDiscoveredWriterHistoryCache(pstDataReader, pstDiscWriter);
		}
		/* Nephalem resource_limits  KEEP_ALL 不删除任何数据 */
		else if (KEEP_ALL_HISTORY_QOS == pstDataReader->stDataReaderQos.history.kind)
		{
			/* Nephalem KeepAll 下不可能超出history上限，缓存上送 */
			CorrectDiscoveredWriterHistoryCache(pstDataReader, pstDiscWriter);
		}

	}

    if (pstDataReader->pstTopic->stUserData.pstHead)
    {
            /* 回调函数，处理用户数据 */
            if (NULL != pstDataReader->recvCallBack)
            {
				/* 本地与远端都为有状态提交数据 */
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
-   函 数 名: DeilverStatusFulUserMsgWhenExpectSeqNumUnderFirstSN
-   功能描述: 提交有状态阅读器数据，当期望seqNum比心跳firstSN还小
-   输    入: 阅读器、远端写入器、用户缓存数据
-   返    回:
-   全局变量:
-   注    释: 提交有状态阅读器数据，当期望seqNum比心跳firstSN还小
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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

    /* 调整期望值大小，为心跳最小值 */
    pstDiscWriter->stAckNack.readerSNState.base = firstSN;

    /* 矫正缓存*/
    if (pstDataReader->pstTopic->stUserData.uiHistoryDataNum > 0)
    {
        CorrectDiscoveredWriterHistoryCache(pstDataReader, pstDiscWriter);
    }
    if (pstDataReader->pstTopic->stUserData.pstHead)
    {
        /* 回调函数，处理用户数据 */
        if (NULL != pstDataReader->recvCallBack)
        {
            (pstDataReader->recvCallBack)(pstDataReader);
        }
        UninitHistoryCache(&pstDiscWriter->pstTopic->stUserData);
    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_DataReader_take
-   功能描述: 获取用户数据
-   输    入: 阅读器、DataSeq
-   返    回: DDS_RETCODE_OK、DDS_RETCODE_NO_DATA
-   全局变量:
-   注    释: 获取用户数据
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
    /* 缓存数据个数赋值 */
    if (strcmp(tmpHistoryData->data, "DeadLine MSG.") == 0)
    {
        pstDataSeq->length = 1;
    }
    else
    {
        pstDataSeq->length = pstDataReader->pstTopic->stUserData.uiHistoryDataNum;
    }
    /* 缓存链表首地址 */
    pstDataSeq->pstHead = tmpHistoryData;

    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_DataReader_take
-   功能描述: 获取用户数据
-   输    入: 阅读器、DataSeq
-   返    回: DDS_RETCODE_OK、DDS_RETCODE_NO_DATA
-   全局变量:
-   注    释: 获取用户数据
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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

    /* 缓存数据个数赋值 */
    pstDataSeq->length = pstDataReader->pstTopic->stUserData.uiHistoryDataNum;

    /* 缓存链表首地址 */
    pstDataSeq->pstHead = pstDataReader->pstTopic->stUserData.pstHead;

    return pstDataSeq;
}

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_StringSeq_length
-   功能描述: 返回用户数据
-   输    入: DataSeq、数据编号
-   返    回: 用户数据指针
-   全局变量:
-   注    释: 返回用户数据
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
-   函 数 名: DDS_StringSeq_get
-   功能描述: 返回用户数据
-   输    入: DataSeq、数据编号
-   返    回: 用户数据指针
-   全局变量:
-   注    释: 返回用户数据
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
-   函 数 名: DDS_StringSeq_get
-   功能描述: 返回用户数据
-   输    入: DataSeq、数据编号
-   返    回: 用户数据指针
-   全局变量:
-   注    释: 返回用户数据
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
-   函 数 名: DDS_StringSeq_get
-   功能描述: 返回用户数据
-   输    入: DataSeq、数据编号
-   返    回: 用户数据指针
-   全局变量:
-   注    释: 返回用户数据
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
-   函 数 名: DDS_StringSeq_get
-   功能描述: 返回用户数据
-   输    入: DataSeq、数据编号
-   返    回: 用户数据指针
-   全局变量:
-   注    释: 返回用户数据
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
-   函 数 名: DDS_StringDataReader_return_loan
-   功能描述: 用户数据资源释放
-   输    入: 本地阅读器
-   返    回: DDS_RETCODE_OK、DDS_RETCODE_NO_DATA
-   全局变量:
-   注    释: 用户数据资源释放, 获取用户数据后必须调用释放，否则出错
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\16       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t DDS_StringDataReader_return_loan(DDS_DataReader* pstDataReader, DataSeq* pstDataSeq)
{
	UINT32 uiNum;
	HistoryData* pstDelHistoryData = NULL;
	HistoryData* pstHistoryData = NULL;
	DDS_DataReader* tmpReader = NULL;
	/* 提交过的数据的DataReader 的数据数目置为0 */
	pstDataReader->totHistoryNum = 0;

    tmpReader = pstDataReader->pstTopic->pstLocalReader;
	if(NULL != pstDataReader->pNext) //如果还有其他reader，直接返回不释放数据
		return DDS_RETCODE_OK;
  //  while (tmpReader!=NULL)
  //  {
		///* JD 2021-12-01 修改判断是否所有的数据都提交过了，有DataReader的数据没有进行提交数据则返回 */
		////if (tmpReader->totHistoryNum == 0)
  //      if (tmpReader->totHistoryNum > 0)
  //      {
  //          return DDS_RETCODE_OK;
  //      }
  //      tmpReader = tmpReader->pNext;
  //  }
	/* JD 2021-12-01 注释 */
    //tmpReader = pstDataReader->pstTopic->pstLocalReader;
    //while (tmpReader != NULL)
    //{
    //    tmpReader->totHistoryNum--;
    //    tmpReader = tmpReader->pNext;
    //}
	/* JD 2021-12-01 所有DataReader都提交了，开始删除数据 */
    pstHistoryData = pstDataSeq->pstHead;

    for (uiNum = 0; uiNum < pstDataSeq->length; uiNum++)
    {
        pstDelHistoryData = pstHistoryData;

        pstHistoryData = pstHistoryData->pNext;

        /* 释放缓存器中的数据区 */
        UninitHistoryData(pstDelHistoryData);

        /* 释放缓存器 */
        DDS_STATIC_FREE(pstDelHistoryData);
		pstDelHistoryData = NULL;
    }

    /* 缓存初始化 */
    InitHistoryCache(&pstDataReader->pstTopic->stUserData);

    return DDS_RETCODE_OK;
}

#if defined QYDDS_MASSDATA
/*---------------------------------------------------------------------------------
-   函 数 名: ProcessMassData
-   功能描述: 对MassData 分片数据进行组包
-   输    入: 本地阅读器
-   返    回: 
-   全局变量:
-   注    释: 
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
    /* Nephalem massdata 分片数据为首包数据*/
    if (ISEQUAL_SEQUENCENUMBER(pstTempHistoryData->massDataBaseSeq, pstTempHistoryData->seqNum))
    {
        /* Nephalem massdata pstTempHistoryData会在外部统一释放，重新申请空间*/
        pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC_T(sizeof(HistoryData));
        if (NULL == pstHistoryData)
        {
            PrintLog(COMMLOG_ERROR, "HandleUSERWriterDataMsg:Failed to create HistoryData, Insufficient memory space.\n");
            return;
        }

        /* Nephalem 初始化 */
        InitHistoryData(pstHistoryData);

        *pstHistoryData = *pstTempHistoryData;

        /* Nephalem 指针不进行赋值 */
        pstHistoryData->pstFragData = NULL;
        pstHistoryData->pNext = NULL;

        pstHistoryData->massDataRecvNum ++;
        pstHistoryData->uiDataLen = pstHistoryData->massDataTotolSize;

        /* Nephalme massdata 重新分片数据内存为数据总大小 */
        pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC_T(pstHistoryData->massDataTotolSize);
        if (NULL == pstHistoryData->data)
        {
            PrintLog(COMMLOG_ERROR, "HandleUSERWriterDataMsg:Failed to create HistoryData->data, Insufficient memory space.\n");
            return;
        }

        /* Nephalem massdata 将数据复制到新空间中 */
        memcpy(pstHistoryData->data, pstTempHistoryData->data, pstTempHistoryData->uiDataLen);

        /* Nephalem 到达historyQos设置上限，缓冲区满时释放旧数据 */
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
        /* Nephalem massdata将HistoryData 插入列表  */
        if (!InsertHistoryCacheOrder(&pstDiscWriter->stMassDataHistoryCache, pstHistoryData))
        {
            PrintLog(COMMLOG_INFO, "HandleUSERWriterDataMsg:User data was not inserted, Because user data already exists.\n");
        }

    }
    /* Nephalem massdata 非首包分片数据*/
    else
    {
        pstTempMassDataHistoryData = pstDiscWriter->stMassDataHistoryCache.pstHead;
        while (NULL != pstTempMassDataHistoryData)
        {
            /* Nephalem massdata 数据编号小于基地址，肯定不为后续数据的分片数据*/
            if (ISLESSTHEN_SEQUENCENUMBER(pstTempHistoryData->seqNum, pstTempMassDataHistoryData->seqNum))
            {
                break;
            }

            /* Nephalem massdata 当前数据期待的下一个分片数据的编号 */
            RecvSeqNum = SEQUENCENUMBER_ZERO;
            RecvSeqNum.low = pstTempMassDataHistoryData->massDataRecvNum;
            nextSeqNum = Add_SequenceNumber(pstTempMassDataHistoryData->massDataBaseSeq, RecvSeqNum);

            /* Nephalem massdata 组装数据 */
            if (ISEQUAL_SEQUENCENUMBER(nextSeqNum, pstTempHistoryData->seqNum))
            {
                //printf("Nephalem : Memcpy SeqNum = %d \n", pstTempHistoryData->seqNum.low);

                memcpy(pstTempMassDataHistoryData->data + (63 * 1024) * pstTempMassDataHistoryData->massDataRecvNum, pstTempHistoryData->data, pstTempHistoryData->uiDataLen);

                /* Nephalem massdata已接收数据自增一 */
                pstTempMassDataHistoryData->massDataRecvNum++;
                break;
            }
            pstTempMassDataHistoryData = pstTempMassDataHistoryData->pNext;
        }

    }

    /* Nepalem massdata 存在组包数据，检查数据是否组包完成 */
    if (0 < pstDiscWriter->stMassDataHistoryCache.uiHistoryDataNum &&
        pstDiscWriter->stMassDataHistoryCache.pstHead->massDataTotal == pstDiscWriter->stMassDataHistoryCache.pstHead->massDataRecvNum)
    {      
        //printf("Nephalem : notify SeqNum = %d \n", pstDiscWriter->stMassDataHistoryCache.pstHead->seqNum.low);

        pstTempMassDataHistoryData = pstDiscWriter->stMassDataHistoryCache.pstHead;
        /* Nephalem massdata 用户数据个数自增 */
        pstDiscWriter->pstTopic->stUserData.uiHistoryDataNum++;
        /* Nephalme massdata 将该数据从MassDataHistoryCache剔除 */
        pstDiscWriter->stMassDataHistoryCache.pstHead = pstDiscWriter->stMassDataHistoryCache.pstHead->pNext;
        /* Nephalem massdata MassDataHistoryCache中数据数量少1 */
        pstDiscWriter->stMassDataHistoryCache.uiHistoryDataNum--;
        /* Nephalem massdata DOUBLE_INSERT_TAIL会破坏原有链表关系 */
        /* Nephalem massdata 提交组包完成的数据 */
        DOUBLE_INSERT_TAIL(pstDiscWriter->pstTopic->stUserData.pstHead, pstDiscWriter->pstTopic->stUserData.pstTail, pstTempMassDataHistoryData);
    }
}

#endif
