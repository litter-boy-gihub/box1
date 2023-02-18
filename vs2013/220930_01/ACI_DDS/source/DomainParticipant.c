#include "../include/GlobalDefine.h"

#define HEART_TIME 1000

static int init_num = 0;
BOOL g_RapdIOValid = FALSE;
BOOL g_ConfValid = FALSE;
DDS_MUTEX g_sendMutex;  //�̼߳���
/*ǰ������*/
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
DDS_DomainParticipant *g_pDomainParticipant = NULL; //����һ��Ӧ��ֻ�ܴ���һ��������
#endif //  STATIC_DISCOVERY

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_ParticipantFactory_init
-   ��������: ��Դ��ʼ��
-   ��    ��: ��
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��Դ��ʼ��
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
=======================================================
-   2019\7\23       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL VOID DDS_ParticipantFactory_init()
{
    if (0 == init_num)
    {
		DDS_MUTEX_INIT(g_sendMutex);
        /* ��־ģ���ʼ�� */
        InitCommLog();

        /* �����б��ʼ�� */
        InitializeSubTasks();

#if defined(USE_STATIC_MALLOC) || defined(USE_STATIC_MALLOC_T)
		/* ��̬�ڴ��ʼ�� */
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
-   �� �� ��: InitDomainParticipant
-   ��������: ��ʼ��participant
-   ��    ��: participantָ��
-   ��    ��: 
-   ȫ�ֱ���: 
-   ע    ��: ��ʼ��participant��Ա
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
        /* ��ʼ��Ĭ��qos */
        InitialDefaultQos();

        /* ��ʼ������ʱ�� */
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

       
        /* �ڽ���д����ʼ�� */
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
-   �� �� ��: DDS_DomainParticipantFactory_create_participant
-   ��������: ����participant
-   ��    ��: domainId��participantd��qos
-   ��    ��: participantָ��
-   ȫ�ֱ���: g_domainParticipant
-   ע    ��: ����participant
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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

	/* �����������Ч�Լ�� */
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
			/* TODO����ȫ�����õ�IP��ַ��ֵ�����ַ�ʽ���������������ߣ������˲�ͬ��ַ����������⣬LJM */
			strncpy(g_ipAddr, pstDomPartQos->property.ipAddr, strlen(pstDomPartQos->property.ipAddr));
		}

		if (strcmp(pstDomPartQos->property.netCardName, DDS_PARTICIPANT_QOS_DEFAULT.property.netCardName) != 0)
			strncpy(g_netCardName, pstDomPartQos->property.netCardName, strlen(pstDomPartQos->property.netCardName));
	}

#if defined(_VXWORKS_653)
    /* 653�����޷����Σ�ʹ��ȫ�ֱ��� */
	if(NULL != pstRealDomainParticipant)
		return pstRealDomainParticipant;
#endif
    
    /* �����ڴ洴��Participant */
    pstDomainParticipant = (DDS_DomainParticipant*)DDS_STATIC_MALLOC(sizeof(DDS_DomainParticipant));
    if (NULL == pstDomainParticipant)
    {
        PrintLog(COMMLOG_ERROR, "DomainParticipant: Failed to create Participant, Insufficient memory space.\n");
        return NULL;
    }
#if defined(_VXWORKS_653) || defined (_TM1) || defined _TM3
    /* 653�����޷����Σ�ʹ��ȫ�ֱ��� */
    pstRealDomainParticipant = pstDomainParticipant;
#endif
    /* ���뾲̬�ڴ洴��HistoryData����ڵ� */
    pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC(sizeof(HistoryData));
    if (NULL == pstHistoryData)
    {
        DDS_STATIC_FREE(pstDomainParticipant);
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData, Insufficient memory space.\n");
        return NULL;
    }

	/* Nephalem ���г�ʼ���������ͷŻ���� */
	InitHistoryData(pstHistoryData);

    /* ���뾲̬�ڴ洴����ʷ�������� */
    pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC(SEDP_BYTE_MAX_SIZE);
    if (NULL == pstHistoryData->data)
    {
        /* ʧ���ͷ�ǰ��������ڴ� */
        DDS_STATIC_FREE(pstDomainParticipant);
        DDS_STATIC_FREE(pstHistoryData);
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData buff, Insufficient memory space.\n");
        return NULL;
    }

    /* ��ʼ��Participant�����л��ʼ��Ĭ��QoS */
    InitDomainParticipant(pstDomainParticipant);

    pstDomainParticipant->processId = getProcessId();
    /* �����ʼ�� */
    if (!InitNetWorkStructure(pstDomainParticipant, domainId))
    {
		DDS_STATIC_FREE(pstHistoryData->data);
		DDS_STATIC_FREE(pstHistoryData);
		DDS_STATIC_FREE(pstDomainParticipant);
        PrintLog(COMMLOG_ERROR, "DomainParticipant: Failed to initialize NetWork!\n");
        return NULL;
    }

    /* �����ֽ����ʼ�������淢������ */
    INITIAL_MEMORYBLOCK(stMemBlock, pstHistoryData->data, SEDP_BYTE_MAX_SIZE);

    pstDomainParticipant->dcpsParticipantData.domainID = domainId;

    /* ÿ����һ�����ز����ߣ��ڽ�seqNum�Լ�1*/
    pstDomainParticipant->stBuiltinDataWriter[0].seqNum = Add_SequenceNumber(pstDomainParticipant->stBuiltinDataWriter[0].seqNum, SEQUENCENUMBER_START);

    /* ������seqNum��� */
    pstHistoryData->seqNum = pstDomainParticipant->stBuiltinDataWriter[0].seqNum;
#ifdef QYDDS_LIVELINESS_QOS
	if (pstDomPartQos != NULL)
	{
		pstDomainParticipant->pstParticipantQos.liveliness = pstDomPartQos->liveliness;
		pstDomainParticipant->livelinessDuration = pstDomPartQos->liveliness.lease_duration;
	}
#endif
    /* ���л�spdp���� */
    ParticipantConvertNetworkByteOrder(pstDomainParticipant, &stMemBlock);

    /* ���ĳ��Ȳ��� */
    pstHistoryData->uiDataLen = stMemBlock.writeIndex;

    /* ���뵽�ڽ�����д�������� */
    InsertHistoryCacheTail(&pstDomainParticipant->stBuiltinDataWriter[0].stHistoryCache, pstHistoryData);

 #ifdef RAPID_IO_CHANNEL
     HANDLE rioHandle = CreateThread(NULL, 0, (VOID*)&NetworkTransceiverTask, (VOID*)pstDomainParticipant, 0, NULL);
