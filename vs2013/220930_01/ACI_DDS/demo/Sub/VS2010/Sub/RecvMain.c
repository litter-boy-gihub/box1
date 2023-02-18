#include "GlobalDefine.h"

/*
��ʾ���Ľ����н������Ķ������Ķ���Ϊ�ɿ����䣬���Ķ����յ�д�������͵����ݺ�Ὣ���յ����ݴ�ӡ���նˡ�
�Ķ�������KEEP_LAST_HISTORY_QOS��ģʽ�£���ʷ���depth��ֵΪ10����Դ���Ƶ�max_samples��ֵ
ҲΪ10�����ó־û�TRANSIENT_DURABILITY_QOSģʽ��������DDS_Send.exe,��DDS_Send.exe����5�����ݺ�������DDS_Recv.exe��DDS_Recv.exe���Խ��յ�DDS_Send.exe
֮ǰ���͵�5�����ݡ�
*/

/* �Ķ��������յ����ܰ��� */
static int g_numpkg = 0;

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
			printf("********** g_numpkg = %d, data = %d  len = %d \n", g_numpkg, userData[0], strlen(userData));
		}
		/* �ͷŻ��� */
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

	/* �������AA ,��ͬ����֮��ֻ�к��й�ͬ�ķ����ſ���ͨ�� */
	//AddPartition(pstTopic, "AA");

	/* ��ȡ�Ķ���qos */
	returnCode = DDS_Topic_get_default_datareader_qos(pstTopic, &stReaderQos);
	if (DDS_RETCODE_ERROR == returnCode)
	{
		printf("ERROR! Data reader setup error !");
		return 1;
	}

	/* ����д�����ɿ���
	���ɿ�����reliability.kind = 1�����ɿ������޷���֤�����ܱ��Ķ�������
	�ɿ�����reliability.kind = 2 �ɿ������ܹ���֤�����ܱ��Ķ�������*/
	stReaderQos.reliability.kind = 2;

	/* ������ʷ����
	��ΪKEEP_LAST_HISTORY_QOSʱ����������ݴﵽdepthֵʱ�����������ᶪ���ɵ����ݣ�����������
	��ΪKEEP_ALL_HISTORY_QOSʱ����������ݴﵽdepthֵʱ��������ֱ�����汻���
	*/
	//stReaderQos.history.kind = KEEP_LAST_HISTORY_QOS;

	/* ����depthֵ�Լ�resource_limits��ֵ
	����KEEP_LAST_HISTORY_QOSģʽʱ��depth��ֵ���ɴ���resource_limits.max_samplesֵ��ʵ�ʻ�����������depth��С
	����KEEP_ALL_HISTORY_QOSģʽʱ��ʵ�ʻ������ȡ��resource_limits.max_samplesֵ
	*/
	//stReaderQos.history.depth = 10;
	//stReaderQos.resource_limits.max_samples = 10;
	//stReaderQos.resource_limits.max_samples_per_instance = 10;
	//stReaderQos.resource_limits.max_samples_per_instance = 10;

	/* ����deadline��ֵ������500ms�����ٷ���һ������ */
	//stReaderQos.deadline.period.sec = 0;
	//stReaderQos.deadline.period.nanosec = 500;

	/* �����Ķ������󶨽��ջص�����ReceiveData */
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

	/* ����Ķ��� */
	DDS_Topic_delete_datareader(pstTopic);

	/* ������� */
	DDS_DomainParticipant_delete_topic(pstParticipant, pstTopic);
	
	/* ��������� */
	DDS_DomainParticipantFactory_delete_participant(pstParticipant);
	getchar();
	return 0;
}
