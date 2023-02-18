#ifndef YTOPIC_RECEIVER_H
#define YTOPIC_RECEIVER_H

//�������ݲ���
struct  QYTopicParam
{
	QYTOPIC_TYPE				m_topicType;					//������������
	char					m_topicName[256];	            //��������
	unsigned int 			m_paramLength;					//��������
	const char *			m_param;						//��������
};

/*�������ݽ��ն�
ʹ��ʾ����
//�����Լ��Ľ��ն�
class MyTopicReceiver : public QYTopicReceiver
{
	//���յ���������ʱ�Ļص�����
	virtual QYMessageResult onReceiveTopic(const QYTopicParam & param)
	{
		return QYMessageResult(true, "hello", 6);
	}
};
MyTopicReceiver * myReceiver = new MyTopicReceiver();	//��������
myReceiver->registTopic("TestTopic", YTOPIC_NET);		//ע���������
//���������̷��͵���������ʱ�����Զ�ִ��MyTopicReceiver::onReceiveTopic����
*/
class DDS_DLL QYTopicReceiverImp;
class  DDS_DLL QYTopicReceiver
{
public:
	QYTopicReceiver();
	virtual ~QYTopicReceiver();

	//ע��������⣬topicName=�������ƣ�����С��YMAX_NAME_LENGTH�ֽڣ���topicType=��������
	//����ͬʱע������������
    bool registTopic(const char * topicName, DDS_DataReaderQos* rdQos, QYTOPIC_TYPE topicType = YTOPIC_NET);
	bool registClock();
	//ȡ��ע��������⣬topicName=�������ƣ�����С��YMAX_NAME_LENGTH�ֽڣ���topicType=��������
	bool unregistTopic(const char * topicName, QYTOPIC_TYPE topicType = YTOPIC_NET);

	//���յ���������ʱ�Ļص�����
	virtual QYMessageResult onReceiveTopic(const QYTopicParam & param) = 0;

private:
	QYTopicReceiverImp *			m_receiverImp;		//�ڲ�ʵ����

	char m_topicName[256];
	QYTOPIC_TYPE m_topicType;

	DDS_Topic* pstTopic;
	DDS_Topic* clkTopic;
	DDS_DataReader* pstDataReader;
	DDS_DataReader* clkDataReader;
};
DDS_DLL void DDS_SetReaderQos(DDS_DataReaderQos* rdQos);
#endif //YTOPIC_RECEIVER_H
