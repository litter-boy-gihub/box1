#include "../include/GlobalDefine.h"

 //_CrtMemState g_startMemState;
 //_CrtMemState g_endMemState;
 //_CrtMemState g_diffMemState;

 CHAR g_ipAddr[32] = "";
 CHAR g_netCardName[256] = "";

extern int iSPDPFCSEND;

extern int iUserFCSEND;

#ifdef  STATIC_DISCOVERY
extern STATICINOF g_stStaticInfo;
extern int g_iLocalStaticIndex;
#endif
 /* �ڴ�й©��� */
 //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //_CrtMemCheckpoint(&g_startMemState);
 //CHECK_MEMORY();
/* ����ѡ�� 
 * 1 ����ʱ�Ӳ���
 * 2 
 */
int iTestOptions = 0;

void HiperDGetHostName(BoundedString256 * name)
{
    //��ȡ������������
#if defined _TM1 || defined _TM3
	strcpy(name->value, "admin");
#else
    if (gethostname(name->value, 256) < 0)
    {
        printf("Error when getting local host name.");
        return ;
    }
#endif
    name->length = (UINT32)strlen(name->value);
}

BOOL FuzzyMatch(char* srcStr, char*destStr)
{
    unsigned int i = 0;
    char tmpStr[32];
    for (i = 0; i < 32; i++)
    {
        if ('*' == destStr[i])
        {
            tmpStr[i] = '\0';
            break;
        }
        tmpStr[i] = destStr[i];
    }

    if (!strstr(srcStr, tmpStr))
    {
        return FALSE;
    }

    return TRUE;
}

