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
-   函 数 名: DDS_DataWriter_Init
-   功能描述: 初始化写入器
-   输    入: 写入器指针
-   返    回: 
-   全局变量:
-   注    释: 初始化写入器属性信息
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
-   函 数 名: DDS_DataWriter_Uninit
-   功能描述: 析构写入器
-   输    入: 写入器指针
-   返    回:
-   全局变量:
-   注    释: 析构写入器
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
-   函 数 名: DDS_DataWriter_delete
-   功能描述: 写入器删除
-   输    入: 写入器指针
-   返    回:
-   全局变量:
-   注    释: 写入器删除
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
-   函 数 名: JointHeartbeatMsgSendUnknownReader
-   功能描述: 组装心跳报文发送未指定阅读器
-   输    入: 本地DomainParticipant指针
-   返    回:
-   全局变量:
-   注    释: 组装心跳报文发送指定阅读器
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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

    /* 本地无缓存，不发送心跳 */
    if (NULL == pstDataWriter->stHistoryCache.pstHead)
    {
        return;
    }

    /* 网络字节序初始化，缓存发布报文 */
    INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

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
    stSubMsgHeader.flags[2] = FALSE;    //dataFlag
    stSubMsgHeader.flags[3] = FALSE;   //keyFlag
    stInfoTS.value = GetNowTime();

    /* 序列化时间报文头 */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* 序列化时间子报文体 */
    SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

    /* 心跳报文头 */
    stSubMsgHeader.submessageId = HEARTBEAT;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;

    /* 心跳报文消息体 */
    pstDataWriter->stHeartbeat.readerId = ENTITYID_UNKNOWN;
    pstDataWriter->stHeartbeat.writerId = pstDataWriter->guid.entityId;
    pstDataWriter->stHeartbeat.firstSN = pstDataWriter->stHistoryCache.pstHead->seqNum;
    pstDataWriter->stHeartbeat.lastSN = pstDataWriter->stHistoryCache.pstTail->seqNum;
    pstDataWriter->stHeartbeat.count++;

    /* 序列化用户数据子报文头 */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* 序列化用户数据子报文消息体，同时探针记录readerId位置 */
    SERIALIZE_HEARTBEAT(pstDataWriter->stHeartbeat, stMemBlock);

    /* 计算子报文消息体长度，不带消息头 */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - pstDataWriter->stHeartbeat.uiStartIndex;

    /* 序列化修改子报文长度 */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

    pstDiscReader = pstDataWriter->pstTopic->pstDiscReader;
    while (NULL != pstDiscReader)
    {
        /* Nephalem parition 不是同一分区不发送 */
        if (FALSE == pstDiscReader->bSamePartiton)
        {
            pstDiscReader = pstDiscReader->pNext;
            continue;
        }

        /* 只给有状态发心跳 */
        if (RELIABLE_RELIABILITY_QOS == pstDiscReader->stDataReaderQos.reliability.kind)
        {
            /* 序列化修改子阅读器id */
            INITIAL_MEMORYBLOCK(stMemBlockMod, pstDataWriter->stHeartbeat.pcReadId, sizeof(EntityId_t));

            SERIALIZE_ENTITYID(pstDiscReader->guid.entityId, stMemBlockMod);

            if (!pstParticipant->bLoopBack && GuidPrefix_Is_Equal(&pstParticipant->stRTPSMsgHeader.guidPrefix, &pstDiscReader->guid.prefix))
            {
                pstDiscReader = pstDiscReader->pNext;
                continue;
            }

            /* 发送到对应的每个远端阅读器 */
            RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscReader->pstUnicastLocator);
        }

        pstDiscReader = pstDiscReader->pNext;
    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: JointHeartbeatMsgSendAssignReader
