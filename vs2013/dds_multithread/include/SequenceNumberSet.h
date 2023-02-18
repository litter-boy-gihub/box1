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
*  @file     SequenceNumberSet.h
*  @brief
*  Details.
*
*  @author   Hunter
*  @email    1028251654@qq.com
*  @version  1.0
*  @date     2019/08/20
*  @license
*
*--------------------------------------------------------------------------------
*  Remark         : Description
*--------------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>  | <operation> | <Description>
*--------------------------------------------------------------------------------
*  2019/08/20 | 1.0       | Hunter    | Create file |
*--------------------------------------------------------------------------------
*
*********************************************************************************/

#ifndef HIPERD_SEQUENCENUMBERSET_H
#define HIPERD_SEQUENCENUMBERSET_H

// ����������ʾbitmap�����4�ֽڸ���
#define SEQUENCE_BIT_NUMBER  8

/****************************************************************************************************************************************/
//�ṹ�嶨��
/****************************************************************************************************************************************/

typedef struct _SequenceNumberSet
{
    SequenceNumber_t base                       ; 
    EX_UINT32        numBits                    ;   /* bitMap����Ч�ı���λ�� */
    EX_UINT32        bitMap[SEQUENCE_BIT_NUMBER];

}SequenceNumberSet;

/* SequenceNumberSet�������ع��ߺ� */
#define SERIALIZE_SEQUENCENUMBERSET(seqNumSet, memBlock)                        \
    do                                                                          \
	    {                                                                       \
		int number;                                                             \
        SERIALIZE_SEQUENCENUMBER(seqNumSet.base, memBlock)                      \
        PUT_UNSIGNED_INT(memBlock, seqNumSet.numBits)                           \
        number = ((seqNumSet.numBits + 31) >> 5);                               \
        PUT_UNSIGNED_INT_ARRAY(memBlock, seqNumSet.bitMap, number)              \
	    } while (0);

#define DESERIALIZE_SEQUENCENUMBERSET(seqNumSet, memBlock,exchange)                      \
    do                                                                          \
	    {                                                                           \
		int number; \
        DESERIALIZE_SEQUENCENUMBER(seqNumSet.base, memBlock,exchange)                    \
        GET_UNSIGNED_INT(memBlock, seqNumSet.numBits,exchange)                           \
        number = (seqNumSet.numBits %32)?(seqNumSet.numBits /32 +1):(seqNumSet.numBits /32);/* Ŀǰֻʹ��8λ������numBits�޹أ�bitMapδʹ�ã���ʱ����Ҫ�����л�((seqNumSet.numBits + 31) >> 5); */                           \
        GET_UNSIGNED_INT_ARRAY(memBlock, seqNumSet.bitMap, number,exchange)              \
     } while (0);

/****************************************************************************************************************************************/
//��������
/****************************************************************************************************************************************/

#endif
