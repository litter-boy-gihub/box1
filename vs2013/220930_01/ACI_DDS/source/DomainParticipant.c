#include "../include/GlobalDefine.h"

#define HEART_TIME 1000

static int init_num = 0;
BOOL g_RapdIOValid = FALSE;
BOOL g_ConfValid = FALSE;
DDS_MUTEX g_sendMutex;  //线程加锁
/*前置声明*/
VOID DiscHEARTCheckTask(DDS_DomainParticipant* pstParticipant);

DDS_DomainParticipant* pstRealDomainParticipant = NULL;
#ifdef _FC
YCCommFCConfig g_commFCConfig = {0, 0, 0, { 0 } };

extern int iTestTimes;
extern int iTestOptions;
#define DCFG_CCSR_PAMUBYPENR 0xF20E0604
#elif defined _653_PORT
Port653Config g_PortConfig; 

#endif

extern int iDDSRECV;

extern int iUSERRECV;

#ifdef  STATIC_DISCOVERY
extern STATICINOF g_stStaticInfo;
extern int g_iLocalStaticIndex;
DDS_DomainParticipant *g_pDomainParticipant = NULL; //限制一个应用只能创建一个参与者
#endif //  STATIC_DISCOVERY

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_ParticipantFactory_init
-   功能描述: 资源初始化
-   输    入: 无
-   返    回:
-   全局变量:
-   注    释: 资源初始化
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2019\7\23       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL VOID DDS_ParticipantFactory_init()
{
    if (0 == init_num)
    {
		DDS_MUTEX_INIT(g_sendMutex);
        /* 日志模块初始化 */
        InitCommLog();

        /* 任务列表初始化 */
        InitializeSubTasks();

#if defined(USE_STATIC_MALLOC) || defined(USE_STATIC_MALLOC_T)
		/* 静态内存初始化 */
		InitMemBlock();
#endif

#ifdef RAPID_IO_CHANNEL
        if (0 != InitRioNetWork())
        {
            g_RapdIOValid = TRUE;
        }
#endif
    }

    init_num++;
}

/*---------------------------------------------------------------------------------
-   函 数 名: InitDomainParticipant
-   功能描述: 初始化participant
-   输    入: participant指针
-   返    回: 
-   全局变量: 
-   注    释: 初始化participant成员
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2019\7\23       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID InitDomainParticipant(DDS_DomainParticipant* pstDomainParticipant)
{
	UINT32 uiNum;
    if (NULL != pstDomainParticipant)
    {
        DDS_MUTEX_INIT(pstDomainParticipant->threadMutex);
        DDS_MUTEX_INIT(pstDomainParticipant->m_discWtMutex);
        DDS_MUTEX_INIT(pstDomainParticipant->m_discRdMutex);
        DDS_MUTEX_INIT(pstDomainParticipant->monMutex);
		DDS_MUTEX_INIT(pstDomainParticipant->sleepMutex);
 
#ifdef  STATIC_DISCOVERY
		if (-1 != g_iLocalStaticIndex)
			pstDomainParticipant->bOnlyStaticMathc = TRUE;
		else
			pstDomainParticipant->bOnlyStaticMathc = FALSE;
#endif

        pstDomainParticipant->threadSignal = TRUE;
        /* 初始化默认qos */
        InitialDefaultQos();

        /* 初始化心跳时标 */
        HPTB_INIT(pstDomainParticipant->heartBeatMark);
        HPTB_BEGIN(pstDomainParticipant->heartBeatMark);
        
		if(IS_MACHINE_LITTLE_ENDIAN)
			pstDomainParticipant->stRTPSMsgHeader.protocolId = PROTOCOL_RTPS_LITTLE_ENDIAN;
		else
			pstDomainParticipant->stRTPSMsgHeader.protocolId = PROTOCOL_RTPS_BIG_ENDIAN;

        pstDomainParticipant->stRTPSMsgHeader.vendorId = VENDORID_UNKNOWN;
        pstDomainParticipant->stRTPSMsgHeader.protocolVersion = PROTOCOLVERSION;
        pstDomainParticipant->destroyFlag = FALSE;

		pstDomainParticipant->sleepTime = 1000;
		pstDomainParticipant->livelinessNum = 0;
		pstDomainParticipant->livelinessDuration.sec = 0;
		pstDomainParticipant->livelinessDuration.nanosec = 0;
		pstDomainParticipant->discMinDuration = pstDomainParticipant->livelinessDuration;
        pstDomainParticipant->pstDiscParticipant = NULL;
        pstDomainParticipant->pstTopic = NULL;
        pstDomainParticipant->pstMonitor = NULL;
        pstDomainParticipant->bLoopBack = TRUE;

		pstDomainParticipant->ipAddr = 0;
		pstDomainParticipant->processId = 0;
//        pstDomainParticipant->netMask = 0;

		pstDomainParticipant->pstParticipantQos = DDS_PARTICIPANT_QOS_DEFAULT;

        pstDomainParticipant->rtpsParticipantProxy.defaultUnicastLocator = NULL;
        pstDomainParticipant->rtpsParticipantProxy.defaultMulticastLocator = NULL;
        pstDomainParticipant->rtpsParticipantProxy.metatrafficMulticastLocator = NULL;
        pstDomainParticipant->rtpsParticipantProxy.metatrafficUnicastLocator = NULL;

		pstDomainParticipant->dcpsParticipantData.userData.value.length = 0;
		SetPropertyList(&pstDomainParticipant->dcpsParticipantData.propertyList);

        pstDomainParticipant->rtpsParticipantProxy.availableBuiltinEndpoints =
            DISC_BUILTIN_ENDPOINT_PARTICIPANT_ANNOUNCER  |
            DISC_BUILTIN_ENDPOINT_PARTICIPANT_DETECTOR   |
            DISC_BUILTIN_ENDPOINT_PUBLICATION_ANNOUNCER  |
            DISC_BUILTIN_ENDPOINT_PUBLICATION_DETECTOR   |
            DISC_BUILTIN_ENDPOINT_SUBSCRIPTION_ANNOUNCER |
            DISC_BUILTIN_ENDPOINT_SUBSCRIPTION_DETECTOR;

       
        /* 内建读写器初始化 */
        for (uiNum = 0; uiNum < 3; uiNum++)
        {
            pstDomainParticipant->stBuiltinDataWriter[uiNum].guid.prefix = pstDomainParticipant->stRTPSMsgHeader.guidPrefix;
            pstDomainParticipant->stBuiltinDataReader[uiNum].guid.prefix = pstDomainParticipant->stRTPSMsgHeader.guidPrefix;
            InitBuiltinDataWriter(&pstDomainParticipant->stBuiltinDataWriter[uiNum]);
            InitBuiltinDataReader(&pstDomainParticipant->stBuiltinDataReader[uiNum]);
        }

        pstDomainParticipant->stBuiltinDataWriter[0].guid.entityId = ENTITYID_SPDP_BUILTIN_PARTICIPANT_WRITER;
        pstDomainParticipant->stBuiltinDataReader[0].guid.entityId = ENTITYID_SPDP_BUILTIN_PARTICIPANT_READER;
        pstDomainParticipant->stBuiltinDataWriter[0].pstParticipant = pstDomainParticipant;
        pstDomainParticipant->stBuiltinDataReader[0].pstParticipant = pstDomainParticipant;

        pstDomainParticipant->stBuiltinDataWriter[1].guid.entityId = ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER;
        pstDomainParticipant->stBuiltinDataReader[1].guid.entityId = ENTITYID_SEDP_BUILTIN_PUBLICATIONS_READER;
        pstDomainParticipant->stBuiltinDataWriter[1].pstParticipant = pstDomainParticipant;
        pstDomainParticipant->stBuiltinDataReader[1].pstParticipant = pstDomainParticipant;

        pstDomainParticipant->stBuiltinDataWriter[2].guid.entityId = ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_WRITER;
        pstDomainParticipant->stBuiltinDataReader[2].guid.entityId = ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_READER;
        pstDomainParticipant->stBuiltinDataWriter[2].pstParticipant = pstDomainParticipant;
        pstDomainParticipant->stBuiltinDataReader[2].pstParticipant = pstDomainParticipant;
    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_DomainParticipantFactory_create_participant
-   功能描述: 创建participant
-   输    入: domainId、participantd的qos
-   返    回: participant指针
-   全局变量: g_domainParticipant
-   注    释: 创建participant
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2019\7\23       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_DomainParticipant* DDS_DomainParticipantFactory_create_participant(USHORT domainId, DDS_DomainParticipantQos* pstDomPartQos)
{
	MemoryBlock stMemBlock;
	DDS_DomainParticipant*  pstDomainParticipant = NULL;
	HistoryData* pstHistoryData = NULL;

#ifdef  STATIC_DISCOVERY
	if (g_pDomainParticipant != NULL)
	{
		return g_pDomainParticipant;
	}
#endif //  STATIC_DISCOVERY

	/* 传入参数，有效性检查 */
#ifdef QYDDS_LIVELINESS_QOS
	if (pstDomPartQos != NULL)
	{
		if (pstDomPartQos->liveliness.kind != AUTOMATIC_LIVELINESS_QOS && pstDomPartQos->liveliness.lease_duration.sec == 0 && pstDomPartQos->liveliness.lease_duration.nanosec == 0)
		{
			printf("LIVELINESS lease_duration Error!\n");
			return NULL;
		}
	}
#endif
	if (domainId > 255)
	{
		printf("Error domainId, legal range: [0,255]!\n");
		return NULL;
	}

	if (pstDomPartQos != NULL)
	{
		if (strcmp(pstDomPartQos->property.ipAddr, DDS_PARTICIPANT_QOS_DEFAULT.property.ipAddr)!=0)
		{
			/* TODO：给全局配置的IP地址赋值；这种方式如果创建多个参与者，配置了不同地址，会存在问题，LJM */
			strncpy(g_ipAddr, pstDomPartQos->property.ipAddr, strlen(pstDomPartQos->property.ipAddr));
		}

		if (strcmp(pstDomPartQos->property.netCardName, DDS_PARTICIPANT_QOS_DEFAULT.property.netCardName) != 0)
			strncpy(g_netCardName, pstDomPartQos->property.netCardName, strlen(pstDomPartQos->property.netCardName));
	}

#if defined(_VXWORKS_653)
    /* 653任务无法传参，使用全局变量 */
	if(NULL != pstRealDomainParticipant)
		return pstRealDomainParticipant;
#endif
    
    /* 申请内存创建Participant */
    pstDomainParticipant = (DDS_DomainParticipant*)DDS_STATIC_MALLOC(sizeof(DDS_DomainParticipant));
    if (NULL == pstDomainParticipant)
    {
        PrintLog(COMMLOG_ERROR, "DomainParticipant: Failed to create Participant, Insufficient memory space.\n");
        return NULL;
    }
#if defined(_VXWORKS_653) || defined (_TM1) || defined _TM3
    /* 653任务无法传参，使用全局变量 */
    pstRealDomainParticipant = pstDomainParticipant;
#endif
    /* 申请静态内存创建HistoryData链表节点 */
    pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC(sizeof(HistoryData));
    if (NULL == pstHistoryData)
    {
        DDS_STATIC_FREE(pstDomainParticipant);
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData, Insufficient memory space.\n");
        return NULL;
    }

	/* Nephalem 进行初始化，否则释放会崩溃 */
	InitHistoryData(pstHistoryData);

    /* 申请静态内存创建历史缓存数据 */
    pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC(SEDP_BYTE_MAX_SIZE);
    if (NULL == pstHistoryData->data)
    {
        /* 失败释放前面申请的内存 */
        DDS_STATIC_FREE(pstDomainParticipant);
        DDS_STATIC_FREE(pstHistoryData);
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData buff, Insufficient memory space.\n");
        return NULL;
    }

    /* 初始化Participant，其中会初始化默认QoS */
    InitDomainParticipant(pstDomainParticipant);

    pstDomainParticipant->processId = getProcessId();
    /* 网络初始化 */
    if (!InitNetWorkStructure(pstDomainParticipant, domainId))
    {
		DDS_STATIC_FREE(pstHistoryData->data);
		DDS_STATIC_FREE(pstHistoryData);
		DDS_STATIC_FREE(pstDomainParticipant);
        PrintLog(COMMLOG_ERROR, "DomainParticipant: Failed to initialize NetWork!\n");
        return NULL;
    }

    /* 网络字节序初始化，保存发布报文 */
    INITIAL_MEMORYBLOCK(stMemBlock, pstHistoryData->data, SEDP_BYTE_MAX_SIZE);

    pstDomainParticipant->dcpsParticipantData.domainID = domainId;

    /* 每创建一个本地参与者，内建seqNum自加1*/
    pstDomainParticipant->stBuiltinDataWriter[0].seqNum = Add_SequenceNumber(pstDomainParticipant->stBuiltinDataWriter[0].seqNum, SEQUENCENUMBER_START);

    /* 参与者seqNum编号 */
    pstHistoryData->seqNum = pstDomainParticipant->stBuiltinDataWriter[0].seqNum;
#ifdef QYDDS_LIVELINESS_QOS
	if (pstDomPartQos != NULL)
	{
		pstDomainParticipant->pstParticipantQos.liveliness = pstDomPartQos->liveliness;
		pstDomainParticipant->livelinessDuration = pstDomPartQos->liveliness.lease_duration;
	}
#endif
    /* 序列化spdp报文 */
    ParticipantConvertNetworkByteOrder(pstDomainParticipant, &stMemBlock);

    /* 报文长度插入 */
    pstHistoryData->uiDataLen = stMemBlock.writeIndex;

    /* 插入到内建发布写入器缓存 */
    InsertHistoryCacheTail(&pstDomainParticipant->stBuiltinDataWriter[0].stHistoryCache, pstHistoryData);

 #ifdef RAPID_IO_CHANNEL
     HANDLE rioHandle = CreateThread(NULL, 0, (VOID*)&NetworkTransceiverTask, (VOID*)pstDomainParticipant, 0, NULL);
#ifdef QYDDS_LIVELINESS_QOS
     /*TODO：远端心跳检测报文*/
     HANDLE heartcheckhandle = CreateThread(NULL, 0, (VOID*)&DiscHEARTCheckTask, (VOID*)pstDomainParticipant, 0, NULL);
#endif

 #else
    printf("  [satrt NetworkSendReceiveTask ] \n");
 	NetworkSendReceiveTask(pstDomainParticipant);
 #endif

#ifdef  STATIC_DISCOVERY
	if(-1 != g_iLocalStaticIndex)
	{
		for (int i = 0; i < g_stStaticInfo.stStaticApps[g_iLocalStaticIndex].iWriteTopicNum; i++)
		{
			CreateStaticLocalDataWriter(pstDomainParticipant, g_stStaticInfo.stStaticApps[g_iLocalStaticIndex].WriteTopic[i].sTopicName, 
				g_stStaticInfo.stStaticApps[g_iLocalStaticIndex].WriteTopic[i].sTopicType);
		}

		for (int i = 0; i < g_stStaticInfo.stStaticApps[g_iLocalStaticIndex].iReadTopicNum; i++)
		{
			CreateStaticLocalDataReader(pstDomainParticipant, g_stStaticInfo.stStaticApps[g_iLocalStaticIndex].ReadTopic[i].sTopicName,
				g_stStaticInfo.stStaticApps[g_iLocalStaticIndex].ReadTopic[i].sTopicType);
		}
	}

	AddStaticDiscDomainParticipant(pstDomainParticipant); //静态远端节点配置
#endif
    //NetworkSendReceiveTask(pstDomainParticipant);
#ifdef  STATIC_DISCOVERY
	if (g_pDomainParticipant == NULL)
	{
		g_pDomainParticipant = pstDomainParticipant;
	}
#endif //  STATIC_DISCOVERY
    return pstDomainParticipant;
}

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_DomainParticipant_ignore_participant
-   功能描述: 阻止participant本地回环
-   输    入: pstDomainParticipan
-   返    回: 无
-   全局变量:
-   注    释: 阻止participant本地回环
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2020\03\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL VOID DDS_DomainParticipant_not_ignore_participant(DDS_DomainParticipant* pstDomainParticipant)
{
    pstDomainParticipant->bLoopBack = TRUE;
}

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_DomainParticipant_ignore_participant
-   功能描述: 阻止participant本地回环
-   输    入: pstDomainParticipan
-   返    回: 无
-   全局变量:
-   注    释: 阻止participant本地回环
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2020\03\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL VOID DDS_DomainParticipant_ignore_participant(DDS_DomainParticipant* pstDomainParticipant)
{
    pstDomainParticipant->bLoopBack = FALSE;
}

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_DomainParticipantFactory_delete_participant
-   功能描述: 删除participant
-   输    入: pstDomainParticipan
-   返    回: 返回码
-   全局变量: 
-   注    释: 删除participant
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2020\03\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/