-   功能描述: 组装心跳报文发送指定阅读器
-   输    入: 本地DomainParticipant指针
-   返    回:
-   全局变量:
-   注    释: 组装心跳报文发送指定阅读器
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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

    /* 本地无缓存，不发送心跳 */
    if (NULL == pstDataWriter->stHistoryCache.pstHead)
    {
        return;
    }

	/* Nephalem partition 不是同一分区不发送 */
	if (FALSE == pstDiscdReader->bSamePartiton)
		return ;

    /* 网络字节序初始化，缓存发布报文 */
    INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

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
    stInfoTS.value = GetNowTime();

    /* 序列化时间报文头 */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* 序列化时间子报文体 */
    SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

    /* 心跳报文头 */
    stSubMsgHeader.submessageId = HEARTBEAT;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;

    /* 心跳报文消息体 */
    pstDataWriter->stHeartbeat.readerId = pstDiscdReader->guid.entityId;
    pstDataWriter->stHeartbeat.writerId = pstDataWriter->guid.entityId;
    pstDataWriter->stHeartbeat.firstSN = pstDataWriter->stHistoryCache.pstHead->seqNum;
    pstDataWriter->stHeartbeat.lastSN = pstDataWriter->stHistoryCache.pstTail->seqNum;
    pstDataWriter->stHeartbeat.count++;

    /* 序列化用户数据子报文头 */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* 序列化用户数据子报文消息体，同时探针记录readerId位置 */
    SERIALIZE_HEARTBEAT(pstDataWriter->stHeartbeat, stMemBlock);

    /* 计算子报文消息体长度，不带消息头 */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - pstDataWriter->stHeartbeat.uiStartIndex;

    /* 序列化修改子报文长度 */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

    if (!pstParticipant->bLoopBack && GuidPrefix_Is_Equal(&pstParticipant->stRTPSMsgHeader.guidPrefix, &pstDiscdReader->guid.prefix))
    {
        return;
    }

    /* 发送到对应的远端阅读器 */
    RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscdReader->pstUnicastLocator);
}

/*---------------------------------------------------------------------------------
-   函 数 名: JointHeartbeatMsgSendAllReader
-   功能描述: 组装心跳报文发送所有阅读器
-   输    入: 本地DomainParticipant指针
-   返    回:
-   全局变量:
-   注    释: 组装心跳报文发送所有阅读器
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
-   函 数 名: JointHeartbeatFragMsgSendAssignReader
-   功能描述: 组装分片心跳报文发送指定阅读器
-   输    入: 
-   返    回:
-   全局变量:
-   注    释: 组装分片心跳报文发送指定阅读器
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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

    ///* 本地无缓存，不发送心跳 */
    //if (NULL == pstDataWriter->stHistoryCache.pstHead)
    //{
    //    return;
    //}

    /* Nephalem partition 不是同一分区不发送 */
    if (FALSE == pstDiscdReader->bSamePartiton)
        return;

    /* 网络字节序初始化，缓存发布报文 */
    INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

    /* 第一步序列化RTPS报文头 */
    SERIALIZE_RTPS_MESSAGE_HEADER(pstParticipant->stRTPSMsgHeader, stMemBlock);

    /* 时间报文头设置 */
    stSubMsgHeader.submessageId = INFO_TS;
    stSubMsgHeader.submessageLength = sizeof(Time_t);
    stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
    stSubMsgHeader.flags[2] = FALSE;   //dataFlag
    stSubMsgHeader.flags[3] = FALSE;   //keyFlag
    stInfoTS.value = GetNowTime();

    /* 序列化时间报文头 */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* 序列化时间子报文体 */
    SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

    /* 心跳报文头 */
    stSubMsgHeader.submessageId = HEARTBEAT_FRAG;
    stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;

     
    /* 分片心跳报文消息体 */
    stHeartbeatFrag.readerId = pstDiscdReader->guid.entityId;
    stHeartbeatFrag.writerId = pstDataWriter->guid.entityId;
    stHeartbeatFrag.writerSN = pstHistoryData->seqNum;
    stHeartbeatFrag.lastFragmentNum = pstHistoryData->uiFragTotal;
    stHeartbeatFrag.count = ++pstDataWriter->stHeartbeat.count;


    /* 序列化用户数据子报文头 */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* 序列化用户数据子报文消息体，同时探针记录readerId位置 */
    SERIALIZE_HEARTBEATFRAG(stHeartbeatFrag, stMemBlock);

    /* 计算子报文消息体长度，不带消息头 */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stHeartbeatFrag.uiStartIndex;

    /* 序列化修改子报文长度 */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

    if (!pstParticipant->bLoopBack && GuidPrefix_Is_Equal(&pstParticipant->stRTPSMsgHeader.guidPrefix, &pstDiscdReader->guid.prefix))
    {
        return;
    }

    /* 发送到对应的远端阅读器 */
    RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscdReader->pstUnicastLocator);
}