// ��ȡ������IP��ַ�б�
int getHostIPv4AddrList(BoundedString256 * ipAddr)
{
    int listSize = 0;
	int i = 0;
	char host_name[255];
	char info[1024 * 8];
#ifdef STATIC_DISCOVERY //��̬����
	if (g_iLocalStaticIndex >= 0)
	{
		sprintf(ipAddr->value, "%u.%u.%u.%u", (g_stStaticInfo.stStaticApps[g_iLocalStaticIndex].iStaticAddr >> 24 & 0xff),
			(g_stStaticInfo.stStaticApps[g_iLocalStaticIndex].iStaticAddr >> 16 & 0xff),
			(g_stStaticInfo.stStaticApps[g_iLocalStaticIndex].iStaticAddr >> 8 & 0xff),
			(g_stStaticInfo.stStaticApps[g_iLocalStaticIndex].iStaticAddr & 0xff));
		return 1;
	}
		
#endif
#if defined _TM1 || defined _TM3

#else
	struct in_addr addr;
#endif
	
#if defined _FC
	sprintf(ipAddr->value,"%u.%u.%u.%u", *((char*)&g_commFCConfig.m_thisNodeFCPort) + 192,*(((char*)&g_commFCConfig.m_thisNodeFCPort)+1)+168,
			*(((char*)&g_commFCConfig.m_thisNodeFCPort)+2)+1,*(((char*)&g_commFCConfig.m_thisNodeFCPort)+3));
	ipAddr->length = strlen(ipAddr->value);
	printf("FCPort = %x , ip = %s\n",g_commFCConfig.m_thisNodeFCPort,ipAddr->value);
	listSize++;
#elif defined _653_PORT

	sprintf(ipAddr->value,"192.168.1.%u", g_PortConfig.m_localPartitionId+1);
	ipAddr->length = strlen(ipAddr->value);
	printf("%x  %s",g_PortConfig.m_localPartitionId,ipAddr->value);
	listSize++;
	
#else
#if defined(_WIN32)
    // ��ʼ��winsock
    WSADATA dat;
	struct hostent *phe;
    PIP_ADAPTER_INFO pIpAdapterInfo = (PIP_ADAPTER_INFO)info;
    unsigned long stSize = 1024 * 8;
    int nRel;
    IP_ADDR_STRING* pIpAddrString;
    int IPnumPerNetCard = 0;

    if (0 != WSAStartup(MAKEWORD(2, 2), &dat))
    {
        printf("��ʼ��winsocketʧ�ܣ�");
    }
    
    ////��ȡ������������
    //if (gethostname(host_name, sizeof(host_name)) < 0)
    //{
    //    printf("Error when getting local host name.");
    //    return 0;
    //}
    //phe = gethostbyname(host_name);
    //if (phe == 0)
    //{
    //    printf("Bad host lookup.");
    //    return 0;
    //}

    /* ѭ���ó����ػ�������IP��ַ */
    /* ֻʹ�õ�һ������ */
  //  for (i = 0; phe->h_addr_list[i] != 0; ++i)
  //  {
  //      memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
  //      ipList->length = (UINT32)(strlen(inet_ntoa(addr)) + 1);
		//if (!strcmp(g_ipAddr, "127.0.0.1"))
		//{
		//	memcpy(ipList->value, g_ipAddr, 32);
		//	return listSize;
		//}

  //      if (!strcmp(g_ipAddr, ""))
  //      {
  //          memcpy(ipList->value, inet_ntoa(addr), ipList->length);
  //          listSize++;
  //          return listSize;
  //      }
  //      else
  //      {
  //          if (FuzzyMatch(inet_ntoa(addr), g_ipAddr))
  //          {
  //              memcpy(ipList->value, inet_ntoa(addr), ipList->length);
  //              listSize++;
  //              return listSize;
  //          }
  //      }
  //  }

	//IP_ADAPTER_INFO�ṹ��洢����������Ϣ
	//�õ��ṹ���С,����GetAdaptersInfo����
	//����GetAdaptersInfo����,���pIpAdapterInfoָ�����;����stSize��������һ��������Ҳ��һ�������
	nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);

	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		printf("Network Card not exist!\n");
		return listSize;
	}

	//���������Ϣ
	//�����ж�����,���ͨ��ѭ��ȥ�ж�
	while (pIpAdapterInfo)
	{
		//printf("AdapterName : %s \n", pIpAdapterInfo->AdapterName);
		//printf("Description : %s \n", pIpAdapterInfo->Description);

		// ��������������for VMWare MAC:00-50-56-xx-xx
		if (pIpAdapterInfo->Address[0] == 0x00
			&& pIpAdapterInfo->Address[1] == 0x50
			&& pIpAdapterInfo->Address[2] == 0x56)
		{
			pIpAdapterInfo = pIpAdapterInfo->Next;
			continue;
		}
		// ��������������for VirtualBox MAC:08-00-27-xx-xx
		if (pIpAdapterInfo->Address[0] == 0x08
			&& pIpAdapterInfo->Address[1] == 0x00
			&& pIpAdapterInfo->Address[2] == 0x27)
		{
			pIpAdapterInfo = pIpAdapterInfo->Next;
			continue;
		}

		//����������������ƣ�������ҵ���Ӧ����
		if (0 != strcmp(g_netCardName, "") && 0 != strcmp(g_netCardName, pIpAdapterInfo->Description))
		{
			pIpAdapterInfo = pIpAdapterInfo->Next;
			continue;
		}

		pIpAddrString = &(pIpAdapterInfo->IpAddressList);
		do
		{
			//printf("ip count = %d \n", ++IPnumPerNetCard);
			//printf("ip = %s \n", pIpAddrString->IpAddress.String);
			//printf("IpMask = %s \n", pIpAddrString->IpMask.String);
			//printf("GatewayList = %s \n", pIpAdapterInfo->GatewayList.IpAddress.String);

			if (0 != strcmp(g_ipAddr, "") && 0 != strcmp(g_ipAddr, pIpAddrString->IpAddress.String))
			{
				pIpAddrString = pIpAddrString->Next;
				continue;
			}

			/* Nephalem ���˻ػ���ַ */
			if (0 == strcmp("127.0.0.1", pIpAddrString->IpAddress.String) 
				|| 0 == strcmp("0.0.0.0", pIpAddrString->IpAddress.String))
			{
				pIpAddrString = pIpAddrString->Next;
				continue;
			}

			printf("ip = %s \n", pIpAddrString->IpAddress.String);
			ipAddr->length = strlen(pIpAddrString->IpAddress.String) + 1;
			memcpy(ipAddr->value, pIpAddrString->IpAddress.String, ipAddr->length);
			listSize++;

			return listSize;

		} while (pIpAddrString);
		pIpAdapterInfo = pIpAdapterInfo->Next;
	}
	/* û�в�ѯ����Чip��ַ��Ĭ��Ϊ�ػ���ַ����ֹû����ʱ���� */
	strcpy(ipAddr->value, "127.0.0.1"); // Ĭ��ֵ
	listSize++;
