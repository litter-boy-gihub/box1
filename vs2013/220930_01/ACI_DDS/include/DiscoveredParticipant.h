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
*  @file     DiscoveredParticipant.h
*  @brief    DiscoveredParticipant
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

#ifndef HIPERD_DISCOVEREDPARTICIPANT_H
#define HIPERD_DISCOVEREDPARTICIPANT_H

typedef struct _DiscoveredParticipant
{
	/************************ ������Ϣ ****************************/
    RTPSMessageHeader             stRTPSMsgHeader;
    ParticipantData               dcpsParticipantData;
    ParticipantProxy              rtpsParticipantProxy;
	/************************ ��Ϊ��Ϣ ****************************/
	DDS_MUTEX					  threadMutex;
#ifdef  STATIC_DISCOVERY
	BOOL						  bOnlyStaticMathc;
#endif
    Duration_t                    leaseDuration;			//Զ�����õ��������
	Duration_t					  lastHeartBeat;			//Զ����һ��������ʱ��
	UINT32						  livelinessHeartMissNum;			//Զ������û�յ������Ĵ���

	/*********************** ����������Ϣ ************************/
	UINT32						  IPAddr;//IP��ַ
//  UINT32					      netMask;//��������
	int							  processID;//����ID

    struct _DiscoveredParticipant* pNext;
}DDS_DiscoveredParticipant;

extern VOID DDS_DiscoveredParticipant_Init(DDS_DiscoveredParticipant* pstSPDPDiscoveredParticipant);

extern VOID DDS_DiscoveredParticipant_Uninit(DDS_DiscoveredParticipant* pstSPDPDiscoveredParticipant);

#endif
