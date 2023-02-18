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
*  @file     ParticipantProxy.h
*  @brief    ParticipantProxy
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
ParticipantProxy.h
RTPS��Discoveryģ��������SPDP��ParticipantProxy�ṹ��������װ��һ��
Զ�̵�Participant����SPDP��SEDP�����ı�Ҫ��Ϣ�����������ַ�Ͷ˿�
********************************************************************/

#ifndef HIPERD_PARTICIPANT_PROXY_H
#define HIPERD_PARTICIPANT_PROXY_H

typedef struct _ParticipantProxy
{
    BOOL                    expectsInlineQos;
    BuiltinEndpointSet_t    availableBuiltinEndpoints;   
	//�����Ա�ʶ�������п��õ�����SEDP �˵�����ͣ�ʹ�����߿���ָʾ�����������ܵ����ö˵���Ӽ���
	/*  BuiltinEndpointSet_t �Ŀ���ֵΪ��PUBLICATIONS_READER��PUBLICATIONS_WRITER��
	SUBSCRIPTIONS_READER��SUBSCRIPTIONS_WRITER��
	TOPIC_READER ��TOPIC_WRITER */
    Locator_t*                metatrafficUnicastLocator;//����
    Locator_t*                metatrafficMulticastLocator;//�鲥
    Locator_t*                defaultUnicastLocator;
    Locator_t*                defaultMulticastLocator;
    int                     manualLivelinessCount;
}ParticipantProxy;

/*
ParticipantProxy �ĳ�ʼ����
*/
#define INITIAL_PARTICIPANT_PROXY(proxy)                                \
    do                                                                  \
    {                                                                   \
        INITIAL_LOCATOR_LIST(proxy.metatrafficUnicastLocatorList)       \
        INITIAL_LOCATOR_LIST(proxy.metatrafficMulticastLocatorList)     \
        INITIAL_LOCATOR_LIST(proxy.defaultUnicastLocatorList)           \
        INITIAL_LOCATOR_LIST(proxy.defaultMulticastLocatorList)         \
    } while (0);

/*
 ����Ϊ�ýṹ����빤�ߺ꣬��Ҫ���
*/

#define SERIALIZE_PARTICIPANT_PROXY(proxy , memBlock) 

#define DESERIALIZE_PARTICIPANT_PROXY(proxy , memBlock) 

#endif