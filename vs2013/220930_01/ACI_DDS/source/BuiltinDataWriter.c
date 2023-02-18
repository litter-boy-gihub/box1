#include "../include/GlobalDefine.h"

/*---------------------------------------------------------------------------------
-   �� �� ��: InitBuiltinDataWriter
-   ��������: �ڽ�д������ʼ��
-   ��    ��: �ڽ�д����
-   ��    ��: 
-   ȫ�ֱ���:
-   ע    ��: �ڽ�д������ʼ��
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID InitBuiltinDataWriter(BuiltinDataWriter* pstBuiltinDataWriter)
{
    if (NULL != pstBuiltinDataWriter)
    {
        pstBuiltinDataWriter->pstBuiltinDiscReader = NULL;
        pstBuiltinDataWriter->stHeartbeat.count = 0;
        pstBuiltinDataWriter->seqNum = SEQUENCENUMBER_ZERO;
        InitHistoryCache(&pstBuiltinDataWriter->stHistoryCache);
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: UnInitBuiltinDiscReader
-   ��������: �ڽ�д������ʼ��
-   ��    ��: �ڽ�д����
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �ڽ�д������ʼ��
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\03\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID UnInitBuiltinDiscReader(BuiltinDiscReader*  pstBuiltinDiscReader)
{
    if (NULL != pstBuiltinDiscReader)
    {
        //Locator_t* pstTempNode = NULL;

        ///* ����������ַ */
        //LIST_DELETE_ALL(pstBuiltinDiscReader->pstUnicastLocator, pstTempNode);

        ///* �����鲥��ַ */
        //LIST_DELETE_ALL(pstBuiltinDiscReader->pstMulticastLocator, pstTempNode);
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: UnInitBuiltinDataWriter
-   ��������: �ڽ�д������ʼ��
-   ��    ��: �ڽ�д����
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �ڽ�д������ʼ��
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\03\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID UnInitBuiltinDataWriter(BuiltinDataWriter* pstBuiltinDataWriter)
{
	BuiltinDiscReader*  pstBuiltinDiscReader = NULL;
    if (NULL != pstBuiltinDataWriter)
    {
        UninitHistoryCache(&pstBuiltinDataWriter->stHistoryCache);
        pstBuiltinDiscReader = pstBuiltinDataWriter->pstBuiltinDiscReader;
        while (NULL != pstBuiltinDiscReader)
        {
            pstBuiltinDataWriter->pstBuiltinDiscReader = pstBuiltinDiscReader->pNext;
            UnInitBuiltinDiscReader(pstBuiltinDiscReader);
            DDS_STATIC_FREE(pstBuiltinDiscReader);

            pstBuiltinDiscReader = pstBuiltinDataWriter->pstBuiltinDiscReader;
        }
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: JointSEDPDataMsgSendALLBuiltinReader
-   ��������: ��װSEDP���ķ�������Զ���ڽ��Ķ���
-   ��    ��: RTPSͷ��Զ���Ķ���EntityId���������ݡ������ڽ�д����
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��װSEDP���ķ�������Զ���ڽ��Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID JointSEDPDataMsgSendALLBuiltinReader(RTPSMessageHeader* pstRTPSMsgHeader, HistoryData* pstHistoryData, BuiltinDataWriter* pstBuiltinDataWriter)
{
    MemoryBlock       stMemBlock;
    MemoryBlock       stMemBlockMod;
    SubMessageHeader  stSubMsgHeader;
    ParameterHead     stParamHead;
    InfoTimestamp     stInfoTS;
    InfoDestination   stInfoDest;
    Data              stData;
    CHAR              buff[NETWORK_BYTE_MAX_SIZE];
	BuiltinDiscReader*  pstBuiltinDiscReader = NULL;

    /* �����ֽ����ʼ�������淢������ */
    INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

    /* ��һ�����л�RTPS����ͷ */
    SERIALIZE_RTPS_MESSAGE_HEADER((*pstRTPSMsgHeader), stMemBlock);

    /* ʱ�䱨��ͷ���� */
    stSubMsgHeader.submessageId = INFO_TS;
    stSubMsgHeader.submessageLength = sizeof(Time_t);
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
    stSubMsgHeader.flags[2] = FALSE;   //dataFlag
    stSubMsgHeader.flags[3] = FALSE;   //keyFlag
    stInfoTS.value = GetNowTime();

    /* ���л�ʱ�䱨��ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���л�ʱ���ӱ����� */
    SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

    stSubMsgHeader.submessageId = INFO_DST;
    stSubMsgHeader.submessageLength = 12;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;
    stSubMsgHeader.flags[3] = FALSE;
    stInfoDest.guidPrefix = GUIDPREFIX_UNKNOWN;

    /* �ڶ������л��ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���������л��ӱ�����Ϣ�� */
    SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

    /* data����ͷ���� */
    stSubMsgHeader.submessageId = DATA;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = TRUE;    //inlineQosFlag
    stSubMsgHeader.flags[2] = TRUE;    //dataFlag
    stSubMsgHeader.flags[3] = FALSE;   //keyFlag

    /* data���������� */
    stData.extraFlags[0] = 0x00;
    stData.extraFlags[1] = 0x00;
    stData.octetsToInlineQos = 0x0010;
    stData.readerId = ENTITYID_UNKNOWN;
    stData.writerId = pstBuiltinDataWriter->guid.entityId;
    stData.writerSN = pstHistoryData->seqNum;
	if (IS_MACHINE_LITTLE_ENDIAN)
		stData.serializedPayload.m_cdrType = PL_CDR_LE;
	else
		stData.serializedPayload.m_cdrType = PL_CDR_BE;
    
    stData.serializedPayload.m_option = 0x0000;

    /* �ڶ������л��ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���������л��ӱ�����Ϣ�� */
    SERIALIZE_DATA(stData, stMemBlock);

    /* ���л������б� */
    stParamHead.paramType = PID_KEY_HASH;
    stParamHead.length = sizeof(GUID_t);
    SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);
    SERIALIZE_GUID(pstHistoryData->stGuid, stMemBlock);

    /* ���л������б��β */
    stParamHead.paramType = PID_SENTINEL;
    stParamHead.length = 0;
    SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);

    /* ���л�����Ч�غ� */
    SERIALIZE_SERIALIZEDDATA(stData.serializedPayload, stMemBlock);

    /* ���л�д������Ϣ */
    PUT_BYTE_ARRAY(stMemBlock, pstHistoryData->data, pstHistoryData->uiDataLen);

    /* �����ӱ�����Ϣ�峤�ȣ�������Ϣͷ */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stData.uiStartIndex;

    /* ���л��޸��ӱ��ĳ��� */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

    /* ���ͷ������� */
    pstBuiltinDiscReader = pstBuiltinDataWriter->pstBuiltinDiscReader;
    while (NULL != pstBuiltinDiscReader)
    {
        /* �޸�Ŀ�ĵ�GuidPrefix */
        INITIAL_MEMORYBLOCK(stMemBlockMod, stInfoDest.pcGuidPre, sizeof(GuidPrefix_t));

        SERIALIZE_GUID_PREFIX(pstBuiltinDiscReader->guid.prefix, stMemBlockMod);

        /* �޸�Ŀ�ĵ�GuidEntityId */
        INITIAL_MEMORYBLOCK(stMemBlockMod, stData.pcReaderId, sizeof(EntityId_t));

        SERIALIZE_ENTITYID(pstBuiltinDiscReader->guid.entityId, stMemBlockMod);

        RTPSSendMsg(&stMemBlock, &pstBuiltinDataWriter->pstParticipant->socketList.sedp_data_sender, pstBuiltinDiscReader->pstUnicastLocator);

        pstBuiltinDiscReader = pstBuiltinDiscReader->pNext;
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: JointSEDPDataMsgSendAssignBuiltinReader
-   ��������: ��װSEDP���ķ���ָ��Զ���ڽ��Ķ���
-   ��    ��: RTPSͷ��Զ���Ķ���EntityId���������ݡ������ڽ�д������Զ���ڽ��Ķ���
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��װSEDP���ķ���ָ��Զ���ڽ��Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID JointSEDPDataMsgSendAssignBuiltinReader(RTPSMessageHeader* pstRTPSMsgHeader, HistoryData* pstHistoryData, BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader*  pstBuiltinDiscReader)
{
    MemoryBlock stMemBlock;
    MemoryBlock stMemBlockMod;
    SubMessageHeader  stSubMsgHeader;
    ParameterHead     stParamHead;
    InfoTimestamp     stInfoTS;
    InfoDestination   stInfoDest;
    Data              stData;
    CHAR              buff[NETWORK_BYTE_MAX_SIZE];

    /* �����ֽ����ʼ�������淢������ */
    INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

    /* ��һ�����л�RTPS����ͷ */
    SERIALIZE_RTPS_MESSAGE_HEADER((*pstRTPSMsgHeader), stMemBlock);

    /* ʱ�䱨��ͷ���� */
    stSubMsgHeader.submessageId = INFO_TS;
    stSubMsgHeader.submessageLength = sizeof(Time_t);
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
    stSubMsgHeader.flags[2] = FALSE;   //dataFlag
    stSubMsgHeader.flags[3] = FALSE;   //keyFlag
    stInfoTS.value = GetNowTime();

    /* ���л�ʱ�䱨��ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���л�ʱ���ӱ����� */
    SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

    stSubMsgHeader.submessageId = INFO_DST;
    stSubMsgHeader.submessageLength = 12;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;
    stSubMsgHeader.flags[3] = FALSE;
    stInfoDest.guidPrefix = pstBuiltinDiscReader->guid.prefix;

    /* �ڶ������л��ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���������л��ӱ�����Ϣ�� */
    SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

    /* data����ͷ���� */
    stSubMsgHeader.submessageId = DATA;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = TRUE;   //inlineQosFlag
    stSubMsgHeader.flags[2] = TRUE;    //dataFlag
    stSubMsgHeader.flags[3] = FALSE;   //keyFlag

    /* data���������� */
    stData.extraFlags[0] = 0x00;
    stData.extraFlags[1] = 0x00;
    stData.octetsToInlineQos = 0x0010;
    stData.readerId = pstBuiltinDiscReader->guid.entityId;
    stData.writerId = pstBuiltinDataWriter->guid.entityId;
    stData.writerSN = pstHistoryData->seqNum;
	if (IS_MACHINE_LITTLE_ENDIAN)
		stData.serializedPayload.m_cdrType = PL_CDR_LE;
	else
		stData.serializedPayload.m_cdrType = PL_CDR_BE;
    
    stData.serializedPayload.m_option = 0x0000;

    /* �ڶ������л��ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���������л��ӱ�����Ϣ�� */
    SERIALIZE_DATA(stData, stMemBlock);

    /* ���л������б� */
    stParamHead.paramType = PID_KEY_HASH;
    stParamHead.length = sizeof(GUID_t);
    SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);
    SERIALIZE_GUID(pstHistoryData->stGuid, stMemBlock);

    /* ���л������б��β */
    stParamHead.paramType = PID_SENTINEL;
    stParamHead.length = 0;
    SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);

    /* ���л�����Ч�غ� */
    SERIALIZE_SERIALIZEDDATA(stData.serializedPayload, stMemBlock);

    /* ���л�д������Ϣ */
    PUT_BYTE_ARRAY(stMemBlock, pstHistoryData->data, pstHistoryData->uiDataLen);

    /* �����ӱ�����Ϣ�峤�ȣ�������Ϣͷ */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stData.uiStartIndex;

    /* ���л��޸��ӱ��ĳ��� */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

    RTPSSendMsg(&stMemBlock, &pstBuiltinDataWriter->pstParticipant->socketList.sedp_data_sender, pstBuiltinDiscReader->pstUnicastLocator);
}

