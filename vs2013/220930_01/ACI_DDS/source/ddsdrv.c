#include "../include/GlobalDefine.h"
#ifdef _FC
/* JD 2021-12-03 判断ParticipantFactory是否进行初始化 TRUE - 没有进行初始化 */
BOOL globle_bDDS_ParticipantFactory_init = TRUE;

int iOwnerShipStrength = 0;

typedef struct _DRV_DataWriter{
	DDS_DataWriter* pDataWriter;
	
}DRV_DataWriter;

//typedef struct _DRV_DataReader {
//	DDS_DataReader* pDataReader;
//	DRV_DataReader* pNext;
//	DDS_RECV_CALLBACK_FUNC RecvData;
//}DRV_DataReader;

typedef struct _DRV_DomainParticipant{
	struct _DRV_DomainParticipant* pHead;		/* 链表头 */
	struct _DRV_DomainParticipant* pPrevious;	/* 上一个*/
	struct _DRV_DomainParticipant* pNext;		/* 下一个*/

	DDS_DomainParticipant* pstParticipant;

}DRV_DomainParticipant;





typedef struct _DRV_DomainParticipant_List
{
	DRV_DomainParticipant* pHead;		/* 链表头 */
	DRV_DomainParticipant* pTail;		/* 尾一个*/

	UINT32  uiParticipantCount;

}DRV_DomainParticipant_list;


int iQosOptions = 0;
static DRV_DomainParticipant_list* pstglobleDDS_DomainParticipantlist = NULL;

/* 返回值  :  1   - 成功；
		  -1   - 主题所属的域参与者初始化不成功；
		  -2   - 初始化主题信息失败；
		  -3   - 初始化主题数据结构失败；
		  -4   - 创建主题实体失败；
		  -5   - 创建数据写者失败；
		  -6   - 创建数据监听器失败；
		  -7   - 创建数据读者失败；
		  -8   - 设置数据监听器失败；
		  -9   - 主题非法；
		  */
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
			
//			printf("Nephalem Get Data len = %d \n",dataLen);
			
			{
//			PrintBinData(userData,dataLen, pstDataReader->pstTopic->topicName.value);
			}

			if (pstDataReader->recvData)
			{
				pstDataReader->recvData(userData, dataLen, pstDataReader->pstTopic->pstParticipant->dcpsParticipantData.domainID, pstDataReader->pstTopic->topicName.value);
			}
		}
		/* 释放缓存 */
		DDS_StringDataReader_return_loan(pstDataReader, &stDataSeq);
	}
}

int insert_Domainparticipant(DDS_DomainParticipant* pstParticipant)
{
	DRV_DomainParticipant* pTempDrvDomainParticipant= malloc(sizeof(DRV_DomainParticipant));
	if (NULL == pTempDrvDomainParticipant)
	{
		printf("malloc DRV_DomainParticipanterror\n");
		return 1;
	}
	pTempDrvDomainParticipant->pNext = NULL;
	pTempDrvDomainParticipant->pstParticipant = pstParticipant;
	pTempDrvDomainParticipant->pPrevious = pstglobleDDS_DomainParticipantlist->pTail;

	if (NULL == pstglobleDDS_DomainParticipantlist->pHead)
	{
		pTempDrvDomainParticipant->pHead = pTempDrvDomainParticipant;

		pstglobleDDS_DomainParticipantlist->pHead = pTempDrvDomainParticipant;
		pstglobleDDS_DomainParticipantlist->pTail = pTempDrvDomainParticipant;
		pstglobleDDS_DomainParticipantlist->uiParticipantCount += 1;

		return 0;
	}

	pTempDrvDomainParticipant->pHead = pstglobleDDS_DomainParticipantlist->pHead;
	pstglobleDDS_DomainParticipantlist->pTail->pNext = pTempDrvDomainParticipant;

	pstglobleDDS_DomainParticipantlist->uiParticipantCount += 1;
	pstglobleDDS_DomainParticipantlist->pTail = pTempDrvDomainParticipant;

	//DRV_DomainParticipant* pstDmainParticipant = pstglobleDDS_DomainParticipantlist->pHead;
	//while (NULL != pstglobleDDS_DomainParticipantlist->pTail)
		
	return 0;
}

