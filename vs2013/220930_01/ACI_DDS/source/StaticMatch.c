#include "../include/GlobalDefine.h"

DDS_DLL STATICINOF g_stStaticInfo;

void TestInitStaticConfig()
{
	int i = 0;
	g_stStaticInfo.iAppSize = 2;
	//0
	g_stStaticInfo.stStaticApps[i].iStaticAddr = 0xC0A8825D;//С��ģʽ��ntohl(inet_addr("192.168.130.93"));
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
	g_stStaticInfo.stStaticApps[i].iProcessNum = 2; //ͬһ����ַ�£������ظ�����Ȼ������id���ظ�
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
		// FC���ݽڵ�IDƥ�䣬�ҵ���ǰӦ�õ��±�
		if (g_commFCConfig.m_thisNodeFCPort == g_stStaticInfo.stStaticApps[i].iStaticAddr)
		{
			g_iLocalStaticIndex = i;
			return;
		}
	}
#else
	g_iLocalStaticIndex = localIndex; //��¼��ǰӦ�õ��±�
#endif
}

RETSTATUS GenStaticPrefix(StaticParticipant appConfig, GuidPrefix_t *guidPrefix)
{
	if (g_iLocalStaticIndex < 0) 
	{
		printf("δ���õ�ǰ��̬�ڵ��\n");
		return STA_UNREG_PREFIX;
	}
	memset(guidPrefix->value, 0, 12);
	getStrFromLong(appConfig.iStaticAddr, &guidPrefix->value[0], &guidPrefix->value[1], 
		&guidPrefix->value[2], &guidPrefix->value[3]); /* 0-3 �����ַ */
	getStrFromLong(appConfig.iProcessNum,&guidPrefix->value[4], &guidPrefix->value[5],
		&guidPrefix->value[6], &guidPrefix->value[7]); /* 4-7 ����id */
	/* 8-11 ʵ��id��Ĭ��Ϊ0��������ʵ����ͬһ�����̶���һ�������� */
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

	/* ���游�ڵ�,��������ϲ����� */
	pstDataWriter->pstTopic = pstTopic;

	/* ����д����GUID */
	pstDataWriter->guid.prefix = pstTopic->pstParticipant->stRTPSMsgHeader.guidPrefix;
	pstDataWriter->guid.entityId.entityKind = ENTITYKIND_WRITER_NO_KEY;
	memset(pstDataWriter->guid.entityId.entityKey, 0, 3);
	memcpy(pstDataWriter->guid.entityId.entityKey, &uiStaticDataWriterKind, sizeof(UINT16));
	uiStaticDataWriterKind++;

	LIST_INSERT_TAIL(pstTopic->pstLocalWriter, pstDataWriter, pstTempDataWriter);
	/* Զ��reader��ӱ���writer */
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

	/* ���뱾���Ķ���qos */
	pstDataReader->stDataReaderQos = DDS_DATAREADER_QOS_DEFAULT;

	/* ����ص����� */
	pstDataReader->recvCallBack = NULL;

	/* ��ʼ�������Ķ��� */
	DDS_DataReader_Init(pstDataReader);

	pstDataReader->guid.prefix = pstTopic->pstParticipant->stRTPSMsgHeader.guidPrefix;
	pstDataReader->guid.entityId.entityKind = ENTITYKIND_READER_NO_KEY;
	memset(pstDataReader->guid.entityId.entityKey, 0, 3);
	memcpy(pstDataReader->guid.entityId.entityKey, &uiStaticDataReaderKind, sizeof(UINT16));
	uiStaticDataReaderKind++;

	LIST_INSERT_TAIL(pstTopic->pstLocalReader, pstDataReader, pstTempDataWriter);

	/* ���游�ڵ� */
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

		/* �����ڴ����ڷ���Զ��Participant */
		pstDiscParticipant = (DDS_DiscoveredParticipant*)DDS_STATIC_MALLOC(sizeof(DDS_DiscoveredParticipant));
		if (NULL == pstDiscParticipant)
		{
			PrintLog(COMMLOG_ERROR, "SPDPDiscoveredParticipantData: Failed to create DiscoveredParticipant, Insufficient memory space.\n");
			return FALSE;
		}

		/* ��ʼ��Զ��Participant */
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

		/* ���ɾ�̬ǰ׺id */
		GenStaticPrefix(g_stStaticInfo.stStaticApps[i], &pstDiscParticipant->stRTPSMsgHeader.guidPrefix);
		uiIp = g_stStaticInfo.stStaticApps[i].iStaticAddr;
		/* ��װ������ַ */
		pstUnicastLocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
		if (NULL != pstUnicastLocator)
		{
			memset(pstUnicastLocator->address, 0, 16);			
			//��λ����ַ�����4���ֽڱ���ip��ַ
			IpUintToStr(uiIp, pstUnicastLocator->address);
			pstUnicastLocator->kind = LOCATOR_KIND_UDPv4;
			pstUnicastLocator->port = g_stStaticInfo.stStaticApps[i].usPort[0];
			LIST_INSERT_TAIL(pstDiscParticipant->rtpsParticipantProxy.metatrafficUnicastLocator, pstUnicastLocator, pstTempLocator);
		}
		/* ��װUSER������ַ */
		pstUserLocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
		if (NULL != pstUserLocator)
		{
			memset(pstUserLocator->address, 0, 16);
			
			//��λ����ַ�����4���ֽڱ���ip��ַ
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
	UINT32 uiStaticDiscDataWriterKind = 0x1000; //writer����ʵ����ţ�ÿ���һ��ʵ��������1
	int i;
	for (i = 0; i < stStaticApps.iWriteTopicNum; i++)
	{	
		pstTopic = DDS_DomainParticipant_get_topic_by_name(pstParticipant, stStaticApps.WriteTopic[i].sTopicName, stStaticApps.WriteTopic[i].sTopicType);

		if (NULL == pstTopic)
		{
			pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, stStaticApps.WriteTopic[i].sTopicName, stStaticApps.WriteTopic[i].sTopicType, &DDS_TOPIC_QOS_DEFAULT);
		}

		/* ����Զ��д���������ڴ� */
		pstDiscWriter = (DDS_DiscoveredWriter*)DDS_STATIC_MALLOC(sizeof(DDS_DiscoveredWriter));
		if (NULL == pstDiscWriter)
		{
			PrintLog(COMMLOG_ERROR, "SEDPDiscoveredWriter: Failed to create remote DiscoveredWriter, Insufficient memory space.\n");
			return FALSE;
		}

		/* ��ʼ��д���� */
		DDS_DiscoveredWriter_Init(pstDiscWriter);
		uiStaticDiscDataWriterKind = uiStaticDiscDataWriterKind + i;
		memcpy(pstDiscWriter->guid.prefix.value, pstDiscParticipant->stRTPSMsgHeader.guidPrefix.value, 12);
		pstDiscWriter->guid.entityId.entityKind = ENTITYKIND_WRITER_NO_KEY;
		memset(pstDiscWriter->guid.entityId.entityKey, 0, 3);
		memcpy(pstDiscWriter->guid.entityId.entityKey, &uiStaticDiscDataWriterKind, sizeof(UINT16));

		pstDiscWriter->pstRelateTopic = pstTopic;

		pstDiscWriter->pstUnicastLocator = pstDiscParticipant->rtpsParticipantProxy.defaultUnicastLocator;

		/* ���븸�ڵ㣬��������ϲ����ݽṹ */
		pstDiscWriter->pstTopic = pstTopic;

		/* Nephalem ��Topic�ָ���Ĭ����partition״̬ */
		UnInitTopicPartitionQoS(&pstTopic->stTopicQos);

		/* Զ��д�������뵽��������*/
		LIST_INSERT_HEAD(pstTopic->pstDiscWriter, pstDiscWriter);

		/* ���븸�ڵ㣬��������ϲ����ݽṹ */
		pstTopic->pstParticipant = pstParticipant;
	}
	return 0;
}

