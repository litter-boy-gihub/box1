#include "../include/GlobalDefine.h"
#ifdef _FC
extern YCCommFCConfig g_commFCConfig;
#endif
VOID MonParticipantConvertNetworkByteOrder(const DDS_DomainParticipant * pstParticipant, MemoryBlock* pstMemBlock)
{
    UINT32 uiStarIndex = 0;
    UINT32 uiEndIndex = 0;
    UINT32 uiStrLen = 0;
	MemoryBlock   stMemBlock;
    ParameterHead stParamHead;
    ParameterHead stWtParamHead;
    ParameterHead stRdParamHead;
    ParameterHead stDwtParamHead;
    ParameterHead stDrdParamHead;
	GUID_t partGuid;

    DDS_Topic* pstTopic = NULL;
    Locator_t* pstlocator = NULL;
    DDS_DataWriter* pstLocalWriter  = NULL;
    DDS_DataReader* pstLocalReader  = NULL;
    DDS_DiscoveredWriter* pstDiscWriter = NULL;
    DDS_DiscoveredReader* pstDiscReader = NULL;
    DDS_DiscoveredReader* pstTwinDiscReader = NULL;

    stParamHead.paramType = PID_PROTOCOL_VERSION;
    stParamHead.length = sizeof(ProtocolVersion_t);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_PROTOCOL_VERSION(pstParticipant->stRTPSMsgHeader.protocolVersion, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

    partGuid.prefix = pstParticipant->stRTPSMsgHeader.guidPrefix;
    partGuid.entityId = ENTITYID_PARTICIPANT;
    stParamHead.paramType = PID_PARTICIPANT_GUID;
    stParamHead.length = sizeof(GUID_t);

    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_GUID(partGuid, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

    stParamHead.paramType = PID_VENDORID;
    stParamHead.length = sizeof(VendorId_t);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_VENDORID(pstParticipant->stRTPSMsgHeader.vendorId, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

	stParamHead.paramType = PID_PROPERTY_LIST;
	if (PARAM_HEAD_SIZE + sizeof(PropertyList) <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
	{
		SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
		uiStarIndex = pstMemBlock->writeIndex;
		SERIALIZE_PROPERTY_LIST_SPDP(pstParticipant->dcpsParticipantData.propertyList, (*pstMemBlock));
		stParamHead.length = pstMemBlock->writeIndex - uiStarIndex;
		INITIAL_MEMORYBLOCK(stMemBlock, stParamHead.pcLen, sizeof(UINT32));
		PUT_UNSIGNED_SHORT(stMemBlock, stParamHead.length);
	}

    pstlocator = pstParticipant->rtpsParticipantProxy.defaultUnicastLocator;
    while (NULL != pstlocator)
    {
        stParamHead.paramType = PID_DEFAULT_UNICAST_LOCATOR;
        stParamHead.length = sizeof(Locator_t) - sizeof(VOID*);
        if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
        {
            SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
            uiStarIndex = pstMemBlock->writeIndex;
            SERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock));
            uiEndIndex = pstMemBlock->writeIndex;
            MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
        }
        pstlocator = pstlocator->pNext;
    }

    pstTopic = pstParticipant->pstTopic;
    while (NULL != pstTopic)
    {
        if (Guid_Is_Equal(&pstTopic->tpGuid, &GUID_UNKNOWN))
        {
            pstTopic = pstTopic->pNext;
            continue;
        }

		/* 主题GUID */
		stParamHead.paramType = TOPIC_GUID;
		stParamHead.length = sizeof(GUID_t);

        if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
        {
            SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
            uiStarIndex = pstMemBlock->writeIndex;
            SERIALIZE_GUID(pstTopic->tpGuid, (*pstMemBlock));
            uiEndIndex = pstMemBlock->writeIndex;
            MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
        }

        /* 主题名称 */
        stParamHead.paramType = PID_TOPIC_NAME;
        stParamHead.length = sizeof(UINT32) + (USHORT)(pstTopic->topicName.length);
        uiStrLen = pstTopic->topicName.length & 0x03;
        stParamHead.length += (uiStrLen == 0) ? 0 : (4 - uiStrLen);
        if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
        {
            SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
            uiStarIndex = pstMemBlock->writeIndex;
            SERIALIZE_BOUNDED_STRING(pstTopic->topicName, (*pstMemBlock));
            uiEndIndex = pstMemBlock->writeIndex;
            MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
        }

        /* 主题类型 */
        stParamHead.paramType = PID_TYPE_NAME;
        stParamHead.length = sizeof(UINT32) + (USHORT)(pstTopic->topicType.length);
        uiStrLen = pstTopic->topicType.length & 0x03;
        stParamHead.length += (uiStrLen == 0) ? 0 : (4 - uiStrLen);
        if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
        {
            SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
            uiStarIndex = pstMemBlock->writeIndex;
            SERIALIZE_BOUNDED_STRING(pstTopic->topicType, (*pstMemBlock));
            uiEndIndex = pstMemBlock->writeIndex;
            MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
        }

        pstLocalWriter = pstTopic->pstLocalWriter;
        while (NULL != pstLocalWriter)
        {
            /* 主题下的写入器 */
            stWtParamHead.paramType = WRITER_GUID;
            stWtParamHead.length = sizeof(GUID_t);

            if (PARAM_HEAD_SIZE + (UINT32)stWtParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
            {
                SERIALIZE_PARAM_HEAD(stWtParamHead, (*pstMemBlock));
                uiStarIndex = pstMemBlock->writeIndex;
                SERIALIZE_GUID(pstLocalWriter->guid, (*pstMemBlock));
                uiEndIndex = pstMemBlock->writeIndex;
                MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stWtParamHead.length + uiStarIndex - uiEndIndex);
            }

            pstLocalWriter = pstLocalWriter->pNext;
        }

        pstDiscReader = pstTopic->pstDiscReader;
        while (NULL != pstDiscReader)
        {
            /* 写入器下的对端阅读器 */
            stDrdParamHead.paramType = DISC_READER_GUID;
            stDrdParamHead.length = sizeof(GUID_t);

            if (PARAM_HEAD_SIZE + (UINT32)stDrdParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
            {
                SERIALIZE_PARAM_HEAD(stDrdParamHead, (*pstMemBlock));
                uiStarIndex = pstMemBlock->writeIndex;
                SERIALIZE_GUID(pstDiscReader->guid, (*pstMemBlock));
                uiEndIndex = pstMemBlock->writeIndex;
                MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stDrdParamHead.length + uiStarIndex - uiEndIndex);
            }

            pstTwinDiscReader = pstDiscReader->pstTwinDiscReader;
            while (NULL != pstTwinDiscReader)
            {
                /* 写入器下的对端阅读器 */
                stDrdParamHead.paramType = DISC_READER_GUID;
                stDrdParamHead.length = sizeof(GUID_t);

                if (PARAM_HEAD_SIZE + (UINT32)stDrdParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
                {
                    SERIALIZE_PARAM_HEAD(stDrdParamHead, (*pstMemBlock));
                    uiStarIndex = pstMemBlock->writeIndex;
                    SERIALIZE_GUID(pstTwinDiscReader->guid, (*pstMemBlock));
                    uiEndIndex = pstMemBlock->writeIndex;
                    MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stDrdParamHead.length + uiStarIndex - uiEndIndex);
                }

                pstTwinDiscReader = pstTwinDiscReader->pNext;
            }

            pstDiscReader = pstDiscReader->pNext;
        }

        pstLocalReader = pstTopic->pstLocalReader;
        while (NULL != pstLocalReader)
        {
            /* 主题下的阅读器 */
            stRdParamHead.paramType = READER_GUID;
            stRdParamHead.length = sizeof(GUID_t);

            if (PARAM_HEAD_SIZE + (UINT32)stRdParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
            {
                SERIALIZE_PARAM_HEAD(stRdParamHead, (*pstMemBlock));
                uiStarIndex = pstMemBlock->writeIndex;
                SERIALIZE_GUID(pstLocalReader->guid, (*pstMemBlock));
                uiEndIndex = pstMemBlock->writeIndex;
                MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stRdParamHead.length + uiStarIndex - uiEndIndex);
            }

            pstLocalReader = pstLocalReader->pNext;
        }

        pstDiscWriter = pstTopic->pstDiscWriter;
        while (NULL != pstDiscWriter)
        {
            /* 阅读器下的对端写入器 */
            stDwtParamHead.paramType = DISC_WRITER_GUID;
            stDwtParamHead.length = sizeof(GUID_t);

            if (PARAM_HEAD_SIZE + (UINT32)stDwtParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
            {
                SERIALIZE_PARAM_HEAD(stDwtParamHead, (*pstMemBlock));
                uiStarIndex = pstMemBlock->writeIndex;
                SERIALIZE_GUID(pstDiscWriter->guid, (*pstMemBlock));
                uiEndIndex = pstMemBlock->writeIndex;
                MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stDwtParamHead.length + uiStarIndex - uiEndIndex);
            }

            pstDiscWriter = pstDiscWriter->pNext;
        }

        pstTopic = pstTopic->pNext;
    }

    // 插入结束标志
    if (PARAM_HEAD_SIZE < pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        stParamHead.paramType = PID_SENTINEL;
        stParamHead.length = 0;
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: ParticipantConvertNetworkByteOrder
-   功能描述: 网络缓字节序转换
-   输    入: Participant、内存块
-   返    回:
-   全局变量:
-   注    释: 将Participant转换为网络字节序
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\7\28       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID ParticipantConvertNetworkByteOrder(const DDS_DomainParticipant * pstParticipant, MemoryBlock* pstMemBlock)
{
	UINT32 uiStarIndex = 0;
	UINT32 uiEndIndex = 0;
	UINT32 uiStrLen = 0;
	Locator_t* pstlocator = NULL;
	ParameterHead stParamHead;
	MemoryBlock   stMemBlock;
	GUID_t partGuid;
    Duration_t livelinessDuration;

//	stParamHead.paramType = PID_PROCESSID;
//	stParamHead.length = sizeof(int);
//	if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
//	{
//		SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//		uiStarIndex = pstMemBlock->writeIndex;
//		PUT_INT((*pstMemBlock), pstParticipant->processId);
//		uiEndIndex = pstMemBlock->writeIndex;
//		MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//	}
//
//	stParamHead.paramType = PID_IPADDR;
//	stParamHead.length = sizeof(EX_UINT32);
//	if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
//	{
//		SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//		uiStarIndex = pstMemBlock->writeIndex;
//		PUT_UNSIGNED_INT((*pstMemBlock), pstParticipant->ipAddr);
//		uiEndIndex = pstMemBlock->writeIndex;
//		MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//	}

//     stParamHead.paramType = PID_NETMASK;
//     stParamHead.length = sizeof(EX_UINT32);
//     if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
//     {
//         SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//         uiStarIndex = pstMemBlock->writeIndex;
//         PUT_UNSIGNED_INT((*pstMemBlock), pstParticipant->netMask);
//         uiEndIndex = pstMemBlock->writeIndex;
//         MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//     }

    stParamHead.paramType = PID_PARTICIPANT_LEASE_DURATION;
    stParamHead.length = sizeof(Duration_t);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        livelinessDuration.sec = 2;
        livelinessDuration.nanosec = 0;
        SERIALIZE_DURATION(/*pstParticipant->*/livelinessDuration, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

    partGuid.prefix = pstParticipant->stRTPSMsgHeader.guidPrefix;
    partGuid.entityId = ENTITYID_PARTICIPANT;
    stParamHead.paramType = PID_PARTICIPANT_GUID;
    stParamHead.length = sizeof(GUID_t);

    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_GUID(partGuid, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }
    
    stParamHead.paramType = PID_PROTOCOL_VERSION;
    stParamHead.length = 4; //sizeof(ProtocolVersion_t); RTI的发现报文中此参数是4个字节
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_PROTOCOL_VERSION(pstParticipant->stRTPSMsgHeader.protocolVersion, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        //MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length - (uiStarIndex - uiEndIndex));
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), 2);	// 简单粗暴进行4字节对齐
    }

    stParamHead.paramType = PID_VENDORID;
    stParamHead.length = 4; //sizeof(VendorId_t); RTI的发现报文中此参数是4个字节
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_VENDORID(pstParticipant->stRTPSMsgHeader.vendorId, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        //MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), 2);	// 简单粗暴进行4字节对齐
    }

    stParamHead.paramType = PID_BUILTIN_ENDPOINT_SET;
    stParamHead.length = sizeof(BuiltinEndpointSet_t);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        /* JD 此处将EnponitSet当做unsigned int 进行序列化，在调用 CDRTOOl中PUT_UNSIGNED_LONG进行序列化的时候可能出现问题。
        PUT_UNSIGNED_LONG是考虑字节对齐的，如果在封装该字段前的字节数值不是4的倍数，会有一个字节对齐的长度。
        例如 这个时候封装的实际长度大于4个字节 原本为0x3f00 00，会变成 0x0000 3f00 0000。
        有因为 uiStarIndex值没有改变，会导致后两个字节被覆盖变为 0x0000 3f00。
        下面封装的的其他信息也会因为相同的原因出现错误。
         */
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_BUILTINENDPOINTSET(pstParticipant->rtpsParticipantProxy.availableBuiltinEndpoints, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

    pstlocator = pstParticipant->rtpsParticipantProxy.metatrafficUnicastLocator;
    while (NULL != pstlocator)
    {
        stParamHead.paramType = PID_METATRAFFIC_UNICAST_LOCATOR;
        stParamHead.length = sizeof(Locator_t) - sizeof(VOID*);
        if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
        {
            SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
            uiStarIndex = pstMemBlock->writeIndex;
            SERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock));
            uiEndIndex = pstMemBlock->writeIndex;
            MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
        }
        pstlocator = pstlocator->pNext;
    }

//    pstlocator = pstParticipant->rtpsParticipantProxy.metatrafficMulticastLocator;
//    while (NULL != pstlocator)
//    {
//        stParamHead.paramType = PID_METATRAFFIC_MULTICAST_LOCATOR;
//        stParamHead.length = sizeof(Locator_t) - sizeof(VOID*);
//        if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
//        {
//            SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//            uiStarIndex = pstMemBlock->writeIndex;
//            SERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock));
//            uiEndIndex = pstMemBlock->writeIndex;
//            MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//        }
//        pstlocator = pstlocator->pNext;
//    }

    pstlocator = pstParticipant->rtpsParticipantProxy.defaultUnicastLocator;
    while (NULL != pstlocator)
    {
        stParamHead.paramType = PID_DEFAULT_UNICAST_LOCATOR;
        stParamHead.length = sizeof(Locator_t) -sizeof(VOID*);
        if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
        {
            SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
            uiStarIndex = pstMemBlock->writeIndex;
            SERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock));
            uiEndIndex = pstMemBlock->writeIndex;
            MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
        }
        pstlocator = pstlocator->pNext;
    }

    pstlocator = pstParticipant->rtpsParticipantProxy.defaultMulticastLocator;
    while (NULL != pstlocator)
    {
        stParamHead.paramType = PID_DEFAULT_MULTICAST_LOCATOR;
        stParamHead.length = sizeof(Locator_t) - sizeof(VOID*);
        if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
        {
            SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
            uiStarIndex = pstMemBlock->writeIndex;
            SERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock));
            uiEndIndex = pstMemBlock->writeIndex;
            MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
        }
        pstlocator = pstlocator->pNext;
    }