DDS_DLL DDS_ReturnCode_t DDS_DomainParticipantFactory_delete_participant(DDS_DomainParticipant* pstDomainParticipant)
{
    UINT32 uiBuiltinNun = 0;
	Locator_t* pstTempNode = NULL;
	DDS_DiscoveredParticipant* pstDiscParticipant = NULL;
	DDS_Topic* pstTopic = NULL;

    if (NULL != pstDomainParticipant)
    {
        DDS_MUTEX_LOCK(pstDomainParticipant->threadMutex);

//         TerminateThread(pstDomainParticipant->hThread[0], 0);
//         TerminateThread(pstDomainParticipant->hThread[1], 0);
//         TerminateThread(pstDomainParticipant->hThread[2], 0);
//         TerminateThread(pstDomainParticipant->hThread[3], 0);
//         TerminateThread(pstDomainParticipant->hThread[4], 0);
// #ifdef QYDDS_LIVELINESS_QOS
// 		TerminateThread(pstDomainParticipant->hThread[5], 0);
// #endif
        pstDomainParticipant->destroyFlag = TRUE;

        pstDomainParticipant->threadSignal = FALSE;

		QY_Sleep(100);
        /* 所有成员指针析构 */
        
		LIST_DELETE_ALL(pstDomainParticipant->rtpsParticipantProxy.metatrafficUnicastLocator, pstTempNode);
		LIST_DELETE_ALL(pstDomainParticipant->rtpsParticipantProxy.metatrafficMulticastLocator, pstTempNode);
		LIST_DELETE_ALL(pstDomainParticipant->rtpsParticipantProxy.defaultUnicastLocator, pstTempNode);
		LIST_DELETE_ALL(pstDomainParticipant->rtpsParticipantProxy.defaultMulticastLocator, pstTempNode);

        /* 析构远端参与者 */
        pstDiscParticipant = pstDomainParticipant->pstDiscParticipant;
        while (NULL != pstDiscParticipant)
        {
            pstDomainParticipant->pstDiscParticipant = pstDiscParticipant->pNext;
            DDS_DiscoveredParticipant_Uninit(pstDiscParticipant);
            DDS_STATIC_FREE(pstDiscParticipant);
            pstDiscParticipant = pstDomainParticipant->pstDiscParticipant;
        }

        /* 析构所有主题 */
        pstTopic = pstDomainParticipant->pstTopic;
        while (NULL != pstTopic)
        {
            pstDomainParticipant->pstTopic = pstTopic->pNext;
            DDS_Topic_Uninit(pstTopic);
            DDS_STATIC_FREE(pstTopic);
            pstTopic = pstDomainParticipant->pstTopic;
        }

        /* 析构内建实体 */
        for (uiBuiltinNun = 0; uiBuiltinNun < 3; uiBuiltinNun++)
        {
            /* 内建写入器清除 */
            UnInitBuiltinDataWriter(&pstDomainParticipant->stBuiltinDataWriter[uiBuiltinNun]);

            /* 内建阅读器清除 */
            UnInitBuiltinDataReader(&pstDomainParticipant->stBuiltinDataReader[uiBuiltinNun]);
        }

		/* Nephalem 关闭Socket，否则内存泄露 */
		UnInitNetWorkStructure(pstDomainParticipant);

		CloseNetworkSendReceiveTask(pstDomainParticipant);

        DDS_MUTEX_UNLOCK(pstDomainParticipant->threadMutex);

        //等待一百毫秒再析构
        QY_Sleep(100);
        DDS_STATIC_FREE(pstDomainParticipant);
    }

    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_DomainParticipant_get_default_topic_qos
-   功能描述: 获取域参与者默认qos
-   输    入: participant、主题qos
-   返    回: 质量码
-   全局变量:
-   注    释: 通过participant获取一个默认主题qos
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2019\7\23       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t DDS_DomainParticipantFactory_get_defaul_participant_qos(DDS_DomainParticipantQos* pstDomPartQos)
{
    if (NULL == pstDomPartQos)
    {
        PrintLog(COMMLOG_ERROR, "ParticipantFactory: please pass in a non-null pointer of participant qos.\n");
        return DDS_RETCODE_ERROR;
    }

    *pstDomPartQos = DDS_PARTICIPANT_QOS_DEFAULT;
    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_DomainParticipant_get_default_topic_qos
-   功能描述: 获取域参与者默认qos
-   输    入: participant、主题qos
-   返    回: 质量码
-   全局变量:
-   注    释: 通过participant获取一个默认主题qos
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2019\7\23       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_DomainParticipantQos* DDS_DomainParticipantFactory_get_defaul_participant_qos_cs()
{
    DDS_DomainParticipantQos* pstDomPartQos = (DDS_DomainParticipantQos*)DDS_STATIC_MALLOC(sizeof(DDS_DomainParticipantQos));
    if (NULL == pstDomPartQos)
    {
        PrintLog(COMMLOG_ERROR, "ParticipantFactory: please pass in a non-null pointer of participant qos.\n");
        return NULL;
    }

    *pstDomPartQos = DDS_PARTICIPANT_QOS_DEFAULT;

    return pstDomPartQos;
}

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_Participant_bind_network
-   功能描述: 域参与者绑定网卡
-   输    入: 参与者qos
-   返    回: 无
-   全局变量:
-   注    释: 绑定网卡Ip
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2020\03\17       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL VOID DDS_Participant_bind_network(DDS_DomainParticipantQos* pstDomPartQos, const CHAR* pcIpAddr)
{
	strncpy(pstDomPartQos->property.ipAddr, pcIpAddr, strlen(pcIpAddr));
}

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_DomainParticipant_create_topic
-   功能描述: 创建主题
-   输    入: participant、主题名、主题类型、主题qos
-   返    回: 主题信息的指针
-   全局变量: 
-   注    释: 通过participant创建主题，若该主题存在则返回空指针，不存在则新建主题并添加到participant下的主题链表中
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2019\7\23       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_Topic*  DDS_DomainParticipant_create_topic(DDS_DomainParticipant* pstParticipant, const char* pcTopicName, const char* pcTopicType, DDS_TopicQos* pstTopicQos)
{
    DDS_Topic* pstTopic = pstParticipant->pstTopic;
    while (NULL != pstTopic)
    {
		//LJM，鲍进说前期topicQoS没起作用，所以当远端有时，没有保留本地Topic信息；一般来说本地topic会先建立，不会出现问题。但这样是不对的
        if (!strcmp(pstTopic->topicName.value, pcTopicName))
        {
            /* 若发现远端已创建该主题，类型一致，直接返回该主题 */
            if (Guid_Is_Equal(&pstTopic->tpGuid, &GUID_UNKNOWN) && !strcmp(pstTopic->topicType.value, pcTopicType))
            {
                /* 生成该主题本地GUID，上面通过GUID来判断是本地还是远端主题 */
                GenGuid(&pstTopic->tpGuid, &pstParticipant->stRTPSMsgHeader.guidPrefix, (ENTITYKIND_TOPIC << 24) | AUTO_ENTITYID);
                return pstTopic;
            }
			else if (!strcmp(pstTopic->topicType.value, pcTopicType))
            {
#ifdef EVO_DDS
				//printf("Warning: Created a completely same topic, topicName: %s\n", pcTopicName);
				pstTopic->topicNum++;
				return pstTopic;
#endif
				//printf("ERROR: Can't created a completely same topic, topicName: %s\n", pcTopicName);
                return pstTopic;
            }
        }
        pstTopic = pstTopic->pNext;
    }

    /* 申请内存创建主题 */
    pstTopic = (DDS_Topic*)DDS_STATIC_MALLOC(sizeof(DDS_Topic));
    if (NULL == pstTopic)
    {
        PrintLog(COMMLOG_ERROR, "DomainParticipant_create_topic: Failed to create topic, Insufficient memory space.\n");
        return NULL;
    }

    /* 初始化主题 */
    DDS_Topic_Init(pstTopic);

    /* 保存父节点指针  */
    pstTopic->pstParticipant = pstParticipant;

    if (NULL != pstTopicQos)
    {
        /* 插入主题qos */
        pstTopic->stTopicQos = *pstTopicQos;
    }
    else
    {
        /* 默认主题 */
        pstTopic->stTopicQos = DDS_TOPIC_QOS_DEFAULT;
    }

	/* 生成主题GUID */
	GenGuid(&pstTopic->tpGuid, &pstParticipant->stRTPSMsgHeader.guidPrefix, ENTITYKIND_TOPIC);
	
	/* 插入主题名称 */
	pstTopic->topicName.length = (UINT32)(strlen(pcTopicName) + 1);
#if defined(_WIN32)
	strcpy_s(pstTopic->topicName.value, pstTopic->topicName.length, pcTopicName);
#elif defined(__linux__)
    strncpy(pstTopic->topicName.value, pcTopicName, pstTopic->topicName.length);
#elif defined (_VXWORKS)
    memcpy(pstTopic->topicName.value,pcTopicName,pstTopic->topicName.length);
#endif
    /* 插入主题类型 */
    pstTopic->topicType.length = (UINT32)(strlen(pcTopicType) + 1);

#if defined(_WIN32)
    strcpy_s(pstTopic->topicType.value, pstTopic->topicType.length, pcTopicType);
#elif defined(__linux__)
    strncpy(pstTopic->topicType.value, pcTopicType, pstTopic->topicType.length);
#elif defined (_VXWORKS)
    memcpy(pstTopic->topicType.value, pcTopicType, pstTopic->topicType.length);
#endif
    /* 该主题插入到Participant下的主题链表 */
    LIST_INSERT_HEAD(pstParticipant->pstTopic, pstTopic);

    PrintLog(COMMLOG_INFO, "DomainParticipant_create_topic: create a topic successfully | topic name: %s.\n", pcTopicName);

    return pstTopic;
}

DDS_DLL DDS_Topic*  DDS_DomainParticipant_get_topic_by_name(DDS_DomainParticipant* pstParticipant, const CHAR* pcTopicName, const CHAR* pcTopicType)
{
	DDS_Topic* pstTopic = pstParticipant->pstTopic;
	while (NULL != pstTopic)
	{
		if (!strcmp(pstTopic->topicName.value, pcTopicName) && !strcmp(pstTopic->topicType.value, pcTopicType))
		{
#ifdef EVO_DDS
			pstTopic->topicNum++;
#endif
			return pstTopic;
		}
		pstTopic = pstTopic->pNext;
	}
	return NULL;
}

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_DomainParticipant_get_topic_num
-   功能描述: 获取主题个数
-   输    入: participant
-   返    回: 主题个数
-   全局变量:
-   注    释: 获取主题个数
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2020\07\16       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL UINT32  DDS_DomainParticipant_get_topic_num(DDS_DomainParticipant* pstParticipant)
{
    UINT32 uiTopicNum = 0;

	DDS_Topic* pstTopic = pstParticipant->pstTopic;
	while (NULL != pstTopic)
	{
		if (!Guid_Is_Equal(&pstTopic->tpGuid, &GUID_UNKNOWN))
		{
			uiTopicNum++;
		}

        pstTopic = pstTopic->pNext;
    }

    return uiTopicNum;
}

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_DomainParticipant_delete_topic
-   功能描述: 删除指定参与者下的主题
-   输    入: participant、主题
-   返    回: 质量码
-   全局变量:
-   注    释: 删除指定参与者下的主题
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2019\7\23       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t  DDS_DomainParticipant_delete_topic(DDS_DomainParticipant* pstParticipant, DDS_Topic* pstTopic)
{
	DDS_ReturnCode_t retCode;
	DDS_Topic* pstTempTopic = NULL;
    if (pstParticipant == NULL || pstTopic == NULL)
    {
        return DDS_RETCODE_ERROR;
    }
#ifdef EVO_DDS
    if (pstTopic->topicNum > 1)
    {
        pstTopic->topicNum--;
        return DDS_RETCODE_OK;
    }
#endif
    //printf("delete topic\n");
    
    

    if (pstParticipant && pstTopic)
    {
        pstTempTopic = pstParticipant->pstTopic;
        while (NULL != pstTempTopic)
        {
            if (pstTempTopic == pstTopic)
            {

                retCode = DDS_Topic_delete_datawriter(pstTempTopic);
                if (DDS_RETCODE_OK != retCode)
                {
                    return retCode;
                }

                retCode = DDS_Topic_delete_datareader(pstTempTopic);
                if (DDS_RETCODE_OK != retCode)
                {
                    return retCode;
                }

                pstTempTopic->tpGuid = GUID_UNKNOWN;
                break;
            }
            pstTempTopic = pstTempTopic->pNext;
        }
    }
    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_DomainParticipant_delete_topic_by_name
-   功能描述: 通过主题名称删除主题
-   输    入: participant、主题名
-   返    回: 质量码
-   全局变量:
-   注    释: 通过主题名称删除主题
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2019\7\23       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t  DDS_DomainParticipant_delete_topic_by_name(DDS_DomainParticipant* pstParticipant, const CHAR* pcTopicName, const CHAR* pcTopicType)
{
	DDS_ReturnCode_t retCode;
	DDS_Topic* pstTempTopic = NULL;

	if (pstParticipant && pcTopicName)
	{
		pstTempTopic = pstParticipant->pstTopic;
		while (NULL != pstTempTopic)
		{
			if (!strcmp(pstTempTopic->topicName.value, pcTopicName) && !strcmp(pstTempTopic->topicType.value, pcTopicType))
			{
#ifdef EVO_DDS
				if (pstTempTopic->topicNum > 1)
				{
					pstTempTopic->topicNum--;
					return DDS_RETCODE_OK;
				}
#endif
				retCode = DDS_Topic_delete_datawriter(pstTempTopic);
				if (DDS_RETCODE_OK != retCode)
				{
					return retCode;
				}

				retCode = DDS_Topic_delete_datareader(pstTempTopic);
				if (DDS_RETCODE_OK != retCode)
				{
					return retCode;
				}

				pstTempTopic->tpGuid = GUID_UNKNOWN;

				break;
			}
			pstTempTopic = pstTempTopic->pNext;
		}
	}

	return DDS_RETCODE_OK;
}
/*---------------------------------------------------------------------------------
-   函 数 名: DDS_DomainParticipant_get_default_topic_qos
-   功能描述: 获取主题默认qos
-   输    入: participant、主题qos
-   返    回: 质量码
-   全局变量:
-   注    释: 通过participant获取一个默认主题qos
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2019\7\23       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t DDS_DomainParticipant_get_default_topic_qos(DDS_DomainParticipant* pstParticipant, DDS_TopicQos* pstTopicQos)
{
    if (NULL == pstTopicQos)
    {
        PrintLog(COMMLOG_ERROR, "DomainParticipant_get_default_topic_qos: please pass in a non-null pointer of topic qos.\n");
        return DDS_RETCODE_ERROR;
    }

    *pstTopicQos = DDS_TOPIC_QOS_DEFAULT;
    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   函 数 名: DDS_DomainParticipant_get_default_topic_qos
-   功能描述: 获取主题默认qos
-   输    入: participant、主题qos
-   返    回: 质量码
-   全局变量:
-   注    释: 通过participant获取一个默认主题qos
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
=======================================================
-   2019\7\23       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_TopicQos* DDS_DomainParticipant_get_default_topic_qos_cs(DDS_DomainParticipant* pstParticipant)
{
    DDS_TopicQos* pstTopicQos = (DDS_TopicQos*)DDS_STATIC_MALLOC(sizeof(DDS_TopicQos));
    if (NULL == pstTopicQos)
    {
        PrintLog(COMMLOG_ERROR, "DomainParticipant_get_default_topic_qos: please pass in a non-null pointer of topic qos.\n");
        return NULL;
    }

    *pstTopicQos = DDS_TOPIC_QOS_DEFAULT;

    return pstTopicQos;
}

/*---------------------------------------------------------------------------------
-   函 数 名: GetBuiltinWriterFromParticipantByEntityId
-   功能描述: 查找内建写入器
-   输    入: Participant、EntityId
-   返    回:
-   全局变量:
-   注    释: 从Participant中通过EntityId查找相应内建写入器
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BuiltinDataWriter* GetBuiltinWriterFromParticipantByEntityId(DDS_DomainParticipant* pstDomainParticipant, EntityId_t* pstEntityId)
{
    UINT32 i = 0;

    for (i = 0; i < 3; i++)
    {
        if (EntityId_Is_Equal(&pstDomainParticipant->stBuiltinDataWriter[i].guid.entityId, pstEntityId))
        {
            return &pstDomainParticipant->stBuiltinDataWriter[i];
        }
    }

    return NULL;
}

/*---------------------------------------------------------------------------------
-   函 数 名: GetBuiltinReaderFromParticipantByEntityId
-   功能描述: 查找内建阅读器
-   输    入: Participant、EntityId
-   返    回:
-   全局变量:
-   注    释: 从Participant中通过EntityId查找相应内建阅读器
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BuiltinDataReader* GetBuiltinReaderFromParticipantByEntityId(DDS_DomainParticipant* pstDomainParticipant, EntityId_t* pstEntityId)
{
    UINT32 i = 0;
    BuiltinDiscWriter* pstBuiltinDisWriter = NULL;
    for (i = 0; i < 3; i++)
    {
        pstBuiltinDisWriter = pstDomainParticipant->stBuiltinDataReader[i].pstBuiltinDataWriter;
        while (NULL != pstBuiltinDisWriter)
        {
            if (EntityId_Is_Equal(&pstBuiltinDisWriter->guid.entityId, pstEntityId))
            {
                return &pstDomainParticipant->stBuiltinDataReader[i];
            }

            pstBuiltinDisWriter = pstBuiltinDisWriter->pNext;
        }
    }

    return NULL;
}

/*---------------------------------------------------------------------------------
-   函 数 名: GetDiscoveredWriterFromParticipantByEntityId
-   功能描述: 查找写入器
-   输    入: Participant、Guid
-   返    回:
-   全局变量:
-   注    释: 从Participant中通过Guid查找相应远端写入器
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\8       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DiscoveredWriter* GetDiscoveredWriterFromParticipantByGuid(DDS_DomainParticipant* pstDomainParticipant, GUID_t* pstGuid)
{
    DDS_Topic* pstTopic = NULL;
    DDS_DiscoveredWriter* pstDiscWriter = NULL;

    if (NULL != pstDomainParticipant)
    {
        /* 遍历每一个主题 */
        pstTopic = pstDomainParticipant->pstTopic;
        while (NULL != pstTopic)
        {
            /* 遍历主题下的每个远端写入器 */
            pstDiscWriter = GetDiscoveredWriterFromTopicByGuid(pstTopic, pstGuid);
            if (NULL != pstDiscWriter)
            {
                return pstDiscWriter;
            }

            pstTopic = pstTopic->pNext;
        }
    }

    return NULL;
}

/*---------------------------------------------------------------------------------
-   函 数 名: GetDiscoveredReaderFromParticipantByEntityId
-   功能描述: 查找远端器
-   输    入: Participant、Guid
-   返    回:
-   全局变量:
-   注    释: 从Participant通过Guid查找相应远端阅读器
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\8       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DiscoveredReader* GetDiscoveredReaderFromParticipantByGuid(DDS_DomainParticipant* pstDomainParticipant, GUID_t* pstGuid)
{
    DDS_Topic* pstTopic = NULL;
    DDS_DiscoveredReader* pstDiscReader = NULL;

    if (NULL != pstDomainParticipant)
    {
        /* 遍历每一个主题 */
        pstTopic = pstDomainParticipant->pstTopic;
        while (NULL != pstTopic)
        {
            /* 遍历主题下的每个远端阅读器 */
            pstDiscReader = GetDiscoveredReaderFromTopicByGuid(pstTopic, pstGuid);
            if (NULL != pstDiscReader)
            {
                return pstDiscReader;
            }

            pstTopic = pstTopic->pNext;
        }
    }

    return NULL;
}