/* JD 未完成 */
int remove_DomainParticipant(int domain_id)
{
	DRV_DomainParticipant* pTempDrvDomainParticipant= pstglobleDDS_DomainParticipantlist->pHead;
	while (NULL != pTempDrvDomainParticipant)
	{
		if (pTempDrvDomainParticipant->pstParticipant->dcpsParticipantData.domainID == domain_id)
		{
			DDS_DomainParticipantFactory_delete_participant(pTempDrvDomainParticipant->pstParticipant);
			pTempDrvDomainParticipant->pstParticipant = NULL;
			/* 如果为首节点*/
			if(pTempDrvDomainParticipant == pstglobleDDS_DomainParticipantlist->pHead)
			{
				if (NULL == pTempDrvDomainParticipant->pNext)
				{
					pstglobleDDS_DomainParticipantlist->pHead = NULL;
					pstglobleDDS_DomainParticipantlist->pTail = NULL;
					pstglobleDDS_DomainParticipantlist->uiParticipantCount = 0;

					free(pTempDrvDomainParticipant);
					pTempDrvDomainParticipant = NULL;
					break;
				}
				pstglobleDDS_DomainParticipantlist->pHead = pstglobleDDS_DomainParticipantlist->pHead->pNext;
				pstglobleDDS_DomainParticipantlist->pHead->pPrevious = NULL;
				free(pTempDrvDomainParticipant);
				pTempDrvDomainParticipant = NULL;

				break;

			}
			/* 如果是尾节点 */
			if (pTempDrvDomainParticipant == pstglobleDDS_DomainParticipantlist->pTail)
			{

				pstglobleDDS_DomainParticipantlist->pTail = pstglobleDDS_DomainParticipantlist->pTail->pPrevious;
				pstglobleDDS_DomainParticipantlist->pTail->pNext = NULL;
				free(pTempDrvDomainParticipant);
				pTempDrvDomainParticipant = NULL;
				break;
				
			}
			/* 中间节点删除 */
			pTempDrvDomainParticipant->pPrevious->pNext = pTempDrvDomainParticipant->pNext;
			pTempDrvDomainParticipant->pNext->pPrevious = pTempDrvDomainParticipant->pPrevious;

			free(pTempDrvDomainParticipant);
			pTempDrvDomainParticipant = NULL;
			
			return 0;
		}
		pTempDrvDomainParticipant = pTempDrvDomainParticipant->pNext;
	}
	return 1;

}

DRV_DomainParticipant* find_DomainParticipant(int domain_id)
{
	DRV_DomainParticipant* pTempDomainParticipant = pstglobleDDS_DomainParticipantlist->pHead;
	while (NULL != pTempDomainParticipant)
	{
		if (pTempDomainParticipant->pstParticipant->dcpsParticipantData.domainID == domain_id)
			return pTempDomainParticipant;
	}
	return NULL;

}



DDS_DLL int ddsdrv_init_domain(int domain_id)
{
	/*printf("%s-%d:\n",__FUNCTION__, __LINE__);*/
	DDS_DomainParticipant* pstParticipant = NULL;
	DDS_DomainParticipantQos stDomainQos;
	DRV_DomainParticipant* pstDrvParticipant = NULL;

	if (globle_bDDS_ParticipantFactory_init)
	{
		DDS_ParticipantFactory_init();
		globle_bDDS_ParticipantFactory_init = FALSE;

		pstglobleDDS_DomainParticipantlist = malloc(sizeof(DRV_DomainParticipant_list));
		pstglobleDDS_DomainParticipantlist->pHead = NULL;
		pstglobleDDS_DomainParticipantlist->pTail = NULL;
		pstglobleDDS_DomainParticipantlist->uiParticipantCount = 0;
	}
	if(NULL != pstglobleDDS_DomainParticipantlist)
		pstDrvParticipant = pstglobleDDS_DomainParticipantlist->pHead;
	while (NULL != pstDrvParticipant)
	{
		if (pstDrvParticipant->pstParticipant->dcpsParticipantData.domainID == domain_id)
			return 1;
		pstDrvParticipant = pstDrvParticipant->pNext;
	}


	/* 获取参与者的qos */
	DDS_DomainParticipantFactory_get_defaul_participant_qos(&stDomainQos);
	pstParticipant = DDS_DomainParticipantFactory_create_participant(domain_id, &stDomainQos);
	if (NULL == pstParticipant)
	{
		printf("ERROR! Failed to create participant! domain_id =%d\n,domain_id");
		return -1;
	}

	insert_Domainparticipant(pstParticipant);

	return 1;
}

DDS_DLL int ddsdrv_destory_domain(int domain_id)
{
	remove_DomainParticipant(domain_id);
	return 1;
}

