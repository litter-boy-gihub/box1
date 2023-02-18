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
*  @file     GlobalDefine.h
*  @brief    GlobalDefine
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
  该文件定义了整个HiperD的代码架构，主要包括如下内容：
  1）对整个工程有效的编译选项
  2）罗列出所有的头文件并给出头文件在架构中的功能
********************************************************************/
#pragma warning(disable:4996)
#ifndef _GLOBAL_DEFINE_H_
#define _GLOBAL_DEFINE_H_

//#define USE_STATIC_MALLOC     //静态内存
//#define USE_STATIC_MALLOC_T   //用户静态内存

//#define RAPID_IO_CHANNEL
//#define DDS_MONITOR
//TODO：非阻塞模式单独拿出来，不强制rapidio支持
//#define QYDDS_NON_BLOCK
#define _DDS_DLLEXPORT

#define _CRT_SECURE_NO_WARNINGS

//软无平台特殊版本，可针对相同topicName多次创建topic，每次删除会检查topicNum，直到为0才真正删除
//#define EVO_DDS

/*QoS相关宏*/
/*目前QoS收发配置关系：
RELIABILITY：当收设置为BEST_EFFORT则为不可靠，都设置为RELIABLE则为可靠，若收为RELIABLE而发为BEST_EFFORT则不匹配无法接收
DURABILITY：只需要发端设置TRANSIENT即视为持久化，需要收发同时配置RELIABLE才能生效
LIVELINESS：写在Participant中，目前只支持Participant的存活性
TIME_BASED_FILTER：只在收端配置
DEADLINE：只有发端配置有用
HISTORY：只有发端配置有用*/
//#define QYDDS_DURABILITY_QOS
// #define QYDDS_TIME_BASED_FILTER_QOS
#define QYDDS_LIVELINESS_QOS
//#define QYDDS_DEADLINE_QOS
#define QYDDS_HISTORY_QOS

/*大包重传方式宏，打开则为单包重传，关闭为整包重传*/
//#define QYDDS_MASSDATA

/********************************************************************
    在此处设置平台和操作系统选项
********************************************************************/
//#define _WRS_KERNEL
/********************************************************************
 在此处给出需要的所有系统头文件
********************************************************************/
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include <io.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <direct.h>
#include <process.h>
#include <time.h>
#include <shlwapi.h>
#include <assert.h>
#include <Iphlpapi.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#include <fcntl.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib")
#elif defined (__linux__) 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include <time.h>
#include <assert.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netdb.h>
#ifndef __USE_GNU
#define __USE_GNU

#include <spawn.h>
#include <ifaddrs.h>
#include <dlfcn.h>
#include <sys/io.h>


#endif

#endif

#ifdef _VXWORKS
//#include <ioLib.h>
#include <taskLib.h>
#include <net/ifaddrs.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdio.h>
#include <netinet/in.h>
#include <time.h>

#endif

/********************************************************************
    X86平台WIN32环境，对应常规的windows
********************************************************************/
#if defined(_WIN32) || defined(_WIN64)
    #if defined(_DDS_DLLEXPORT)
        #define DDS_DLL __declspec(dllexport)
    #elif defined(_HIPERD_DLLIMPORT)
        #define HIPERD_DLL __declspec(dllimport)
    #else
        #define HIPERD_DLL
    #endif
#elif defined (__linux__)
#define DDS_DLL __attribute__((visibility("default")))
#define DDS_DLL __attribute__((visibility("default")))
#endif

/********************************************************************
    其它系统平台的环境配置
********************************************************************/


#if defined (__linux__)
typedef void *HANDLE;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;
typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef unsigned long       DWORD;
typedef long long LONGLONG;
#define SOCKET int
#define socklen_t unsigned int
#define ssize_t int
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define closesocket close
#define ioctlsocket fcntl
#define WSAGetLastError() errno
#define WSAEWOULDBLOCK EWOULDBLOCK
#define WSAEMSGSIZE EWOULDBLOCK
#endif
#define INIT_SOCKADDR_LEN(addr)

