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
*  @file     Topic.h
*  @brief    topic information
*  Details.
*
*  @author   Hunter
*  @email    1028251654@qq.com
*  @version  1.0
*  @date     2019/07/24
*  @license
*
*--------------------------------------------------------------------------------
*  Remark         : Description
*--------------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>  | <operation> | <Description>
*--------------------------------------------------------------------------------
*  2019/07/24 | 1.0       | Hunter    | Create file |
*--------------------------------------------------------------------------------
*
*********************************************************************************/

#ifndef HIPERD_TOPIC_H
#define HIPERD_TOPIC_H

struct _DomainParticipant;
/****************************************************************************************************************************************/
//�ṹ�嶨��
/****************************************************************************************************************************************/

/*������Ϣ*/
typedef struct _Topic
{
	/******************** ������Ϣ ******************************/
	GUID_t                 tpGuid;			 /* ����guid */
	BoundedString256       topicName;		 /* ������ */
	BoundedString256       topicType;		 /* �������� */
	BOOL                   hasKey;			 /* ���������Ƿ�Ϊkey */
	DDS_TopicQos           stTopicQos;		 /* ������� */
	/*************** ��������ķ��������Ķ��б� *****************/
	DDS_DataWriter*        pstLocalWriter;	 /* ����д���� */
	DDS_DataReader*        pstLocalReader;   /* ���ض�ȡ�� */
	DDS_DiscoveredWriter*  pstDiscWriter;    /* Զ��д���� */
	DDS_DiscoveredReader*  pstDiscReader;    /* Զ�˶�ȡ�� */
	/********************* �û��ύ���ݵĻ��� ********************/
	HistoryCache           stUserData;		 /* �ύ����û����� */

	Locator_t*             pstMonLocator;    /* ��ع��߽ڵ��ַ */
	struct _DomainParticipant* pstParticipant;/* ������Participant */
#ifdef EVO_DDS
	INT topicNum;
#endif
	/* �¸��ڵ�ָ�� */
	struct _Topic*      pNext;
}DDS_Topic;

/****************************************************************************************************************************************/
// �����û��ӿں�������
/****************************************************************************************************************************************/
/******************************* ���������á�ɾ��dp ***************************************/
/* ��ȡĬ��д����qos */
extern DDS_DLL DDS_ReturnCode_t DDS_Topic_get_default_datawriter_qos(DDS_Topic* pstTopic, DDS_DataWriterQos* pstWriterQos);
/* ��ȡĬ��д����qos c# */
extern DDS_DLL DDS_DataWriterQos* DDS_Topic_get_default_datawriter_qos_cs(DDS_Topic* pstTopic);
/* ��ȡĬ���Ķ���qos */
extern DDS_DLL DDS_ReturnCode_t DDS_Topic_get_default_datareader_qos(DDS_Topic* pstTopic, DDS_DataReaderQos* pstReaderQos);
/* ��ȡĬ���Ķ���qos c#*/
extern DDS_DLL DDS_DataReaderQos* DDS_Topic_get_default_datareader_qos_cs(DDS_Topic* pstTopic);
/* д�������ÿɿ��� */
extern DDS_DLL DDS_ReturnCode_t DDS_Topic_datawriter_qos_set_reliability(DDS_DataWriterQos* pstWriterQos, ReliabilityQosPolicyKind kind);
/* �Ķ������ÿɿ��� */
extern DDS_DLL DDS_ReturnCode_t DDS_Topic_datareader_qos_set_reliability(DDS_DataReaderQos* pstReaderQos, ReliabilityQosPolicyKind kind);

/* ���ⴴ��д���� */
extern DDS_DLL DDS_DataWriter* DDS_Topic_create_datawriter(DDS_Topic* pstTopic, DDS_DataWriterQos* pstWriterQos, CALLBACKTIME callBackTime);
/* �����ȡд�������� */
extern DDS_DLL UINT32 DDS_Topic_get_datawriter_num(DDS_Topic* pstTopic);

