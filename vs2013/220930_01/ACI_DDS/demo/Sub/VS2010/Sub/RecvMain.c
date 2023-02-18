#include "GlobalDefine.h"

/*
该示例的进程中仅具有阅读器，阅读器为可靠传输，当阅读器收到写入器发送的数据后会将接收的数据打印在终端。
阅读器均在KEEP_LAST_HISTORY_QOS的模式下，历史深度depth的值为10，资源限制的max_samples的值
也为10。设置持久化TRANSIENT_DURABILITY_QOS模式，先启动DDS_Send.exe,待DDS_Send.exe发送5包数据后，再启动DDS_Recv.exe，DDS_Recv.exe可以接收到DDS_Send.exe
之前发送的5包数据。
*/

/* 阅读器共接收到的总包数 */
static int g_numpkg = 0;

/* 阅读器绑定的回调函数 */
VOID STACALL ReceiveData(DDS_DataReader* pstDataReader)
{
	UINT32 dataNum = 0;
	DataSeq stDataSeq;
	DataLen dataLen;
	static unsigned int seqNum = 0;

	/* 收数据 */
	DDS_ReturnCode_t retCode = DDS_StringDataReader_take(pstDataReader, &stDataSeq);
	if (DDS_RETCODE_NO_DATA != retCode)
	{
		for (dataNum = 0; dataNum < stDataSeq.length; dataNum++)
		{
			char * userData = DDS_StringSeq_get(&stDataSeq, dataNum, &dataLen);
			g_numpkg++;
			printf("********** g_numpkg = %d, data = %d  len = %d \n", g_numpkg, userData[0], strlen(userData));
		}
		/* 释放缓存 */
		DDS_StringDataReader_return_loan(pstDataReader, &stDataSeq);
	}
}

int main()
{
	UINT32 i = 0;
	CHAR* buff = NULL;
	DDS_ReturnCode_t returnCode;
	DDS_DataReaderQos stReaderQos;
	DDS_DomainParticipantQos stDomainQos;

	DDS_DomainParticipant* pstParticipant = NULL;
	DDS_Topic* pstTopic = NULL;
	DDS_DataReader* pstDataReader = NULL;

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

	/* 创建主题 */
	pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, "1", "1", NULL);
	if (NULL == pstTopic)
	{ 
		printf("ERROR! Failed to create topic!");
		return 1;
	}

	/* 加入分区AA ,相同主题之间只有含有共同的分区才可以通信 */
	//AddPartition(pstTopic, "AA");

	/* 获取阅读器qos */
	returnCode = DDS_Topic_get_default_datareader_qos(pstTopic, &stReaderQos);
	if (DDS_RETCODE_ERROR == returnCode)
	{
		printf("ERROR! Data reader setup error !");
		return 1;
	}

	/* 设置写入器可靠性
	不可靠发送reliability.kind = 1，不可靠发送无法保证数据能被阅读器接收
	可靠发送reliability.kind = 2 可靠发送能够保证数据能被阅读器接收*/
	stReaderQos.reliability.kind = 2;

	/* 设置历史缓存
	当为KEEP_LAST_HISTORY_QOS时，缓存的数据达到depth值时不会阻塞，会丢弃旧的数据，缓存新数据
	当为KEEP_ALL_HISTORY_QOS时，缓存的数据达到depth值时会阻塞，直至缓存被清空
	*/
	//stReaderQos.history.kind = KEEP_LAST_HISTORY_QOS;

	/* 配置depth值以及resource_limits的值
	当在KEEP_LAST_HISTORY_QOS模式时，depth的值不可大于resource_limits.max_samples值，实际缓存的深度依据depth大小
	当在KEEP_ALL_HISTORY_QOS模式时，实际缓存深度取决resource_limits.max_samples值
	*/
	//stReaderQos.history.depth = 10;
	//stReaderQos.resource_limits.max_samples = 10;
	//stReaderQos.resource_limits.max_samples_per_instance = 10;
	//stReaderQos.resource_limits.max_samples_per_instance = 10;

	/* 配置deadline的值，设置500ms内至少发送一次数据 */
	//stReaderQos.deadline.period.sec = 0;
	//stReaderQos.deadline.period.nanosec = 500;

	/* 创建阅读器并绑定接收回调函数ReceiveData */
	pstDataReader = DDS_Topic_create_datareader(pstTopic, &stReaderQos, ReceiveData);
	if (NULL == pstDataReader)
	{
		printf("ERROR! Failed to create dataReader!");
		return 1;
	}
	DDS_Topic_create_datareader(pstTopic, &stReaderQos, ReceiveData);
	DDS_Topic_create_datareader(pstTopic, &stReaderQos, ReceiveData);
	DDS_Topic_create_datareader(pstTopic, &stReaderQos, ReceiveData);
	DDS_Topic_create_datareader(pstTopic, &stReaderQos, ReceiveData);
	DDS_Topic_create_datareader(pstTopic, &stReaderQos, ReceiveData);
	DDS_Topic_create_datareader(pstTopic, &stReaderQos, ReceiveData);
	DDS_Topic_create_datareader(pstTopic, &stReaderQos, ReceiveData);
	DDS_Topic_create_datareader(pstTopic, &stReaderQos, ReceiveData);
	DDS_Topic_create_datareader(pstTopic, &stReaderQos, ReceiveData);
	DDS_Topic_create_datareader(pstTopic, &stReaderQos, ReceiveData);

	getchar();
	printf("Actually receive %d pack data\n", g_numpkg);

	/* 清除阅读器 */
	DDS_Topic_delete_datareader(pstTopic);

	/* 清除主题 */
	DDS_DomainParticipant_delete_topic(pstParticipant, pstTopic);
	
	/* 清除参与者 */
	DDS_DomainParticipantFactory_delete_participant(pstParticipant);
	getchar();
	return 0;
}