#elif defined(__linux__)
    struct ifaddrs* ifAddrStruct = NULL;
    void* tmpAddrPtr = NULL;
    // ��ȡ��������
    getifaddrs(&ifAddrStruct);
    if (ifAddrStruct == NULL)
    {
        printf("Error when getifaddrs.\n");
        return -1;
    }
    struct ifaddrs* ifaddrIter = ifAddrStruct;
    char tmpIP[16] = { 0 };
    while (ifaddrIter != NULL)
    {
		if (NULL != ifaddrIter->ifa_addr)
		{
			//CAN�ӿ�Ҳ����socket, ����CANû��IP��ַ, ifa_addrΪ�գ�����Ҫ�����ж�, �������ֲ���Ұָ������������Segmentation fault��
			if (ifaddrIter->ifa_addr->sa_family == AF_INET)
			{
				// ��ipv4
				if (strcmp(ifaddrIter->ifa_name, "lo") == 0)
				{
					// �������ػ���
					ifaddrIter = ifaddrIter->ifa_next;
					continue;
				}
				tmpAddrPtr = &((struct sockaddr_in *)ifaddrIter->ifa_addr)->sin_addr;
				inet_ntop(AF_INET, tmpAddrPtr, tmpIP, 16);
				//TODO:add for vxworks,ȥ��lo
				if (strcmp(tmpIP, "127.0.0.1") == 0)
				{
					ifaddrIter = ifaddrIter->ifa_next;
					continue;
				}
				ipAddr->length = 16;
				memcpy(ipAddr->value, tmpIP, 16);
				listSize++;
			}
		}
        ifaddrIter = ifaddrIter->ifa_next;
    }
    freeifaddrs(ifAddrStruct);
#elif defined(_VXWORKS)
    //��ȡ������������
    if (gethostname(host_name, sizeof(host_name)) < 0)
    {
        printf("Error when getting local host name.");
        return 0;
    }
    int hostAddr = hostGetByName(host_name);
    if (hostAddr == ERROR)
    {
        printf("Error when called hostGetByName.");
        return 0;
    }
    struct in_addr iaddr;
    char* inet;
    iaddr.s_addr = hostAddr;
    inet = inet_ntoa(iaddr);
    ipAddr->length = strlen(inet_ntoa(iaddr));
    memcpy(ipAddr->value,inet_ntoa(iaddr),ipAddr->length);
    listSize++;
#else
    printf("δʵ�ֵķ���.\n");
    return -1;
#endif
#endif
    //printf("Network Card not exist!\n");
    return listSize;
}

// ��ʮ��ʽIP���long��
long getLongFromIPAddr(const char* ipAddr)
{
#if defined _TM1 || defined _TM3
	return IpStrToUint(ipAddr);
#else
    return ntohl(inet_addr(ipAddr));
#endif
}

/*---------------------------------------------------------------------------------
-   �� �� ��: IpUintToStr
-   ��������: ����IPת�����ַ���
-   ��    ��: ��
-   ��    ��: ����
-   ȫ�ֱ���:
-   ע    ��: ����IPת�����ַ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\04\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID IpUintToStr(ULONG ulAddr, CHAR* cpAddr)
{
	//ע��ȫ��С�˴��������ֽ���Ϊ��ˣ�windows��ΪС�ˡ�LJM
	memset(cpAddr, 0, LOCATOR_ADDR_IP4_STR_BEGIN);
    cpAddr[LOCATOR_ADDR_IP4_STR_BEGIN + 3] = ulAddr & 0xff;
    cpAddr[LOCATOR_ADDR_IP4_STR_BEGIN + 2] = (ulAddr >> 8) & 0xff;
    cpAddr[LOCATOR_ADDR_IP4_STR_BEGIN + 1] = (ulAddr >> 16) & 0xff;
    cpAddr[LOCATOR_ADDR_IP4_STR_BEGIN + 0] = (ulAddr >> 24) & 0xff;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: IpStrToUint
-   ��������: �ַ���IPת��������
-   ��    ��: IP��ַ
-   ��    ��: ����������
-   ȫ�ֱ���:
-   ע    ��: �ַ���IPת��������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\04\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
ULONG IpStrToUint(CHAR* address)
{
    ULONG ulAddr = 0;
    ulAddr |= (address[LOCATOR_ADDR_IP4_STR_BEGIN + 3]) & 0xff;
    ulAddr |= (address[LOCATOR_ADDR_IP4_STR_BEGIN + 2] << 8) & 0xff00;
    ulAddr |= (address[LOCATOR_ADDR_IP4_STR_BEGIN + 1] << 16) & 0xff0000;
    ulAddr |= (address[LOCATOR_ADDR_IP4_STR_BEGIN + 0] << 24) & 0xff000000;
#if defined _TM1 || defined _TM3
    return ulAddr;
#else
    return htonl(ulAddr);
#endif
}

/*---------------------------------------------------------------------------------
-   �� �� ��: InitSendSocketInfo
-   ��������: ��ʼ�����Ͷ�socket
-   ��    ��: socket��Ϣ
-   ��    ��: ����
-   ȫ�ֱ���:
-   ע    ��: ��ʼ�����Ͷ�socket
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\04\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL InitSendSocketInfo(SocketInfo* socketInfo, const char *localAddr, UINT32 Multicastport, int isMulticast)
{
/*TODO����Ӳ���vxWorks֧�֣�ͬʱ������ģʽȥ��rapidio�İ󶨡�����Initsocket�����޸�*/
//#ifdef _WIN32
//	u_long mode = 0; /* 0���� 1������ */
//#elif defined (__linux)
//#ifndef _VXWORKS
//    u_long mode = O_NONBLOCK;
//#else
//    u_long mode = TRUE;
//#endif
//#endif
    