//定义系统内核工具，例如互斥锁、信号量等
#if defined(_WIN32)
    #define STACALL __stdcall
    //互斥锁资源
    #define DDS_MUTEX HANDLE
    #define DDS_SEMA HANDLE
    #define DDS_MUTEX_LOCK(mutex) WaitForSingleObject(mutex, INFINITE)
    #define DDS_MUTEX_UNLOCK(mutex) ReleaseMutex(mutex)
    #define DDS_MUTEX_INIT(mutex) mutex = CreateMutex(0, FALSE, 0)
    #define DDS_MUTEX_DESTROY(mutex) \
            {\
            CloseHandle(mutex);\
            mutex = NULL;\
        }
    //信号量资源
    #define DDS_SEMA_WAIT_TIME(sema,delay) WaitForSingleObject(sema, delay)
    #define DDS_SEMA_WAIT(sema) WaitForSingleObject(sema, INFINITE)
    #define DDS_SEMA_POST(sema) ReleaseSemaphore(sema, 1, NULL)
    #define DDS_SEMA_TRYWAIT(sema) WaitForSingleObject(sema, 0)
    #define DDS_SEMA_DESTROY(sema) \
            {\
            CloseHandle(sema);\
            sema = NULL;\
        }
    #define DDS_SEMA_INIT(sema, initCount, maxCount) sema = CreateSemaphore(NULL, initCount, maxCount, NULL)
    //线程资源
    typedef  unsigned int DDS_THREAD_ID;
    typedef HANDLE    DDS_THREAD_HANDLE;
#elif defined (__linux__)
#define STACALL __attribute__((__cdecl))
/*******************锁******************/
typedef pthread_mutex_t		DDS_MUTEX;												/* 定义锁 */
/* 创建可重入锁 */
/* do-while内容包含宏定义 */
#ifndef _DO_WHILE_0
#define _DO_WHILE_0(content) do { content } while (0)
#endif /* _DO_WHILE_0 */
typedef pthread_mutex_t		DDS_MUTEX;												/* 定义锁 */
#define DDS_MUTEX_INIT(v)		_DO_WHILE_0({ pthread_mutexattr_t attr; pthread_mutexattr_init(&attr); pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); pthread_mutex_init(&(v), &attr); }) 	/* 初始化锁 */
#define DDS_MUTEX_LOCK(v)		pthread_mutex_lock(&(v))							/* 加锁 */
#define DDS_MUTEX_UNLOCK(v)		pthread_mutex_unlock(&(v))							/* 解锁 */
#define DDS_MUTEX_DESTROY(v)	pthread_mutex_destroy(&(v))							/* 销毁锁 */

/* POSIX平台下进程锁的定义 */
/* 名称最好以“/”开头，且只有一个“/”，如“/MyLock”，以兼容linux等系统 */
typedef sem_t *				MUTEXN;												/* 定义有名锁 */
#define DDS_MUTEX_OPEN(v, n)	(v) = sem_open(n, O_CREAT, 0644, 1); while (sem_trywait(v) == 0) ; sem_post(v)	/* 打开有名锁，此处要求名称长度小于YMAX_NAME_LENGTH字节 */
#define DDS_MUTEX_LOCKN(v)		sem_wait(v)											/* 加锁有名锁 */
#define DDS_MUTEX_UNLOCKN(v)	sem_post(v)											/* 解锁有名锁 */
#define DDS_MUTEX_CLOSE(v)		sem_close(v)										/* 关闭有名锁 */
/*******************信号量******************/
/* 定义信号量 */
typedef struct _SEMA
{
    sem_t		m_semS;
    sem_t *		m_semP;
} SEMA;																	/* 定义信号量 */
#define DDS_SEMA_INIT(v, i)		(v).m_semP = &(v).m_semS; sem_init((v).m_semP, 0, i)	/* 初始化信号量 */
#define DDS_SEMA_WAIT(v)		sem_wait((v).m_semP)						/* 等待信号量 */
/* 尝试等待信号量 */
#define DDS_SEMA_TRYWAIT(v, t) \
    _DO_WHILE_0({ \
		struct timespec ts; \
		clock_gettime(CLOCK_REALTIME, &ts); \
		ts.tv_sec += (t) / 1000; \
		ts.tv_nsec += (t) % 1000 * 1000000; \
		if (ts.tv_nsec >= 1000000000) { \
			++ts.tv_sec; \
			ts.tv_nsec -= 1000000000; \
                		} \
		sem_timedwait((v).m_semP, &ts); \
    })
