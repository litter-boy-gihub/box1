/*********************************************************************************
*  OpenST Basic tool library
*  Copyright (C) 2019 QiYuan
*
*
*  This program is security software
*  without permission you cannot use or distribute the software
*  otherwise, legal liability will be investigated
*
*  If you want to use the software, please contact the authorities to obtain a license
*  < http://www.iamqy.com.cn >.
*
*  @file     Guid_t.h
*  @brief    Guid_t
*  Details.
*
*  @author   Hunter
*  @email    1028251654@qq.com
*  @version  1.0
*  @date     2019/08/08
*  @license
*
*--------------------------------------------------------------------------------
*  Remark         : Description
*--------------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>  | <operation> | <Description>
*--------------------------------------------------------------------------------
*  2019/08/08 | 1.0       | Hunter    | Create file |
*--------------------------------------------------------------------------------
*
*********************************************************************************/

/********************************************************************
Guid_t.h
RTPS�淶��Guid_t��Ӧ���������ͼ���ش������
********************************************************************/

#ifndef HIPERD_GUID_T_H
#define HIPERD_GUID_T_H


/* GuidPrefix_tΨһ��ʶһ��DDS���е�Participant */
extern const EX_UINT8 GUIDPREFIX_UNKNOWN_VALUE[12];

/* GuidPrefix_t �ṹ�����л���Parameterʱռ�ÿռ�Ĵ�С ��Ҫ�����ֽڶ���*/
#define SIZE_OF_GUIDPREFIX 12
#define GUID_SIZE 16
typedef struct GuidPrefix_t
{
    /* GuidPrefix_tΪ12����λ�ֽڵ�GUIDǰ׺��Ψһ��ʶһ��Participant */    
	EX_UINT8 value[SIZE_OF_GUIDPREFIX];
}GuidPrefix_t;

#define ISEQUAL_GUIDPREFIX(left, right) (memcmp(left.value, right.value, SIZE_OF_GUIDPREFIX) == 0)

/* RTPS����GuidPrefix_t����ֵ */
extern const GuidPrefix_t GUIDPREFIX_UNKNOWN;    

/* EntityId_tΨһ��ʶĳһ��Participant�ڵ�Entity */
typedef struct EntityId_t
{
    // �ڱ�֤Ψһ�ԣ�����Ԥ�����ֵ������������ȡֵ
    EX_UINT8 entityKey[3];
    /**
     * entityKind���������λ����������Entity������
     *     0x00: �û�����Entity��RTPSЭ�鶨�壬�����û�ʵ����
     *     0x11: ����Entity��RTPSЭ�鶨�壬�Զ�ʵ����
     *     0x01: �����ر��Entity
     * ����λ�����б��Table9.1(P169)
     */
    EX_UINT8 entityKind;
}EntityId_t;

#define ENTITYID_IS_EQUAL(leftEntity,rightEntity)                   \
        ( (leftEntity.entityKey[0] == rightEntity.entityKey[0]) &&  \
          (leftEntity.entityKey[1] == rightEntity.entityKey[1]) &&  \
          (leftEntity.entityKey[2] == rightEntity.entityKey[2]) &&  \
          (leftEntity.entityKind == rightEntity.entityKind))
