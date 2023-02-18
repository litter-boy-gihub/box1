#include "../include/GlobalDefine.h"
 
/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_DiscoveredParticipant_Init
-   ��������: ��ʼ��SPDP���ֹ���
-   ��    ��: SPDPDiscoveredParticipantָ��
-   ��    ��: 
-   ȫ�ֱ���:
-   ע    ��: SPDPDiscoveredParticipant��Աָ�븳NULL
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\04       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DDS_DiscoveredParticipant_Init(DDS_DiscoveredParticipant* pstDiscoveredParticipant)
{
    if (NULL != pstDiscoveredParticipant)
    {
        /* ����ָ�붼����NULL */
        pstDiscoveredParticipant->rtpsParticipantProxy.metatrafficUnicastLocator = NULL;
        pstDiscoveredParticipant->rtpsParticipantProxy.metatrafficMulticastLocator = NULL;
        pstDiscoveredParticipant->rtpsParticipantProxy.defaultUnicastLocator = NULL;
        pstDiscoveredParticipant->rtpsParticipantProxy.defaultMulticastLocator = NULL;
        pstDiscoveredParticipant->pNext = NULL;
		pstDiscoveredParticipant->leaseDuration.sec = 0;
		pstDiscoveredParticipant->leaseDuration.nanosec = 0;
		pstDiscoveredParticipant->lastHeartBeat = pstDiscoveredParticipant->leaseDuration;
		pstDiscoveredParticipant->livelinessHeartMissNum = 0;
		pstDiscoveredParticipant->IPAddr = 0;
//        pstDiscoveredParticipant->netMask = 0;
		pstDiscoveredParticipant->processID = 0;
		DDS_MUTEX_INIT(pstDiscoveredParticipant->threadMutex);

#ifdef  STATIC_DISCOVERY
		pstDiscoveredParticipant->bOnlyStaticMathc = TRUE;  //??��LJM
#endif

    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DDS_DiscoveredParticipant_Uninit
-   ��������: ����SPDP���ֹ���
-   ��    ��: ����Participantָ��
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��������Participant��Աָ��
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\04       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DDS_DiscoveredParticipant_Uninit(DDS_DiscoveredParticipant* pstDiscoveredParticipant)
{
	Locator_t* pstTempNode = NULL;
    if (NULL != pstDiscoveredParticipant)
    {
        pstTempNode = NULL;
        /* ���г�Աָ������ */
        LIST_DELETE_ALL(pstDiscoveredParticipant->rtpsParticipantProxy.metatrafficUnicastLocator, pstTempNode);
        LIST_DELETE_ALL(pstDiscoveredParticipant->rtpsParticipantProxy.metatrafficMulticastLocator, pstTempNode);
        LIST_DELETE_ALL(pstDiscoveredParticipant->rtpsParticipantProxy.defaultUnicastLocator, pstTempNode);
        LIST_DELETE_ALL(pstDiscoveredParticipant->rtpsParticipantProxy.defaultMulticastLocator, pstTempNode);
    }
}
