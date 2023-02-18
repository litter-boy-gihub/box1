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
 /* 内存泄漏检查 */
 //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //_CrtMemCheckpoint(&g_startMemState);
 //CHECK_MEMORY();
/* 测试选项 
 * 1 发送时延测试
 * 2 
 */
int iTestOptions = 0;

void HiperDGetHostName(BoundedString256 * name)
{
    //获取本地主机名称
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

// 获取本机的IP地址列表
int getHostIPv4AddrList(BoundedString256 * ipAddr)
{
    int listSize = 0;
	int i = 0;
	char host_name[255];
	char info[1024 * 8];
#ifdef STATIC_DISCOVERY //静态配置
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
    // 初始化winsock
    WSADATA dat;
	struct hostent *phe;
    PIP_ADAPTER_INFO pIpAdapterInfo = (PIP_ADAPTER_INFO)info;
    unsigned long stSize = 1024 * 8;
    int nRel;
    IP_ADDR_STRING* pIpAddrString;
    int IPnumPerNetCard = 0;

    if (0 != WSAStartup(MAKEWORD(2, 2), &dat))
    {
        printf("初始化winsocket失败！");
    }
    
    ////获取本地主机名称
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

    /* 循环得出本地机器所有IP地址 */
    /* 只使用第一个网卡 */
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

	//IP_ADAPTER_INFO结构体存储本机网卡信息
	//得到结构体大小,用于GetAdaptersInfo参数
	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);

	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		printf("Network Card not exist!\n");
		return listSize;
	}

	//输出网卡信息
	//可能有多网卡,因此通过循环去判断
	while (pIpAdapterInfo)
	{
		//printf("AdapterName : %s \n", pIpAdapterInfo->AdapterName);
		//printf("Description : %s \n", pIpAdapterInfo->Description);

		// 屏蔽虚拟网卡，for VMWare MAC:00-50-56-xx-xx
		if (pIpAdapterInfo->Address[0] == 0x00
			&& pIpAdapterInfo->Address[1] == 0x50
			&& pIpAdapterInfo->Address[2] == 0x56)
		{
			pIpAdapterInfo = pIpAdapterInfo->Next;
			continue;
		}
		// 屏蔽虚拟网卡，for VirtualBox MAC:08-00-27-xx-xx
		if (pIpAdapterInfo->Address[0] == 0x08
			&& pIpAdapterInfo->Address[1] == 0x00
			&& pIpAdapterInfo->Address[2] == 0x27)
		{
			pIpAdapterInfo = pIpAdapterInfo->Next;
			continue;
		}

		//如果配置了网卡名称，则必须找到对应网卡
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

			/* Nephalem 过滤回环地址 */
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
	/* 没有查询到有效ip地址，默认为回环地址，防止没网卡时崩溃 */
	strcpy(ipAddr->value, "127.0.0.1"); // 默认值
	listSize++;
#elif defined(__linux__)
    struct ifaddrs* ifAddrStruct = NULL;
    void* tmpAddrPtr = NULL;
    // 获取网卡链表
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
			//CAN接口也是用socket, 但是CAN没有IP地址, ifa_addr为空；这里要进行判断, 否则会出现操作野指针的情况，导致Segmentation fault。
			if (ifaddrIter->ifa_addr->sa_family == AF_INET)
			{
				// 是ipv4
				if (strcmp(ifaddrIter->ifa_name, "lo") == 0)
				{
					// 跳过本地环回
					ifaddrIter = ifaddrIter->ifa_next;
					continue;
				}
				tmpAddrPtr = &((struct sockaddr_in *)ifaddrIter->ifa_addr)->sin_addr;
				inet_ntop(AF_INET, tmpAddrPtr, tmpIP, 16);
				//TODO:add for vxworks,去除lo
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
    //获取本地主机名称
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
    printf("未实现的方法.\n");
    return -1;
#endif
#endif
    //printf("Network Card not exist!\n");
    return listSize;
}

// 点十形式IP变成long型
long getLongFromIPAddr(const char* ipAddr)
{
#if defined _TM1 || defined _TM3
	return IpStrToUint(ipAddr);
#else
    return ntohl(inet_addr(ipAddr));
#endif
}

/*---------------------------------------------------------------------------------
-   函 数 名: IpUintToStr
-   功能描述: 整形IP转换成字符串
-   输    入: 无
-   返    回: 布尔
-   全局变量:
-   注    释: 整形IP转换成字符串
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2020\04\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID IpUintToStr(ULONG ulAddr, CHAR* cpAddr)
{
	//注：全当小端处理，网络字节序为大端，windows下为小端。LJM
	memset(cpAddr, 0, LOCATOR_ADDR_IP4_STR_BEGIN);
    cpAddr[LOCATOR_ADDR_IP4_STR_BEGIN + 3] = ulAddr & 0xff;
    cpAddr[LOCATOR_ADDR_IP4_STR_BEGIN + 2] = (ulAddr >> 8) & 0xff;
    cpAddr[LOCATOR_ADDR_IP4_STR_BEGIN + 1] = (ulAddr >> 16) & 0xff;
    cpAddr[LOCATOR_ADDR_IP4_STR_BEGIN + 0] = (ulAddr >> 24) & 0xff;
}

/*---------------------------------------------------------------------------------
-   函 数 名: IpStrToUint
-   功能描述: 字符串IP转换成整形
-   输    入: IP地址
-   返    回: 网络主机序
-   全局变量:
-   注    释: 字符串IP转换成整形
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
-   函 数 名: InitSendSocketInfo
-   功能描述: 初始化发送端socket
-   输    入: socket信息
-   返    回: 布尔
-   全局变量:
-   注    释: 初始化发送端socket
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2020\04\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL InitSendSocketInfo(SocketInfo* socketInfo, const char *localAddr, UINT32 Multicastport, int isMulticast)
{
/*TODO：添加部分vxWorks支持，同时将阻塞模式去除rapidio的绑定。几个Initsocket都有修改*/
//#ifdef _WIN32
//	u_long mode = 0; /* 0阻塞 1非阻塞 */
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
    u_long mode = 0; /* 0阻塞 1非阻塞 */
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

    //设置发送端socket参数
    if (SOCKET_ERROR == setsockopt(socketInfo->socket, SOL_SOCKET, SO_SNDBUF, (char *)&sendbufsize, sizeof(sendbufsize)))
    {
        closesocket(socketInfo->socket);
        return FALSE;
    }

    /* 阻塞模式设置 */
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
		/*TODO：vxworks下此处不应开启，下面还有一条。linux下未测试,不知是否必须保留*/
		int temp = 8;//应该是spdp组播发送端才需要，LJM，设置成1还是8
		setsockopt(socketInfo->socket, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&temp, sizeof(temp));
		if (err == SOCKET_ERROR)
		{
			printf("setsockopt failed\n");
			return 0;
		}
	}

