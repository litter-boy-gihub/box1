#include "../include/GlobalDefine.h"

/* HANDLE g_hDev = NULL; */
#if defined _FC
extern YCCommFCConfig g_commFCConfig;
#elif defined _653_PORT
extern Port653Config g_PortConfig; 
#endif

#ifdef  STATIC_DISCOVERY
extern STATICINOF g_stStaticInfo;
extern int g_iLocalStaticIndex;
#endif

BOOL InitNetWorkStructure(DDS_DomainParticipant*  pstDomainParticipant, USHORT domainId)
{
    BoundedString256 ipList;
	UINT32 recvMulticastport;
	UINT32 recvUnicastPort;
	UINT32 recvUserUnicastPort;
	int participantId;
	Locator_t* SPDPMulticastLocator = NULL;
	Locator_t* SEDPUnicastLocator = NULL;
	Locator_t* USERUnicastLocator = NULL;
	
    SPDPMulticastLocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
    if (NULL == SPDPMulticastLocator)
    {
        return FALSE;
    }

    SEDPUnicastLocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
    if (NULL == SEDPUnicastLocator)
    {
		DDS_STATIC_FREE(SPDPMulticastLocator);
        return FALSE;
    }

    USERUnicastLocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
    if (NULL == USERUnicastLocator)
    {
		DDS_STATIC_FREE(SPDPMulticastLocator);
		DDS_STATIC_FREE(SEDPUnicastLocator);
        return FALSE;
    }
    
#if defined _FC //ʹ��FC
//	FCInit();
#ifdef STATIC_DISCOVERY //��̬���ýڵ㣬����Ҫ����SPDP
	//ǰ׺id��Ҫ�Ӿ�̬�������ܹ�ƴ�ճ���
	GenStaticPrefix(g_stStaticInfo.stStaticApps[g_iLocalStaticIndex], &pstDomainParticipant->stRTPSMsgHeader.guidPrefix);
#else
    /* ��ʼ�����ر�����ȫ����Ϣ*/
    participantId = pstDomainParticipant->processId;
    participantId >>= 2;
    InitializeDiscoveryInfo(ipList.value, participantId);
	GenPrefix(pstDomainParticipant, AUTO_GUID_FROM_IP);
#endif
	SPDPMulticastLocator->kind = LOCATOR_KIND_FC;
	SPDPMulticastLocator->port = g_commFCConfig.m_thisNodeFCPort;
    
    SEDPUnicastLocator->kind = LOCATOR_KIND_FC;
    SEDPUnicastLocator->port = g_commFCConfig.m_thisNodeFCPort;
    memset(SEDPUnicastLocator->address,0,LOCATOR_ADDR_STR_LENGTH);
    SEDPUnicastLocator->address[2] = 0x28;
    SEDPUnicastLocator->address[1] = g_commFCConfig.m_thisNodeFCPort & 0xFF;
    
    USERUnicastLocator->kind = LOCATOR_KIND_FC;
    USERUnicastLocator->port = g_commFCConfig.m_thisNodeFCPort;
    memset(USERUnicastLocator->address,0,LOCATOR_ADDR_STR_LENGTH);
    USERUnicastLocator->address[2] = 0x28;
    USERUnicastLocator->address[1] = g_commFCConfig.m_thisNodeFCPort & 0xFF;

#elif defined _653_PORT //_FC
    
    PortInit();
    
    /* ��ʼ�����ر�����ȫ����Ϣ*/
    participantId = pstDomainParticipant->processId;
    participantId >>= 2;
    InitializeDiscoveryInfo(ipList.value, participantId);
	GenPrefix(pstDomainParticipant, AUTO_GUID_FROM_IP);
	
	SPDPMulticastLocator->kind = LOCATOR_KIND_UDPv4;
	SPDPMulticastLocator->port = g_PortConfig.m_localPartitionId;
    
    SEDPUnicastLocator->kind = LOCATOR_KIND_UDPv4;
    SEDPUnicastLocator->port = g_PortConfig.m_localPartitionId;
    
    USERUnicastLocator->kind = LOCATOR_KIND_UDPv4;
    USERUnicastLocator->port = g_PortConfig.m_localPartitionId;
    
#else //_653_PORT
    /* ��ȡ���������ַ */
    if (0 == getHostIPv4AddrList(&ipList))
    {
        DDS_STATIC_FREE(SPDPMulticastLocator);
        DDS_STATIC_FREE(SEDPUnicastLocator);
        DDS_STATIC_FREE(USERUnicastLocator);
        return FALSE;
    }

    printf("Nephalem Localip %s \n",ipList.value);
#ifdef STATIC_DISCOVERY //��̬���ýڵ㣬����Ҫ����SPDP
	if (g_iLocalStaticIndex < 0)
	{
		printf("δ�ҵ���̬���ýڵ���Ϣ\n");
		return FALSE;
	}
	recvUnicastPort = g_stStaticInfo.stStaticApps[g_iLocalStaticIndex].usPort[0];//0��ʾ�����˿ں�
	recvUserUnicastPort = g_stStaticInfo.stStaticApps[g_iLocalStaticIndex].usPort[1];//1��ʾ�û�USER�����˿ں�
	//ǰ׺id��Ҫ�Ӿ�̬�������ܹ�ƴ�ճ���
	GenStaticPrefix(g_stStaticInfo.stStaticApps[g_iLocalStaticIndex], &pstDomainParticipant->stRTPSMsgHeader.guidPrefix);
#else
	/* ��ʼ�����ر�����ȫ����Ϣ*/
	participantId = pstDomainParticipant->processId;
	participantId >>= 2; //ΪʲôҪ��λ��
	InitializeDiscoveryInfo(ipList.value, participantId);
	GenPrefix(pstDomainParticipant, AUTO_GUID_FROM_IP);

    /* ����SPDP�鲥�˿ںţ���RTPS v2.1 ��9.8 */
    recvMulticastport = g_rtps_discovery_info.pb + (g_rtps_discovery_info.dg * domainId) + g_rtps_discovery_info.d0;
    /* ����SEDP�����˿� */
    recvUnicastPort = g_rtps_discovery_info.pb + (g_rtps_discovery_info.dg * domainId) + g_rtps_discovery_info.rootParticipantId * g_rtps_discovery_info.pg  + g_rtps_discovery_info.d1;
	recvUserUnicastPort = ++recvUnicastPort;

	/* SPDP���Ͷ˳�ʼ�� */
	if (!InitSendSocketInfo(&pstDomainParticipant->socketList.spdp_data_sender, ipList.value, 0, 1))
	{
		DDS_STATIC_FREE(SPDPMulticastLocator);
		DDS_STATIC_FREE(SEDPUnicastLocator);
		DDS_STATIC_FREE(USERUnicastLocator);
		return FALSE;
	}

	/* SPDP���ն˳�ʼ�� */
	if (!InitMulticastReceiveSocketInfo(&pstDomainParticipant->socketList.spdp_data_receiver, ipList.value, recvMulticastport, g_rtps_discovery_info.groupAddr, SPDPMulticastLocator))
	{
		DDS_STATIC_FREE(SPDPMulticastLocator);
		DDS_STATIC_FREE(SEDPUnicastLocator);
		DDS_STATIC_FREE(USERUnicastLocator);
		return FALSE;
	}
#endif

    /* SEDP���Ͷ˳�ʼ�� */
    if (!InitSendSocketInfo(&pstDomainParticipant->socketList.sedp_data_sender, ipList.value, 0, 0))
    {
		DDS_STATIC_FREE(SPDPMulticastLocator);
		DDS_STATIC_FREE(SEDPUnicastLocator);
		DDS_STATIC_FREE(USERUnicastLocator);
        return FALSE;
    }

    /* SEDP���ն˳�ʼ�� */
    if (!InitUnicastReceiveSocketInfo(&pstDomainParticipant->socketList.sedp_data_receiver, ipList.value, recvUnicastPort, SEDPUnicastLocator))
    {
		DDS_STATIC_FREE(SPDPMulticastLocator);
		DDS_STATIC_FREE(SEDPUnicastLocator);
		DDS_STATIC_FREE(USERUnicastLocator);
        return FALSE;
    }

    /* USER���Ͷ˳�ʼ�� */
    if (!InitSendSocketInfo(&pstDomainParticipant->socketList.user_data_sender, ipList.value, 0, 0))
    {
		DDS_STATIC_FREE(SPDPMulticastLocator);
		DDS_STATIC_FREE(SEDPUnicastLocator);
		DDS_STATIC_FREE(USERUnicastLocator);
        return FALSE;
    }

    /* USER���ն˳�ʼ�� */
    if (!InitUnicastReceiveSocketInfo(&pstDomainParticipant->socketList.user_data_receiver, ipList.value, recvUserUnicastPort, USERUnicastLocator))
    {
		DDS_STATIC_FREE(SPDPMulticastLocator);
		DDS_STATIC_FREE(SEDPUnicastLocator);
		DDS_STATIC_FREE(USERUnicastLocator);
        return FALSE;
    }
#endif //��̫��
    LIST_INSERT_HEAD(pstDomainParticipant->rtpsParticipantProxy.metatrafficMulticastLocator, SPDPMulticastLocator);

    LIST_INSERT_HEAD(pstDomainParticipant->rtpsParticipantProxy.metatrafficUnicastLocator, SEDPUnicastLocator);

    LIST_INSERT_HEAD(pstDomainParticipant->rtpsParticipantProxy.defaultUnicastLocator, USERUnicastLocator);

#ifdef RAPID_IO_CHANNEL

    if (g_RapdIOValid)
    {
        /* ���û�ȡ�����豸id */
        if (!GetStrValue("rapidio", "location", ipList.value))
        {
            printf("localId not exist!\n");
            return TRUE;
        }

        Locator_t* SPDPMulticastLocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
        if (NULL == SPDPMulticastLocator)
        {
            return TRUE;
        }

        Locator_t* SEDPUnicastLocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
        if (NULL == SEDPUnicastLocator)
        {
            return TRUE;
        }

        Locator_t* USERUnicastLocator = (Locator_t*)DDS_STATIC_MALLOC(sizeof(Locator_t));
        if (NULL == USERUnicastLocator)
        {
            return TRUE;
        }

        strncpy(SPDPMulticastLocator->address, ipList.value, strlen(ipList.value));
        SPDPMulticastLocator->port = 0;

		strncpy(SEDPUnicastLocator->address, ipList.value, strlen(ipList.value));
        SEDPUnicastLocator->port = 0;

        strncpy(USERUnicastLocator->address, ipList.value, strlen(ipList.value));
        USERUnicastLocator->port = 0;

        LIST_INSERT_HEAD(pstDomainParticipant->rtpsParticipantProxy.metatrafficMulticastLocator, SPDPMulticastLocator);

        LIST_INSERT_HEAD(pstDomainParticipant->rtpsParticipantProxy.metatrafficUnicastLocator, SEDPUnicastLocator);

        LIST_INSERT_HEAD(pstDomainParticipant->rtpsParticipantProxy.defaultUnicastLocator, USERUnicastLocator);
    }
#endif //RAPID_IO_CHANNEL

    return TRUE;

}

