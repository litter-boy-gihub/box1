#include "../include/GlobalDefine.h"


extern int iDDSRECV;

extern int iUSERRECV;

/*---------------------------------------------------------------------------------
-   �� �� ��: ParseAllNetworkOrder
-   ��������: �ӱ��Ľ���
-   ��    ��: �����ֽ��򡢳��ȡ�����Participant
-   ��    ��: ��������
-   ȫ�ֱ���:
-   ע    ��: �����ֽ���RTPSЭ�����Ϊ�ӱ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL ParseAllNetworkOrder(CHAR * buffer, UINT32 length, DDS_DomainParticipant* pstParticipant)
{
    AckNack         stAckNackMsg;
    NackFrag        stNackFragMsg;
    Heartbeat       stHeartbeatMsg;
    HeartbeatFrag   stHeartbeatFragMsg;
    Data            stDataMsg;
    DataFrag        stDataFrag;
    InfoTimestamp   stTimestamp;
    Gap             stGapMsg;

    RTPSMessageHeader stRTPSMsgHeader;
    SubMessageHeader  stSubMsgHeader;
	BOOL ret;

	BOOL			bNeedExchangeBytes;
	UINT32          IsLittleEndian;

    /* ��ʼ���ڴ�� */
    MemoryBlock stMemBlock;
    INITIAL_READ_MEMORYBLOCK(stMemBlock, buffer, length);

    /* ��һ������RTPS����ͷ */
    DESERIALIZE_RTPS_MESSAGE_HEADER(stRTPSMsgHeader, stMemBlock);

    /* ������RTPS���� */
//	if (stRTPSMsgHeader.protocolId != PROTOCOL_RTPS_LITTLE_ENDIAN || stRTPSMsgHeader.protocolId != PROTOCOL_RTPS_BIG_ENDIAN)
//		return FALSE;

//    if (!(stRTPSMsgHeader.protocolId == PROTOCOL_RTPS_LITTLE_ENDIAN ))//|| stRTPSMsgHeader.protocolId == PROTOCOL_RTPX_LITTLE_ENDIAN))
//    {
//        return FALSE;
//    }
    
    /* ������RTPS���� */
    //if (!(stRTPSMsgHeader.protocolId == PROTOCOL_RTPS_BIG_ENDIAN ))//|| stRTPSMsgHeader.protocolId == PROTOCOL_RTPX_LITTLE_ENDIAN))
    //{
    //    return FALSE;
    //}

    /* ��ʼ��ʱ�� */
    stTimestamp.value = GetNowTime();
    while (stMemBlock.readIndex <  stMemBlock.totalSize)
    {
        /* �ڶ��������ӱ�����Ϣͷ */
        DESERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);  

		/* Nephalem �������豸��С�����ò�һ��������д�С��ת�� */
		bNeedExchangeBytes = FALSE;
		IsLittleEndian = IS_MACHINE_LITTLE_ENDIAN;
		if (IsLittleEndian != stSubMsgHeader.flags[0])
		{
			bNeedExchangeBytes = TRUE;
		}

		if(bNeedExchangeBytes)
		{
			stSubMsgHeader.submessageLength = BIG_LITTLE_SWAP_UINT16(stSubMsgHeader.submessageLength);
		}

        /* �����������ӱ�����Ϣ�� */
        switch (stSubMsgHeader.submessageId)
        {
        case GAP:
//        	printf("Nephalem before HandleGapMsg \n");
        	
            DESERIALIZE_GAP(stGapMsg, stMemBlock, bNeedExchangeBytes);
            DDS_MUTEX_LOCK(pstParticipant->m_discWtMutex);
            ret = HandleGapMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stTimestamp, &stGapMsg, &stMemBlock, pstParticipant);
            DDS_MUTEX_UNLOCK(pstParticipant->m_discWtMutex);