/*---------------------------------------------------------------------------------
-   �� �� ��: JointSEDPDataMsgSendBuiltinReader
-   ��������: ��װSEDP���ķ���Զ���ڽ��Ķ���
-   ��    ��: RTPSͷ���������ݡ������ڽ�д������Զ���ڽ��Ķ���
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��װSEDP���ķ���ָ��Զ���ڽ��Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\7\24       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID JointSEDPDataMsgSendBuiltinReader(RTPSMessageHeader* pstRTPSMsgHeader, HistoryData* pstHistoryData, BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader*  pstBuiltinDiscReader)
{
	
    MemoryBlock       stMemBlock;
    MemoryBlock       stMemBlockMod;
    SubMessageHeader  stSubMsgHeader;
    ParameterHead     stParamHead;
    InfoTimestamp     stInfoTS;
    InfoDestination   stInfoDest;
    Data              stData;
    CHAR              buff[NETWORK_BYTE_MAX_SIZE];
	UINT32 Flags = 0;

    /* �����ֽ����ʼ�������淢������ */
    INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

    /* ��һ�����л�RTPS����ͷ */
    SERIALIZE_RTPS_MESSAGE_HEADER((*pstRTPSMsgHeader), stMemBlock);

    /* ʱ�䱨��ͷ���� */
    stSubMsgHeader.submessageId = INFO_TS;
    stSubMsgHeader.submessageLength = sizeof(Time_t);
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
    stSubMsgHeader.flags[2] = FALSE;   //dataFlag
    stSubMsgHeader.flags[3] = FALSE;   //keyFlag
    stInfoTS.value = GetNowTime();

    /* ���л�ʱ�䱨��ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���л�ʱ���ӱ����� */
    SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

    stSubMsgHeader.submessageId = INFO_DST;
    stSubMsgHeader.submessageLength = 12;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;
    stSubMsgHeader.flags[3] = FALSE;
    stInfoDest.guidPrefix = GUIDPREFIX_UNKNOWN;

    /* �ڶ������л��ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���������л��ӱ�����Ϣ�� */
    SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

    /* data����ͷ���� */
    stSubMsgHeader.submessageId = DATA;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;    //inlineQosFlag
    stSubMsgHeader.flags[2] = (pstHistoryData->data) ? TRUE : FALSE;  //dataPresent
    stSubMsgHeader.flags[3] = FALSE;   //keyFlag

    /* data���������� */
    stData.extraFlags[0] = 0x00;
    stData.extraFlags[1] = 0x00;
    stData.octetsToInlineQos = 0x0010;
    stData.readerId = ENTITYID_UNKNOWN;
    stData.writerId = pstBuiltinDataWriter->guid.entityId;
    stData.writerSN = pstHistoryData->seqNum;
	if (IS_MACHINE_LITTLE_ENDIAN)
		stData.serializedPayload.m_cdrType = PL_CDR_LE;   //littleEndian
	else
		stData.serializedPayload.m_cdrType = PL_CDR_BE;    //littleEndian
    stData.serializedPayload.m_option = 0x0000;

    /* �ڶ������л��ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���л�����QOSȡ���������� */
    if (pstHistoryData->bCancel)
    {
    	stSubMsgHeader.flags[1] = TRUE;    //inlineQosFlag
    }
    
    /* ���������л��ӱ�����Ϣ�� */
    SERIALIZE_DATA(stData, stMemBlock);


    /* ���л�����QOSȡ���������� */
    if (pstHistoryData->bCancel)
    {  	
        /* ���л������б� */
        stParamHead.paramType = PID_KEY_HASH;
        stParamHead.length = sizeof(GUID_t);
        SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);
        SERIALIZE_GUID(pstHistoryData->stGuid, stMemBlock);
        
        /* ���л������б� */
        Flags = 0x01000000;
        stParamHead.paramType = PID_STATUS_INFO;
        stParamHead.length = sizeof(UINT32);
        SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);
        PUT_UNSIGNED_INT(stMemBlock, Flags);
        
        /* ���л������б��β */
        stParamHead.paramType = PID_SENTINEL;
        stParamHead.length = 0;
        SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);
    }

    /* ���л��û����� */
    if (pstHistoryData->data)
    {
        /* ���л�����Ч�غ� */
        SERIALIZE_SERIALIZEDDATA(stData.serializedPayload, stMemBlock);

        /* ���л�д������Ϣ */
        PUT_BYTE_ARRAY(stMemBlock, pstHistoryData->data, pstHistoryData->uiDataLen);
    }

    /* �����ӱ�����Ϣ�峤�ȣ�������Ϣͷ */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - stData.uiStartIndex;

    /* ���л��޸��ӱ��ĳ��� */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

    /* ���͵�ָ���Զ��Ķ��� */
    if (NULL != pstBuiltinDiscReader)
    {
        /* �޸�Ŀ�ĵ�GuidPrefix */
        INITIAL_MEMORYBLOCK(stMemBlockMod, stInfoDest.pcGuidPre, sizeof(GuidPrefix_t));
        SERIALIZE_GUID_PREFIX(pstBuiltinDiscReader->guid.prefix, stMemBlockMod);

        /* �޸�Ŀ�ĵ�GuidEntityId */
        INITIAL_MEMORYBLOCK(stMemBlockMod, stData.pcReaderId, sizeof(EntityId_t));
        SERIALIZE_ENTITYID(pstBuiltinDiscReader->guid.entityId, stMemBlockMod);

//		PrintBinData(stMemBlock.base, stMemBlock.writeIndex, "send SEDP");
        RTPSSendMsg(&stMemBlock, &pstBuiltinDataWriter->pstParticipant->socketList.sedp_data_sender, pstBuiltinDiscReader->pstUnicastLocator);
        return;
    }

    /* ���͵����жԶ��Ķ��� */
    pstBuiltinDiscReader = pstBuiltinDataWriter->pstBuiltinDiscReader;
    while (NULL != pstBuiltinDiscReader)
    {
        /* �޸�Ŀ�ĵ�GuidPrefix */
        INITIAL_MEMORYBLOCK(stMemBlockMod, stInfoDest.pcGuidPre, sizeof(GuidPrefix_t));
        SERIALIZE_GUID_PREFIX(pstBuiltinDiscReader->guid.prefix, stMemBlockMod);

        /* �޸�Ŀ�ĵ�GuidEntityId */
        INITIAL_MEMORYBLOCK(stMemBlockMod, stData.pcReaderId, sizeof(EntityId_t));
        SERIALIZE_ENTITYID(pstBuiltinDiscReader->guid.entityId, stMemBlockMod);

//		PrintBinData(stMemBlock.base, stMemBlock.writeIndex, "send SEDP");
        RTPSSendMsg(&stMemBlock, &pstBuiltinDataWriter->pstParticipant->socketList.sedp_data_sender, pstBuiltinDiscReader->pstUnicastLocator);
        pstBuiltinDiscReader = pstBuiltinDiscReader->pNext;
    }

    return;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: JointHeartbeatMsgSendAllBuiltinReader