DDS_DLL void ddsdrv_destory_domains()
{
	DRV_DomainParticipant* pstDrvDomainParticipant = pstglobleDDS_DomainParticipantlist->pHead;
	while (NULL != pstDrvDomainParticipant)
	{
		remove_DomainParticipant(pstDrvDomainParticipant->pstParticipant->dcpsParticipantData.domainID);
		pstDrvDomainParticipant = pstglobleDDS_DomainParticipantlist->pHead;
	}

	free(pstglobleDDS_DomainParticipantlist);
	pstglobleDDS_DomainParticipantlist = NULL;
	globle_bDDS_ParticipantFactory_init = FALSE;
	return;
}

DDS_DLL int ddsdrv_init_topic(int domain_id, const char* topic_name, long max_length, DDS_INIT_MASK init_mask)
{
	DDS_DataWriterQos stWriterQos;
	DDS_DataWriter* pstDataWriter = NULL;
	DDS_DataReaderQos stReaderQos;
	DDS_DataReader* pstDataReader = NULL;
	DDS_Topic* pstTopic = NULL;
	DDS_ReturnCode_t returnCode;

	BOOL bWriter = FALSE;
	BOOL bReader = FALSE;

	DRV_DomainParticipant* pstDrvParticipant = NULL;

	pstDrvParticipant = find_DomainParticipant(domain_id);
	if (NULL == pstDrvParticipant)
		return -2;

	pstTopic = DDS_DomainParticipant_create_topic(pstDrvParticipant->pstParticipant, topic_name, "TSS_COMM_TYPE_NAME", NULL);
	if (NULL == pstTopic)
	{
		printf("ERROR! Failed to create topic!");
		return -2;
	}

	if (DDS_INIT_TOPIC == init_mask)
		return 1;

	if (DDS_INIT_SEND == init_mask)
		bWriter = TRUE;
	if (DDS_INIT_RECV == init_mask)
		bReader = TRUE;
	if (DDS_INIT_SENDRECV == init_mask)
	{
		bWriter = TRUE, bReader = TRUE;
	}
		

	if (bWriter)
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
		
		if(1 == iQosOptions)
		{
			stWriterQos.deadline.period.sec = 3;
			stWriterQos.deadline.period.nanosec = 0;
		}
		
		if(2 == iQosOptions)
		{
			stWriterQos.liveliness.kind = MANUAL_BY_TOPIC_LIVELINESS_QOS;
			stWriterQos.liveliness.lease_duration.sec = 3;
			stWriterQos.liveliness.lease_duration.nanosec = 0;
		}
		
		if(3 == iQosOptions)
		{
			stWriterQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
			stWriterQos.history.depth = 10;
		}
		
		if(4 == iQosOptions)
		{
			stWriterQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;			
			stWriterQos.ownership_strength.value = iOwnerShipStrength;
		}

		pstDataWriter = DDS_Topic_create_datawriter(pstTopic, &stWriterQos, NULL);
		if (NULL == pstDataWriter)
		{
			printf("ERROR! Failed to create dataWriter!");
			return -5;
		}
	}

	if (bReader)
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
		
		if(1 == iQosOptions)
		{
			stReaderQos.deadline.period.sec = 6;
			stReaderQos.deadline.period.nanosec = 0;
		}
		
		if(2 == iQosOptions)
		{
			stReaderQos.liveliness.kind = MANUAL_BY_TOPIC_LIVELINESS_QOS;
			stReaderQos.liveliness.lease_duration.sec = 6;
			stReaderQos.liveliness.lease_duration.nanosec = 0;
		}
		
		if(3 == iQosOptions)
		{
			stReaderQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
			stReaderQos.history.depth = 10;
		}
		
		if(4 == iQosOptions)
		{
			stReaderQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
		}
		
		pstDataReader = DDS_Topic_create_datareader(pstTopic, &stReaderQos, ReceiveData);
		if (NULL == pstDataReader)
		{
			printf("ERROR! Failed to create dataReader!");
			return -7;
		}
	}

	return 1;
}


DDS_DLL int ddsdrv_send(int domain_id, const char* topic_name, const void* buf, long length)
{
	DRV_DomainParticipant* pstDrvDomainParticipant = NULL;
	DDS_Topic* pstTempTopic = NULL;
	DDS_DataWriter* pstTempDataWriter = NULL;
	DDS_ReturnCode_t returncode;
	pstDrvDomainParticipant = find_DomainParticipant(domain_id);
	
	pstTempTopic = pstDrvDomainParticipant->pstParticipant->pstTopic;
	while (NULL != pstTempTopic)
	{
		if (0 == strcmp(pstTempTopic->topicName.value, topic_name))
			break;
		pstTempTopic = pstTempTopic->pNext;
	}
	if (NULL == pstTempTopic)
	{
		printf("Topic is null\n");
		return -4;
	}
	
	pstTempDataWriter = pstTempTopic->pstLocalWriter;
	while (NULL != pstTempDataWriter)
	{
		returncode = DDS_StringDataWriter_write(pstTempDataWriter, buf, length);
		if(0 != returncode)
		{
			static int count = 0;
#if defined _DEBUG_LOG
			if( (++count) % 200 == 0)
				printf("Nephalem Send User Data %s len = %d  ret = %d \n",topic_name,length,returncode);
#endif
			return 1;
		}
		//printf("Nephalem Send User Data %s len = %d  ret = %d \n",topic_name,length,returncode);
		pstTempDataWriter = pstTempDataWriter->pNext;
	}

	return 1;
}

