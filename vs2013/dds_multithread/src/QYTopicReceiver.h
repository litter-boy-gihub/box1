#ifndef YTOPIC_RECEIVER_H
#define YTOPIC_RECEIVER_H

//主题数据参数
struct  QYTopicParam
{
	QYTOPIC_TYPE				m_topicType;					//主题数据类型
	char					m_topicName[256];	            //主题名称
	unsigned int 			m_paramLength;					//参数长度
	const char *			m_param;						//参数内容
};

/*主题数据接收端
使用示例：
//定义自己的接收端
class MyTopicReceiver : public QYTopicReceiver
{
	//接收到主题数据时的回调函数
	virtual QYMessageResult onReceiveTopic(const QYTopicParam & param)
	{
		return QYMessageResult(true, "hello", 6);
	}
};
MyTopicReceiver * myReceiver = new MyTopicReceiver();	//创建对象
myReceiver->registTopic("TestTopic", YTOPIC_NET);		//注册接收主题
//当其它进程发送的主题数据时，会自动执行MyTopicReceiver::onReceiveTopic函数
*/
class DDS_DLL QYTopicReceiverImp;
class  DDS_DLL QYTopicReceiver
{
public:
	QYTopicReceiver();
	virtual ~QYTopicReceiver();

	//注册接收主题，topicName=主题名称（长度小于YMAX_NAME_LENGTH字节），topicType=主题类型
	//可以同时注册多个接收主题
    bool registTopic(const char * topicName, DDS_DataReaderQos* rdQos, QYTOPIC_TYPE topicType = YTOPIC_NET);
	bool registClock();
	//取消注册接收主题，topicName=主题名称（长度小于YMAX_NAME_LENGTH字节），topicType=主题类型
	bool unregistTopic(const char * topicName, QYTOPIC_TYPE topicType = YTOPIC_NET);

	//接收到主题数据时的回调函数
	virtual QYMessageResult onReceiveTopic(const QYTopicParam & param) = 0;

private:
	QYTopicReceiverImp *			m_receiverImp;		//内部实现类

	char m_topicName[256];
	QYTOPIC_TYPE m_topicType;

	DDS_Topic* pstTopic;
	DDS_Topic* clkTopic;
	DDS_DataReader* pstDataReader;
	DDS_DataReader* clkDataReader;
};
DDS_DLL void DDS_SetReaderQos(DDS_DataReaderQos* rdQos);
#endif //YTOPIC_RECEIVER_H