#endif
    InitSockAddr(&socketInfo->sockAddr, inet_addr(localAddr), 0);//发送端不需要指定端口号
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
-   函 数 名: InitUnicastReceiveSocketInfo
-   功能描述: 初始化接收端单播socket
-   输    入: socket信息
-   返    回: 布尔
-   全局变量:
-   注    释: 初始化接收端单播socket
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2020\04\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL InitUnicastReceiveSocketInfo(SocketInfo* socketInfo, const char *localAddr, unsigned int port,  Locator_t* locator)
{
#if defined _TM1 || defined _TM3

#else

#ifdef _WIN32
    u_long mode = 0; /* 0阻塞 1非阻塞 */
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

    //设置接收端socket参数
    if (SOCKET_ERROR == setsockopt(socketInfo->socket, SOL_SOCKET, SO_RCVBUF, (char *)&recvbufsize, sizeof(recvbufsize)))
    {
        closesocket(socketInfo->socket);
        return FALSE;
    }

    /* 阻塞模式设置 */
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
			printf("udp接收端绑定 ：【%s : %u 】\n", localAddr, port);
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
-   函 数 名: InitMulticastReceiveSocketInfo
-   功能描述: 初始化接收端组播socket
-   输    入: socket信息
-   返    回: 布尔
-   全局变量:
-   注    释: 初始化接收端组播socket
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
==================================================================================
-   2020\04\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL InitMulticastReceiveSocketInfo(SocketInfo* socketInfo, const char *localAddr, unsigned int port, const char*multicastAddr, Locator_t* locator)
{
#if defined _TM1 || defined _TM3

#else
#ifdef _WIN32
    u_long mode = 0; /* 0阻塞 1非阻塞 */
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

    //设置接收端socket参数
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
    socketInfo->sockAddr.sin_addr.s_addr= htonl(INADDR_ANY); //组播接收端没有绑定网卡？LJM
    socketInfo->sockAddr.sin_port = htons(port);
    //InitSockAddr(&socketInfo->sockAddr, inet_addr(localAddr), port);
	/* Nephalem 实际数据类型为sockaddr_in，在linux下使用sockaddr的大小会到时收不到数据 */
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
-   函 数 名: RapIOSendMulticastMsg
-   功能描述: RapIO发送组播消息
-   输    入: 内存块、socket信息、发送地址
-   返    回: 布尔
-   全局变量:
-   注    释: RapIO发送组播消息
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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

                /* 配置获取本机设备id */
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
-   函 数 名: RTPSReceiveMsg
-   功能描述: 消息接收
-   输    入: 内存块、socket信息、发送地址
-   返    回:消息长度
-   全局变量:
-   注    释: RapIO发送组播消息
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
	/* Nephalem 实际数据类型为sockaddr_in，在linux下使用sockaddr的大小会到时收不到数据 */
    int addrLen = sizeof(struct sockaddr_in);
	int iLen = recvfrom(locator->socket, buffer, MAX_UDP_MSG_SIZE, 0, (struct sockaddr *)&locator->sockAddr, &addrLen);
	//printf("ReceiveMsg From %x port = %d\n",locator->sockAddr.sin_addr,locator->sockAddr.sin_port);
	return iLen;
#endif
}

/*---------------------------------------------------------------------------------
-   函 数 名: RTPSSendMsg
-   功能描述: 消息发送
-   输    入: 内存块、socket信息、发送地址
-   返    回:消息长度
-   全局变量:
-   注    释: RapIO发送组播消息
-   修改记录:
-   修改日期        版本        修改人      修改原因及内容
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
    /*Nephalem 模拟丢包*/
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
//                    printf("UDP发送延迟 :%d\n", err);
//                }
//                else
//                {
//                    printf("UDP发送错误 :%d\n", err);
//                }
				UINT32 iAddr;
				char ipaddr[16];
				memcpy(&iAddr, pstLocarot->address + 12, 4);
				sprintf(ipaddr, "%u.%u.%u.%u", (iAddr >> 24 & 0xff),
					(iAddr >> 16 & 0xff),
					(iAddr >> 8 & 0xff),
					(iAddr & 0xff));
                printf("UDP Send To【%s:%u】Error :%s\n", ipaddr, pstLocarot->port, strerror(WSAGetLastError()));
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

                /* 配置获取本机设备id */
                if (!GetLongValue("rapidio", "location", &location))
                {
                    printf("location not exist!");
                    return FALSE;
                }

                if (location != atol(pstLocarot->address))
                {
                    UnicastSend(atol(pstLocarot->address), memBlock->base, memBlock->writeIndex);
                }
				//TODO：修复BUG
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
