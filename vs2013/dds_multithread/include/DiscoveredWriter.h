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
*  @file     DiscoveredWriter.h
*  @brief    DiscoveredWriter
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

#ifndef HIPERD_DISCOVEREDWRITE_H
#define HIPERD_DISCOVEREDWRITE_H

#define HELDDATASIZE 512
#define HELDFRAGSIZE 512

struct _Topic;

typedef struct _DiscoveredWriter
{
    BOOL                        bVaild;           /* �Զ��ڽ��Ƿ�Ҳ�ѽ������ӣ�ͨ���Զ˻ظ���ack����ȷ�� */
    GUID_t                      guid;
    Locator_t*                  pstUnicastLocator;
    DDS_DataWriterQos           stDataWriterQos;
    AckNack                     stAckNack;          /* �����Ķ����ظ�Զ��д����Acknack���� */
    Heartbeat                   stHeartbeat;        /* Զ��д�������ͱ����Ķ������� */
    HistoryCache                stHistoryCache;     /* д������ʷ���� */
    struct _Topic*              pstTopic;
    struct _DiscoveredWriter*   pNext;

    
    struct _Topic*           pstRelateTopic;     /* Nephalem �����Ӧ��Topic */

#if defined QYDDS_MASSDATA
    HistoryCache                stMassDataHistoryCache; /* Nephalem massdata ���ݻ���*/
#endif
	
    UINT                        heartMsgNum;    //�ն��յ�����������˶���������
    Duration_t                  lastHeartTime;  //�ϴν��յ�����������ʱ��

	BOOL						durabilityFlag;		//�Ƿ��ѽ�����־û��������ʷ����

    BOOL                        bSamePartiton;    /* Nephalem �ж��Ƿ�����ͬ���� */

}DDS_DiscoveredWriter;

/* ��ʼ��Զ��д���� */
extern VOID DDS_DiscoveredWriter_Init(DDS_DiscoveredWriter * pstDiscWriter);

/* ȥʼ��Զ��д���� */
extern VOID DDS_DiscoveredWriter_Uninit(DDS_DiscoveredWriter * pstDiscWriter);

#endif