//    stParamHead.paramType = PID_USER_DATA;
//    stParamHead.length = (USHORT)(sizeof(UINT32) + pstParticipant->dcpsParticipantData.userData.value.length);
//    uiStrLen = pstParticipant->dcpsParticipantData.userData.value.length & 0x03;
//    stParamHead.length += (uiStrLen == 0) ? 0 : (4 - uiStrLen);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_USER_DATA_QOS_POLICY(pstParticipant->dcpsParticipantData.userData, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

//	stParamHead.paramType = PID_PROPERTY_LIST;
//	if (PARAM_HEAD_SIZE + sizeof(PropertyList) <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
//	{
//		SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//		uiStarIndex = pstMemBlock->writeIndex;
//		SERIALIZE_PROPERTY_LIST_SPDP(pstParticipant->dcpsParticipantData.propertyList, (*pstMemBlock));
//		stParamHead.length = pstMemBlock->writeIndex - uiStarIndex;
//		INITIAL_MEMORYBLOCK(stMemBlock, stParamHead.pcLen, sizeof(UINT32));
//		PUT_UNSIGNED_SHORT(stMemBlock, stParamHead.length);
//	}

//    stParamHead.paramType = PID_DOMAINID;
//    stParamHead.length = sizeof(UINT32);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        PUT_UNSIGNED_INT((*pstMemBlock), pstParticipant->dcpsParticipantData.domainID);
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

    // 插入结束标志
    if (PARAM_HEAD_SIZE < pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        stParamHead.paramType = PID_SENTINEL;
        stParamHead.length = 0;
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: DataWriterConvertNetworkByteOrder
-   功能描述: 网络缓字节序转换
-   输    入: 主题、写入器、内存块
-   返    回:
-   全局变量:
-   注    释: 将主题、写入器转换为网络字节序
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\7\28       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DataWriterConvertNetworkByteOrder(const DDS_Topic* pstTopic, const DDS_DataWriter* pstDataWriter, MemoryBlock* pstMemBlock)
{
    UINT32 uiStrLen = 0;
    UINT32 uiStarIndex = 0;
    UINT32 uiEndIndex = 0;
    Locator_t* pstlocator = NULL;
    ParameterHead stParamHead;
	UINT32 uiStrSize;
	PartitionName* pstPartitionName = NULL;
	MemoryBlock stMemBlockMod;

    stParamHead.paramType = PID_ENDPOINT_GUID;
    stParamHead.length = sizeof(GUID_t);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_GUID(pstDataWriter->guid, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

#if 0
    pstlocator = pstDataWriter->pstTopic->pstParticipant->rtpsParticipantProxy.defaultUnicastLocator;
    while (NULL != pstlocator)
    {
        stParamHead.paramType = PID_UNICAST_LOCATOR;
        stParamHead.length = sizeof(Locator_t) - sizeof(VOID*);
        if (PARAM_HEAD_SIZE + stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
        {
            SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
            uiStarIndex = pstMemBlock->writeIndex;
            SERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock));
            uiEndIndex = pstMemBlock->writeIndex;
            MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
        }
        pstlocator = pstlocator->pNext;
    }

    pstlocator = pstDataWriter->pstTopic->pstParticipant->rtpsParticipantProxy.defaultMulticastLocator;
    while (NULL != pstlocator)
    {
        stParamHead.paramType = PID_MULTICAST_LOCATOR;
        stParamHead.length = sizeof(Locator_t) - sizeof(VOID*);
        if (PARAM_HEAD_SIZE + stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
        {
            SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
            uiStarIndex = pstMemBlock->writeIndex;
            SERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock));
            uiEndIndex = pstMemBlock->writeIndex;
            MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
        }
        pstlocator = pstlocator->pNext;
    }
#endif

    stParamHead.paramType = PID_TOPIC_NAME;
    stParamHead.length = sizeof(UINT32) + (USHORT)(pstTopic->topicName.length);
    uiStrLen = pstTopic->topicName.length & 0x03;
    stParamHead.length += (uiStrLen == 0) ? 0 : (4 - uiStrLen);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead,  (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_BOUNDED_STRING(pstTopic->topicName, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

    stParamHead.paramType = PID_TYPE_NAME;
    stParamHead.length = sizeof(UINT32) + (USHORT)(pstTopic->topicType.length);
    uiStrLen = pstTopic->topicType.length & 0x03;
    stParamHead.length += (uiStrLen == 0) ? 0 : (4 - uiStrLen);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_BOUNDED_STRING(pstTopic->topicType, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

	stParamHead.paramType = PID_DURABILITY;
	stParamHead.length = sizeof(DurabilityQosPolicy);
	if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
	{
		SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
		uiStarIndex = pstMemBlock->writeIndex;
		SERIALIZE_DURABILITY_QOS_POLICY(pstDataWriter->stDataWriterQos.durability, (*pstMemBlock));
		uiEndIndex = pstMemBlock->writeIndex;
		MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
	}

//    stParamHead.paramType = PID_CONTROLLER;
//    stParamHead.length = sizeof(ControllerQosPolicy);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_CONTROLLER_QOS_POLICY(pstDataWriter->stDataWriterQos.controller, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

//	stParamHead.paramType = PID_HISTORY;
//	stParamHead.length = sizeof(HistoryQosPolicy);
//	if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
//	{
//		SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//		uiStarIndex = pstMemBlock->writeIndex;
//		SERIALIZE_HISTORY_QOS_POLICY(pstDataWriter->stDataWriterQos.history, (*pstMemBlock));
//		uiEndIndex = pstMemBlock->writeIndex;
//		MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//	}

//    stParamHead.paramType = PID_DURABILITY_SERVICE;
//    stParamHead.length = sizeof(DurabilityServiceQosPolicy);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_DURABILITY_SERVICE_QOS_POLICY(pstDataWriter->stDataWriterQos.durability_service, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

    stParamHead.paramType = PID_DEADLINE;
    stParamHead.length = sizeof(DeadlineQosPolicy);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_DEADLINE_QOS_POLICY(pstDataWriter->stDataWriterQos.deadline, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

//    stParamHead.paramType = PID_LATENCY_BUDGET;
//    stParamHead.length = sizeof(LatencyBudgetQosPolicy);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_LATENCY_BUDGET_QOS_POLICY(pstDataWriter->stDataWriterQos.latency_budget, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

	if (pstDataWriter->stDataWriterQos.liveliness.kind != AUTOMATIC_LIVELINESS_QOS)
	{
		stParamHead.paramType = PID_LIVELINESS;
		stParamHead.length = sizeof(LivelinessQosPolicy);
		if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
		{
			SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
			uiStarIndex = pstMemBlock->writeIndex;
			SERIALIZE_LIVELINESS_QOS_POLICY(pstDataWriter->stDataWriterQos.liveliness, (*pstMemBlock));
			uiEndIndex = pstMemBlock->writeIndex;
			MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
		}
	}

    stParamHead.paramType = PID_RELIABILITY;
    stParamHead.length = sizeof(ReliabilityQosPolicy);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_RELIABILITY_QOS_POLICY(pstDataWriter->stDataWriterQos.reliability, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

//    stParamHead.paramType = PID_LIFESPAN;
//    stParamHead.length = sizeof(LifespanQosPolicy);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_LIFESPAN_QOS_POLICY(pstDataWriter->stDataWriterQos.lifespan,  (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

//    stParamHead.paramType = PID_USER_DATA;
//    stParamHead.length = (USHORT)(sizeof(UINT32) + pstDataWriter->stDataWriterQos.user_data.value.length);
//    uiStrLen = pstDataWriter->stDataWriterQos.user_data.value.length & 0x03;
//    stParamHead.length += (uiStrLen == 0) ? 0 : (4 - uiStrLen);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_USER_DATA_QOS_POLICY(pstDataWriter->stDataWriterQos.user_data, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

    stParamHead.paramType = PID_OWNERSHIP;
    stParamHead.length = sizeof(OwnershipQosPolicy);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_OWNERSHIP_QOS_POLICY(pstDataWriter->stDataWriterQos.ownership, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

    stParamHead.paramType = PID_OWNERSHIP_STRENGTH;
    stParamHead.length = sizeof(OwnershipStrengthQosPolicy);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_OWNERSHIP_STRENGTH_QOS_POLICY(pstDataWriter->stDataWriterQos.ownership_strength, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

//    stParamHead.paramType = PID_DESTINATION_ORDER;
//    stParamHead.length = sizeof(DestinationOrderQosPolicy);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_DESTINATION_ORDER_QOS_POLICY(pstDataWriter->stDataWriterQos.destination_order, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

//    stParamHead.paramType = PID_PRESENTATION;
//    stParamHead.length = sizeof(PresentationQosPolicy);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_PRESENTATION_QOS_POLICY(pstTopic->stTopicQos.presentation, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

    /*  Nephalem partition 序列化分区信息 */
    if (0 < pstTopic->stTopicQos.partition.size)
    {
        /* Nephalem partition 属性长度的内存地址 */
        INITIAL_MEMORYBLOCK(stMemBlockMod, pstMemBlock->wrPtr + sizeof(USHORT), sizeof(USHORT));

        /* Nephalem partition 序列化属性头 */
        stParamHead.paramType = PID_PARTITION;
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));

        /* Nephalem partition 序列化分区数量 */
        PUT_UNSIGNED_INT((*pstMemBlock), pstTopic->stTopicQos.partition.size);
        pstPartitionName = pstTopic->stTopicQos.partition.pValue;
        while (NULL != pstPartitionName)
        {
            uiStrLen = pstPartitionName->partitionValue.length & 0x03;
            /* Nephalem partition 添加补齐空间后的实际长度 */
            uiStrSize = pstPartitionName->partitionValue.length + ((uiStrLen == 0) ? 0 : (4 - uiStrLen));
            PUT_UNSIGNED_INT((*pstMemBlock), uiStrSize);
            uiStarIndex = pstMemBlock->writeIndex;
            PUT_BYTE_ARRAY((*pstMemBlock), pstPartitionName->partitionValue.value, pstPartitionName->partitionValue.length);
            uiEndIndex = pstMemBlock->writeIndex;
            MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), uiStrSize + uiStarIndex - uiEndIndex);
            pstPartitionName = pstPartitionName->pNext;
        }

        /* Nephalem partition 修正为实际partition长度 */
        stParamHead.length = (USHORT)(pstMemBlock->wrPtr - stMemBlockMod.wrPtr) - sizeof(USHORT);
        PUT_SHORT(stMemBlockMod, stParamHead.length);
    }
     //stParamHead.length = (USHORT)(sizeof(UINT32) + pstTopic->stTopicQos.partition.value.length);
     //uiStrLen = pstTopic->stTopicQos.partition.value.length & 0x03;
     //stParamHead.length += (uiStrLen == 0) ? 0 : (4 - uiStrLen);
     //if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
     //{
     //    SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
     //    uiStarIndex = pstMemBlock->writeIndex;
     //    SERIALIZE_PARTITION_QOS_POLICY(pstTopic->stTopicQos.partition, (*pstMemBlock));
     //    uiEndIndex = pstMemBlock->writeIndex;
     //    MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
     //}

//    stParamHead.paramType = PID_TOPIC_DATA;
//    stParamHead.length = (USHORT)(sizeof(UINT32) + pstTopic->stTopicQos.topic_data.value.length);
//    uiStrLen = pstTopic->stTopicQos.topic_data.value.length & 0x03;
//    stParamHead.length += (uiStrLen == 0) ? 0 : (4 - uiStrLen);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_TOPIC_DATA_QOS_POLICY(pstTopic->stTopicQos.topic_data, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

//    stParamHead.paramType = PID_GROUP_DATA;
//    stParamHead.length = (USHORT)(sizeof(UINT32) + pstTopic->stTopicQos.group_data.value.length);
//    uiStrLen = pstTopic->stTopicQos.group_data.value.length & 0x03;
//    stParamHead.length += (uiStrLen == 0) ? 0 : (4 - uiStrLen);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length <  pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_GROUP_DATA_QOS_POLICY(pstTopic->stTopicQos.group_data, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

    // 插入结束标志
    if (PARAM_HEAD_SIZE < pstMemBlock->totalSize -  pstMemBlock->writeIndex)
    {
        stParamHead.paramType = PID_SENTINEL;
        stParamHead.length = 0;
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: DataReaderConvertNetworkByteOrder
-   功能描述: 网络缓字节序转换
-   输    入: 主题、读取器、网络缓存字节、缓存大小
-   返    回:
-   全局变量:
-   注    释: 将主题、阅读器转换为网络字节序
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\7\26       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DataReaderConvertNetworkByteOrder(const DDS_Topic* pstTopic, const DDS_DataReader* pstDataReader, MemoryBlock* pstMemBlock)
{
    UINT32 uiStrLen = 0;
    UINT32 uiStarIndex = 0;
    UINT32 uiEndIndex = 0;
    Locator_t* pstlocator = NULL;
    ParameterHead stParamHead;
	UINT32 uiStrSize;
	MemoryBlock stMemBlockMod;
	PartitionName* pstPartitionName = NULL;

    stParamHead.paramType = PID_ENDPOINT_GUID;
    stParamHead.length = sizeof(GUID_t);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_GUID(pstDataReader->guid, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

#if 0
    pstlocator = pstDataReader->pstTopic->pstParticipant->rtpsParticipantProxy.defaultUnicastLocator;
    while (NULL != pstlocator)
    {
        stParamHead.paramType = PID_UNICAST_LOCATOR;
        stParamHead.length = sizeof(Locator_t) - sizeof(VOID*);
        if (PARAM_HEAD_SIZE + stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
        {
            SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
            uiStarIndex = pstMemBlock->writeIndex;
            SERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock));
            uiEndIndex = pstMemBlock->writeIndex;
            MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
        }
        pstlocator = pstlocator->pNext;
    }

    pstlocator = pstDataReader->pstTopic->pstParticipant->rtpsParticipantProxy.defaultMulticastLocator;
    while (NULL != pstlocator)
    {
        stParamHead.paramType = PID_MULTICAST_LOCATOR;
        stParamHead.length = sizeof(Locator_t) - sizeof(VOID*);
        if (PARAM_HEAD_SIZE + stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
        {
            SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
            uiStarIndex = pstMemBlock->writeIndex;
            SERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock));
            uiEndIndex = pstMemBlock->writeIndex;
            MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
        }
        pstlocator = pstlocator->pNext;
    }
#endif

    stParamHead.paramType = PID_TOPIC_NAME;
    stParamHead.length = sizeof(UINT32) + (USHORT)(pstTopic->topicName.length);
    uiStrLen = pstTopic->topicName.length & 0x03;
    stParamHead.length += (uiStrLen == 0) ? 0 : (4 - uiStrLen);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_BOUNDED_STRING(pstTopic->topicName, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

    stParamHead.paramType = PID_TYPE_NAME;
    stParamHead.length = sizeof(UINT32) + (USHORT)(pstTopic->topicType.length);
    uiStrLen = pstTopic->topicType.length & 0x03;
    stParamHead.length += (uiStrLen == 0) ? 0 : (4 - uiStrLen);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_BOUNDED_STRING(pstTopic->topicType, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

//    stParamHead.paramType = PID_DURABILITY;
//    stParamHead.length = sizeof(DurabilityQosPolicy);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_DURABILITY_QOS_POLICY(pstDataReader->stDataReaderQos.durability, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }
    
    stParamHead.paramType = PID_DEADLINE;
    stParamHead.length = sizeof(DeadlineQosPolicy);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_DEADLINE_QOS_POLICY(pstDataReader->stDataReaderQos.deadline, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

//    stParamHead.paramType = PID_LATENCY_BUDGET;
//    stParamHead.length = sizeof(LatencyBudgetQosPolicy);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_LATENCY_BUDGET_QOS_POLICY(pstDataReader->stDataReaderQos.latency_budget, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

	if (pstDataReader->stDataReaderQos.liveliness.kind != AUTOMATIC_LIVELINESS_QOS)
	{
		stParamHead.paramType = PID_LIVELINESS;
		stParamHead.length = sizeof(LivelinessQosPolicy);
		if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
		{
			SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
			uiStarIndex = pstMemBlock->writeIndex;
			SERIALIZE_LIVELINESS_QOS_POLICY(pstDataReader->stDataReaderQos.liveliness, (*pstMemBlock));
			uiEndIndex = pstMemBlock->writeIndex;
			MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
		}
	}

    stParamHead.paramType = PID_RELIABILITY;
    stParamHead.length = sizeof(ReliabilityQosPolicy);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_RELIABILITY_QOS_POLICY(pstDataReader->stDataReaderQos.reliability, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

    stParamHead.paramType = PID_OWNERSHIP;
    stParamHead.length = sizeof(OwnershipQosPolicy);
    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
    {
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
        uiStarIndex = pstMemBlock->writeIndex;
        SERIALIZE_OWNERSHIP_QOS_POLICY(pstDataReader->stDataReaderQos.ownership, (*pstMemBlock));
        uiEndIndex = pstMemBlock->writeIndex;
        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
    }

//    stParamHead.paramType = PID_TIME_BASED_FILTER;
//    stParamHead.length = sizeof(TimeBasedFilterQosPolicy);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_TIME_BASED_FILTER_QOS_POLICY(pstDataReader->stDataReaderQos.time_based_filter, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

//    stParamHead.paramType = PID_USER_DATA;
//    stParamHead.length = (USHORT)(sizeof(UINT32) + pstDataReader->stDataReaderQos.user_data.value.length);
//    uiStrLen = pstDataReader->stDataReaderQos.user_data.value.length & 0x03;
//    stParamHead.length += (uiStrLen == 0) ? 0 : (4 - uiStrLen);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_USER_DATA_QOS_POLICY(pstDataReader->stDataReaderQos.user_data, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

//    stParamHead.paramType = PID_PRESENTATION;
//    stParamHead.length = sizeof(PresentationQosPolicy);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_PRESENTATION_QOS_POLICY(pstTopic->stTopicQos.presentation, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

//    stParamHead.paramType = PID_DESTINATION_ORDER;
//    stParamHead.length = sizeof(DestinationOrderQosPolicy);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_DESTINATION_ORDER_QOS_POLICY(pstTopic->stTopicQos.destination_order, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

    ///*  Nephalem partition 序列化分区信息 */
    if (0 < pstTopic->stTopicQos.partition.size)
    {
        /* Nephalem partition 属性长度的内存地址 */

        INITIAL_MEMORYBLOCK(stMemBlockMod, pstMemBlock->wrPtr + sizeof(USHORT), sizeof(USHORT));

        /* Nephalem partition 序列化属性头 */
        stParamHead.paramType = PID_PARTITION;
        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));

        /* Nephalem partition 序列化分区数量 */
        PUT_UNSIGNED_INT((*pstMemBlock), pstTopic->stTopicQos.partition.size);
        pstPartitionName = pstTopic->stTopicQos.partition.pValue;
        while (NULL != pstPartitionName)
        {
            uiStrLen = pstPartitionName->partitionValue.length & 0x03;
            /* Nephalem partition 添加补齐空间后的实际长度 */
            uiStrSize = pstPartitionName->partitionValue.length + ((uiStrLen == 0) ? 0 : (4 - uiStrLen));
            PUT_UNSIGNED_INT((*pstMemBlock), uiStrSize);
            uiStarIndex = pstMemBlock->writeIndex;
            PUT_BYTE_ARRAY((*pstMemBlock), pstPartitionName->partitionValue.value, pstPartitionName->partitionValue.length);
            uiEndIndex = pstMemBlock->writeIndex;
            MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), uiStrSize + uiStarIndex - uiEndIndex);
            pstPartitionName = pstPartitionName->pNext;
        }

        /* Nephalem partition 修正为实际partition长度 */
        stParamHead.length = (USHORT)(pstMemBlock->wrPtr - stMemBlockMod.wrPtr) - sizeof(USHORT);
        PUT_SHORT(stMemBlockMod, stParamHead.length);
    }

//    stParamHead.paramType = PID_TOPIC_DATA;
//    stParamHead.length = (USHORT)(sizeof(UINT32) + pstTopic->stTopicQos.topic_data.value.length);
//    uiStrLen = pstTopic->stTopicQos.topic_data.value.length & 0x03;
//    stParamHead.length += (uiStrLen == 0) ? 0 : (4 - uiStrLen);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_TOPIC_DATA_QOS_POLICY(pstTopic->stTopicQos.topic_data, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

//    stParamHead.paramType = PID_GROUP_DATA;
//    stParamHead.length = (USHORT)(sizeof(UINT32) + pstTopic->stTopicQos.group_data.value.length);
//    uiStrLen = pstTopic->stTopicQos.group_data.value.length & 0x03;
//    stParamHead.length += (uiStrLen == 0) ? 0 : (4 - uiStrLen);
//    if (PARAM_HEAD_SIZE + (UINT32)stParamHead.length < pstMemBlock->totalSize - pstMemBlock->writeIndex)
//    {
//        SERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock));
//        uiStarIndex = pstMemBlock->writeIndex;
//        SERIALIZE_GROUP_DATA_QOS_POLICY(pstTopic->stTopicQos.group_data, (*pstMemBlock));
//        uiEndIndex = pstMemBlock->writeIndex;
//        MEMORYBLOCK_SKIP_WRITE((*pstMemBlock), stParamHead.length + uiStarIndex - uiEndIndex);
//    }

    // 插入结束标志
    if (PARAM_HEAD_SIZE  < pstMemBlock->totalSize -  pstMemBlock->writeIndex)
    {
        stParamHead.paramType = PID_SENTINEL;
        stParamHead.length = 0;
        SERIALIZE_PARAM_HEAD(stParamHead,  (*pstMemBlock));
    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: NetworkByteOrderConvertDiscParticipant
-   功能描述: 网络缓字节序转换
-   输    入: 内存块、pstDiscParticipant
-   返    回:
-   全局变量:
-   注    释: 网络字节序转换为pstDiscParticipant
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\7\28       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL NetworkByteOrderConvertDiscParticipant(MemoryBlock*  pstMemBlock, DDS_DiscoveredParticipant * pstDiscParticipant, BOOL bNeedExchangeBytes)
{
    GUID_t parGuid = GUID_UNKNOWN;
    CHAR expect = -1;
    USHORT usStartIndex= 0;
    USHORT usEndIndex = 0;
    Locator_t* pstlocator     = NULL;
    Locator_t* pstTempLocator = NULL;
    ParameterHead stParamHead;
    stParamHead.length = 0;
    stParamHead.paramType = PID_PAD;

    while (pstMemBlock->readIndex <  pstMemBlock->totalSize)
    {
        DESERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock), bNeedExchangeBytes);
   
        if (stParamHead.length == 0 || stParamHead.paramType == PID_PAD)
        {
            break;
        }
        usStartIndex = pstMemBlock->readIndex;

        switch (stParamHead.paramType)
        {
			case PID_IPADDR:
				GET_UNSIGNED_INT((*pstMemBlock), pstDiscParticipant->IPAddr, bNeedExchangeBytes);
				break;

//             case PID_NETMASK:
//                 GET_UNSIGNED_INT((*pstMemBlock), pstDiscParticipant->netMask);
//                 break;

			case PID_PROCESSID:
				GET_INT((*pstMemBlock), pstDiscParticipant->processID, bNeedExchangeBytes);
				break;

            case PID_USER_DATA:

                DESERIALIZE_USER_DATA_QOS_POLICY((pstDiscParticipant->dcpsParticipantData.userData), (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_PARTICIPANT_LEASE_DURATION:

                DESERIALIZE_DURATION(pstDiscParticipant->leaseDuration, (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_PROTOCOL_VERSION:

                DESERIALIZE_PROTOCOL_VERSION(pstDiscParticipant->stRTPSMsgHeader.protocolVersion, (*pstMemBlock));
                break;

            case PID_PARTICIPANT_GUID:

                DESERIALIZE_GUID(parGuid, (*pstMemBlock));
                pstDiscParticipant->stRTPSMsgHeader.guidPrefix = parGuid.prefix;
                break;

            case PID_KEY_HASH:
    
                DESERIALIZE_GUID(parGuid, (*pstMemBlock));
                pstDiscParticipant->stRTPSMsgHeader.guidPrefix = parGuid.prefix;
                break;

            case PID_VENDORID:

                DESERIALIZE_VENDORID(pstDiscParticipant->stRTPSMsgHeader.vendorId, (*pstMemBlock));
                break;
    
            case PID_EXPECTS_INLINE_QOS:
        
                GET_BYTE((*pstMemBlock), expect);
                pstDiscParticipant->rtpsParticipantProxy.expectsInlineQos = (expect == 0) ? 0 : 1;
                break;

            case PID_BUILTIN_ENDPOINT_SET:

                DESERIALIZE_BUILTINENDPOINTSET(pstDiscParticipant->rtpsParticipantProxy.availableBuiltinEndpoints, (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_DEFAULT_UNICAST_LOCATOR:

                pstlocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
                if (NULL != pstlocator)
                {
                    DESERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock), bNeedExchangeBytes);
                    if (pstlocator->kind == 2)
                    {
                        DDS_STATIC_FREE(pstlocator);
                        continue;
                    }
                    LIST_INSERT_TAIL(pstDiscParticipant->rtpsParticipantProxy.defaultUnicastLocator, pstlocator, pstTempLocator);
#ifdef _FC                    
					{
						int i = 0;
						for(; i < g_commFCConfig.m_nodeNum;i++)
						{
							if(g_commFCConfig.m_configs[i].m_FCPort == pstDiscParticipant->rtpsParticipantProxy.defaultUnicastLocator->port)
							{
								pstDiscParticipant->rtpsParticipantProxy.defaultUnicastLocator->port = g_commFCConfig.m_configs[i].m_localUserSendMsgID;
								break;
							}
						}
					}
#endif
                }
                else
                {
                    PrintLog(COMMLOG_ERROR, "NetworkByteOrderConvertParticipant: Insufficient memory space.\n");
                    return FALSE;
                }
                break;

            case PID_DEFAULT_MULTICAST_LOCATOR:

                pstlocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
                if (NULL != pstlocator)
                {
                    DESERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock), bNeedExchangeBytes);
					if(pstlocator->kind == 2)
					{
						DDS_STATIC_FREE(pstlocator);
						continue;
					}
                    LIST_INSERT_TAIL(pstDiscParticipant->rtpsParticipantProxy.defaultMulticastLocator, pstlocator, pstTempLocator);
                }
                else
                {
                    PrintLog(COMMLOG_ERROR, "NetworkByteOrderConvertParticipant: Insufficient memory space.\n");
                    return FALSE;
                }
                break;

            case PID_METATRAFFIC_UNICAST_LOCATOR:

                pstlocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
                if (NULL != pstlocator)
                {
                    DESERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock), bNeedExchangeBytes);
					if(pstlocator->kind == 2)
					{
						DDS_STATIC_FREE(pstlocator);
						continue;
					}
                    LIST_INSERT_TAIL(pstDiscParticipant->rtpsParticipantProxy.metatrafficUnicastLocator, pstlocator, pstTempLocator);
#ifdef _FC          
					{
						int i = 0;
						for(; i < g_commFCConfig.m_nodeNum;i++)
						{
							if(g_commFCConfig.m_configs[i].m_FCPort == pstDiscParticipant->rtpsParticipantProxy.metatrafficUnicastLocator->port)
							{
								pstDiscParticipant->rtpsParticipantProxy.metatrafficUnicastLocator->port = g_commFCConfig.m_configs[i].m_localSEPDSendMsgID;
								break;
							}
						}
					}
#endif
                }
                else
                {
                    PrintLog(COMMLOG_ERROR, "NetworkByteOrderConvertParticipant: Insufficient memory space.\n");
                    return FALSE;
                }
                break;

            case PID_METATRAFFIC_MULTICAST_LOCATOR:

				pstlocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
				if (NULL != pstlocator)
				{
					DESERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock), bNeedExchangeBytes);
					if(pstlocator->kind == 2)
					{
						DDS_STATIC_FREE(pstlocator);
						continue;
					}
					LIST_INSERT_TAIL(pstDiscParticipant->rtpsParticipantProxy.metatrafficMulticastLocator, pstlocator, pstTempLocator);
				}
				else
				{
					PrintLog(COMMLOG_ERROR, "NetworkByteOrderConvertParticipant: Insufficient memory space.\n");
					return FALSE;
				}
				break;

			case PID_PROPERTY_LIST:

				DESERIALIZE_PROPERTY_LIST_SPDP(pstDiscParticipant->dcpsParticipantData.propertyList, (*pstMemBlock), bNeedExchangeBytes);
				break;

			case PID_SENTINEL:

                return TRUE;

            default:

                MEMORYBLOCK_SKIP_READER((*pstMemBlock), stParamHead.length);
                break;
        }

        usEndIndex = pstMemBlock->readIndex;
        /* 偏移量矫正 */
        if (0 < stParamHead.length + usStartIndex - usEndIndex)
        {
            pstMemBlock->rdPtr += stParamHead.length + usStartIndex - usEndIndex;
            pstMemBlock->readIndex += stParamHead.length + usStartIndex - usEndIndex;
        }
    }
    return TRUE;
}

/*---------------------------------------------------------------------------------
-   函 数 名: NetworkByteOrderConvertDataWriter
-   功能描述: 网络缓字节序转换
-   输    入: 主题、读取器、网络缓存字节、缓存大小
-   返    回:
-   全局变量:
-   注    释: 解析网络字节序转换为写入器
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\7\26       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID NetworkByteOrderConvertDiscDataWriter(MemoryBlock*  pstMemBlock, DDS_Topic* pstTopic, DDS_DiscoveredWriter* pstDataWriter, BOOL bNeedExchangeBytes)
{
    USHORT usStartIndex = 0;
    USHORT usEndIndex = 0;
    Locator_t* pstlocator     = NULL;
    Locator_t* pstTempLocator = NULL;
    ParameterHead stParamHead;
    stParamHead.length = 0;
    stParamHead.paramType = PID_PAD;

    while ( pstMemBlock->readIndex <  pstMemBlock->totalSize)
    {
        DESERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock), bNeedExchangeBytes);
        if (stParamHead.length == 0 || stParamHead.paramType == PID_PAD)
        {
            break;
        }
        usStartIndex = pstMemBlock->readIndex;

        switch (stParamHead.paramType)
        {
            case PID_ENDPOINT_GUID:

                DESERIALIZE_GUID(pstDataWriter->guid,  (*pstMemBlock));
                break;

            case PID_KEY_HASH: 

                DESERIALIZE_GUID(pstDataWriter->guid,  (*pstMemBlock));
                break;
#if 0
            case PID_UNICAST_LOCATOR:

                pstlocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
                if (NULL != pstlocator)
                {
                    DESERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock));
                    LIST_INSERT_TAIL(pstDataWriter->unicastLocator, pstlocator, pstTempLocator);
                }
                else
                {
                    PrintLog(COMMLOG_ERROR, "NetworkByteOrderConvertDataWriter: Insufficient memory space.\n");
                    return FALSE;
                }
                break;

            case PID_MULTICAST_LOCATOR:

                pstlocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
                if (NULL != pstlocator)
                {
                    DESERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock));
                    LIST_INSERT_TAIL(pstDataWriter->multicastLocator, pstlocator, pstTempLocator);
                }
                else
                {
                    PrintLog(COMMLOG_ERROR, "NetworkByteOrderConvertDataWriter: Insufficient memory space.\n");
                    return FALSE;
                }
                break;
#endif
            case PID_TOPIC_NAME:

                DESERIALIZE_BOUNDED_STRING(pstTopic->topicName,  (*pstMemBlock), bNeedExchangeBytes);
#if defined _DEBUG_LOG
                printf("Nephalem : Recv Remote DataWriter topicName = %s from %d.%d.%d.%d \n",pstTopic->topicName.value
                		,pstDataWriter->guid.prefix.value[0],pstDataWriter->guid.prefix.value[1],pstDataWriter->guid.prefix.value[2],pstDataWriter->guid.prefix.value[3]);
#endif
                break;

            case PID_TYPE_NAME:

                DESERIALIZE_BOUNDED_STRING(pstTopic->topicType,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_DURABILITY:

                DESERIALIZE_DURABILITY_QOS_POLICY(pstDataWriter->stDataWriterQos.durability,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_DURABILITY_SERVICE:

                DESERIALIZE_DURABILITY_SERVICE_QOS_POLICY(pstDataWriter->stDataWriterQos.durability_service,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_DEADLINE:

                DESERIALIZE_DEADLINE_QOS_POLICY(pstDataWriter->stDataWriterQos.deadline,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_LATENCY_BUDGET:

                DESERIALIZE_LATENCY_BUDGET_QOS_POLICY(pstDataWriter->stDataWriterQos.latency_budget,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_LIVELINESS:

                DESERIALIZE_LIVELINESS_QOS_POLICY(pstDataWriter->stDataWriterQos.liveliness,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_RELIABILITY:

                DESERIALIZE_RELIABILITY_QOS_POLICY(pstDataWriter->stDataWriterQos.reliability,  (*pstMemBlock), bNeedExchangeBytes);
                break;

			case PID_HISTORY:

				DESERIALIZE_HISTORY_QOS_POLICY(pstDataWriter->stDataWriterQos.history, (*pstMemBlock), bNeedExchangeBytes);
				break;

            case PID_CONTROLLER:
                DESERIALIZE_CONTROLLER_QOS_POLICY(pstDataWriter->stDataWriterQos.controller, (*pstMemBlock), bNeedExchangeBytes);
                break;

			case PID_LIFESPAN:

                DESERIALIZE_LIFESPAN_QOS_POLICY(pstDataWriter->stDataWriterQos.lifespan,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_USER_DATA:

                DESERIALIZE_USER_DATA_QOS_POLICY(pstDataWriter->stDataWriterQos.user_data,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_OWNERSHIP:

                DESERIALIZE_OWNERSHIP_QOS_POLICY(pstDataWriter->stDataWriterQos.ownership,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_OWNERSHIP_STRENGTH:

                DESERIALIZE_OWNERSHIP_STRENGTH_QOS_POLICY(pstDataWriter->stDataWriterQos.ownership_strength,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_DESTINATION_ORDER:

                DESERIALIZE_DESTINATION_ORDER_QOS_POLICY(pstDataWriter->stDataWriterQos.destination_order,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_PRESENTATION:

                DESERIALIZE_PRESENTATION_QOS_POLICY(pstTopic->stTopicQos.presentation,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_PARTITION:

                DESERIALIZE_PARTITION_QOS_POLICY(pstTopic->stTopicQos.partition,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_TOPIC_DATA:

                DESERIALIZE_TOPIC_DATA_QOS_POLICY(pstTopic->stTopicQos.topic_data,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_GROUP_DATA:

                DESERIALIZE_GROUP_DATA_QOS_POLICY(pstTopic->stTopicQos.group_data,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_SENTINEL:
                return;

            default:
                MEMORYBLOCK_SKIP_READER((*pstMemBlock), stParamHead.length);
                break;
        }

        usEndIndex = pstMemBlock->readIndex;
        /* 偏移量矫正 */
        if (0 < stParamHead.length + usStartIndex - usEndIndex)
        {
            pstMemBlock->rdPtr += stParamHead.length + usStartIndex - usEndIndex;
            pstMemBlock->readIndex += stParamHead.length + usStartIndex - usEndIndex;
        }
    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: NetworkByteOrderConvertDataReader
-   功能描述: 网络缓字节序转换
-   输    入: 主题、读取器、网络缓存字节、缓存大小
-   返    回:
-   全局变量:
-   注    释: 解析网络字节序转换为阅读器
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\7\26       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID NetworkByteOrderConvertDiscDataReader(MemoryBlock*  pstMemBlock, DDS_Topic* pstTopic, DDS_DiscoveredReader* pstDataReader,BOOL bNeedExchangeBytes)
{
    USHORT usStartIndex = 0;
    USHORT usEndIndex = 0;
    Locator_t* pstlocator     = NULL;
    Locator_t* pstTempLocator = NULL;
    ParameterHead stParamHead;
    stParamHead.length = 0;
    stParamHead.paramType = PID_PAD;

    while ( pstMemBlock->readIndex <  pstMemBlock->totalSize)
    {
        DESERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock), bNeedExchangeBytes);
        if (stParamHead.length == 0 || stParamHead.paramType == PID_PAD)
        {
            break;
        }
        usStartIndex = pstMemBlock->readIndex;

        switch (stParamHead.paramType)
        {
            case PID_ENDPOINT_GUID:

                DESERIALIZE_GUID(pstDataReader->guid,  (*pstMemBlock));
                break;

            case PID_KEY_HASH:

                DESERIALIZE_GUID(pstDataReader->guid,  (*pstMemBlock));
                break;
#if 0
            case PID_UNICAST_LOCATOR:

                pstlocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
                if (NULL != pstlocator)
                {
                    DESERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock));
                    LIST_INSERT_TAIL(pstDataReader->unicastLocator, pstlocator, pstTempLocator);
                }
                else
                {
                    PrintLog(COMMLOG_ERROR, "NetworkByteOrderConvertDataReader: Insufficient memory space.\n");
                    return FALSE;
                }
                break;

            case PID_MULTICAST_LOCATOR:

                pstlocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
                if (NULL != pstlocator)
                {
                    DESERIALIZE_LOCATOR((*pstlocator), (*pstMemBlock));
                    LIST_INSERT_TAIL(pstDataReader->multicastLocator, pstlocator, pstTempLocator);
                }
                else
                {
                    PrintLog(COMMLOG_ERROR, "NetworkByteOrderConvertDataReader: Insufficient memory space.\n");
                    return FALSE;
                }
                break;
#endif
            case PID_TOPIC_NAME:

                DESERIALIZE_BOUNDED_STRING(pstTopic->topicName,  (*pstMemBlock), bNeedExchangeBytes);
#if defined _DEBUG_LOG
                printf("Nephalem : Recv Remote DataReader topicName = %s from %d.%d.%d.%d \n",pstTopic->topicName.value
                		,pstDataReader->guid.prefix.value[0],pstDataReader->guid.prefix.value[1],pstDataReader->guid.prefix.value[2],pstDataReader->guid.prefix.value[3]);
#endif
                break;

            case PID_TYPE_NAME:

                DESERIALIZE_BOUNDED_STRING(pstTopic->topicType,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_DURABILITY:

                DESERIALIZE_DURABILITY_QOS_POLICY(pstDataReader->stDataReaderQos.durability,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_DEADLINE:

                DESERIALIZE_DEADLINE_QOS_POLICY(pstDataReader->stDataReaderQos.deadline,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_LATENCY_BUDGET:

                DESERIALIZE_LATENCY_BUDGET_QOS_POLICY(pstDataReader->stDataReaderQos.latency_budget,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_LIVELINESS:

                DESERIALIZE_LIVELINESS_QOS_POLICY(pstDataReader->stDataReaderQos.liveliness,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_RELIABILITY:

                DESERIALIZE_RELIABILITY_QOS_POLICY(pstDataReader->stDataReaderQos.reliability,  (*pstMemBlock), bNeedExchangeBytes);
                break;

			case PID_HISTORY:

				DESERIALIZE_HISTORY_QOS_POLICY(pstDataReader->stDataReaderQos.history, (*pstMemBlock), bNeedExchangeBytes);
				break;

			case PID_OWNERSHIP:

                DESERIALIZE_OWNERSHIP_QOS_POLICY(pstDataReader->stDataReaderQos.ownership,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_DESTINATION_ORDER:

                DESERIALIZE_DESTINATION_ORDER_QOS_POLICY(pstDataReader->stDataReaderQos.destination_order,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_USER_DATA:

                DESERIALIZE_USER_DATA_QOS_POLICY(pstDataReader->stDataReaderQos.user_data,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_TIME_BASED_FILTER:

                DESERIALIZE_TIME_BASED_FILTER_QOS_POLICY(pstDataReader->stDataReaderQos.time_based_filter,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_PRESENTATION:

                DESERIALIZE_PRESENTATION_QOS_POLICY(pstTopic->stTopicQos.presentation,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_PARTITION:

                DESERIALIZE_PARTITION_QOS_POLICY(pstTopic->stTopicQos.partition,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_TOPIC_DATA:

                DESERIALIZE_TOPIC_DATA_QOS_POLICY(pstTopic->stTopicQos.topic_data,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_GROUP_DATA:

                DESERIALIZE_GROUP_DATA_QOS_POLICY(pstTopic->stTopicQos.group_data,  (*pstMemBlock), bNeedExchangeBytes);
                break;

            case PID_SENTINEL:
                return;

            default:

                MEMORYBLOCK_SKIP_READER((*pstMemBlock), stParamHead.length);
                break;
        }

        usEndIndex = pstMemBlock->readIndex;
        /* 偏移量矫正 */
        if (0 < stParamHead.length + usStartIndex - usEndIndex)
        {
            pstMemBlock->rdPtr += stParamHead.length + usStartIndex - usEndIndex;
            pstMemBlock->readIndex += stParamHead.length + usStartIndex - usEndIndex;
        }
    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: NetworkByteOrderConvertInlineQos
-   功能描述: 解析网络字节序，在线qos过滤
-   输    入: 网络缓存字节
-   返    回:
-   全局变量:
-   注    释: 解析网络字节序，在线qos过滤
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\29       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID NetworkByteOrderConvertInlineQos(MemoryBlock* pstMemBlock, InlineQos* pstInlineQos, BOOL bNeedExchangeBytes)
{
    USHORT usStartIndex = 0;
    USHORT usEndIndex = 0;
    ParameterHead stParamHead;
    stParamHead.length = 0;
    stParamHead.paramType = PID_PAD;

    while (pstMemBlock->readIndex <  pstMemBlock->totalSize)
    {
        DESERIALIZE_PARAM_HEAD(stParamHead, (*pstMemBlock), bNeedExchangeBytes);
        if (stParamHead.length == 0 || stParamHead.paramType == PID_PAD)
        {
            break;
        }
        usStartIndex = pstMemBlock->readIndex;

        switch (stParamHead.paramType)
        { 
            case PID_KEY_HASH:

                DESERIALIZE_GUID(pstInlineQos->guid, (*pstMemBlock));
                break;

            case PID_STATUS_INFO:

                GET_UNSIGNED_INT((*pstMemBlock), pstInlineQos->disposed, bNeedExchangeBytes);
                break;

            case PID_SENTINEL:
                return;

            default:

                MEMORYBLOCK_SKIP_READER((*pstMemBlock), stParamHead.length);
                break;
        }

        usEndIndex = pstMemBlock->readIndex;
        /* 偏移量矫正 */
        if (0 < stParamHead.length + usStartIndex - usEndIndex)
        {
            pstMemBlock->rdPtr += stParamHead.length + usStartIndex - usEndIndex;
            pstMemBlock->readIndex += stParamHead.length + usStartIndex - usEndIndex;
        }
    }
}
