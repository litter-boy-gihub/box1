/*********************************************************************************
*  OpenST Basic tool library
*  Copyright (C) 2019 QiYuan
*
*
*  This program is security software
*  without permission you cannot use or distribute the software
*  otherwise, legal liability will be investigated
*
*  If you want to use the software, please contact the authorities to obtain a license
*  < http://www.iamqy.com.cn >.
*
*  @file     NetWorkUnity.h
*  @brief    NetWorkUnity
*  Details.
*
*  @author   Hunter
*  @email    1028251654@qq.com
*  @version  1.0
*  @date     2019/08/08
*  @license
*
*--------------------------------------------------------------------------------
*  Remark         : Description
*--------------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>  | <operation> | <Description>
*--------------------------------------------------------------------------------
*  2019/08/08 | 1.0       | Hunter    | Create file |
*--------------------------------------------------------------------------------
*
*********************************************************************************/

/********************************************************************
NetWorkUnity.h
����ģʽ���繤���࣬��װ���õ��������
********************************************************************/
#ifndef HIPERD_NETWORK_UNITY_H
#define HIPERD_NETWORK_UNITY_H

//#define _CRTDBG_MAP_ALLOC
//
//#if defined(_WIN32)
//
//#include <crtdbg.h>
//extern _CrtMemState g_startMemState;
//extern _CrtMemState g_endMemState;
//extern _CrtMemState g_diffMemState;
//#endif

extern CHAR g_ipAddr[32];
extern CHAR g_netCardName[256];

#define CHECK_MEMORY()\
    _CrtMemCheckpoint(&g_endMemState);\
    _CrtMemDifference(&g_diffMemState, &g_startMemState,&g_endMemState);\
    _CrtMemDumpStatistics(&g_diffMemState);\
    _CrtDumpMemoryLeaks()

#define InitSockAddr(addr, ip, port) \
	memset((addr)->sin_zero, 0, sizeof((addr)->sin_zero)); \
	INIT_SOCKADDR_LEN(addr); \
	(addr)->sin_family = AF_INET; \
	(addr)->sin_addr.s_addr = ip; \
	(addr)->sin_port = htons(port)

void HiperDGetHostName(BoundedString256 * name);

/* ��ȡ������IP��ַ�б� ����ip���� */
int getHostIPv4AddrList(BoundedString256 * ipList);

/* ��ȡ������Mac��ַ�б� */
int getHostMacAddrList(BoundedString256 * macList);

/* ��ʮ��ʽIP���long�� */
long getLongFromIPAddr(const char* ipAddr);

/********************************************************************/
/* UDP�����ʼ����غ��� */
/********************************************************************/
extern BOOL InitSendSocketInfo(SocketInfo* socketInfo, const char *localAddr, UINT32 Multicastport);

extern BOOL InitUnicastReceiveSocketInfo(SocketInfo* socketInfo, const char *localAddr, unsigned int port, Locator_t* locator);

extern BOOL InitMulticastReceiveSocketInfo(SocketInfo* socketInfo, const char *localAddr, unsigned int port, const char*multicastAddr, Locator_t* locator);

/********************************************************************/
/* �������ݷ��ͽ�����غ�����װ */
/********************************************************************/
/* �������� */
BOOL SendMulticastMsg(MemoryBlock * memBlock, SocketInfo * socketInfo, Locator_t * locator);

/* �������� */
int RTPSReceiveMsg(SocketInfo * locator, char * buffer);

/* �������ݵ�����Locator_t */
BOOL RTPSSendMsg(MemoryBlock * memBlock, SocketInfo * socketInfo, Locator_t * locator);

#endif