/*---------------------------------------------------------------------------------
-   函 数 名: GetLocalWriterFromParticipantByEntityId
-   功能描述: 查找本地写入器
-   输    入: Participant、EntityId
-   返    回:
-   全局变量:
-   注    释: 从Participant通过EntityId查找相应写入器
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\8       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DataWriter* GetLocalWriterFromParticipantByEntityId(DDS_DomainParticipant* pstDomainParticipant, EntityId_t* pstEntityId)
{
    DDS_Topic* pstTopic = NULL;
    DDS_DataWriter* pstLocalWriter = NULL;

    if (NULL != pstDomainParticipant)
    {
        /* 遍历每一个主题 */
        pstTopic = pstDomainParticipant->pstTopic;
        while (NULL != pstTopic)
        {
            pstLocalWriter = pstTopic->pstLocalWriter;
            while (NULL != pstLocalWriter)
            {
                if (EntityId_Is_Equal(&pstLocalWriter->guid.entityId, pstEntityId))
                {
                    return pstLocalWriter;
                }

                pstLocalWriter = pstLocalWriter->pNext;
            }

            pstTopic = pstTopic->pNext;
        }
    }
    return NULL;
}

/*---------------------------------------------------------------------------------
-   函 数 名: GetLocalWriterFromParticipantByEntityId
-   功能描述: 查找阅读器
-   输    入: Participant、EntityId
-   返    回:
-   全局变量:
-   注    释: 从Participant中通过EntityId查找相应写入器
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\8       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DataReader* GetLocalReaderFromParticipantByEntityId(DDS_DomainParticipant* pstDomainParticipant, EntityId_t* pstEntityId)
{
    DDS_Topic* pstTopic = NULL;
    DDS_DataReader* pstLocalReader = NULL;

    if (NULL != pstDomainParticipant)
    {
        /* 遍历每一个主题 */
        pstTopic = pstDomainParticipant->pstTopic;
        while (NULL != pstTopic)
        {
            pstLocalReader = pstTopic->pstLocalReader;
            while (NULL != pstLocalReader)
            {
                /* 遍历主题下的每个本地阅读器 */
                if (EntityId_Is_Equal(&pstLocalReader->guid.entityId, pstEntityId))
                {
                    return pstLocalReader;
                }
                pstLocalReader = pstLocalReader->pNext;
            }

            pstTopic = pstTopic->pNext;
        }
    }

    return NULL;
}

/*---------------------------------------------------------------------------------
-   函 数 名: HandleAckNackMsg
-   功能描述: 处理Ack报文
-   输    入: ack消息、内存块、Participant
-   返    回:
-   全局变量:
-   注    释: 解析处理Ack报文
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\9       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL HandleBuiltinAckNackMsg(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader* pstSubMsgHeader, AckNack* pstAckNackMsg, DDS_DomainParticipant* pstParticipant)
{
	BuiltinDataWriter* pstBuiltinDataWriter = NULL;
	BuiltinDiscReader* pstBuiltinDiscReader = NULL;
	DDS_DiscoveredParticipant* pstDiscParticipant = NULL;
    /* 查找该EntityId所在的本地写入器 */
    pstBuiltinDataWriter = GetBuiltinWriterFromParticipantByEntityId(pstParticipant, &pstAckNackMsg->writerId);
    if (NULL == pstBuiltinDataWriter)
    {
        PrintLog(COMMLOG_ERROR, "HandleBuiltinAckNackMsg: Not find local data writer by entityId.\n");
        return FALSE;
    }

    /* 查找同一主题下远端阅读器  */
    pstBuiltinDiscReader = GetBuiltinDiscReaderFromBuiltinDataWriterByPrefix(pstBuiltinDataWriter, &pstRTPSMsgHeader->guidPrefix);
    if (NULL == pstBuiltinDiscReader)
    {
        PrintLog(COMMLOG_ERROR, "HandleBuiltinAckNackMsg: Not find remote data reader by entityId.\n");
        return FALSE;
    }

    /* 每收到一次ack报文，心跳积累数清空，不清空的话，每发送一次心跳积累一次，到一定数目会判定没有ack回复，自动断开该远端阅读器（删除） */
    pstBuiltinDiscReader->uiSendHeartBeatNum = 0;
    
//    if(0 == pstAckNackMsg->readerSNState.numBits)
//    {
//    	Time_t nowTime = GetNowTime();
//    	printf("%d.%d Nephalem Ack base = %d num = %d \n", nowTime.sec,nowTime.nanosec,pstAckNackMsg->readerSNState.base.low,pstAckNackMsg->readerSNState.numBits);
//    }
//    printf("Nephalem Ack base = %d num = %d \n ",pstAckNackMsg->readerSNState.base.low,pstAckNackMsg->readerSNState.numBits);
    
#ifdef QYDDS_LIVELINESS_QOS
	//线程不安全
	DDS_MUTEX_LOCK(pstParticipant->m_discRdMutex);
	pstDiscParticipant = pstParticipant->pstDiscParticipant;
	while (pstDiscParticipant != NULL)
	{
		if (GuidPrefix_Is_Equal(&pstDiscParticipant->stRTPSMsgHeader.guidPrefix, &pstRTPSMsgHeader->guidPrefix))
		{
			DDS_MUTEX_LOCK(pstDiscParticipant->threadMutex);
			pstDiscParticipant->livelinessHeartMissNum = 0;
			pstDiscParticipant->lastHeartBeat = GetNowTime();
			DDS_MUTEX_UNLOCK(pstDiscParticipant->threadMutex);
			break;
		}
		pstDiscParticipant = pstDiscParticipant->pNext;
	}
	DDS_MUTEX_UNLOCK(pstParticipant->m_discRdMutex);
#endif
	
    /* 过期ack报文，直接跳过 */
    //if (pstAckNackMsg->count <= pstBuiltinDiscReader->stAckNack.count)
    //{
    //    return TRUE;
    //}
    
    /* 更新ack报文序号 */
    pstBuiltinDiscReader->stAckNack = *pstAckNackMsg;

    /* 丢失分片重传 */
    RecoverBuiltinWriterMissSeqNum(pstBuiltinDataWriter, pstBuiltinDiscReader, pstAckNackMsg);

    /* 心跳设置，ack为最后一次对此不再发心跳 */
    if(pstBuiltinDiscReader->stAckNack.readerSNState.numBits != 0)
    	JointHeartbeatMsgSendAssignBuiltinReader(&pstBuiltinDataWriter->pstParticipant->stRTPSMsgHeader,pstBuiltinDataWriter, pstBuiltinDiscReader);
//    if(!pstSubMsgHeader->flags[1])
//    {
//        JointHeartbeatMsgSendAssignBuiltinReader(&pstBuiltinDataWriter->pstParticipant->stRTPSMsgHeader,pstBuiltinDataWriter, pstBuiltinDiscReader);
//    }

    return TRUE;
}

/*---------------------------------------------------------------------------------
-   函 数 名: HandleAckNackMsg
-   功能描述: 处理Ack报文
-   输    入: ack消息、内存块、Participant
-   返    回:
-   全局变量:
-   注    释: 解析处理Ack报文
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\9       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL HandleAckNackMsg(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader* pstSubMsgHeader, AckNack* pstAckNackMsg, DDS_DomainParticipant* pstParticipant)
{
    GUID_t stDiscRdGuid;
	DDS_DataWriter* pstDataWriter = NULL;
	DDS_DiscoveredReader* pstDiscReader = NULL;
	DDS_LocalWriter* pstLocalWriter = NULL;

    /* 获取远端写入器guid */
    stDiscRdGuid.prefix = pstRTPSMsgHeader->guidPrefix;
    stDiscRdGuid.entityId = pstAckNackMsg->readerId;

    /* 查找该EntityId所在的本地写入器 */
    pstDataWriter = GetLocalWriterFromParticipantByEntityId(pstParticipant, &pstAckNackMsg->writerId);
    if (NULL == pstDataWriter)
    {
        PrintLog(COMMLOG_WARN, "HandleAckNackMsg: Not find local data writer by entityId.\n");
        return FALSE;
    }

    /* 本地写入器为无状态，则直接返回，不处理该ack报文 */
    if (BEST_EFFORT_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
    {
        PrintLog(COMMLOG_ERROR, "HandleAckNackMsg: Local reader is stateless, ignore heartbeat.\n");
        return FALSE;
    }

    /* 查找同一主题下远端阅读器*/
    pstDiscReader = GetDiscoveredReaderFromTopicByGuid(pstDataWriter->pstTopic, &stDiscRdGuid);
    if (NULL == pstDiscReader)
    {
        PrintLog(COMMLOG_WARN, "HandleAckNackMsg: Not find remote data reader by entityId.\n");
        return FALSE;
    }

    pstLocalWriter = GetDiscoveredReaderLocalWriterByGuid(pstDiscReader, &pstDataWriter->guid);
    if (NULL == pstLocalWriter)
    {
        PrintLog(COMMLOG_WARN, "HandleAckNackMsg: Not find local data writer by entityId.\n");
        return FALSE;
    }

    /*  远端阅读器为无状态，则直接返回，不处理该ack报文 */
    if (BEST_EFFORT_RELIABILITY_QOS == pstDiscReader->stDataReaderQos.reliability.kind)
    {
        PrintLog(COMMLOG_WARN, "HandleAckNackMsg: Remote writer is stateless, ignore heartbeat.\n");
        return FALSE;
    }

    /* 每收到一次ack报文，心跳积累数清空，不清空的话，每发送一次心跳积累一次，到一定数目会判定没有ack回复，自动断开该远端阅读器（删除） */
    pstLocalWriter->uiSendHeartBeatNum = 0;

    /* 过期ack报文，直接跳过 */
    //if (pstAckNackMsg->count <= pstLocalWriter->stAckNack.count)
    //{
    //    return TRUE;
    //}

    /* 更新ack报文序号 */
    pstLocalWriter->stAckNack.count = pstAckNackMsg->count;

    /* 更新每个远端阅读器期望的下个seqNum，以便本地缓存清除 */
    pstLocalWriter->stAckNack.readerSNState.base = pstAckNackMsg->readerSNState.base;

	/* 过期缓存清除 */
	if (pstDataWriter->stDataWriterQos.durability.kind != TRANSIENT_DURABILITY_QOS)
	{
		DelDataWriterHistoryCache(pstDataWriter);
	}

    /* 丢失分片重传 */
    RecoverWriterMissSeqNum(pstDataWriter, pstDiscReader, pstAckNackMsg);

    /* ack报文有数据缺失，重传后立马补发心跳，以便数据快速重传 */
    if (!pstSubMsgHeader->flags[1])
    {
        JointHeartbeatMsgSendAssignReader(pstDataWriter, pstDiscReader);
    }

    return TRUE;
}

/*---------------------------------------------------------------------------------
-   函 数 名: HandleNackFragMsg
-   功能描述: 处理Ack报文
-   输    入: NackFrag消息、内存块、Participant
-   返    回:
-   全局变量:
-   注    释: 解析处理NackFrag报文
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2021\4\14       1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
BOOL HandleNackFragMsg(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader* pstSubMsgHeader, NackFrag* pstNackFragMsg, DDS_DomainParticipant* pstParticipant)
{
    GUID_t stDiscRdGuid;
    DDS_DataWriter* pstDataWriter = NULL;
    DDS_DiscoveredReader* pstDiscReader = NULL;
    DDS_LocalWriter* pstLocalWriter = NULL;

    /* 获取远端写入器guid */
    stDiscRdGuid.prefix = pstRTPSMsgHeader->guidPrefix;
    stDiscRdGuid.entityId = pstNackFragMsg->readerId;

    /* 查找该EntityId所在的本地写入器 */
    pstDataWriter = GetLocalWriterFromParticipantByEntityId(pstParticipant, &pstNackFragMsg->writerId);
    if (NULL == pstDataWriter)
    {
        PrintLog(COMMLOG_WARN, "HandleAckNackMsg: Not find local data writer by entityId.\n");
        return FALSE;
    }

    /* 本地写入器为无状态，则直接返回，不处理该ack报文 */
    if (BEST_EFFORT_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
    {
        PrintLog(COMMLOG_ERROR, "HandleAckNackMsg: Local reader is stateless, ignore heartbeat.\n");
        return FALSE;
    }

    /* 查找同一主题下远端阅读器*/
    pstDiscReader = GetDiscoveredReaderFromTopicByGuid(pstDataWriter->pstTopic, &stDiscRdGuid);
    if (NULL == pstDiscReader)
    {
        PrintLog(COMMLOG_WARN, "HandleAckNackMsg: Not find remote data reader by entityId.\n");
        return FALSE;
    }

    pstLocalWriter = GetDiscoveredReaderLocalWriterByGuid(pstDiscReader, &pstDataWriter->guid);
    if (NULL == pstLocalWriter)
    {
        PrintLog(COMMLOG_WARN, "HandleAckNackMsg: Not find local data writer by entityId.\n");
        return FALSE;
    }

    /*  远端阅读器为无状态，则直接返回，不处理该ack报文 */
    if (BEST_EFFORT_RELIABILITY_QOS == pstDiscReader->stDataReaderQos.reliability.kind)
    {
        PrintLog(COMMLOG_WARN, "HandleAckNackMsg: Remote writer is stateless, ignore heartbeat.\n");
        return FALSE;
    }

    /* Nephalem 重传DataFrag数据  */
    RecoverWriterMissDataFrag(pstDataWriter, pstDiscReader, pstNackFragMsg);

    return TRUE;
}

/*--------------------------------------------------------------------------------
-   函 数 名: HandleBuiltinHeartbeatMsg
-   功能描述: 内建心跳处理函数
-   输    入: 子报文消息头、心跳数据、Participant
-   返    回: 布尔
-   全局变量:
-   注    释: 内建心跳处理.是否需要数据重传
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\14       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL HandleBuiltinHeartbeatMsg(RTPSMessageHeader* pstRTPSMsgHeader, Heartbeat* pstHeartbeatMsg, DDS_DomainParticipant* pstParticipant)
{
	BuiltinDataReader* pstBuiltinDataReader = NULL;
	BuiltinDiscWriter* pstBuiltinDiscWriter = NULL;
	DDS_DiscoveredParticipant* pstDiscParticipant = NULL;

	if(iDDSRECV == 1)
	{
		if (EntityId_Is_Equal(&pstHeartbeatMsg->writerId, &ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER))
			printf("Recv PUBLICATIONS : ");
		if (EntityId_Is_Equal(&pstHeartbeatMsg->writerId, &ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_WRITER))
			printf("Recv SUBSCRIPTIONS : ");
	    printf("Nephalem  Heartbeat %d  %d-%d\n",pstHeartbeatMsg->count,pstHeartbeatMsg->firstSN.low, pstHeartbeatMsg->lastSN.low);
	}
	
#if defined _DEBUG_LOG
	if (EntityId_Is_Equal(&pstHeartbeatMsg->writerId, &ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER))
		printf("Recv PUBLICATIONS : ");
	if (EntityId_Is_Equal(&pstHeartbeatMsg->writerId, &ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_WRITER))
		printf("Recv SUBSCRIPTIONS : ");
    printf("Nephalem  Heartbeat %d  %d-%d\n",pstHeartbeatMsg->count,pstHeartbeatMsg->firstSN.low, pstHeartbeatMsg->lastSN.low);
#endif
    
    /* 无效心跳，不予处理 */
    if (ISLESSTHEN_SEQUENCENUMBER(pstHeartbeatMsg->lastSN, pstHeartbeatMsg->firstSN))
    {
        return TRUE;
    }
    
    /* 查找本地内建阅读器 */
    pstBuiltinDataReader = GetBuiltinReaderFromParticipantByEntityId(pstParticipant, &pstHeartbeatMsg->writerId);
    if (NULL == pstBuiltinDataReader)
    {
        PrintLog(COMMLOG_ERROR, "HandleBuiltinHeartbeatMsg: Cannot discovered local dataWriter.\n");
        return FALSE;
    }
    /* 查找本地内建阅读器对应的远端内建写入器 */
    pstBuiltinDiscWriter = GetBuiltinDiscWriterFromBuiltinDataReaderByPrefix(pstBuiltinDataReader, &pstRTPSMsgHeader->guidPrefix);
    if (NULL == pstBuiltinDiscWriter)
    {
        PrintLog(COMMLOG_ERROR, "HandleBuiltinHeartbeatMsg: Cannot discovered remote dataWriter.\n");
        return FALSE;
    }

//    printf("pstHeartbeatMsg->count %d  pstBuiltinDiscWriter->stHeartbeat.count %d \n",pstHeartbeatMsg->count,pstBuiltinDiscWriter->stHeartbeat.count);
    /* 心跳过期，不予处理 */
    if (pstHeartbeatMsg->count <= pstBuiltinDiscWriter->stHeartbeat.count)
    {
        return TRUE;
    }
    else
    {
        /* 心跳计数同步 */
        pstBuiltinDiscWriter->stHeartbeat.count = pstHeartbeatMsg->count;
    }

    /* 查找缺失内建数据包 */
    GetBuiltinReaderMissSeqNum(pstBuiltinDataReader, pstBuiltinDiscWriter, pstHeartbeatMsg);

    /* 序列化ack报文发送 */
    JointAckNackMsgSendAssignBuiltinWriter(pstBuiltinDataReader, pstBuiltinDiscWriter);