-   ��������: ��װ�������ķ���ָ���ڽ��Ķ���
-   ��    ��: ����DomainParticipantָ��
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��װ�������ķ���ָ���ڽ��Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID JointHeartbeatMsgSendAllBuiltinReader(RTPSMessageHeader* pstRTPSMsgHeader, BuiltinDataWriter* pstBuiltinDataWriter)
{
    MemoryBlock stMemBlock;
    MemoryBlock stMemBlockMod;
    SubMessageHeader  stSubMsgHeader;
    InfoTimestamp     stInfoTS;
    InfoDestination   stInfoDest;
    CHAR buff[NETWORK_BYTE_MAX_SIZE];
	BuiltinDiscReader* pstBuiltinDiscReader = NULL;

    /* �����޻��棬���������� */
    if (NULL == pstBuiltinDataWriter->stHistoryCache.pstHead)
    {
        return;
    }

    /* �����ֽ����ʼ�������淢������ */
    INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

    /* ��һ�����л�RTPS����ͷ */
    SERIALIZE_RTPS_MESSAGE_HEADER((*pstRTPSMsgHeader), stMemBlock);

    /* ʱ�䱨��ͷ���� */
//    stSubMsgHeader.submessageId = INFO_TS;
//    stSubMsgHeader.submessageLength = sizeof(Time_t);
//    stSubMsgHeader.flags[0] = FALSE;    //littleEndian
//    stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
//    stSubMsgHeader.flags[2] = FALSE;   //dataFlag
//    stSubMsgHeader.flags[3] = FALSE;   //keyFlag
//    stInfoTS.value = GetNowTime();
//
//    /* ���л�ʱ�䱨��ͷ */
//    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);
//
//    /* ���л�ʱ���ӱ����� */
//    SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

    /* InfoDT���� */
    stSubMsgHeader.submessageId = INFO_DST;
    stSubMsgHeader.submessageLength = 12;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;
    stSubMsgHeader.flags[3] = FALSE;
    stInfoDest.guidPrefix = GUIDPREFIX_UNKNOWN;

    /* �ڶ������л��ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���������л��ӱ�����Ϣ�� */
    SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

    /* ��������ͷ */
    stSubMsgHeader.submessageId = HEARTBEAT;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;
    stSubMsgHeader.flags[3] = FALSE;

    /* ����������Ϣ�� */
    pstBuiltinDataWriter->stHeartbeat.readerId = ENTITYID_UNKNOWN;
    pstBuiltinDataWriter->stHeartbeat.writerId = pstBuiltinDataWriter->guid.entityId;
    pstBuiltinDataWriter->stHeartbeat.firstSN = pstBuiltinDataWriter->stHistoryCache.pstHead->seqNum;
    pstBuiltinDataWriter->stHeartbeat.lastSN = pstBuiltinDataWriter->stHistoryCache.pstTail->seqNum;
    pstBuiltinDataWriter->stHeartbeat.count++;
    
    /* ���л��û������ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���л��û������ӱ�����Ϣ�壬ͬʱ̽���¼readerIdλ�� */
    SERIALIZE_HEARTBEAT(pstBuiltinDataWriter->stHeartbeat, stMemBlock);

    /* �����ӱ�����Ϣ�峤�ȣ�������Ϣͷ */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - pstBuiltinDataWriter->stHeartbeat.uiStartIndex;

    /* ���л��޸��ӱ��ĳ��� */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));
    
    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);

    pstBuiltinDiscReader = pstBuiltinDataWriter->pstBuiltinDiscReader;
    while (NULL != pstBuiltinDiscReader)
    {
#ifndef QYDDS_LIVELINESS_QOS
		/* �Ͽ����ӣ��������Զ����Ϣ */
		if (HEARTBEAT_DISCONNECT < pstBuiltinDiscReader->uiSendHeartBeatNum)// || Compare_Duration(usedTime,pstBuiltinDataWriter->pstParticipant->durationEdge))
		{
			DDS_DiscoveredParticipant* pstDiscParticipant = pstBuiltinDataWriter->pstParticipant->pstDiscParticipant;
			while (pstDiscParticipant != NULL)
			{
				if (GuidPrefix_Is_Equal(&pstDiscParticipant->stRTPSMsgHeader.guidPrefix, &pstBuiltinDiscReader->guid.prefix))
				{
					int k[4];
					UINT32 addr = pstDiscParticipant->IPAddr;
					k[0] = addr & 0xFF;
					addr >>= 8;
					k[1] = addr & 0xFF;
					addr >>= 8;
					k[2] = addr & 0xFF;
					addr >>= 8;
					k[3] = addr & 0xFF;
					printf("583The remote participant disconnect! IPAddr: %d.%d.%d.%d, ProcessID: %d.\n", k[0], k[1], k[2], k[3], pstDiscParticipant->processID);
                    DeleteALLDiscoveredFromByGuidPrefix(pstBuiltinDataWriter->pstParticipant, pstBuiltinDiscReader->guid.prefix);
					break;
				}
				pstDiscParticipant = pstDiscParticipant->pNext;
			}
			return;
		}
#endif

        /* ���л��޸���infoDT */
        INITIAL_MEMORYBLOCK(stMemBlockMod, stInfoDest.pcGuidPre, sizeof(GuidPrefix_t));
        SERIALIZE_GUID_PREFIX(pstBuiltinDiscReader->guid.prefix, stMemBlockMod);

        /* ���л��޸�����readId*/
        INITIAL_MEMORYBLOCK(stMemBlockMod, pstBuiltinDataWriter->stHeartbeat.pcReadId, sizeof(EntityId_t));
        SERIALIZE_ENTITYID(pstBuiltinDiscReader->guid.entityId, stMemBlockMod);

        //printf("Nephalem Send HeartBeat %d - %d \n",pstBuiltinDataWriter->stHeartbeat.firstSN.low,pstBuiltinDataWriter->stHeartbeat.lastSN.low);
        //PrintBinData(stMemBlock.base,stMemBlock.writeIndex, "Send HeartBeat");
        /* ���͵���Ӧ��Զ���Ķ��� */
        RTPSSendMsg(&stMemBlock, &pstBuiltinDataWriter->pstParticipant->socketList.sedp_data_sender, pstBuiltinDiscReader->pstUnicastLocator);

        /* ÿ����һ�Σ������ۻ�һ�� */
        pstBuiltinDiscReader->uiSendHeartBeatNum++;
        
        pstBuiltinDiscReader = pstBuiltinDiscReader->pNext;
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: JointHeartbeatMsgSendAssignBuiltinReader
-   ��������: ��װ�������ķ���ָ���ڽ��Ķ���
-   ��    ��: �����ڽ�д������Զ���ڽ��Ķ���
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��װ�������ķ���ָ��Զ���ڽ��Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID JointHeartbeatMsgSendAssignBuiltinReader(RTPSMessageHeader* pstRTPSMsgHeader, BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader* pstBuiltinDiscReader)
{
    MemoryBlock stMemBlock;
    MemoryBlock stMemBlockMod;
    SubMessageHeader  stSubMsgHeader;
    InfoTimestamp     stInfoTS;
    InfoDestination   stInfoDest;
    CHAR buff[NETWORK_BYTE_MAX_SIZE];

    /* �����޻��棬���������� */
    if (NULL == pstBuiltinDataWriter->stHistoryCache.pstHead)
    {
        return;
    }

    /* �����ֽ����ʼ�������淢������ */
    INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);

    /* ��һ�����л�RTPS����ͷ */
    SERIALIZE_RTPS_MESSAGE_HEADER((*pstRTPSMsgHeader), stMemBlock);

    /* ʱ�䱨��ͷ���� */
    stSubMsgHeader.submessageId = INFO_TS;
    stSubMsgHeader.submessageLength = sizeof(Time_t);
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
    stSubMsgHeader.flags[2] = FALSE;   //dataFlag
    stSubMsgHeader.flags[3] = FALSE;   //keyFlag
    stInfoTS.value = GetNowTime();

    /* ���л�ʱ�䱨��ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���л�ʱ���ӱ����� */
    SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

    /* InfoDT���� */
    stSubMsgHeader.submessageId = INFO_DST;
    stSubMsgHeader.submessageLength = 12;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;
    stSubMsgHeader.flags[3] = FALSE;
    stInfoDest.guidPrefix = pstBuiltinDiscReader->guid.prefix;

    /* �ڶ������л��ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���������л��ӱ�����Ϣ�� */
    SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);

    /* ��������ͷ */
    stSubMsgHeader.submessageId = HEARTBEAT;
	if (0 == IS_MACHINE_LITTLE_ENDIAN)
		stSubMsgHeader.flags[0] = FALSE;    //littleEndian
	else
		stSubMsgHeader.flags[0] = TRUE;    //littleEndian
    stSubMsgHeader.flags[1] = FALSE;
    stSubMsgHeader.flags[2] = FALSE;
    stSubMsgHeader.flags[3] = FALSE;

    /* ����������Ϣ�� */
    pstBuiltinDataWriter->stHeartbeat.readerId = pstBuiltinDiscReader->guid.entityId;
    pstBuiltinDataWriter->stHeartbeat.writerId = pstBuiltinDataWriter->guid.entityId;
    pstBuiltinDataWriter->stHeartbeat.firstSN = pstBuiltinDataWriter->stHistoryCache.pstHead->seqNum;
    pstBuiltinDataWriter->stHeartbeat.lastSN = pstBuiltinDataWriter->stHistoryCache.pstTail->seqNum;
    pstBuiltinDataWriter->stHeartbeat.count++;

    /* ���л��û������ӱ���ͷ */
    SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

    /* ���л��û������ӱ�����Ϣ�壬ͬʱ̽���¼readerIdλ�� */
    SERIALIZE_HEARTBEAT(pstBuiltinDataWriter->stHeartbeat, stMemBlock);

    /* �����ӱ�����Ϣ�峤�ȣ�������Ϣͷ */
    stSubMsgHeader.submessageLength = stMemBlock.writeIndex - pstBuiltinDataWriter->stHeartbeat.uiStartIndex;

    /* ���л��޸��ӱ��ĳ��� */
    INITIAL_MEMORYBLOCK(stMemBlockMod, stSubMsgHeader.pcLenFlag, sizeof(USHORT));

    PUT_UNSIGNED_SHORT(stMemBlockMod, stSubMsgHeader.submessageLength);