BOOL UnInitNetWorkStructure(DDS_DomainParticipant*  pstDomainParticipant)
{
	closesocket(pstDomainParticipant->socketList.sedp_data_receiver.socket);
	closesocket(pstDomainParticipant->socketList.sedp_data_sender.socket);
	closesocket(pstDomainParticipant->socketList.spdp_data_receiver.socket);
	closesocket(pstDomainParticipant->socketList.spdp_data_sender.socket);
	closesocket(pstDomainParticipant->socketList.user_data_receiver.socket);
	closesocket(pstDomainParticipant->socketList.user_data_sender.socket);
	return TRUE;
}


// BOOL getNetMask(DDS_DomainParticipant* pstDomainParticipant, char* ipAddr)
// {
//     char* szMark = DDS_STATIC_MALLOC(16);
//     PIP_ADAPTER_INFO pAdapterInfo;
//     PIP_ADAPTER_INFO pAdapter = NULL;
//     DWORD dwRetVal = 0;
//     pAdapterInfo = DDS_STATIC_MALLOC(sizeof(IP_ADAPTER_INFO));
//     ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
//     if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
//     {
//         DDS_STATIC_FREE(pAdapterInfo);
//         pAdapterInfo = DDS_STATIC_MALLOC(ulOutBufLen);
//     }
//     if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
//     {
//         pAdapter = pAdapterInfo;
//     }
// 
//     while (&pAdapter != NULL)
//     {
//         if (FuzzyMatch(&pAdapter->IpAddressList.IpAddress, ipAddr))
//         {
//             memcpy(szMark, &pAdapter->IpAddressList.IpMask, 16);
//             pstDomainParticipant->netMask = htonl(ntohl(inet_addr(szMark)));
//             DDS_STATIC_FREE(szMark);
//             DDS_STATIC_FREE(pAdapterInfo);
//             return TRUE;
//         }
//         pAdapter = pAdapter->Next;
//     }
// }