#if defined _TM1  || defined _TM3

#else
#ifdef _WIN32
    u_long mode = 0; /* 0���� 1������ */
#elif defined (__linux)
    u_long mode = O_NONBLOCK;
#else
    u_long mode = TRUE;
#endif
    int sendbufsize = 1024 * 1024;//UDP_BUF_SIZE;
#if defined (_WIN32)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        return FALSE;
    }
#endif
   // int temp, err;
    socketInfo->socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (INVALID_SOCKET == socketInfo->socket)
    {
        return FALSE;
    }

    //���÷��Ͷ�socket����
    if (SOCKET_ERROR == setsockopt(socketInfo->socket, SOL_SOCKET, SO_SNDBUF, (char *)&sendbufsize, sizeof(sendbufsize)))
    {
        closesocket(socketInfo->socket);
        return FALSE;
    }

    /* ����ģʽ���� */
#if defined(_WIN32) && !defined(QYDDS_NON_BLOCK)
    if (SOCKET_ERROR == ioctlsocket(socketInfo->socket, FIONBIO, (u_long*)&mode))
    {
        closesocket(socketInfo->socket);
        return FALSE;
    }
#elif defined(_VXWORKS)&& defined(QYDDS_NON_BLOCK)
    if (ERROR == ioctl(socketInfo->socket, FIONBIO, (u_long*)&mode))
    {
        closesocket(socketInfo->socket);
        return FALSE;
    }
#endif
#ifdef __linux__
	struct in_addr inaddr;
	inaddr.s_addr = inet_addr(localAddr);
	int err = setsockopt(socketInfo->socket, IPPROTO_IP, IP_MULTICAST_IF, (char *)&inaddr, sizeof(inaddr));
	if (err != 0)
	{
		printf("setsocketopt failed\n");
		return 0;
	}
	if(isMulticast)
	{
		/*TODO��vxworks�´˴���Ӧ���������滹��һ����linux��δ����,��֪�Ƿ���뱣��*/
		int temp = 8;//Ӧ����spdp�鲥���Ͷ˲���Ҫ��LJM�����ó�1����8
		setsockopt(socketInfo->socket, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&temp, sizeof(temp));
		if (err == SOCKET_ERROR)
		{
			printf("setsockopt failed\n");
			return 0;
		}
	}

#endif
    InitSockAddr(&socketInfo->sockAddr, inet_addr(localAddr), 0);//���Ͷ˲���Ҫָ���˿ں�
	if (SOCKET_ERROR != bind(socketInfo->socket, (struct sockaddr *)&socketInfo->sockAddr, sizeof(socketInfo->sockAddr)))
	{
		//printf("udp send bind %s success\n", localAddr);
	}
	else
	{
		printf("udp send bind %s error\n", localAddr); 
		return FALSE;
	}
#endif
    return TRUE;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: InitUnicastReceiveSocketInfo