/*---------------------------------------------------------------------------------
-   函 数 名: RecoverMissSeqNum
-   功能描述: 复原丢失分片，进行网络重传
-   输    入: 本地写入器、远端阅读器，ack报文
-   返    回:
-   全局变量:
-   注    释: 复原丢失分片，进行网络重传
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
        /* 位运算计算bitMap中每一位值，大于0说明该位有效，通过偏移量计算出缺失 */
        if (0 < ((bitStart >> (uiNumbit >> 5))&(pstAckNackMsg->readerSNState.bitMap[uiNumbit >> 5])))
        {
            seqNumOffset.low = uiNumbit;
            seqNumMiss = Add_SequenceNumber(pstAckNackMsg->readerSNState.base, seqNumOffset);

#if defined TG 
			missSeqVec[missSeqVecPos++] = seqNumMiss;
#endif

#ifndef TG 
            /* 缺失分片重发 */
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
                //todo发送gap报文
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
-   函 数 名: RecoverWriterMissDataFrag
-   功能描述: 重传指定DataFrag数据
-   输    入: 本地写入器、远端阅读器，Nack报文
-   返    回:
-   全局变量:
-   注    释: 重传指定DataFrag数据
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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

    /* Nephalem 查找指定writerSN的数据 */
    pstHistoryData = pstDataWriter->stHistoryCache.pstHead;
    while (NULL != pstHistoryData)
    {
        if (ISEQUAL_SEQUENCENUMBER(pstNackFragMsg->writerSN, pstHistoryData->seqNum))
            break;
        pstHistoryData = pstHistoryData->pNext;
    }

    /* Nephalem writerSN的数据不存在 直接返回 */
    if (NULL == pstHistoryData)
        return;
	/* JD 2021-11-29 这个会导致大包数据的Heartbeat 和AckNack的报文的异常，在收到HEARTBEAT_FRAG回复NACK_FRAG，
	导致之后接收到的报文出现问题,问题描述：数据读者回复Nack_Frag报文后与数据写者断开链接，之后会导致数据写者一直保持者数据
	但是数据写者不会在发送Nack_Frag报文，但是数据读者会发送Acknack要求发送这个Data报文，DW始终无法重新发送这个Data报文，导致死循环 */
    pstHistoryData->bUseNackFrag = TRUE;
    
    /* Nephalem 列表为空时，直接插入 */
    if (NULL == pstHistoryData->pstNackReader)
    {
        pstHistoryData->pstNackReader = (NackFragReader*)DDS_STATIC_MALLOC_T(sizeof(NackFragReader));
        memcpy(&pstHistoryData->pstNackReader->stReaderID, &pstDiscReader->guid, sizeof(GUID_t));
        pstHistoryData->pstNackReader->pNext = NULL;
    }
    else
    {
        /* Nephalem 查找该reader是否添加到使用NackFrag重传列表中 */
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
        /* 位运算计算bitMap中每一位值，大于0说明该位有效，通过偏移量计算出缺失 */
        if (0 < ((bitStart >> (uiNumbit % 32))&(pstNackFragMsg->fragmentNumberState.bitMap[uiNumbit >> 5])))
        {
            /* 缺失分片重发 */
            JointUSERDataFragMsgSendAssignReader(pstDataWriter, pstDiscReader, pstHistoryData, pstNackFragMsg->fragmentNumberState.base + uiNumbit);
        }
    }
#if !defined(NO_HEARTBEAT)
	if(pstNackFragMsg->fragmentNumberState.numBits != 0)
		JointHeartbeatFragMsgSendAssignReader(pstHistoryData, pstDataWriter, pstDiscReader);
#endif
}
/*---------------------------------------------------------------------------------
-   函 数 名: DDS_StringDataWriter_write
-   功能描述: 用户数据发送接口
-   输    入: 写入器、用户数据、数据长度
-   返    回:
-   全局变量:
-   注    释: 用户数据发送接口
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
    /* Nephalem 将大包数据进行分片，多次发送 */
    if (length > DataFragSize && RELIABLE_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
    {
        DDS_ReturnCode_t retCode;

        INT32 iNum = length / DataFragSize;
		/*TODO：变量前置以支持VS2010的旧编译器*/
        HistoryData massDataInfo;
        int i = 0;
		if ((KEEP_LAST_HISTORY_QOS == pstDataWriter->stDataWriterQos.history.kind) && (pstDataWriter->stDataWriterQos.history.depth < iNum + 1))
            printf("Nephalem Warnning : history.depth is small \n");
        if (LENGTH_UNLIMITED != pstDataWriter->stDataWriterQos.resource_limits.max_samples)
        {
            if ((KEEP_ALL_HISTORY_QOS == pstDataWriter->stDataWriterQos.history.kind) && (pstDataWriter->stDataWriterQos.resource_limits.max_samples < iNum + 1))
                printf("Nephalem Warnning : resource_limits.max_samples is small \n");
        }

        /* Nephalem massdata 存放相关信息，用作参数*/
        /* Nephalem massdata的基序号以等于当前分片数据的首序号 相同基序号为统一系列分片数据*/
        massDataInfo.massDataBaseSeq = Add_SequenceNumber(pstDataWriter->seqNum, SEQUENCENUMBER_START);
        /* Nephalem massdata分片总数量 */
        massDataInfo.massDataTotal = iNum + 1;
        /* Nephalem massdata数据总大小 */
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
	/* 申请静态内存创建HistoryData链表节点 */
	pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC_T(sizeof(HistoryData));
	if (NULL == pstHistoryData)
	{
		PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData, Insufficient memory space.\n");
		return DDS_RETCODE_ERROR;
	}
	InitHistoryData(pstHistoryData);

    /* Nephalem 无论是否分片，都需要将数据保存到HistoryData中，等数据重发时使用*/
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
		/* 申请静态内存创建历史缓存数据 */
		pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC_T(topicLen + msgLen + length);
		if (NULL == pstHistoryData->data)
		{
			/* 失败释放前面申请的内存 */
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
		/* 数据拷贝到缓存区 */
		memcpy(pstHistoryData->data + topicLen + msgLen, buff, length);
		length += topicLen + msgLen;
	}

    /* 设置缓存数据长度 */
    pstHistoryData->uiDataLen = length;

    /* 每写入一条数据写入器当前seqNum自加1 */
    pstDataWriter->seqNum = Add_SequenceNumber(pstDataWriter->seqNum, SEQUENCENUMBER_START);

    /* 缓存数据记录当前seqNum编号以便数据重传 */
    pstHistoryData->seqNum = pstDataWriter->seqNum;

    /* 数据添加时间戳 */
    if (pstDataWriter->writerCallBack)
    {
        /* 用户自定义时间回调 */
		/* Nephalem 该回调函数会被deadline触发，此处不进行回调 */
        //pstDataWriter->writerCallBack(&pstHistoryData->timeInfo);
    }
    else
    {
        /* 默认时间回调 */
        pstHistoryData->timeInfo = GetNowTime();
    }
   
    if (pstHistoryData->uiDataLen > DataFragSize)
    {
        pstHistoryData->uiFragTotal = pstHistoryData->uiDataLen / DataFragSize + 1;

        /* Nephalem uiDataLen > DataFragSize 时，分片数量必定大于0 */
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
                /* 失败释放前面申请的内存 */
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
            /* 数据拷贝到缓存区 */
            memcpy(pstHistoryData->pstFragData->data, buff + (i - 1)*DataFragSize, pstLength);
            /* 组装发布报文发送给订阅的所有阅读者 */
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
		retCode = DDS_RETCODE_OK; //只有返回值为ok，才会保存到cache
		/* Nephalem 28是DATA报文头的长度 */
		if (pstDataWriter->uiHistoryDataSize + pstHistoryData->uiDataLen + 28 > 63 * 1024)
		{
			DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
			retCode = SendHistoryData(pstDataWriter);
			DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);		
		}

		//不管当前数据包有没有发送；都需要在历史数据发完之后，将当前数据插入到历史缓存中
		pstDataWriter->uiHistoryDataSize += pstHistoryData->uiDataLen + 28;
		/* Nephalem 缓存新数据 */
		DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
		InsertHistoryCacheTail(&pstDataWriter->stHistoryCache, pstHistoryData);
		DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
		return retCode;//直接返回，因为下方判断是否保存的逻辑有点混乱
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

    /* Nephalem 可靠传输下判断是否需要缓存数据 */
    if (RELIABLE_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
    {
        /* Nephalem 发送成功，缓存数据*/
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
                //todo 将数据储存到文件系统中（硬盘上）
            }
        }
    }
    /* Nephalem 非可靠传输下判断是否需要缓存数据 */
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
            //todo 将数据储存到文件系统中（硬盘上）
        }
    }

	if(1 == iWriteLog)
	{
		printf("Nephalem : StoreFlag = %d ",StoreFlag);
		printf("Topic = %s \n",pstDataWriter->pstTopic->topicName.value);
	}
	
    if (StoreFlag)
    {
        /* Nephalem 判断是否需要释放最旧缓存 */
        FreeOldFlag = FALSE;

        /* Nephalem resource_limits  KEEP_LAST */
        if (KEEP_LAST_HISTORY_QOS == pstDataWriter->stDataWriterQos.history.kind)
        {
            /* 数缓存据插入到本地缓存链 */
            /* Nephalem resource_limits 超出history上限，清理最旧缓存 */
            if (pstDataWriter->stHistoryCache.uiHistoryDataNum >= pstDataWriter->stDataWriterQos.history.depth)
            {
                /* Nephalem resource_limits 直接删除旧数据 */
                    FreeOldFlag = TRUE;
            }
        }
        /* Nephalem resource_limits  KEEP_ALL */
        else if (KEEP_ALL_HISTORY_QOS == pstDataWriter->stDataWriterQos.history.kind)
        {
            /* 数缓存据插入到本地缓存链 */
            /* Nephalem resource_limits 超出resource_limits上限，清理最旧缓存 */
            if (LENGTH_UNLIMITED != pstDataWriter->stDataWriterQos.resource_limits.max_samples
                && pstDataWriter->stHistoryCache.uiHistoryDataNum >= pstDataWriter->stDataWriterQos.resource_limits.max_samples)
            {
                /* Nephalem resource_limits 在best_effort下直接删除旧数据 */
                if (BEST_EFFORT_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
                    FreeOldFlag = TRUE;
                else if (RELIABLE_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
                {   /* Nephalem resource_limits 在reliable下堵塞等待stHistoryCache腾出空间 */
                    while (pstDataWriter->stHistoryCache.uiHistoryDataNum >= pstDataWriter->stDataWriterQos.resource_limits.max_samples)
                    {
                        if (pstDataWriter->stDataWriterQos.durability.kind == TRANSIENT_DURABILITY_QOS)
                            return DDS_RETCODE_OUT_OF_RESOURCES;
                        QY_Sleep(100);
                    }
                }
            }
        }
        /* Nephalem 删除旧数据 */
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

        /* Nephalem 缓存新数据 */
        DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
        InsertHistoryCacheTail(&pstDataWriter->stHistoryCache, pstHistoryData);
        DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
    }
    else
    {
        /* Nephalem 无论是否分片都需要释放data内存 */
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
-   函 数 名: DDS_StringDataWriter_writeFragData
-   功能描述: 发送分片数据
-   输    入: 写入器、用户数据、数据长度，分片信息（分片数据基序号，分片总数，数据总大小）
-   返    回:
-   全局变量:
-   注    释: 发送分片数据
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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

    /* 申请静态内存创建HistoryData链表节点 */
    pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC_T(sizeof(HistoryData));
    if (NULL == pstHistoryData)
    {
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData, Insufficient memory space.\n");
        return DDS_RETCODE_ERROR;
    }
    InitHistoryData(pstHistoryData);

    /* Nephalem massData相关信息赋值*/
    pstHistoryData->massDataBaseSeq = pstMassDataInfo->massDataBaseSeq;
    pstHistoryData->massDataTotal = pstMassDataInfo->massDataTotal;
    pstHistoryData->massDataTotolSize = pstMassDataInfo->massDataTotolSize;

    pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC_T(length);

    if (NULL == pstHistoryData->data)
    {
        /* 失败释放前面申请的内存 */
        DDS_STATIC_FREE(pstHistoryData);
        pstHistoryData = NULL;
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData buff, Insufficient memory space.\n");
        return DDS_RETCODE_ERROR;
    }

    /* 数据拷贝到缓存区 */
    memcpy(pstHistoryData->data, buff, length);

    /* 设置缓存数据长度 */
    pstHistoryData->uiDataLen = length;

    /* 每写入一条数据写入器当前seqNum自加1 */
    pstDataWriter->seqNum = Add_SequenceNumber(pstDataWriter->seqNum, SEQUENCENUMBER_START);

    /* 缓存数据记录当前seqNum编号以便数据重传 */
    pstHistoryData->seqNum = pstDataWriter->seqNum;

    /* 数据添加时间戳 */
    if (pstDataWriter->writerCallBack)
    {
        /* 用户自定义时间回调 */
        pstDataWriter->writerCallBack(&pstHistoryData->timeInfo);
    }
    else
    {
        /* 默认时间回调 */
        pstHistoryData->timeInfo = GetNowTime();
    }
    

    /* 组装发布报文发送给订阅的所有阅读者 */
    DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
    retCode = JointUSERDataMsgSendUnknownReader(pstDataWriter, pstHistoryData);
    DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);

    StoreFlag = FALSE;

    /* Nephalem 可靠传输下判断是否需要缓存数据 */

    /* Nephalem 发送成功，缓存数据*/
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
            //todo 将数据储存到文件系统中（硬盘上）
        }
    }

    if (StoreFlag)
    {
        /* Nephalem 判断是否需要释放最旧缓存 */
        FreeOldFlag = FALSE;

        /* Nephalem resource_limits  KEEP_LAST */
        if (KEEP_LAST_HISTORY_QOS == pstDataWriter->stDataWriterQos.history.kind)
        {
            /* 数缓存据插入到本地缓存链 */
            /* Nephalem resource_limits 超出history上限，清理最旧缓存 */
            if (pstDataWriter->stHistoryCache.uiHistoryDataNum >= pstDataWriter->stDataWriterQos.history.depth)
            {
                /* Nephalem resource_limits 在best_effort下直接删除旧数据 */
                if (BEST_EFFORT_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
                    FreeOldFlag = TRUE;
            }
        }
        /* Nephalem resource_limits  KEEP_ALL */
        else if (KEEP_ALL_HISTORY_QOS == pstDataWriter->stDataWriterQos.history.kind)
        {
            /* 数缓存据插入到本地缓存链 */
            /* Nephalem resource_limits 超出resource_limits上限，清理最旧缓存 */
            if (LENGTH_UNLIMITED != pstDataWriter->stDataWriterQos.resource_limits.max_samples
                && pstDataWriter->stHistoryCache.uiHistoryDataNum >= pstDataWriter->stDataWriterQos.resource_limits.max_samples)
            {
                /* Nephalem resource_limits 在best_effort下直接删除旧数据 */
                if (BEST_EFFORT_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
                    FreeOldFlag = TRUE;
                else if (RELIABLE_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
                {   /* Nephalem resource_limits 在reliable下堵塞等待stHistoryCache腾出空间 */
                    while (pstDataWriter->stHistoryCache.uiHistoryDataNum >= pstDataWriter->stDataWriterQos.history.depth)
                    {
                        if (pstDataWriter->stDataWriterQos.durability.kind == TRANSIENT_DURABILITY_QOS)
                            return DDS_RETCODE_OUT_OF_RESOURCES;

                        QY_Sleep(100);
                    }
                }
            }
        }
        /* Nephalem 删除旧数据 */
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

        /* Nephalem 缓存新数据 */
        DDS_MUTEX_LOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
        InsertHistoryCacheTail(&pstDataWriter->stHistoryCache, pstHistoryData);
        DDS_MUTEX_UNLOCK(pstDataWriter->pstTopic->pstParticipant->m_discRdMutex);
    }
    else
    {
        /* Nephalem 释放data内存 */
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
-   函 数 名: DataWriterSendCacheMsg
-   功能描述: 缓存数据重传
-   输    入: 写入器、远端阅读器，缺失seqNum
-   返    回: DDS_RETCODE_OK、DDS_RETCODE_ERROR
-   全局变量:
-   注    释: 缓存数据重传
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\16       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_ReturnCode_t DataWriterSendCacheMsg(DDS_DataWriter* pstDataWriter, DDS_DiscoveredReader* pstDiscReader, SequenceNumber_t seqNum)
{
    SequenceNumber_t   seqNumOffset;
    UINT32             uiOffset;
	HistoryData* pstHistoryData = NULL;

    /* 本地无缓存数据或者ack报文无丢失，直接返回不处理 */
    if (NULL == pstDataWriter->stHistoryCache.pstHead)
    {
        //printf("1\n");
        return DDS_RETCODE_ERROR;
    }

    /* 缺失报文不在缓存区间，直接返回不处理 */
    if (ISLESSTHEN_SEQUENCENUMBER(pstDataWriter->stHistoryCache.pstTail->seqNum, seqNum) || ISLESSTHEN_SEQUENCENUMBER(seqNum, pstDataWriter->stHistoryCache.pstHead->seqNum))
    {
        //printf("2\n");
		//printf("cache missed, seqNum: %d, headNum: %d, tailNum: %d.\n", seqNum.low, pstDataWriter->stHistoryCache.pstHead->seqNum.low, pstDataWriter->stHistoryCache.pstTail->seqNum.low);
        return DDS_RETCODE_ERROR;
    }

    /* 缺失缓存报文偏移量 */
    seqNumOffset = Sub_SequenceNumber(seqNum, pstDataWriter->stHistoryCache.pstHead->seqNum);

    pstHistoryData = pstDataWriter->stHistoryCache.pstHead;

    /* 通过偏移量查找缺失报文，因为缓存是连续的，不用一个个比较，直接找到对应位置 */
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

    /* 偏移量查找到的缓存报文对不上，检查程序逻辑问题 */
    if (!ISEQUAL_SEQUENCENUMBER(pstHistoryData->seqNum, seqNum))
    {
        printf("DataWriterSendCacheMsg: There is a problem with the history cache linked list, inconsistency of data, expect: %d, actually: %d.\n",seqNum.low, pstHistoryData->seqNum.low);
        return DDS_RETCODE_ERROR;
    }

    /* Nephalem 通过NackFrag进行丢失分片的重传，不发送整个数据包 */
    if (TRUE == pstHistoryData->bUseNackFrag)
    {
        /* Nephalem 查找该reader是否添加到使用NackFrag重传列表中 */
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

    /* 开始发送报文 */
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
		/* Nephalem seqNum为0 没有数据需要重传 */
		if (0 == seqNum[uiSeqNum].high && 0 == seqNum[uiSeqNum].low)
			return DDS_RETCODE_OK;

		/* 网络字节序初始化，保存发布报文 */
		INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

		/* 第一步序列化RTPS报文头 */
		SERIALIZE_RTPS_MESSAGE_HEADER(pstDataWriter->pstTopic->pstParticipant->stRTPSMsgHeader, stMemBlock);

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
		stInfoTS.value = GetNowTime();

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
		stInfoDest.guidPrefix = pstDiscReader->guid.prefix;

		/* 第二步序列化子报文头 */
		SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

		/* 第三步序列化子报文消息体 */
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

			/* Nephalem 通过NackFrag进行丢失分片的重传，不发送整个数据包 */
			if (TRUE == pstHistoryData->bUseNackFrag)
			{
				/* Nephalem 查找该reader是否添加到使用NackFrag重传列表中 */
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

			/* data报文头设置 */
			stSubMsgHeader.submessageId = DATA;

			/* Nephlaem 计算Data报文长度 */
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

			//空间不够组装数据 
			if (stMemBlock.writeIndex + stSubMsgHeader.submessageLength + 4 >= NETWORK_BYTE_MAX_SIZE)
				break;

			if (0 == IS_MACHINE_LITTLE_ENDIAN)
				stSubMsgHeader.flags[0] = FALSE;    //littleEndian
			else
				stSubMsgHeader.flags[0] = TRUE;    //littleEndian
			stSubMsgHeader.flags[1] = FALSE;    //inlineQosFlag
			stSubMsgHeader.flags[2] = (pstHistoryData->data) ? TRUE : FALSE;  //dataPresent
			stSubMsgHeader.flags[3] = FALSE;   //keyFlag

			/* data报文体设置 */
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

			/* 第二步序列化子报文头 */
			SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

			/* 第三步序列化子报文消息体 */
			SERIALIZE_DATA(stData, stMemBlock);

			/* 序列化的有效载荷 */
			SERIALIZE_SERIALIZEDDATA(stData.serializedPayload, stMemBlock);

			/* 序列化写入器信息 */
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

    /* 网络字节序初始化，缓存发布报文 */
    INITIAL_MEMORYBLOCK(stMemBlock, cUserBuffer, 1024);

    /* 第一步序列化RTPS报文头 */
    SERIALIZE_RTPS_MESSAGE_HEADER(pstParticipant->stRTPSMsgHeader, stMemBlock);

    /* 时间报文头设置 */
    stSubMsgHeader.submessageId = INFO_TS;
    stSubMsgHeader.submessageLength = sizeof(Time_t);
    stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
    stSubMsgHeader.flags[2] = FALSE;   //dataFlag
    stSubMsgHeader.flags[3] = FALSE;   //keyFlag
    stInfoTS.value = GetNowTime();

    /* 序列化时间报文头 */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* 序列化时间子报文体 */
    SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

    /* InfoDT报文 */
    stSubMsgHeader.submessageId = INFO_DST;
    stSubMsgHeader.submessageLength = 12;
    stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;
    stSubMsgHeader.flags[3] = FALSE;
    stInfoDest.guidPrefix = pstDiscReader->guid.prefix;

    /* 第二步序列化子报文头 */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* 第三步序列化子报文消息体 */
    SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

    stSubMsgHeader.submessageId = GAP;
    stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
    stSubMsgHeader.flags[2] = FALSE;
    stSubMsgHeader.flags[3] = FALSE;

    /* 用户数据报文体设置 */
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
    /* 序列化Gap报文头 */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);
    /* 序列化Gap报文体 */
    SERIALIZE_GAP(stGapMsg, stMemBlock);

    RTPSSendMsg(&stMemBlock, &pstParticipant->socketList.user_data_sender, pstDiscReader->pstUnicastLocator);
    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   函 数 名: DelDataWriterHistoryCache
-   功能描述: 过期缓存清除
-   输    入: 主题
-   返    回: 
-   全局变量:
-   注    释: 过期缓存清除
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
            /* 设置最小期望seqNum，比本地写入器最大缓存大1，若远端阅读器期望seqNum都大于等于这个，则缓存全部清除 */
            seqNumMin = Add_SequenceNumber(pstDataWriter->stHistoryCache.pstTail->seqNum, SEQUENCENUMBER_START);

            /* 获取每个主题下远端阅读器最小的期望seqNum */
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
            /* 本地写入器缓存中删除最小期望seqNum之前所有数据 */
            psHistoryData = pstDataWriter->stHistoryCache.pstHead;
            while (NULL != psHistoryData)
            {
                if (ISLESSTHEN_SEQUENCENUMBER(psHistoryData->seqNum, seqNumMin))
                {
                    psHistoryDataDel = psHistoryData;
                }
                /* 大于等于最小seqNum值，保留 */
                else
                {
                    break;
                }
                psHistoryData = psHistoryData->pNext;

                /* 缓存释放 */
                DDS_STATIC_FREE(psHistoryDataDel->data);
                psHistoryDataDel->data = NULL;
                DDS_STATIC_FREE(psHistoryDataDel);
                psHistoryDataDel = NULL;

                /* 头节点偏移 */
                pstDataWriter->stHistoryCache.pstHead = psHistoryData;
                pstDataWriter->stHistoryCache.uiHistoryDataNum--;
            }
			//printf("headSeqNum: %d\n", pstDataWriter->stHistoryCache.pstHead->seqNum.low);
            /* 头节点为空，此为空链表，尾节点也需置空 */
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
	/* 申请静态内存创建历史缓存数据 */
	pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC_T(len);

	if (NULL == pstHistoryData->data)
	{
		/* 失败释放前面申请的内存 */
		DDS_STATIC_FREE(pstHistoryData);
		PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData buff, Insufficient memory space.\n");
		return DDS_RETCODE_ERROR;
	}

	/* 数据拷贝到缓存区 */
	memcpy(pstHistoryData->data, buf, len);

	/* 设置缓存数据长度 */
	pstHistoryData->uiDataLen = len;

	/* 数据添加时间戳 */
	if (pstDataWriter->writerCallBack)
	{
		/* 用户自定义时间回调 */
		pstDataWriter->writerCallBack(&pstHistoryData->timeInfo);
	}
	else
	{
		/* 默认时间回调 */
		pstHistoryData->timeInfo = GetNowTime();
	}
	
	/* 组装发布报文发送给订阅的所有阅读者 */
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

	/* 合包的格式是INFO_TS + INFO_DST + n个Data子报文，先构造前面的报文，再通过循环将需要发送的数据包封装在一起 */
	INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);/* 固定报文长度为64KB，网络字节序初始化，保存发布报文 */

	/* 第一步序列化RTPS报文头 */
	SERIALIZE_RTPS_MESSAGE_HEADER(pstDataWriter->pstTopic->pstParticipant->stRTPSMsgHeader, stMemBlock);

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
	stInfoTS.value = GetNowTime();

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
	stInfoDest.guidPrefix = pstDataWriter->guid.prefix;

	/* 第二步序列化子报文头 */
	SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

	/* 第三步序列化子报文消息体 */
	SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

	pstHistoryData = pstDataWriter->stHistoryCache.pstHead;

	/* 如果没有数据，则不需要再做下面的操作*/
	if (pstHistoryData == NULL || pstDataWriter->uiHistoryDataSize == 0)
	{
		return DDS_RETCODE_NO_DATA;
	}
	while (NULL != pstHistoryData)/* 通过循环将数据拼到同一个报文 */
	{
		if (FALSE == pstHistoryData->bSend)
		{
			pstHistoryData = pstHistoryData->pNext;
			continue;
		}

		pstHistoryData->bSend = FALSE;

		/* data报文头设置 */
		stSubMsgHeader.submessageId = DATA;

		/* Nephlaem 计算Data报文长度 */
		stSubMsgHeader.submessageLength = sizeof(stData.extraFlags) +
			sizeof(stData.octetsToInlineQos) + sizeof(stData.readerId) +
			sizeof(stData.writerId) + sizeof(stData.writerSN);

		stSubMsgHeader.submessageLength += sizeof(unsigned short) * 2 + pstHistoryData->uiDataLen;

		/* Nephalem 字节对齐*/	
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

		/* data报文体设置 */
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

		/* 第二步序列化子报文头 */
		SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

		/* 第三步序列化子报文消息体 */
		SERIALIZE_DATA(stData, stMemBlock);

		/* 序列化的有效载荷 */
		SERIALIZE_SERIALIZEDDATA(stData.serializedPayload, stMemBlock);

		/* 序列化写入器信息 */
		PUT_BYTE_ARRAY(stMemBlock, pstHistoryData->data, pstHistoryData->uiDataLen);

		stMemBlock.writeIndex += uiOffset;
		stMemBlock.wrPtr += uiOffset;

		pstHistoryData = pstHistoryData->pNext;
	}

	pstDiscReader = pstDataWriter->pstTopic->pstDiscReader;
	if (pstDiscReader == NULL) return DDS_RETCODE_NO_AVAILABLE_READER;

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

	pstDataWriter->uiHistoryDataSize = 0; //发完将历史数据长度记录清零
	return retCode;
}

