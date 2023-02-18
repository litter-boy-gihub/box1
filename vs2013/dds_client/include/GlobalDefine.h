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
  ���ļ�����������HiperD�Ĵ���ܹ�����Ҫ�����������ݣ�
  1��������������Ч�ı���ѡ��
  2�����г����е�ͷ�ļ�������ͷ�ļ��ڼܹ��еĹ���
********************************************************************/
#pragma warning(disable:4996)
#ifndef _GLOBAL_DEFINE_H_
#define _GLOBAL_DEFINE_H_

//#define USE_STATIC_MALLOC     //��̬�ڴ�
//#define USE_STATIC_MALLOC_T   //�û���̬�ڴ�

//#define RAPID_IO_CHANNEL
//#define DDS_MONITOR
//TODO��������ģʽ�����ó�������ǿ��rapidio֧��
//#define QYDDS_NON_BLOCK
#define _DDS_DLLEXPORT

#define _CRT_SECURE_NO_WARNINGS

//����ƽ̨����汾���������ͬtopicName��δ���topic��ÿ��ɾ������topicNum��ֱ��Ϊ0������ɾ��
//#define EVO_DDS

/*QoS��غ�*/
/*ĿǰQoS�շ����ù�ϵ��
RELIABILITY����������ΪBEST_EFFORT��Ϊ���ɿ���������ΪRELIABLE��Ϊ�ɿ�������ΪRELIABLE����ΪBEST_EFFORT��ƥ���޷�����
DURABILITY��ֻ��Ҫ��������TRANSIENT����Ϊ�־û�����Ҫ�շ�ͬʱ����RELIABLE������Ч
LIVELINESS��д��Participant�У�Ŀǰֻ֧��Participant�Ĵ����
TIME_BASED_FILTER��ֻ���ն�����
DEADLINE��ֻ�з�����������
HISTORY��ֻ�з�����������*/
//#define QYDDS_DURABILITY_QOS
// #define QYDDS_TIME_BASED_FILTER_QOS
#define QYDDS_LIVELINESS_QOS
//#define QYDDS_DEADLINE_QOS
#define QYDDS_HISTORY_QOS

/*����ش���ʽ�꣬����Ϊ�����ش����ر�Ϊ�����ش�*/
//#define QYDDS_MASSDATA

/********************************************************************
    �ڴ˴�����ƽ̨�Ͳ���ϵͳѡ��
********************************************************************/
//#define _WRS_KERNEL
/********************************************************************
 �ڴ˴�������Ҫ������ϵͳͷ�ļ�
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
    X86ƽ̨WIN32��������Ӧ�����windows
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
    ����ϵͳƽ̨�Ļ�������
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

//����ϵͳ�ں˹��ߣ����绥�������ź�����
#if defined(_WIN32)
    #define STACALL __stdcall
    //��������Դ
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
    //�ź�����Դ
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
    //�߳���Դ
    typedef  unsigned int DDS_THREAD_ID;
    typedef HANDLE    DDS_THREAD_HANDLE;
#elif defined (__linux__)
#define STACALL __attribute__((__cdecl))
/*******************��******************/
typedef pthread_mutex_t		DDS_MUTEX;												/* ������ */
/* ������������ */
/* do-while���ݰ����궨�� */
#ifndef _DO_WHILE_0
#define _DO_WHILE_0(content) do { content } while (0)
#endif /* _DO_WHILE_0 */
typedef pthread_mutex_t		DDS_MUTEX;												/* ������ */
#define DDS_MUTEX_INIT(v)		_DO_WHILE_0({ pthread_mutexattr_t attr; pthread_mutexattr_init(&attr); pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); pthread_mutex_init(&(v), &attr); }) 	/* ��ʼ���� */
#define DDS_MUTEX_LOCK(v)		pthread_mutex_lock(&(v))							/* ���� */
#define DDS_MUTEX_UNLOCK(v)		pthread_mutex_unlock(&(v))							/* ���� */
#define DDS_MUTEX_DESTROY(v)	pthread_mutex_destroy(&(v))							/* ������ */