#define DDS_SEMA_POST(v)		sem_post((v).m_semP)						/* 释放信号量 */
#define DDS_SEMA_DESTROY(v)		sem_destroy((v).m_semP)						/* 销毁信号量 */
/* 名称最好以“/”开头，且只有一个“/”，如“/MySema”，以兼容linux等系统 */
#define DDS_SEMA_OPEN(v, n)		(v).m_semP = sem_open(n, O_CREAT, 0644, 0)	/* 打开有名信号量，此处要求名称长度小于YMAX_NAME_LENGTH字节 */
#define DDS_SEMA_CLOSE(v)		sem_close((v).m_semP)						/* 关闭有名信号量 */
/*******************线程******************/
#define DDS_THREAD_ID pthread_t							/* 线程ID类型 */
typedef HANDLE    DDS_THREAD_HANDLE;
#endif


#ifdef _VXWORKS
#define STACALL

#ifndef _DO_WHILE_0
#define _DO_WHILE_0(content) do { content } while (0)
#endif /* _DO_WHILE_0 */

/* Nephalem 互斥锁资源 */
typedef SEM_ID		DDS_MUTEX;												/* 定义锁 */
#define DDS_MUTEX_INIT(v)		_DO_WHILE_0({ v = semMCreate(0); }) 	/* 初始化锁 */
#define DDS_MUTEX_LOCK(v)		semTake(v,WAIT_FOREVER);							/* 加锁 */
#define DDS_MUTEX_UNLOCK(v)		semGive(v);							/* 解锁 */
#define DDS_MUTEX_DESTROY(v)	semDelete(v);

#define DDS_THREAD_ID pthread_t							/* 线程ID类型 */
typedef int   HANDLE;
typedef HANDLE    DDS_THREAD_HANDLE;

#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define closesocket close
#endif 

#ifndef DDS_DLL
#ifdef _WIN32
#define DDS_DLL __declspec(dllexport)
#else
#define DDS_DLL
#endif
#endif
//定义系统相关的数据类型
/*TODO：去掉BOOL和INT的定义，以在vxWorks下成功编译*/
#ifndef VOID
typedef  void VOID;
#ifndef _VXWORKS
typedef  char BOOL;
#endif
#endif
#ifndef _VXWORKS
typedef  int INT;
#endif
typedef  int INT;
typedef  float FLOAT32;
typedef  double DOUBLE64;
typedef  unsigned long ULONG;
typedef  unsigned char UCHAR;
typedef  char CHAR;
typedef  char NETBYTE;
typedef  short SHORT;
typedef  unsigned short USHORT;
typedef  unsigned int UINT32;
typedef  long long INT64;
typedef  unsigned long long UINT64;
typedef  float FLOAT;
typedef  double DOUBLE;
typedef long LONG;


#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

/*Sleep函数在不同操作系统中的实现*/
static void QY_Sleep(UINT sleepTime)
{
#if defined(_WIN32)
    Sleep(sleepTime);
#elif defined(__linux__)
/*TODO：sleep接口，vxWorks的usleep可能无法使用*/
	   struct timespec tc;
       tc.tv_sec = sleepTime / 1000;
       tc.tv_nsec = (sleepTime % 1000) *  1,000,000;
       nanosleep(&tc,NULL);
#elif defined(_VXWORKS)
        
       taskDelay(sleepTime * sysClkRateGet() / 1000);
#endif
}