#ifndef QYDDS_LIVELINESS_QOS
		if (HEARTBEAT_DISCONNECT < pstBuiltinDiscReader->uiSendHeartBeatNum)// || Compare_Duration(usedTime, pstBuiltinDataWriter->pstParticipant->durationEdge))
		{
			DDS_DiscoveredParticipant* pstDiscParticipant = pstBuiltinDataWriter->pstParticipant->pstDiscParticipant;
			while (pstDiscParticipant != NULL)
			{
				if (GuidPrefix_Is_Equal(&pstDiscParticipant->stRTPSMsgHeader.guidPrefix, &pstBuiltinDiscReader->guid.prefix))
				{
					int k[4];
					UINT32 addr = pstDiscParticipant->IPAddr;
					k[0] = addr & 0xFF;
					addr >>= 8;
					k[1] = addr & 0xFF;
					addr >>= 8;
					k[2] = addr & 0xFF;
					addr >>= 8;
					k[3] = addr & 0xFF;
					printf("The remote participant disconnect! IPAddr: %d.%d.%d.%d, ProcessID: %d.\n", k[0], k[1], k[2], k[3], pstDiscParticipant->processID);
                    DeleteALLDiscoveredFromByGuidPrefix(pstBuiltinDataWriter->pstParticipant, pstBuiltinDiscReader->guid.prefix);
					break;
				}
				pstDiscParticipant = pstDiscParticipant->pNext;
			}
			return;
		}