//          printf("gap handle finished ,start_seqNum: %u, totMiss: %u\n", stGapMsg.gapStart.low,stGapMsg.gapList.numBits);
            break;

        case DATA_FRAG:
        	
//        	printf("Nephalem before HandleUSERWriterDataFrag \n");
        	
            DESERIALIZE_DATAFRAG(stDataFrag, stMemBlock, bNeedExchangeBytes);
            DDS_MUTEX_LOCK(pstParticipant->m_discWtMutex);
            //printf("fragNum: %u, seqNum: %u\n", stDataFrag.fragmentStartingNum, stDataFrag.writerSN.low);
            HandleUSERWriterDataFrag(&stRTPSMsgHeader, &stSubMsgHeader, &stTimestamp, &stDataFrag, &stMemBlock, pstParticipant);
            DDS_MUTEX_UNLOCK(pstParticipant->m_discWtMutex);
            break;

        case ACKNACK:

            DESERIALIZE_ACKNACK(stAckNackMsg, stMemBlock, bNeedExchangeBytes);

//            printf("Nephalem before HandleBuiltinAckNackMsg \n");
            
            if (IsBuiltinEndpoint(stAckNackMsg.writerId))
            {
                HandleBuiltinAckNackMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stAckNackMsg, pstParticipant);
            }
            else
            {
                DDS_MUTEX_LOCK(pstParticipant->m_discRdMutex);
                HandleAckNackMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stAckNackMsg, pstParticipant);
                DDS_MUTEX_UNLOCK(pstParticipant->m_discRdMutex);
            }
            break;

        case NACK_FRAG:

        	//printf("Nephalem before HandleNackFragMsg \n");
        	 
            DESERIALIZE_NACKFRAG(stNackFragMsg, stMemBlock, bNeedExchangeBytes);
            DDS_MUTEX_LOCK(pstParticipant->m_discWtMutex);
            HandleNackFragMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stNackFragMsg, pstParticipant);
            DDS_MUTEX_UNLOCK(pstParticipant->m_discWtMutex);

        case HEARTBEAT:

            DESERIALIZE_HEARTBEAT(stHeartbeatMsg, stMemBlock, bNeedExchangeBytes);

