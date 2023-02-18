#include "../include/GlobalDefine.h"

DDS_DLL STATICINOF g_stStaticInfo;

void TestInitStaticConfig()
{
	int i = 0;
	g_stStaticInfo.iAppSize = 2;
	//0
	g_stStaticInfo.stStaticApps[i].iStaticAddr = 0xC0A8825D;//小端模式，ntohl(inet_addr("192.168.130.93"));
	g_stStaticInfo.stStaticApps[i].iProcessNum = 1;
	g_stStaticInfo.stStaticApps[i].usPort[0] = 1234;
	g_stStaticInfo.stStaticApps[i].usPort[1] = 1235;
	g_stStaticInfo.stStaticApps[i].iWriteTopicNum = 0;
	g_stStaticInfo.stStaticApps[i].iReadTopicNum = 1;
	g_stStaticInfo.stStaticApps[i].ReadTopic[0].sTopicName = "testTopic1";
	g_stStaticInfo.stStaticApps[i].ReadTopic[0].sTopicType = "DDS::STRING";
	//1
	i = 1;
	g_stStaticInfo.stStaticApps[i].iStaticAddr = 0xC0A8825D;
	g_stStaticInfo.stStaticApps[i].iProcessNum = 2; //同一个地址下，不能重复，不然参与者id会重复
	g_stStaticInfo.stStaticApps[i].usPort[0] = 5678;
	g_stStaticInfo.stStaticApps[i].usPort[1] = 5679;
	g_stStaticInfo.stStaticApps[i].iWriteTopicNum = 1;
	g_stStaticInfo.stStaticApps[i].WriteTopic[0].sTopicName = "testTopic1";
	g_stStaticInfo.stStaticApps[i].WriteTopic[0].sTopicType = "DDS::STRING";
	g_stStaticInfo.stStaticApps[i].iReadTopicNum = 0;
}
int g_iLocalStaticIndex = -1;
void InitSTATICINOF(int localIndex)
{
#ifdef _FC
	int i = 0;
	for (; i < g_stStaticInfo.iAppSize; i++)
	{
		// FC根据节点ID匹配，找到当前应用的下标
		if (g_commFCConfig.m_thisNodeFCPort == g_stStaticInfo.stStaticApps[i].iStaticAddr)
		{
			g_iLocalStaticIndex = i;
			return;
		}
	}
#else
	g_iLocalStaticIndex = localIndex; //记录当前应用的下标
#endif
}

RETSTATUS GenStaticPrefix(StaticParticipant appConfig, GuidPrefix_t *guidPrefix)
{
	if (g_iLocalStaticIndex < 0) 
	{
		printf("未配置当前静态节点号\n");
		return STA_UNREG_PREFIX;
	}
	memset(guidPrefix->value, 0, 12);
	getStrFromLong(appConfig.iStaticAddr, &guidPrefix->value[0], &guidPrefix->value[1], 
		&guidPrefix->value[2], &guidPrefix->value[3]); /* 0-3 网络地址 */
	getStrFromLong(appConfig.iProcessNum,&guidPrefix->value[4], &guidPrefix->value[5],
		&guidPrefix->value[6], &guidPrefix->value[7]); /* 4-7 进程id */
	/* 8-11 实例id，默认为0，不区分实例，同一个进程都用一个参与者 */
	return STA_OK;
}

void CreateStaticLocalDataWriter(DDS_DomainParticipant* pstParticipant, const CHAR* pcTopicName, const CHAR* pcTopicType)
{
	DDS_Topic* pstTopic = NULL;
	DDS_DataWriter* pstDataWriter = NULL;
	DDS_DiscoveredReader*  pstDiscReader = NULL;
	DDS_DataWriter* pstTempDataWriter = NULL;
	static UINT16  uiStaticDataWriterKind = 0x1000;

	pstTopic = DDS_DomainParticipant_get_topic_by_name(pstParticipant, pcTopicName, pcTopicType);

	if (NULL == pstTopic)
	{
		pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, pcTopicName, pcTopicType, &DDS_TOPIC_QOS_DEFAULT);
	}

	pstDataWriter = (DDS_DataWriter*)DDS_STATIC_MALLOC(sizeof(DDS_DataWriter));
	if (NULL == pstDataWriter)
	{
		PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData, Insufficient memory space.\n");
		return;
	}

	DDS_DataWriter_Init(pstDataWriter);

	pstDataWriter->stDataWriterQos = DDS_DATAWRITER_QOS_DEFAULT;

	/* 保存父节点,方便访问上层数据 */
	pstDataWriter->pstTopic = pstTopic;

	/* 生成写入器GUID */
	pstDataWriter->guid.prefix = pstTopic->pstParticipant->stRTPSMsgHeader.guidPrefix;
	pstDataWriter->guid.entityId.entityKind = ENTITYKIND_WRITER_NO_KEY;
	memset(pstDataWriter->guid.entityId.entityKey, 0, 3);
	memcpy(pstDataWriter->guid.entityId.entityKey, &uiStaticDataWriterKind, sizeof(UINT16));
	uiStaticDataWriterKind++;

	LIST_INSERT_TAIL(pstTopic->pstLocalWriter, pstDataWriter, pstTempDataWriter);
	/* 远端reader添加本地writer */
	pstDiscReader = pstTopic->pstDiscReader;

	while (NULL != pstDiscReader)
	{
		DiscoveredReaderAddLocalWriter(pstDiscReader, &pstDataWriter->guid);
		pstDiscReader = pstDiscReader->pNext;
	}
}

