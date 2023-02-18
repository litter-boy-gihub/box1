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
*  @file     RTPSDiscovery.h
*  @brief    RTPSDiscovery
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
RTPSDiscovery.h
RTPS�淶����Discovery Module��صĹ��ܽṹ������SPDP��SEDP��
********************************************************************/
#ifndef HIPERD_DISCOVERY_H
#define HIPERD_DISCOVERY_H

typedef struct RTPSDiscovery
{
    /* ����˿ںŵĲ������μ��μ�RTPS�淶��P197   */
    /* �˿ں��������ֽڱ�ʾ                       */
    unsigned short pb;
    unsigned short dg;
    unsigned short pg;
    unsigned short d0;
    unsigned short d1;
    unsigned short d2;
    unsigned short d3;

    const char*     groupAddr           ; /* Ĭ���鲥��ַ                             */
    unsigned int    rootParticipantId   ; /* Ψһ�����Participant                    */
    char            unicastAddr[16]     ; /* ���ڵ��IP��ַ������ͨ��������ͼ�������� */
  //  GuidPrefix_t    guidPrefix          ; /* ϵͳ��ʼ���׶����ɵ�GUID_Prefix          */

}RTPSDiscovery;

extern RTPSDiscovery g_rtps_discovery_info;

extern BOOL InitializeDiscoveryInfo(const char* ipAddr, unsigned int rootParticipantId);

#endif
