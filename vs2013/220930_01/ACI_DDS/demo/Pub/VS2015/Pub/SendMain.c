#include "GlobalDefine.h"

/*
该用例的进程中仅具有写入器，写入器为可靠传输，通过写入器发送50包字节大小为32的数据，启动接收端进程，在接收端
进程中有同主题的阅读器，阅读器收到写入器发送的数据后会将接收的数据打印在终端。
可以通过修改全局变量g_packNum的值设置发送的总包数，通过g_dataLength的值规定每包数据的字节大小。
写入器和阅读器均在KEEP_LAST_HISTORY_QOS的模式下，历史深度depth的值为10，资源限制的max_samples的值
也为10。写入器间隔1s发送一次数据，阅读器回调函数ReceiveData(DDS_DataReader* pstDataReader)中会打印
写入器发送的数据。设置持久化TRANSIENT_DURABILITY_QOS模式，先启动DDS_Send.exe,待DDS_Send.exe发送5包数据后，再启动DDS_Recv.exe，DDS_Recv.exe可以接收到DDS_Send.exe
之前发送的5包数据。
*/

/* 写入器发送的总包数 */
static int g_packNum = 50;

/* 写入器发送的包的字节大小 */
static int g_dataLength = 32;

extern __declspec(dllimport) int sendmsgnum;
extern struct FCSendMsgIDType  fc_send_msgid_table[256];
extern struct FCRecvMsgIDType  fc_recv_msgid_table[256];
extern FC_DEVICE_DRIVER fc_driver;

int main()
{
	sendmsgnum = 0;
	fc_send_msgid_table[0].ifmc = 0;
	fc_driver.FC_Open = NULL;

	UINT32 i = 0;
	CHAR* buff = NULL;
	DDS_ReturnCode_t returnCode;
	DDS_DataWriterQos stWriterQos;
	DDS_DomainParticipantQos stDomainQos;
	//DDS_PublisherQos stPublisherQos;
	DDS_DataReaderQos rqos;

	DDS_DomainParticipant* pstParticipant = NULL;
	DDS_Topic* pstTopic = NULL;
	//DDS_Publisher* pstPublisher = NULL;
	DDS_DataWriter* pstDataWriter[50] = { NULL };

	//DDS_Publisher* pstDelPublisher = NULL;

	/* 资源初始化 */
	DDS_ParticipantFactory_init();

	/* 获取参与者的qos */
	DDS_DomainParticipantFactory_get_defaul_participant_qos(&stDomainQos);

	/* 创建参与者 */
	pstParticipant = DDS_DomainParticipantFactory_create_participant(1, &stDomainQos);
	if (NULL == pstParticipant)
	{
		printf("ERROR! Failed to create participant!");
		return 1;
	}


	//char topic[2];
	//topic[0] = '0' + ;
	//topic[1] = 0;
	/* 创建主题 */
	pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, "myData", "myData", NULL);
	if (NULL == pstTopic)
	{
		printf("ERROR! Failed to create topic!");
		return 1;
	}

	/* 获取写入器qos */
	returnCode = DDS_Topic_get_default_datawriter_qos(pstTopic, &stWriterQos);
	if (DDS_RETCODE_ERROR == returnCode)
	{
		printf("ERROR! Data writer setup error !");
		return 1;
	}

	/* 设置写入器可靠性
	不可靠发送reliability.kind = 1，不可靠发送无法保证数据能被阅读器接收
	可靠发送reliability.kind = 2 可靠发送能够保证数据能被阅读器接收*/
	stWriterQos.reliability.kind = 2;

	//stWriterQos.history.depth = 20;

	/* 设置写入器持久化
	VOLATILE_DURABILITY_QOS模式
	发送给所有已知订阅者后丢弃数据不保存，订阅者无法收到在建立连接前发送的样本
	TRANSIENT_LOCAL_DURABILITY_QOS模式
	仅当数据写者存在时，与数据写者关联/连接的数据读者将被发送到数据写者历史记录中的所有样本
	*/
	//stWriterQos.durability.kind = TRANSIENT_DURABILITY_QOS;

	/* 设置历史缓存
	当为KEEP_LAST_HISTORY_QOS时，缓存的数据达到depth值时不会阻塞，会丢弃旧的数据，缓存新数据
	当为KEEP_ALL_HISTORY_QOS时，缓存的数据达到depth值时会阻塞，直至缓存被清空
	*/
	//stWriterQos.history.kind = KEEP_LAST_HISTORY_QOS;

	/* 配置depth值以及resource_limits的值
	当在KEEP_LAST_HISTORY_QOS模式时，depth的值不可大于resource_limits.max_samples值，实际缓存的深度依据depth大小
	当在KEEP_ALL_HISTORY_QOS模式时，实际缓存深度取决resource_limits.max_samples值
	*/
	//stWriterQos.history.depth = 10;
	//stWriterQos.resource_limits.max_samples = 10;
	//stWriterQos.resource_limits.max_samples_per_instance = 10;
	//stWriterQos.resource_limits.max_samples_per_instance = 10;

	/* 配置deadline的值，设置500ms内至少发送一次数据 */
	//stWriterQos.deadline.period.sec = 0;
	//stWriterQos.deadline.period.nanosec = 500;

	/* 创建写入器 */
	//pstDataWriter = DDS_Topic_create_datawriter(pstTopic, &stWriterQos, NULL);
	//for(int j = 0; j < i ; j++ )
	for (int i = 0; i < 30; i++)
	{
		pstDataWriter[i] = DDS_Topic_create_datawriter(pstTopic, &stWriterQos, NULL);
		if (NULL == pstDataWriter)
		{
			printf("ERROR! Failed to create dataWriter!");
			return 1;
		}
	}
	g_dataLength = 500 * 1024;
	//DDS_DomainParticipant_delete_publisher(pstParticipant,pstDelPublisher);
	/* 待发送数据内存分配 */
	buff = (CHAR*)malloc(g_dataLength);
	if (NULL == buff)
	{
		printf("ERROR! Insufficient memory space.\n");
		return 1;
	}
	memset(buff, 'g', g_dataLength);
	buff[g_dataLength - 1] = '\0';
	for (int i = 10000; i < 970000; i = i + 10000)
	{
		buff[i / 10000] = 'a';
	}
	buff[0] = 0;
	Sleep(2000);
	printf("start sending...\n");
	int SendCount = 0;
	while (1)
	{
		buff[0] += 1;
		for (i = 0; i < 30; i++)
		{
			//int b;
			//scanf("%d",&b);//itoa(i, buff, 10);
			//buff = (CHAR*)malloc(b);
			//memset(buff, 'a', b);
			//buff[b - 1] = '\0';
			returnCode = DDS_StringDataWriter_write(pstDataWriter[i], buff, g_dataLength);
			if (DDS_RETCODE_OK != returnCode)
			{
				printf("Failed to send %d message, retureCode = %d.\n", i, returnCode);
			}

			printf("send i = %d\n", SendCount++);

			Sleep(25);
		}
	}


	printf("end sending...\n");
	memset(buff, 0, g_dataLength);
	getchar();
	free(buff);

	/* 清除写入器 */
	DDS_Topic_delete_datawriter(pstTopic);

	/* 清除主题 */
	DDS_DomainParticipant_delete_topic(pstParticipant, pstTopic);

	/* 清除参与者 */
	DDS_DomainParticipantFactory_delete_participant(pstParticipant);
	getchar();
	return 0;
}