void CreateStaticLocalDataReader(DDS_DomainParticipant* pstParticipant, const CHAR* pcTopicName, const CHAR* pcTopicType)
{
	DDS_Topic* pstTopic = NULL;
	DDS_DataReader*  pstDataReader = NULL;
	DDS_DataReader*  pstTempDataWriter = NULL;
	static UINT16  uiStaticDataReaderKind = 0x2000;

	pstTopic = DDS_DomainParticipant_get_topic_by_name(pstParticipant, pcTopicName, pcTopicType);

	if (NULL == pstTopic)
	{
		pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, pcTopicName, pcTopicType, &DDS_TOPIC_QOS_DEFAULT);
	}

	pstDataReader = (DDS_DataReader*)DDS_STATIC_MALLOC(sizeof(DDS_DataReader));
	if (NULL == pstDataReader)
	{
		PrintLog(COMMLOG_ERROR, "DataReader: Failed to create HistoryData, Insufficient memory space.\n");
		return;
	}

	/* 插入本地阅读器qos */
	pstDataReader->stDataReaderQos = DDS_DATAREADER_QOS_DEFAULT;

	/* 插入回调函数 */
	pstDataReader->recvCallBack = NULL;

	/* 初始化本地阅读器 */
	DDS_DataReader_Init(pstDataReader);

	pstDataReader->guid.prefix = pstTopic->pstParticipant->stRTPSMsgHeader.guidPrefix;
	pstDataReader->guid.entityId.entityKind = ENTITYKIND_READER_NO_KEY;
	memset(pstDataReader->guid.entityId.entityKey, 0, 3);
	memcpy(pstDataReader->guid.entityId.entityKey, &uiStaticDataReaderKind, sizeof(UINT16));
	uiStaticDataReaderKind++;

	LIST_INSERT_TAIL(pstTopic->pstLocalReader, pstDataReader, pstTempDataWriter);

	/* 保存父节点 */
	pstDataReader->pstTopic = pstTopic;
}

