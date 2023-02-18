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
	/************************ 基础信息 ****************************/
    GUID_t                      guid;
    DDS_DataWriterQos           stDataWriterQos;
    Locator_t*                  pstUnicastLocator;
	HistoryCache                stHistoryCache;     /* 写入器历史数据 */
#if defined QYDDS_MASSDATA
	HistoryCache                stMassDataHistoryCache; /* Nephalem massdata 数据缓存*/
#endif
	struct _Topic*              pstTopic;			/* 远端Topic信息，Data(w)中包含的主题信息，如TopicQoS */
	/************************ 行为信息 ****************************/
    BOOL                        bVaild;           /* 对端内建是否也已建立连接，通过对端回复的ack报文确认 */
    AckNack                     stAckNack;          /* 本地阅读器回复远端写入器Acknack报文 */
    Heartbeat                   stHeartbeat;        /* 远端写入器发送本地阅读器心跳 */
	UINT                        heartMsgNum;    //收端收到心跳后接收了多少新数据
	Duration_t                  lastHeartTime;  //上次接收到发端心跳的时间
	Duration_t					lastLivelinessTime;	//上次接收数据的时间
	BOOL						durabilityFlag;		//是否已接收完持久化缓存的历史数据
	BOOL                        bSamePartiton;    /* Nephalem 判断是否是相同分区 */
   
    struct _Topic*				pstRelateTopic;     /* Nephalem 自身对应的本地Topic父节点 */
	struct _DiscoveredWriter*   pNext;
}DDS_DiscoveredWriter;

/* 初始化远端写入器 */
extern VOID DDS_DiscoveredWriter_Init(DDS_DiscoveredWriter * pstDiscWriter);

/* 去始化远端写入器 */
extern VOID DDS_DiscoveredWriter_Uninit(DDS_DiscoveredWriter * pstDiscWriter);

#endif
