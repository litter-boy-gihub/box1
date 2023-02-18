#include "../include/GlobalDefine.h"

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_Topic_Init
-   ��������: ��ʼ����
-   ��    ��: ����ָ��
-   ��    ��:
-   ȫ�ֱ���: 
-   ע    ��: ��ʼ����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
=======================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
void DDS_Topic_Init(DDS_Topic* pstTopic)
{
    if (NULL != pstTopic)
    {
        InitHistoryCache(&pstTopic->stUserData);
        pstTopic->pstLocalWriter  = NULL;
        pstTopic->pstLocalReader  = NULL;
        pstTopic->pstDiscWriter = NULL;
        pstTopic->pstDiscReader = NULL;
        pstTopic->pNext               = NULL;
        pstTopic->tpGuid              = GUID_UNKNOWN;
		pstTopic->pstMonLocator = NULL;
		/** JD 20211116 ��ӳ�ʼ����ֵ 
		 ** ���յ�Զ��DR��DW�ǻᴴ�����Ӧ�����⣬���֮�󱾵ش�����Ӧ�����⣬���ܻ���Ϊû�и�TopicQos����ֵ
		 ** ���·���DR����DW�е�ֵ���ִ���
		***/
		memcpy(&(pstTopic->stTopicQos), &DDS_TOPIC_QOS_DEFAULT, sizeof(DDS_TOPIC_QOS_DEFAULT));

        pstTopic->stTopicQos.partition.pValue = NULL;
        pstTopic->stTopicQos.partition.size = 0;
#ifdef EVO_DDS
        pstTopic->topicNum = 1;
#endif
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_Topic_Uninit
-   ��������: ��������
-   ��    ��: ����ָ��
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
=======================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
void DDS_Topic_Uninit(DDS_Topic* pstTopic)
{
	DDS_DataWriter*  pstDataWriter = NULL;
	DDS_DataReader*  pstDataReader = NULL;
	DDS_DiscoveredWriter*  pstDiscWriter = NULL;
	DDS_DiscoveredReader*  pstDiscReader = NULL;
    if (NULL != pstTopic)
    {
        UninitHistoryCache(&pstTopic->stUserData);
        UnInitTopicPartitionQoS(&pstTopic->stTopicQos);

        /* ��������д���� */
        pstDataWriter = pstTopic->pstLocalWriter;
        while (NULL != pstDataWriter)
        {
            pstTopic->pstLocalWriter = pstDataWriter->pNext;

            DDS_DataWriter_Uninit(pstDataWriter);
            DDS_STATIC_FREE(pstDataWriter);

            pstDataWriter = pstTopic->pstLocalWriter;
        }

        /* ���������Ķ��� */
        pstDataReader = pstTopic->pstLocalReader;
        while (NULL != pstDataReader)
        {
            pstTopic->pstLocalReader = pstDataReader->pNext;

            DDS_DataReader_Uninit(pstDataReader);
            DDS_STATIC_FREE(pstDataReader);

            pstDataReader = pstTopic->pstLocalReader;
        }

        /* ����Զ��д���� */
        pstDiscWriter = pstTopic->pstDiscWriter;
        while (NULL != pstDiscWriter)
        {
            pstTopic->pstDiscWriter = pstDiscWriter->pNext;

            DDS_DiscoveredWriter_Uninit(pstDiscWriter);
            DDS_STATIC_FREE(pstDiscWriter);

            pstDiscWriter = pstTopic->pstDiscWriter;
        }

        /* ����Զ���Ķ��� */
        pstDiscReader = pstTopic->pstDiscReader;
        while (NULL != pstDiscReader)
        {
            pstTopic->pstDiscReader = pstDiscReader->pNext;
            DDS_DiscoveredReader_Uninit(pstDiscReader);
            DDS_STATIC_FREE(pstDiscReader);

            pstDiscReader = pstTopic->pstDiscReader;
        }
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_Topic_get_default_datawriter_qos
-   ��������: ��ȡĬ��д����qos
-   ��    ��: ����ָ�롢д����qosָ��
-   ��    ��: д����ָ��
-   ȫ�ֱ���:
-   ע    ��: ��ȡĬ��д����qos
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t DDS_Topic_get_default_datawriter_qos(DDS_Topic* pstTopic, DDS_DataWriterQos* pstWriterQos)
{
    if (NULL == pstWriterQos)
    {
        PrintLog(COMMLOG_ERROR, "topic: please pass in a non-null pointer of datawriter qos.\n");
        return DDS_RETCODE_ERROR;
    }

    *pstWriterQos = DDS_DATAWRITER_QOS_DEFAULT;
    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_Topic_get_default_datawriter_qos
-   ��������: ��ȡĬ��д����qos
-   ��    ��: ����ָ�롢д����qosָ��
-   ��    ��: д����ָ��
-   ȫ�ֱ���:
-   ע    ��: ��ȡĬ��д����qos
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_DataWriterQos* DDS_Topic_get_default_datawriter_qos_cs(DDS_Topic* pstTopic)
{
    DDS_DataWriterQos* pstWriterQos = (DDS_DataWriterQos*)DDS_STATIC_MALLOC(sizeof(DDS_DataWriterQos));
    if (NULL == pstWriterQos)
    {
        PrintLog(COMMLOG_ERROR, "topic: please pass in a non-null pointer of datawriter qos.\n");
        return NULL;
    }

    *pstWriterQos = DDS_DATAWRITER_QOS_DEFAULT;

    return pstWriterQos;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_Topic_get_default_datawriter_qos
-   ��������: ��ȡĬ��д����qos
-   ��    ��: ����ָ�롢д����qosָ��
-   ��    ��: д����ָ��
-   ȫ�ֱ���:
-   ע    ��: ��ȡĬ���Ķ���qos
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t DDS_Topic_get_default_datareader_qos(DDS_Topic* pstTopic, DDS_DataReaderQos* pstReaderQos)
{
    if (NULL == pstReaderQos)
    {
        PrintLog(COMMLOG_ERROR, "topic: please pass in a non-null pointer of datawriter qos.\n");
        return DDS_RETCODE_ERROR;
    }

    *pstReaderQos = DDS_DATAREADER_QOS_DEFAULT;
    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_Topic_datawriter_qos_set_reliability
-   ��������: д�������ÿɿ���
-   ��    ��: д�������ɿ���
-   ��    ��: ����ֵ
-   ȫ�ֱ���:
-   ע    ��: д�������ÿɿ���qos
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t DDS_Topic_datawriter_qos_set_reliability(DDS_DataWriterQos* pstWriterQos, ReliabilityQosPolicyKind kind)
{
    if (NULL == pstWriterQos)
    {
        PrintLog(COMMLOG_ERROR, "topic: please pass in a non-null pointer of datawriter qos.\n");
        return DDS_RETCODE_ERROR;
    }

    pstWriterQos->reliability.kind = kind;

    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_Topic_datareader_qos_set_reliability
-   ��������: �Ķ������ÿɿ���
-   ��    ��: �Ķ������ɿ���
-   ��    ��: ����ֵ
-   ȫ�ֱ���:
-   ע    ��: �Ķ������ÿɿ���qos
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t DDS_Topic_datareader_qos_set_reliability(DDS_DataReaderQos* pstReaderQos, ReliabilityQosPolicyKind kind)
{
    if (NULL == pstReaderQos)
    {
        PrintLog(COMMLOG_ERROR, "topic: please pass in a non-null pointer of datawriter qos.\n");
        return DDS_RETCODE_ERROR;
    }

    pstReaderQos->reliability.kind = kind;

    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_Topic_get_default_datawriter_qos
-   ��������: ��ȡĬ��д����qos
-   ��    ��: ����ָ�롢д����qosָ��
-   ��    ��: д����ָ��
-   ȫ�ֱ���:
-   ע    ��: ��ȡĬ���Ķ���qos
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_DataReaderQos* DDS_Topic_get_default_datareader_qos_cs(DDS_Topic* pstTopic)
{
    DDS_DataReaderQos* pstReaderQos = (DDS_DataReaderQos*)DDS_STATIC_MALLOC(sizeof(DDS_DataReaderQos));
    if (NULL == pstReaderQos)
    {
        PrintLog(COMMLOG_ERROR, "topic: please pass in a non-null pointer of datawriter qos.\n");
        return NULL;
    }

    *pstReaderQos = DDS_DATAREADER_QOS_DEFAULT;

    return pstReaderQos;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_Topic_create_datawriter
-   ��������: ��������д����
-   ��    ��: ����ָ�롢д����qosָ��
-   ��    ��: д����ָ��
-   ȫ�ֱ���:
-   ע    ��: ��������д����������������Ϣ�����Ķ�
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_DataWriter* DDS_Topic_create_datawriter(DDS_Topic* pstTopic, DDS_DataWriterQos* pstWriterQos, CALLBACKTIME callBackTime)
{

	MemoryBlock stMemBlock;
	DDS_DataWriter* pstTempDataWriter = NULL;
	DDS_DiscoveredReader*  pstDiscReader = NULL;
	DDS_DataWriter* pstDataWriter = NULL;
	HistoryData* pstHistoryData = NULL;

    if (pstWriterQos == NULL)
    {
		pstWriterQos = (DDS_DataWriterQos*)DDS_STATIC_MALLOC(sizeof(DDS_DataWriterQos));
		*pstWriterQos = DDS_DATAWRITER_QOS_DEFAULT;
		pstWriterQos->reliability.kind = BEST_EFFORT_RELIABILITY_QOS;
    }
	else
	{
		/* Nephalem DataWriterQoS���ü�� */
		if (0 > DataWriterQoSCheck(pstWriterQos))
		{
			return NULL;
		}
	}
    
#if defined _DEBUG_LOG
    static int dwcount = 0;
    printf("Nephalem : Create DataWrtire %d Topic = %s \n",++dwcount,pstTopic->topicName.value);
#endif
    
#ifdef  STATIC_DISCOVERY
	if (pstTopic->pstLocalWriter != NULL)
		return pstTopic->pstLocalWriter;

	pstTopic->pstParticipant->bOnlyStaticMathc = FALSE; //��̬���ķ��������ⲻ���ٵ���ע��ӿڣ�LJM
#endif

    pstDataWriter = (DDS_DataWriter*)DDS_STATIC_MALLOC(sizeof(DDS_DataWriter));
    if (NULL == pstDataWriter)
    {
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData, Insufficient memory space.\n");
        return NULL;
    }

    /* ���뾲̬�ڴ洴��HistoryData����ڵ� */
    pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC(sizeof(HistoryData));
    if (NULL == pstHistoryData)
    {
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData, Insufficient memory space.\n");
        DDS_STATIC_FREE(pstDataWriter);
        return NULL;
    }

	/* Nephalem ��ʼ���������ͷ�ʱ���� */
	InitHistoryData(pstHistoryData);

    /* ���뾲̬�ڴ洴����ʷ�������� */
    pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC(SEDP_BYTE_MAX_SIZE);
    if (NULL == pstHistoryData->data)
    {
        /* ʧ���ͷ�ǰ��������ڴ� */
        DDS_STATIC_FREE(pstHistoryData);
        DDS_STATIC_FREE(pstDataWriter);
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData buff, Insufficient memory space.\n");
        return NULL;
    }

    /* �����ֽ����ʼ�������淢������ */
    INITIAL_MEMORYBLOCK(stMemBlock, pstHistoryData->data, SEDP_BYTE_MAX_SIZE);

    if (NULL != pstWriterQos)
    {
		if (pstWriterQos->history.depth <= 0)
		{
			printf("HISTORY_QOS ERROR: depth must greater than 0! \n");
            DDS_STATIC_FREE(pstHistoryData->data);
			DDS_STATIC_FREE(pstHistoryData);
            DDS_STATIC_FREE(pstDataWriter);
			return NULL;
		}
        /* ���뱾��д����qos */
        pstDataWriter->stDataWriterQos = *pstWriterQos;
    }
    else
    {
        /* ���뱾��д����Ĭ��qos */
        pstDataWriter->stDataWriterQos = DDS_DATAWRITER_QOS_DEFAULT;
    }

    /* ��ʼ������д���� */
    DDS_DataWriter_Init(pstDataWriter);

    /* ���游�ڵ�,��������ϲ����� */
    pstDataWriter->pstTopic = pstTopic;

    /* ����ʱ��������ص� */
    pstDataWriter->writerCallBack = callBackTime;

	/* ����д����GUID */
	GenGuid(&pstDataWriter->guid, &pstTopic->pstParticipant->stRTPSMsgHeader.guidPrefix, ENTITYKIND_WRITER_NO_KEY);

    /* ÿ����һ������д�������ڽ�seqNum�Լ�1*/
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

    /* Զ��reader��ӱ���writer */
    pstDiscReader = pstTopic->pstDiscReader;
    
    while (NULL != pstDiscReader)
    {
        DiscoveredReaderAddLocalWriter(pstDiscReader, &pstDataWriter->guid);
        pstDiscReader = pstDiscReader->pNext;
    }

    PrintLog(COMMLOG_INFO, "Topic: The dataWriter was created successfully.\n");

    LIST_INSERT_TAIL(pstTopic->pstLocalWriter, pstDataWriter, pstTempDataWriter);
#ifdef QYDDS_DEADLINE_QOS
	if (pstDataWriter->stDataWriterQos.deadline.period.sec != 0 || pstDataWriter->stDataWriterQos.deadline.period.nanosec != 0)
	{
		DeadLineSendTask(pstDataWriter);
	}
#endif
	
#if !defined(NO_HEARTBEAT)
    if (pstDataWriter->stDataWriterQos.reliability.kind == RELIABLE_RELIABILITY_QOS)
    {
        UserDataHeartSendTask(pstDataWriter);
    }
#endif
    //QY_Sleep(1);
    return pstDataWriter;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_Topic_get_datawriter_num
-   ��������: ��ȡ�����±���д��������
-   ��    ��: ����ָ��
-   ��    ��: д��������
-   ȫ�ֱ���:
-   ע    ��: ��ȡ�����±���д��������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\07\16       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL UINT32 DDS_Topic_get_datawriter_num(DDS_Topic* pstTopic)
{
    UINT32 uiWriterNum = 0;

    DDS_DataWriter* pstDataWriter = pstTopic->pstLocalWriter;
    while (NULL != pstDataWriter)
    {
        uiWriterNum++;

        pstDataWriter = pstDataWriter->pNext;
    }

    return uiWriterNum;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_Topic_create_datareader
-   ��������: ���������Ķ���
-   ��    ��: ����ָ�롢�Ķ���qosָ�롢�ص�����
-   ��    ��: д����ָ��
-   ȫ�ֱ���:
-   ע    ��: ��������д����������������Ϣ�����Ķ�
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_DataReader* DDS_Topic_create_datareader(DDS_Topic* pstTopic, DDS_DataReaderQos* pstReaderQos, CALLBACKFUN callBackFun)
{
	MemoryBlock stMemBlock;
	DDS_DataReader*  pstTempDataReader = NULL;
	DDS_DataReader*  pstDataReader = NULL;
	HistoryData* pstHistoryData = NULL;

	// ������Ч�Լ��
    if (pstReaderQos == NULL)
    {
		pstReaderQos = (DDS_DataReaderQos*)DDS_STATIC_MALLOC(sizeof(DDS_DataReaderQos));
        *pstReaderQos = DDS_DATAREADER_QOS_DEFAULT;
        pstReaderQos->reliability.kind = BEST_EFFORT_RELIABILITY_QOS;
    }
	else
	{
		/* Nephalem DataReaderQoS���ü�� */
		if (0 > DataReaderQoSCheck(pstReaderQos))
		{
			return NULL;
		}
	}

#if defined _DEBUG_LOG
    static int drcount = 0;
    printf("Nephalem : Create DataReder %d Topic = %s \n",++drcount,pstTopic->topicName.value);
#endif

#ifdef  STATIC_DISCOVERY
	if (pstTopic->pstLocalReader != NULL)
	{
		pstTopic->pstLocalReader->recvCallBack = callBackFun;
		return pstTopic->pstLocalReader;
	}


	pstTopic->pstParticipant->bOnlyStaticMathc = FALSE;
#endif
    pstDataReader = (DDS_DataReader*)DDS_STATIC_MALLOC(sizeof(DDS_DataReader));
    if (NULL == pstDataReader)
    {
        PrintLog(COMMLOG_ERROR, "DataReader: Failed to create HistoryData, Insufficient memory space.\n");
        return NULL;
    }
    
    /* ��ʼ�������Ķ��� */
    DDS_DataReader_Init(pstDataReader);

    /* ���뾲̬�ڴ洴��HistoryData����ڵ� */
    pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC(sizeof(HistoryData));
    if (NULL == pstHistoryData)
    {
        DDS_STATIC_FREE(pstDataReader);
        PrintLog(COMMLOG_ERROR, "DataReader: Failed to create HistoryData, Insufficient memory space.\n");
        return NULL;
    }

	/* Nephalem ��ʼ���������ͷ�ʱ���� */
	InitHistoryData(pstHistoryData);

    /* ���뾲̬�ڴ洴����ʷ�������� */
    pstHistoryData->data = (CHAR*)DDS_STATIC_MALLOC(SEDP_BYTE_MAX_SIZE);
    if (NULL == pstHistoryData->data)
    {
        /* ʧ���ͷ�ǰ��������ڴ� */
        DDS_STATIC_FREE(pstDataReader);
        DDS_STATIC_FREE(pstHistoryData);
        PrintLog(COMMLOG_ERROR, "DataReader: Failed to create HistoryData buff, Insufficient memory space.\n");
        return NULL;
    }

    /* �����ֽ����ʼ�������淢������ */
    INITIAL_MEMORYBLOCK(stMemBlock, pstHistoryData->data, SEDP_BYTE_MAX_SIZE);

    /* ���뱾���Ķ���qos */
    pstDataReader->stDataReaderQos = *pstReaderQos;

    /* ����ص����� */
    pstDataReader->recvCallBack = callBackFun;

    /* ���游�ڵ� */
    pstDataReader->pstTopic = pstTopic;

	/* ����д����GUID */
	GenGuid(&pstDataReader->guid, &pstTopic->pstParticipant->stRTPSMsgHeader.guidPrefix, ENTITYKIND_READER_NO_KEY);

    /* ÿ����һ�������Ķ������ڽ�seqNum�Լ�1*/
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

    LIST_INSERT_TAIL(pstTopic->pstLocalReader, pstDataReader, pstTempDataReader);
    return pstDataReader;
}


/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_Topic_get_datareader_num
-   ��������: ��ȡ�����±����Ķ�������
-   ��    ��: ����ָ��
-   ��    ��: �Ķ�������
-   ȫ�ֱ���:
-   ע    ��: ��ȡ�����±����Ķ�������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\07\16       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL UINT32 DDS_Topic_get_datareader_num(DDS_Topic* pstTopic)
{
    UINT32 uiReaderNum = 0;

    DDS_DataReader* pstDataReader = pstTopic->pstLocalReader;
    while (NULL != pstDataReader)
    {
        uiReaderNum++;

        pstDataReader = pstDataReader->pNext;
    }

    return uiReaderNum;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_Topic_delete_datawriter
-   ��������: ����ɾ��д����
-   ��    ��: ����ָ��
-   ��    ��: д����ָ��
-   ȫ�ֱ���:
-   ע    ��: ����ɾ��д����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\03\18       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t DDS_Topic_delete_datawriter(DDS_Topic* pstTopic)
{
	HistoryData* pstHistoryData = NULL;
    /* ��������д���� */
    DDS_DataWriter*  pstDataWriter = pstTopic->pstLocalWriter;
    while (NULL != pstDataWriter)
    {
        /* ���뾲̬�ڴ洴��HistoryData����ڵ� */
        pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC(sizeof(HistoryData));
        if (NULL == pstHistoryData)
        {
            PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData, Insufficient memory space.\n");
            return DDS_RETCODE_ERROR;
        }

		/* Nephalem ���г�ʼ���������ͷŻ���� */
		InitHistoryData(pstHistoryData);

        /* ÿ����һ������д�������ڽ�seqNum�Լ�1*/
        pstTopic->pstParticipant->stBuiltinDataWriter[1].seqNum = Add_SequenceNumber(pstTopic->pstParticipant->stBuiltinDataWriter[1].seqNum, SEQUENCENUMBER_START);

        /* ������������ */
        pstHistoryData->data = NULL;

        /* �����Ƿ�ȡ������ */
        pstHistoryData->bCancel = TRUE;

        /* �����¼seqNum��� */
        pstHistoryData->seqNum = pstTopic->pstParticipant->stBuiltinDataWriter[1].seqNum;

        /* �����¼ʵ��GUID */
        pstHistoryData->stGuid = pstDataWriter->guid;

        /* ���뵽�ڽ�����д�������� */
        InsertHistoryCacheTail(&pstTopic->pstParticipant->stBuiltinDataWriter[1].stHistoryCache, pstHistoryData);

        /* ��װ���ı��� */
        JointSEDPDataMsgSendBuiltinReader(&pstTopic->pstParticipant->stRTPSMsgHeader, pstHistoryData, &pstTopic->pstParticipant->stBuiltinDataWriter[1], NULL);

        PrintLog(COMMLOG_INFO, "Topic: The dataWriter was deleted successfully.\n");

        /* ����д���� */
        
        DDS_DataWriter_Uninit(pstDataWriter);
        

        pstDataWriter = pstDataWriter->pNext;
    }

    /* �ڵ�ɾ�� */
    LIST_DELETE_ALL(pstTopic->pstLocalWriter, pstDataWriter);

    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_Topic_delete_datawriter
-   ��������: ����ɾ��д����
-   ��    ��: ����ָ��
-   ��    ��: д����ָ��
-   ȫ�ֱ���:
-   ע    ��: ����ɾ��д����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\03\18       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t DDS_DataWriter_delete_from_topic(DDS_DataWriter*  pstDataWriter)
{
    DDS_DomainParticipant* pstParticipant = pstDataWriter->pstTopic->pstParticipant;

    /* ���뾲̬�ڴ洴��HistoryData����ڵ� */
    HistoryData* pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC(sizeof(HistoryData));
    if (NULL == pstHistoryData)
    {
        PrintLog(COMMLOG_ERROR, "DataWriter: Failed to create HistoryData, Insufficient memory space.\n");
        return DDS_RETCODE_ERROR;
    }

	/* Nephalem ���г�ʼ���������ͷŻ���� */
	InitHistoryData(pstHistoryData);

    /* ÿ����һ������д�������ڽ�seqNum�Լ�1*/
    pstParticipant->stBuiltinDataWriter[1].seqNum = Add_SequenceNumber(pstParticipant->stBuiltinDataWriter[1].seqNum, SEQUENCENUMBER_START);

    /* ������������ */
    pstHistoryData->data = NULL;

    /* �����Ƿ�ȡ������ */
    pstHistoryData->bCancel = TRUE;

    /* �����¼seqNum��� */
    pstHistoryData->seqNum = pstParticipant->stBuiltinDataWriter[1].seqNum;

    /* �����¼ʵ��GUID */
    pstHistoryData->stGuid = pstDataWriter->guid;

    /* ���뵽�ڽ�����д�������� */
    InsertHistoryCacheTail(&pstParticipant->stBuiltinDataWriter[1].stHistoryCache, pstHistoryData);

    /* ��װ���ı��� */
    JointSEDPDataMsgSendBuiltinReader(&pstParticipant->stRTPSMsgHeader, pstHistoryData, &pstParticipant->stBuiltinDataWriter[1], NULL);

    PrintLog(COMMLOG_INFO, "Topic: The dataWriter was deleted successfully.\n");

    /* ����д���� */
    DDS_DataWriter_delete(pstDataWriter);

    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_Topic_delete_datawriter
-   ��������: ����ɾ���Ķ���
-   ��    ��: ����ָ�롢д����qosָ��
-   ��    ��: д����ָ��
-   ȫ�ֱ���:
-   ע    ��: ����ɾ���Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\03\18       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t DDS_Topic_delete_datareader(DDS_Topic* pstTopic)
{
    /* ���������Ķ��� */
    DDS_DataReader*  pstDataReader = pstTopic->pstLocalReader;
	HistoryData* pstHistoryData = NULL;

    while (NULL != pstDataReader)
    {
        /* ���뾲̬�ڴ洴��HistoryData����ڵ� */
        pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC(sizeof(HistoryData));
        if (NULL == pstHistoryData)
        {
            PrintLog(COMMLOG_ERROR, "DataReader: Failed to create HistoryData, Insufficient memory space.\n");
            return DDS_RETCODE_ERROR;
        }

		/* Nephalem ���г�ʼ���������ͷŻ���� */
		InitHistoryData(pstHistoryData);

        /* ÿ����һ������д�������ڽ�seqNum�Լ�1*/
        pstTopic->pstParticipant->stBuiltinDataWriter[2].seqNum = Add_SequenceNumber(pstTopic->pstParticipant->stBuiltinDataWriter[2].seqNum, SEQUENCENUMBER_START);

        /* ������������ */
        pstHistoryData->data = NULL;

        /* �����Ƿ�ȡ������ */
        pstHistoryData->bCancel = TRUE;

        /* �����¼seqNum��� */
        pstHistoryData->seqNum = pstTopic->pstParticipant->stBuiltinDataWriter[2].seqNum;

        /* �����¼ʵ��GUID */
        pstHistoryData->stGuid = pstDataReader->guid;

        /* ���뵽�ڽ�����д�������� */
        InsertHistoryCacheTail(&pstTopic->pstParticipant->stBuiltinDataWriter[2].stHistoryCache, pstHistoryData);

        /* ��װ���ı��� */
        JointSEDPDataMsgSendBuiltinReader(&pstTopic->pstParticipant->stRTPSMsgHeader, pstHistoryData, &pstTopic->pstParticipant->stBuiltinDataWriter[2], NULL);

        PrintLog(COMMLOG_INFO, "Topic: The dataWriter was deleted successfully.\n");

        /* ����д���� */
        DDS_DataReader_Uninit(pstDataReader);

        pstDataReader = pstDataReader->pNext;
    }

    /* �ڵ�ɾ�� */
    LIST_DELETE_ALL(pstTopic->pstLocalReader, pstDataReader);

    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_Topic_delete_datawriter
-   ��������: ����ɾ���Ķ���
-   ��    ��: ����ָ�롢д����qosָ��
-   ��    ��: д����ָ��
-   ȫ�ֱ���:
-   ע    ��: ����ɾ���Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\03\18       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL DDS_ReturnCode_t DDS_DataReader_delete_from_topic(DDS_DataReader*  pstDataReader)
{
	DDS_DataReader* tmpDataReader = NULL;
    DDS_DomainParticipant* pstParticipant =  pstDataReader->pstTopic->pstParticipant;

    /* ���뾲̬�ڴ洴��HistoryData����ڵ� */
    HistoryData* pstHistoryData = (HistoryData*)DDS_STATIC_MALLOC(sizeof(HistoryData));
    if (NULL == pstHistoryData)
    {
        PrintLog(COMMLOG_ERROR, "DataReader: Failed to create HistoryData, Insufficient memory space.\n");
        return DDS_RETCODE_ERROR;
    }

	/* Nephalem ���г�ʼ���������ͷŻ���� */
	InitHistoryData(pstHistoryData);

    /* ÿ����һ������д�������ڽ�seqNum�Լ�1*/
    pstParticipant->stBuiltinDataWriter[2].seqNum = Add_SequenceNumber(pstParticipant->stBuiltinDataWriter[2].seqNum, SEQUENCENUMBER_START);

    /* ������������ */
    pstHistoryData->data = NULL;

    /* �����Ƿ�ȡ������ */
    pstHistoryData->bCancel = TRUE;

    /* �����¼seqNum��� */
    pstHistoryData->seqNum = pstParticipant->stBuiltinDataWriter[2].seqNum;

    /* �����¼ʵ��GUID */
    pstHistoryData->stGuid = pstDataReader->guid;

    /* ���뵽�ڽ�����д�������� */
    InsertHistoryCacheTail(&pstParticipant->stBuiltinDataWriter[2].stHistoryCache, pstHistoryData);

    /* ��װ���ı��� */
    JointSEDPDataMsgSendBuiltinReader(&pstParticipant->stRTPSMsgHeader, pstHistoryData, &pstParticipant->stBuiltinDataWriter[2], NULL);

    PrintLog(COMMLOG_INFO, "Topic: The dataWriter was deleted successfully.\n");

    /* ����д���� */
    DDS_DataReader_delete(pstDataReader);
    if (pstDataReader->pNext != NULL)
    {
        while (pstDataReader->pNext != NULL)
        {
            tmpDataReader = pstDataReader;
            pstDataReader = pstDataReader->pNext;
            DDS_STATIC_FREE(tmpDataReader);
        }
    }
    else
    {
        DDS_STATIC_FREE(pstDataReader);
        pstDataReader = NULL;
    }
    return DDS_RETCODE_OK;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: GetDiscoveredWriterFromTopicByEntityId
-   ��������: ����д����
-   ��    ��: ���⡢Guid
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��������ͨ��Guid������ӦԶ��д����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\8       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DiscoveredWriter* GetDiscoveredWriterFromTopicByGuid(DDS_Topic* pstTopic, GUID_t* pstGuid)
{
	DDS_DiscoveredWriter* pstDiscWriter = NULL;
    if (NULL != pstTopic)
    {
        /* ���������µ�ÿ��Զ��д���� */
        pstDiscWriter = pstTopic->pstDiscWriter;
        while (NULL != pstDiscWriter)
        {
            if (Guid_Is_Equal(&pstDiscWriter->guid, pstGuid))
            {
                return pstDiscWriter;
            }
            pstDiscWriter = pstDiscWriter->pNext;
        }
    }
    return NULL;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: GetDiscoveredReaderFromTopicByEntityId
-   ��������: ����Զ����
-   ��    ��: ���⡢Guid
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��������ͨ��Guid������ӦԶ���Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\8       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DiscoveredReader* GetDiscoveredReaderFromTopicByGuid(DDS_Topic* pstTopic, GUID_t* pstGuid)
{
	DDS_DiscoveredReader* pstTwinDiscReader = NULL;
	DDS_DiscoveredReader* pstDiscReader = NULL;
    if (NULL != pstTopic)
    {    
        /* ���������µ�ÿ��Զ���Ķ��� */
        pstDiscReader = pstTopic->pstDiscReader;
        while (NULL != pstDiscReader)
        {
            if (Guid_Is_Equal(&pstDiscReader->guid, pstGuid))
            {
                return pstDiscReader;
            }

            pstTwinDiscReader = pstDiscReader->pstTwinDiscReader;
            while (NULL != pstTwinDiscReader)
            {
                if (Guid_Is_Equal(&pstTwinDiscReader->guid, pstGuid))
                {
                    return pstTwinDiscReader;
                }

                pstTwinDiscReader = pstTwinDiscReader->pNext;
            }

            pstDiscReader = pstDiscReader->pNext;
        }
    }

    return NULL;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: GetDiscoveredReaderFromTopicByEntityId
-   ��������: ����Զ����
-   ��    ��: ���⡢Guid
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��������ͨ��Guid������ӦԶ���Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\8       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DiscoveredReader* GetDiscoveredReaderFromTopicByPrefix(DDS_Topic* pstTopic, GuidPrefix_t* pstPrefix)
{
	DDS_DiscoveredReader* pstDiscReader = NULL;
    if (NULL != pstTopic)
    {
        /* ���������µ�ÿ��Զ���Ķ��� */
        pstDiscReader = pstTopic->pstDiscReader;
        while (NULL != pstDiscReader)
        {
            if (GuidPrefix_Is_Equal(&pstDiscReader->guid.prefix, pstPrefix))
            {
                return pstDiscReader;
            }
            pstDiscReader = pstDiscReader->pNext;
        }
    }

    return NULL;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DeleteDiscWriterFromTopicByGuidPrefix
-   ��������: ɾ��Զ��д����
-   ��    ��: ���⡢Guid
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ������ɾ��ָ��Զ��д����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DeleteDiscWriterFromTopicByGuidPrefix(DDS_Topic* pstTopic, GuidPrefix_t*  pstPrefix)
{
    DDS_DiscoveredWriter* pstPrevDiscoveredWriter = NULL;
    DDS_DiscoveredWriter* pstDiscWriter = pstTopic->pstDiscWriter;

    while (NULL != pstDiscWriter)
    {
        if (GuidPrefix_Is_Equal(&pstDiscWriter->guid.prefix, pstPrefix))
        {
            if (!pstPrevDiscoveredWriter)
            {
                pstDiscWriter = pstDiscWriter->pNext;
                DDS_DiscoveredWriter_Uninit(pstTopic->pstDiscWriter);
                DDS_STATIC_FREE(pstTopic->pstDiscWriter);
                pstTopic->pstDiscWriter = pstDiscWriter;
            }
            else
            {
                pstPrevDiscoveredWriter->pNext = pstDiscWriter->pNext;
                DDS_DiscoveredWriter_Uninit(pstDiscWriter);
                DDS_STATIC_FREE(pstDiscWriter);
                pstDiscWriter = pstPrevDiscoveredWriter->pNext;
            }
        }
        else
        {
            pstPrevDiscoveredWriter = pstDiscWriter;
            pstDiscWriter = pstDiscWriter->pNext;
        }
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DeleteDiscReaderFromTopicByGuidPrefix
-   ��������: ɾ��Զ���Ķ���
-   ��    ��: ���⡢Guid
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ������ɾ��ָ��Զ���Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DeleteDiscReaderFromTopicByGuidPrefix(DDS_Topic* pstTopic, GuidPrefix_t*  pstPrefix)
{
    DDS_DiscoveredReader* pstPrevDiscoveredReader = NULL;
    DDS_DiscoveredReader* pstDiscReader = pstTopic->pstDiscReader;

    while (NULL != pstDiscReader)
    {
        if (GuidPrefix_Is_Equal(&pstDiscReader->guid.prefix, pstPrefix))
        {
            if (!pstPrevDiscoveredReader)
            {
                pstDiscReader = pstDiscReader->pNext;
                DDS_DiscoveredReader_Uninit(pstTopic->pstDiscReader);
                DDS_STATIC_FREE(pstTopic->pstDiscReader);
                pstTopic->pstDiscReader = pstDiscReader;
            }
            else
            {
                pstPrevDiscoveredReader->pNext = pstDiscReader->pNext;
                DDS_DiscoveredReader_Uninit(pstDiscReader);
                DDS_STATIC_FREE(pstDiscReader);
                pstDiscReader = pstPrevDiscoveredReader->pNext;
            }
        }
        else
        {
            pstPrevDiscoveredReader = pstDiscReader;
            pstDiscReader = pstDiscReader->pNext;
        }
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DeleteDiscWriterFromTopicByGuid
-   ��������: ɾ��Զ��д����
-   ��    ��: ���⡢Guid
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ������ɾ��ָ��Զ��д����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\03\19       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DeleteDiscWriterFromTopicByGuid(DDS_Topic* pstTopic, GUID_t*  pstGuid)
{
    DDS_DiscoveredWriter* pstPrevDiscoveredWriter = NULL;
    DDS_DiscoveredWriter* pstDiscWriter = pstTopic->pstDiscWriter;

    while (NULL != pstDiscWriter)
    {
        if (Guid_Is_Equal(&pstDiscWriter->guid, pstGuid))
        {
            if (!pstPrevDiscoveredWriter)
            {
                pstDiscWriter = pstDiscWriter->pNext;
                DDS_DiscoveredWriter_Uninit(pstTopic->pstDiscWriter);
                DDS_STATIC_FREE(pstTopic->pstDiscWriter);
                pstTopic->pstDiscWriter = pstDiscWriter;
            }
            else
            {
                pstPrevDiscoveredWriter->pNext = pstDiscWriter->pNext;
                DDS_DiscoveredWriter_Uninit(pstDiscWriter);
                DDS_STATIC_FREE(pstDiscWriter);
                pstDiscWriter = pstPrevDiscoveredWriter->pNext;
            }
        }
        else
        {
            pstPrevDiscoveredWriter = pstDiscWriter;
            pstDiscWriter = pstDiscWriter->pNext;
        }
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DeleteDiscReaderFromTopicByGuidPrefix
-   ��������: ɾ��Զ���Ķ���
-   ��    ��: ���⡢Guid
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ������ɾ��ָ��Զ���Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\03\19       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DeleteTwinDiscReaderFromTopicByGuid(DDS_DiscoveredReader* pstDiscReader, GUID_t* pstGuid)
{
    DDS_DiscoveredReader* pstPrevDiscoveredReader = NULL;
    DDS_DiscoveredReader* pstTwinDiscReader = pstDiscReader->pstTwinDiscReader;

    while (NULL != pstTwinDiscReader)
    {
        if (Guid_Is_Equal(&pstTwinDiscReader->guid, pstGuid))
        {
            if (!pstPrevDiscoveredReader)
            {
                pstTwinDiscReader = pstTwinDiscReader->pNext;
                DDS_DiscoveredReader_Uninit(pstDiscReader->pstTwinDiscReader);
                DDS_STATIC_FREE(pstDiscReader->pstTwinDiscReader);
                pstDiscReader->pstTwinDiscReader = pstTwinDiscReader;
            }
            else
            {
                pstPrevDiscoveredReader->pNext = pstTwinDiscReader->pNext;
                DDS_DiscoveredReader_Uninit(pstTwinDiscReader);
                DDS_STATIC_FREE(pstTwinDiscReader);
                pstTwinDiscReader = pstPrevDiscoveredReader->pNext;
            }
        }
        else
        {
            pstPrevDiscoveredReader = pstTwinDiscReader;
            pstTwinDiscReader = pstTwinDiscReader->pNext;
        }
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DeleteDiscReaderFromTopicByGuidPrefix
-   ��������: ɾ��Զ���Ķ���
-   ��    ��: ���⡢Guid
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ������ɾ��ָ��Զ���Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\03\19       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DeleteDiscReaderFromTopicByGuid(DDS_Topic* pstTopic, GUID_t* pstGuid)
{
    DDS_DiscoveredReader* pstPrevDiscoveredReader = NULL;
    DDS_DiscoveredReader* pstDiscReader = pstTopic->pstDiscReader;

    while (NULL != pstDiscReader)
    {
        DeleteTwinDiscReaderFromTopicByGuid(pstDiscReader, pstGuid);

        if (Guid_Is_Equal(&pstDiscReader->guid, pstGuid))
        {
            if (!pstPrevDiscoveredReader)
            {
                if (NULL != pstDiscReader->pstTwinDiscReader)
                {
                    TwinDiscoveredReaderChangePosition(pstDiscReader);
                    return;
                }

                pstTopic->pstDiscReader = pstDiscReader->pNext;
                DDS_DiscoveredReader_Uninit(pstDiscReader);
                DDS_STATIC_FREE(pstDiscReader);
                pstDiscReader = pstTopic->pstDiscReader;
                return;
                
            }
            else
            {
                if (NULL != pstDiscReader->pstTwinDiscReader)
                {
                    TwinDiscoveredReaderChangePosition(pstDiscReader);
                    return;
                }

                pstPrevDiscoveredReader->pNext = pstDiscReader->pNext;
                DDS_DiscoveredReader_Uninit(pstDiscReader);
                DDS_STATIC_FREE(pstDiscReader);
                pstDiscReader = pstPrevDiscoveredReader->pNext;
                return;
            }
        }
        else
        {
            pstPrevDiscoveredReader = pstDiscReader;
            pstDiscReader = pstDiscReader->pNext;
        }
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: FindOwnerWriter
-   ��������: ��ȡ����Ȩ��ߵ�Զ��д����
-   ��    ��: ����
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ����Ȩֵ��ͬ��ѡ��GUIDֵ����д����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\12\30       1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
void FindOwnerWriter(DDS_Topic* pstTopic)
{
	DDS_DataReader* localReader = NULL;
	DDS_DiscoveredWriter* pstOwnerWriter = NULL;
	DDS_DiscoveredWriter* pstDiscWriter = NULL;
    if (NULL == pstTopic)
        return;

    localReader = pstTopic->pstLocalReader;

    while (NULL != localReader)
    {
        /* Nephalem ������������Ȩ���� */
        if (SHARED_OWNERSHIP_QOS == localReader->stDataReaderQos.ownership.kind)
        {
            localReader = localReader->pNext;
            continue;
        }

        pstDiscWriter = pstTopic->pstDiscWriter;
        while (NULL != pstDiscWriter)
        {
            /* Nephalem reliability��ƥ�䣬���� */
            if (RELIABLE_RELIABILITY_QOS == localReader->stDataReaderQos.reliability.kind
                && BEST_EFFORT_RELIABILITY_QOS == pstDiscWriter->stDataWriterQos.reliability.kind)
            {
                pstDiscWriter = pstDiscWriter->pNext;
                continue;
            }

            /* Nephalem ������������Ȩ���� */
            if (SHARED_OWNERSHIP_QOS == pstDiscWriter->stDataWriterQos.ownership.kind)
            {
                pstDiscWriter = pstDiscWriter->pNext;
                continue;
            }

			/* Nephalem ����ͬһ���� ���� */
			if (FALSE == pstDiscWriter->bSamePartiton)
			{
				pstDiscWriter = pstDiscWriter->pNext;
				continue;
			}

            /* Nephalem ���ֵ�һ����ռ����д���� */
            if (NULL == pstOwnerWriter)
            {
                pstOwnerWriter = pstDiscWriter;
                pstDiscWriter = pstDiscWriter->pNext;
                continue;
            }

            /* Nephalem ѡ���������Ȩ��д���� */
            if (pstDiscWriter->stDataWriterQos.ownership_strength.value > pstOwnerWriter->stDataWriterQos.ownership_strength.value)
            {
                pstOwnerWriter = pstDiscWriter;
                pstDiscWriter = pstDiscWriter->pNext;
                continue;
            }

            /* Nephalme ��ͬ����Ȩ��ѡ��Guid�ϴ��д����*/
            if (pstDiscWriter->stDataWriterQos.ownership_strength.value == pstOwnerWriter->stDataWriterQos.ownership_strength.value)
            {
                if (Guid_Is_Bigger(&pstDiscWriter->guid, &pstOwnerWriter->guid))
                {
                    pstOwnerWriter = pstDiscWriter;
                    pstDiscWriter = pstDiscWriter->pNext;
                    continue;
                }
            }
            pstDiscWriter = pstDiscWriter->pNext;
        }

        localReader->pstOwnerDiscWriter = pstOwnerWriter;

        localReader = localReader->pNext;
    }
}


/*---------------------------------------------------------------------------------
-   �� �� ��: EqualPartitionName
-   ��������: �ж��Ƿ�����ͬ�ķ�����
-   ��    ��: pstTopicQoSL pstTopicQoSR
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: 
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\1\21       1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
BOOL EqualPartitionName(DDS_TopicQos* pstTopicQoSL, DDS_TopicQos* pstTopicQoSR)
{
	PartitionName* pstPartitionNameL = NULL;
	PartitionName* pstPartitionNameR = NULL;
	BOOL LWithAsteriskW;
	BOOL RWithAsteriskW;
    /* Nephalem partition ˫�����޷��� */
    if (0 == pstTopicQoSL->partition.size && 0 == pstTopicQoSR->partition.size)
        return TRUE;

    pstPartitionNameL = pstTopicQoSL->partition.pValue;
    
    while (NULL != pstPartitionNameL)
    {
        /* Nephalem �жϷ������Ƿ���ͨ��� */
        LWithAsteriskW = containAsteriskW(pstPartitionNameL->partitionValue.value);

        pstPartitionNameR = pstTopicQoSR->partition.pValue;
        while (NULL != pstPartitionNameR)
        {
            /* Nephalem �жϷ������Ƿ���ͨ��� */
            RWithAsteriskW = containAsteriskW(pstPartitionNameR->partitionValue.value);

            if (TRUE == LWithAsteriskW && TRUE == RWithAsteriskW)
            {
                /* ������ͨ��� �����бȽ� */
            }

            if (TRUE == LWithAsteriskW && TRUE != RWithAsteriskW)
            {
                if (TRUE == matchStr(pstPartitionNameR->partitionValue.value, pstPartitionNameL->partitionValue.value))
                    return TRUE;
            }

            if (TRUE != LWithAsteriskW && TRUE == RWithAsteriskW)
            {
                if (TRUE == matchStr(pstPartitionNameL->partitionValue.value, pstPartitionNameR->partitionValue.value))
                    return TRUE;
            }

            if (TRUE != LWithAsteriskW && TRUE != RWithAsteriskW)
            {
                if (0 == strcmp(pstPartitionNameL->partitionValue.value, pstPartitionNameR->partitionValue.value))
                    return TRUE;
            }

            pstPartitionNameR = pstPartitionNameR->pNext;
        }
        pstPartitionNameL = pstPartitionNameL->pNext;
    }

    return FALSE;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: matchStr
-   ��������: �ж��ַ����Ƿ���ȣ�֧��ͨ���*�ͣ�
-   ��    ��: pstStr pstPattern
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��:
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\1\21       1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
BOOL matchStr(char* pstStr, char* pstPattern)
{
	int iMark = 0;
    int iStrPos = 0;
    int iPatternPos = 0;
	int iStrLen = 0;
	int iPatternLen =0;
    if (NULL == pstStr || NULL == pstPattern)
        return FALSE;

    iStrLen = strlen(pstStr);
    iPatternLen = strlen(pstPattern);


    while (iStrPos < iStrLen && iPatternPos < iPatternLen)
    {
        if ('?' == pstPattern[iPatternPos])
        {
            iStrPos++;
            iPatternPos++;
            continue;
        }

        if ('*' == pstPattern[iPatternPos])
        {
            iPatternPos++;
            iMark = iPatternPos;
            continue;
        }

        if (pstStr[iStrPos] != pstPattern[iPatternPos])
        {
            if (0 == iStrPos && 0 == iPatternPos)
                return FALSE;

            iStrPos -= iPatternPos - iMark - 1;
            iPatternPos = iMark;
            continue;
        }

        iStrPos++;
        iPatternPos++;
    }

    if (iPatternPos == iPatternLen)
    {
        if (iStrPos == iStrLen)
            return TRUE;

        if ('*' == pstPattern[iPatternPos - 1])
            return TRUE;
    }

    while (iPatternPos < iPatternLen)
    {
        if ('*' != pstPattern[iPatternPos])
            return FALSE;
        iPatternPos++;
    }

    return TRUE;
}


/*---------------------------------------------------------------------------------
-   �� �� ��: containAsteriskW
-   ��������: �ж��ַ����Ƿ���ͨ���*�ͣ�
-   ��    ��: pstStr pstPattern
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��:
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\1\21       1.0         Nephalem      Created
----------------------------------------------------------------------------------*/
BOOL containAsteriskW(char* pstStr)
{
	int i = 0; 
	int iStrLen;
    if (NULL == pstStr)
        return FALSE;

    iStrLen = strlen(pstStr);

    for (i = 0; i < iStrLen; i++)
    {
        if ('*' == pstStr[i] || '?' == pstStr[i])
            return TRUE;
    }

    return FALSE;
}