int AddStaticDiscDomainParticipant(DDS_DomainParticipant* pstParticipant)
{
	DDS_DiscoveredParticipant * pstDiscParticipant = NULL;
	Locator_t* pstUnicastLocator = NULL;
	Locator_t* pstUserLocator = NULL;
	Locator_t* pstTempLocator = NULL;
	int i;
	UINT32 uiIp;
	for (i = 0; i < g_stStaticInfo.iAppSize; i++)
	{
		if (i == g_iLocalStaticIndex)
			continue;

		/* 申请内存用于发现远端Participant */
		pstDiscParticipant = (DDS_DiscoveredParticipant*)DDS_STATIC_MALLOC(sizeof(DDS_DiscoveredParticipant));
		if (NULL == pstDiscParticipant)
		{
			PrintLog(COMMLOG_ERROR, "SPDPDiscoveredParticipantData: Failed to create DiscoveredParticipant, Insufficient memory space.\n");
			return FALSE;
		}

		/* 初始化远端Participant */
		DDS_DiscoveredParticipant_Init(pstDiscParticipant);

		pstDiscParticipant->stRTPSMsgHeader.protocolId = PROTOCOL_RTPS_LITTLE_ENDIAN;
		pstDiscParticipant->stRTPSMsgHeader.vendorId = VENDORID_UNKNOWN;
		pstDiscParticipant->stRTPSMsgHeader.protocolVersion = PROTOCOLVERSION;

		pstDiscParticipant->rtpsParticipantProxy.availableBuiltinEndpoints =
			DISC_BUILTIN_ENDPOINT_PARTICIPANT_ANNOUNCER |
			DISC_BUILTIN_ENDPOINT_PARTICIPANT_DETECTOR |
			DISC_BUILTIN_ENDPOINT_PUBLICATION_ANNOUNCER |
			DISC_BUILTIN_ENDPOINT_PUBLICATION_DETECTOR |
			DISC_BUILTIN_ENDPOINT_SUBSCRIPTION_ANNOUNCER |
			DISC_BUILTIN_ENDPOINT_SUBSCRIPTION_DETECTOR;

		pstDiscParticipant->leaseDuration.sec = 2;
		pstDiscParticipant->leaseDuration.nanosec = 0;

		/* 生成静态前缀id */
		GenStaticPrefix(g_stStaticInfo.stStaticApps[i], &pstDiscParticipant->stRTPSMsgHeader.guidPrefix);
		uiIp = g_stStaticInfo.stStaticApps[i].iStaticAddr;
		/* 封装单播地址 */
		pstUnicastLocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
		if (NULL != pstUnicastLocator)
		{
			memset(pstUnicastLocator->address, 0, 16);			
			//定位器地址的最高4个字节保存ip地址
			IpUintToStr(uiIp, pstUnicastLocator->address);
			pstUnicastLocator->kind = LOCATOR_KIND_UDPv4;
			pstUnicastLocator->port = g_stStaticInfo.stStaticApps[i].usPort[0];
			LIST_INSERT_TAIL(pstDiscParticipant->rtpsParticipantProxy.metatrafficUnicastLocator, pstUnicastLocator, pstTempLocator);
		}
		/* 封装USER单播地址 */
		pstUserLocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
		if (NULL != pstUserLocator)
		{
			memset(pstUserLocator->address, 0, 16);
			
			//定位器地址的最高4个字节保存ip地址
			IpUintToStr(uiIp, pstUserLocator->address);
			pstUserLocator->kind = LOCATOR_KIND_UDPv4;
			pstUserLocator->port = g_stStaticInfo.stStaticApps[i].usPort[1];
			LIST_INSERT_TAIL(pstDiscParticipant->rtpsParticipantProxy.defaultUnicastLocator, pstUserLocator, pstTempLocator);
		}

		LIST_INSERT_HEAD(pstParticipant->pstDiscParticipant, pstDiscParticipant);

		AddStaticDiscWriter(pstParticipant, pstDiscParticipant, g_stStaticInfo.stStaticApps[i]);

		AddStaticDiscReader(pstParticipant, pstDiscParticipant, g_stStaticInfo.stStaticApps[i]);
	}
	return 0;
}

int AddStaticDiscWriter(DDS_DomainParticipant* pstParticipant, DDS_DiscoveredParticipant * pstDiscParticipant, StaticParticipant stStaticApps)
{
	DDS_DiscoveredWriter* pstDiscWriter = NULL;
	DDS_Topic* pstTopic = NULL;
	UINT32 uiStaticDiscDataWriterKind = 0x1000; //writer类型实例序号，每添加一个实例，增加1
	int i;
	for (i = 0; i < stStaticApps.iWriteTopicNum; i++)
	{	
		pstTopic = DDS_DomainParticipant_get_topic_by_name(pstParticipant, stStaticApps.WriteTopic[i].sTopicName, stStaticApps.WriteTopic[i].sTopicType);

		if (NULL == pstTopic)
		{
			pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, stStaticApps.WriteTopic[i].sTopicName, stStaticApps.WriteTopic[i].sTopicType, &DDS_TOPIC_QOS_DEFAULT);
		}

		/* 创建远端写入器申请内存 */
		pstDiscWriter = (DDS_DiscoveredWriter*)DDS_STATIC_MALLOC(sizeof(DDS_DiscoveredWriter));
		if (NULL == pstDiscWriter)
		{
			PrintLog(COMMLOG_ERROR, "SEDPDiscoveredWriter: Failed to create remote DiscoveredWriter, Insufficient memory space.\n");
			return FALSE;
		}

		/* 初始化写入器 */
		DDS_DiscoveredWriter_Init(pstDiscWriter);
		uiStaticDiscDataWriterKind = uiStaticDiscDataWriterKind + i;
		memcpy(pstDiscWriter->guid.prefix.value, pstDiscParticipant->stRTPSMsgHeader.guidPrefix.value, 12);
		pstDiscWriter->guid.entityId.entityKind = ENTITYKIND_WRITER_NO_KEY;
		memset(pstDiscWriter->guid.entityId.entityKey, 0, 3);
		memcpy(pstDiscWriter->guid.entityId.entityKey, &uiStaticDiscDataWriterKind, sizeof(UINT16));

		pstDiscWriter->pstRelateTopic = pstTopic;

		pstDiscWriter->pstUnicastLocator = pstDiscParticipant->rtpsParticipantProxy.defaultUnicastLocator;

		/* 插入父节点，方便访问上层数据结构 */
		pstDiscWriter->pstTopic = pstTopic;

		/* Nephalem 新Topic恢复陈默认无partition状态 */
		UnInitTopicPartitionQoS(&pstTopic->stTopicQos);

		/* 远端写入器插入到新主题下*/
		LIST_INSERT_HEAD(pstTopic->pstDiscWriter, pstDiscWriter);

		/* 插入父节点，方便访问上层数据结构 */
		pstTopic->pstParticipant = pstParticipant;
	}
	return 0;
}