#ifdef QYDDS_LIVELINESS_QOS
	//Liveliness计数清零
	pstDiscParticipant = pstParticipant->pstDiscParticipant;
	while (pstDiscParticipant != NULL)
	{
		if (GuidPrefix_Is_Equal(&pstDiscParticipant->stRTPSMsgHeader.guidPrefix, &pstRTPSMsgHeader->guidPrefix))
		{
			DDS_MUTEX_LOCK(pstDiscParticipant->threadMutex);
			pstDiscParticipant->livelinessHeartMissNum = 0;
			pstDiscParticipant->lastHeartBeat = GetNowTime();
			DDS_MUTEX_UNLOCK(pstDiscParticipant->threadMutex);
			break;
		}
		pstDiscParticipant = pstDiscParticipant->pNext;
	}
#endif
    return TRUE;
}

/*--------------------------------------------------------------------------------
-   函 数 名: HandleHeartbeatMsg
-   功能描述: 心跳处理函数
-   输    入: 子报文消息头、心跳数据、Participant
-   返    回: 布尔
-   全局变量:
-   注    释: 心跳处理.是否需要数据重传
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\14       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL HandleHeartbeatMsg(RTPSMessageHeader* pstRTPSMsgHeader, Heartbeat* pstHeartbeatMsg, DDS_DomainParticipant* pstParticipant)
{
    GUID_t stDiscWtGuid;
    DDS_DataReader* pstDataReader = NULL;
	DDS_DiscoveredWriter* pstDiscWriter = NULL;
    /* 无效心跳，不予处理 */
    if (ISLESSTHEN_SEQUENCENUMBER(pstHeartbeatMsg->lastSN, pstHeartbeatMsg->firstSN))
    {
        return TRUE;
    }

    /* 获取远端写入器guid */
    stDiscWtGuid.prefix = pstRTPSMsgHeader->guidPrefix;
    stDiscWtGuid.entityId = pstHeartbeatMsg->writerId;

    /* 查找该用户数据对应的远端写入器 */
    pstDiscWriter = GetDiscoveredWriterFromParticipantByGuid(pstParticipant, &stDiscWtGuid);
    if (NULL == pstDiscWriter)
    {
        PrintLog(COMMLOG_ERROR, "HandleHeartbeatMsg: Cannot discovered remote dataWriter.\n");
        return FALSE;
    }

    /* 心跳过期，不予处理 */
    if (pstHeartbeatMsg->count <= pstDiscWriter->stHeartbeat.count)
    {
        return TRUE;
    }
    else
    {
        pstDiscWriter->stHeartbeat.count = pstHeartbeatMsg->count;
    }

    /* 没有指定阅读器Id，即关联为同一主题下的阅读器 */
    if (EntityId_Is_Equal(&ENTITYID_UNKNOWN, &pstHeartbeatMsg->readerId))
    {
        pstDataReader = pstDiscWriter->pstTopic->pstLocalReader;
        if (NULL == pstDataReader)
        {
            PrintLog(COMMLOG_ERROR, "HandleHeartbeatMsg: Cannot discovered local dataReader.\n");
            return FALSE;
        }
    }
    else
    {
        pstDataReader = pstDiscWriter->pstTopic->pstLocalReader;
        while (NULL != pstDataReader)
        {
            /* 指定了阅读器id，需要校验是否一致 */
            if (EntityId_Is_Equal(&pstDataReader->guid.entityId, &pstHeartbeatMsg->readerId))
            {
                break;
            }
            pstDataReader = pstDataReader->pNext;
        }

        if (NULL == pstDataReader)
        {
            PrintLog(COMMLOG_ERROR, "HandleHeartbeatMsg: Cannot discovered local dataReader.\n");
            return FALSE;
        }
    }

    /* 本地阅读器为无状态，则直接返回，不处理该心跳报文 */
    if (BEST_EFFORT_RELIABILITY_QOS == pstDataReader->stDataReaderQos.reliability.kind)
    {
        PrintLog(COMMLOG_ERROR, "HandleHeartbeatMsg: Local reader is stateless, ignore heartbeat.\n");
        return FALSE;
    }

    /*  远端写入器为无状态，则直接返回，不处理该心跳报文 */
    if (BEST_EFFORT_RELIABILITY_QOS == pstDiscWriter->stDataWriterQos.reliability.kind)
    {
        PrintLog(COMMLOG_ERROR, "HandleHeartbeatMsg: Remote writer is stateless, ignore heartbeat.\n");
        return FALSE;
    }

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

    /* 期望值比最小的seqNum还小，可能是因为网络中断, 发送方缓存进行了清理，期望值调整为最小seqNum，损失降到最低 */
    if (ISLESSTHEN_SEQUENCENUMBER(pstDiscWriter->stAckNack.readerSNState.base, pstHeartbeatMsg->firstSN))
    {
        DeilverStatusFulUserMsgWhenExpectSeqNumUnderFirstSN(pstDataReader, pstDiscWriter, pstHeartbeatMsg->firstSN);
        PrintLog(COMMLOG_WARN, "HandleHeartbeatMsg: Beause of network interruption, the data maybe missed.\n");
    }

    /* 数据缺失，提交缺失编号，对端获取ack报文后立马重传，ack报文丢了也没关系，等待下次心跳再校验 */
    GetDataReaderMissSeqNum(pstDataReader, pstDiscWriter, pstHeartbeatMsg->lastSN);

//    if(0 == pstDiscWriter->stAckNack.readerSNState.numBits)
//    {
//    	Time_t nowTime = GetNowTime();
//    	printf("%d.%d Nephalem : Ack bitmap %d  nums %d ", nowTime.sec,nowTime.nanosec,pstDiscWriter->stAckNack.readerSNState.base.low,pstDiscWriter->stAckNack.readerSNState.numBits);
//    }

    /* 序列化ack报文发送 */
    JointAckNackMsgSendAssignWriter(pstDataReader, pstDiscWriter);
     
    pstDiscWriter->heartMsgNum = 0;
    pstDiscWriter->lastHeartTime = GetNowTime();
    return FALSE;
}

/*--------------------------------------------------------------------------------
-   函 数 名: HandleHeartbeatFragMsg
-   功能描述: 分片心跳处理函数
-   输    入: 子报文消息头、分片心跳数据、Participant
-   返    回: 布尔
-   全局变量:
-   注    释: 心跳处理.是否需要数据重传
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2021\4\13       1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
BOOL HandleHeartbeatFragMsg(RTPSMessageHeader* pstRTPSMsgHeader, HeartbeatFrag* pstHeartbeatFragMsg, DDS_DomainParticipant* pstParticipant)
{
    GUID_t stDiscWtGuid;
    DDS_DataReader* pstDataReader = NULL;
    DDS_DiscoveredWriter* pstDiscWriter = NULL;

    /* 无效心跳，不予处理 */
    if (pstHeartbeatFragMsg->lastFragmentNum <= 0)
    {
        return TRUE;
    }

    /* 获取远端写入器guid */
    stDiscWtGuid.prefix = pstRTPSMsgHeader->guidPrefix;
    stDiscWtGuid.entityId = pstHeartbeatFragMsg->writerId;

    /* 查找该用户数据对应的远端写入器 */
    pstDiscWriter = GetDiscoveredWriterFromParticipantByGuid(pstParticipant, &stDiscWtGuid);
    if (NULL == pstDiscWriter)
    {
        PrintLog(COMMLOG_ERROR, "HandleHeartbeatMsg: Cannot discovered remote dataWriter.\n");
        return FALSE;
    }

    /* 心跳过期，不予处理 */
    if (pstHeartbeatFragMsg->count <= pstDiscWriter->stHeartbeat.count)
    {
        return TRUE;
    }
    else
    {
        pstDiscWriter->stHeartbeat.count = pstHeartbeatFragMsg->count;
    }

    /* 没有指定阅读器Id，即关联为同一主题下的阅读器 */
    if (EntityId_Is_Equal(&ENTITYID_UNKNOWN, &pstHeartbeatFragMsg->readerId))
    {
        pstDataReader = pstDiscWriter->pstTopic->pstLocalReader;
        if (NULL == pstDataReader)
        {
            PrintLog(COMMLOG_ERROR, "HandleHeartbeatMsg: Cannot discovered local dataReader.\n");
            return FALSE;
        }
    }
    else
    {
        pstDataReader = pstDiscWriter->pstTopic->pstLocalReader;
        while (NULL != pstDataReader)
        {
            /* 指定了阅读器id，需要校验是否一致 */
            if (EntityId_Is_Equal(&pstDataReader->guid.entityId, &pstHeartbeatFragMsg->readerId))
            {
                break;
            }
            pstDataReader = pstDataReader->pNext;
        }

        if (NULL == pstDataReader)
        {
            PrintLog(COMMLOG_ERROR, "HandleHeartbeatMsg: Cannot discovered local dataReader.\n");
            return FALSE;
        }
    }

    /* 本地阅读器为无状态，则直接返回，不处理该心跳报文 */
    if (BEST_EFFORT_RELIABILITY_QOS == pstDataReader->stDataReaderQos.reliability.kind)
    {
        PrintLog(COMMLOG_ERROR, "HandleHeartbeatMsg: Local reader is stateless, ignore heartbeat.\n");
        return FALSE;
    }

    /*  远端写入器为无状态，则直接返回，不处理该心跳报文 */
    if (BEST_EFFORT_RELIABILITY_QOS == pstDiscWriter->stDataWriterQos.reliability.kind)
    {
        PrintLog(COMMLOG_ERROR, "HandleHeartbeatMsg: Remote writer is stateless, ignore heartbeat.\n");
        return FALSE;
    }

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

    /* 序列化ack报文发送 */
    JointNackFragMsgSendAssignWriter(pstHeartbeatFragMsg->writerSN,pstDataReader, pstDiscWriter);

    //pstDiscWriter->heartMsgNum = 0;
    //pstDiscWriter->lastHeartTime = GetNowTime();
    return FALSE;
}

/*---------------------------------------------------------------------------------
-   函 数 名: SendHeartbeatTasksOrder
-   功能描述: 定时心跳任务
-   输    入: pstParticipant
-   返    回: 布尔
-   全局变量:
-   注    释: 发送定时心跳任务
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\14       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID SendHeartbeatTasksOrder(DDS_DomainParticipant* pstParticipant)
{
    UINT32 uiNum;
	ULONG ulHeartbeat;
    DDS_DataWriter*  pstLocalWriter = NULL;
	DDS_Topic* pstTopic = NULL;

    /* 更新时间 */
    HPTB_END(pstParticipant->heartBeatMark);

    /* 计算心跳间隔(us)*/
    ulHeartbeat = HPTB_INTERVAL_MICROSEC(pstParticipant->heartBeatMark);
    
    if (ulHeartbeat > HEART_TIME * 1000 * 1)
	{
		/* 内建实体心跳发送 */
		int init = 1;
#ifdef QYDDS_LIVELINESS_QOS
//#ifndef _FC
//		init = 0;
//#endif
//#ifndef _653_PORT
//		init = 0;
//#endif
#endif
        for (uiNum = init; uiNum < 3; uiNum++)
		{
        	//printf("Nephalem Send HeartBeat %d \n",uiNum);
			JointHeartbeatMsgSendAllBuiltinReader(&pstParticipant->stRTPSMsgHeader, &pstParticipant->stBuiltinDataWriter[uiNum]);
		}

		/* 用户数据心跳发送 */
		pstTopic = pstParticipant->pstTopic;
		while (NULL != pstTopic)
		{
			pstLocalWriter = pstTopic->pstLocalWriter;
			while (NULL != pstLocalWriter)
			{
				if (RELIABLE_RELIABILITY_QOS == pstLocalWriter->stDataWriterQos.reliability.kind)
				{
					JointHeartbeatMsgSendUnknownReader(pstLocalWriter);
				}

				pstLocalWriter = pstLocalWriter->pNext;
			}

			pstTopic = pstTopic->pNext;
		}

		HPTB_BEGIN(pstParticipant->heartBeatMark);
	}
}

/*---------------------------------------------------------------------------------
-   函 数 名: NetworkTransceiverTask
-   功能描述: 网络收发任务
-   输    入: pstParticipant
-   返    回: 布尔
-   全局变量:
-   注    释: 网络收发任务
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\14       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL VOID NetworkTransceiverTask(DDS_DomainParticipant* pstParticipant)
{
#if defined(_VXWORKS_653) || defined (_TM1) || defined _TM3
    /* 653任务无法传参，使用全局变量 */
	pstParticipant = pstRealDomainParticipant;
#endif
	/* Nephalem 接收缓冲区 */
	CHAR buffer[NETWORK_BYTE_MAX_SIZE];
	INT length = 0;
	ULONG microSecInterval;
	MemoryBlock stMemBlock;
	/* Nephalem 发送缓冲区 */
    CHAR buff[NETWORK_BYTE_MAX_SIZE];
	//TODO：修改卡顿问题
	INT i = 0;
	INT recvlen = 0;
	INT ret = 0;
	
	INT Count = 0;
    while (1)
    {
        recvlen = 0;
        DDS_MUTEX_LOCK(pstParticipant->threadMutex);
        
        if (!pstParticipant->threadSignal)
        {
            DDS_MUTEX_UNLOCK(pstParticipant->threadMutex);
            break;
        }

        if (g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].active)
        {
            /* SPDP 发送任务 */
            HPTB_END(g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].timeBlock);
            microSecInterval = HPTB_INTERVAL_MICROSEC(g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].timeBlock);

//            printf("begin time = %d.%d  end time = %d.%d \n",
//            		g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].timeBlock.nBeginTime.tv_sec,
//					g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].timeBlock.nBeginTime.tv_usec,
//					g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].timeBlock.nEndTime.tv_sec,
//					g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].timeBlock.nEndTime.tv_usec);
            //printf("microSecInterval = %d, %d \n",microSecInterval,g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].miliSecPeriod * 1000);
            if (microSecInterval > g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].miliSecPeriod * 1000)
            {
            	//printf("microSecInterval = %d, %d ",microSecInterval,g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].miliSecPeriod * 1000);
            
            	SendHeartbeatTasksOrder(pstParticipant);
                /* 网络字节序初始化，保存发布报文 */
                INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);
                //printf("Nephalem Send SPDP\n");
                JointSPDPDataMsg(&pstParticipant->stRTPSMsgHeader, pstParticipant->stBuiltinDataWriter[0].stHistoryCache.pstHead, &pstParticipant->stBuiltinDataWriter[0], &stMemBlock);
                SendMulticastMsg(&stMemBlock, &pstParticipant->socketList.spdp_data_sender, pstParticipant->rtpsParticipantProxy.metatrafficMulticastLocator);
                               
                //重新开始计时
                HPTB_BEGIN(g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].timeBlock);
            }
        }
      
#if defined _FC
        
        RecvAndParseFAllNetworkOrder(0,buffer,pstParticipant);
        
        RecvAndParseFAllNetworkOrder(1,buffer,pstParticipant);
        
        RecvAndParseFAllNetworkOrder(2,buffer,pstParticipant);
        
       QY_Sleep(1);
        
#elif defined _653_PORT
        RETURN_CODE_TYPE retCode = NO_ACTION;
        for( i = 0 ; i < g_PortConfig.recvPortNum ; i++)
        {
			/* 接收消息 */
			RECEIVE_QUEUING_MESSAGE(g_PortConfig.recvPort[i].portID, (APEX_LONG_INTEGER)0, (MESSAGE_ADDR_TYPE)buffer, (MESSAGE_SIZE_TYPE  *)&length, &retCode);
			/* 成功收到消息 */
			if (retCode == 0)
			{
				ParseAllNetworkOrder(buffer, length, pstParticipant);
				recvlen = length;
				printf("Nephalem FC Recv Data From %X Ret = %d , len = %d \n",g_PortConfig.recvPort[i].portSeq,retCode,length);
			}
		}
#else
        /* SPDP 接收任务 */
        if (g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_RECV].active)
        {
            length = RTPSReceiveMsg(&pstParticipant->socketList.spdp_data_receiver, buffer);
            if (length > 0)
            {
                //printf("Nephalem  SPDP RTPSReceiveMsg %d  \n",length);
                ParseAllNetworkOrder(buffer, length, pstParticipant);
                recvlen = length;
            }
        }

        /* SEDP 接收任务 */
        if (g_sub_task_ctrl_blocks[SUB_TASK_SEDP_DATA_RECV].active)
        {
            length = RTPSReceiveMsg(&pstParticipant->socketList.sedp_data_receiver, buffer);    
			if (length > 0)
			{
	            //printf("Nephalem SEDP  RTPSReceiveMsg %d  \n",length);    
                ParseAllNetworkOrder(buffer, length, pstParticipant);
                recvlen = length;
            }
		}

        /* USER 接收任务 */
        if (g_sub_task_ctrl_blocks[SUB_TASK_USER_DATA_RECV].active)
        {
            length = RTPSReceiveMsg(&pstParticipant->socketList.user_data_receiver, buffer);        
			if (length > 0)
			{
	            //printf("Nephalem USER RTPSReceiveMsg %d  \n",length);
                ParseAllNetworkOrder(buffer, length, pstParticipant);
                recvlen = length;
            }
		}
                
#if !defined(NO_HEARTBEAT)
        /* 心跳任务 */
        if (g_sub_task_ctrl_blocks[SUB_TASK_HEARTBEAT].active)
        {
        	SendHeartbeatTasksOrder(pstParticipant);
        }
#endif
        
		if (recvlen <= 0)
		{
			QY_Sleep(1);
		}
#endif
		DDS_MUTEX_UNLOCK(pstParticipant->threadMutex);
	}

    return;
}

