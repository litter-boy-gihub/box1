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
    AckNack                     stAckNack;			   /* 远端阅读器回复本地写入器Acknack报文 */
    UINT32                      uiSendHeartBeatNum;    /* 最后回复ack截至后发送心跳个数，超过规定值，失去连接，删除该远端阅读器 */
    struct _LocalWriter*        pNext;
}DDS_LocalWriter;

typedef struct _DiscoveredReader
{
	/************************ 基础信息 ****************************/
    GUID_t                      guid;
    DDS_DataReaderQos           stDataReaderQos;
	Locator_t*                  pstUnicastLocator;
	struct _Topic*              pstTopic;         /* 远端Topic信息，Data(r)中包含的主题信息，如TopicQoS */
	/************************ 行为信息 ****************************/
	BOOL                        bVaild;           /* 对端内建是否也已建立连接，通过对端回复的ack报文确认 */
    BOOL                        bSamePartiton;    /* Nephalem 判断是否是相同分区 */
	UINT                        heartMsgNum;	  // 发端在发送心跳前已经发送了多少包数据
	Duration_t                  lastHeartTime;    // 发端上次发送心跳的时间

	/************************ 关联节点 ****************************/
	DDS_LocalWriter*            pstLocalWriter;   // 父节点Topic下关联的本地writer链表，？？奇怪的设计
    struct _Topic*              pstRelateTopic;   /* Nephalem 自身对应的本地Topic父节点 */

	struct _DiscoveredReader*   pstTwinDiscReader; /* 相同域参与者的兄弟节点链表，当发现有相同域参与者的reader注册时，插入到链表尾部 */
    struct _DiscoveredReader*   pNext;             /* 不同域参与者的节点 */
}DDS_DiscoveredReader;

/* 初始化远端阅读器 */
extern VOID DDS_DiscoveredReader_Init(DDS_DiscoveredReader * pstDiscReader);
/* 去始化远端阅读器 */
extern VOID DDS_DiscoveredReader_Uninit(DDS_DiscoveredReader* pstDiscReader);
/* 添加本地写入器 */
extern BOOL DiscoveredReaderAddLocalWriter(DDS_DiscoveredReader* pstDiscReader, GUID_t* pstGuid);
/* 删除本地写入器 */
extern BOOL DiscoveredReaderDelLocalWriter(DDS_DiscoveredReader* pstDiscReader, GUID_t* pstGuid);
/* 兄弟写入器置顶 */
extern VOID TwinDiscoveredReaderChangePosition(DDS_DiscoveredReader* pstDiscReader);
/* 获取本地写入器 */
extern DDS_LocalWriter*  GetDiscoveredReaderLocalWriterByGuid(DDS_DiscoveredReader* pstDiscReader, GUID_t* pstGuid);

#endif
