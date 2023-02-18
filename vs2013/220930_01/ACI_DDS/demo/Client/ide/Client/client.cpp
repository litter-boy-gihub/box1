#include "QYdds.h"
#pragma comment(lib,"YDDS.lib")
struct DataLoader
{
	long long pid;
	long long val;
};
bool registed = false;
std::string senderid;
class ClientReplyReceiver :public QYTopicReceiver
{
	// 通过 QYTopicReceiver 继承
	virtual QYMessageResult onReceiveTopic(const QYTopicParam& param) override
	{
		registed = true;
		DataLoader data = { 0 };
		data.pid = GetCurrentProcessId();
		printf_s("Server regist Success,pid:%llu\n", data.pid);
		return QYMessageResult();
	}
};
QYTopicSender* g_sender = NULL;
class ServerLocationTopicReceiver :public QYTopicReceiver
{
	// 通过 QYTopicReceiver 继承
	virtual QYMessageResult onReceiveTopic(const QYTopicParam& param) override
	{
		if (!registed)
		{
			DataLoader* d = (DataLoader*)param.m_param;
			char buf[32] = { 0 };
			sprintf_s(buf, "%llu", d->pid);
			DataLoader data = { 0 };
			data.pid = GetCurrentProcessId();
			//printf_s("Server regist,pid:%llu\n", data.pid);
			if (!g_sender)
			{
				g_sender = new QYTopicSender(buf, NULL);
			}
			//printf("Sender %s reggisting!\n", senderid.c_str());
			QYMessageResult ret = g_sender->sendData((const char*)&data, sizeof(data));
			if (!ret.m_success)
			{
				printf("Regist Failed:%llu\n", data.pid);
			}
		}
		return QYMessageResult();
	}
};

int main(int argc,char* argv[])
{
	if (argc > 1)
	{
		printf("Sender %s begin!\n",argv[1]);
		senderid = argv[1];
	}
	YddsInit(110);
	DataLoader data = { 0 };
	data.pid = GetCurrentProcessId();
	char buf[32] = { 0 };
	sprintf_s(buf, "%llu", data.pid);
	ClientReplyReceiver creceiver;
	creceiver.registTopic(buf,NULL);


	ServerLocationTopicReceiver receiver;
	receiver.registTopic("ServerLocation",NULL);
	while (true)
	{
		Sleep(1000);
	}
}