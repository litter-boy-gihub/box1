#include "QYdds.h"

class QYTopicSenderImp
{
public:
	char m_topicName[256];
	QYTOPIC_TYPE m_topicType;

	DDS_Topic* pstTopic;
	DDS_DataWriter* pstDataWriter;
};


DDS_DomainParticipant* g_pstParticipant;
DDS_DomainParticipant* clock_Participant;

QYTopicSender::QYTopicSender(const char * topicName, DDS_DataWriterQos* wrQos, QYTOPIC_TYPE topicType)
{
	m_senderImp = new QYTopicSenderImp();
	if (NULL == m_senderImp)
	{
		return ;
	}

	strcpy(m_senderImp->m_topicName, topicName);
	m_senderImp->m_topicType = topicType;

	/* 创建主题 */
    m_senderImp->pstTopic = DDS_DomainParticipant_get_topic_by_name(g_pstParticipant, topicName, topicName);
    if (m_senderImp->pstTopic == NULL)
    {
        m_senderImp->pstTopic = DDS_DomainParticipant_create_topic(g_pstParticipant, topicName, topicName, NULL);
        if (NULL == m_senderImp->pstTopic)
        {
            printf("ERROR! Failed to create topic! %s\n", topicName);
        }
    }
	DDS_DataWriterQos strdQos = {};
	DDS_Topic_get_default_datawriter_qos(m_senderImp->pstTopic, &strdQos);
	strdQos.reliability.kind = BEST_EFFORT_RELIABILITY_QOS;
	strdQos.history.kind = KEEP_LAST_HISTORY_QOS;
	strdQos.history.depth = 80;

	//strdQos.resource_limits.max_samples = 500;
	//strdQos.resource_limits.max_instances = 1;
	//strdQos.resource_limits.max_samples_per_instance = 500;

	/* 创建第一个写入器 */
    m_senderImp->pstDataWriter = DDS_Topic_create_datawriter(m_senderImp->pstTopic, &strdQos, NULL);
	if (NULL == m_senderImp->pstDataWriter)
	{
		printf("ERROR! Failed to create dataWriter!");
	}
}

QYTopicSender::~QYTopicSender()
{
	DDS_DataWriter_delete_from_topic(m_senderImp->pstDataWriter);
}

const char * QYTopicSender::topicName() const
{
	return m_senderImp->m_topicName;
}


QYTOPIC_TYPE  QYTopicSender::topicType() const
{
	return m_senderImp->m_topicType;
}


QYMessageResult QYTopicSender::sendData(const char * data, unsigned short length, unsigned int waitMSecond)
{
	QYMessageResult msgRet = {true, 0, NULL};

	DDS_ReturnCode_t retureCode = DDS_StringDataWriter_write(m_senderImp->pstDataWriter, data, length);
	//printf("retCode: %d\n", retureCode);
	if (DDS_RETCODE_OK != retureCode /*&& DDS_RETCODE_NO_AVAILABLE_READER != retureCode*/)
	{
		printf("ERROR! Insufficient memory space.\n");
	    msgRet.m_success = false;
	}

	return msgRet;
}


DDS_DLL void YddsInit(int domainNum)
{
	/* 资源初始化 */
	DDS_ParticipantFactory_init();

    g_pstParticipant = DDS_DomainParticipantFactory_create_participant(domainNum, NULL);
	//if (true)
	//{
	//	if (domainNum != 110)
	//	{
	//		clock_Participant = DDS_DomainParticipantFactory_create_participant(110, NULL);
	//	}
	//	else
	//	{
	//		clock_Participant = g_pstParticipant;
	//	}
	//}
	//else
	//{
	//	clock_Participant = DDS_DomainParticipantFactory_create_participant(110, NULL);
	//}
}
