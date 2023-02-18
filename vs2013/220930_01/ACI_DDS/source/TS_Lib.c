#include "../include/GlobalDefine.h"
#ifdef _FC

//PART_PCT_TYPE my_pct[];
PART_PCT_TYPE pct[256];
unsigned int t_config_num;
DDS_DLL int sendmsgnum = 0;
int recvmsgnum = 0;

struct FCSendMsgIDType  fc_send_msgid_table[256];
struct FCRecvMsgIDType  fc_recv_msgid_table[256];

extern YCCommFCConfig g_commFCConfig;

FC_DEVICE_DRIVER fc_driver;
/*DevID of FC Card*/

void commInit(int * fcHandle)
{
	int ix;
	int iy;

	int iz;
	int im;
	int portNum = 0;

	int retCode;
	//int phy_id =0;
	int temp = 0;
	int temp1 = 0;
	int temp2 = 0;
	int temp3 = 0;
	int temp4 = 0;
	int temp5 = 0;
	int temp6 = 0;

#if defined(HS_FC)
	portNum = 0x10001;
#else
	FC_NP_GetLocalPortID(fcHandle, &portNum);
#endif

	//printf("commint fc_device is %x \n", fcHandle);
	//printf("portnum is %x \n",portNum);

	//GET_MID(&phy_id, &retCode);
	switch (portNum)
	{
	case 0x1000C:    //DPU1
		temp = 0;
		break;
	case 0x1000D:    //DPU2
		temp = 1;
		break;
	case 0x1000E:    //DPU3
		temp = 2;
		break;
	case 0X1000F:    //DPU4
		temp = 3;
		break;
	case 0X10010:    //DPU5
		temp = 4;
		break;
	case 0X10011:    //DPU6
		temp = 5;
		break;
	case 0X10000:    //DPU7
		temp = 6;
		break;
	case 0x10001:    //MIU1
		temp = 7;
		break;
	case 0X10004:    //ISP1
		temp = 7;
		break;
	case 0X10005:    //ISP2
		temp = 8;
		break;
	case 0X10028:
		temp = 9;      //VIU
		break;
	default:
		printf("port_id is error \n");
		while (1);
		break;

	}
	//printf("total cfg num =%d\n", (sizeof(fc_send_msgid_table_all[temp]) / sizeof((fc_send_msgid_table_all)[0][0])));
	for (ix = 0; ix < (sizeof(fc_send_msgid_table_all[temp]) / sizeof((fc_send_msgid_table_all)[0][0])); ix++)
	{
		if (fc_send_msgid_table_all[temp][ix].ifmc == 0x1)
		{
			g_commFCConfig.m_configs[temp1].m_localDDSSendMsgID = fc_send_msgid_table_all[temp][ix].msgid;
			temp1++;
		}
		else if (fc_send_msgid_table_all[temp][ix].ifmc == 0x2)
		{
			g_commFCConfig.m_configs[temp2].m_localSEPDSendMsgID = fc_send_msgid_table_all[temp][ix].msgid;
			temp2++;
		}
		else /*if(fc_send_msgid_table_all[temp][ix].ifmc ==0x0)*/
		{
			g_commFCConfig.m_configs[temp5].m_localUserSendMsgID = fc_send_msgid_table_all[temp][ix].msgid;
			temp5++;
		}

	}


	for (ix = 0; ix < (sizeof(fc_recv_msgid_table_all[temp]) / sizeof((fc_recv_msgid_table_all)[0][0])); ix++)
	{
		if (fc_recv_msgid_table_all[temp][ix].ifmc == 0x1)
		{
			g_commFCConfig.m_configs[temp3].m_localDDSRecvMsgID = fc_recv_msgid_table_all[temp][ix].msgid;
			g_commFCConfig.m_configs[temp3].m_FCPort = fc_recv_msgid_table_all[temp][ix].srcfcid;
			temp3++;
		}
		else if (fc_recv_msgid_table_all[temp][ix].ifmc == 0x2)
		{
			g_commFCConfig.m_configs[temp4].m_localSEPDRecvMsgID = fc_recv_msgid_table_all[temp][ix].msgid;
			temp4++;
		}
		else /*if(fc_recv_msgid_table_all[temp][ix].ifmc ==0x0)*/
		{
			g_commFCConfig.m_configs[temp6].m_localUserRecvMsgID = fc_recv_msgid_table_all[temp][ix].msgid;
			temp6++;
		}
	}


	//printf("phy_id = %x\n", phy_id);

	g_commFCConfig.m_nodeNum = temp4;
	g_commFCConfig.m_thisNodeFCPort = portNum;
/*
	printf("+++++++++++++++++++++++++++++++++++++++++++++++\n");
	for (ix = 0; ix < g_commFCConfig.m_nodeNum; ix++)
	{
		printf("seq id =%d\n", ix);
		printf("m_FCPort = %x\n", g_commFCConfig.m_configs[ix].m_FCPort);
		printf("m_localDDSSendMsgID = %x\n", g_commFCConfig.m_configs[ix].m_localDDSSendMsgID);
		printf("m_localDDSRecvMsgID = %x\n", g_commFCConfig.m_configs[ix].m_localDDSRecvMsgID);
		printf("m_localSEDPSendMsgID = %x\n", g_commFCConfig.m_configs[ix].m_localSEPDSendMsgID);
		printf("m_localSEDPRecvMsgID = %x\n", g_commFCConfig.m_configs[ix].m_localSEPDRecvMsgID);
		printf("m_localUserSendMsgID = %x\n", g_commFCConfig.m_configs[ix].m_localUserSendMsgID);
		printf("m_localUserRecvMsgID = %x\n", g_commFCConfig.m_configs[ix].m_localUserRecvMsgID);
	}
	printf("thisNodeFCPort = %x \n", g_commFCConfig.m_thisNodeFCPort);
*/
	/*
	g_commFCConfig.m_nodeNum =2;
	g_commFCConfig.m_thisNodeSeq =1;
	g_commFCConfig.m_configs[0].m_localUserSendMsgID =0X0C0E00;
	g_commFCConfig.m_configs[0].m_localUserRecvMsgID =0X0E0C00;
	g_commFCConfig.m_configs[0].m_localDDSRecvMsgID =0X0E0C01;
	g_commFCConfig.m_configs[0].m_localDDSSendMsgID=0X0C0E01;
	g_commFCConfig.m_configs[0].m_FCPort = 0x1000E;

	g_commFCConfig.m_configs[1].m_localUserSendMsgID =0X0C0F00;
	g_commFCConfig.m_configs[1].m_localUserRecvMsgID =0X0F0C00;
	g_commFCConfig.m_configs[1].m_localDDSRecvMsgID =0X0F0C01;
	g_commFCConfig.m_configs[1].m_localDDSSendMsgID=0X0C0F01;
	g_commFCConfig.m_configs[1].m_FCPort = 0x1000C;
	*/

	/*
	 * g_commFCConfig.m_configs[2].m_localUserSendMsgID =0X0C0C00;
	g_commFCConfig.m_configs[2].m_localUserRecvMsgID =0X0C0C00;
	g_commFCConfig.m_configs[2].m_localDDSRecvMsgID =0X0C0C01;
	g_commFCConfig.m_configs[2].m_localDDSSendMsgID=0X0C0C01;
	*/

	g_commFCConfig.m_FCdev = fcHandle;

}

