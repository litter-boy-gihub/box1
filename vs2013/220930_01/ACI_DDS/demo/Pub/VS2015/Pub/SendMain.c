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
static int g_packNum = 50;

/* д�������͵İ����ֽڴ�С */
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


	//char topic[2];
	//topic[0] = '0' + ;
	//topic[1] = 0;
	/* �������� */
	pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, "myData", "myData", NULL);
	if (NULL == pstTopic)
	{
		printf("ERROR! Failed to create topic!");
		return 1;
	}

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

	//stWriterQos.history.depth = 20;

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
	/* �����������ڴ���� */
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

	/* ���д���� */
	DDS_Topic_delete_datawriter(pstTopic);

	/* ������� */
	DDS_DomainParticipant_delete_topic(pstParticipant, pstTopic);

	/* ��������� */
	DDS_DomainParticipantFactory_delete_participant(pstParticipant);
	getchar();
	return 0;
}
