#ifndef YTOPIC_SENDER_H
#define YTOPIC_SENDER_H

/*主题数据发送端
使用示例：
//创建发送端
QYTopicSender * sender = new QYTopicSender("TestTopic", YTOPIC_NET);
//发送数据，在等待时间YNORMAL_WAIT内得到返回值
QYMessageResult result = sender->sendData("hello", 6, YNORMAL_WAIT);
*/

class QYTopicSenderImp;

class  DDS_DLL QYTopicSender
{
public:
	//创建指定主题发送端，topicName=主题名称（长度小于YMAX_NAME_LENGTH字节），topicType=主题类型
    QYTopicSender(const char * topicName, DDS_DataWriterQos* wrQos, QYTOPIC_TYPE topicType = YTOPIC_NET);
	~QYTopicSender();

	//取得主题名称
	const char * topicName() const;
	//取得主题类型
	QYTOPIC_TYPE topicType() const;

	//发送主题数据 成功返回1 失败返回0
	QYMessageResult sendData(const char * data, unsigned short length, unsigned int = 0);

private:
	QYTopicSenderImp *			m_senderImp;			//内部实现类

};

extern DDS_DomainParticipant* g_pstParticipant;
extern DDS_DomainParticipant* clock_Participant;

DDS_DLL void YddsInit(int domainNum);

#endif //YTOPIC_SENDER_H