static DDS_DomainParticipant* pstParticipant = NULL;

void Initialize ( CONFIGURATION_RESOURCE configuration,RETURN_CODE_TYPE *return_code)
{
	DDS_DomainParticipantQos stDomainQos;
	DDS_DomainParticipantFactory_get_defaul_participant_qos(&stDomainQos);
	pstParticipant = DDS_DomainParticipantFactory_create_participant(0, &stDomainQos);

	*return_code = NO_ERROR;
}


void Create_Connection (
	/* in  */ CONNECTION_NAME_TYPE 		connection_name,
	/* in  */ MESSAGING_PATTERN_TYPE 	pattern,
	/* out */ CONNECTION_ID_TYPE 		*connection_id,
	/* out */ CONNECTION_DIRECTION_TYPE *connection_direction,
	/* out */ MESSAGE_SIZE_TYPE 		*max_message_size,
	/* in */  TIMEOUT_TYPE              timeout,
	/* out */ RETURN_CODE_TYPE 			*return_code
)
{

	DDS_DataWriterQos stWriterQos;
	DDS_DataWriter* pstDataWriter = NULL;
	DDS_DataReaderQos stReaderQos;
	DDS_DataReader* pstDataReader = NULL;
	DDS_Topic* pstTopic = NULL;
	DDS_ReturnCode_t returnCode;

	pstTopic = DDS_DomainParticipant_create_topic(pstParticipant, connection_name, "TSS_COMM_TYPE_NAME", NULL);
	if (NULL == pstTopic)
	{
		printf("ERROR! Failed to create topic!");
		return -2;
	}

    /* SOURCE = 0, DESTINATION = 1 */
	if(connection_direction == 0)
	{

		returnCode = DDS_Topic_get_default_datawriter_qos(pstTopic, &stWriterQos);
		if (DDS_RETCODE_ERROR == returnCode)
		{
			printf("ERROR! Data writer setup error !");
			return -5;
		}

		stWriterQos.reliability.kind = BEST_EFFORT_RELIABILITY_QOS;
//		stWriterQos.reliability.max_blocking_time.nanosec = 0;
//		stWriterQos.reliability.max_blocking_time.sec = 0;
		stWriterQos.history.kind = KEEP_LAST_HISTORY_QOS;
		stWriterQos.history.depth = 1;

		pstDataWriter = DDS_Topic_create_datawriter(pstTopic, &stWriterQos, NULL);
		if (NULL == pstDataWriter)
		{
			printf("ERROR! Failed to create dataWriter!");
			return -5;
		}

		/* 64位时使用指针存在问题 */
		*connection_id = pstDataWriter;
	}

	if(connection_direction == 1)
	{
		returnCode = DDS_Topic_get_default_datareader_qos(pstTopic, &stReaderQos);

		if (DDS_RETCODE_ERROR == returnCode)
		{
			printf("ERROR! Data reader setup error !");
			return -7;
		}
		stReaderQos.reliability.kind = BEST_EFFORT_RELIABILITY_QOS;
		stReaderQos.history.depth = 1;
		stReaderQos.history.kind = KEEP_LAST_HISTORY_QOS;

		pstDataReader = DDS_Topic_create_datareader(pstTopic, &stReaderQos, NULL);
		if (NULL == pstDataReader)
		{
			printf("ERROR! Failed to create dataReader!");
			return -7;
		}

		/* 64位时使用指针存在问题 */
		*connection_id = pstDataReader;
	}

	*max_message_size = 0;

	*return_code = NO_ERROR;

}

