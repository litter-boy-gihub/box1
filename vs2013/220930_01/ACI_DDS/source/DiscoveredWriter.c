#include "../include/GlobalDefine.h"

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_DiscoveredWriter_Init
-   ��������: ��ʼ��Զ��д����
-   ��    ��: ����д����
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��ʼ��Զ��д����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\8       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DDS_DiscoveredWriter_Init(DDS_DiscoveredWriter * pstDiscWriter)
{
    if (NULL != pstDiscWriter)
    {
        pstDiscWriter->pstUnicastLocator = NULL;
        pstDiscWriter->pNext = NULL;
        pstDiscWriter->stAckNack.count = 0;
        pstDiscWriter->stAckNack.readerSNState.base = SEQUENCENUMBER_START;
        pstDiscWriter->stHeartbeat.count = 0;
        pstDiscWriter->lastHeartTime.sec = 0;
        pstDiscWriter->lastHeartTime.nanosec = 0;
        pstDiscWriter->heartMsgNum = 0;
		pstDiscWriter->durabilityFlag = FALSE;
        InitHistoryCache(&pstDiscWriter->stHistoryCache);
        /* JDXCZ 给其中的DWQos进行初始化，防止初始值和默认值不同，影响匹配*/
        memcpy(&(pstDiscWriter->stDataWriterQos), &DDS_DATAWRITER_QOS_DEFAULT, sizeof(DDS_DataWriterQos));

#if defined QYDDS_MASSDATA
        /*Nephalem massdata ��������ʼ��*/
        InitHistoryCache(&pstDiscWriter->stMassDataHistoryCache);
#endif 
        pstDiscWriter->pstTopic = NULL;
        pstDiscWriter->bSamePartiton = TRUE;
		pstDiscWriter->lastLivelinessTime = TIME_ZERO;
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_DiscoveredWriter_Uninit
-   ��������: ȥʼ��Զ��д����
-   ��    ��: ����д����
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ȥʼ��Զ��д����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\8       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DDS_DiscoveredWriter_Uninit(DDS_DiscoveredWriter * pstDiscWriter)
{
    if (NULL != pstDiscWriter)
    {
        /* ������������*/
        UninitHistoryCache(&pstDiscWriter->stHistoryCache);

        //Locator_t* pstTempNode = NULL;
        ///* ����������ַ */
        //LIST_DELETE_ALL(pstDiscWriter->unicastLocator, pstTempNode);

        ///* �����鲥��ַ */
        //LIST_DELETE_ALL(pstDiscWriter->multicastLocator, pstTempNode);

#if defined QYDDS_MASSDATA
        /*Nephalem massdata ����������*/
        UninitHistoryCache(&pstDiscWriter->stMassDataHistoryCache);
#endif 

        /* Nephalem �ڴ��ͷ� */
         if (NULL != pstDiscWriter->pstRelateTopic)
         {
             if (pstDiscWriter->pstTopic != pstDiscWriter->pstRelateTopic)
             {
                 UnInitTopicPartitionQoS(&pstDiscWriter->pstRelateTopic->stTopicQos);
                 DDS_STATIC_FREE(pstDiscWriter->pstRelateTopic);
                 pstDiscWriter->pstRelateTopic = NULL;
             }
         }        
    }
}