/* POSIXƽ̨�½������Ķ��� */
/* ��������ԡ�/����ͷ����ֻ��һ����/�����硰/MyLock�����Լ���linux��ϵͳ */
typedef sem_t *				MUTEXN;												/* ���������� */
#define DDS_MUTEX_OPEN(v, n)	(v) = sem_open(n, O_CREAT, 0644, 1); while (sem_trywait(v) == 0) ; sem_post(v)	/* �����������˴�Ҫ�����Ƴ���С��YMAX_NAME_LENGTH�ֽ� */
#define DDS_MUTEX_LOCKN(v)		sem_wait(v)											/* ���������� */
#define DDS_MUTEX_UNLOCKN(v)	sem_post(v)											/* ���������� */
#define DDS_MUTEX_CLOSE(v)		sem_close(v)										/* �ر������� */
/*******************�ź���******************/
/* �����ź��� */
typedef struct _SEMA
{
    sem_t		m_semS;
    sem_t *		m_semP;
} SEMA;																	/* �����ź��� */
#define DDS_SEMA_INIT(v, i)		(v).m_semP = &(v).m_semS; sem_init((v).m_semP, 0, i)	/* ��ʼ���ź��� */
#define DDS_SEMA_WAIT(v)		sem_wait((v).m_semP)						/* �ȴ��ź��� */
/* ���Եȴ��ź��� */
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
#define DDS_SEMA_POST(v)		sem_post((v).m_semP)						/* �ͷ��ź��� */
#define DDS_SEMA_DESTROY(v)		sem_destroy((v).m_semP)						/* �����ź��� */
/* ��������ԡ�/����ͷ����ֻ��һ����/�����硰/MySema�����Լ���linux��ϵͳ */
#define DDS_SEMA_OPEN(v, n)		(v).m_semP = sem_open(n, O_CREAT, 0644, 0)	/* �������ź������˴�Ҫ�����Ƴ���С��YMAX_NAME_LENGTH�ֽ� */
#define DDS_SEMA_CLOSE(v)		sem_close((v).m_semP)						/* �ر������ź��� */
/*******************�߳�******************/
#define DDS_THREAD_ID pthread_t							/* �߳�ID���� */
typedef HANDLE    DDS_THREAD_HANDLE;
#endif


#ifdef _VXWORKS
#define STACALL

#ifndef _DO_WHILE_0
#define _DO_WHILE_0(content) do { content } while (0)
#endif /* _DO_WHILE_0 */

/* Nephalem ��������Դ */
typedef SEM_ID		DDS_MUTEX;												/* ������ */
#define DDS_MUTEX_INIT(v)		_DO_WHILE_0({ v = semMCreate(0); }) 	/* ��ʼ���� */
#define DDS_MUTEX_LOCK(v)		semTake(v,WAIT_FOREVER);							/* ���� */
#define DDS_MUTEX_UNLOCK(v)		semGive(v);							/* ���� */
#define DDS_MUTEX_DESTROY(v)	semDelete(v);

#define DDS_THREAD_ID pthread_t							/* �߳�ID���� */
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
//����ϵͳ��ص���������
/*TODO��ȥ��BOOL��INT�Ķ��壬����vxWorks�³ɹ�����*/
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

/*Sleep�����ڲ�ͬ����ϵͳ�е�ʵ��*/
static void QY_Sleep(UINT sleepTime)
{
#if defined(_WIN32)
    Sleep(sleepTime);
#elif defined(__linux__)
/*TODO��sleep�ӿڣ�vxWorks��usleep�����޷�ʹ��*/
	   struct timespec tc;
       tc.tv_sec = sleepTime / 1000;
       tc.tv_nsec = (sleepTime % 1000) *  1,000,000;
       nanosleep(&tc,NULL);
#elif defined(_VXWORKS)
        
       taskDelay(sleepTime * sysClkRateGet() / 1000);
#endif
}