int AddStaticDiscReader(DDS_DomainParticipant* pstParticipant, DDS_DiscoveredParticipant * pstDiscParticipant, StaticParticipant stStaticApps)
{
	DDS_DiscoveredReader* pstDiscReader = NULL;
	DDS_Topic* pstTopic = NULL;
	DDS_Topic* pstTempTopic = NULL;
	UINT32 uiStaticDiscDataReaderKind = 0x2000; //reader类型实例序号，每添加一个实例，增加1
	int i;
	for (i = 0; i < stStaticApps.iReadTopicNum; i++)
	{
		pstTopic = DDS_DomainParticipant_get_topic_by_name(pstParticipant, stStaticApps.ReadTopic[i].sTopicName, stStaticApps.ReadTopic[i].sTopicType);

		if (NULL == pstTopic)
		{
			pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, stStaticApps.ReadTopic[i].sTopicName, stStaticApps.ReadTopic[i].sTopicType, &DDS_TOPIC_QOS_DEFAULT);
		}

		/* 创建远端写入器申请内存 */
		pstDiscReader = (DDS_DiscoveredReader*)DDS_STATIC_MALLOC(sizeof(DDS_DiscoveredReader));
		if (NULL == pstDiscReader)
		{
			/* 失败需释放之前创建的主题并返回 */
			DDS_STATIC_FREE(pstTopic);
			PrintLog(COMMLOG_ERROR, "SEDPDiscoveredReader: Failed to create remote DiscoveredWriter, Insufficient memory space.\n");
			return FALSE;
		}

		/* 初始化远端阅读器 */
		DDS_DiscoveredReader_Init(pstDiscReader);

		uiStaticDiscDataReaderKind = uiStaticDiscDataReaderKind + i;
		memcpy(pstDiscReader->guid.prefix.value, pstDiscParticipant->stRTPSMsgHeader.guidPrefix.value, 12);
		pstDiscReader->guid.entityId.entityKind = ENTITYKIND_READER_NO_KEY;
		memset(pstDiscReader->guid.entityId.entityKey, 0, 3);
		memcpy(pstDiscReader->guid.entityId.entityKey, &uiStaticDiscDataReaderKind, sizeof(UINT16));

		//memcpy(pstTopic->topicName.value, stStaticApps.ReadTopic[i], strlen(stStaticApps.ReadTopic[i]) + 1);
		//pstTopic->topicName.length = strlen(stStaticApps.ReadTopic[i]) + 1;

		//memcpy(pstTopic->topicType.value, pstDataType, strlen(pstDataType) + 1);
		//pstTopic->topicType.length = strlen(pstDataType) + 1;

		/* Nephalem 远端阅读器自身相关的主题设置信息 */
		pstDiscReader->pstRelateTopic = pstTopic;

		pstDiscReader->pstUnicastLocator = pstDiscParticipant->rtpsParticipantProxy.defaultUnicastLocator;

		/* 校验本地是否存在该主题 */
		pstTempTopic = pstParticipant->pstTopic;

		/* 插入父节点，方便访问上层数据结构 */
		pstDiscReader->pstTopic = pstTopic;

		/* Nephalem 新Topic恢复陈默认无partition状态 */
		UnInitTopicPartitionQoS(&pstTopic->stTopicQos);

		/* 远端写入器插入到新主题下*/
		LIST_INSERT_HEAD(pstTopic->pstDiscReader, pstDiscReader);

		/* 插入父节点，方便访问上层数据结构 */
		pstTopic->pstParticipant = pstParticipant;
	}
	return 0;
}