//DDS_DLL VOID NetworkTransceiverTask(DDS_DomainParticipant* pstParticipant)
//{
//#if defined(_VXWORKS_653) || defined (_TM1) || defined _TM3
//    /* 653任务无法传参，使用全局变量 */
//	pstParticipant = pstRealDomainParticipant;
//#endif
//	/* Nephalem 接收缓冲区 */
//	CHAR buffer[NETWORK_BYTE_MAX_SIZE];
//	INT length = 0;
//	ULONG microSecInterval;
//	MemoryBlock stMemBlock;
//	/* Nephalem 发送缓冲区 */
//    CHAR buff[NETWORK_BYTE_MAX_SIZE];
//	//TODO：修改卡顿问题
//	INT i = 0;
//	INT recvlen = 0;
//	INT ret = 0;
//	
//	INT Count = 0;
//    while (1)
//    {
//        recvlen = 0;
//        DDS_MUTEX_LOCK(pstParticipant->threadMutex);
//        
//        if (!pstParticipant->threadSignal)
//        {
//            DDS_MUTEX_UNLOCK(pstParticipant->threadMutex);
//            break;
//        }
//
//        if (g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].active)
//        {
//            /* SPDP 发送任务 */
//            HPTB_END(g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].timeBlock);
//            microSecInterval = HPTB_INTERVAL_MICROSEC(g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].timeBlock);
//
////            printf("begin time = %d.%d  end time = %d.%d \n",
////            		g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].timeBlock.nBeginTime.tv_sec,
////					g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].timeBlock.nBeginTime.tv_usec,
////					g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].timeBlock.nEndTime.tv_sec,
////					g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].timeBlock.nEndTime.tv_usec);
//            printf("microSecInterval = %d, %d",microSecInterval,g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].miliSecPeriod * 1000);
//            if (microSecInterval > g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].miliSecPeriod * 1000)
//            {
//            	//printf("microSecInterval = %d, %d ",microSecInterval,g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].miliSecPeriod * 1000);
//            
//            	SendHeartbeatTasksOrder(pstParticipant);
//                /* 网络字节序初始化，保存发布报文 */
//                INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);
//                //printf("Nephalem Send SPDP\n");
//                JointSPDPDataMsg(&pstParticipant->stRTPSMsgHeader, pstParticipant->stBuiltinDataWriter[0].stHistoryCache.pstHead, &pstParticipant->stBuiltinDataWriter[0], &stMemBlock);
//                SendMulticastMsg(&stMemBlock, &pstParticipant->socketList.spdp_data_sender, pstParticipant->rtpsParticipantProxy.metatrafficMulticastLocator);
//                               
//                //重新开始计时
//                HPTB_BEGIN(g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].timeBlock);
//            }
//        }
//      
//#if defined _FC
//        
//        /* Nephalem 遍历DDS数据接收MsgID */ 
//        for(i = 0 ; i < g_commFCConfig.m_nodeNum;i++)
//        {    	
//        	length = NETWORK_BYTE_MAX_SIZE;
//            ret = FC_NP_RecvMsg(g_commFCConfig.m_FCdev, g_commFCConfig.m_configs[i].m_localDDSRecvMsgID, buffer, &length);
//        	ret = -1;
//            if (0 == ret)
//            {
//#if defined _DEBUG_LOG 
//                printf("Nephalem FC Recv Data From %X Ret = %x , len = %d \n",g_commFCConfig.m_configs[i].m_localDDSRecvMsgID,ret,length);
//#endif  
//                ParseAllNetworkOrder(buffer, length, pstParticipant);
//                recvlen = length;           
//            }
// 
//        }
//        
//		/* Nephalem 遍历SEDP数据接收MsgID */ 
//		for(i = 0 ; i < g_commFCConfig.m_nodeNum;i++)
//		{			
//			length = NETWORK_BYTE_MAX_SIZE;
//			ret = 0;
//			while(0 == ret)
//			{
//				//ret = FC_NP_RecvMsg(g_commFCConfig.m_FCdev, g_commFCConfig.m_configs[i].m_localSEPDRecvMsgID, buffer, &length);
//				ret = -1;
//				if (0 == ret)
//				{
//#if defined _DEBUG_LOG 
//					printf("E:Nephalem FC Recv Data From %X Ret = %d , len = %d \n",g_commFCConfig.m_configs[i].m_localUserRecvMsgID,ret,length);
//#endif  
//					ParseAllNetworkOrder(buffer, length, pstParticipant);
//					recvlen = length;           
//				}
//			}
//		}
//		
//		/* Nephalem 遍历用户数据接收MsgID */ 
//		for(i = 0 ; i < g_commFCConfig.m_nodeNum;i++)
//		{			
//			length = NETWORK_BYTE_MAX_SIZE;
//			ret = 0;
//			while(0 == ret)
//			{
//				//ret = FC_NP_RecvMsg(g_commFCConfig.m_FCdev, g_commFCConfig.m_configs[i].m_localUserRecvMsgID, buffer, &length);
//				ret = -1;
//				if (0 == ret)
//				{
//#if defined _DEBUG_LOG 
//					printf("U:Nephalem FC Recv Data From %X Ret = %x , len = %d \n",g_commFCConfig.m_configs[i].m_localUserRecvMsgID,ret,length);
//#endif  
//#if defined _DDS_TEST
//	
//				  if(2 == iTestOptions)
//				  {
//					  int iCounts = 0;
//					  for(;iCounts < iTestTimes ; iCounts++)
//						  ParseAllNetworkOrder(buffer, length, pstParticipant);
//				  }
//				  else
//				  {
//					  ParseAllNetworkOrder(buffer, length, pstParticipant);
//				  }
//				  recvlen = length; 
//#else
//					ParseAllNetworkOrder(buffer, length, pstParticipant);
//					recvlen = length; 
//#endif
//				}
//			}
//		}
//
//        
//#elif defined _653_PORT
//        RETURN_CODE_TYPE retCode = NO_ACTION;
//        for( i = 0 ; i < g_PortConfig.recvPortNum ; i++)
//        {
//			/* 接收消息 */
//			RECEIVE_QUEUING_MESSAGE(g_PortConfig.recvPort[i].portID, (APEX_LONG_INTEGER)0, (MESSAGE_ADDR_TYPE)buffer, (MESSAGE_SIZE_TYPE  *)&length, &retCode);
//			/* 成功收到消息 */
//			if (retCode == 0)
//			{
//				ParseAllNetworkOrder(buffer, length, pstParticipant);
//				recvlen = length;
//				printf("Nephalem FC Recv Data From %X Ret = %d , len = %d \n",g_PortConfig.recvPort[i].portSeq,retCode,length);
//			}
//		}
//#else
//        /* SPDP 接收任务 */
//        if (g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_RECV].active)
//        {
//            length = RTPSReceiveMsg(&pstParticipant->socketList.spdp_data_receiver, buffer);
//            if (length > 0)
//            {
//                //printf("Nephalem  SPDP RTPSReceiveMsg %d  \n",length);
//                ParseAllNetworkOrder(buffer, length, pstParticipant);
//                recvlen = length;
//            }
//        }
//
//        /* SEDP 接收任务 */
//        if (g_sub_task_ctrl_blocks[SUB_TASK_SEDP_DATA_RECV].active)
//        {
//            length = RTPSReceiveMsg(&pstParticipant->socketList.sedp_data_receiver, buffer);    
//			if (length > 0)
//			{
//	            //printf("Nephalem SEDP  RTPSReceiveMsg %d  \n",length);    
//                ParseAllNetworkOrder(buffer, length, pstParticipant);
//                recvlen = length;
//            }
//		}
//
//        /* USER 接收任务 */
//        if (g_sub_task_ctrl_blocks[SUB_TASK_USER_DATA_RECV].active)
//        {
//            length = RTPSReceiveMsg(&pstParticipant->socketList.user_data_receiver, buffer);        
//			if (length > 0)
//			{
//	            //printf("Nephalem USER RTPSReceiveMsg %d  \n",length);
//                ParseAllNetworkOrder(buffer, length, pstParticipant);
//                recvlen = length;
//            }
//		}
//              
//#endif
//        
////#if !defined(NO_HEARTBEAT)
////        /* 心跳任务 */
////        if (g_sub_task_ctrl_blocks[SUB_TASK_HEARTBEAT].active)
////        {
////        	SendHeartbeatTasksOrder(pstParticipant);
////        }
////#endif
//        
//		if (recvlen <= 0)
//		{
////			QY_Sleep(1);
//		}
//
//		DDS_MUTEX_UNLOCK(pstParticipant->threadMutex);
//	}
//
//    return;
//}

/*---------------------------------------------------------------------------------
-   函 数 名: SPDPSendTask
-   功能描述: 参与者发现数据发送任务
-   输    入: 参与者
-   返    回:
-   全局变量:
-   注    释: 参与者发现数据发送任务
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2020\06\08       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID SPDPSendTask(DDS_DomainParticipant* pstParticipant)
{
    MemoryBlock stMemBlock;
    CHAR buff[SEDP_BYTE_MAX_SIZE];
    /* 网络字节序初始化，保存发布报文 */
    while (pstParticipant->destroyFlag == FALSE)
    {
#ifdef  STATIC_DISCOVERY //运行过程中可能加入新节点，目前静态匹配不发送spdp LJM
		if (TRUE == pstParticipant->bOnlyStaticMathc)
		{
			QY_Sleep(50);
			continue;
		}
#endif

        INITIAL_MEMORYBLOCK(stMemBlock, buff, SEDP_BYTE_MAX_SIZE);
        JointSPDPDataMsg(&pstParticipant->stRTPSMsgHeader, pstParticipant->stBuiltinDataWriter[0].stHistoryCache.pstHead, &pstParticipant->stBuiltinDataWriter[0], &stMemBlock);
        SendMulticastMsg(&stMemBlock, &pstParticipant->socketList.spdp_data_sender, pstParticipant->rtpsParticipantProxy.metatrafficMulticastLocator);
        QY_Sleep(1000);
    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: SPDPReceiveTask
-   功能描述: 参与者发现数据接收任务
-   输    入: 参与者
-   返    回:
-   全局变量:
-   注    释: 参与者发现数据接收任务
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2020\06\08       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID SPDPReceiveTask(DDS_DomainParticipant* pstParticipant)
{
    INT length;
	/* Nephalem buf大小需要与recvfrom中的参数保持一致，否则堵塞不生效，cpu占用率高 */
	CHAR buffer[MAX_UDP_MSG_SIZE];
    while (pstParticipant->destroyFlag == FALSE)
    {
        length = RTPSReceiveMsg(&pstParticipant->socketList.spdp_data_receiver, buffer);

        if (length > 0)
        {
            ParseAllNetworkOrder(buffer, length, pstParticipant);
        } 
    }
	QY_Sleep(200);
}

/*---------------------------------------------------------------------------------
-   函 数 名: SEDPReceiveTask
-   功能描述: 内建实体数据接收任务
-   输    入: 参与者
-   返    回:
-   全局变量:
-   注    释: 内建实体数据接收任务
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2020\06\08       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID SEDPReceiveTask(DDS_DomainParticipant* pstParticipant)
{
    INT length;
    CHAR buffer[MAX_UDP_MSG_SIZE];
    while (pstParticipant->destroyFlag == FALSE)
    {
        length = RTPSReceiveMsg(&pstParticipant->socketList.sedp_data_receiver, buffer);

        if (length > 0)
        {
            ParseAllNetworkOrder(buffer, length, pstParticipant);
        }
    }
    QY_Sleep(100);

}

/*---------------------------------------------------------------------------------
-   函 数 名: USERReceiveTask
-   功能描述: 用户数据接收任务
-   输    入: 参与者
-   返    回:
-   全局变量:
-   注    释: 用户数据接收任务
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2020\06\08       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID USERReceiveTask(DDS_DomainParticipant* pstParticipant)
{
    INT length;
    CHAR buffer[MAX_UDP_MSG_SIZE];
    while (pstParticipant->destroyFlag == FALSE)
    {
        length = RTPSReceiveMsg(&pstParticipant->socketList.user_data_receiver, buffer);

        if (length > 0)
        {
            ParseAllNetworkOrder(buffer, length, pstParticipant);
        }
    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: HEARTSendTask
-   功能描述: 心跳发送任务
-   输    入: 参与者
-   返    回:
-   全局变量:
-   注    释: 心跳发送任务
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2020\06\08       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID HEARTSendTask(DDS_DomainParticipant* pstParticipant)
{
    UINT32 uiNum;
    DDS_DataWriter*  pstLocalWriter = NULL;
    int sleepTime = HEART_TIME;
#ifdef QYDDS_LIVELINESS_QOS
	if (pstParticipant->pstParticipantQos.liveliness.kind == MANUAL_BY_PARTICIPANT_LIVELINESS_QOS)
	{
		sleepTime = pstParticipant->pstParticipantQos.liveliness.lease_duration.sec * 1000;
		sleepTime += pstParticipant->pstParticipantQos.liveliness.lease_duration.nanosec / 1000 / 1000;
	}
	else
	{
		sleepTime = 100;
	}
#endif
    while (pstParticipant->destroyFlag == FALSE)
    {
		int init = 1;
#ifdef QYDDS_LIVELINESS_QOS
		init = 0;
#endif
        /* 内建实体心跳发送 */
		/*TODO：加锁放崩溃*/
        DDS_MUTEX_LOCK(pstParticipant->m_discRdMutex);
        for (uiNum = init; uiNum < 3; uiNum++)
        {
            JointHeartbeatMsgSendAllBuiltinReader(&pstParticipant->stRTPSMsgHeader, &pstParticipant->stBuiltinDataWriter[uiNum]);
        }
        DDS_MUTEX_UNLOCK(pstParticipant->m_discRdMutex);

//         /* 用户数据心跳发送；最新的版本把这部分放到了datawriter */
//         DDS_Topic* pstTopic = pstParticipant->pstTopic;
//         while (NULL != pstTopic)
//         {
//             pstLocalWriter = pstTopic->pstLocalWriter;
//             while (NULL != pstLocalWriter)
//             {
//                 if (RELIABLE_RELIABILITY_QOS == pstLocalWriter->stDataWriterQos.reliability.kind)
//                 {
//                     JointHeartbeatMsgSendUnknownReader(pstLocalWriter);
//                 }
//                 pstLocalWriter = pstLocalWriter->pNext;
//             }
// 
//             pstTopic = pstTopic->pNext;
//         }
		QY_Sleep(sleepTime);
    }
}

VOID DiscHEARTCheckTask(DDS_DomainParticipant* pstParticipant)
{
#if defined(_VXWORKS_653) || defined (_TM1) || defined _TM3
    /* 653任务无法传参，使用全局变量 */
	pstParticipant = pstRealDomainParticipant;
#endif
	DDS_DiscoveredParticipant* pstDiscParticipant = NULL;
	DDS_DiscoveredParticipant* delDiscParticipant = NULL;
	UINT32 addr;
	Duration_t nowTime;
	int k[4];
	QY_Sleep(1000);
	printf("start DiscHEARTCheckTask ..\n");
    while (pstParticipant->destroyFlag == FALSE)
	{
		/*TODO：应需求，对于远端未配置liveness的情况，也进行检测，合并时确认标准定义*/
		//if (pstParticipant->livelinessNum == 0)
		//{
		//	QY_Sleep(HEART_TIME);
		//	continue;
		//}
		TopicLiveLinessCheck(pstParticipant);

		pstDiscParticipant = pstParticipant->pstDiscParticipant;
#ifdef  STATIC_DISCOVERY
		while (NULL != pstDiscParticipant && pstDiscParticipant->bOnlyStaticMathc == FALSE)//静态匹配的参与者
#else
		while (NULL != pstDiscParticipant)
#endif
		{
			/*TODO：应需求，对于远端未配置liveness的情况，也进行检测，合并时确认标准定义*/
            if (pstDiscParticipant->leaseDuration.sec == 0 && pstDiscParticipant->leaseDuration.nanosec == 0)
            {
                pstDiscParticipant->leaseDuration.sec = 3;
            }
			if (Compare_Duration(pstParticipant->discMinDuration, pstDiscParticipant->leaseDuration) == TRUE && (pstDiscParticipant->leaseDuration.sec != 0 || pstDiscParticipant->leaseDuration.nanosec != 0))
			{
				DDS_MUTEX_LOCK(pstParticipant->sleepMutex);
				pstParticipant->sleepTime = pstDiscParticipant->leaseDuration.sec * 1000;
				pstParticipant->sleepTime += pstDiscParticipant->leaseDuration.nanosec / 1000 / 1000;
				pstParticipant->discMinDuration = pstDiscParticipant->leaseDuration;
				DDS_MUTEX_UNLOCK(pstParticipant->sleepMutex);
			}
			nowTime = GetNowTime();
			DDS_MUTEX_LOCK(pstDiscParticipant->threadMutex);
			//printf("nowTime: %d:%d, lastTime: %d:%d\n", nowTime.sec, nowTime.nanosec, pstDiscParticipant->lastHeartBeat.sec, pstDiscParticipant->lastHeartBeat.nanosec);
			nowTime = Sub_Duration(nowTime, pstDiscParticipant->lastHeartBeat);
			//printf("HeartusedTime: %d, %d\n", nowTime.sec, nowTime.nanosec);
			if (Compare_Duration(nowTime, pstDiscParticipant->leaseDuration) == TRUE && (pstDiscParticipant->leaseDuration.nanosec != 0 || pstDiscParticipant->leaseDuration.sec != 0))
			{
				pstDiscParticipant->livelinessHeartMissNum++;
				//printf("livelinessNum: %d, duration: %d, sleepTime: %d\n", pstDiscParticipant->livelinessNum, pstDiscParticipant->leaseDuration.nanosec,pstParticipant->sleepTime);
			}
			if (pstDiscParticipant->livelinessHeartMissNum > 3 && ISEQUAL_GUIDPREFIX(pstParticipant->stRTPSMsgHeader.guidPrefix, pstDiscParticipant->stRTPSMsgHeader.guidPrefix) == 0)
			{
				delDiscParticipant = pstDiscParticipant;
				DDS_MUTEX_UNLOCK(pstDiscParticipant->threadMutex);
				pstDiscParticipant = pstDiscParticipant->pNext;
				
				
				addr = delDiscParticipant->IPAddr;

				k[0] = addr & 0xFF;
				addr >>= 8;

				k[1] = addr & 0xFF;
				addr >>= 8;

				k[2] = addr & 0xFF;
				addr >>= 8;

				k[3] = addr & 0xFF;

				//printf("The remote participant disconnect! IPAddr: %d.%d.%d.%d, ProcessID: %d.\n", k[0], k[1], k[2], k[3], delDiscParticipant->processID);
				printf("The remote participant disconnect! guid: %d.%d.%d.%d, ProcessID: %d.\n", 
						delDiscParticipant->stRTPSMsgHeader.guidPrefix.value[0], 
						delDiscParticipant->stRTPSMsgHeader.guidPrefix.value[1],
						delDiscParticipant->stRTPSMsgHeader.guidPrefix.value[2],
						delDiscParticipant->stRTPSMsgHeader.guidPrefix.value[3],
						delDiscParticipant->processID);

              /*TODO：移除节点之前加锁防崩溃*/
			    DDS_MUTEX_LOCK(pstParticipant->threadMutex);
                DDS_MUTEX_LOCK(pstParticipant->m_discWtMutex);
                DDS_MUTEX_LOCK(pstParticipant->m_discRdMutex);
                
                DeleteALLDiscoveredFromByGuidPrefix(pstParticipant, delDiscParticipant->stRTPSMsgHeader.guidPrefix);
                DDS_MUTEX_UNLOCK(pstParticipant->threadMutex);
                DDS_MUTEX_UNLOCK(pstParticipant->m_discWtMutex);
                DDS_MUTEX_UNLOCK(pstParticipant->m_discRdMutex);

				pstParticipant->sleepTime = 1000;
				pstParticipant->discMinDuration.sec = 1;
				pstParticipant->discMinDuration.nanosec = 0;
				pstParticipant->livelinessNum--;
				continue;
			}
            else
            {
                DDS_MUTEX_UNLOCK(pstDiscParticipant->threadMutex);
            }
			pstDiscParticipant = pstDiscParticipant->pNext;
			
		}
		QY_Sleep(pstParticipant->sleepTime);
	}
}