#ifdef QYDDS_LIVELINESS_QOS
     /*TODO��Զ��������ⱨ��*/
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

	AddStaticDiscDomainParticipant(pstDomainParticipant); //��̬Զ�˽ڵ�����
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
-   �� �� ��: DDS_DomainParticipant_ignore_participant
-   ��������: ��ֹparticipant���ػػ�
-   ��    ��: pstDomainParticipan
-   ��    ��: ��
-   ȫ�ֱ���:
-   ע    ��: ��ֹparticipant���ػػ�
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
=======================================================
-   2020\03\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL VOID DDS_DomainParticipant_not_ignore_participant(DDS_DomainParticipant* pstDomainParticipant)
{
    pstDomainParticipant->bLoopBack = TRUE;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_DomainParticipant_ignore_participant
-   ��������: ��ֹparticipant���ػػ�
-   ��    ��: pstDomainParticipan
-   ��    ��: ��
-   ȫ�ֱ���:
-   ע    ��: ��ֹparticipant���ػػ�
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
=======================================================
-   2020\03\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL VOID DDS_DomainParticipant_ignore_participant(DDS_DomainParticipant* pstDomainParticipant)
{
    pstDomainParticipant->bLoopBack = FALSE;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_DomainParticipantFactory_delete_participant
-   ��������: ɾ��participant
-   ��    ��: pstDomainParticipan
-   ��    ��: ������
-   ȫ�ֱ���: 
-   ע    ��: ɾ��participant
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
        /* ���г�Աָ������ */
        
		LIST_DELETE_ALL(pstDomainParticipant->rtpsParticipantProxy.metatrafficUnicastLocator, pstTempNode);
		LIST_DELETE_ALL(pstDomainParticipant->rtpsParticipantProxy.metatrafficMulticastLocator, pstTempNode);
		LIST_DELETE_ALL(pstDomainParticipant->rtpsParticipantProxy.defaultUnicastLocator, pstTempNode);
		LIST_DELETE_ALL(pstDomainParticipant->rtpsParticipantProxy.defaultMulticastLocator, pstTempNode);

        /* ����Զ�˲����� */
        pstDiscParticipant = pstDomainParticipant->pstDiscParticipant;
        while (NULL != pstDiscParticipant)
        {
            pstDomainParticipant->pstDiscParticipant = pstDiscParticipant->pNext;
            DDS_DiscoveredParticipant_Uninit(pstDiscParticipant);
            DDS_STATIC_FREE(pstDiscParticipant);
            pstDiscParticipant = pstDomainParticipant->pstDiscParticipant;
        }

        /* ������������ */
        pstTopic = pstDomainParticipant->pstTopic;
        while (NULL != pstTopic)
        {
            pstDomainParticipant->pstTopic = pstTopic->pNext;
            DDS_Topic_Uninit(pstTopic);
            DDS_STATIC_FREE(pstTopic);
            pstTopic = pstDomainParticipant->pstTopic;
        }

        /* �����ڽ�ʵ�� */
        for (uiBuiltinNun = 0; uiBuiltinNun < 3; uiBuiltinNun++)
        {
            /* �ڽ�д������� */
            UnInitBuiltinDataWriter(&pstDomainParticipant->stBuiltinDataWriter[uiBuiltinNun]);

            /* �ڽ��Ķ������ */
            UnInitBuiltinDataReader(&pstDomainParticipant->stBuiltinDataReader[uiBuiltinNun]);
        }

		/* Nephalem �ر�Socket�������ڴ�й¶ */
		UnInitNetWorkStructure(pstDomainParticipant);

		CloseNetworkSendReceiveTask(pstDomainParticipant);

        DDS_MUTEX_UNLOCK(pstDomainParticipant->threadMutex);

        //�ȴ�һ�ٺ���������
        QY_Sleep(100);
        DDS_STATIC_FREE(pstDomainParticipant);
    }

    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_DomainParticipant_get_default_topic_qos
-   ��������: ��ȡ�������Ĭ��qos
-   ��    ��: participant������qos
-   ��    ��: ������
-   ȫ�ֱ���:
-   ע    ��: ͨ��participant��ȡһ��Ĭ������qos
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
-   �� �� ��: DDS_DomainParticipant_get_default_topic_qos
-   ��������: ��ȡ�������Ĭ��qos
-   ��    ��: participant������qos
-   ��    ��: ������
-   ȫ�ֱ���:
-   ע    ��: ͨ��participant��ȡһ��Ĭ������qos
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
-   �� �� ��: DDS_Participant_bind_network
-   ��������: ������߰�����
-   ��    ��: ������qos
-   ��    ��: ��
-   ȫ�ֱ���:
-   ע    ��: ������Ip
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
=======================================================
-   2020\03\17       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL VOID DDS_Participant_bind_network(DDS_DomainParticipantQos* pstDomPartQos, const CHAR* pcIpAddr)
{
	strncpy(pstDomPartQos->property.ipAddr, pcIpAddr, strlen(pcIpAddr));
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_DomainParticipant_create_topic
-   ��������: ��������
-   ��    ��: participant�����������������͡�����qos
-   ��    ��: ������Ϣ��ָ��
-   ȫ�ֱ���: 
-   ע    ��: ͨ��participant�������⣬������������򷵻ؿ�ָ�룬���������½����Ⲣ��ӵ�participant�µ�����������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
=======================================================
-   2019\7\23       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_Topic*  DDS_DomainParticipant_create_topic(DDS_DomainParticipant* pstParticipant, const char* pcTopicName, const char* pcTopicType, DDS_TopicQos* pstTopicQos)
{
    DDS_Topic* pstTopic = pstParticipant->pstTopic;
    while (NULL != pstTopic)
    {
		//LJM������˵ǰ��topicQoSû�����ã����Ե�Զ����ʱ��û�б�������Topic��Ϣ��һ����˵����topic���Ƚ���������������⡣�������ǲ��Ե�
        if (!strcmp(pstTopic->topicName.value, pcTopicName))
        {
            /* ������Զ���Ѵ��������⣬����һ�£�ֱ�ӷ��ظ����� */
            if (Guid_Is_Equal(&pstTopic->tpGuid, &GUID_UNKNOWN) && !strcmp(pstTopic->topicType.value, pcTopicType))
            {
                /* ���ɸ����Ȿ��GUID������ͨ��GUID���ж��Ǳ��ػ���Զ������ */
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

    /* �����ڴ洴������ */
    pstTopic = (DDS_Topic*)DDS_STATIC_MALLOC(sizeof(DDS_Topic));
    if (NULL == pstTopic)
    {
        PrintLog(COMMLOG_ERROR, "DomainParticipant_create_topic: Failed to create topic, Insufficient memory space.\n");
        return NULL;
    }

    /* ��ʼ������ */
    DDS_Topic_Init(pstTopic);

    /* ���游�ڵ�ָ��  */
    pstTopic->pstParticipant = pstParticipant;

    if (NULL != pstTopicQos)
    {
        /* ��������qos */
        pstTopic->stTopicQos = *pstTopicQos;
    }
    else
    {
        /* Ĭ������ */
        pstTopic->stTopicQos = DDS_TOPIC_QOS_DEFAULT;
    }

	/* ��������GUID */
	GenGuid(&pstTopic->tpGuid, &pstParticipant->stRTPSMsgHeader.guidPrefix, ENTITYKIND_TOPIC);
	
	/* ������������ */
	pstTopic->topicName.length = (UINT32)(strlen(pcTopicName) + 1);
#if defined(_WIN32)
	strcpy_s(pstTopic->topicName.value, pstTopic->topicName.length, pcTopicName);
#elif defined(__linux__)
    strncpy(pstTopic->topicName.value, pcTopicName, pstTopic->topicName.length);
#elif defined (_VXWORKS)
    memcpy(pstTopic->topicName.value,pcTopicName,pstTopic->topicName.length);
#endif
    /* ������������ */
    pstTopic->topicType.length = (UINT32)(strlen(pcTopicType) + 1);

#if defined(_WIN32)
    strcpy_s(pstTopic->topicType.value, pstTopic->topicType.length, pcTopicType);
#elif defined(__linux__)
    strncpy(pstTopic->topicType.value, pcTopicType, pstTopic->topicType.length);
#elif defined (_VXWORKS)
    memcpy(pstTopic->topicType.value, pcTopicType, pstTopic->topicType.length);
#endif
    /* ��������뵽Participant�µ��������� */
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
-   �� �� ��: DDS_DomainParticipant_get_topic_num
-   ��������: ��ȡ�������
-   ��    ��: participant
-   ��    ��: �������
-   ȫ�ֱ���:
-   ע    ��: ��ȡ�������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
-   �� �� ��: DDS_DomainParticipant_delete_topic
-   ��������: ɾ��ָ���������µ�����
-   ��    ��: participant������
-   ��    ��: ������
-   ȫ�ֱ���:
-   ע    ��: ɾ��ָ���������µ�����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
-   �� �� ��: DDS_DomainParticipant_delete_topic_by_name
-   ��������: ͨ����������ɾ������
-   ��    ��: participant��������
-   ��    ��: ������
-   ȫ�ֱ���:
-   ע    ��: ͨ����������ɾ������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
-   �� �� ��: DDS_DomainParticipant_get_default_topic_qos
-   ��������: ��ȡ����Ĭ��qos
-   ��    ��: participant������qos
-   ��    ��: ������
-   ȫ�ֱ���:
-   ע    ��: ͨ��participant��ȡһ��Ĭ������qos
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
-   �� �� ��: DDS_DomainParticipant_get_default_topic_qos
-   ��������: ��ȡ����Ĭ��qos
-   ��    ��: participant������qos
-   ��    ��: ������
-   ȫ�ֱ���:
-   ע    ��: ͨ��participant��ȡһ��Ĭ������qos
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
-   �� �� ��: GetBuiltinWriterFromParticipantByEntityId
-   ��������: �����ڽ�д����
-   ��    ��: Participant��EntityId
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��Participant��ͨ��EntityId������Ӧ�ڽ�д����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
-   �� �� ��: GetBuiltinReaderFromParticipantByEntityId
-   ��������: �����ڽ��Ķ���
-   ��    ��: Participant��EntityId
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��Participant��ͨ��EntityId������Ӧ�ڽ��Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
-   �� �� ��: GetDiscoveredWriterFromParticipantByEntityId
-   ��������: ����д����
-   ��    ��: Participant��Guid
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��Participant��ͨ��Guid������ӦԶ��д����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\8       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DiscoveredWriter* GetDiscoveredWriterFromParticipantByGuid(DDS_DomainParticipant* pstDomainParticipant, GUID_t* pstGuid)
{
    DDS_Topic* pstTopic = NULL;
    DDS_DiscoveredWriter* pstDiscWriter = NULL;

    if (NULL != pstDomainParticipant)
    {
        /* ����ÿһ������ */
        pstTopic = pstDomainParticipant->pstTopic;
        while (NULL != pstTopic)
        {
            /* ���������µ�ÿ��Զ��д���� */
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
-   �� �� ��: GetDiscoveredReaderFromParticipantByEntityId
-   ��������: ����Զ����
-   ��    ��: Participant��Guid
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��Participantͨ��Guid������ӦԶ���Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\8       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DiscoveredReader* GetDiscoveredReaderFromParticipantByGuid(DDS_DomainParticipant* pstDomainParticipant, GUID_t* pstGuid)
{
    DDS_Topic* pstTopic = NULL;
    DDS_DiscoveredReader* pstDiscReader = NULL;

    if (NULL != pstDomainParticipant)
    {
        /* ����ÿһ������ */
        pstTopic = pstDomainParticipant->pstTopic;
        while (NULL != pstTopic)
        {
            /* ���������µ�ÿ��Զ���Ķ��� */
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
-   �� �� ��: GetLocalWriterFromParticipantByEntityId
-   ��������: ���ұ���д����
-   ��    ��: Participant��EntityId
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��Participantͨ��EntityId������Ӧд����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\8       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DataWriter* GetLocalWriterFromParticipantByEntityId(DDS_DomainParticipant* pstDomainParticipant, EntityId_t* pstEntityId)
{
    DDS_Topic* pstTopic = NULL;
    DDS_DataWriter* pstLocalWriter = NULL;

    if (NULL != pstDomainParticipant)
    {
        /* ����ÿһ������ */
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
-   �� �� ��: GetLocalWriterFromParticipantByEntityId
-   ��������: �����Ķ���
-   ��    ��: Participant��EntityId
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��Participant��ͨ��EntityId������Ӧд����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\8       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DataReader* GetLocalReaderFromParticipantByEntityId(DDS_DomainParticipant* pstDomainParticipant, EntityId_t* pstEntityId)
{
    DDS_Topic* pstTopic = NULL;
    DDS_DataReader* pstLocalReader = NULL;

    if (NULL != pstDomainParticipant)
    {
        /* ����ÿһ������ */
        pstTopic = pstDomainParticipant->pstTopic;
        while (NULL != pstTopic)
        {
            pstLocalReader = pstTopic->pstLocalReader;
            while (NULL != pstLocalReader)
            {
                /* ���������µ�ÿ�������Ķ��� */
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
-   �� �� ��: HandleAckNackMsg
-   ��������: ����Ack����
-   ��    ��: ack��Ϣ���ڴ�顢Participant
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��������Ack����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\9       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL HandleBuiltinAckNackMsg(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader* pstSubMsgHeader, AckNack* pstAckNackMsg, DDS_DomainParticipant* pstParticipant)
{
	BuiltinDataWriter* pstBuiltinDataWriter = NULL;
	BuiltinDiscReader* pstBuiltinDiscReader = NULL;
	DDS_DiscoveredParticipant* pstDiscParticipant = NULL;
    /* ���Ҹ�EntityId���ڵı���д���� */
    pstBuiltinDataWriter = GetBuiltinWriterFromParticipantByEntityId(pstParticipant, &pstAckNackMsg->writerId);
    if (NULL == pstBuiltinDataWriter)
    {
        PrintLog(COMMLOG_ERROR, "HandleBuiltinAckNackMsg: Not find local data writer by entityId.\n");
        return FALSE;
    }

    /* ����ͬһ������Զ���Ķ���  */
    pstBuiltinDiscReader = GetBuiltinDiscReaderFromBuiltinDataWriterByPrefix(pstBuiltinDataWriter, &pstRTPSMsgHeader->guidPrefix);
    if (NULL == pstBuiltinDiscReader)
    {
        PrintLog(COMMLOG_ERROR, "HandleBuiltinAckNackMsg: Not find remote data reader by entityId.\n");
        return FALSE;
    }

    /* ÿ�յ�һ��ack���ģ�������������գ�����յĻ���ÿ����һ����������һ�Σ���һ����Ŀ���ж�û��ack�ظ����Զ��Ͽ���Զ���Ķ�����ɾ���� */
    pstBuiltinDiscReader->uiSendHeartBeatNum = 0;
    
//    if(0 == pstAckNackMsg->readerSNState.numBits)
//    {
//    	Time_t nowTime = GetNowTime();
//    	printf("%d.%d Nephalem Ack base = %d num = %d \n", nowTime.sec,nowTime.nanosec,pstAckNackMsg->readerSNState.base.low,pstAckNackMsg->readerSNState.numBits);
//    }
//    printf("Nephalem Ack base = %d num = %d \n ",pstAckNackMsg->readerSNState.base.low,pstAckNackMsg->readerSNState.numBits);
    
#ifdef QYDDS_LIVELINESS_QOS
	//�̲߳���ȫ
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
	
    /* ����ack���ģ�ֱ������ */
    //if (pstAckNackMsg->count <= pstBuiltinDiscReader->stAckNack.count)
    //{
    //    return TRUE;
    //}
    
    /* ����ack������� */
    pstBuiltinDiscReader->stAckNack = *pstAckNackMsg;

    /* ��ʧ��Ƭ�ش� */
    RecoverBuiltinWriterMissSeqNum(pstBuiltinDataWriter, pstBuiltinDiscReader, pstAckNackMsg);

    /* �������ã�ackΪ���һ�ζԴ˲��ٷ����� */
    if(pstBuiltinDiscReader->stAckNack.readerSNState.numBits != 0)
    	JointHeartbeatMsgSendAssignBuiltinReader(&pstBuiltinDataWriter->pstParticipant->stRTPSMsgHeader,pstBuiltinDataWriter, pstBuiltinDiscReader);
//    if(!pstSubMsgHeader->flags[1])
//    {
//        JointHeartbeatMsgSendAssignBuiltinReader(&pstBuiltinDataWriter->pstParticipant->stRTPSMsgHeader,pstBuiltinDataWriter, pstBuiltinDiscReader);
//    }

    return TRUE;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: HandleAckNackMsg
-   ��������: ����Ack����
-   ��    ��: ack��Ϣ���ڴ�顢Participant
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��������Ack����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\9       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL HandleAckNackMsg(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader* pstSubMsgHeader, AckNack* pstAckNackMsg, DDS_DomainParticipant* pstParticipant)
{
    GUID_t stDiscRdGuid;
	DDS_DataWriter* pstDataWriter = NULL;
	DDS_DiscoveredReader* pstDiscReader = NULL;
	DDS_LocalWriter* pstLocalWriter = NULL;

    /* ��ȡԶ��д����guid */
    stDiscRdGuid.prefix = pstRTPSMsgHeader->guidPrefix;
    stDiscRdGuid.entityId = pstAckNackMsg->readerId;

    /* ���Ҹ�EntityId���ڵı���д���� */
    pstDataWriter = GetLocalWriterFromParticipantByEntityId(pstParticipant, &pstAckNackMsg->writerId);
    if (NULL == pstDataWriter)
    {
        PrintLog(COMMLOG_WARN, "HandleAckNackMsg: Not find local data writer by entityId.\n");
        return FALSE;
    }

    /* ����д����Ϊ��״̬����ֱ�ӷ��أ��������ack���� */
    if (BEST_EFFORT_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
    {
        PrintLog(COMMLOG_ERROR, "HandleAckNackMsg: Local reader is stateless, ignore heartbeat.\n");
        return FALSE;
    }

    /* ����ͬһ������Զ���Ķ���*/
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

    /*  Զ���Ķ���Ϊ��״̬����ֱ�ӷ��أ��������ack���� */
    if (BEST_EFFORT_RELIABILITY_QOS == pstDiscReader->stDataReaderQos.reliability.kind)
    {
        PrintLog(COMMLOG_WARN, "HandleAckNackMsg: Remote writer is stateless, ignore heartbeat.\n");
        return FALSE;
    }

    /* ÿ�յ�һ��ack���ģ�������������գ�����յĻ���ÿ����һ����������һ�Σ���һ����Ŀ���ж�û��ack�ظ����Զ��Ͽ���Զ���Ķ�����ɾ���� */
    pstLocalWriter->uiSendHeartBeatNum = 0;

    /* ����ack���ģ�ֱ������ */
    //if (pstAckNackMsg->count <= pstLocalWriter->stAckNack.count)
    //{
    //    return TRUE;
    //}

    /* ����ack������� */
    pstLocalWriter->stAckNack.count = pstAckNackMsg->count;

    /* ����ÿ��Զ���Ķ����������¸�seqNum���Ա㱾�ػ������ */
    pstLocalWriter->stAckNack.readerSNState.base = pstAckNackMsg->readerSNState.base;

	/* ���ڻ������ */
	if (pstDataWriter->stDataWriterQos.durability.kind != TRANSIENT_DURABILITY_QOS)
	{
		DelDataWriterHistoryCache(pstDataWriter);
	}

    /* ��ʧ��Ƭ�ش� */
    RecoverWriterMissSeqNum(pstDataWriter, pstDiscReader, pstAckNackMsg);

    /* ack����������ȱʧ���ش����������������Ա����ݿ����ش� */
    if (!pstSubMsgHeader->flags[1])
    {
        JointHeartbeatMsgSendAssignReader(pstDataWriter, pstDiscReader);
    }

    return TRUE;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: HandleNackFragMsg
-   ��������: ����Ack����
-   ��    ��: NackFrag��Ϣ���ڴ�顢Participant
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��������NackFrag����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2021\4\14       1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
BOOL HandleNackFragMsg(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader* pstSubMsgHeader, NackFrag* pstNackFragMsg, DDS_DomainParticipant* pstParticipant)
{
    GUID_t stDiscRdGuid;
    DDS_DataWriter* pstDataWriter = NULL;
    DDS_DiscoveredReader* pstDiscReader = NULL;
    DDS_LocalWriter* pstLocalWriter = NULL;

    /* ��ȡԶ��д����guid */
    stDiscRdGuid.prefix = pstRTPSMsgHeader->guidPrefix;
    stDiscRdGuid.entityId = pstNackFragMsg->readerId;

    /* ���Ҹ�EntityId���ڵı���д���� */
    pstDataWriter = GetLocalWriterFromParticipantByEntityId(pstParticipant, &pstNackFragMsg->writerId);
    if (NULL == pstDataWriter)
    {
        PrintLog(COMMLOG_WARN, "HandleAckNackMsg: Not find local data writer by entityId.\n");
        return FALSE;
    }

    /* ����д����Ϊ��״̬����ֱ�ӷ��أ��������ack���� */
    if (BEST_EFFORT_RELIABILITY_QOS == pstDataWriter->stDataWriterQos.reliability.kind)
    {
        PrintLog(COMMLOG_ERROR, "HandleAckNackMsg: Local reader is stateless, ignore heartbeat.\n");
        return FALSE;
    }

    /* ����ͬһ������Զ���Ķ���*/
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

    /*  Զ���Ķ���Ϊ��״̬����ֱ�ӷ��أ��������ack���� */
    if (BEST_EFFORT_RELIABILITY_QOS == pstDiscReader->stDataReaderQos.reliability.kind)
    {
        PrintLog(COMMLOG_WARN, "HandleAckNackMsg: Remote writer is stateless, ignore heartbeat.\n");
        return FALSE;
    }

    /* Nephalem �ش�DataFrag����  */
    RecoverWriterMissDataFrag(pstDataWriter, pstDiscReader, pstNackFragMsg);

    return TRUE;
}

/*--------------------------------------------------------------------------------
-   �� �� ��: HandleBuiltinHeartbeatMsg
-   ��������: �ڽ�����������
-   ��    ��: �ӱ�����Ϣͷ���������ݡ�Participant
-   ��    ��: ����
-   ȫ�ֱ���:
-   ע    ��: �ڽ���������.�Ƿ���Ҫ�����ش�
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
    
    /* ��Ч���������账�� */
    if (ISLESSTHEN_SEQUENCENUMBER(pstHeartbeatMsg->lastSN, pstHeartbeatMsg->firstSN))
    {
        return TRUE;
    }
    
    /* ���ұ����ڽ��Ķ��� */
    pstBuiltinDataReader = GetBuiltinReaderFromParticipantByEntityId(pstParticipant, &pstHeartbeatMsg->writerId);
    if (NULL == pstBuiltinDataReader)
    {
        PrintLog(COMMLOG_ERROR, "HandleBuiltinHeartbeatMsg: Cannot discovered local dataWriter.\n");
        return FALSE;
    }
    /* ���ұ����ڽ��Ķ�����Ӧ��Զ���ڽ�д���� */
    pstBuiltinDiscWriter = GetBuiltinDiscWriterFromBuiltinDataReaderByPrefix(pstBuiltinDataReader, &pstRTPSMsgHeader->guidPrefix);
    if (NULL == pstBuiltinDiscWriter)
    {
        PrintLog(COMMLOG_ERROR, "HandleBuiltinHeartbeatMsg: Cannot discovered remote dataWriter.\n");
        return FALSE;
    }

//    printf("pstHeartbeatMsg->count %d  pstBuiltinDiscWriter->stHeartbeat.count %d \n",pstHeartbeatMsg->count,pstBuiltinDiscWriter->stHeartbeat.count);
    /* �������ڣ����账�� */
    if (pstHeartbeatMsg->count <= pstBuiltinDiscWriter->stHeartbeat.count)
    {
        return TRUE;
    }
    else
    {
        /* ��������ͬ�� */
        pstBuiltinDiscWriter->stHeartbeat.count = pstHeartbeatMsg->count;
    }

    /* ����ȱʧ�ڽ����ݰ� */
    GetBuiltinReaderMissSeqNum(pstBuiltinDataReader, pstBuiltinDiscWriter, pstHeartbeatMsg);

    /* ���л�ack���ķ��� */
    JointAckNackMsgSendAssignBuiltinWriter(pstBuiltinDataReader, pstBuiltinDiscWriter);

#ifdef QYDDS_LIVELINESS_QOS
	//Liveliness��������
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
-   �� �� ��: HandleHeartbeatMsg
-   ��������: ����������
-   ��    ��: �ӱ�����Ϣͷ���������ݡ�Participant
-   ��    ��: ����
-   ȫ�ֱ���:
-   ע    ��: ��������.�Ƿ���Ҫ�����ش�
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\14       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL HandleHeartbeatMsg(RTPSMessageHeader* pstRTPSMsgHeader, Heartbeat* pstHeartbeatMsg, DDS_DomainParticipant* pstParticipant)
{
    GUID_t stDiscWtGuid;
    DDS_DataReader* pstDataReader = NULL;
	DDS_DiscoveredWriter* pstDiscWriter = NULL;
    /* ��Ч���������账�� */
    if (ISLESSTHEN_SEQUENCENUMBER(pstHeartbeatMsg->lastSN, pstHeartbeatMsg->firstSN))
    {
        return TRUE;
    }

    /* ��ȡԶ��д����guid */
    stDiscWtGuid.prefix = pstRTPSMsgHeader->guidPrefix;
    stDiscWtGuid.entityId = pstHeartbeatMsg->writerId;

    /* ���Ҹ��û����ݶ�Ӧ��Զ��д���� */
    pstDiscWriter = GetDiscoveredWriterFromParticipantByGuid(pstParticipant, &stDiscWtGuid);
    if (NULL == pstDiscWriter)
    {
        PrintLog(COMMLOG_ERROR, "HandleHeartbeatMsg: Cannot discovered remote dataWriter.\n");
        return FALSE;
    }

    /* �������ڣ����账�� */
    if (pstHeartbeatMsg->count <= pstDiscWriter->stHeartbeat.count)
    {
        return TRUE;
    }
    else
    {
        pstDiscWriter->stHeartbeat.count = pstHeartbeatMsg->count;
    }

    /* û��ָ���Ķ���Id��������Ϊͬһ�����µ��Ķ��� */
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
            /* ָ�����Ķ���id����ҪУ���Ƿ�һ�� */
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

    /* �����Ķ���Ϊ��״̬����ֱ�ӷ��أ���������������� */
    if (BEST_EFFORT_RELIABILITY_QOS == pstDataReader->stDataReaderQos.reliability.kind)
    {
        PrintLog(COMMLOG_ERROR, "HandleHeartbeatMsg: Local reader is stateless, ignore heartbeat.\n");
        return FALSE;
    }

    /*  Զ��д����Ϊ��״̬����ֱ�ӷ��أ���������������� */
    if (BEST_EFFORT_RELIABILITY_QOS == pstDiscWriter->stDataWriterQos.reliability.kind)
    {
        PrintLog(COMMLOG_ERROR, "HandleHeartbeatMsg: Remote writer is stateless, ignore heartbeat.\n");
        return FALSE;
    }

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

    /* ����ֵ����С��seqNum��С����������Ϊ�����ж�, ���ͷ������������������ֵ����Ϊ��СseqNum����ʧ������� */
    if (ISLESSTHEN_SEQUENCENUMBER(pstDiscWriter->stAckNack.readerSNState.base, pstHeartbeatMsg->firstSN))
    {
        DeilverStatusFulUserMsgWhenExpectSeqNumUnderFirstSN(pstDataReader, pstDiscWriter, pstHeartbeatMsg->firstSN);
        PrintLog(COMMLOG_WARN, "HandleHeartbeatMsg: Beause of network interruption, the data maybe missed.\n");
    }

    /* ����ȱʧ���ύȱʧ��ţ��Զ˻�ȡack���ĺ������ش���ack���Ķ���Ҳû��ϵ���ȴ��´�������У�� */
    GetDataReaderMissSeqNum(pstDataReader, pstDiscWriter, pstHeartbeatMsg->lastSN);

//    if(0 == pstDiscWriter->stAckNack.readerSNState.numBits)
//    {
//    	Time_t nowTime = GetNowTime();
//    	printf("%d.%d Nephalem : Ack bitmap %d  nums %d ", nowTime.sec,nowTime.nanosec,pstDiscWriter->stAckNack.readerSNState.base.low,pstDiscWriter->stAckNack.readerSNState.numBits);
//    }

    /* ���л�ack���ķ��� */
    JointAckNackMsgSendAssignWriter(pstDataReader, pstDiscWriter);
     
    pstDiscWriter->heartMsgNum = 0;
    pstDiscWriter->lastHeartTime = GetNowTime();
    return FALSE;
}

/*--------------------------------------------------------------------------------
-   �� �� ��: HandleHeartbeatFragMsg
-   ��������: ��Ƭ����������
-   ��    ��: �ӱ�����Ϣͷ����Ƭ�������ݡ�Participant
-   ��    ��: ����
-   ȫ�ֱ���:
-   ע    ��: ��������.�Ƿ���Ҫ�����ش�
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2021\4\13       1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
BOOL HandleHeartbeatFragMsg(RTPSMessageHeader* pstRTPSMsgHeader, HeartbeatFrag* pstHeartbeatFragMsg, DDS_DomainParticipant* pstParticipant)
{
    GUID_t stDiscWtGuid;
    DDS_DataReader* pstDataReader = NULL;
    DDS_DiscoveredWriter* pstDiscWriter = NULL;

    /* ��Ч���������账�� */
    if (pstHeartbeatFragMsg->lastFragmentNum <= 0)
    {
        return TRUE;
    }

    /* ��ȡԶ��д����guid */
    stDiscWtGuid.prefix = pstRTPSMsgHeader->guidPrefix;
    stDiscWtGuid.entityId = pstHeartbeatFragMsg->writerId;

    /* ���Ҹ��û����ݶ�Ӧ��Զ��д���� */
    pstDiscWriter = GetDiscoveredWriterFromParticipantByGuid(pstParticipant, &stDiscWtGuid);
    if (NULL == pstDiscWriter)
    {
        PrintLog(COMMLOG_ERROR, "HandleHeartbeatMsg: Cannot discovered remote dataWriter.\n");
        return FALSE;
    }

    /* �������ڣ����账�� */
    if (pstHeartbeatFragMsg->count <= pstDiscWriter->stHeartbeat.count)
    {
        return TRUE;
    }
    else
    {
        pstDiscWriter->stHeartbeat.count = pstHeartbeatFragMsg->count;
    }

    /* û��ָ���Ķ���Id��������Ϊͬһ�����µ��Ķ��� */
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
            /* ָ�����Ķ���id����ҪУ���Ƿ�һ�� */
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

    /* �����Ķ���Ϊ��״̬����ֱ�ӷ��أ���������������� */
    if (BEST_EFFORT_RELIABILITY_QOS == pstDataReader->stDataReaderQos.reliability.kind)
    {
        PrintLog(COMMLOG_ERROR, "HandleHeartbeatMsg: Local reader is stateless, ignore heartbeat.\n");
        return FALSE;
    }

    /*  Զ��д����Ϊ��״̬����ֱ�ӷ��أ���������������� */
    if (BEST_EFFORT_RELIABILITY_QOS == pstDiscWriter->stDataWriterQos.reliability.kind)
    {
        PrintLog(COMMLOG_ERROR, "HandleHeartbeatMsg: Remote writer is stateless, ignore heartbeat.\n");
        return FALSE;
    }

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

    /* ���л�ack���ķ��� */
    JointNackFragMsgSendAssignWriter(pstHeartbeatFragMsg->writerSN,pstDataReader, pstDiscWriter);

    //pstDiscWriter->heartMsgNum = 0;
    //pstDiscWriter->lastHeartTime = GetNowTime();
    return FALSE;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: SendHeartbeatTasksOrder
-   ��������: ��ʱ��������
-   ��    ��: pstParticipant
-   ��    ��: ����
-   ȫ�ֱ���:
-   ע    ��: ���Ͷ�ʱ��������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\14       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID SendHeartbeatTasksOrder(DDS_DomainParticipant* pstParticipant)
{
    UINT32 uiNum;
	ULONG ulHeartbeat;
    DDS_DataWriter*  pstLocalWriter = NULL;
	DDS_Topic* pstTopic = NULL;

    /* ����ʱ�� */
    HPTB_END(pstParticipant->heartBeatMark);

    /* �����������(us)*/
    ulHeartbeat = HPTB_INTERVAL_MICROSEC(pstParticipant->heartBeatMark);
    
    if (ulHeartbeat > HEART_TIME * 1000 * 1)
	{
		/* �ڽ�ʵ���������� */
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

		/* �û������������� */
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
-   �� �� ��: NetworkTransceiverTask
-   ��������: �����շ�����
-   ��    ��: pstParticipant
-   ��    ��: ����
-   ȫ�ֱ���:
-   ע    ��: �����շ�����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\14       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL VOID NetworkTransceiverTask(DDS_DomainParticipant* pstParticipant)
{
#if defined(_VXWORKS_653) || defined (_TM1) || defined _TM3
    /* 653�����޷����Σ�ʹ��ȫ�ֱ��� */
	pstParticipant = pstRealDomainParticipant;
#endif
	/* Nephalem ���ջ����� */
	CHAR buffer[NETWORK_BYTE_MAX_SIZE];
	INT length = 0;
	ULONG microSecInterval;
	MemoryBlock stMemBlock;
	/* Nephalem ���ͻ����� */
    CHAR buff[NETWORK_BYTE_MAX_SIZE];
	//TODO���޸Ŀ�������
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
            /* SPDP �������� */
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
                /* �����ֽ����ʼ�������淢������ */
                INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);
                //printf("Nephalem Send SPDP\n");
                JointSPDPDataMsg(&pstParticipant->stRTPSMsgHeader, pstParticipant->stBuiltinDataWriter[0].stHistoryCache.pstHead, &pstParticipant->stBuiltinDataWriter[0], &stMemBlock);
                SendMulticastMsg(&stMemBlock, &pstParticipant->socketList.spdp_data_sender, pstParticipant->rtpsParticipantProxy.metatrafficMulticastLocator);
                               
                //���¿�ʼ��ʱ
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
			/* ������Ϣ */
			RECEIVE_QUEUING_MESSAGE(g_PortConfig.recvPort[i].portID, (APEX_LONG_INTEGER)0, (MESSAGE_ADDR_TYPE)buffer, (MESSAGE_SIZE_TYPE  *)&length, &retCode);
			/* �ɹ��յ���Ϣ */
			if (retCode == 0)
			{
				ParseAllNetworkOrder(buffer, length, pstParticipant);
				recvlen = length;
				printf("Nephalem FC Recv Data From %X Ret = %d , len = %d \n",g_PortConfig.recvPort[i].portSeq,retCode,length);
			}
		}
#else
        /* SPDP �������� */
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

        /* SEDP �������� */
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

        /* USER �������� */
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
        /* �������� */
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
//    /* 653�����޷����Σ�ʹ��ȫ�ֱ��� */
//	pstParticipant = pstRealDomainParticipant;
//#endif
//	/* Nephalem ���ջ����� */
//	CHAR buffer[NETWORK_BYTE_MAX_SIZE];
//	INT length = 0;
//	ULONG microSecInterval;
//	MemoryBlock stMemBlock;
//	/* Nephalem ���ͻ����� */
//    CHAR buff[NETWORK_BYTE_MAX_SIZE];
//	//TODO���޸Ŀ�������
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
//            /* SPDP �������� */
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
//                /* �����ֽ����ʼ�������淢������ */
//                INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);
//                //printf("Nephalem Send SPDP\n");
//                JointSPDPDataMsg(&pstParticipant->stRTPSMsgHeader, pstParticipant->stBuiltinDataWriter[0].stHistoryCache.pstHead, &pstParticipant->stBuiltinDataWriter[0], &stMemBlock);
//                SendMulticastMsg(&stMemBlock, &pstParticipant->socketList.spdp_data_sender, pstParticipant->rtpsParticipantProxy.metatrafficMulticastLocator);
//                               
//                //���¿�ʼ��ʱ
//                HPTB_BEGIN(g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].timeBlock);
//            }
//        }
//      
//#if defined _FC
//        
//        /* Nephalem ����DDS���ݽ���MsgID */ 
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
//		/* Nephalem ����SEDP���ݽ���MsgID */ 
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
//		/* Nephalem �����û����ݽ���MsgID */ 
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
//			/* ������Ϣ */
//			RECEIVE_QUEUING_MESSAGE(g_PortConfig.recvPort[i].portID, (APEX_LONG_INTEGER)0, (MESSAGE_ADDR_TYPE)buffer, (MESSAGE_SIZE_TYPE  *)&length, &retCode);
//			/* �ɹ��յ���Ϣ */
//			if (retCode == 0)
//			{
//				ParseAllNetworkOrder(buffer, length, pstParticipant);
//				recvlen = length;
//				printf("Nephalem FC Recv Data From %X Ret = %d , len = %d \n",g_PortConfig.recvPort[i].portSeq,retCode,length);
//			}
//		}
//#else
//        /* SPDP �������� */
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
//        /* SEDP �������� */
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
//        /* USER �������� */
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
////        /* �������� */
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
-   �� �� ��: SPDPSendTask
-   ��������: �����߷������ݷ�������
-   ��    ��: ������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �����߷������ݷ�������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\06\08       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID SPDPSendTask(DDS_DomainParticipant* pstParticipant)
{
    MemoryBlock stMemBlock;
    CHAR buff[SEDP_BYTE_MAX_SIZE];
    /* �����ֽ����ʼ�������淢������ */
    while (pstParticipant->destroyFlag == FALSE)
    {
#ifdef  STATIC_DISCOVERY //���й����п��ܼ����½ڵ㣬Ŀǰ��̬ƥ�䲻����spdp LJM
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
-   �� �� ��: SPDPReceiveTask
-   ��������: �����߷������ݽ�������
-   ��    ��: ������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �����߷������ݽ�������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\06\08       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID SPDPReceiveTask(DDS_DomainParticipant* pstParticipant)
{
    INT length;
	/* Nephalem buf��С��Ҫ��recvfrom�еĲ�������һ�£������������Ч��cpuռ���ʸ� */
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
-   �� �� ��: SEDPReceiveTask
-   ��������: �ڽ�ʵ�����ݽ�������
-   ��    ��: ������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �ڽ�ʵ�����ݽ�������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
-   �� �� ��: USERReceiveTask
-   ��������: �û����ݽ�������
-   ��    ��: ������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �û����ݽ�������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
-   �� �� ��: HEARTSendTask
-   ��������: ������������
-   ��    ��: ������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ������������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
        /* �ڽ�ʵ���������� */
		/*TODO�������ű���*/
        DDS_MUTEX_LOCK(pstParticipant->m_discRdMutex);
        for (uiNum = init; uiNum < 3; uiNum++)
        {
            JointHeartbeatMsgSendAllBuiltinReader(&pstParticipant->stRTPSMsgHeader, &pstParticipant->stBuiltinDataWriter[uiNum]);
        }
        DDS_MUTEX_UNLOCK(pstParticipant->m_discRdMutex);

//         /* �û������������ͣ����µİ汾���ⲿ�ַŵ���datawriter */
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
    /* 653�����޷����Σ�ʹ��ȫ�ֱ��� */
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
		/*TODO��Ӧ���󣬶���Զ��δ����liveness�������Ҳ���м�⣬�ϲ�ʱȷ�ϱ�׼����*/
		//if (pstParticipant->livelinessNum == 0)
		//{
		//	QY_Sleep(HEART_TIME);
		//	continue;
		//}
		TopicLiveLinessCheck(pstParticipant);

		pstDiscParticipant = pstParticipant->pstDiscParticipant;
#ifdef  STATIC_DISCOVERY
		while (NULL != pstDiscParticipant && pstDiscParticipant->bOnlyStaticMathc == FALSE)//��̬ƥ��Ĳ�����
#else
		while (NULL != pstDiscParticipant)
#endif
		{
			/*TODO��Ӧ���󣬶���Զ��δ����liveness�������Ҳ���м�⣬�ϲ�ʱȷ�ϱ�׼����*/
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

              /*TODO���Ƴ��ڵ�֮ǰ����������*/
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
-   �� �� ��: NetworkSendReceiveTask
-   ��������: ���������շ�
-   ��    ��: ������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ���������շ�
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\06\08       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID NetworkSendReceiveTask(DDS_DomainParticipant* pstParticipant)
{
#if defined(_WIN32)
#ifdef SINGLE_THREAD //���߳�ģʽ��FC��RAPIDIO��653Port����̫����������߳̽������ݣ���FC��Rapidio���ڵ�����±���ʹ�õ��߳�
	HANDLE threadHandle = CreateThread(NULL, 0, (VOID*)&NetworkTransceiverTask, (VOID*)pstParticipant, 0, NULL);
#else //���߳�ģʽ
	/* SPDP���ͱ��� */
	pstParticipant->hThread[0] = CreateThread(NULL, 0, (VOID*)&SPDPSendTask, (VOID*)pstParticipant, 0, NULL);
	/* SPDP���ձ��� */
	pstParticipant->hThread[1] = CreateThread(NULL, 0, (VOID*)&SPDPReceiveTask, (VOID*)pstParticipant, 0, NULL);
	/* SEDP���ձ��� */
	pstParticipant->hThread[2] = CreateThread(NULL, 0, (VOID*)&SEDPReceiveTask, (VOID*)pstParticipant, 0, NULL);
	/* USER���ձ��� */
	pstParticipant->hThread[3] = CreateThread(NULL, 0, (VOID*)&USERReceiveTask, (VOID*)pstParticipant, 0, NULL);
	/* �������ͱ��� */
	pstParticipant->hThread[4] = CreateThread(NULL, 0, (VOID*)&HEARTSendTask, (VOID*)pstParticipant, 0, NULL);
#endif

#ifdef QYDDS_LIVELINESS_QOS
	/*Զ��������ⱨ��*/
	pstParticipant->hThread[5] = CreateThread(NULL, 0, (VOID*)&DiscHEARTCheckTask, (VOID*)pstParticipant, 0, NULL);
#endif //QYDDS_LIVELINESS_QOS
#ifdef TG
	/* ��������������ݻ����߳� */
	pstParticipant->hThread[6] = CreateThread(NULL, 0, (VOID*)&UserDataPeriodTask, (VOID*)pstParticipant, 0, NULL);
#endif //TG
#elif defined(__linux__)//_WIN32
    pthread_create(&pstParticipant->hThread[0], 0, SPDPSendTask, pstParticipant);
    pthread_create(&pstParticipant->hThread[1], 0, SPDPReceiveTask, pstParticipant);
    pthread_create(&pstParticipant->hThread[2], 0, SEDPReceiveTask, pstParticipant);
    pthread_create(&pstParticipant->hThread[3], 0, USERReceiveTask, pstParticipant);
    pthread_create(&pstParticipant->hThread[4], 0, HEARTSendTask, pstParticipant);
#ifdef QYDDS_LIVELINESS_QOS
    /*Զ��������ⱨ��*/
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

/* �����������շ�����ر� */
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
-   �� �� ��: DeleteBuiltinDiscParticipantFromParticipantByGuidPrefix
-   ��������: ɾ��Զ���ڽ�������
-   ��    ��: ���⡢Guid
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ������ɾ��ָ��Զ���ڽ�������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
-   �� �� ��: DeleteALLDiscoveredFromByGuidPrefix
-   ��������: ɾ��Զ������������Ϣ
-   ��    ��: �����ߡ�pstPrefix
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ɾ��Զ������������Ϣ
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DeleteALLDiscoveredFromByGuidPrefix(DDS_DomainParticipant* pstParticipant, GuidPrefix_t stPrefix)
{
	//return ;
    UINT32 uiBuiltinNun = 0;
	DDS_Topic* pstTopic = NULL;

    /* ����ڽ�Զ�˲����� */
    DeleteBuiltinDiscParticipantFromParticipantByGuidPrefix(pstParticipant, &stPrefix);

    for (uiBuiltinNun = 0; uiBuiltinNun < 3; uiBuiltinNun++)
    {
        /* Զ���ڽ�д������� */
        DeleteBuiltinDiscReaderFromBuiltinWriterByGuidPrefix(&pstParticipant->stBuiltinDataWriter[uiBuiltinNun], &stPrefix);

        /* Զ���ڽ��Ķ������ */
        DeleteBuiltinDiscWriterFromBuiltinReaderByGuidPrefix(&pstParticipant->stBuiltinDataReader[uiBuiltinNun], &stPrefix);
    }

    pstTopic = pstParticipant->pstTopic;

    while (NULL != pstTopic)
    {
        /* Զ��д������� */
		DDS_MUTEX_LOCK(pstParticipant->m_discWtMutex);
		DeleteDiscWriterFromTopicByGuidPrefix(pstTopic, &stPrefix);
		DDS_MUTEX_UNLOCK(pstParticipant->m_discWtMutex);

        /* Զ���Ķ������ */
        DDS_MUTEX_LOCK(pstParticipant->m_discRdMutex);
        DeleteDiscReaderFromTopicByGuidPrefix(pstTopic, &stPrefix);
        DDS_MUTEX_UNLOCK(pstParticipant->m_discRdMutex);

        /* Nephalem ownership ��������Ȩǿ����ߵ�д���� */
        FindOwnerWriter(pstTopic);

        pstTopic = pstTopic->pNext;
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: UserMsgSendMonitor
-   ��������: ɾ��Զ������������Ϣ
-   ��    ��: �����ߡ�pstPrefix
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ɾ��Զ������������Ϣ
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
        /*  ����3�������ؽڵ� */
        if (time(NULL) - pstMonitor->deadtime > 3)
        {
            if (NULL == pstPreMonitor)
            {
                pstParticipant->pstMonitor = pstMonitor->pNext;
                /* ������ؽڵ� */
                UninitMonitor(pstMonitor);
                DDS_STATIC_FREE(pstMonitor);
                pstMonitor = pstParticipant->pstMonitor;
            }
            else
            {
                pstPreMonitor->pNext = pstMonitor->pNext;
                /* ������ؽڵ� */
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
                    /* ������Ϣ��־λ */
                    pstMemBlock->base[6] = type;
                    /* ���͵���Ӧ��ÿ��Զ���Ķ��� */
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
-   �� �� ��: AddPartition
-   ��������: ����������
-   ��    ��: pstTopic��������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: 
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020/01/20      1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t AddPartition(DDS_Topic* pstTopic, const CHAR* pstPartitionname)
{
    DDS_TopicQos* pstTopicQos = &pstTopic->stTopicQos;
	PartitionName* pstTempPartitionName = NULL;
	PartitionName* pstPartitionName = NULL;
    /* Nephalem Partition ����������256  ̫�� */
    if (strlen(pstPartitionname) + 1 > 256)
    {
        printf("Nephalem : AddPartition pstPartitionname is too big \n");
        return DDS_RETCODE_ERROR;
    }

    pstTempPartitionName = pstTopicQos->partition.pValue;
    while (NULL != pstTempPartitionName)
    {
        /* Nephalem Partition �������Ѵ��� ֱ�ӷ��� */
        if (!strcmp(pstTempPartitionName->partitionValue.value, pstPartitionname))
            return DDS_RETCODE_OK;

        pstTempPartitionName = pstTempPartitionName->pNext;
    }

    /* Nephalem Partition �����ڴ� */
    pstPartitionName = (PartitionName*)DDS_STATIC_MALLOC(sizeof(PartitionName));
    if (NULL == pstPartitionName)
    {
        printf("Nephalem : AddPartition DDS_STATIC_MALLOC Error \n");
        return DDS_RETCODE_ERROR;
    }
    pstPartitionName->pNext = NULL;
    /* Nephalem Partition ���÷������ͳ��� */
    pstPartitionName->partitionValue.length = strlen(pstPartitionname) + 1;
    memcpy(pstPartitionName->partitionValue.value, pstPartitionname, pstPartitionName->partitionValue.length);

    /* Nephalem Partition ���뵽������ */
    LIST_INSERT_HEAD(pstTopicQos->partition.pValue, pstPartitionName);

    pstTopicQos->partition.size++;

    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DeletePartition
-   ��������: ����ɾ������
-   ��    ��: pstTopic��������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��:
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020/01/20      1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
DDS_ReturnCode_t DeletePartition(DDS_Topic* pstTopic, const CHAR* pstPartitionname)
{
	PartitionName* pstPrePartitionName = NULL;
	PartitionName* pstTempPartitionName = NULL;
    DDS_TopicQos* pstTopicQos = &pstTopic->stTopicQos;
    /* Nephalem Partition ����������256  ̫�� */
    if (strlen(pstPartitionname) + 1 > 256)
    {
        printf("Nephalem : AddPartition pstPartitionname is too big \n");
        return DDS_RETCODE_ERROR;
    }

    pstTempPartitionName = pstTopicQos->partition.pValue;
    while (NULL != pstTempPartitionName)
    {
        /* Nephalem Partition ���������� ����ɾ�� */
        if (!strcmp(pstTempPartitionName->partitionValue.value, pstPartitionname))
        {
            /* Nephalem Partition Ҫɾ���Ľڵ�Ϊͷ�ڵ� */
            if (NULL == pstPrePartitionName)
                pstTopicQos->partition.pValue = pstTempPartitionName->pNext;
            /* Nephalem Partition Ҫɾ���Ľڵ�Ϊ�м�ڵ� */
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
-   �� �� ��: SetTopicQoS
-   ��������: ��������TopicQoS ʹ����Ч
-   ��    ��: pstTopic
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��:
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
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
    /* Nephalem ���±���Զ��д�������Ƿ�����ͬpartition��״̬ */
    while (NULL != pstDDS_DiscoveredWriter)
    {
        pstDDS_DiscoveredWriter->bSamePartiton = EqualPartitionName(&pstTopic->stTopicQos, &pstDDS_DiscoveredWriter->pstRelateTopic->stTopicQos);
        pstDDS_DiscoveredWriter = pstDDS_DiscoveredWriter->pNext;
    }

    pstDDS_DiscoveredReader = pstTopic->pstDiscReader;
    /* Nephalem ���±���Զ���Ķ������Ƿ�����ͬpartition��״̬ */
    while (NULL != pstDDS_DiscoveredReader)
    {
        pstDDS_DiscoveredReader->bSamePartiton = EqualPartitionName(&pstTopic->stTopicQos, &pstDDS_DiscoveredReader->pstRelateTopic->stTopicQos);
        pstDDS_DiscoveredReader = pstDDS_DiscoveredReader->pNext;
    }

    /* Nephalem �����ĺ�partition��״̬���͸������ڵ� */
    pstDataWriter = pstTopic->pstLocalWriter;

    while (NULL != pstDataWriter)
    {
        /* ���뾲̬�ڴ洴��HistoryData����ڵ� */
        pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC(sizeof(HistoryData));
        if (NULL == pstHistoryData)
        {
            PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData, Insufficient memory space.\n");
            DDS_STATIC_FREE(pstDataWriter);
            pstDataWriter = NULL;
            return DDS_RETCODE_ERROR;
        }

		/* Nephalem ���г�ʼ���������ͷŻ���� */
		InitHistoryData(pstHistoryData);

        /* ���뾲̬�ڴ洴����ʷ�������� */
        pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC(SEDP_BYTE_MAX_SIZE);
        if (NULL == pstHistoryData->data)
        {
            /* ʧ���ͷ�ǰ��������ڴ� */
            DDS_STATIC_FREE(pstHistoryData);
            pstHistoryData = NULL;
            DDS_STATIC_FREE(pstDataWriter);
            pstDataWriter = NULL;
            PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData buff, Insufficient memory space.\n");
            return DDS_RETCODE_ERROR;
        }

        /* �����ֽ����ʼ�������淢������ */
        INITIAL_MEMORYBLOCK(stMemBlock, pstHistoryData->data, SEDP_BYTE_MAX_SIZE);

        /* �ڽ�seqNum�Լ�1*/
        pstTopic->pstParticipant->stBuiltinDataWriter[1].seqNum = Add_SequenceNumber(pstTopic->pstParticipant->stBuiltinDataWriter[1].seqNum, SEQUENCENUMBER_START);

        /* �����Ƿ�ȡ������ */
        pstHistoryData->bCancel = FALSE;

        /* �����¼seqNum��� */
        pstHistoryData->seqNum = pstTopic->pstParticipant->stBuiltinDataWriter[1].seqNum;

        /* �����¼ʵ��GUID */
        pstHistoryData->stGuid = pstDataWriter->guid;

        /* ���л�д������Ϣ�� */
        DataWriterConvertNetworkByteOrder(pstTopic, pstDataWriter, &stMemBlock);

        /* ��¼���л����ĳ��� */
        pstHistoryData->uiDataLen = stMemBlock.writeIndex;

        /* ���뵽�ڽ�����д�������� */
        InsertHistoryCacheTail(&pstTopic->pstParticipant->stBuiltinDataWriter[1].stHistoryCache, pstHistoryData);

        /* ��װ�������� */
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

        /* ���뾲̬�ڴ洴��HistoryData����ڵ� */
        pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC(sizeof(HistoryData));
        if (NULL == pstHistoryData)
        {
            DDS_STATIC_FREE(pstDataReader);
            pstDataReader = NULL;
            PrintLog(COMMLOG_ERROR, "DataReader: Failed to create HistoryData, Insufficient memory space.\n");
            return DDS_RETCODE_ERROR;
        }

		/* Nephalem ���г�ʼ���������ͷŻ���� */
		InitHistoryData(pstHistoryData);

        /* ���뾲̬�ڴ洴����ʷ�������� */
        pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC(SEDP_BYTE_MAX_SIZE);
        if (NULL == pstHistoryData->data)
        {
            /* ʧ���ͷ�ǰ��������ڴ� */
            DDS_STATIC_FREE(pstDataReader);
            pstDataReader = NULL;
            DDS_STATIC_FREE(pstHistoryData);
            pstHistoryData = NULL;
            PrintLog(COMMLOG_ERROR, "DataReader: Failed to create HistoryData buff, Insufficient memory space.\n");
            return DDS_RETCODE_ERROR;
        }

        /* �����ֽ����ʼ�������淢������ */
        INITIAL_MEMORYBLOCK(stMemBlock, pstHistoryData->data, SEDP_BYTE_MAX_SIZE);

        /* �ڽ�seqNum�Լ�1*/
        pstTopic->pstParticipant->stBuiltinDataWriter[2].seqNum = Add_SequenceNumber(pstTopic->pstParticipant->stBuiltinDataWriter[2].seqNum, SEQUENCENUMBER_START);

        /* �����Ƿ�ȡ������ */
        pstHistoryData->bCancel = FALSE;

        /* �����¼seqNum��� */
        pstHistoryData->seqNum = pstTopic->pstParticipant->stBuiltinDataWriter[2].seqNum;

        /* �����¼ʵ��GUID */
        pstHistoryData->stGuid = pstDataReader->guid;

        /* ���л��Ķ�����Ϣ�� */
        DataReaderConvertNetworkByteOrder(pstTopic, pstDataReader, &stMemBlock);

        /* ��¼���л����ĳ��� */
        pstHistoryData->uiDataLen = stMemBlock.writeIndex;

        /* ���뵽�ڽ�����д�������� */
        InsertHistoryCacheTail(&pstTopic->pstParticipant->stBuiltinDataWriter[2].stHistoryCache, pstHistoryData);

        /* ��װ���ı��� */
        JointSEDPDataMsgSendBuiltinReader(&pstTopic->pstParticipant->stRTPSMsgHeader, pstHistoryData, &pstTopic->pstParticipant->stBuiltinDataWriter[2], NULL);

        PrintLog(COMMLOG_INFO, "Topic: The dataReader was created successfully.\n");

        pstDataReader = pstDataReader->pNext;
    }

	/* Nephalem �����޸ĺ�ˢ��ownership */
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

		if (NULL == pstDataReader) //����û�и�����Ķ��Ķˣ�����������
		{
			pstTopic = pstTopic->pNext;
			continue;
		}

		//��������reader�Ĵ����QoS��Ϊ�������⣬ֱ������
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
			//Զ������writer�Ĵ����QoS��Ϊ�������⣬ֱ������
			if (MANUAL_BY_TOPIC_LIVELINESS_QOS != pstDiscDataWriter->stDataWriterQos.liveliness.kind)
			{
				pstDiscDataWriter = pstDiscDataWriter->pNext;
				continue;
			}
			
			//�����ʱ������Ϊ0���൱��û�����ã�����
			if (0 == pstDiscDataWriter->lastLivelinessTime.sec && 0 == pstDiscDataWriter->lastLivelinessTime.nanosec)
			{
				pstDiscDataWriter = pstDiscDataWriter->pNext;
				continue;
			}

			//�ж�Զ������writer���״̬
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
    /* 653�����޷����Σ�ʹ��ȫ�ֱ��� */
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
				/* δ����deadline ����  */
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
				/* δ����deadline ����  */
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
	/* ��������  0 SPDP���� 1 SEDP���� 2�û�����*/
	unsigned int length = NETWORK_BYTE_MAX_SIZE;
	int ret = 0;
	int i = 0;
	if(0 == kind)
	{
	    /* Nephalem ����DDS���ݽ���MsgID */ 
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
		/* Nephalem ����SEDP���ݽ���MsgID */ 
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
				/* ��ǰͨ���ӵ����ݺ� i-1,�´μ�����ȡ��ͨ�������� */
				i--;
			}
		}
	}
	
	if(2 == kind)
	{
		/* Nephalem �����û����ݽ���MsgID */ 
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
				/* ��ǰͨ���ӵ����ݺ� i-1,�´μ�����ȡ��ͨ�������� */
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
//    /* ����FC���� */
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
//	/*��ѯ�豸*/
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
//	/*���豸*/
//	ret = FC_NP_DevOpen(&FCdev, 0);
//	if (ret)
//	{
//		printf("FC_NP_DevOpen failed ret=0x%08x \n", ret);
//		return NULL;
//	}
//	FC_BLUE_PRINT gBluePrint;
//	
//	/*ʹ��Ӧ�ö������ͼ*/
//    /* ��ʼ����ͼ���� */
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
//    /* ���������շ�ͨ�� */
//    for (i = 0U; i < g_commFCConfig.m_fcNum; ++i)
//    {
//    	printf("FCID[%u] = 0x%X \n", i, g_commFCConfig.m_deviceFCId[i]);
//    	printf("g_commFCConfig.m_fcSeq =%d and msg_id = %x \n",g_commFCConfig.m_fcSeq,(g_commFCConfig.m_fcSeq << 8U) | i);
//        /* ������ͨ��Ϣ */
//        gBluePrint.config_block[0].scheme[0].send_item[i].msg_id = (g_commFCConfig.m_fcSeq << 8U) | i;
//        sprintf(gBluePrint.config_block[0].scheme[0].send_item[i].msg_name, "N_%u_%u", g_commFCConfig.m_fcSeq, i);
//        gBluePrint.config_block[0].scheme[0].send_item[i].msg_max_len = 2096;
//        gBluePrint.config_block[0].scheme[0].send_item[i].msg_src_id = g_commFCConfig.m_fcId;
//        gBluePrint.config_block[0].scheme[0].send_item[i].msg_dst_id = g_commFCConfig.m_deviceFCId[i];
//        gBluePrint.config_block[0].scheme[0].send_item[i].msg_type = FC_MSG_TYPE_EVENT;
//        /* ��������Ϣ */
//        gBluePrint.config_block[0].scheme[0].send_item[i + g_commFCConfig.m_fcNum].msg_id = 0x10000U | ((g_commFCConfig.m_fcSeq << 8U) | i);
//        sprintf(gBluePrint.config_block[0].scheme[0].send_item[i + g_commFCConfig.m_fcNum].msg_name, "S_%u_%u", g_commFCConfig.m_fcSeq, i);
//        gBluePrint.config_block[0].scheme[0].send_item[i + g_commFCConfig.m_fcNum].msg_max_len = 0x00100000U;
//        gBluePrint.config_block[0].scheme[0].send_item[i + g_commFCConfig.m_fcNum].msg_src_id = g_commFCConfig.m_fcId;
//        gBluePrint.config_block[0].scheme[0].send_item[i + g_commFCConfig.m_fcNum].msg_dst_id = g_commFCConfig.m_deviceFCId[i];
//        gBluePrint.config_block[0].scheme[0].send_item[i + g_commFCConfig.m_fcNum].msg_type = FC_MSG_TYPE_STREAM;
//        /* ������ͨ��Ϣ */
//        gBluePrint.config_block[0].scheme[0].recv_item[i].msg_id = (i << 8U) | g_commFCConfig.m_fcSeq;
//        sprintf(gBluePrint.config_block[0].scheme[0].recv_item[i].msg_name, "N_%u_%u", i, g_commFCConfig.m_fcSeq);
//        gBluePrint.config_block[0].scheme[0].recv_item[i].msg_max_len = 2096;
//        gBluePrint.config_block[0].scheme[0].recv_item[i].msg_type = FC_MSG_TYPE_EVENT;
//        gBluePrint.config_block[0].scheme[0].recv_item[i].msg_recv_mode = FC_MSG_RECV_MODE_POLL;
//        gBluePrint.config_block[0].scheme[0].recv_item[i].msg_src_id = g_commFCConfig.m_deviceFCId[i];
//        gBluePrint.config_block[0].scheme[0].recv_item[i].msg_recover = 0U;
//        /* ��������Ϣ */
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
//	/*�����豸*/
//	ret = FC_NP_LoadConfig(FCdev, 0, 0);
//	if (ret)
//	{
//		printf("FC_NP_LoadConfig failed ret=0x%08x \n", ret);
//		return NULL;
//	}
//	
//	if (g_commFCConfig.m_role == FC_NET_ROLE_NCT)
//	{
//		/*����WDT��ʱ��ʹ��*/
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
//	/*��ȡ�˿�״̬*/
//	ret = FC_NP_GetPortStatus(FCdev, &value);
//	/* �����ȡ�˿�״̬ʧ�ܣ����߶˿�û�����ߣ���ֱ�ӷ��� */
//	if (ret || value == 0)
//	{
//		printf("FC_NP_GetPortStatus portStatus=%d ret = %d \n", value, ret);
//		return 0;
//	}
//	
//	/* ����������ؽڵ㣬����Ҫ�������� */
//	if (g_commFCConfig.m_role != FC_NET_ROLE_NCT)
//	{
//		/*��ѯNCT�Ƿ�����*/
//		ret = FC_NM_NctIsOnLine(FCdev, &value);
//		if (ret || value == 0)
//		{
//    		printf("FC_NM_NctIsOnLine online=%d ret = %d \n", value, ret);
//    		return 0;
//		}
//		printf("FC_NM_NctIsOnLine online=%d ret = %d \n", value, ret);
//		/*��������*/
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
