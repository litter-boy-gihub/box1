#include "QYdds.h"
#include <map>
#pragma comment(lib,"YDDS.lib")


struct DataLoader
{
	long long pid;
	long long val;
};


class ServerCommunicationReceiver :public QYTopicReceiver
{
	// 通过 QYTopicReceiver 继承
	virtual QYMessageResult onReceiveTopic(const QYTopicParam& param) override
	{
		return QYMessageResult();
	}
};
int regcount;
class ServerRegistTopicReceiver:public QYTopicReceiver
{
	std::map<long long, QYTopicSender*> mapSenders;
	// 通过 QYTopicReceiver 继承
	virtual QYMessageResult onReceiveTopic(const QYTopicParam& param) override
	{
		DataLoader* d = (DataLoader*)param.m_param;
		//printf_s("Recv regist,pid:%llu\n", d->pid);


		char buf[32] = { 0 };
		sprintf_s(buf, "%llu", d->pid);
		std::map<long long, QYTopicSender*>::iterator it = mapSenders.find(d->pid);
		if (it == mapSenders.end())
		{
			mapSenders[d->pid] = new QYTopicSender(buf, NULL);
			it = mapSenders.find(d->pid);
		}
		DataLoader data = { 0 };
		QYMessageResult ret = mapSenders[d->pid]->sendData((const char*)&data, sizeof(data));
		if (!ret.m_success)
		{
			printf("Reply Failed:%llu\n", d->pid);
		}
		regcount = mapSenders.size();

		return QYMessageResult();
	}
};


int main()
{
	YddsInit(50);
	char buf[32] = { 0 };
	DataLoader data = { 0 };
	data.pid = GetCurrentProcessId();
	sprintf_s(buf, "%llu", data.pid);
	ServerRegistTopicReceiver regsrv;
	regsrv.registTopic(buf,NULL);
	QYTopicSender sender("ServerLocation",NULL);
	int tm = 0;
	time_t realtime = time(NULL);
	time_t currenttime = realtime;
	time_t lasttime = currenttime;
	while (true)
	{
		currenttime = time(NULL);
		if (currenttime > lasttime)
		{
			lasttime = currenttime;
			sender.sendData((const char*)&data, sizeof(data));
			printf("%d %d\n", regcount, currenttime - realtime);
		}
		Sleep(100);
	}
}