-   ��������: ��ʼ�����ն˵���socket
-   ��    ��: socket��Ϣ
-   ��    ��: ����
-   ȫ�ֱ���:
-   ע    ��: ��ʼ�����ն˵���socket
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\04\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL InitUnicastReceiveSocketInfo(SocketInfo* socketInfo, const char *localAddr, unsigned int port,  Locator_t* locator)
{
#if defined _TM1 || defined _TM3

#else

#ifdef _WIN32
    u_long mode = 0; /* 0���� 1������ */
#elif defined (__linux)
    u_long mode = O_NONBLOCK;
#else
    u_long mode = TRUE;
#endif

    u_int bOptval = 1;
    int recvbufsize = 4 * 1024 * 1024;//UDP_BUF_SIZE;

    socketInfo->socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (INVALID_SOCKET == socketInfo->socket)
    {
        return FALSE;
    }

    //���ý��ն�socket����
    if (SOCKET_ERROR == setsockopt(socketInfo->socket, SOL_SOCKET, SO_RCVBUF, (char *)&recvbufsize, sizeof(recvbufsize)))
    {
        closesocket(socketInfo->socket);
        return FALSE;
    }

    /* ����ģʽ���� */
#if defined(_WIN32) && !defined(QYDDS_NON_BLOCK)
    if (SOCKET_ERROR == ioctlsocket(socketInfo->socket, FIONBIO, (u_long*)&mode))
    {
        closesocket(socketInfo->socket);
        return FALSE;
    }
#elif defined(_VXWORKS)&& defined(QYDDS_NON_BLOCK)
    if (ERROR == ioctl(socketInfo->socket, FIONBIO, (u_long*)&mode))
    {
        closesocket(socketInfo->socket);
        return FALSE;
    }
#endif
// #elif defined(__linux__)
//     int flags = fcntl(socketInfo->socket, F_GETFL, 0);
//     ioctlsocket(socketInfo->socket, F_SETFL, flags | O_NONBLOCK);
//#endif
    socketInfo->sockAddr.sin_family = AF_INET;
    socketInfo->sockAddr.sin_addr.s_addr = inet_addr(localAddr);

    while (TRUE)
    {
        socketInfo->sockAddr.sin_port = htons(port);
        if (SOCKET_ERROR != bind(socketInfo->socket, (struct sockaddr *)&socketInfo->sockAddr, sizeof(socketInfo->sockAddr)))
        {
			printf("udp���ն˰� ����%s : %u ��\n", localAddr, port);
            IpUintToStr(ntohl(inet_addr(localAddr)), locator->address);
            locator->kind = LOCATOR_KIND_UDPv4;
            locator->port = port;
            break;
        }
        port++;
    }
#endif
    return TRUE;
}


/*---------------------------------------------------------------------------------
-   �� �� ��: InitMulticastReceiveSocketInfo
-   ��������: ��ʼ�����ն��鲥socket
-   ��    ��: socket��Ϣ
-   ��    ��: ����
-   ȫ�ֱ���:
-   ע    ��: ��ʼ�����ն��鲥socket
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\04\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL InitMulticastReceiveSocketInfo(SocketInfo* socketInfo, const char *localAddr, unsigned int port, const char*multicastAddr, Locator_t* locator)
{
#if defined _TM1 || defined _TM3

#else
#ifdef _WIN32
    u_long mode = 0; /* 0���� 1������ */
#elif defined (__linux)
    u_long mode = O_NDELAY;
#else
    u_long mode = TRUE;
#endif

	int temp;
    int bOptval = 1;
    int recvbufsize = 1024 * 1024;//UDP_BUF_SIZE;
	struct ip_mreq multiCast;
    socketInfo->socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (INVALID_SOCKET == socketInfo->socket)
    {
        return FALSE;
    }
#if defined(_WIN32)  && !defined(QYDDS_NON_BLOCK)
    if (SOCKET_ERROR == ioctlsocket(socketInfo->socket, FIONBIO, (u_long*)&mode))
    {
        closesocket(socketInfo->socket);
        return FALSE;
    }
#elif defined(_VXWORKS) && defined(QYDDS_NON_BLOCK)
    if (ERROR == ioctl(socketInfo->socket, FIONBIO, (u_long*)&mode))
    {
        closesocket(socketInfo->socket);
        return FALSE;
    }
#endif
// #elif defined(__linux__)
//     int flags = fcntl(socketInfo->socket, F_GETFL, 0);
//     ioctlsocket(socketInfo->socket, F_SETFL, flags | O_NONBLOCK);
//#endif

    //���ý��ն�socket����
    temp = 4 << 20;
    if (SOCKET_ERROR == setsockopt(socketInfo->socket, SOL_SOCKET, SO_RCVBUF, (char *)&temp, sizeof(temp)))
    {
        closesocket(socketInfo->socket);
        return FALSE;
    }

    if (SOCKET_ERROR == setsockopt(socketInfo->socket, SOL_SOCKET, SO_REUSEADDR, (char *)&bOptval, sizeof(bOptval)))
    {
        closesocket(socketInfo->socket);
        return FALSE;
    }
