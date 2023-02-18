#ifndef YTOPIC_SENDER_H
#define YTOPIC_SENDER_H

/*�������ݷ��Ͷ�
ʹ��ʾ����
//�������Ͷ�
QYTopicSender * sender = new QYTopicSender("TestTopic", YTOPIC_NET);
//�������ݣ��ڵȴ�ʱ��YNORMAL_WAIT�ڵõ�����ֵ
QYMessageResult result = sender->sendData("hello", 6, YNORMAL_WAIT);
*/

class QYTopicSenderImp;

class  DDS_DLL QYTopicSender
{
public:
	//����ָ�����ⷢ�Ͷˣ�topicName=�������ƣ�����С��YMAX_NAME_LENGTH�ֽڣ���topicType=��������
    QYTopicSender(const char * topicName, DDS_DataWriterQos* wrQos, QYTOPIC_TYPE topicType = YTOPIC_NET);
	~QYTopicSender();

	//ȡ����������
	const char * topicName() const;
	//ȡ����������
	QYTOPIC_TYPE topicType() const;

	//������������ �ɹ�����1 ʧ�ܷ���0
	QYMessageResult sendData(const char * data, unsigned short length, unsigned int = 0);

private:
	QYTopicSenderImp *			m_senderImp;			//�ڲ�ʵ����

};

extern DDS_DomainParticipant* g_pstParticipant;
extern DDS_DomainParticipant* clock_Participant;

DDS_DLL void YddsInit(int domainNum);

#endif //YTOPIC_SENDER_H