/*---------------------------------------------------------------------------------
-   函 数 名: NetworkSendReceiveTask
-   功能描述: 多任务报文收发
-   输    入: 参与者
-   返    回:
-   全局变量:
-   注    释: 多任务报文收发
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2020\06\08       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID NetworkSendReceiveTask(DDS_DomainParticipant* pstParticipant)
{
#if defined(_WIN32)
#ifdef SINGLE_THREAD //单线程模式，FC、RAPIDIO、653Port、以太网都在这个线程接收数据；在FC和Rapidio存在的情况下必须使用单线程
	HANDLE threadHandle = CreateThread(NULL, 0, (VOID*)&NetworkTransceiverTask, (VOID*)pstParticipant, 0, NULL);
#else //多线程模式
	/* SPDP发送报文 */
	pstParticipant->hThread[0] = CreateThread(NULL, 0, (VOID*)&SPDPSendTask, (VOID*)pstParticipant, 0, NULL);
	/* SPDP接收报文 */
	pstParticipant->hThread[1] = CreateThread(NULL, 0, (VOID*)&SPDPReceiveTask, (VOID*)pstParticipant, 0, NULL);
	/* SEDP接收报文 */
	pstParticipant->hThread[2] = CreateThread(NULL, 0, (VOID*)&SEDPReceiveTask, (VOID*)pstParticipant, 0, NULL);
	/* USER接收报文 */
	pstParticipant->hThread[3] = CreateThread(NULL, 0, (VOID*)&USERReceiveTask, (VOID*)pstParticipant, 0, NULL);
	/* 心跳发送报文 */
	pstParticipant->hThread[4] = CreateThread(NULL, 0, (VOID*)&HEARTSendTask, (VOID*)pstParticipant, 0, NULL);
#endif

#ifdef QYDDS_LIVELINESS_QOS
	/*远端心跳检测报文*/
	pstParticipant->hThread[5] = CreateThread(NULL, 0, (VOID*)&DiscHEARTCheckTask, (VOID*)pstParticipant, 0, NULL);
#endif //QYDDS_LIVELINESS_QOS
#ifdef TG
	/* 周期组包发送数据缓存线程 */
	pstParticipant->hThread[6] = CreateThread(NULL, 0, (VOID*)&UserDataPeriodTask, (VOID*)pstParticipant, 0, NULL);
#endif //TG
#elif defined(__linux__)//_WIN32
    pthread_create(&pstParticipant->hThread[0], 0, SPDPSendTask, pstParticipant);
    pthread_create(&pstParticipant->hThread[1], 0, SPDPReceiveTask, pstParticipant);
    pthread_create(&pstParticipant->hThread[2], 0, SEDPReceiveTask, pstParticipant);
    pthread_create(&pstParticipant->hThread[3], 0, USERReceiveTask, pstParticipant);
    pthread_create(&pstParticipant->hThread[4], 0, HEARTSendTask, pstParticipant);
#ifdef QYDDS_LIVELINESS_QOS
    /*远端心跳检测报文*/
    pthread_create(&pstParticipant->hThread[5], 0, DiscHEARTCheckTask, pstParticipant);
#endif
#elif defined (_VXWORKS)
    
#if defined (_VXWORKS_653) || defined (_TM3)
    	
    RETURN_CODE_TYPE rtCode = 0;
    
    PROCESS_ID_TYPE procId;
		
    //PROCESS_ATTRIBUTE_TYPE processTable = {"NetworkTransceiverTask",NetworkTransceiverTask,1024 * 1024, 50, 0, t_4sec, HARD};
    PROCESS_ATTRIBUTE_TYPE processTable = {"NetworkTransceiverTask",NetworkTransceiverTask,256 * 1024, 50, -1, -1, SOFT};
    
    CREATE_PROCESS(&processTable,&procId,&rtCode);
    
	if (rtCode != NO_ERROR)
	{
		printf ("CREATE_PROCESS failed : NetworkTransceiverTask \n");
	}   
	
	START(procId, &rtCode);
	if (rtCode != NO_ERROR)
	{
		printf("START failed : NetworkTransceiverTask\n");
	}
	
//	PROCESS_ATTRIBUTE_TYPE processTable2 = {"DiscHEARTCheckTask",DiscHEARTCheckTask,1024 * 1024, 60, 0, t_4sec, HARD};
	PROCESS_ATTRIBUTE_TYPE processTable2 = {"DiscHEARTCheckTask",DiscHEARTCheckTask,128 * 1024, 60, -1, -1, SOFT};

	CREATE_PROCESS(&processTable2,&procId,&rtCode);

	if (rtCode != NO_ERROR)
	{
		printf ("CREATE_PROCESS failed : DiscHEARTCheckTask \n");
	}

	START(procId, &rtCode);
	if (rtCode != NO_ERROR)
	{
		printf("START failed : DiscHEARTCheckTask\n");
	}
	
	PROCESS_ATTRIBUTE_TYPE processTable3 = {"DeadlineCheck",DeadlineCheck,128 * 1024, 60, -1, -1, SOFT};
	    
	CREATE_PROCESS(&processTable3,&procId,&rtCode);
	
	if (rtCode != NO_ERROR)
	{
		printf ("CREATE_PROCESS failed : DiscHEARTCheckTask \n");
	}   
	
	START(procId, &rtCode);
	if (rtCode != NO_ERROR)
	{
		printf("START failed : DiscHEARTCheckTask\n");
	}
#elif defined _TM1

	ACoreOs_task_create("NetworkTransceiverTask",
			75,
			malloc( ACOREOS_MINIMUM_STACK_SIZE * 8 ),
			ACOREOS_MINIMUM_STACK_SIZE * 8,
			ACOREOS_DEFAULT_ATTRIBUTES,
			NULL,
			&pstParticipant->hThread[0]);

	ACoreOs_task_start(pstParticipant->hThread[0],NetworkTransceiverTask,0);

	ACoreOs_task_create("DiscHEARTCheckTask",
			75,
			malloc( ACOREOS_MINIMUM_STACK_SIZE * 8 ),
			ACOREOS_MINIMUM_STACK_SIZE * 8,
			ACOREOS_DEFAULT_ATTRIBUTES,
			NULL,
			&pstParticipant->hThread[1]);

	ACoreOs_task_start(pstParticipant->hThread[1],DiscHEARTCheckTask,0);

	ACoreOs_task_create("DeadlineCheck",
			75,
			malloc( ACOREOS_MINIMUM_STACK_SIZE * 8 ),
			ACOREOS_MINIMUM_STACK_SIZE * 8,
			ACOREOS_DEFAULT_ATTRIBUTES,
			NULL,
			&pstParticipant->hThread[2]);

	ACoreOs_task_start(pstParticipant->hThread[2],DeadlineCheck,0);

#elif defined _TM3

	ACoreOs_Task_Param options;
	ACoreOs_status_code retCode = ACOREOS_SUCCESSFUL;
	options.affinity = ACOREOS_TASK_NO_AFFINITY;
	options.attribute_set =  ACOREOS_PREEMPT | ACOREOS_TIMESLICE;
	options.domain = (void *)ACOREOS_KERNEL_ID;
    options.initial_priority = 11;
    options.stack_size = 16 * 1024 ;

    retCode = ACoreOs_task_create((ACoreOs_name)"NetworkTransceiverTask",&options,&pstParticipant->hThread[0]);
    if ( retCode != ACOREOS_SUCCESSFUL )
    {
    	printf("Failed to create NetworkTransceiverTask, return code:%#x!\n",retCode);
    }

    retCode = ACoreOs_task_start(pstParticipant->hThread[0],(ACoreOs_task_entry)NetworkTransceiverTask,0);
    if ( retCode != ACOREOS_SUCCESSFUL )
    {
      	printf("Failed to start NetworkTransceiverTask, return code:%d!\n", retCode);
    }

    retCode = ACoreOs_task_create((ACoreOs_name)"DiscHEARTCheckTask",&options,&pstParticipant->hThread[1]);
    if ( retCode != ACOREOS_SUCCESSFUL )
    {
    	printf("Failed to create DiscHEARTCheckTask, return code:%#x!\n",retCode);
    }

    retCode = ACoreOs_task_start(pstParticipant->hThread[1],(ACoreOs_task_entry)DiscHEARTCheckTask,0);
    if ( retCode != ACOREOS_SUCCESSFUL )
    {
      	printf("Failed to start DiscHEARTCheckTask, return code:%d!\n", retCode);
    }

    retCode = ACoreOs_task_create((ACoreOs_name)"DeadlineCheck",&options,&pstParticipant->hThread[2]);
    if ( retCode != ACOREOS_SUCCESSFUL )
    {
    	printf("Failed to create DeadlineCheck, return code:%#x!\n",retCode);
    }

    retCode = ACoreOs_task_start(pstParticipant->hThread[2],(ACoreOs_task_entry)DeadlineCheck,0);
    if ( retCode != ACOREOS_SUCCESSFUL )
    {
      	printf("Failed to start DeadlineCheck, return code:%d!\n", retCode);
    }


#else
    
#ifdef SINGLE_THREAD
    
    pstParticipant->hThread[0] = taskSpawn("NetworkTransceiverTask",70,VX_FP_TASK,1024 * 1024 * 4,NetworkTransceiverTask,(int)pstParticipant,0,0,0,0,0,0,0,0,0);
    
//    pstParticipant->hThread[1] = taskSpawn("DiscHEARTCheckTask",115,VX_FP_TASK,1024 * 1024 * 4,DiscHEARTCheckTask,(int)pstParticipant,0,0,0,0,0,0,0,0,0);
//    
//    pstParticipant->hThread[2] = taskSpawn("DeadlineCheck",115,VX_FP_TASK,1024 * 1024 * 4,DeadlineCheck,(int)pstParticipant,0,0,0,0,0,0,0,0,0);
    
#else
    pstParticipant->hThread[0] = taskSpawn("SPDPSendTask",115,VX_FP_TASK,1024 * 1024 * 4,SPDPSendTask,(int)pstParticipant,0,0,0,0,0,0,0,0,0);
    pstParticipant->hThread[1] = taskSpawn("SPDPReceiveTask",115,VX_FP_TASK,1024 * 1024 * 4,SPDPReceiveTask,(int)pstParticipant,0,0,0,0,0,0,0,0,0);
    pstParticipant->hThread[2] = taskSpawn("SEDPReceiveTask",115,VX_FP_TASK,1024 * 1024 * 4,SEDPReceiveTask,(int)pstParticipant,0,0,0,0,0,0,0,0,0);
    pstParticipant->hThread[3] = taskSpawn("USERReceiveTask",115,VX_FP_TASK,1024 * 1024 * 4,USERReceiveTask,(int)pstParticipant,0,0,0,0,0,0,0,0,0);
    pstParticipant->hThread[4] = taskSpawn("HEARTSendTask",115,VX_FP_TASK,1024 * 1024 * 4,HEARTSendTask,(int)pstParticipant,0,0,0,0,0,0,0,0,0);
#ifdef QYDDS_LIVELINESS_QOS
    pstParticipant->hThread[5] = taskSpawn("DiscHEARTCheckTask",115,VX_FP_TASK,1024 * 1024 * 4,DiscHEARTCheckTask,(int)pstParticipant,0,0,0,0,0,0,0,0,0);
#endif
    
#endif
    
#endif
    
#endif
}

/* 多任务网络收发任务关闭 */
VOID CloseNetworkSendReceiveTask(DDS_DomainParticipant* pstParticipant)
{
	int iNum = 0;

	pstParticipant->destroyFlag = TRUE;

#if defined(_WIN32)

#ifdef RAPID_IO_CHANNEL

#else
	//for(iNum = 0 ; iNum <= 5 ; iNum++)
	//{
	//	TerminateThread(pstParticipant->hThread[iNum], 0);

	//	WaitForSingleObject(pstParticipant->hThread[iNum], 500);

	//	CloseHandle(pstParticipant->hThread[iNum]);
	//}
#endif
#endif

}


/*---------------------------------------------------------------------------------
-   函 数 名: DeleteBuiltinDiscParticipantFromParticipantByGuidPrefix
-   功能描述: 删除远端内建参与者
-   输    入: 主题、Guid
-   返    回:
-   全局变量:
-   注    释: 主题中删除指定远端内建参与者
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DeleteBuiltinDiscParticipantFromParticipantByGuidPrefix(DDS_DomainParticipant* pstParticipant, GuidPrefix_t*  pstPrefix)
{
    DDS_DiscoveredParticipant*  pstPrevDiscParticipant = NULL;
    DDS_DiscoveredParticipant*  pstDiscParticipant = pstParticipant->pstDiscParticipant;

    while (NULL != pstDiscParticipant)
    {
        if (GuidPrefix_Is_Equal(&pstDiscParticipant->stRTPSMsgHeader.guidPrefix, pstPrefix))
        {
            if (!pstPrevDiscParticipant)
            {
                pstDiscParticipant = pstDiscParticipant->pNext;
                DDS_DiscoveredParticipant_Uninit(pstParticipant->pstDiscParticipant);
                DDS_STATIC_FREE(pstParticipant->pstDiscParticipant);
                pstParticipant->pstDiscParticipant = pstDiscParticipant;
            }
            else
            {
                pstPrevDiscParticipant->pNext = pstDiscParticipant->pNext;
                DDS_DiscoveredParticipant_Uninit(pstDiscParticipant);
                DDS_STATIC_FREE(pstDiscParticipant);
                pstDiscParticipant = pstPrevDiscParticipant->pNext;
            }
        }
        else
        {
            pstPrevDiscParticipant = pstDiscParticipant;
            pstDiscParticipant = pstDiscParticipant->pNext;
        }
    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: DeleteALLDiscoveredFromByGuidPrefix
-   功能描述: 删除远端所有连接信息
-   输    入: 参与者、pstPrefix
-   返    回:
-   全局变量:
-   注    释: 删除远端所有连接信息
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DeleteALLDiscoveredFromByGuidPrefix(DDS_DomainParticipant* pstParticipant, GuidPrefix_t stPrefix)
{
	//return ;
    UINT32 uiBuiltinNun = 0;
	DDS_Topic* pstTopic = NULL;

    /* 清除内建远端参与者 */
    DeleteBuiltinDiscParticipantFromParticipantByGuidPrefix(pstParticipant, &stPrefix);

    for (uiBuiltinNun = 0; uiBuiltinNun < 3; uiBuiltinNun++)
    {
        /* 远端内建写入器清除 */
        DeleteBuiltinDiscReaderFromBuiltinWriterByGuidPrefix(&pstParticipant->stBuiltinDataWriter[uiBuiltinNun], &stPrefix);

        /* 远端内建阅读器清除 */
        DeleteBuiltinDiscWriterFromBuiltinReaderByGuidPrefix(&pstParticipant->stBuiltinDataReader[uiBuiltinNun], &stPrefix);
    }

    pstTopic = pstParticipant->pstTopic;

    while (NULL != pstTopic)
    {
        /* 远端写入器清除 */
		DDS_MUTEX_LOCK(pstParticipant->m_discWtMutex);
		DeleteDiscWriterFromTopicByGuidPrefix(pstTopic, &stPrefix);
		DDS_MUTEX_UNLOCK(pstParticipant->m_discWtMutex);

        /* 远端阅读器清除 */
        DDS_MUTEX_LOCK(pstParticipant->m_discRdMutex);
        DeleteDiscReaderFromTopicByGuidPrefix(pstTopic, &stPrefix);
        DDS_MUTEX_UNLOCK(pstParticipant->m_discRdMutex);

        /* Nephalem ownership 更新所有权强度最高的写入器 */
        FindOwnerWriter(pstTopic);

        pstTopic = pstTopic->pNext;
    }
}