#endif
	/* �Ͽ����ӣ��������Զ����Ϣ */
	

    RTPSSendMsg(&stMemBlock, &pstBuiltinDataWriter->pstParticipant->socketList.sedp_data_sender, pstBuiltinDiscReader->pstUnicastLocator);

    /* ÿ����һ�Σ������ۻ�һ�� */
    pstBuiltinDiscReader->uiSendHeartBeatNum++;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: RecoverBuiltinWriterMissSeqNum
-   ��������: ��ԭ��ʧ��Ƭ�����������ش�
-   ��    ��: �����ڽ�д������Զ���ڽ��Ķ�����ack����
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��ԭ�ڽ���ʧ��Ƭ�����������ش�
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\14       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID RecoverBuiltinWriterMissSeqNum(BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader* pstBuiltinDiscReader, AckNack* pstAckNackMsg)
{
    SequenceNumber_t seqNumMiss = SEQUENCENUMBER_ZERO;
    SequenceNumber_t seqNumOffset = SEQUENCENUMBER_ZERO;
    DDS_ReturnCode_t returnCode = DDS_RETCODE_OK;
    const UINT32  bitStart = 0x80000000;
    UINT32 uiNumbit = 0;

#if defined TG
	SequenceNumber_t missSeqVec[257];
	memset(missSeqVec, 0, sizeof(missSeqVec));
	UINT32 missSeqVecPos = 0;
#endif

    for (uiNumbit = 0; uiNumbit < pstAckNackMsg->readerSNState.numBits; uiNumbit++)
    {
        /* λ�������bitMap��ÿһλֵ������0˵����λ��Ч��ͨ��ƫ���������ȱʧ */
        if (0 < ((bitStart >> (uiNumbit >> 5))&(pstAckNackMsg->readerSNState.bitMap[uiNumbit >> 5])))
        {
            seqNumOffset.low = uiNumbit;
            seqNumMiss = Add_SequenceNumber(pstAckNackMsg->readerSNState.base, seqNumOffset);
#if defined TG
			//�������¼ȱʧ�����
			missSeqVec[missSeqVecPos++] = seqNumMiss;
#endif

#ifndef TG
			/* ȱʧ��Ƭ�ط� */
			returnCode = BuiltinDataWriterSendCacheMsg(pstBuiltinDataWriter, pstBuiltinDiscReader, seqNumMiss);
            if (DDS_RETCODE_OK != returnCode)
            {
                //todo����gap����
                 //DataWriterSendGapMsg(pstBuiltinDataWriter, pstBuiltinDiscReader, seqNumMiss);
//                 printf("gap send 772\n");
            }
#endif 
        }
    }
#if defined TG
	if(0 != missSeqVecPos)
		BuiltinDataWriterSendCacheMsgTogether(pstBuiltinDataWriter, pstBuiltinDiscReader, missSeqVec);
#endif
}

