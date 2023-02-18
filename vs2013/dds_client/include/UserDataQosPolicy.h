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
*  @file     UserDataQosPolicy.h
*  @brief    UserDataQosPolicy
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
UserDataQosPolicy.h
DCPS�淶��UserDataQosPolicy��Ӧ���������ͼ���ش������
********************************************************************/

#ifndef HIPERD_USER_DATA_QOS_POLICY_H
#define HIPERD_USER_DATA_QOS_POLICY_H

typedef struct _UserDataQosPolicy
{
    BoundedString256   value;

}UserDataQosPolicy;

/*
 ����Ϊ�ýṹ����빤�ߺ�
*/
#define SERIALIZE_USER_DATA_QOS_POLICY(locator , memBlock)                  \
    do                                                                     \
    {                                                                      \
        SERIALIZE_BOUNDED_STRING(locator.value, memBlock)                  \
    } while (0);


#define DESERIALIZE_USER_DATA_QOS_POLICY(locator , memBlock,exchange)                \
    do                                                                     \
    {                                                                      \
        DESERIALIZE_BOUNDED_STRING(locator.value, memBlock,exchange)                \
    } while (0);

#endif