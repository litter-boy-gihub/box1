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
*  @file     FragmentNumber.h
*  @brief    FragmentNumber
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
FragmentNumber.h
NackFrag�ӱ�����ؽṹ����ͱ���빤�ߺ�
********************************************************************/

#ifndef HIPERD_FRAGMENTNUMBER_H
#define HIPERD_FRAGMENTNUMBER_H

/*
* 32���صķ�Ƭ��
*/
typedef EX_UINT32 FragmentNumber_t;
/// ������FragmentNumber_tֵ
extern const FragmentNumber_t FRAGMENT_UNKNOWN;
extern const FragmentNumber_t MAX_FRAGMENT_NUMBER;
extern const FragmentNumber_t MIN_FRAGMENT_NUMBER;

typedef struct FragmentNumberRange
{
    FragmentNumber_t first;
    FragmentNumber_t second;
}FragmentNumberRange;

#endif