#ifndef _VXWORKS
    temp = 8;
    if (SOCKET_ERROR == setsockopt(socketInfo->socket, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&temp, sizeof(temp)))
    {
        closesocket(socketInfo->socket);
        return FALSE;
    }
#endif
    memset((socketInfo->sockAddr).sin_zero, 0, sizeof((socketInfo->sockAddr).sin_zero));
    INIT_SOCKADDR_LEN(socketInfo->sockAddr);
    socketInfo->sockAddr.sin_family = AF_INET;
    //socketInfo->sockAddr.sin_addr.s_addr = inet_addr(localAddr);
    socketInfo->sockAddr.sin_addr.s_addr= htonl(INADDR_ANY); //�鲥���ն�û�а�������LJM
    socketInfo->sockAddr.sin_port = htons(port);
    //InitSockAddr(&socketInfo->sockAddr, inet_addr(localAddr), port);
	/* Nephalem ʵ����������Ϊsockaddr_in����linux��ʹ��sockaddr�Ĵ�С�ᵽʱ�ղ������� */
    if (SOCKET_ERROR == bind(socketInfo->socket, (struct sockaddr *)&socketInfo->sockAddr, sizeof(struct sockaddr_in)))
    {
        closesocket(socketInfo->socket);
        return FALSE;
    }

    IpUintToStr(ntohl(inet_addr(multicastAddr)), locator->address);
    locator->kind = LOCATOR_KIND_UDPv4;
    locator->port = port;

    
    multiCast.imr_multiaddr.s_addr = inet_addr(multicastAddr);
    multiCast.imr_interface.s_addr = inet_addr(localAddr);
    //multiCast.imr_interface.s_addr = INADDR_ANY;

    if (SOCKET_ERROR == setsockopt(socketInfo->socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&multiCast, sizeof(struct ip_mreq)))
    {
//        int err = WSAGetLastError();
//        printf("%s\n", strerror(err));
        closesocket(socketInfo->socket);
        return FALSE;
    }
#endif
    return TRUE;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: RapIOSendMulticastMsg
-   ��������: RapIO�����鲥��Ϣ
-   ��    ��: �ڴ�顢socket��Ϣ�����͵�ַ
-   ��    ��: ����
-   ȫ�ֱ���:
-   ע    ��: RapIO�����鲥��Ϣ
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\04\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/

