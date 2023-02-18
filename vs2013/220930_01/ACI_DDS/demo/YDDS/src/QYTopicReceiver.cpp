#include "QYdds.h"
void STACALL ReceiveUserData(DDS_DataReader* pstDataReader)
{
	UINT32 dataNum = 0;
	DataSeq stDataSeq;
	DataLen dataLen;
	CHAR* msg;
	QYTopicParam param;
	QYTopicReceiver* receiver = (QYTopicReceiver *)pstDataReader->params[0];
	/* 收数据 */
	DDS_ReturnCode_t retCode = DDS_StringDataReader_take(pstDataReader, &stDataSeq);
	if (DDS_RETCODE_NO_DATA != retCode)
	{
		for (dataNum = 0; dataNum < stDataSeq.length; dataNum++)
		{
			msg = DDS_StringSeq_get(&stDataSeq, dataNum, &dataLen);

			param.m_param = msg;
			param.m_paramLength = dataLen;
			param.m_topicType = YTOPIC_NET;
			strcpy(param.m_topicName, pstDataReader->pstTopic->topicName.value);

			receiver->onReceiveTopic(param);

		}
		/* 释放缓存 */
        DDS_StringDataReader_return_loan(pstDataReader, &stDataSeq);
	}
}

QYTopicReceiver::QYTopicReceiver()
{

}

QYTopicReceiver::~QYTopicReceiver()
{

}

bool QYTopicReceiver::registTopic(const char * topicName, DDS_DataReaderQos* rdQos, QYTOPIC_TYPE topicType)
{
	strcpy(m_topicName, topicName);
	m_topicType = topicType;
	/* 创建主题 */
    pstTopic = DDS_DomainParticipant_get_topic_by_name(g_pstParticipant, m_topicName, m_topicName);
	if (pstTopic == NULL)
	{
		pstTopic = DDS_DomainParticipant_create_topic(g_pstParticipant, m_topicName, m_topicName, NULL);
		if (NULL == pstTopic)
		{
			printf("ERROR! Failed to create topic! %s\n", m_topicName);
			return false;
		}
 	}
	DDS_DataReaderQos strdQos;
	DDS_Topic_get_default_datareader_qos(clkTopic, &strdQos);
	strdQos.reliability.kind = BEST_EFFORT_RELIABILITY_QOS;
	strdQos.history.kind = KEEP_LAST_HISTORY_QOS;
	strdQos.history.depth = 80;
	pstDataReader = DDS_Topic_create_datareader(pstTopic, &strdQos, ReceiveUserData);
	if (NULL == pstDataReader)
	{
		printf("ERROR! Failed to create dataReader!");
		return false;
	}
	pstDataReader->params[0] = (UINT64)this;
	return true;
}
bool QYTopicReceiver::registClock()
{
	/* 创建主题 */
	clkTopic = DDS_DomainParticipant_get_topic_by_name(clock_Participant, "Timer", "Timer");
	if (clkTopic == NULL)
	{
		clkTopic = DDS_DomainParticipant_create_topic(clock_Participant, "Timer", "Timer", NULL);
		if (NULL == clkTopic)
		{
			printf("ERROR! Failed to create topic! %s\n", "Timer");
			return false;
		}
	}
	DDS_DataReaderQos strdQos;
	DDS_Topic_get_default_datareader_qos(clkTopic, &strdQos);
	strdQos.reliability.kind = BEST_EFFORT_RELIABILITY_QOS;
	strdQos.history.kind = KEEP_ALL_HISTORY_QOS;
	strdQos.history.depth = 80;


	clkDataReader = DDS_Topic_create_datareader(clkTopic, &strdQos, ReceiveUserData);
	if (NULL == clkDataReader)
	{
		printf("ERROR! Failed to create dataReader!");
		return false;
	}
	clkDataReader->params[0] = (UINT64)this;
	return true;

}
//取消注册接收主题，topicName=主题名称（长度小于YMAX_NAME_LENGTH字节），topicType=主题类型
bool QYTopicReceiver::unregistTopic(const char * topicName, QYTOPIC_TYPE topicType)
{
	DDS_DataReader_delete_from_topic(pstDataReader);

	return false;
}
