#include "GlobalDefine.h"

/* 
�������Ľ����н�����д������д����Ϊ�ɿ����䣬ͨ��д��������50���ֽڴ�СΪ32�����ݣ��������ն˽��̣��ڽ��ն�
��������ͬ������Ķ������Ķ����յ�д�������͵����ݺ�Ὣ���յ����ݴ�ӡ���նˡ�
����ͨ���޸�ȫ�ֱ���g_packNum��ֵ���÷��͵��ܰ�����ͨ��g_dataLength��ֵ�涨ÿ�����ݵ��ֽڴ�С��
д�������Ķ�������KEEP_LAST_HISTORY_QOS��ģʽ�£���ʷ���depth��ֵΪ10����Դ���Ƶ�max_samples��ֵ
ҲΪ10��д�������1s����һ�����ݣ��Ķ����ص�����ReceiveData(DDS_DataReader* pstDataReader)�л��ӡ
д�������͵����ݡ����ó־û�TRANSIENT_DURABILITY_QOSģʽ��������DDS_Send.exe,��DDS_Send.exe����5�����ݺ�������DDS_Recv.exe��DDS_Recv.exe���Խ��յ�DDS_Send.exe
֮ǰ���͵�5�����ݡ�
*/

/* д�������͵��ܰ��� */
static int g_packNum = 50000;

/* д�������͵İ����ֽڴ�С */
static int g_dataLength = 3 * 1024;

extern BLUE_PRINT gBluePrint[1];