int AddStaticDiscReader(DDS_DomainParticipant* pstParticipant, DDS_DiscoveredParticipant * pstDiscParticipant, StaticParticipant stStaticApps)
{
	DDS_DiscoveredReader* pstDiscReader = NULL;
	DDS_Topic* pstTopic = NULL;
	DDS_Topic* pstTempTopic = NULL;
	UINT32 uiStaticDiscDataReaderKind = 0x2000; //reader����ʵ����ţ�ÿ���һ��ʵ��������1
	int i;
	for (i = 0; i < stStaticApps.iReadTopicNum; i++)
	{
		pstTopic = DDS_DomainParticipant_get_topic_by_name(pstParticipant, stStaticApps.ReadTopic[i].sTopicName, stStaticApps.ReadTopic[i].sTopicType);

		if (NULL == pstTopic)
		{
			pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, stStaticApps.ReadTopic[i].sTopicName, stStaticApps.ReadTopic[i].sTopicType, &DDS_TOPIC_QOS_DEFAULT);
		}

		/* ����Զ��д���������ڴ� */
		pstDiscReader = (DDS_DiscoveredReader*)DDS_STATIC_MALLOC(sizeof(DDS_DiscoveredReader));
		if (NULL == pstDiscReader)
		{
			/* ʧ�����ͷ�֮ǰ���������Ⲣ���� */
			DDS_STATIC_FREE(pstTopic);
			PrintLog(COMMLOG_ERROR, "SEDPDiscoveredReader: Failed to create remote DiscoveredWriter, Insufficient memory space.\n");
			return FALSE;
		}

		/* ��ʼ��Զ���Ķ��� */
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

		/* Nephalem Զ���Ķ���������ص�����������Ϣ */
		pstDiscReader->pstRelateTopic = pstTopic;

		pstDiscReader->pstUnicastLocator = pstDiscParticipant->rtpsParticipantProxy.defaultUnicastLocator;

		/* У�鱾���Ƿ���ڸ����� */
		pstTempTopic = pstParticipant->pstTopic;

		/* ���븸�ڵ㣬��������ϲ����ݽṹ */
		pstDiscReader->pstTopic = pstTopic;

		/* Nephalem ��Topic�ָ���Ĭ����partition״̬ */
		UnInitTopicPartitionQoS(&pstTopic->stTopicQos);

		/* Զ��д�������뵽��������*/
		LIST_INSERT_HEAD(pstTopic->pstDiscReader, pstDiscReader);

		/* ���븸�ڵ㣬��������ϲ����ݽṹ */
		pstTopic->pstParticipant = pstParticipant;
	}
	return 0;
}