/********************************************************************
 以下是该项目相关的所有头文件及其功能描述
********************************************************************/
/*TODO：添加Cpp的编译支持*/
#ifdef __cplusplus
extern "C"{
#endif
//类型映射，屏蔽平台类型差异
#include "CommonTypeDefine.h"

/* ProcessUnity.h */
#include "ProcessUnity.h"

/* MemoryBlock结构体相关定义及操作 */
#include "MemoryBlock.h"

//OMG对IDL的CDR编解码相关机制
#include "CDRTool.h"

#include "ConfFile.h"

//RTPS规范中Duration_t对应的数据类型及相关处理机制
#include "Duration_t.h"

//RTPS规范中Guid_t对应的数据类型及相关处理机制
#include "GUID_t.h"

#include "ReturnCode.h"

//高精度时间相关的数据结构和处理机制
#include "TimeTool.h"

/* 分片数据号 */
#include "FragmentNumber.h"

//64比特的序列号
#include "SequenceNumber_t.h"

/* SequenceNumber集合结构体定义 */
#include "SequenceNumberSet.h"

#include "FragmentNumber.h"

#include "FragmentNumberSet.h"

/* 历史数据相数据类型定义及相关操作 */
#include "HistoryCache.h"

//定义了多个干固定最大长度的字符串工具结构
#include "BoundedString.h"

//RTPS规范中BuiltinTopicKey_t对应的数据类型及相关处理机制
#include "BuiltinTopicKey_t.h"

//DCPS规范中UserDataQosPolicy对应的数据类型及相关处理机制
#include "UserDataQosPolicy.h"

//存储字符串化名值对的列表结构
#include "PropertyList.h"

//表示RTPS协议中的参数以及参数列表结构体ParameterList及其相关操作机制
#include "ParameterList.h"

//RTPS规范中BuiltinEndpointSet_t对应的数据类型及相关处理机制
#include "BuiltinEndpointSet_t.h"

//规范中Locator_t对应的数据类型及相关处理机制
#include "Locator_t.h"

//RTPS规范中ProtocolVersion_t对应的数据类型及相关处理机制
#include "ProtocolVersion_t.h"

//RTPS规范中VendorId_t对应的数据类型及相关处理机制
#include "VendorId_t.h"

/* QosPolicy.h */
#include "QosPolicy.h"

/* QosService.h */
#include "QosService.h"

//#include "Configure.h"

//RTPS规范中所有消息的Header结构
#include "RTPSMessageHeader.h"

#include "RTPSDiscovery.h"

//RTPS子消息头，标明子消息类型标志及长度
#include "SubMessageHeader.h"

//RTPS的Discovery模块中用于SPDP的ParticipantData结构
#include "ParticipantData.h"

//远程的Participant进行SPDP和SEDP交互的必要信息，例如网络地址和端口
#include "ParticipantProxy.h"

#include "SerializedData.h"

/* 报文消息头数据结构及其相关操作处理机制 */
#include "Data.h"

/* 用于生成GUID */
#include "GuidGen.h"

/* 单例模式网络工具类，封装常用的网络操作 */
#include "NetWorkUnity.h"

/* 封装与字符串有关的操作 */
#include "StringUnity.h"

/* AckNack子消息结构体及其相关编解码操作 */
#include "AckNack.h"

#include "NackFrag.h"

/* Heartbeat子消息结构体及其相关编解码操作 */
#include "Heartbeat.h"

/* 不相交的SequenceNumber_t区间集合 */
//#include "DisjointSequence.h"

#include "MajorTask.h"

/* 存储某条链路的分片收到的信息 */
//#include "RTPSFragInfo.h"

/* DataFrag子报文 */
#include "DataFrag.h"

/* InfoTimestamp子报文 */
#include "InfoTimestamp.h"

/* InfoDestination 子报文 */
#include "InfoDestination.h"

/* InfoReply 子报文 */
#include "InfoReply.h"

/* gap子报文*/
#include "Gap.h"

/*日志模块*/
#include "CommLog.h"

/*链表操作*/
#include "ListOper.h"

/* 静态内存管理 */
#include "MemoryAlloc.h"

/* 远端写入器 */
#include "DiscoveredWriter.h"

/* 远端读取器 */
#include "DiscoveredReader.h"

/* 数据写入器 */
#include "DataWriter.h"

/* 数据读取器*/
#include "DataReader.h"

/* 主题 */
#include "Topic.h"

/* 发现参与者 */
#include "DiscoveredParticipant.h"

/* 数据写入器 */
#include "BuiltinDataWriter.h"

/* 数据读取器*/
#include "BuiltinDataReader.h"

/* 监控配置项 */
#include "Monitor.h"

/* 参与者 */
#include "DomainParticipant.h"
 
#ifdef RAPID_IO_CHANNEL
#include "RapidIO/tsi721api.h"
#include "RapidIO/tsi721ioctl.h"
#include "RapidIO/RioUtility.h"
#endif

/* 网络初始化 */
#include "NetWorkStructure.h"

/* 网络字节序解析 */
#include "NetWorkOrder.h"

/* SPDP报文处理模块 */
#include "Spdp.h"

/* SEDP处理模块 */
#include "Sedp.h"

/* 用户收发数据 */
#include "User.h"

/* 处理除了SPDP发现报文之外的所有报文 */
 #include "RTPSReceiveStrategy.h"
#ifdef __cplusplus
}
#endif

//#ifdef _WIN32
#include "FACE/TypeAbstraction_TS.h"
//#endif
#endif