void Receive_Message
(
	/* in    */	 CONNECTION_ID_TYPE 	connection_id,
	/* in    */	 TIMEOUT_TYPE 			timeout,
	/* inout */  TRANSACTION_ID_TYPE 	*transaction_id,
	/* out   */  void 					*message,
	/* out   */	 MESSAGE_SIZE_TYPE 		*message_size,
	/* out   */  RETURN_CODE_TYPE 		*return_code
)
{

	DDS_DataReader* pstDataReader = connection_id;
	DataSeq stDataSeq;
	DataLen dataLen;
	int dataNum ;
	DDS_ReturnCode_t retCode = DDS_StringDataReader_take(pstDataReader, &stDataSeq);
	if (DDS_RETCODE_NO_DATA != retCode)
	{
		for (dataNum = 0; dataNum < stDataSeq.length; dataNum++)
		{
			char * userData = DDS_StringSeq_get(&stDataSeq, dataNum, &dataLen);

			memcpy(message,userData,dataNum);

			//pstDataReader->recvData(userData, dataLen, pstDataReader->pstTopic->pstParticipant->dcpsParticipantData.domainID, pstDataReader->pstTopic->topicName.value);

			*return_code = NO_ERROR;
			return ;
		}
		/* 释放缓存 */
		DDS_StringDataReader_return_loan(pstDataReader, &stDataSeq);
	}

	*return_code = NOT_AVAILABLE;
}

void Send_Message
(
	/* in    */	 CONNECTION_ID_TYPE 	connection_id,
	/* in    */	 TIMEOUT_TYPE 			timeout,
	/* inout */  TRANSACTION_ID_TYPE 	*transaction_id,
	/* inout */  void                	*message,
	/* in    */  MESSAGE_SIZE_TYPE 		message_size,
	/* out   */	 RETURN_CODE_TYPE 		*return_code
)
{
	DDS_DataWriter* pstDataWriter = connection_id;

	DDS_ReturnCode_t returncode = DDS_StringDataWriter_write(pstDataWriter, message, message_size);
	if(0 != returncode)
	{
		return 1;
	}

	*return_code = NO_ERROR;

}

void Get_Connection_Id(
	/* in  */ CONNECTION_NAME_TYPE connection_name,
	/* out */ CONNECTION_ID_TYPE   *connection_id,
	/* out */ RETURN_CODE_TYPE 	   *retCode
)
{
	DDS_Topic *pstTopic;
	//printf("Getting_Connection_Id, pstParticipant=%x, connection_name addr=%x, connection_name = %s\n", pstParticipant, connection_name, connection_name);
	if (!pstParticipant)
	{
		printf("DDS Participant Not Inited!\n");
		*retCode = NOT_AVAILABLE;
	}
	pstTopic = DDS_DomainParticipant_get_topic_by_name(pstParticipant, connection_name, "TSS_COMM_TYPE_NAME");
	if (!pstTopic)
	{ 
		printf("DDS Topic Not Found, name：%s!\n", connection_name);
		*retCode = NOT_AVAILABLE;
	}
	*connection_id = pstTopic;
	*retCode = NO_ERROR;
}

void Register_Callback(
		/* in    */	 CONNECTION_ID_TYPE 	connection_id,
		/* in    */	 TIMEOUT_TYPE 			timeout,
		/* in    */  Read_Callback			Cback,
		/* in    */  MESSAGE_SIZE_TYPE 		message_size,
		/* out   */	 RETURN_CODE_TYPE 		*return_code
)
{
	DDS_DataReader* pstTempDataReader = (DDS_DataReader*)connection_id;
	pstTempDataReader->recvData = (DDS_RECV_CALLBACK_FUNC)Cback;
}

char * codeToStr(RETURN_CODE_TYPE retCode)
{
	switch (retCode)
	{
	case NO_ERROR:
		return "NO_ERROR";
		break;
	case NO_ACTION:
		return "NO_ACTION";
		break;
	case NOT_AVAILABLE:
		return "NOT_AVAILABLE";
		break;
	case INVALID_PARAM:
		return "INVALID_PARAM";
		break;
	case INVALID_CONFIG:
		return "INVALID_CONFIG";
		break;
	case INVALID_MODE:
		return "INVALID_MODE";
		break;
	case TIMED_OUT:
		return "TIMED_OUT";
		break;
	}
	/* should not go here */
	return "Unknown code";
}
#endif