/*---------------------------------------------------------------------------------
-   �� �� ��: BuiltinDataWriterInsertDiscDataReader
-   ��������: �ڽ�д����������Զ���Ķ���
-   ��    ��: �ڽ�д������Զ��guid����ַ
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: �ڽ�д����������Զ���Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL BuiltinDataWriterInsertDiscDataReader(BuiltinDataWriter* pstBuiltinDataWriter, GUID_t* pstGuid, Locator_t* pstUnicastLocator)
{
	BuiltinDiscReader*  pstBuiltinDiscReader = NULL;
    if (NULL != pstBuiltinDataWriter)
    {
        /* ���뾲̬�ڴ洴���ڽ�Զ���Ķ��� */
        pstBuiltinDiscReader = (BuiltinDiscReader*)DDS_STATIC_MALLOC(sizeof(BuiltinDiscReader));
        if (NULL == pstBuiltinDiscReader)
        {
            PrintLog(COMMLOG_ERROR, "BuiltinDiscReader: Failed to create BuiltinDiscReader, Insufficient memory space.\n");
            return FALSE;
        }

        pstBuiltinDiscReader->guid = *pstGuid;
        pstBuiltinDiscReader->stAckNack.count = 0;
        pstBuiltinDiscReader->uiSendHeartBeatNum = 0;
		pstBuiltinDiscReader->livelinessDuration.sec = 0;
		pstBuiltinDiscReader->livelinessDuration.nanosec = 0;
        pstBuiltinDiscReader->stAckNack.readerSNState.base = SEQUENCENUMBER_ZERO;
        pstBuiltinDiscReader->pstUnicastLocator = pstUnicastLocator;

        /* ����ͷ�� */
        LIST_INSERT_HEAD(pstBuiltinDataWriter->pstBuiltinDiscReader, pstBuiltinDiscReader);
    }

    return TRUE;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: GetBuiltinDiscReaderFromBuiltinDataWriterByPrefix
