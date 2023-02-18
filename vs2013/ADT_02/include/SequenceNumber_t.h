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
*  @file     SequenceNumber_t.h
*  @brief    SequenceNumber_t
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

/*
SequenceNumber_t.h
64���ص����к�
*/

#ifndef HIPERD_SEQUENCE_NUMBER_T_H
#define HIPERD_SEQUENCE_NUMBER_T_H

typedef struct _SequenceNumber_t
{
    EX_INT32    high    ; /* ��λ */ 
    EX_UINT32   low     ; /* ��λ */
}SequenceNumber_t;

#define ISEQUAL_SEQUENCENUMBER(left,right)  ((left.high == right.high) && (left.low == right.low))

#define ISLESSTHEN_SEQUENCENUMBER(left,right)  ((left.high < right.high) || ((left.high == right.high) && (left.low < right.low)))

#define ISEQUAL_OR_LESSTHEN_SEQUENCENUMBER(left,right)  (ISEQUAL_SEQUENCENUMBER(left,right) || ISLESSTHEN_SEQUENCENUMBER(left,right))

#define ISGRATERTHEN_SEQUENCENUMBER(left,right)  (!ISEQUAL_OR_LESSTHEN_SEQUENCENUMBER(left,right))

#define ISEQUAL_OR_GRATERTHEN_SEQUENCENUMBER(left,right) (!ISLESSTHEN_SEQUENCENUMBER(left,right))

/*
����Ϊ�ýṹ����빤�ߺ�
*/
#define SERIALIZE_SEQUENCENUMBER(seqNum, memBlock)    \
    do                                               \
    {                                                \
        PUT_INT(memBlock, seqNum.high)               \
        PUT_UNSIGNED_INT(memBlock, seqNum.low)       \
    } while (0);

#define DESERIALIZE_SEQUENCENUMBER(seqNum, memBlock,exchange)  \
    do                                               \
    {                                                \
        GET_INT(memBlock, seqNum.high,exchange)               \
        GET_UNSIGNED_INT(memBlock, seqNum.low,exchange)       \
    } while (0);

typedef struct SequenceRange
{
    SequenceNumber_t first;
    SequenceNumber_t second;
}SequenceRange;

extern SequenceNumber_t SEQUENCENUMBER_UNKNOWN;    // RTPS������SequenceNumber_tֵ
extern SequenceNumber_t SEQUENCENUMBER_MINIMUM;    // ��С��SequenceNumber_tֵ
extern SequenceNumber_t SEQUENCENUMBER_MAXIMUM;    // ��С��SequenceNumber_tֵ
extern SequenceNumber_t SEQUENCENUMBER_START;
extern SequenceNumber_t SEQUENCENUMBER_ZERO;

/* �Ӽ����жϴ�С���� */
extern SequenceNumber_t Add_SequenceNumber(SequenceNumber_t left, SequenceNumber_t right);
extern SequenceNumber_t Sub_SequenceNumber(SequenceNumber_t left, SequenceNumber_t right);

extern SequenceNumber_t GetPubWriterSequenceNumber();
extern SequenceNumber_t GetSubReaderSequenceNumber();

#endif