DDS_DLL int ddsdrv_recv(int domain_id, const char* topic_name, void* buf, long length, unsigned int* src_ip)
{
	DRV_DomainParticipant* pstDrvDomainParticipant = NULL;
	DDS_Topic* pstTempTopic = NULL;
	DDS_DataReader* pstTempDataReader = NULL;
	HistoryCache* pTempHistory = NULL;

	pstDrvDomainParticipant = find_DomainParticipant(domain_id);

	pstTempTopic = pstDrvDomainParticipant->pstParticipant->pstTopic;
	while (NULL != pstTempTopic)
	{
		if (0 == strcmp(pstTempTopic->topicName.value, topic_name))
			break;
		pstTempTopic = pstTempTopic->pNext;
	}
	if (NULL == pstTempTopic)
	{
		printf("Topic is null\n");
		return -4;
	}
	
	if(length < pstTempTopic->stUserData.pstHead->uiDataLen)
	{
		printf("buff is too small \n");
		return -1;
	}
	
	memset(buf, pstTempTopic->stUserData.pstHead->data, pstTempTopic->stUserData.pstHead->uiDataLen);
	
	pTempHistory = pstTempTopic->stUserData.pstHead;
	pstTempTopic->stUserData.pstHead = pstTempTopic->stUserData.pstHead->pNext;

	free(pTempHistory);
	pTempHistory = NULL;
	
	return pstTempTopic->stUserData.pstHead->uiDataLen;
}

DDS_DLL int ddsdrv_setcb(int domain_id, const char* topic_name, DDS_RECV_CALLBACK_FUNC func)
{	
	DRV_DomainParticipant* pstDrvDomainParticipant = NULL;
	DDS_Topic* pstTempTopic = NULL;
	DDS_DataReader* pstTempDataReader = NULL;

	pstDrvDomainParticipant = find_DomainParticipant(domain_id);

	pstTempTopic = pstDrvDomainParticipant->pstParticipant->pstTopic;
	while (NULL != pstTempTopic)
	{
		if (0 == strcmp(pstTempTopic->topicName.value, topic_name))
			break;
		pstTempTopic = pstTempTopic->pNext;
	}
	if (NULL == pstTempTopic)
	{
		printf("Topic is null\n");
		return -4;
	}

	pstTempDataReader = pstTempTopic->pstLocalReader;
	while (NULL != pstTempDataReader)
	{
		pstTempDataReader->recvData = func;
		pstTempDataReader = pstTempDataReader->pNext;
	}
	
	return 1;

}

DDS_DLL Ver_Info ddsdrv_info_report() 
{
	Ver_Info tempinfo = { 0 };
#if defined _VXWORKS
	struct timespec systime;
	struct tm timenow;
//	time_t nowSec;
//	clock_gettime(CLOCK_REALTIME,&system);
//	nowSec = systime.tv_sec;
//	gmtime_r(&nowSec,&timenow);
	
	tempinfo.major_code = 2;
	tempinfo.minor_code = 0;
	tempinfo.release = 2;
	tempinfo.build = 2;

	tempinfo.year = 2021;//timenow.tm_year;
	tempinfo.month = 12;//timenow.tm_mon;
	tempinfo.day = 15;//timenow.tm_mday;
	tempinfo.hour = 9;//timenow.tm_hour;
	tempinfo.minute = 30;//timenow.tm_min;
	tempinfo.second = 0;//timenow.tm_sec;
#else 
//	SYSTEMTIME timenow;
//	GetLocalTime(&timenow);
//
//	tempinfo.major_code = 0;
//	tempinfo.minor_code = 0;
//	tempinfo.release = 0;
//	tempinfo.build = 0;
//
//	tempinfo.year = timenow.wYear;
//	tempinfo.month = timenow.wMonth;
//	tempinfo.day = timenow.wDay;
//	tempinfo.hour = timenow.wHour;
//	tempinfo.minute = timenow.wMinute;
//	tempinfo.second = timenow.wSecond;
#endif
	return tempinfo;
}
#endif
