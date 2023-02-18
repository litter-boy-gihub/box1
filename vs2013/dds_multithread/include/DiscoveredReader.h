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
*  @file     DiscoveredReader.h
*  @brief    DiscoveredReader
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

#ifndef HIPERD_DISCOVEREDREADER_H
#define HIPERD_DISCOVEREDREADER_H

struct _Topic;

typedef struct _LocalWriter
{
    GUID_t                      guid;
    AckNack                     stAckNack;      /* Զ���Ķ����ظ�����д����Acknack���� */
    UINT32                      uiSendHeartBeatNum;    /* ���ظ�ack�����������������������涨ֵ��ʧȥ���ӣ�ɾ����Զ���Ķ��� */
    struct _LocalWriter*        pNext;

}DDS_LocalWriter;

typedef struct _DiscoveredReader
{
    BOOL                        bVaild;           /* �Զ��ڽ��Ƿ�Ҳ�ѽ������ӣ�ͨ���Զ˻ظ���ack����ȷ�� */
    GUID_t                      guid;
    Locator_t*                  pstUnicastLocator;
    DDS_DataReaderQos           stDataReaderQos;
    DDS_LocalWriter*            pstLocalWriter;
    UINT                        heartMsgNum;    //�����ڷ�������ǰ�Ѿ������˶��ٰ�����
    Duration_t                  lastHeartTime;  //�����ϴη���������ʱ��

    struct _Topic*              pstTopic;

    BOOL                        bSamePartiton;    /* Nephalem �ж��Ƿ�����ͬ���� */
    struct _Topic*              pstRelateTopic;   /* Nephalem �����Ӧ��Topic */

    struct _DiscoveredReader*   pstTwinDiscReader;

    struct _DiscoveredReader*   pNext;

}DDS_DiscoveredReader;

/* ��ʼ��Զ���Ķ��� */
extern VOID DDS_DiscoveredReader_Init(DDS_DiscoveredReader * pstDiscReader);

/* ȥʼ��Զ���Ķ��� */
extern VOID DDS_DiscoveredReader_Uninit(DDS_DiscoveredReader* pstDiscReader);

/* ��ӱ���д���� */
extern BOOL DiscoveredReaderAddLocalWriter(DDS_DiscoveredReader* pstDiscReader, GUID_t* pstGuid);

/* ɾ������д���� */
extern BOOL DiscoveredReaderDelLocalWriter(DDS_DiscoveredReader* pstDiscReader, GUID_t* pstGuid);

/* �ֵ�д�����ö� */
extern VOID TwinDiscoveredReaderChangePosition(DDS_DiscoveredReader* pstDiscReader);

/* ��ȡ����д���� */
extern DDS_LocalWriter*  GetDiscoveredReaderLocalWriterByGuid(DDS_DiscoveredReader* pstDiscReader, GUID_t* pstGuid);

#endif
