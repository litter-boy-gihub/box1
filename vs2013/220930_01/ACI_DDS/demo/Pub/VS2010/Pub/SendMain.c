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
static int g_packNum = 50000;

/* 写入器发送的包的字节大小 */
static int g_dataLength = 3 * 1024;

extern BLUE_PRINT gBluePrint[1];

int g_numpkg = 0;
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
			PrintBinData(userData, dataLen, pstDataReader->pstTopic->topicName.value);
			//printf("********** g_numpkg = %d, data = %d  len = %d \n", g_numpkg, userData[0], dataLen);
		}
		/* 释放缓存 */
		DDS_StringDataReader_return_loan(pstDataReader, &stDataSeq);
	}
}

int main()
{
	char tt;
	FCUINT ret, count = 0, workState, t_nm_role, fcid, state;
	FCHANDLE h_dev;
	unsigned int devId = 0;
	int majorScheme = 0;
	int minorScheme = 0;
	//int role = ROLE_NC;
	int role = ROLE_NRT;
	unsigned int localPortID = 0;
	FILE * f = fopen("e:/qy/1.txt", "r");
	int mode = 0;
	if (f)
	{
		fscanf(f, "%d", &mode);
		fclose(f);
	}
	/*查询设备*/
	ret = FC_NP_QueryDev(&count);
	printf("FC_NP_QueryDev ret=0x%08x, count=%d\n", ret, count);
	if (ret)
	{
		return 0;
	}
	/*打开设备*/
	ret = FC_NP_Open(&h_dev, devId);
	printf("FC_NP_Open ret=0x%08x, devId=%d, dev=%08x\n", ret, devId, h_dev);
	if (ret)
	{
		return 0;
	}
	/*选择ASM通信模式*/
	ret = FC_NP_ConfigMode(h_dev, CONFIG_MODE_PROJECT);
	printf("FC_NP_ConfigMode ret=0x%08x\n", ret);
	if (ret)
	{
		return;
	}
	/*配置设备*/
	ret = FC_NP_LoadConfig(h_dev,&gBluePrint[0], majorScheme, minorScheme);
    printf("FC_NP_LoadConfig majorScheme=%d, minorScheme=%d ret=0x%08x\n", majorScheme, minorScheme, ret);
	if (ret)
	{
		printf("[devId=%d]FC_NP_LoadConfig failed ret=0x%08x\n", devId, ret);
		return;
	}
	/*获取网络管理角色*/
	ret = FC_NM_RoleGet(h_dev, &t_nm_role);
	printf("FC_NM_RoleGet ret=0x%08x, t_nm_role = %d\n", ret, t_nm_role);
	if (ret)
	{
		return;
	}
	/*设置心跳发送使能*/
	if (role == ROLE_NC)
	{
		ret = FC_NM_WDTEnable(h_dev, WDT_NET_MODE);
		printf("FC_NM_WDTEnable ret=0x%08x\n", ret);
	}
	//ret = FC_NP_GetLocalPortID(h_dev, &localPortID);
	//printf("FC_NP_GetLocalPortID ret=0x%08x, localPortID = %d\n", ret, localPortID);
	//if (ret)
	//{
	//	return;
	//}
	if (1 == 1) //loopback
	{
		/*自环测试，等待自己上网*/
		fcid = 0x010002;
	}
	else
	{
		/*点对点测试，等待对端上网*/
		if (role == ROLE_NC)
		{
			fcid = 0x010002;
		}
		else 
		{
			fcid = 0x010001;
		}
	}
	printf("fcid=0x%08x\n", fcid);
	if (role != ROLE_NC)
	{
		ret = FC_NM_NRT_OnNetReq(h_dev);
		printf("FC_NM_NRT_OnNetReq ret=0x%08x", ret);
		if (ret)
		{
			return 0;
		}
	}
	while (1)
	{
		state = PORT_LINK_DOWN;
		ret = FC_NP_GetState(h_dev, &state);
		printf("FC_NP_GetState ret=0x%08x, state = %d\n", ret, state);
		if (ret)
		{
			return 0;
		}
		if (PORT_LINK_DOWN == state)
		{
			continue;
		}
		/*检查自己的在线状态*/
		state = FC_NODE_STATE_OFF_LINE;
		ret = FC_NM_GetOnNetState(h_dev, &state);
		printf("FC_NM_GetOnNetState ret=0x%08x, state = %d\n", ret, state);
		if (ret)
		{
			return 0;
		}
		if (FC_NODE_STATE_ON_LINE == state)
		{
			/*本地节点已经在网*/
			break;
		}
		if (role != ROLE_NC)
		{
			/*检查NC在线状态*/
			state = FC_NODE_STATE_OFF_LINE;
			ret = FC_NM_NRT_NcIsOnNet(h_dev, &state);
			printf("FC_NM_NRT_NcIsOnNet ret=0x%08x, state = %d\n", ret, state);
			if (ret)
			{
				return 0;
			}
			if (FC_NODE_STATE_OFF_LINE == state)
			{
				/*NC节点不在网*/	
				continue;
			}

			/*发起上网请求*/
			ret = FC_NM_NRT_OnNetReq(h_dev);
			printf("FC_NM_NRT_OnNetReq ret=0x%08x", ret);
			if (ret)
			{
				return 0;
			}
		}
		else
		{
			break;
		}
	}

	commInit(h_dev);
	
	/* 使用DDS收发数据 */
	{
		UINT32 i = 0;
		CHAR* buff = NULL;
		DDS_ReturnCode_t returnCode;
		DDS_DataWriterQos stWriterQos;
		DDS_DomainParticipantQos stDomainQos;

		DDS_DomainParticipant* pstParticipant = NULL;
		DDS_Topic* pstTopic = NULL;
		DDS_DataWriter* pstDataWriter = NULL;
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
		pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, "IAGSM1_IAGSM2_CMD", "TSS_COMM_TYPE_NAME", NULL);
		//pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, "IAGSM2_DPU_LIFE_MSG", "TSS_COMM_TYPE_NAME", NULL);
		if (NULL == pstTopic)
		{
			printf("ERROR! Failed to create topic!");
			return 1;
		}
		
		/* 创建阅读器并绑定接收回调函数ReceiveData */
		pstDataReader = DDS_Topic_create_datareader(pstTopic, 0, ReceiveData);
		if (NULL == pstDataReader)
		{
			printf("ERROR! Failed to create dataReader!");
			return 1;
		}

		//Sleep(100000000);

		/* 创建主题 */
		pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, "IAGSM2_DPU_LIFE_MSG", "TSS_COMM_TYPE_NAME", NULL);
		//pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, "IAGSM1_IAGSM2_CMD", "TSS_COMM_TYPE_NAME", NULL);
		if (NULL == pstTopic)
		{
			printf("ERROR! Failed to create topic!");
			return 1;
		}
		
		/* 创建阅读器并绑定接收回调函数ReceiveData */
		//pstDataReader = DDS_Topic_create_datareader(pstTopic, 0, ReceiveData);
		//if (NULL == pstDataReader)
		//{
		//	printf("ERROR! Failed to create dataReader!");
		//	return 1;
		//}

		/* 获取写入器qos */
		returnCode = DDS_Topic_get_default_datawriter_qos(pstTopic, &stWriterQos);
		if (DDS_RETCODE_ERROR == returnCode)
		{
			printf("ERROR! Data writer setup error !");
			return 1;
		}

		/* 创建写入器 */
		pstDataWriter = DDS_Topic_create_datawriter(pstTopic, &stWriterQos, NULL);
		if (NULL == pstDataWriter)
		{
			printf("ERROR! Failed to create dataWriter!");
			return 1;
		}

		/* 待发送数据内存分配 */
		buff = (CHAR*)malloc(g_dataLength);
		if (NULL == buff)
		{
			printf("ERROR! Insufficient memory space.\n");
			return 1;
		}
		memset(buff, 0, g_dataLength);
		buff[g_dataLength - 1] = '\0';

		Sleep(1000);

		printf("start sending...\n");
		{
			/* IAGSM1_IAGSM2_CMD */
			//g_dataLength = 4;
			//buff[0] = 0x80;
			//buff[1] = 0x00;
			//buff[2] = 0x00;
			//buff[3] = 0x00;
		}
		{
			/* IAGSM2_DPU_LIFE_MSG */
			char tempBuffer[52] = {
				  0x42, 0x21, 0x20, 00, 0x80, 00, 00, 00, 00, 03, 00, 02, 00, 02, 00, 01, 00, 06, 00, 01, 00, 0x08, 00, 01, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
			};
			memcpy(buff, tempBuffer, 52);
			g_dataLength = 52;
		}
		for (i = 1; ; i++)
		{
			returnCode = DDS_StringDataWriter_write(pstDataWriter, buff, g_dataLength);
			if (DDS_RETCODE_OK != returnCode)
			{
				printf("Failed to send %d message, retureCode = %d.\n", i, returnCode);
			}

			printf("send i = %d\n", i);
			memset(buff, 0, g_dataLength);
			Sleep(1000);
		}
	}

	/* 直接使用FC收发数据 */
