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
*  @file     RTPSMessageHeader.h
*  @brief    RTPSMessageHeader
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
RTPSMessageHeader.h
RTPS�淶��������Ϣ��Header�ṹ
********************************************************************/
#ifndef HIPERD_RTPSMESSAGE_HEADER_H
#define HIPERD_RTPSMESSAGE_HEADER_H


typedef EX_INT32 ProtocolId_t;

/*
 * ������ProtocolId_tֵ
 */
extern const ProtocolId_t ProtocalId_RTPS               ; /* Э����Ĭ��RTPS�汾 */
extern const ProtocolId_t PROTOCOL_RTPS_LITTLE_ENDIAN   ; /* С��RTPSЭ��       */
extern const ProtocolId_t PROTOCOL_RTPX_LITTLE_ENDIAN   ; /* С��RTPXЭ��       */
extern const ProtocolId_t PROTOCOL_RTPS_BIG_ENDIAN      ; /* ���RTPSЭ��       */
extern const ProtocolId_t PROTOCOL_RTPX_BIG_ENDIAN      ; /* ���RTPXЭ��       */

#define RTPS_HEADER_SIZE 20

typedef struct _RTPSMessageHeader
{
    ProtocolId_t        protocolId      ; /* Э������     */
    ProtocolVersion_t   protocolVersion ; /* Э��汾     */
    VendorId_t          vendorId        ; /* ��Ӧ����ID   */
    GuidPrefix_t        guidPrefix      ; /* ��Ϣ����ǰ׺ */
}RTPSMessageHeader;

/*
 ����Ϊ�ýṹ����빤�ߺ�
*/

#define SERIALIZE_RTPS_MESSAGE_HEADER(header , memBlock)                \
    do                                                                  \
    {                                                                   \
        PUT_UNSIGNED_INT(memBlock, header.protocolId)                   \
        SERIALIZE_PROTOCOL_VERSION(header.protocolVersion, memBlock)    \
        SERIALIZE_VENDORID(header.vendorId, memBlock)                   \
        SERIALIZE_GUID_PREFIX(header.guidPrefix, memBlock)            \
    } while (0);

#define DESERIALIZE_RTPS_MESSAGE_HEADER(header , memBlock)              \
    do                                                                  \
    {                                                                   \
        GET_UNSIGNED_INT(memBlock, header.protocolId,0)                   \
        DESERIALIZE_PROTOCOL_VERSION(header.protocolVersion, memBlock)  \
        DESERIALIZE_VENDORID(header.vendorId, memBlock)                 \
        DESERIALIZE_GUID_PREFIX(header.guidPrefix, memBlock)          \
    } while (0);


extern VOID InitRTPSMessageHeader(RTPSMessageHeader* pstRTPSMsgHeader);

#endif