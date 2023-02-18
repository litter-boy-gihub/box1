#include "../include/GlobalDefine.h"

const EX_UINT32 ENTITY_KEY_MASK = 0x00ffffff;
const EX_UINT32 ENTITY_KIND_MASK = 0xff000000;
const EX_UINT32 AUTO_ENTITYID = 0x00ffffff;

/* entity���� */
const EX_UINT8 ENTITYKIND_TOPIC             = 0x0a;
const EX_UINT8 ENTITYKIND_WRITER_NO_KEY     = 0x03;
const EX_UINT8 ENTITYKIND_READER_NO_KEY     = 0x04;
const EX_UINT8 ENTITYKIND_WRITER_WITH_KEY   = 0x02;
const EX_UINT8 ENTITYKIND_READER_WITH_KEY   = 0x07;

const EX_UINT32 GEN_INVALID = 0xffffffff;

static UINT32 internal_id = 1;

static UINT16 kind_id = 0;

/* ����ǰ׺ */
RETSTATUS GenPrefix(DDS_DomainParticipant*  pstDomainParticipant, AUTO_GEN_PREFIX_KIND gen_kind)
{
	int listSizeRT = 0;
	BoundedString256 ipList;
	unsigned char prefix[12];
    EX_UINT32 t_nRtpsHostId, t_nRtpsAppId, t_nRtpsInstanceId, t_nInternalid;
    t_nRtpsHostId = 0;
    t_nRtpsAppId = 0;
    t_nRtpsInstanceId = 0;
    t_nInternalid = 0;

    t_nInternalid = gen_internal_id();
    if (AUTO_GUID_FROM_IP == gen_kind)
    {
        // ��ȡ����IP

        listSizeRT = getHostIPv4AddrList(&ipList);
        if (listSizeRT <= 0)
        {
            /* TODO ��ȡ����ipʧ�� */;
            return STA_ERROR;
        }
        /* Ĭ��ʹ��0������ */
        t_nRtpsHostId = getLongFromIPAddr(ipList.value);
        // ��ȡ������ID
        t_nRtpsAppId = pstDomainParticipant->processId;
#if defined _TM1 || defined _TM3
        t_nRtpsHostId = g_commFCConfig.m_thisNodeFCPort;
        pstDomainParticipant->ipAddr = t_nRtpsHostId; //������3����FC��ַ
#else
		pstDomainParticipant->ipAddr = htonl(t_nRtpsHostId);
#endif
/*        getNetMask(pstDomainParticipant, ipList.value);*/
        // ��������ֵ
        t_nRtpsInstanceId = t_nInternalid;
    }
    // ���ݲ�������ǰ׺

    getStrFromLong(
        t_nRtpsHostId,
        &prefix[0], &prefix[1], &prefix[2], &prefix[3]);///* 0-3�����ַ */
    getStrFromLong(
        t_nRtpsAppId,
        &prefix[4], &prefix[5], &prefix[6], &prefix[7]);// 4-7����id
    getStrFromLong(
        t_nRtpsInstanceId,
        &prefix[8], &prefix[9], &prefix[10], &prefix[11]);// 8-11�����ߵ���ʵ��id

	memcpy(&pstDomainParticipant->stRTPSMsgHeader.guidPrefix.value, prefix, SIZE_OF_GUIDPREFIX);
    return STA_OK;
}

/* ����GUID */
RETSTATUS GenGuid(GUID_t * guid_t, const GuidPrefix_t*  guidPrefix, UINT8 kind)
{
    kind_id++;
    guid_t->prefix = *guidPrefix;
    guid_t->entityId.entityKind = kind;
    memset(guid_t->entityId.entityKey, 0, 3);
    memcpy(guid_t->entityId.entityKey, &kind_id, sizeof(UINT16));

    return STA_OK;
}

EX_UINT32 gen_internal_id()
{
    return internal_id++;
}