#if 0
	{
		int idx, nLen = 1024 * 1024;
		int msgId;
		unsigned char * msg = (unsigned char *)malloc(nLen);
		int idIdx;
		for (idx = 0; ; ++idx)
		{
			for (idIdx = 0; idIdx < 6; ++idIdx)
			{
				/*检查端口链路状态*/
				state = PORT_LINK_DOWN;
				ret = FC_NP_GetState(h_dev, &state);
				printf("FC_NP_GetState ret=0x%08x, state = %d\n", ret, state);
				if (ret)
				{
					break;
				}
				if (PORT_LINK_DOWN == state)
				{
					/*链路断则不接收*/
					continue;
				}
				/*检查自己的在线状态*/
				state = FC_NODE_STATE_OFF_LINE;
				ret = FC_NM_GetOnNetState(h_dev, &state);
				printf("FC_NM_GetOnNetState ret=0x%08x, state = %d\n", ret, state);
				if (ret)
				{
					break;
				}
				if (FC_NODE_STATE_OFF_LINE == state)
				{
					/*本地节点不在网则不发送*/
					continue;
				}
				msgId = gBluePrint[majorScheme].config_block[minorScheme].send_item[idIdx].msg_id;
				nLen =1024;//\\ cd->msg_info[ch_idx].length;
				if (mode & 1)
				{
					ret = FC_NP_SendMsg(h_dev, msgId, msg, nLen);
					printf("FC_NP_SendMsg msgId=%08X nLen=%lld, ret=0x%08x\n", msgId, nLen, ret);
					if (ret)
					{
						continue;
					}
				}
				Sleep(1000);
				if (mode & 2)
				{
					ret = FC_NP_RecvMsg(h_dev, msgId, msg, (unsigned int *)&nLen);
					printf("FC_NP_RecvMsg msgId=%08X nLen=%lld, ret=0x%08x\n", msgId, nLen, ret);
					if (ret)
					{
						break;
					}
				}
				Sleep(1000);
			}
		}
	}