/* Ԥ�����Entity_tֵ,RTPSЭ����Ҫ�õ�������Entity */
extern const EntityId_t ENTITYID_INVALID                                ; /* ��Ч��EntityID                  */
extern const EntityId_t ENTITYID_UNKNOWN                                ; /* RTPS����EntityId_t����ֵ        */
extern const EntityId_t ENTITYID_PARTICIPANT                            ; /* Participant                     */
extern const EntityId_t ENTITYID_SEDP_BUILTIN_TOPIC_WRITER              ; /* SEDPBuildinTopicWriter          */
extern const EntityId_t ENTITYID_SEDP_BUILTIN_TOPIC_READER              ; /* SEDPbuiltinTopicReader          */
extern const EntityId_t ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER       ; /* SEDPbuiltinPublicationsWriter   */
extern const EntityId_t ENTITYID_SEDP_BUILTIN_PUBLICATIONS_READER       ; /* SEDPbuiltinPublicationsReader   */
extern const EntityId_t ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_WRITER      ; /* SEDPbuiltinSubscriptionsWriter  */
extern const EntityId_t ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_READER      ; /* SEDPbuiltinSubscriptionsReader  */
extern const EntityId_t ENTITYID_SPDP_BUILTIN_PARTICIPANT_WRITER        ; /* SPDPbuiltinParticipantWriter    */
extern const EntityId_t ENTITYID_SPDP_BUILTIN_PARTICIPANT_READER        ; /* SPDPbuiltinSdpParticipantReader */
extern const EntityId_t ENTITYID_P2P_BUILTIN_PARTICIPANT_MESSAGE_WRITER ; /* BuiltinParticipantMessageWriter */
extern const EntityId_t ENTITYID_P2P_BUILTIN_PARTICIPANT_MESSAGE_READER ; /* BuiltinParticipantMessageReader */

/* ��һ��DDS����ÿһ��Entity����һ��GUID������Ψһ��ʶ���ɶ�Ԫ��<prefix��entitId>���ɣ����ͷֱ�ΪGuidPrefix_t��EntityId_t���� */
typedef struct GUID_t
{
    GuidPrefix_t    prefix  ; /* prefixΨһ��ʶParticipant                   */
    EntityId_t      entityId; /* entityIdΨһ��ʶĳһ��Participant�ڵ�Entity */
}GUID_t;

/* �ж��Ƿ�Ϊ�ڽ���entity */
extern BOOL IsBuiltinEndpoint(const EntityId_t entityid);

/* �ж��Ƿ���� Ϊ�˷�ֹif�жϹ������ú������õ���ʽʵ�� */
extern BOOL Guid_Is_Equal(const GUID_t * leftGuid, const GUID_t * rightGuid);

extern BOOL GuidPrefix_Is_Equal(const GuidPrefix_t * leftGuidPrefix, const GuidPrefix_t * rightGuidPrefix);

extern BOOL EntityId_Is_Equal(const EntityId_t* lefEntityId, const EntityId_t* rightEntityId);

extern BOOL Guid_Is_Bigger(const GUID_t * leftGuid, const GUID_t * rightGuid);

/*
 ����Ϊ�ýṹ����빤�ߺ�
*/
#define SERIALIZE_GUID_PREFIX(prefix , memBlock) PUT_BYTE_ARRAY(memBlock,prefix.value,SIZE_OF_GUIDPREFIX)

#define DESERIALIZE_GUID_PREFIX(prefix , memBlock)    GET_BYTE_ARRAY(memBlock,prefix.value,SIZE_OF_GUIDPREFIX)

#define SERIALIZE_ENTITYID(entityId , memBlock)         \
    do                                                  \
    {                                                   \
        PUT_BYTE_ARRAY(memBlock, entityId.entityKey, 3) \
        PUT_BYTE(memBlock, entityId.entityKind)         \
    } while (0);

#define DESERIALIZE_ENTITYID(entityId , memBlock)       \
    do                                                  \
    {                                                   \
        GET_BYTE_ARRAY(memBlock, entityId.entityKey, 3) \
        GET_BYTE(memBlock, entityId.entityKind)         \
    } while (0);

#define SERIALIZE_GUID(guid , memBlock)                 \
    do                                                  \
    {                                                   \
        SERIALIZE_GUID_PREFIX(guid.prefix , memBlock)   \
        SERIALIZE_ENTITYID(guid.entityId , memBlock)    \
    } while (0);

#define DESERIALIZE_GUID(guid , memBlock)               \
    do                                                  \
    {                                                   \
        DESERIALIZE_GUID_PREFIX(guid.prefix , memBlock) \
        DESERIALIZE_ENTITYID(guid.entityId , memBlock)  \
    } while (0);

/**
 * ������GUID_tֵ
 */
extern const GUID_t GUID_UNKNOWN;    // RTPS����GUID_t����

#endif