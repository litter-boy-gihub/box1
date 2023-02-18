#include "../include/GlobalDefine.h"

/**
 * GuidPrefix_tΨһ��ʶһ��DDS���е�Participant
 */
const UINT8 GUIDPREFIX_UNKNOWN_VALUE[12] =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// RTPS����GuidPrefix_t����ֵ
const GuidPrefix_t GUIDPREFIX_UNKNOWN = {{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};


/**
 * Ԥ�����Entity_tֵ,RTPSЭ����Ҫ�õ�������Entity
 */
const EntityId_t ENTITYID_INVALID = { 0xff, 0xff, 0xff, 0xff };   /* ��ЧentityID */
const EntityId_t ENTITYID_UNKNOWN = {0x00, 0x00, 0x00, 0x00};    // RTPS����EntityId_t����ֵ
const EntityId_t ENTITYID_PARTICIPANT = {0x00, 0x00, 0x01, 0xc1};    // Participant
const EntityId_t ENTITYID_SEDP_BUILTIN_TOPIC_WRITER = {0x00, 0x00, 0x02, 0xc2};    // SEDPBuildinTopicWriter
const EntityId_t ENTITYID_SEDP_BUILTIN_TOPIC_READER = {0x00, 0x00, 0x02, 0xc7};    // SEDPbuiltinTopicReader
const EntityId_t ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER = {0x00, 0x00, 0x03, 0xc2};    // SEDPbuiltinPublicationsWriter
const EntityId_t ENTITYID_SEDP_BUILTIN_PUBLICATIONS_READER = {0x00, 0x00, 0x03, 0xc7};    // SEDPbuiltinPublicationsReader
const EntityId_t ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_WRITER = {0x00, 0x00, 0x04, 0xc2};    // SEDPbuiltinSubscriptionsWriter
const EntityId_t ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_READER = {0x00, 0x00, 0x04, 0xc7};    // SEDPbuiltinSubscriptionsReader
const EntityId_t ENTITYID_SPDP_BUILTIN_PARTICIPANT_WRITER = {0x00, 0x01, 0x00, 0xc2};    // SPDPbuiltinParticipantWriter
const EntityId_t ENTITYID_SPDP_BUILTIN_PARTICIPANT_READER = {0x00, 0x01, 0x00, 0xc7};    // SPDPbuiltinSdpParticipantReader
const EntityId_t ENTITYID_P2P_BUILTIN_PARTICIPANT_MESSAGE_WRITER = {0x00, 0x02, 0x00, 0xc2};    // BuiltinParticipantMessageWriter
const EntityId_t ENTITYID_P2P_BUILTIN_PARTICIPANT_MESSAGE_READER = {0x00, 0x02, 0x00, 0xc7};    // BuiltinParticipantMessageReader

/**
 * ������GUID_tֵ
 */
const GUID_t GUID_UNKNOWN = {{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, 
{0x00, 0x00, 0x00, 0x00}};    // RTPS����GUID_t����





BOOL IsBuiltinEndpoint(const EntityId_t entityid)
{
    return (entityid.entityKind & 0xc0) == 0xc0;
}

BOOL GuidPrefix_Is_Equal(const GuidPrefix_t * leftGuidPrefix, const GuidPrefix_t * rightGuidPrefix)
{
    int i;
    for (i = 0; i < 12; i++)
    {
        if (leftGuidPrefix->value[i] != rightGuidPrefix->value[i])
            return FALSE;
    }
    return TRUE;
}

BOOL EntityId_Is_Equal(const EntityId_t* lefEntityId, const EntityId_t* rightEntityId)
{
    /* ���ǵ��ṹ��������� ����ֱ���ж��ڴ��Ƿ���� */
    int i;

    if (lefEntityId->entityKind != rightEntityId->entityKind)
        return FALSE;
    for (i = 0; i < 3; i++)
    {
        if (lefEntityId->entityKey[i] != rightEntityId->entityKey[i])
            return FALSE;
    }
    return TRUE;
}

BOOL Guid_Is_Equal(const GUID_t * leftGuid, const GUID_t * rightGuid)
{
	/* ���ǵ��ṹ��������� ����ֱ���ж��ڴ��Ƿ���� */
	return (EntityId_Is_Equal(&leftGuid->entityId, &rightGuid->entityId) && GuidPrefix_Is_Equal(&leftGuid->prefix,&rightGuid->prefix));
}

BOOL Guid_Is_Bigger(const GUID_t * leftGuid, const GUID_t * rightGuid)
{
    /* Nephalem �ж�GUIDֵ�Ƿ����*/
    int i;
    for (i = 0; i < 12; i++)
    {
        if (leftGuid->prefix.value[i] > rightGuid->prefix.value[i])
            return TRUE;
    }

    for (i = 0; i < 3; i++)
    {
        if (leftGuid->entityId.entityKey[i] > rightGuid->entityId.entityKey[i])
            return TRUE;
    }

    if (leftGuid->entityId.entityKind > rightGuid->entityId.entityKind)
        return TRUE;

    return FALSE;
}