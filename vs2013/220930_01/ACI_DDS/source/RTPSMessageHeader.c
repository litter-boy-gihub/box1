#include "../include/GlobalDefine.h"

const int ProtocalId_RTPS = 0x53505452;

/*
* ������ProtocolId_tֵ
*/
const ProtocolId_t PROTOCOL_RTPS_LITTLE_ENDIAN = 0x53505452;    // С��RTPSЭ��
const ProtocolId_t PROTOCOL_RTPX_LITTLE_ENDIAN = 0x58505452;    // С��RTPXЭ��
const ProtocolId_t PROTOCOL_RTPS_BIG_ENDIAN = 0x52545053;    // ���RTPSЭ��
const ProtocolId_t PROTOCOL_RTPX_BIG_ENDIAN = 0x52505458;    // ���RTPXЭ��

VOID InitRTPSMessageHeader(RTPSMessageHeader* pstRTPSMsgHeader)
{
    pstRTPSMsgHeader->protocolId      = PROTOCOL_RTPS_LITTLE_ENDIAN;
    pstRTPSMsgHeader->vendorId        = VENDORID_EVO;
    pstRTPSMsgHeader->protocolVersion = PROTOCOLVERSION;
    //pstRTPSMsgHeader->guidPrefix      = g_rtps_discovery_info.guidPrefix;
}