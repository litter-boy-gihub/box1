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
*  @file     Locator_t.h
*  @brief    Locator_t
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
Locator_t.h
RTPS�淶��Locator_t��Ӧ���������ͼ���ش������
********************************************************************/

#ifndef HIPERD_LOCATOR_T_H
#define HIPERD_LOCATOR_T_H

#define UDP_BUF_SIZE 256*1024
#define MAX_UDP_MSG_SIZE 64*1024

#define LOCATOR_ADDR_STR_LENGTH 16
#define LOCATOR_ADDR_IP4_LENGTH 4
#define LOCATOR_ADDR_IP4_STR_BEGIN 12

#define SIZE_OF_LOCATOR (4+4+LOCATOR_ADDR_STR_LENGTH)

/* Socket���� */
typedef enum SocketType
{
    UNICAST_SEND_LOCATOR,   /* �������Ͷ� */
    UNICAST_RECV_LOCATOR,   /* �������ն� */
    MULTICAST_SEND_LOCATOR, /* �ಥ���Ͷ� */
    MULTICAST_RECV_LOCATOR, /* �ಥ���ն� */
}SocketType;

/* Socket��Ϣ */
typedef struct SocketInfo
{
    SOCKET              socket;
#if defined _TM1 ||defined _TM3

#else
    struct sockaddr_in  sockAddr;
#endif
}SocketInfo;

/* �������� */
typedef enum LocatorKind
{
    LOCATOR_KIND_RESERVED = -1,
    LOCATOR_KIND_UDPv4 = 1,
    LOCATOR_KIND_UDPv6 = 2,
	LOCATOR_KIND_FC	   = 5
}LocatorKind;

typedef struct Locator_t
{
    EX_INT32    kind    ; /* ���䷽ʽ */
    EX_UINT32   port    ; /* �˿ں�   */
    /* address��ֵ��kind��ֵ������                                        */
    /* LOCATOR_KIND_UDPv4: ��12�ֽ�Ϊ0�������ֽ�abcdӳ��Ϊ��ַ"a.b.c.d"   */
    /* LOCATOR_KIND_UDPv6: 16�ֽ���IPv6�ĵ�ַһһ��Ӧ                     */
    EX_UINT8    address[LOCATOR_ADDR_STR_LENGTH];
    struct Locator_t* pNext;

}Locator_t;

#define LOCATOR_IS_EQUAL(leftLocator , rightLocator) \
    (## leftLocator ##.kind == ## rightLocator ##.kind) && \
    (## leftLocator ##.port == ## rightLocator ##.port) && \
    (!memcmp(## leftLocator ##.address , ## rightLocator ##.address , 16))


/*
 ����Ϊ�ýṹ����빤�ߺ�
*/
#define SERIALIZE_LOCATOR(locator , memBlock)                               \
    do                                                                      \
    {                                                                       \
        PUT_INT(memBlock, locator.kind)                                     \
        PUT_UNSIGNED_INT(memBlock, locator.port)                            \
        PUT_BYTE_ARRAY(memBlock, locator.address, LOCATOR_ADDR_STR_LENGTH)  \
    } while (0);                 

#define DESERIALIZE_LOCATOR(locator , memBlock,exchange)                             \
    do                                                                      \
    {                                                                       \
        GET_INT(memBlock, locator.kind,exchange)                                     \
        GET_UNSIGNED_INT(memBlock, locator.port,exchange)                            \
        GET_BYTE_ARRAY(memBlock, locator.address, LOCATOR_ADDR_STR_LENGTH)  \
    } while (0);


/*
�޸ĳɾ�̬�б���С�̶�
*/
#define LOCATORLISTSIZE 16
typedef struct LocatorList_t
{
    EX_UINT32 listSize                  ;
    EX_UINT32 curPos                    ;
    Locator_t listBuf[LOCATORLISTSIZE]  ;
}LocatorList_t;


#define INITIAL_LOCATOR_LIST(locatorList)           \
    do {                                            \
        locatorList ## .listSize = LOCATORLISTSIZE; \
        locatorList ## .curPos = 0;                 \
        }while(0);                                                               

#define INSERT_LOCATOR_BY_VALUE(kind , port , address , locatorList)        \
    do {                                                                    \
    if(## locatorList ## .curPos < ## locatorList ## .listSize) {           \
        locatorList ## .listBuf[ ## locatorList ## .curPos].kind = kind;    \
        locatorList ## .listBuf[ ## locatorList ## .curPos].port = port;    \
        memcpy( ## locatorList ## .listBuf[ ## locatorList ## .curPos].address , address , 16);\
        locatorList ## .curPos++;                                           \
    }\
    }while(0);

#define INSERT_LOCATOR(locatorList, srcLocator)                                                 \
    do                                                                                          \
    {                                                                                           \
        if (locatorList.curPos < locatorList.listSize)                                          \
        {                                                                                       \
            locatorList.listBuf[locatorList.curPos].kind = srcLocator.kind;                     \
            locatorList.listBuf[locatorList.curPos].port = srcLocator.port;                     \
            memcpy(locatorList.listBuf[locatorList.curPos].address, srcLocator.address, 16);    \
            locatorList.curPos++;                                                               \
        }                                                                                       \
    } while (0);

/*
 ����Ϊ�ýṹ����빤�ߺ�
*/

#define SERIALIZE_LOCATOR_LIST(locatorList , memBlock)                                                          \
    do                                                                                                          \
    {                                                                                                           \
        int locatorListIndex_local;                                                                             \
        PUT_UNSIGNED_INT(memBlock, locatorList.curPos)                                                          \
        for (locatorListIndex_local = 0; locatorListIndex_local < locatorList.curPos; locatorListIndex_local++) \
        {                                                                                                       \
            SERIALIZE_LOCATOR(locatorList.listBuf[locatorListIndex_local],memBlock)                             \
        }                                                                                                       \
    } while (0);                                                                                                 

#define DESERIALIZE_LOCATOR_LIST(locatorList , memBlock,exchange)                                                        \
    do                                                                                                          \
    {                                                                                                           \
        int locatorListIndex_local;                                                                             \
        GET_UNSIGNED_INT(memBlock,locatorList.curPos,exchange)                                                           \
        if(locatorList.curPos > locatorList.listSize)                                                           \
        {                                                                                                       \
            /* ���ȳ�����Χ */                                                                                  \
            /* TODO : add error handle */                                                                       \
        }                                                                                                       \
        for (locatorListIndex_local = 0; locatorListIndex_local < locatorList.curPos; locatorListIndex_local++) \
        {                                                                                                       \
            DESERIALIZE_LOCATOR(locatorList.listBuf[locatorListIndex_local],memBlock,exchange)                           \
        }                                                                                                       \
    } while (0);   


#endif