//            printf("Nephalem before HandleBuiltinHeartbeatMsg \n");
            
            if (IsBuiltinEndpoint(stHeartbeatMsg.writerId))
            {
            	if( iDDSRECV == 1)
            		printf("Nephalem before HandleBuiltinHeartbeatMsg \n");
                HandleBuiltinHeartbeatMsg(&stRTPSMsgHeader, &stHeartbeatMsg, pstParticipant);
            }
            else
            {
                DDS_MUTEX_LOCK(pstParticipant->m_discWtMutex);
            	if( iUSERRECV == 1)
            		printf("Nephalem before HandleHeartbeatMsg \n");
                HandleHeartbeatMsg(&stRTPSMsgHeader, &stHeartbeatMsg, pstParticipant);
                DDS_MUTEX_UNLOCK(pstParticipant->m_discWtMutex);
            }
            break;

        case HEARTBEAT_FRAG:

            DESERIALIZE_HEARTBEATFRAG(stHeartbeatFragMsg, stMemBlock, bNeedExchangeBytes);

            DDS_MUTEX_LOCK(pstParticipant->m_discWtMutex);

            //printf("Nephalem before HandleHeartbeatFragMsg \n");
            
            HandleHeartbeatFragMsg(&stRTPSMsgHeader, &stHeartbeatFragMsg, pstParticipant);

            DDS_MUTEX_UNLOCK(pstParticipant->m_discWtMutex);

        case DATA:

            DESERIALIZE_DATA(stDataMsg, stMemBlock, bNeedExchangeBytes);

            if (ENTITYID_IS_EQUAL(stDataMsg.writerId, ENTITYID_SPDP_BUILTIN_PARTICIPANT_WRITER))
            {
            	if( iDDSRECV == 1)
            		printf("Nephalem before HandleSPDPDataMsg \n");
                HandleSPDPDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stDataMsg, &stMemBlock, pstParticipant);
				PrintLog(COMMLOG_INFO, "HandleSPDPDataMsg finished, rdIdx: %u, totSize: %u.\n", stMemBlock.readIndex, stMemBlock.totalSize);
            }
            else if (ENTITYID_IS_EQUAL(stDataMsg.writerId, ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER))
            {
            	if( iDDSRECV == 1)
            		printf("Nephalem before Handle SEDP DataWriter \n");
                HandleSEDPDiscoveredWriterDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stDataMsg, &stMemBlock, pstParticipant);
				PrintLog(COMMLOG_INFO, "HandleSEDPDiscoveredWriterDataMsg finished, rdIdx: %u, totSize: %u.\n", stMemBlock.readIndex, stMemBlock.totalSize);
            }
            else if (ENTITYID_IS_EQUAL(stDataMsg.writerId, ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_WRITER))
            {
            	if( iDDSRECV == 1)
            		printf("Nephalem before Handle SEDP DataReader \n");
                HandleSEDPDiscoveredReaderDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stDataMsg, &stMemBlock, pstParticipant);
				PrintLog(COMMLOG_INFO, "HandleSEDPDiscoveredReaderDataMsg finished, rdIdx: %u, totSize: %u.\n", stMemBlock.readIndex, stMemBlock.totalSize);
            }
            else
            {
                DDS_MUTEX_LOCK(pstParticipant->m_discWtMutex);
               
                if( iUSERRECV == 1)
                	printf("Nephalem before Handle USERDATA \n");
                HandleUSERWriterDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stTimestamp, &stDataMsg, &stMemBlock, pstParticipant);
				PrintLog(COMMLOG_INFO, "HandleUSERWriterDataMsg finished, rdIdx: %u, totSize: %u.\n", stMemBlock.readIndex, stMemBlock.totalSize);
                DDS_MUTEX_UNLOCK(pstParticipant->m_discWtMutex);
            }
            break;

        case INFO_TS:

            DESERIALIZE_INFOTIMESTAMP(stTimestamp, stMemBlock, bNeedExchangeBytes);
            break;

#ifdef DDS_MONITOR
            case MONITOR:

                DESERIALIZE_DATA(stDataMsg, stMemBlock);

                if (ENTITYID_IS_EQUAL(stDataMsg.writerId, ENTITYID_SPDP_BUILTIN_PARTICIPANT_WRITER))
                {
                    /* �鲥��ؽڵ� */
                     DDS_MUTEX_LOCK(pstParticipant->monMutex);
                     HandleMonitorDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stDataMsg, &stMemBlock, pstParticipant);
                     DDS_MUTEX_UNLOCK(pstParticipant->monMutex);
                }
                break;

            case TOPIC:

                DESERIALIZE_DATA(stDataMsg, stMemBlock);

                if (ENTITYID_IS_EQUAL(stDataMsg.writerId, ENTITYID_SPDP_BUILTIN_PARTICIPANT_WRITER))
                {
                    /* ������ */
                    DDS_MUTEX_LOCK(pstParticipant->monMutex);
                    HandleTopicDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stDataMsg, &stMemBlock, pstParticipant);
                    DDS_MUTEX_UNLOCK(pstParticipant->monMutex);
                }
                break;
#endif
			default:

				/* ����δʶ����ӱ��� */
				MEMORYBLOCK_SKIP_READER(stMemBlock, stSubMsgHeader.submessageLength);
				break;
		}
	}
	return TRUE;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: ParseAllNetworkOrder