/*---------------------------------------------------------------------------------
-   函 数 名: UserMsgSendMonitor
-   功能描述: 删除远端所有连接信息
-   输    入: 参与者、pstPrefix
-   返    回:
-   全局变量:
-   注    释: 删除远端所有连接信息
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2019\8\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID UserMsgSendMonitor(DDS_DomainParticipant* pstParticipant, GUID_t* tpGuid, MemoryBlock* pstMemBlock, UINT8 type)
{
    DDS_TopicMon* pstTopicMon = NULL;
    DDS_Monitor* pstPreMonitor = NULL;
    DDS_Monitor* pstMonitor = pstParticipant->pstMonitor;
    while (NULL != pstMonitor)
    {
        /*  超过3秒清除监控节点 */
        if (time(NULL) - pstMonitor->deadtime > 3)
        {
            if (NULL == pstPreMonitor)
            {
                pstParticipant->pstMonitor = pstMonitor->pNext;
                /* 析构监控节点 */
                UninitMonitor(pstMonitor);
                DDS_STATIC_FREE(pstMonitor);
                pstMonitor = pstParticipant->pstMonitor;
            }
            else
            {
                pstPreMonitor->pNext = pstMonitor->pNext;
                /* 析构监控节点 */
                UninitMonitor(pstMonitor);
                DDS_STATIC_FREE(pstMonitor);
                pstMonitor = pstPreMonitor->pNext;
            }
        }
        else
        {
            pstTopicMon = pstMonitor->pstTopicMon;
            while (NULL != pstTopicMon)
            {
                if (Guid_Is_Equal(&pstTopicMon->tpGuid, tpGuid))
                {
                    /* 订阅消息标志位 */
                    pstMemBlock->base[6] = type;
                    /* 发送到对应的每个远端阅读器 */
                    RTPSSendMsg(pstMemBlock, &pstParticipant->socketList.user_data_sender, pstMonitor->unicastLocator);
                }

                pstTopicMon = pstTopicMon->pNext;
            }

			pstPreMonitor = pstMonitor;
            pstMonitor = pstMonitor->pNext;
        }
    }

}

/*---------------------------------------------------------------------------------
-   函 数 名: AddPartition
-   功能描述: 主题加入分区
-   输    入: pstTopic、分区名
-   返    回:
-   全局变量:
-   注    释: 
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2020/01/20      1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t AddPartition(DDS_Topic* pstTopic, const CHAR* pstPartitionname)
{
    DDS_TopicQos* pstTopicQos = &pstTopic->stTopicQos;
	PartitionName* pstTempPartitionName = NULL;
	PartitionName* pstPartitionName = NULL;
    /* Nephalem Partition 分区名超过256  太长 */
    if (strlen(pstPartitionname) + 1 > 256)
    {
        printf("Nephalem : AddPartition pstPartitionname is too big \n");
        return DDS_RETCODE_ERROR;
    }

    pstTempPartitionName = pstTopicQos->partition.pValue;
    while (NULL != pstTempPartitionName)
    {
        /* Nephalem Partition 分区名已存在 直接返回 */
        if (!strcmp(pstTempPartitionName->partitionValue.value, pstPartitionname))
            return DDS_RETCODE_OK;

        pstTempPartitionName = pstTempPartitionName->pNext;
    }

    /* Nephalem Partition 申请内存 */
    pstPartitionName = (PartitionName*)DDS_STATIC_MALLOC(sizeof(PartitionName));
    if (NULL == pstPartitionName)
    {
        printf("Nephalem : AddPartition DDS_STATIC_MALLOC Error \n");
        return DDS_RETCODE_ERROR;
    }
    pstPartitionName->pNext = NULL;
    /* Nephalem Partition 设置分区名和长度 */
    pstPartitionName->partitionValue.length = strlen(pstPartitionname) + 1;
    memcpy(pstPartitionName->partitionValue.value, pstPartitionname, pstPartitionName->partitionValue.length);

    /* Nephalem Partition 插入到链表中 */
    LIST_INSERT_HEAD(pstTopicQos->partition.pValue, pstPartitionName);

    pstTopicQos->partition.size++;

    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   函 数 名: DeletePartition