#endif
	getchar();
	return 0;
}

int main1()
{
	UINT32 i = 0;
	CHAR* buff = NULL;
	DDS_ReturnCode_t returnCode;
	DDS_DataWriterQos stWriterQos;
	DDS_DomainParticipantQos stDomainQos;

	DDS_DomainParticipant* pstParticipant = NULL;
	DDS_Topic* pstTopic = NULL;
	DDS_DataWriter* pstDataWriter = NULL;

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

	/* 加入分区AA ,相同主题之间只有含有共同的分区才可以通信*/
	//AddPartition(pstTopic, "AA");

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
	pstDataWriter = DDS_Topic_create_datawriter(pstTopic, &stWriterQos, NULL);
	if (NULL == pstDataWriter)
	{
		printf("ERROR! Failed to create dataWriter!");
		return 1;
	}

	/* 待发送数据内存分配 */
	buff = (CHAR*)malloc(g_dataLength);
	if (NULL == buff)
	{
		printf("ERROR! Insufficient memory space.\n");
		return 1;
	}
	memset(buff, 0, g_dataLength);
	buff[g_dataLength - 1] = '\0';

	Sleep(3000);

	printf("start sending...\n");
	for (i = 1; i <= 100; i++)
	{
		memset(buff, 1 + i, g_dataLength);
		buff[g_dataLength - 1] = 0;
		returnCode = DDS_StringDataWriter_write(pstDataWriter, buff, g_dataLength);
		if (DDS_RETCODE_OK != returnCode)
		{
			printf("Failed to send %d message, retureCode = %d.\n", i, returnCode);
		}

		printf("send i = %d\n", i);
		memset(buff, 0, g_dataLength);
		//Sleep(100);
	}
	printf("end sending...\n");

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