-   ��������: �ӱ��Ľ���
-   ��    ��: �����ֽ��򡢳��ȡ�����Participant
-   ��    ��: ��������
-   ȫ�ֱ���:
-   ע    ��: �����ֽ���RTPSЭ�����Ϊ�ӱ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
//BOOL ParseNoBlockNetworkOrder(CHAR * buffer, UINT32 length, DDS_DomainParticipant* pstParticipant)
//{
//	AckNack         stAckNackMsg;
//	Heartbeat       stHeartbeatMsg;
//	Data            stDataMsg;
//	InfoTimestamp   stTimestamp;
//    Gap             stGapMsg;
//
//	RTPSMessageHeader stRTPSMsgHeader;
//	SubMessageHeader  stSubMsgHeader;
//
//	/* ��ʼ���ڴ�� */
//	MemoryBlock stMemBlock;
//	INITIAL_READ_MEMORYBLOCK(stMemBlock, buffer, length);
//
//	/* ��һ������RTPS����ͷ */
//	DESERIALIZE_RTPS_MESSAGE_HEADER(stRTPSMsgHeader, stMemBlock);
//
//	/* ������RTPS���� */
//	if (!(stRTPSMsgHeader.protocolId == PROTOCOL_RTPS_LITTLE_ENDIAN))//|| stRTPSMsgHeader.protocolId == PROTOCOL_RTPX_LITTLE_ENDIAN))
//	{
//		return FALSE;
//	}
//
//	/* ��ʼ��ʱ�� */
//	stTimestamp.value = GetNowTime();
//
//	while (stMemBlock.readIndex <  stMemBlock.totalSize)
//	{
//		/* �ڶ��������ӱ�����Ϣͷ */
//		DESERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);
//
//		/* �����������ӱ�����Ϣ�� */
//		switch (stSubMsgHeader.submessageId)
//		{
//        case GAP:
//            DESERIALIZE_GAP(stGapMsg, stMemBlock);
//            DDS_MUTEX_LOCK(pstParticipant->m_discWtMutex);
//            HandleGapMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stTimestamp, &stGapMsg, &stMemBlock, pstParticipant);
//            DDS_MUTEX_UNLOCK(pstParticipant->m_discRdMutex);
////            printf("gap handle finished 216\n");
//            break;
//
//		case ACKNACK:
//
//			DESERIALIZE_ACKNACK(stAckNackMsg, stMemBlock);
//
//			if (IsBuiltinEndpoint(stAckNackMsg.writerId))
//			{
//				HandleBuiltinAckNackMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stAckNackMsg, pstParticipant);
//			}
//			else
//			{
//				HandleAckNackMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stAckNackMsg, pstParticipant);
//			}
//			break;
//
//		case HEARTBEAT:
//
//			DESERIALIZE_HEARTBEAT(stHeartbeatMsg, stMemBlock);
//
//			if (IsBuiltinEndpoint(stHeartbeatMsg.writerId))
//			{
//				HandleBuiltinHeartbeatMsg(&stRTPSMsgHeader, &stHeartbeatMsg, pstParticipant);
//			}
//			else
//			{
//				HandleHeartbeatMsg(&stRTPSMsgHeader, &stHeartbeatMsg, pstParticipant);
//			}
//			break;
//
//		case DATA:
//
//			DESERIALIZE_DATA(stDataMsg, stMemBlock);
//
//			if (ENTITYID_IS_EQUAL(stDataMsg.writerId, ENTITYID_SPDP_BUILTIN_PARTICIPANT_WRITER))
//			{
//				HandleSPDPDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stDataMsg, &stMemBlock, pstParticipant);
//			}
//			else if (ENTITYID_IS_EQUAL(stDataMsg.writerId, ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER))
//			{
//				HandleSEDPDiscoveredWriterDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stDataMsg, &stMemBlock, pstParticipant);
//			}
//			else if (ENTITYID_IS_EQUAL(stDataMsg.writerId, ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_WRITER))
//			{
//				HandleSEDPDiscoveredReaderDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stDataMsg, &stMemBlock, pstParticipant);
//			}
//			else
//			{
//				HandleUSERWriterDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stTimestamp, &stDataMsg, &stMemBlock, pstParticipant);
//			}
//			break;
//
//		case INFO_TS:
//
//			DESERIALIZE_INFOTIMESTAMP(stTimestamp, stMemBlock);
//			break;
//
//#ifdef DDS_MONITOR
//		case MONITOR:
//
//			DESERIALIZE_DATA(stDataMsg, stMemBlock);
//
//			if (ENTITYID_IS_EQUAL(stDataMsg.writerId, ENTITYID_SPDP_BUILTIN_PARTICIPANT_WRITER))
//			{
//				/* �鲥��ؽڵ� */
//				HandleMonitorDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stDataMsg, &stMemBlock, pstParticipant);
//			}
//			break;
//
//		case TOPIC:
//
//			DESERIALIZE_DATA(stDataMsg, stMemBlock);
//
//			if (ENTITYID_IS_EQUAL(stDataMsg.writerId, ENTITYID_SPDP_BUILTIN_PARTICIPANT_WRITER))
//			{
//				/* ������ */
//				HandleTopicDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stDataMsg, &stMemBlock, pstParticipant);
//			}
//			break;
//#endif
//		default:
//
//			/* ����δʶ����ӱ��� */
//			MEMORYBLOCK_SKIP_READER(stMemBlock, stSubMsgHeader.submessageLength);
//			break;
//		}
//	}
//	return TRUE;
//}


