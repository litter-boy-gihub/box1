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

int main()
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
