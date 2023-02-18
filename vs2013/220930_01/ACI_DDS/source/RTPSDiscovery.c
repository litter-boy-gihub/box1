#include "../include/GlobalDefine.h"

RTPSDiscovery g_rtps_discovery_info = {7400 , 250 , 2 , 0 , 10 , 1 , 11 , "239.255.0.1" , 0};

BOOL InitializeDiscoveryInfo(const char* ipAddr , unsigned int rootParticipantId)
{
    g_rtps_discovery_info.rootParticipantId = rootParticipantId;
//#if defined(_WIN32)
//    strcpy_s(g_rtps_discovery_info.unicastAddr , 16 , ipAddr);
//#elif defined(__linux__)
//    printf("%d\n",sizeof(g_rtps_discovery_info.unicastAddr));
//    strncpy(g_rtps_discovery_info.unicastAddr, ipAddr, 16);
//#elif defined _VXWORKS
//    memcpy(g_rtps_discovery_info.unicastAddr, ipAddr, 16);
//#endif
    return TRUE;
}