/* ���ⴴ�Ķ��� */
extern DDS_DLL DDS_DataReader* DDS_Topic_create_datareader(DDS_Topic* pstTopic, DDS_DataReaderQos* pstReaderQos, CALLBACKFUN callBackFun);
/* ����ɾ��д���� */
extern DDS_DLL DDS_ReturnCode_t DDS_Topic_delete_datawriter(DDS_Topic* pstTopic);

/* �����ȡ�Ķ������� */
extern DDS_DLL UINT32 DDS_Topic_get_datareader_num(DDS_Topic* pstTopic);

/* ����ɾ��д���� */
extern DDS_DLL DDS_ReturnCode_t DDS_DataWriter_delete_from_topic(DDS_DataWriter*  pstDataWriter);

/* ����ɾ���Ķ��� */
extern DDS_DLL DDS_ReturnCode_t DDS_Topic_delete_datareader(DDS_Topic* pstTopic);

/* ɾ���Ķ��� */
extern DDS_DLL DDS_ReturnCode_t DDS_DataReader_delete_from_topic(DDS_DataReader*  pstDataReader);

/* ��������ͨ��Guid������ӦԶ��д���� */
extern DDS_DiscoveredWriter* GetDiscoveredWriterFromTopicByGuid(DDS_Topic* pstTopic, GUID_t* pstGuid);

/* ��������ͨ��Guid������ӦԶ���Ķ��� */
extern DDS_DiscoveredReader* GetDiscoveredReaderFromTopicByGuid(DDS_Topic* pstTopic, GUID_t* pstGuid);

/* ��������ͨ��Guid������ӦԶ���Ķ��� */
extern DDS_DiscoveredReader* GetDiscoveredReaderFromTopicByPrefix(DDS_Topic* pstTopic, GuidPrefix_t* pstPrefix);

/* ������ɾ��ָ��GuidPrefixԶ��д���� */
extern VOID DeleteDiscWriterFromTopicByGuidPrefix(DDS_Topic* pstTopic, GuidPrefix_t*  pstPrefix);

/* ������ɾ��ָ��GuidPrefixԶ���Ķ��� */
extern VOID DeleteDiscReaderFromTopicByGuidPrefix(DDS_Topic* pstTopic, GuidPrefix_t*  pstPrefix);

/* ������ɾ��ָ��GUIDԶ��д���� */
extern VOID DeleteDiscWriterFromTopicByGuid(DDS_Topic* pstTopic, GUID_t*  pstGuid);

/* ������ɾ��ָ��GUIDԶ���ֵ��Ķ��� */
extern VOID DeleteTwinDiscReaderFromTopicByGuid(DDS_DiscoveredReader* pstDiscReader, GUID_t* pstGuid);

/* ������ɾ��ָ��GUIDԶ���Ķ��� */
extern VOID DeleteDiscReaderFromTopicByGuid(DDS_Topic* pstTopic, GUID_t* pstGuid);

/* Nephlame ��ȡ����Ȩ��ߵ�Զ��д���� */
extern void FindOwnerWriter(DDS_Topic* pstTopic);

/* Nephlame partition �ж��Ƿ�ӵ����ͬ�ķ����� */
extern BOOL EqualPartitionName(DDS_TopicQos* pstTopicQoSL, DDS_TopicQos* pstTopicQoSR);

/* Nephlame partition �ж��ַ����Ƿ���ȣ�֧��ͨ���*�ͣ� */
BOOL matchStr(char* pstStr, char* pstPattern);
/* Nephlame partition �ж��ַ����Ƿ���ͨ���*�� */
BOOL containAsteriskW(char* pstStr);

/****************************************************************************************************************************************/
// �ڲ��ӿں�������
/****************************************************************************************************************************************/
/* ������Ϣ��ʼ�� */
extern void DDS_Topic_Init(DDS_Topic* pstTopic);
/* ������Ϣ���� */
extern void DDS_Topic_Uninit(DDS_Topic* pstTopic);
#endif