-   功能描述: 主题删除分区
-   输    入: pstTopic、分区名
-   返    回:
-   全局变量:
-   注    释:
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2020/01/20      1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
DDS_ReturnCode_t DeletePartition(DDS_Topic* pstTopic, const CHAR* pstPartitionname)
{
	PartitionName* pstPrePartitionName = NULL;
	PartitionName* pstTempPartitionName = NULL;
    DDS_TopicQos* pstTopicQos = &pstTopic->stTopicQos;
    /* Nephalem Partition 分区名超过256  太长 */
    if (strlen(pstPartitionname) + 1 > 256)
    {
        printf("Nephalem : AddPartition pstPartitionname is too big \n");
        return DDS_RETCODE_ERROR;
    }

    pstTempPartitionName = pstTopicQos->partition.pValue;
    while (NULL != pstTempPartitionName)
    {
        /* Nephalem Partition 分区名存在 进行删除 */
        if (!strcmp(pstTempPartitionName->partitionValue.value, pstPartitionname))
        {
            /* Nephalem Partition 要删除的节点为头节点 */
            if (NULL == pstPrePartitionName)
                pstTopicQos->partition.pValue = pstTempPartitionName->pNext;
            /* Nephalem Partition 要删除的节点为中间节点 */
            else
                pstPrePartitionName->pNext = pstTempPartitionName->pNext;

            DDS_STATIC_FREE(pstTempPartitionName);
            pstTempPartitionName = NULL;

            pstTopicQos->partition.size--;

            return DDS_RETCODE_OK;
        }

        pstPrePartitionName = pstTempPartitionName;
        pstTempPartitionName = pstTempPartitionName->pNext;
    }

    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   函 数 名: SetTopicQoS
-   功能描述: 重新设置TopicQoS 使其生效
-   输    入: pstTopic
-   返    回:
-   全局变量:
-   注    释:
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2020/01/20      1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
DDS_ReturnCode_t SetTopicQoS(DDS_Topic* pstTopic)
{
	MemoryBlock stMemBlock;
	DDS_DiscoveredReader* pstDDS_DiscoveredReader = NULL;
	DDS_DataWriter* pstDataWriter = NULL;
	HistoryData* pstHistoryData = NULL;
	DDS_DataReader* pstDataReader = NULL;
    DDS_DiscoveredWriter* pstDDS_DiscoveredWriter = pstTopic->pstDiscWriter;
    /* Nephalem 更新本地远端写入器的是否有相同partition的状态 */
    while (NULL != pstDDS_DiscoveredWriter)
    {
        pstDDS_DiscoveredWriter->bSamePartiton = EqualPartitionName(&pstTopic->stTopicQos, &pstDDS_DiscoveredWriter->pstRelateTopic->stTopicQos);
        pstDDS_DiscoveredWriter = pstDDS_DiscoveredWriter->pNext;
    }

    pstDDS_DiscoveredReader = pstTopic->pstDiscReader;
    /* Nephalem 更新本地远端阅读器的是否有相同partition的状态 */
    while (NULL != pstDDS_DiscoveredReader)
    {
        pstDDS_DiscoveredReader->bSamePartiton = EqualPartitionName(&pstTopic->stTopicQos, &pstDDS_DiscoveredReader->pstRelateTopic->stTopicQos);
        pstDDS_DiscoveredReader = pstDDS_DiscoveredReader->pNext;
    }

    /* Nephalem 将更改后partition的状态发送给其他节点 */
    pstDataWriter = pstTopic->pstLocalWriter;

    while (NULL != pstDataWriter)
    {
        /* 申请静态内存创建HistoryData链表节点 */
        pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC(sizeof(HistoryData));
        if (NULL == pstHistoryData)
        {
            PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData, Insufficient memory space.\n");
            DDS_STATIC_FREE(pstDataWriter);
            pstDataWriter = NULL;
            return DDS_RETCODE_ERROR;
        }

		/* Nephalem 进行初始化，否则释放会崩溃 */
		InitHistoryData(pstHistoryData);

        /* 申请静态内存创建历史缓存数据 */
        pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC(SEDP_BYTE_MAX_SIZE);
        if (NULL == pstHistoryData->data)
        {
            /* 失败释放前面申请的内存 */
            DDS_STATIC_FREE(pstHistoryData);
            pstHistoryData = NULL;
            DDS_STATIC_FREE(pstDataWriter);
            pstDataWriter = NULL;
            PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData buff, Insufficient memory space.\n");
            return DDS_RETCODE_ERROR;
        }

        /* 网络字节序初始化，保存发布报文 */
        INITIAL_MEMORYBLOCK(stMemBlock, pstHistoryData->data, SEDP_BYTE_MAX_SIZE);

        /* 内建seqNum自加1*/
        pstTopic->pstParticipant->stBuiltinDataWriter[1].seqNum = Add_SequenceNumber(pstTopic->pstParticipant->stBuiltinDataWriter[1].seqNum, SEQUENCENUMBER_START);

        /* 设置是否取消发布 */
        pstHistoryData->bCancel = FALSE;

        /* 缓存记录seqNum编号 */
        pstHistoryData->seqNum = pstTopic->pstParticipant->stBuiltinDataWriter[1].seqNum;

        /* 缓存记录实体GUID */
        pstHistoryData->stGuid = pstDataWriter->guid;

        /* 序列化写入器消息体 */
        DataWriterConvertNetworkByteOrder(pstTopic, pstDataWriter, &stMemBlock);

        /* 记录序列化报文长度 */
        pstHistoryData->uiDataLen = stMemBlock.writeIndex;

        /* 插入到内建发布写入器缓存 */
        InsertHistoryCacheTail(&pstTopic->pstParticipant->stBuiltinDataWriter[1].stHistoryCache, pstHistoryData);

        /* 组装发布报文 */
        JointSEDPDataMsgSendBuiltinReader(&pstTopic->pstParticipant->stRTPSMsgHeader, pstHistoryData, &pstTopic->pstParticipant->stBuiltinDataWriter[1], NULL);

        PrintLog(COMMLOG_INFO, "Topic: The dataWriter was created successfully.\n");

#ifdef QYDDS_DEADLINE_QOS
        if (pstDataWriter->stDataWriterQos.deadline.period.sec != 0 || pstDataWriter->stDataWriterQos.deadline.period.nanosec != 0)
        {
            DeadLineSendTask(pstDataWriter);
        }
#endif

        pstDataWriter = pstDataWriter->pNext;
    }

    pstDataReader = pstTopic->pstLocalReader;
    
    while (NULL != pstDataReader)
    {

        /* 申请静态内存创建HistoryData链表节点 */
        pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC(sizeof(HistoryData));
        if (NULL == pstHistoryData)
        {
            DDS_STATIC_FREE(pstDataReader);
            pstDataReader = NULL;
            PrintLog(COMMLOG_ERROR, "DataReader: Failed to create HistoryData, Insufficient memory space.\n");
            return DDS_RETCODE_ERROR;
        }

		/* Nephalem 进行初始化，否则释放会崩溃 */
		InitHistoryData(pstHistoryData);

        /* 申请静态内存创建历史缓存数据 */
        pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC(SEDP_BYTE_MAX_SIZE);
        if (NULL == pstHistoryData->data)
        {
            /* 失败释放前面申请的内存 */
            DDS_STATIC_FREE(pstDataReader);
            pstDataReader = NULL;
            DDS_STATIC_FREE(pstHistoryData);
            pstHistoryData = NULL;
            PrintLog(COMMLOG_ERROR, "DataReader: Failed to create HistoryData buff, Insufficient memory space.\n");
            return DDS_RETCODE_ERROR;
        }

        /* 网络字节序初始化，保存发布报文 */
        INITIAL_MEMORYBLOCK(stMemBlock, pstHistoryData->data, SEDP_BYTE_MAX_SIZE);

        /* 内建seqNum自加1*/
        pstTopic->pstParticipant->stBuiltinDataWriter[2].seqNum = Add_SequenceNumber(pstTopic->pstParticipant->stBuiltinDataWriter[2].seqNum, SEQUENCENUMBER_START);

        /* 设置是否取消订阅 */
        pstHistoryData->bCancel = FALSE;

        /* 缓存记录seqNum编号 */
        pstHistoryData->seqNum = pstTopic->pstParticipant->stBuiltinDataWriter[2].seqNum;

        /* 缓存记录实体GUID */
        pstHistoryData->stGuid = pstDataReader->guid;

        /* 序列化阅读器消息体 */
        DataReaderConvertNetworkByteOrder(pstTopic, pstDataReader, &stMemBlock);

        /* 记录序列化报文长度 */
        pstHistoryData->uiDataLen = stMemBlock.writeIndex;

        /* 插入到内建发布写入器缓存 */
        InsertHistoryCacheTail(&pstTopic->pstParticipant->stBuiltinDataWriter[2].stHistoryCache, pstHistoryData);

        /* 组装订阅报文 */
        JointSEDPDataMsgSendBuiltinReader(&pstTopic->pstParticipant->stRTPSMsgHeader, pstHistoryData, &pstTopic->pstParticipant->stBuiltinDataWriter[2], NULL);

        PrintLog(COMMLOG_INFO, "Topic: The dataReader was created successfully.\n");

        pstDataReader = pstDataReader->pNext;
    }

	/* Nephalem 分区修改后，刷新ownership */
	FindOwnerWriter(pstTopic);
   
    return DDS_RETCODE_OK;
}

DDS_ReturnCode_t TopicLiveLinessCheck(DDS_DomainParticipant* pstParticipant)
{
	DDS_Topic* pstTopic = pstParticipant->pstTopic;
    DDS_DiscoveredWriter* pstDiscDataWriter;
    DDS_DiscoveredWriter* pstPrvDiscDataWriter;
	while (NULL != pstTopic)
	{
		DDS_DataReader* pstDataReader = pstTopic->pstLocalReader;

		if (NULL == pstDataReader) //本地没有该主题的订阅端，跳过该主题
		{
			pstTopic = pstTopic->pNext;
			continue;
		}

		//本地主题reader的存活性QoS不为依赖主题，直接跳过
		if (MANUAL_BY_TOPIC_LIVELINESS_QOS != pstDataReader->stDataReaderQos.liveliness.kind)
		{
			pstTopic = pstTopic->pNext;
			continue;
		}

		pstDiscDataWriter = pstTopic->pstDiscWriter;
#ifdef  STATIC_DISCOVERY
		while (NULL != pstDiscDataWriter && pstDiscDataWriter->pstRelateTopic->pstParticipant->bOnlyStaticMathc == FALSE)
#else
		while (NULL != pstDiscDataWriter)
#endif
		{
			//远端主题writer的存活性QoS不为依赖主题，直接跳过
			if (MANUAL_BY_TOPIC_LIVELINESS_QOS != pstDiscDataWriter->stDataWriterQos.liveliness.kind)
			{
				pstDiscDataWriter = pstDiscDataWriter->pNext;
				continue;
			}
			
			//存活性时间设置为0，相当于没有设置，跳过
			if (0 == pstDiscDataWriter->lastLivelinessTime.sec && 0 == pstDiscDataWriter->lastLivelinessTime.nanosec)
			{
				pstDiscDataWriter = pstDiscDataWriter->pNext;
				continue;
			}

			//判断远端主题writer存活状态
			if (FALSE == Compare_Duration(Add_Duration(pstDiscDataWriter->stDataWriterQos.liveliness.lease_duration, pstDiscDataWriter->lastLivelinessTime), GetNowTime()))
			{
				printf("%s : liviliness disconnect \n",pstTopic->topicName.value);
				pstPrvDiscDataWriter = pstTopic->pstDiscWriter;
				if(pstPrvDiscDataWriter == pstDiscDataWriter)
					pstTopic->pstDiscWriter = pstTopic->pstDiscWriter->pNext;
				else
				{
					while(pstPrvDiscDataWriter->pNext != pstDiscDataWriter)
						pstPrvDiscDataWriter = pstPrvDiscDataWriter->pNext;
					
					pstPrvDiscDataWriter->pNext = pstDiscDataWriter->pNext;
				}
			}

			pstDiscDataWriter = pstDiscDataWriter->pNext;
		}
		pstTopic = pstTopic->pNext;
	}

	return 0;
}


DDS_ReturnCode_t DeadlineCheck(DDS_DomainParticipant* pstParticipant)
{
#if defined(_VXWORKS_653) || defined (_TM1) || defined _TM3
    /* 653任务无法传参，使用全局变量 */
	pstParticipant = pstRealDomainParticipant;
#endif
	
	DDS_Topic* pstTopic = pstParticipant->pstTopic;

	Time_t nowTime;
    DDS_DataReader* pstDataReader;
    DDS_DataWriter* pstDataWriter;
	printf("start DeadlineCheck ..\n");

	while(TRUE)
	{
		pstTopic = pstParticipant->pstTopic;
		
		while (NULL != pstTopic)
		{
			nowTime = GetNowTime();
			pstDataReader = pstTopic->pstLocalReader;
	
			while(NULL != pstDataReader)
			{	
				/* 未设置deadline 忽略  */
				if ( 0x7fffffff == pstDataReader->stDataReaderQos.deadline.period.sec && 0xffffffff == pstDataReader->stDataReaderQos.deadline.period.nanosec)
				{
					pstDataReader = pstDataReader->pNext;
					continue;
				}
		
				//printf("R:DeadLine Topic %s lastTime %d:%d nowTime %d:%d \n",pstTopic->topicName.value,pstDataReader->lastDataTime.sec,pstDataReader->lastDataTime.nanosec,nowTime.sec,nowTime.nanosec);
				if (FALSE == Compare_Duration(Add_Duration(pstDataReader->lastDataTime, pstDataReader->stDataReaderQos.deadline.period), nowTime))
				{
					printf("DataReader Deadline !!! \n");
					if(NULL != pstDataReader->deadlineCallBack)
						pstDataReader->deadlineCallBack(&nowTime);
					pstDataReader->lastDataTime = nowTime;
				}
				
				pstDataReader = pstDataReader->pNext;
			}
			
			pstDataWriter = pstTopic->pstLocalWriter;
			
			while(NULL != pstDataWriter)
			{	
				/* 未设置deadline 忽略  */
				if ( 0x7fffffff == pstDataWriter->stDataWriterQos.deadline.period.sec && 0xffffffff == pstDataWriter->stDataWriterQos.deadline.period.nanosec)
				{
					pstDataWriter = pstDataWriter->pNext;
					continue;
				}
				
				//printf("W:DeadLine Topic %s lastTime %d:%d nowTime %d:%d \n",pstTopic->topicName.value,pstDataWriter->lastWriteTime.sec,pstDataWriter->lastWriteTime.nanosec,nowTime.sec,nowTime.nanosec);
				if (FALSE == Compare_Duration(Add_Duration(pstDataWriter->lastWriteTime, pstDataWriter->stDataWriterQos.deadline.period), nowTime))
				{
					printf("DataWriter Deadline !!!\n");
					if(NULL != pstDataWriter->writerCallBack)
						pstDataWriter->writerCallBack(&nowTime);
					pstDataWriter->lastWriteTime = nowTime;
				}
				
				pstDataWriter = pstDataWriter->pNext;
			}	
			pstTopic = pstTopic->pNext;
		}
		QY_Sleep(100);
	}
}

VOID UserDataPeriodTask(DDS_DomainParticipant* pstParticipant)
{
	DDS_Topic* pstTopic;
	DDS_DataWriter* pstDataWriter;

	while (1)
	{
		QY_Sleep(10);
		pstTopic = pstParticipant->pstTopic;
		while (NULL != pstTopic)
		{
			pstDataWriter = pstTopic->pstLocalWriter;
			while (NULL != pstDataWriter)
			{
				SendHistoryData(pstDataWriter);

				pstDataWriter = pstDataWriter->pNext;
			}

			pstTopic = pstTopic->pNext;
		}

		//QY_Sleep(1000);
	}
}

#if defined _FC

int RecvAndParseFAllNetworkOrder(EX_INT32 kind,CHAR* buffer,DDS_DomainParticipant* pstParticipant)
{
	static int count = 0;
	/* 数据种类  0 SPDP数据 1 SEDP数据 2用户数据*/
	unsigned int length = NETWORK_BYTE_MAX_SIZE;
	int ret = 0;
	int i = 0;
	if(0 == kind)
	{
	    /* Nephalem 遍历DDS数据接收MsgID */ 
	    for(i = 0 ; i < g_commFCConfig.m_nodeNum;i++)
	    {    	
	    	length = NETWORK_BYTE_MAX_SIZE;
			ret = FC_NP_RecvMsg(g_commFCConfig.m_FCdev, g_commFCConfig.m_configs[i].m_localDDSRecvMsgID, buffer, &length);
	        if(iDDSRECV == 1)
	           printf("Nephalem FC Recv SPDP Data From %X Ret = %x , len = %d FC_Dev = %p \n",g_commFCConfig.m_configs[i].m_localDDSRecvMsgID,ret,length,g_commFCConfig.m_FCdev);
	        if (0 == ret && length > 0)
	        {
	        	//printf("Nephalem FC Recv SPDP Data From %X Ret = %x , len = %d FC_Dev = %p \n",g_commFCConfig.m_configs[i].m_localDDSRecvMsgID,ret,length,g_commFCConfig.m_FCdev);
	            ParseAllNetworkOrder(buffer, length, pstParticipant);  
	        }
	    }
	}
	
	if(1 == kind)
	{
		/* Nephalem 遍历SEDP数据接收MsgID */ 
		for(i = 0 ; i < g_commFCConfig.m_nodeNum;i++)
		{			
			length = NETWORK_BYTE_MAX_SIZE;
			ret = 0;
			ret = FC_NP_RecvMsg(g_commFCConfig.m_FCdev, g_commFCConfig.m_configs[i].m_localSEPDRecvMsgID, buffer, &length);
			if(iDDSRECV == 1)
				printf("Nephalem FC Recv SEDP Data From %X Ret = %x , len = %d \n",g_commFCConfig.m_configs[i].m_localSEPDRecvMsgID,ret,length);
			if (0 == ret && length > 0)
			{
//				PrintBinData(buffer, length, "receive SEDP");
				//printf("Nephalem FC Recv SEDP Data From %X Ret = %x , len = %d \n",g_commFCConfig.m_configs[i].m_localSEPDRecvMsgID,ret,length);
				ParseAllNetworkOrder(buffer, length, pstParticipant);
				/* 当前通道接到数据后 i-1,下次继续读取该通道的数据 */
				i--;
			}
		}
	}
	
	if(2 == kind)
	{
		/* Nephalem 遍历用户数据接收MsgID */ 
		for(i = 0 ; i < g_commFCConfig.m_nodeNum;i++)
		{			
			length = NETWORK_BYTE_MAX_SIZE;
			ret = 0;
			ret = FC_NP_RecvMsg(g_commFCConfig.m_FCdev, g_commFCConfig.m_configs[i].m_localUserRecvMsgID, buffer, &length);
			if(iUSERRECV == 1)
				printf("Nephalem FC Recv User Data From %X Ret = %x , len = %d \n",g_commFCConfig.m_configs[i].m_localUserRecvMsgID,ret,length);
			if (0 == ret && length > 0)
			{
				if(iUSERRECV == 2)
					PrintBinData(buffer,length, "USER_DATA");
				//printf("Nephalem FC Recv User Data From %X Ret = %x , len = %d \n",g_commFCConfig.m_configs[i].m_localUserRecvMsgID,ret,length);
				ParseAllNetworkOrder(buffer, length, pstParticipant);
				/* 当前通道接到数据后 i-1,下次继续读取该通道的数据 */
				i--;
			}
		
		}
	}
	
	return 0;
	
}
//void FCInit()
//{
//	int i = 0;
//	int ret = 0;
//    /* 加载FC驱动 */
//    QoriqPexConfigLibInit(3);
//    taskDelay(60);
//    sysPciExpressAutoConfig(3);
//    taskDelay(60);
//    /* Bypass PAMU to enable pciex in */
//    *(UINT32*) DCFG_CCSR_PAMUBYPENR = (UINT32) 0xFF000000;
//    
//    for (i = 0U; i < g_commFCConfig.m_fcNum; ++i)
//    {
//    	if (g_commFCConfig.m_deviceFCId[i] == g_commFCConfig.m_fcId)
//    	{
//    		g_commFCConfig.m_fcSeq = i;
//    		break;
//    	}
//    }
//    
//	/*查询设备*/
//    i = g_commFCConfig.m_fcNum;
//	ret = FC_NP_DevQuery(&i);
//	if (ret)
//	{
//		printf("FC_NP_DevQuery failed ret=0x%08x \n", ret);
//		return NULL;
//	}
////	if (i == 0)
////	{
////		YLogWarning("query count = 0");
////		return NULL;
////	}
//	/*打开设备*/
//	ret = FC_NP_DevOpen(&FCdev, 0);
//	if (ret)
//	{
//		printf("FC_NP_DevOpen failed ret=0x%08x \n", ret);
//		return NULL;
//	}
//	FC_BLUE_PRINT gBluePrint;
//	
//	/*使用应用定义的蓝图*/
//    /* 初始化蓝图数据 */
//    gBluePrint.blue_print_header.file_type = 0;
//    gBluePrint.blue_print_header.length = 0;
//    gBluePrint.blue_print_header.version[0] = 0;
//    gBluePrint.blue_print_header.date = 0;
//    gBluePrint.blue_print_header.crc = 0;
//    gBluePrint.blue_print_header.lrm_sig = 0;
//    gBluePrint.blue_print_header.lrm_num = 0;
//    gBluePrint.blue_print_header.lrm_id = 0;
//    gBluePrint.blue_print_header.soft_version[0] = 0;
//    gBluePrint.config_block_num = 1;
//    gBluePrint.config_block[0].config_block_no = 0;
//    gBluePrint.config_block[0].scheme_num = 1;
//    gBluePrint.config_block[0].scheme[0].scheme_no = 0;
//    gBluePrint.config_block[0].scheme[0].port_config.port_id = g_commFCConfig.m_fcId;
//    sprintf(gBluePrint.config_block[0].scheme[0].port_config.port_name, "FC_%u", g_commFCConfig.m_fcId);
//    gBluePrint.config_block[0].scheme[0].port_config.nm_role = g_commFCConfig.m_role;//g_commData->m_isHost[model->m_netType] ? FC_NET_ROLE_NCT : (g_commData->m_isBakHost ? FC_NET_ROLE_BNCT : FC_NET_ROLE_NRT);
//    gBluePrint.config_block[0].scheme[0].port_config.tm_role = FC_TIME_ROLE_CCS;
//    gBluePrint.config_block[0].scheme[0].port_config.block_txmsg_num = g_commFCConfig.m_fcNum;
//    gBluePrint.config_block[0].scheme[0].port_config.block_rxmsg_num = g_commFCConfig.m_fcNum;
//    gBluePrint.config_block[0].scheme[0].port_config.stream_txmsg_num = g_commFCConfig.m_fcNum;
//    gBluePrint.config_block[0].scheme[0].port_config.stream_rxmsg_num = g_commFCConfig.m_fcNum;
//    if (g_commFCConfig.m_role == FC_NET_ROLE_NCT)
//    {
//        gBluePrint.config_block[0].scheme[0].port_config.wdt_init_value = 500;
//        gBluePrint.config_block[0].scheme[0].port_config.wdt_period = 250;
//        gBluePrint.config_block[0].scheme[0].port_config.wdt_loader_period = 50;
//        gBluePrint.config_block[0].scheme[0].port_config.clk_sync_period = 50;
//    }
//    else
//    {
//        gBluePrint.config_block[0].scheme[0].port_config.wdt_init_value = 1500;
//        gBluePrint.config_block[0].scheme[0].port_config.wdt_period = 500;
//        gBluePrint.config_block[0].scheme[0].port_config.wdt_loader_period = 50;
//        gBluePrint.config_block[0].scheme[0].port_config.clk_sync_period = 50;
//    }
//    printf("FC role = %u \n", gBluePrint.config_block[0].scheme[0].port_config.nm_role);
//    /* 构造所有收发通道 */
//    for (i = 0U; i < g_commFCConfig.m_fcNum; ++i)
//    {
//    	printf("FCID[%u] = 0x%X \n", i, g_commFCConfig.m_deviceFCId[i]);
//    	printf("g_commFCConfig.m_fcSeq =%d and msg_id = %x \n",g_commFCConfig.m_fcSeq,(g_commFCConfig.m_fcSeq << 8U) | i);
//        /* 发送普通消息 */
//        gBluePrint.config_block[0].scheme[0].send_item[i].msg_id = (g_commFCConfig.m_fcSeq << 8U) | i;
//        sprintf(gBluePrint.config_block[0].scheme[0].send_item[i].msg_name, "N_%u_%u", g_commFCConfig.m_fcSeq, i);
//        gBluePrint.config_block[0].scheme[0].send_item[i].msg_max_len = 2096;
//        gBluePrint.config_block[0].scheme[0].send_item[i].msg_src_id = g_commFCConfig.m_fcId;
//        gBluePrint.config_block[0].scheme[0].send_item[i].msg_dst_id = g_commFCConfig.m_deviceFCId[i];
//        gBluePrint.config_block[0].scheme[0].send_item[i].msg_type = FC_MSG_TYPE_EVENT;
//        /* 发送流消息 */
//        gBluePrint.config_block[0].scheme[0].send_item[i + g_commFCConfig.m_fcNum].msg_id = 0x10000U | ((g_commFCConfig.m_fcSeq << 8U) | i);
//        sprintf(gBluePrint.config_block[0].scheme[0].send_item[i + g_commFCConfig.m_fcNum].msg_name, "S_%u_%u", g_commFCConfig.m_fcSeq, i);
//        gBluePrint.config_block[0].scheme[0].send_item[i + g_commFCConfig.m_fcNum].msg_max_len = 0x00100000U;
//        gBluePrint.config_block[0].scheme[0].send_item[i + g_commFCConfig.m_fcNum].msg_src_id = g_commFCConfig.m_fcId;
//        gBluePrint.config_block[0].scheme[0].send_item[i + g_commFCConfig.m_fcNum].msg_dst_id = g_commFCConfig.m_deviceFCId[i];
//        gBluePrint.config_block[0].scheme[0].send_item[i + g_commFCConfig.m_fcNum].msg_type = FC_MSG_TYPE_STREAM;
//        /* 接收普通消息 */
//        gBluePrint.config_block[0].scheme[0].recv_item[i].msg_id = (i << 8U) | g_commFCConfig.m_fcSeq;
//        sprintf(gBluePrint.config_block[0].scheme[0].recv_item[i].msg_name, "N_%u_%u", i, g_commFCConfig.m_fcSeq);
//        gBluePrint.config_block[0].scheme[0].recv_item[i].msg_max_len = 2096;
//        gBluePrint.config_block[0].scheme[0].recv_item[i].msg_type = FC_MSG_TYPE_EVENT;
//        gBluePrint.config_block[0].scheme[0].recv_item[i].msg_recv_mode = FC_MSG_RECV_MODE_POLL;
//        gBluePrint.config_block[0].scheme[0].recv_item[i].msg_src_id = g_commFCConfig.m_deviceFCId[i];
//        gBluePrint.config_block[0].scheme[0].recv_item[i].msg_recover = 0U;
//        /* 接收流消息 */
//        gBluePrint.config_block[0].scheme[0].recv_item[i + g_commFCConfig.m_fcNum].msg_id = 0x10000U | ((i << 8U) | g_commFCConfig.m_fcSeq);
//        sprintf(gBluePrint.config_block[0].scheme[0].recv_item[i + g_commFCConfig.m_fcNum].msg_name, "S_%u_%u", i, g_commFCConfig.m_fcSeq);
//        gBluePrint.config_block[0].scheme[0].recv_item[i + g_commFCConfig.m_fcNum].msg_max_len = 0x00100000U;
//        gBluePrint.config_block[0].scheme[0].recv_item[i + g_commFCConfig.m_fcNum].msg_type = FC_MSG_TYPE_STREAM;
//        gBluePrint.config_block[0].scheme[0].recv_item[i + g_commFCConfig.m_fcNum].msg_recv_mode = FC_MSG_RECV_MODE_POLL;
//        gBluePrint.config_block[0].scheme[0].recv_item[i + g_commFCConfig.m_fcNum].msg_src_id = g_commFCConfig.m_deviceFCId[i];
//        gBluePrint.config_block[0].scheme[0].recv_item[i + g_commFCConfig.m_fcNum].msg_recover = 0U;
//    }
//    
//	ret = FC_NP_SetBluePrint(FCdev, &gBluePrint);
//	if (ret)
//	{
//		printf("FC_NP_SetBluePrint failed ret=0x%08x \n", ret);
//		return NULL;
//	}
//	/*配置设备*/
//	ret = FC_NP_LoadConfig(FCdev, 0, 0);
//	if (ret)
//	{
//		printf("FC_NP_LoadConfig failed ret=0x%08x \n", ret);
//		return NULL;
//	}
//	
//	if (g_commFCConfig.m_role == FC_NET_ROLE_NCT)
//	{
//		/*设置WDT定时器使能*/
//		ret = FC_NM_SetWdtPeriodEnab(FCdev, FC_WDT_PERIOD_ENAB);
//		if (ret)
//		{
//			printf("FC_NM_SetWdtPeriodEnab failed ret=0x%08x \n", ret);
//		}
//		else
//		{
//			printf("FC_NM_SetWdtPeriodEnab success \n");
//		}
//	}
//    
//}

//int FCIsReady()
//{
//	UINT32 ret = 0U;
//	UINT32 value = 0U;
//
//	static int flag = 0;
//	
//	if(1 == flag)
//		return 1;
//	/*获取端口状态*/
//	ret = FC_NP_GetPortStatus(FCdev, &value);
//	/* 如果获取端口状态失败，或者端口没有上线，则直接返回 */
//	if (ret || value == 0)
//	{
//		printf("FC_NP_GetPortStatus portStatus=%d ret = %d \n", value, ret);
//		return 0;
//	}
//	
//	/* 如果不是主控节点，则需要申请上线 */
//	if (g_commFCConfig.m_role != FC_NET_ROLE_NCT)
//	{
//		/*查询NCT是否在线*/
//		ret = FC_NM_NctIsOnLine(FCdev, &value);
//		if (ret || value == 0)
//		{
//    		printf("FC_NM_NctIsOnLine online=%d ret = %d \n", value, ret);
//    		return 0;
//		}
//		printf("FC_NM_NctIsOnLine online=%d ret = %d \n", value, ret);
//		/*上网请求*/
//		ret = FC_NM_OnLineRequest(FCdev);
//		if (ret)
//		{
//    		printf("FC_NM_OnLineRequest failed ret=0x%08x \n", ret);
//    		return 0;
//		}
//		printf("FC_NM_OnLineRequest ret=0x%08x \n", ret);
//	}
//	printf("FC port online \n");
//
//	flag = 1;
//	
//	return 1;
//}
#elif defined _653_PORT
void PortInit()
{
	RETURN_CODE_TYPE retCode;
	
	int i = 0;
	for(i = 0 ; i < g_PortConfig.SendPortNum ; i++)
	{
		CREATE_QUEUING_PORT(g_PortConfig.SendPort[i].portName, 2096, 512, SOURCE, 1, &g_PortConfig.SendPort[i].portID, &retCode);

		if (retCode != 0)
		{
			printf("create send port %s failed : %d", g_PortConfig.SendPort[i], retCode);
		}
	}
	
	for(i = 0 ; i < g_PortConfig.recvPortNum ; i++)
	{
		CREATE_QUEUING_PORT(g_PortConfig.recvPort[i].portName, 2096, 512, DESTINATION, 1, &g_PortConfig.recvPort[i].portID, &retCode);

		if (retCode != 0)
		{
			printf("create receive port %s failed : %d \n", g_PortConfig.recvPort[i], retCode);
		}
	}
}
#endif