/********************************************************************
 �����Ǹ���Ŀ��ص�����ͷ�ļ����书������
********************************************************************/
/*TODO�����Cpp�ı���֧��*/
#ifdef __cplusplus
extern "C"{
#endif
//����ӳ�䣬����ƽ̨���Ͳ���
#include "CommonTypeDefine.h"

/* ProcessUnity.h */
#include "ProcessUnity.h"

/* MemoryBlock�ṹ����ض��弰���� */
#include "MemoryBlock.h"

//OMG��IDL��CDR�������ػ���
#include "CDRTool.h"

#include "ConfFile.h"

//RTPS�淶��Duration_t��Ӧ���������ͼ���ش������
#include "Duration_t.h"

//RTPS�淶��Guid_t��Ӧ���������ͼ���ش������
#include "GUID_t.h"

#include "ReturnCode.h"

//�߾���ʱ����ص����ݽṹ�ʹ������
#include "TimeTool.h"

/* ��Ƭ���ݺ� */
#include "FragmentNumber.h"

//64���ص����к�
#include "SequenceNumber_t.h"

/* SequenceNumber���Ͻṹ�嶨�� */
#include "SequenceNumberSet.h"

#include "FragmentNumber.h"

#include "FragmentNumberSet.h"

/* ��ʷ�������������Ͷ��弰��ز��� */
#include "HistoryCache.h"

//�����˶���ɹ̶���󳤶ȵ��ַ������߽ṹ
#include "BoundedString.h"

//RTPS�淶��BuiltinTopicKey_t��Ӧ���������ͼ���ش������
#include "BuiltinTopicKey_t.h"

//DCPS�淶��UserDataQosPolicy��Ӧ���������ͼ���ش������
#include "UserDataQosPolicy.h"

//�洢�ַ�������ֵ�Ե��б�ṹ
#include "PropertyList.h"

//��ʾRTPSЭ���еĲ����Լ������б�ṹ��ParameterList������ز�������
#include "ParameterList.h"

//RTPS�淶��BuiltinEndpointSet_t��Ӧ���������ͼ���ش������
#include "BuiltinEndpointSet_t.h"

//�淶��Locator_t��Ӧ���������ͼ���ش������
#include "Locator_t.h"

//RTPS�淶��ProtocolVersion_t��Ӧ���������ͼ���ش������
#include "ProtocolVersion_t.h"

//RTPS�淶��VendorId_t��Ӧ���������ͼ���ش������
#include "VendorId_t.h"

/* QosPolicy.h */
#include "QosPolicy.h"

/* QosService.h */
#include "QosService.h"

//#include "Configure.h"

//RTPS�淶��������Ϣ��Header�ṹ
#include "RTPSMessageHeader.h"

#include "RTPSDiscovery.h"

//RTPS����Ϣͷ����������Ϣ���ͱ�־������
#include "SubMessageHeader.h"

//RTPS��Discoveryģ��������SPDP��ParticipantData�ṹ
#include "ParticipantData.h"

//Զ�̵�Participant����SPDP��SEDP�����ı�Ҫ��Ϣ�����������ַ�Ͷ˿�
#include "ParticipantProxy.h"

#include "SerializedData.h"

/* ������Ϣͷ���ݽṹ������ز���������� */
#include "Data.h"

/* ��������GUID */
#include "GuidGen.h"

/* ����ģʽ���繤���࣬��װ���õ�������� */
#include "NetWorkUnity.h"

/* ��װ���ַ����йصĲ��� */
#include "StringUnity.h"

/* AckNack����Ϣ�ṹ�弰����ر������� */
#include "AckNack.h"

#include "NackFrag.h"

/* Heartbeat����Ϣ�ṹ�弰����ر������� */
#include "Heartbeat.h"

/* ���ཻ��SequenceNumber_t���伯�� */
//#include "DisjointSequence.h"

#include "MajorTask.h"

/* �洢ĳ����·�ķ�Ƭ�յ�����Ϣ */
//#include "RTPSFragInfo.h"

/* DataFrag�ӱ��� */
#include "DataFrag.h"

/* InfoTimestamp�ӱ��� */
#include "InfoTimestamp.h"

/* InfoDestination �ӱ��� */
#include "InfoDestination.h"

/* InfoReply �ӱ��� */
#include "InfoReply.h"

/* gap�ӱ���*/
#include "Gap.h"

/*��־ģ��*/
#include "CommLog.h"

/*�������*/
#include "ListOper.h"

/* ��̬�ڴ���� */
#include "MemoryAlloc.h"

/* Զ��д���� */
#include "DiscoveredWriter.h"

/* Զ�˶�ȡ�� */
#include "DiscoveredReader.h"

/* ����д���� */
#include "DataWriter.h"

/* ���ݶ�ȡ��*/
#include "DataReader.h"

/* ���� */
#include "Topic.h"

/* ���ֲ����� */
#include "DiscoveredParticipant.h"

/* ����д���� */
#include "BuiltinDataWriter.h"

/* ���ݶ�ȡ��*/
#include "BuiltinDataReader.h"

/* ��������� */
#include "Monitor.h"

/* ������ */
#include "DomainParticipant.h"
 
#ifdef RAPID_IO_CHANNEL
#include "RapidIO/tsi721api.h"
#include "RapidIO/tsi721ioctl.h"
#include "RapidIO/RioUtility.h"
#endif

/* �����ʼ�� */
#include "NetWorkStructure.h"

/* �����ֽ������ */
#include "NetWorkOrder.h"

/* SPDP���Ĵ���ģ�� */
#include "Spdp.h"

/* SEDP����ģ�� */
#include "Sedp.h"

/* �û��շ����� */
#include "User.h"

/* �������SPDP���ֱ���֮������б��� */
 #include "RTPSReceiveStrategy.h"
#ifdef __cplusplus
}
#endif

//#ifdef _WIN32
#include "FACE/TypeAbstraction_TS.h"
//#endif
#endif



