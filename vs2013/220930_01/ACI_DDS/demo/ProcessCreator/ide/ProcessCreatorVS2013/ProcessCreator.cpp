// ProcessCreator.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
extern "C"
{
#include "../include/GlobalDefine.h"
}
#include <map>
#pragma comment(lib,"ACI_DDS.lib")
std::map<int, PROCESS_INFORMATION> mapHandles;

int main1()
{
	int val = 0;
	std::cin >> val;
	if (val > 0)
	{
		INT32 iType = 0;
		BOOL bIsChangeDataLen = 0;
		int OwnerShipStrengh = 0;
		CHAR  TempPartitionName[256] = { 0 };

		INT32 RelableTest = 0;
		INT32 BigDataTest = 0;
		INT32 PartitionTest = 0;
		INT32 ResourceLimitTest = 0;
		INT32 OwnerShipTest = 0;

		UINT32 i = 0;
		CHAR* buff = NULL;
		UINT32 uiLen = 64 * 15;
		DDS_ReturnCode_t retureCode;
		DDS_DomainParticipantQos stDomPartQos;
		DDS_TopicQos stTopicQos;
		DDS_DataWriterQos stWriterQos;
		DDS_DataReaderQos stReaderQos;


		/* 资源初始化 */
		DDS_ParticipantFactory_init();

		/* 获取参与者qos */
		retureCode = DDS_DomainParticipantFactory_get_defaul_participant_qos(&stDomPartQos);
		if (DDS_RETCODE_ERROR == retureCode)
		{
			printf("ERROR! Data writer setup error !");
			return 0;
		}

		/* 绑定网卡，不设置默认第一个网卡 */
		//DDS_Participant_bind_network(&stDomPartQos, "192.168.100.186");

		/* 创建参与者 */
		DDS_DomainParticipant* pstParticipant = DDS_DomainParticipantFactory_create_participant(55, &stDomPartQos);
		if (NULL == pstParticipant)
		{
			printf("ERROR! Failed to create participant!");
			return 0;
		}

		/* 创建主题 */
		DDS_Topic* pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, "HELLO", "HELLO", NULL);
		if (NULL == pstTopic)
		{
			printf("ERROR! Failed to create topic!");
			return 0;
		}

		/* 获取写入器qos */
		retureCode = DDS_Topic_get_default_datawriter_qos(pstTopic, &stWriterQos);
		if (DDS_RETCODE_ERROR == retureCode)
		{
			printf("ERROR! Data writer setup error !");
			return 0;
		}
		stWriterQos.reliability.kind = BEST_EFFORT_RELIABILITY_QOS;
		
		/* 创建写入器 */
		DDS_DataWriter* pstDataWriter = DDS_Topic_create_datawriter(pstTopic, &stWriterQos, NULL);
		if (NULL == pstDataWriter)
		{
			printf("ERROR! Failed to create dataWriter!");
			system("pause");
			return 0;
		}

		/* 内存监控 */

		DDS_MemoryUsage();

		uiLen = 200 * 1024;
		/* 待发送数据内存分配 */
		buff = (CHAR*)DDS_STATIC_MALLOC(uiLen);
		if (NULL == buff)
		{
			printf("ERROR! Insufficient memory space.\n");
			return 0;
		}

		memset(buff, 'A', uiLen);
		buff[uiLen - 1] = 0;
		Sleep(1000);
		retureCode = DDS_StringDataWriter_write(pstDataWriter, (const char*)&val, 4);


		for (int i = 0; i < val; ++i)
		{
			char buf[32] = { 0 };
			sprintf_s(buf, "sender.exe %d", i);
			PROCESS_INFORMATION p;
			STARTUPINFOA s = {0};
			BOOL b = CreateProcessA("sender.exe", buf,NULL,NULL,FALSE,NULL,NULL,NULL,&s,&p);
			if (b)
			{
				mapHandles[i] = p;
			}
			else
			{
				printf("Create Failed!\n");
			}
			//system(buf);
		}


		std::cin >> val;
		//system("taskkill /im sender.exe /f");
		for (auto& it : mapHandles)
		{
			PROCESS_INFORMATION& p = it.second;
			TerminateProcess(p.hProcess, 0);
			//mapHandles.erase(it.first);
		}
		std::cout << "Hello World!\n";
		return 0;
	}
}

int main()
{
	int val = 20;
	//std::cin >> val;

	for (int i = 0; i < val; ++i)
	{
		Sleep(10);
		char buf[32] = { 0 };
		sprintf_s(buf, "Client.exe %d", i);
		PROCESS_INFORMATION p;
		STARTUPINFOA s = { 0 };
		BOOL b = CreateProcessA("Client.exe", buf, NULL, NULL, FALSE, NULL, NULL, NULL, &s, &p);
		if (b)
		{
			mapHandles[i] = p;
		}
		else
		{
			printf("Create Failed!\n");
		}
		//system(buf);
	}


	std::cin >> val;
	//system("taskkill /im sender.exe /f");
	for (auto& it : mapHandles)
	{
		PROCESS_INFORMATION& p = it.second;
		TerminateProcess(p.hProcess, 0);
		//mapHandles.erase(it.first);
	}
	std::cout << "Hello World!\n";
	return 0;
}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
