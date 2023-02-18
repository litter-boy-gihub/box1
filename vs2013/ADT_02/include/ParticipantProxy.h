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
RTPS的Discovery模块中用于SPDP的ParticipantProxy结构，用来封装与一个
远程的Participant进行SPDP和SEDP交互的必要信息，例如网络地址和端口
********************************************************************/

#ifndef HIPERD_PARTICIPANT_PROXY_H
#define HIPERD_PARTICIPANT_PROXY_H

typedef struct _ParticipantProxy
{
    BOOL                      expectsInlineQos;            //是否期望inlineQos
    BuiltinEndpointSet_t      availableBuiltinEndpoints;   //内建实体兼容性检查，不同值表示不同类型实体，通过类似A|B|C的操作配置多个
    Locator_t*                metatrafficUnicastLocator;   //元传输单播网络地址，spdp单播接收
    Locator_t*                metatrafficMulticastLocator; //元传输组播网络地址，spdp组播接收
    Locator_t*                defaultUnicastLocator;	   //默认单播网络地址，用户数据单播接收
    Locator_t*                defaultMulticastLocator;	   //默认组播网络地址，用户数据组播接收，目前未见使用
    int                       manualLivelinessCount;       //存活性计数，目前未见使用
}ParticipantProxy;

/*
ParticipantProxy 的初始化宏
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
 以下为该结构编解码工具宏，需要完成
*/

#define SERIALIZE_PARTICIPANT_PROXY(proxy , memBlock) 

#define DESERIALIZE_PARTICIPANT_PROXY(proxy , memBlock) 

#endif
