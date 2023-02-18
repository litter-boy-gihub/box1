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
*  @file     FragmentNumberSet.h
*  @brief    FragmentNumberSet
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
FragmentNumberSet.h
FragmentNumber����
********************************************************************/

#ifndef HIPERD_FRAGMENTNUMBERSET_H
#define HIPERD_FRAGMENTNUMBERSET_H

/* ����������ʾbitmap�����4�ֽڸ��� */
#define FRAGMENT_BIT_NUMBER  8
/* 32λ����FRAGMENT_BIT_NUMBER����FragmentSet�����Ա�����Fragment���� */
#define MAX_FRAGMENTNUM_ENCODE  FRAGMENT_BIT_NUMBER * 32;

typedef struct FragmentNumberSet
{

    FragmentNumber_t base                       ; /* bitMap����Ч�ı���λ�� */
    EX_UINT32        numBits                    ;
    EX_UINT32        bitMap[FRAGMENT_BIT_NUMBER];
}FragmentNumberSet;

/* FragmentNumberSet ��ر���빤�ߺ� */
#define SERIALIZE_FRAGMENTNUMBERSET(fragNumSet, memBlock)                       \
    do                                                                          \
            {                                                                           \
        int number = ((fragNumSet.numBits + 31) >> 5);                           \
        PUT_UNSIGNED_INT(memBlock, fragNumSet.base)                             \
        PUT_UNSIGNED_INT(memBlock, fragNumSet.numBits)                          \
        PUT_UNSIGNED_INT_ARRAY(memBlock, fragNumSet.bitMap, number)             \
            } while (0);

#define DESERIALIZE_FRAGMENTNUMBERSET(fragNumSet, memBlock,exchange)                     \
    do                                                                          \
        {                                                                           \
        int number = 8;                          \
        GET_UNSIGNED_INT(memBlock, fragNumSet.base,exchange)                             \
        GET_UNSIGNED_INT(memBlock, fragNumSet.numBits,exchange)                          \
        GET_UNSIGNED_INT_ARRAY(memBlock, fragNumSet.bitMap, number,exchange)             \
    } while (0);
 
#endif