/*---------------------------------------------------------------------------------
-   �� �� ��: ParseAllNetworkOrder
-   ��������: �ӱ��Ľ���
-   ��    ��: �����ֽ��򡢳��ȡ�����Participant
-   ��    ��: ��������
-   ȫ�ֱ���:
-   ע    ��: �����ֽ���RTPSЭ�����Ϊ�ӱ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
//BOOL ParseBlockNetworkOrder(CHAR * buffer, UINT32 length, DDS_DomainParticipant* pstParticipant)
//{
//    AckNack         stAckNackMsg;
//    Heartbeat       stHeartbeatMsg;
//    Data            stDataMsg;
//    InfoTimestamp   stTimestamp;
//    Gap             stGapMsg;
//
//    RTPSMessageHeader stRTPSMsgHeader;
//    SubMessageHeader  stSubMsgHeader;
//
//    /* ��ʼ���ڴ�� */
//    MemoryBlock stMemBlock;
//    INITIAL_READ_MEMORYBLOCK(stMemBlock, buffer, length);
//
//    /* ��һ������RTPS����ͷ */
//    DESERIALIZE_RTPS_MESSAGE_HEADER(stRTPSMsgHeader, stMemBlock);
//
//    /* ������RTPS���� */
//    if (!(stRTPSMsgHeader.protocolId == PROTOCOL_RTPS_LITTLE_ENDIAN))//|| stRTPSMsgHeader.protocolId == PROTOCOL_RTPX_LITTLE_ENDIAN))
//    {
//        return FALSE;
//    }
//
//    /* ��ʼ��ʱ�� */
//    stTimestamp.value = GetNowTime();
//
//    while (stMemBlock.readIndex <  stMemBlock.totalSize)
//    {
//        /* �ڶ��������ӱ�����Ϣͷ */
//        DESERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);
//
//        /* �����������ӱ�����Ϣ�� */
//        switch (stSubMsgHeader.submessageId)
//        {
//        case GAP:
//            DESERIALIZE_GAP(stGapMsg, stMemBlock);
//            DDS_MUTEX_LOCK(pstParticipant->m_discWtMutex);
//            HandleGapMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stTimestamp, &stGapMsg, &stMemBlock, pstParticipant);
//            DDS_MUTEX_UNLOCK(pstParticipant->m_discWtMutex);
////            printf("gap handle finished 360\n");
//            break;
//
//        case ACKNACK:
//
//            DESERIALIZE_ACKNACK(stAckNackMsg, stMemBlock);
//
//            if (IsBuiltinEndpoint(stAckNackMsg.writerId))
//            {
//                HandleBuiltinAckNackMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stAckNackMsg, pstParticipant);
//            }
//            else
//            {
//                DDS_MUTEX_LOCK(pstParticipant->m_discRdMutex);
//                HandleAckNackMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stAckNackMsg, pstParticipant);
//                DDS_MUTEX_UNLOCK(pstParticipant->m_discRdMutex);
//            }
//            break;
//
//        case HEARTBEAT:
//
//            DESERIALIZE_HEARTBEAT(stHeartbeatMsg, stMemBlock);
//
//            if (IsBuiltinEndpoint(stHeartbeatMsg.writerId))
//            {
//                HandleBuiltinHeartbeatMsg(&stRTPSMsgHeader, &stHeartbeatMsg, pstParticipant);
//            }
//            else
//            {
//                DDS_MUTEX_LOCK(pstParticipant->m_discWtMutex);
//                HandleHeartbeatMsg(&stRTPSMsgHeader, &stHeartbeatMsg, pstParticipant);
//                DDS_MUTEX_UNLOCK(pstParticipant->m_discWtMutex);
//            }
//            break;
//
//        case DATA:
//
//            DESERIALIZE_DATA(stDataMsg, stMemBlock);
//
//            if (ENTITYID_IS_EQUAL(stDataMsg.writerId, ENTITYID_SPDP_BUILTIN_PARTICIPANT_WRITER))
//            {
//                HandleSPDPDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stDataMsg, &stMemBlock, pstParticipant);
//            }
//            else if (ENTITYID_IS_EQUAL(stDataMsg.writerId, ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER))
//            {
//                HandleSEDPDiscoveredWriterDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stDataMsg, &stMemBlock, pstParticipant);
//            }
//            else if (ENTITYID_IS_EQUAL(stDataMsg.writerId, ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_WRITER))
//            {
//                HandleSEDPDiscoveredReaderDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stDataMsg, &stMemBlock, pstParticipant);
//            }
//            else
//            {
//                DDS_MUTEX_LOCK(pstParticipant->m_discWtMutex);
//                HandleUSERWriterDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stTimestamp, &stDataMsg, &stMemBlock, pstParticipant);
//                DDS_MUTEX_UNLOCK(pstParticipant->m_discWtMutex);
//            }
//            break;
//
//        case INFO_TS:
//
//            DESERIALIZE_INFOTIMESTAMP(stTimestamp, stMemBlock);
//            break;
//
//#ifdef DDS_MONITOR
//		case MONITOR:
//
//            DESERIALIZE_DATA(stDataMsg, stMemBlock);
//
//			if (ENTITYID_IS_EQUAL(stDataMsg.writerId, ENTITYID_SPDP_BUILTIN_PARTICIPANT_WRITER))
//			{
//				/* �鲥��ؽڵ� */
//				DDS_MUTEX_LOCK(pstParticipant->monMutex);
//				HandleMonitorDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stDataMsg, &stMemBlock, pstParticipant);
//				DDS_MUTEX_UNLOCK(pstParticipant->monMutex);
//			}
//			break;
//
//        case TOPIC:
//
//            DESERIALIZE_DATA(stDataMsg, stMemBlock);
//
//			if (ENTITYID_IS_EQUAL(stDataMsg.writerId, ENTITYID_SPDP_BUILTIN_PARTICIPANT_WRITER))
//			{
//				/* ������ */
//				DDS_MUTEX_LOCK(pstParticipant->monMutex);
//				HandleTopicDataMsg(&stRTPSMsgHeader, &stSubMsgHeader, &stDataMsg, &stMemBlock, pstParticipant);
//				DDS_MUTEX_UNLOCK(pstParticipant->monMutex);
//			}
//			break;
//#endif
//		default:
//
//			/* ����δʶ����ӱ��� */
//			MEMORYBLOCK_SKIP_READER(stMemBlock, stSubMsgHeader.submessageLength);
//			break;
//		}
//	}
//	return TRUE;
//}