-   ��������: ����Զ���ڽ��Ķ���
-   ��    ��: �ڽ�д������EntityId
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ����Զ���ڽ��Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BuiltinDiscReader* GetBuiltinDiscReaderFromBuiltinDataWriterByPrefix(BuiltinDataWriter* pstBuiltinDataWriter, GuidPrefix_t* pstPrefix)
{
    BuiltinDiscReader* pstBuiltinDiscReader = pstBuiltinDataWriter->pstBuiltinDiscReader;

    while (NULL != pstBuiltinDiscReader)
    {
        if (GuidPrefix_Is_Equal(&pstBuiltinDiscReader->guid.prefix, pstPrefix))
        {
            return pstBuiltinDiscReader;
        }
        pstBuiltinDiscReader = pstBuiltinDiscReader->pNext;
    }

    return NULL;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DataWriterSendCacheMsg
-   ��������: �ڽ����������ش�
-   ��    ��: д������Զ���Ķ�����ȱʧseqNum
-   ��    ��: DDS_RETCODE_OK��DDS_RETCODE_ERROR
-   ȫ�ֱ���:
-   ע    ��: �ڽ����������ش�
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\20       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_ReturnCode_t BuiltinDataWriterSendCacheMsg(BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader* pstBuiltinDiscReader, SequenceNumber_t seqNum)
{
	HistoryData* pstHistoryData = NULL;
    /* �����޻������ݻ���ack�����޶�ʧ��ֱ�ӷ��ز����� */
    if (NULL == pstBuiltinDataWriter->stHistoryCache.pstHead)
    {
        return DDS_RETCODE_ERROR;
    }

    /* ȱʧ���Ĳ��ڻ������䣬ֱ�ӷ��ز����� */
    if (ISLESSTHEN_SEQUENCENUMBER(pstBuiltinDataWriter->stHistoryCache.pstTail->seqNum, seqNum) || ISLESSTHEN_SEQUENCENUMBER(seqNum, pstBuiltinDataWriter->stHistoryCache.pstHead->seqNum))
    {
        return DDS_RETCODE_ERROR;
    }

    pstHistoryData = pstBuiltinDataWriter->stHistoryCache.pstHead;

    while (NULL != pstHistoryData)
    {
        if (ISEQUAL_SEQUENCENUMBER(pstHistoryData->seqNum, seqNum))
        {
            /* ��װ�ش����Ĳ����� */
            JointSEDPDataMsgSendBuiltinReader(&pstBuiltinDataWriter->pstParticipant->stRTPSMsgHeader, pstHistoryData, pstBuiltinDataWriter, pstBuiltinDiscReader);

            return DDS_RETCODE_OK;
        }
        pstHistoryData = pstHistoryData->pNext;
    }

    return DDS_RETCODE_ERROR;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: ModifySendDiscReaderHeartbeat
-   ��������: �޸���������
-   ��    ��: �ڽ�д��������־λ
-   ��    ��: 
-   ȫ�ֱ���:
-   ע    ��: �޸���������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\27       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID ModifySendDiscReaderHeartbeat(BuiltinDataWriter* pstBuiltinDataWriter, BOOL bFlag)
{
    BuiltinDiscReader* pstBuiltinDiscReader = pstBuiltinDataWriter->pstBuiltinDiscReader;
    while (NULL != pstBuiltinDiscReader)
    {
        pstBuiltinDiscReader = pstBuiltinDiscReader->pNext;
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DeleteBuiltinDiscReaderFromBuiltinWriterByGuidPrefix
-   ��������: ɾ��Զ���ڽ��Ķ���
-   ��    ��: ���⡢Guid
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ������ɾ��ָ��Զ���ڽ��Ķ���
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\30       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DeleteBuiltinDiscReaderFromBuiltinWriterByGuidPrefix(BuiltinDataWriter* pstBuiltinDataWriter, GuidPrefix_t*  pstPrefix)
{
    BuiltinDiscReader*  pstPrevBuiltinDiscReader = NULL;
    BuiltinDiscReader*  pstBuiltinDiscReader = pstBuiltinDataWriter->pstBuiltinDiscReader;

    while (NULL != pstBuiltinDiscReader)
    {
        if (GuidPrefix_Is_Equal(&pstBuiltinDiscReader->guid.prefix, pstPrefix))
        {
            if (!pstPrevBuiltinDiscReader)
            {
                pstBuiltinDiscReader = pstBuiltinDiscReader->pNext;
                DDS_STATIC_FREE(pstBuiltinDataWriter->pstBuiltinDiscReader);
                pstBuiltinDataWriter->pstBuiltinDiscReader = pstBuiltinDiscReader;
            }
            else
            {
                pstPrevBuiltinDiscReader->pNext = pstBuiltinDiscReader->pNext;
                DDS_STATIC_FREE(pstBuiltinDiscReader);
                pstBuiltinDiscReader = pstPrevBuiltinDiscReader->pNext;
            }
        }
        else
        {
            pstPrevBuiltinDiscReader = pstBuiltinDiscReader;
            pstBuiltinDiscReader = pstBuiltinDiscReader->pNext;
        }
    }
}

DDS_ReturnCode_t BuiltinDataWriterSendCacheMsgTogether(BuiltinDataWriter* pstBuiltinDataWriter, BuiltinDiscReader* pstBuiltinDiscReader, SequenceNumber_t* seqNum)
{
	MemoryBlock       stMemBlock;
	MemoryBlock       stMemBlockMod;
	SubMessageHeader  stSubMsgHeader;
	ParameterHead     stParamHead;
	InfoTimestamp     stInfoTS;
	InfoDestination   stInfoDest;
	Data              stData;
	CHAR              buff[NETWORK_BYTE_MAX_SIZE];
	UINT32 Flags = 0;
	UINT32 uiSeqNum = 0;
	HistoryData* pstHistoryData = NULL;

	// seqNum�����¼��Ҫ����ش������к�
	while (1) // ����seqNum�����¼�����Ϊ��ʱ���Ż��˳��������������Ϊ257����ֻ����0~255�����ݣ��±�Ϊ256��Ϊ�գ�
	{
		/* seqNumΪ0��û��������Ҫ�ش� */
		if (0 == seqNum[uiSeqNum].high && 0 == seqNum[uiSeqNum].low)
			return DDS_RETCODE_OK;

		/* �ϰ��Ľṹ�� INFO_TS + INFO_DST + n��Data�ӱ��ġ�������ƴ��ǰ��ı��ģ�����ѭ��ƴ��Data�ӱ��� */	
		INITIAL_MEMORYBLOCK(stMemBlock, buff, NETWORK_BYTE_MAX_SIZE);/* �����ֽ����ʼ�������淢������ */

		/* ��һ�����л�RTPS����ͷ */
		SERIALIZE_RTPS_MESSAGE_HEADER(pstBuiltinDataWriter->pstParticipant->stRTPSMsgHeader, stMemBlock);

		/* ʱ�䱨��ͷ���� */
		stSubMsgHeader.submessageId = INFO_TS;
		stSubMsgHeader.submessageLength = sizeof(Time_t);
		if (0 == IS_MACHINE_LITTLE_ENDIAN)
			stSubMsgHeader.flags[0] = FALSE;    //littleEndian
		else
			stSubMsgHeader.flags[0] = TRUE;    //littleEndian
		stSubMsgHeader.flags[1] = FALSE;   //inlineQosFlag
		stSubMsgHeader.flags[2] = FALSE;   //dataFlag
		stSubMsgHeader.flags[3] = FALSE;   //keyFlag
		stInfoTS.value = GetNowTime();

		/* ���л�ʱ�䱨��ͷ */
		SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

		/* ���л�ʱ���ӱ����� */
		SERIALIZE_INFOTIMESTAMP(stInfoTS, stMemBlock);

		stSubMsgHeader.submessageId = INFO_DST;
		stSubMsgHeader.submessageLength = 12;
		if (0 == IS_MACHINE_LITTLE_ENDIAN)
			stSubMsgHeader.flags[0] = FALSE;    //littleEndian
		else
			stSubMsgHeader.flags[0] = TRUE;    //littleEndian
		stSubMsgHeader.flags[1] = FALSE;
		stSubMsgHeader.flags[2] = FALSE;
		stSubMsgHeader.flags[3] = FALSE;
		stInfoDest.guidPrefix = pstBuiltinDiscReader->guid.prefix;

		/* �ڶ������л��ӱ���ͷ */
		SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

		/* ���������л��ӱ�����Ϣ�� */
		SERIALIZE_INFODESTINATION(stInfoDest, stMemBlock);
		while (uiSeqNum < 256) /* ��uiSeqNum�±굽256ʱ������װ�����ݳ���64KB���˳�����ѭ�� */
		{
			if (0 == seqNum[uiSeqNum].high && 0 == seqNum[uiSeqNum].low)
				break;

			pstHistoryData = pstBuiltinDataWriter->stHistoryCache.pstHead;
			while (NULL != pstHistoryData)
			{
				// ���ҵ����к�
				if (ISEQUAL_SEQUENCENUMBER(pstHistoryData->seqNum, seqNum[uiSeqNum]))
					break;

				pstHistoryData = pstHistoryData->pNext;
			}

			if (NULL == pstHistoryData)
			{
				uiSeqNum++;
				continue;
			}		

			/* data����ͷ���� */
			stSubMsgHeader.submessageId = DATA;

			/* Nephlaem ����Data���ĳ��� */
			stSubMsgHeader.submessageLength =
				sizeof(stData.extraFlags) +
				sizeof(stData.octetsToInlineQos) +
				sizeof(stData.readerId) +
				sizeof(stData.writerId) +
				sizeof(stData.writerSN);

			if (pstHistoryData->bCancel)
			{
				stSubMsgHeader.submessageLength +=
					sizeof(unsigned short) * 2 + 16 +
					sizeof(unsigned short) * 2 + sizeof(unsigned int) +
					sizeof(unsigned short) * 2;
			}
			else
			{
				stSubMsgHeader.submessageLength += sizeof(unsigned short) * 2 + pstHistoryData->uiDataLen;
			}

			// �ռ䲻����װ���ݣ�������װ 
			if (stMemBlock.writeIndex + stSubMsgHeader.submessageLength +  4 >= NETWORK_BYTE_MAX_SIZE)
				break;

			if (0 == IS_MACHINE_LITTLE_ENDIAN)
				stSubMsgHeader.flags[0] = FALSE;    //littleEndian
			else
				stSubMsgHeader.flags[0] = TRUE;    //littleEndian
			stSubMsgHeader.flags[1] = FALSE;    //inlineQosFlag
			stSubMsgHeader.flags[2] = (pstHistoryData->data) ? TRUE : FALSE;  //dataPresent
			stSubMsgHeader.flags[3] = FALSE;   //keyFlag

			/* data���������� */
			stData.extraFlags[0] = 0x00;
			stData.extraFlags[1] = 0x00;
			stData.octetsToInlineQos = 0x0010;
			stData.readerId = pstBuiltinDiscReader->guid.entityId;
			stData.writerId = pstBuiltinDataWriter->guid.entityId;
			stData.writerSN = pstHistoryData->seqNum;
			if (IS_MACHINE_LITTLE_ENDIAN)
				stData.serializedPayload.m_cdrType = PL_CDR_LE;   //littleEndian
			else
				stData.serializedPayload.m_cdrType = PL_CDR_BE;    //littleEndian
			stData.serializedPayload.m_option = 0x0000;

			/* ���л�����QOSȡ���������� */
			if (pstHistoryData->bCancel)
			{
				stSubMsgHeader.flags[1] = TRUE;    //inlineQosFlag
			}

			/* �ڶ������л��ӱ���ͷ */
			SERIALIZE_SUBMESSAGEHEADER(stSubMsgHeader, stMemBlock);

			/* ���������л��ӱ�����Ϣ�� */
			SERIALIZE_DATA(stData, stMemBlock);

			/* ���л�����QOSȡ���������� */
			if (pstHistoryData->bCancel)
			{
				/* ���л������б� */
				stParamHead.paramType = PID_KEY_HASH;
				stParamHead.length = sizeof(GUID_t);
				SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);
				SERIALIZE_GUID(pstHistoryData->stGuid, stMemBlock);

				/* ���л������б� */
				Flags = 0x01000000;
				stParamHead.paramType = PID_STATUS_INFO;
				stParamHead.length = sizeof(UINT32);
				SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);
				PUT_UNSIGNED_INT(stMemBlock, Flags);

				/* ���л������б��β */
				stParamHead.paramType = PID_SENTINEL;
				stParamHead.length = 0;
				SERIALIZE_PARAM_HEAD(stParamHead, stMemBlock);
			}

			/* ���л��û����� */
			if (pstHistoryData->data)
			{
				/* ���л�����Ч�غ� */
				SERIALIZE_SERIALIZEDDATA(stData.serializedPayload, stMemBlock);

				/* ���л�д������Ϣ */
				PUT_BYTE_ARRAY(stMemBlock, pstHistoryData->data, pstHistoryData->uiDataLen);
			}

			uiSeqNum++;

		}

		RTPSSendMsg(&stMemBlock, &pstBuiltinDataWriter->pstParticipant->socketList.sedp_data_sender, pstBuiltinDiscReader->pstUnicastLocator);

	}

	return DDS_RETCODE_OK;
}