BOOL SendMulticastMsg(MemoryBlock * memBlock, SocketInfo * socketInfo, Locator_t * locator)
{
#if defined _FC
	INT32 i = 0;
	UINT32 ret = 0;
	
//	PrintBinData(memBlock->base,memBlock->writeIndex, "SendMulticastMsg");
//	printf("SendMulticastMsg and len = %d \n",memBlock->writeIndex);
	for(; i < g_commFCConfig.m_nodeNum;i++)
	{	
		ret = FC_NP_SendMsg(g_commFCConfig.m_FCdev, g_commFCConfig.m_configs[i].m_localDDSSendMsgID, memBlock->base,memBlock->writeIndex);
		//PrintBinData(memBlock->base,memBlock->writeIndex, "SendMulticastMsg");
		if(iSPDPFCSEND == 1)
			printf("M:FC_NP_SendMsg len = %d to %X  ret=0x%08x \n", memBlock->writeIndex,g_commFCConfig.m_configs[i].m_localDDSSendMsgID, ret);
		
//		if(ret != 0)
//			printf("M:FC_NP_SendMsg len = %d to %X  ret=0x%08x \n", memBlock->writeIndex,g_commFCConfig.m_configs[i].m_localDDSSendMsgID, ret);
#if defined _DEBUG_LOG
		if(ret)
			printf("M:FC_NP_SendMsg len = %d to %X failed ret=0x%08x \n",memBlock->writeIndex, g_commFCConfig.m_configs[i].m_localDDSSendMsgID, ret);
#endif

	}
#elif defined _653_PORT
	
	RETURN_CODE_TYPE retCode = NO_ACTION;
	int i = 0;
	for(;i<g_PortConfig.SendPortNum;i++)
	{
		SEND_QUEUING_MESSAGE(g_PortConfig.SendPort[i].portID, (UINT8*)memBlock->base, memBlock->writeIndex, (SYSTEM_TIME_TYPE)0, &retCode);
		if (retCode != 0)
			printf("M:SEND_QUEUING_MESSAGE len = %d to %d failed ret=0x%08x \n",memBlock->writeIndex, g_PortConfig.SendPort[i].portSeq, retCode);
	}
	
#else
    Locator_t* pstLocarot = locator;
	int iLen;
    while (NULL != pstLocarot)
    {
        if (0 != pstLocarot->port)
        {
             socketInfo->sockAddr.sin_addr.s_addr = IpStrToUint(pstLocarot->address);
             socketInfo->sockAddr.sin_port = htons(pstLocarot->port);

             iLen = sendto(socketInfo->socket,
                memBlock->base,
                memBlock->writeIndex,
                0,
                (struct sockaddr *) & (socketInfo->sockAddr),
                sizeof(socketInfo->sockAddr));

            if (iLen == SOCKET_ERROR)
            {
//                int err = WSAGetLastError();
//                if (err == WSAEWOULDBLOCK || err == WSAEMSGSIZE)
//                {
//                    printf("UDP Send Delay :%d\n", err);
//                }
//                else
//                {
//                    printf("UDP Send Error :%d\n", err);
//                }
                printf("UDP Send Error \n");
                return FALSE;
            }
        }
        else
        {
#ifdef RAPID_IO_CHANNEL
            if (g_RapdIOValid)
            {
                long location = 1;

                /* ���û�ȡ�����豸id */
                if (!GetLongValue("rapidio", "location", &location))
                {
                    printf("location not exist!");
                    return FALSE;
                }

                for (int i = 0; i < NUM_OF_RIO_NODE; i++)
                {
                    if (location != RIO_NODE_ARRAY[i] && 0 != RIO_NODE_ARRAY[i])
                    {
                        UnicastSend(RIO_NODE_ARRAY[i], memBlock->base, memBlock->writeIndex);
                    }

                }
            }
#endif
        }

        pstLocarot = pstLocarot->pNext;
    }
#endif
    return TRUE;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: RTPSReceiveMsg
-   ��������: ��Ϣ����
-   ��    ��: �ڴ�顢socket��Ϣ�����͵�ַ
-   ��    ��:��Ϣ����
-   ȫ�ֱ���:
-   ע    ��: RapIO�����鲥��Ϣ
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\04\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
int RTPSReceiveMsg(SocketInfo * locator, char * buffer)
{	
#ifdef RAPID_IO_CHANNEL
    if (g_RapdIOValid)
    {
        int msgLen = GetRioData(buffer);
        if (0 < msgLen)
        {
            return msgLen;
        }
    }
#elif defined(_TM1) || defined(_TM3)

    return 0;
#else
	/* Nephalem ʵ����������Ϊsockaddr_in����linux��ʹ��sockaddr�Ĵ�С�ᵽʱ�ղ������� */
    int addrLen = sizeof(struct sockaddr_in);
	int iLen = recvfrom(locator->socket, buffer, MAX_UDP_MSG_SIZE, 0, (struct sockaddr *)&locator->sockAddr, &addrLen);
	//printf("ReceiveMsg From %x port = %d\n",locator->sockAddr.sin_addr,locator->sockAddr.sin_port);
	return iLen;
#endif
}

/*---------------------------------------------------------------------------------
-   �� �� ��: RTPSSendMsg
-   ��������: ��Ϣ����
-   ��    ��: �ڴ�顢socket��Ϣ�����͵�ַ
-   ��    ��:��Ϣ����
-   ȫ�ֱ���:
-   ע    ��: RapIO�����鲥��Ϣ
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\04\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL RTPSSendMsg(MemoryBlock * memBlock, SocketInfo * socketInfo, Locator_t * locator)
{
#if defined _FC
	int ret =  0;
	
#if defined _DDS_TEST
	
	if(1 == iTestOptions)
	{
		if((locator->port & 0x000000FF) == 0x0)
			return TRUE;
	}
		
	ret = FC_NP_SendMsg(g_commFCConfig.m_FCdev, locator->port,memBlock->base,memBlock->writeIndex);
//	if((locator->port & 0x000000FF) == 0x0)
//		printf("FC_NP_SendMsg len = %d to %X  ret=0x%08x \n", memBlock->writeIndex,locator->port, ret);
#else
	
	//printf("RTPSSendMsg and len = %d \n",memBlock->writeIndex);
	ret = FC_NP_SendMsg(g_commFCConfig.m_FCdev, locator->port,memBlock->base,memBlock->writeIndex);
	
	if(iUserFCSEND == 1)
		printf("FC_NP_SendMsg len = %d to %X  ret=0x%08x \n", memBlock->writeIndex,locator->port, ret);
	
#endif

#if defined _DEBUG_LOG 
	if (ret)
	{
		printf("FC_NP_SendMsg len = %d to %X  ret=0x%08x \n", memBlock->writeIndex,locator->port, ret);
	}
#endif

//	int i = 0 ;
////	PrintBinData(memBlock->base,memBlock->writeIndex, "RTPSSendMsg");
//	for(; i < g_commFCConfig.m_nodeNum;i++)
//	{
//		//printf("Nephalem locator->port = %u,g_commFCConfig.m_configs[i].m_FCPort = %u \n",locator->port,g_commFCConfig.m_configs[i].m_FCPort);
//		if(g_commFCConfig.m_configs[i].m_FCPort == locator->port)
//		{
//			ret = FC_NP_SendMsg(g_commFCConfig.m_FCdev, g_commFCConfig.m_configs[i].m_localSEPDSendMsgID,memBlock->base,memBlock->writeIndex);
//			//printf("FC_NP_SendMsg len = %d to %X  ret=0x%08x \n", memBlock->writeIndex,locator->port, ret);
//#if defined _DEBUG_LOG || 1
//			if (ret)
//			{
//				printf("FC_NP_SendMsg len = %d to %X  ret=0x%08x \n", memBlock->writeIndex,locator->port, ret);
//			}
//#endif
//		}
//		
//	}

#elif defined _653_PORT
	RETURN_CODE_TYPE retCode = NO_ACTION;
	int i = 0;
	for(;i<g_PortConfig.SendPortNum;i++)
	{
		if(g_PortConfig.SendPort[i].portSeq == locator->port)
		{
			SEND_QUEUING_MESSAGE(g_PortConfig.SendPort[i].portID, memBlock->base, memBlock->writeIndex, (SYSTEM_TIME_TYPE)0, &retCode);
			if (retCode != 0)
				printf("SEND_QUEUING_MESSAGE len = %d to %d failed ret=0x%08x \n",memBlock->writeIndex, g_PortConfig.SendPort[i].portSeq, retCode);
			break;
		}
	}
#else
    /*Nephalem ģ�ⶪ��*/
	int iLen ;
	Locator_t* pstLocarot = NULL;
	//static i = 0;
    //i++;
    //if (i % 5 == 0)
    //    return TRUE;

    pstLocarot = locator;
	
	DDS_MUTEX_LOCK(g_sendMutex);

    while (NULL != pstLocarot)
    {
        if (0 != pstLocarot->port)
        {
            socketInfo->sockAddr.sin_addr.s_addr = IpStrToUint(pstLocarot->address);
            socketInfo->sockAddr.sin_port = htons(pstLocarot->port);

            iLen = sendto(socketInfo->socket,
                memBlock->base,
                memBlock->writeIndex,
                0,
                (struct sockaddr *) & (socketInfo->sockAddr),
                sizeof(socketInfo->sockAddr));

            if (iLen == SOCKET_ERROR)
            {
//                int err = WSAGetLastError();
//                if (err == WSAEWOULDBLOCK || err == WSAEMSGSIZE)
//                {
//                    printf("UDP�����ӳ� :%d\n", err);
//                }
//                else
//                {
//                    printf("UDP���ʹ��� :%d\n", err);
//                }
				UINT32 iAddr;
				char ipaddr[16];
				memcpy(&iAddr, pstLocarot->address + 12, 4);
				sprintf(ipaddr, "%u.%u.%u.%u", (iAddr >> 24 & 0xff),
					(iAddr >> 16 & 0xff),
					(iAddr >> 8 & 0xff),
					(iAddr & 0xff));
                printf("UDP Send To��%s:%u��Error :%s\n", ipaddr, pstLocarot->port, strerror(WSAGetLastError()));
				DDS_MUTEX_UNLOCK(g_sendMutex);
                return FALSE;
            }
			DDS_MUTEX_UNLOCK(g_sendMutex);
            return TRUE;

        }
        else
        {
#ifdef RAPID_IO_CHANNEL
            if (g_RapdIOValid)
            {
                long location = 1;

                /* ���û�ȡ�����豸id */
                if (!GetLongValue("rapidio", "location", &location))
                {
                    printf("location not exist!");
                    return FALSE;
                }

                if (location != atol(pstLocarot->address))
                {
                    UnicastSend(atol(pstLocarot->address), memBlock->base, memBlock->writeIndex);
                }
				//TODO���޸�BUG
                DDS_MUTEX_UNLOCK(g_sendMutex);

                return TRUE;
            }
#endif
        }

        pstLocarot = pstLocarot->pNext;
    }
	DDS_MUTEX_UNLOCK(g_sendMutex);
#endif
    return TRUE;
}