int g_numpkg = 0;
/* �Ķ����󶨵Ļص����� */
VOID STACALL ReceiveData(DDS_DataReader* pstDataReader)
{
	UINT32 dataNum = 0;
	DataSeq stDataSeq;
	DataLen dataLen;
	static unsigned int seqNum = 0;

	/* ������ */
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
		/* �ͷŻ��� */
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
	/*��ѯ�豸*/
	ret = FC_NP_QueryDev(&count);
	printf("FC_NP_QueryDev ret=0x%08x, count=%d\n", ret, count);
	if (ret)
	{
		return 0;
	}
	/*���豸*/
	ret = FC_NP_Open(&h_dev, devId);
	printf("FC_NP_Open ret=0x%08x, devId=%d, dev=%08x\n", ret, devId, h_dev);
	if (ret)
	{
		return 0;
	}
	/*ѡ��ASMͨ��ģʽ*/
	ret = FC_NP_ConfigMode(h_dev, CONFIG_MODE_PROJECT);
	printf("FC_NP_ConfigMode ret=0x%08x\n", ret);
	if (ret)
	{
		return;
	}
	/*�����豸*/
	ret = FC_NP_LoadConfig(h_dev,&gBluePrint[0], majorScheme, minorScheme);
    printf("FC_NP_LoadConfig majorScheme=%d, minorScheme=%d ret=0x%08x\n", majorScheme, minorScheme, ret);
	if (ret)
	{
		printf("[devId=%d]FC_NP_LoadConfig failed ret=0x%08x\n", devId, ret);
		return;
	}
	/*��ȡ��������ɫ*/
	ret = FC_NM_RoleGet(h_dev, &t_nm_role);
	printf("FC_NM_RoleGet ret=0x%08x, t_nm_role = %d\n", ret, t_nm_role);
	if (ret)
	{
		return;
	}
	/*������������ʹ��*/
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
		/*�Ի����ԣ��ȴ��Լ�����*/
		fcid = 0x010002;
	}
	else
	{
		/*��Ե���ԣ��ȴ��Զ�����*/
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
		/*����Լ�������״̬*/
		state = FC_NODE_STATE_OFF_LINE;
		ret = FC_NM_GetOnNetState(h_dev, &state);
		printf("FC_NM_GetOnNetState ret=0x%08x, state = %d\n", ret, state);
		if (ret)
		{
			return 0;
		}
		if (FC_NODE_STATE_ON_LINE == state)
		{
			/*���ؽڵ��Ѿ�����*/
			break;
		}
		if (role != ROLE_NC)
		{
			/*���NC����״̬*/
			state = FC_NODE_STATE_OFF_LINE;
			ret = FC_NM_NRT_NcIsOnNet(h_dev, &state);
			printf("FC_NM_NRT_NcIsOnNet ret=0x%08x, state = %d\n", ret, state);
			if (ret)
			{
				return 0;
			}
			if (FC_NODE_STATE_OFF_LINE == state)
			{
				/*NC�ڵ㲻����*/	
				continue;
			}

			/*������������*/
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
	
	/* ʹ��DDS�շ����� */
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

		/* ��Դ��ʼ�� */
		DDS_ParticipantFactory_init();

		/* ��ȡ�����ߵ�qos */
		DDS_DomainParticipantFactory_get_defaul_participant_qos(&stDomainQos);

		/* ���������� */
		pstParticipant = DDS_DomainParticipantFactory_create_participant(1, &stDomainQos);
		if (NULL == pstParticipant)
		{
			printf("ERROR! Failed to create participant!");
			return 1;
		}

		/* �������� */
		pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, "IAGSM1_IAGSM2_CMD", "TSS_COMM_TYPE_NAME", NULL);
		//pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, "IAGSM2_DPU_LIFE_MSG", "TSS_COMM_TYPE_NAME", NULL);
		if (NULL == pstTopic)
		{
			printf("ERROR! Failed to create topic!");
			return 1;
		}
		
		/* �����Ķ������󶨽��ջص�����ReceiveData */
		pstDataReader = DDS_Topic_create_datareader(pstTopic, 0, ReceiveData);
		if (NULL == pstDataReader)
		{
			printf("ERROR! Failed to create dataReader!");
			return 1;
		}

		//Sleep(100000000);

		/* �������� */
		pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, "IAGSM2_DPU_LIFE_MSG", "TSS_COMM_TYPE_NAME", NULL);
		//pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, "IAGSM1_IAGSM2_CMD", "TSS_COMM_TYPE_NAME", NULL);
		if (NULL == pstTopic)
		{
			printf("ERROR! Failed to create topic!");
			return 1;
		}
		
		/* �����Ķ������󶨽��ջص�����ReceiveData */
		//pstDataReader = DDS_Topic_create_datareader(pstTopic, 0, ReceiveData);
		//if (NULL == pstDataReader)
		//{
		//	printf("ERROR! Failed to create dataReader!");
		//	return 1;
		//}

		/* ��ȡд����qos */
		returnCode = DDS_Topic_get_default_datawriter_qos(pstTopic, &stWriterQos);
		if (DDS_RETCODE_ERROR == returnCode)
		{
			printf("ERROR! Data writer setup error !");
			return 1;
		}

		/* ����д���� */
		pstDataWriter = DDS_Topic_create_datawriter(pstTopic, &stWriterQos, NULL);
		if (NULL == pstDataWriter)
		{
			printf("ERROR! Failed to create dataWriter!");
			return 1;
		}

		/* �����������ڴ���� */
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

	/* ֱ��ʹ��FC�շ����� */
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
				/*���˿���·״̬*/
				state = PORT_LINK_DOWN;
				ret = FC_NP_GetState(h_dev, &state);
				printf("FC_NP_GetState ret=0x%08x, state = %d\n", ret, state);
				if (ret)
				{
					break;
				}
				if (PORT_LINK_DOWN == state)
				{
					/*��·���򲻽���*/
					continue;
				}
				/*����Լ�������״̬*/
				state = FC_NODE_STATE_OFF_LINE;
				ret = FC_NM_GetOnNetState(h_dev, &state);
				printf("FC_NM_GetOnNetState ret=0x%08x, state = %d\n", ret, state);
				if (ret)
				{
					break;
				}
				if (FC_NODE_STATE_OFF_LINE == state)
				{
					/*���ؽڵ㲻�����򲻷���*/
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

	/* ��Դ��ʼ�� */
	DDS_ParticipantFactory_init();

	/* ��ȡ�����ߵ�qos */
	DDS_DomainParticipantFactory_get_defaul_participant_qos(&stDomainQos);

	/* ���������� */
	pstParticipant = DDS_DomainParticipantFactory_create_participant(1, &stDomainQos);
	if (NULL == pstParticipant)
	{
		printf("ERROR! Failed to create participant!");
		return 1;
	}

	/* �������� */
	pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, "1", "1", NULL);
	if (NULL == pstTopic)
	{
		printf("ERROR! Failed to create topic!");
		return 1;
	}

	/* �������AA ,��ͬ����֮��ֻ�к��й�ͬ�ķ����ſ���ͨ��*/
	//AddPartition(pstTopic, "AA");

	/* ��ȡд����qos */
	returnCode = DDS_Topic_get_default_datawriter_qos(pstTopic, &stWriterQos);
	if (DDS_RETCODE_ERROR == returnCode)
	{
		printf("ERROR! Data writer setup error !");
		return 1;
	}

	/* ����д�����ɿ���
	���ɿ�����reliability.kind = 1�����ɿ������޷���֤�����ܱ��Ķ�������
	�ɿ�����reliability.kind = 2 �ɿ������ܹ���֤�����ܱ��Ķ�������*/
	stWriterQos.reliability.kind = 2;

	/* ����д�����־û� 
	VOLATILE_DURABILITY_QOSģʽ
	���͸�������֪�����ߺ������ݲ����棬�������޷��յ��ڽ�������ǰ���͵�����
	TRANSIENT_LOCAL_DURABILITY_QOSģʽ
	��������д�ߴ���ʱ��������д�߹���/���ӵ����ݶ��߽������͵�����д����ʷ��¼�е���������
	*/
	//stWriterQos.durability.kind = TRANSIENT_DURABILITY_QOS;

	/* ������ʷ����
	��ΪKEEP_LAST_HISTORY_QOSʱ����������ݴﵽdepthֵʱ�����������ᶪ���ɵ����ݣ�����������
	��ΪKEEP_ALL_HISTORY_QOSʱ����������ݴﵽdepthֵʱ��������ֱ�����汻���
	*/
	//stWriterQos.history.kind = KEEP_LAST_HISTORY_QOS;

	/* ����depthֵ�Լ�resource_limits��ֵ
	����KEEP_LAST_HISTORY_QOSģʽʱ��depth��ֵ���ɴ���resource_limits.max_samplesֵ��ʵ�ʻ�����������depth��С
	����KEEP_ALL_HISTORY_QOSģʽʱ��ʵ�ʻ������ȡ��resource_limits.max_samplesֵ
	*/
	//stWriterQos.history.depth = 10;
	//stWriterQos.resource_limits.max_samples = 10;
	//stWriterQos.resource_limits.max_samples_per_instance = 10;
	//stWriterQos.resource_limits.max_samples_per_instance = 10;

	/* ����deadline��ֵ������500ms�����ٷ���һ������ */
	//stWriterQos.deadline.period.sec = 0;
	//stWriterQos.deadline.period.nanosec = 500;

	/* ����д���� */
	pstDataWriter = DDS_Topic_create_datawriter(pstTopic, &stWriterQos, NULL);
	if (NULL == pstDataWriter)
	{
		printf("ERROR! Failed to create dataWriter!");
		return 1;
	}

	/* �����������ڴ���� */
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

	/* ���д���� */
	DDS_Topic_delete_datawriter(pstTopic);

	/* ������� */
	DDS_DomainParticipant_delete_topic(pstParticipant, pstTopic);

	/* ��������� */
	DDS_DomainParticipantFactory_delete_participant(pstParticipant);
	getchar();
	return 0;
}
