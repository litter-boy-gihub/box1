/*********************************************************************************
*  OpenST Basic tool library
*  Copyright (C) 2019 QiYuan
*
*
*  This program is security software
*  without permission you cannot MEMUSE or distribute the software
*  otherwise, legal liability will be investigated
*
*  If you want to MEMUSE the software, please contact the authorities to obtain a license
*  < http://www.iamqy.com.cn >.
*
*  @file     ParticipantData.h
*  @brief    
*  Details.
*
*  @author   Hunter
*  @email    1028251654@qq.com
*  @version  1.0
*  @date     2019/07/29
*  @license  GNU General Public License (GPL)
*
*--------------------------------------------------------------------------------
*  Remark         : Description
*--------------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>  | <operation> | <Description>
*--------------------------------------------------------------------------------
*  2019/07/29 | 1.0       | Hunter    | Create file |
*--------------------------------------------------------------------------------
*
*********************************************************************************/

/********************************************************************
ParticipantData.h
RTPS��Discoveryģ��������SPDP��ParticipantData�ṹ
********************************************************************/

#ifndef HIPERD_PARTICIPANT_BUILTIN_TOPIC_DATA_H
#define HIPERD_PARTICIPANT_BUILTIN_TOPIC_DATA_H

typedef struct _ParticipantData
{
    UserDataQosPolicy   userData;
    UINT32              domainID; /* domainID           */
    BoundedString128    name    ; /* Participant������  */
	PropertyList        propertyList; /* �����б�����ʹ�����ض���PropertyList */
} ParticipantData;


#endif
