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
*  @file     NackFrag.h
*  @brief    NackFrag
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
NackFrag.h
NackFrag�ӱ�����ؽṹ����ͱ���빤�ߺ�
********************************************************************/

#ifndef HIPERD_NACKFRAG_H
#define HIPERD_NACKFRAG_H

#define  NACKFRAG_MAXSIZE 64 /* 4 + 4 + 4 + 8 + 4 + 4 + 32 + 4 */

typedef struct NackFrag
{
    EntityId_t          readerId            ;
    EntityId_t          writerId            ;
    SequenceNumber_t    writerSN            ;
    FragmentNumberSet   fragmentNumberState ;
    unsigned int        count               ;
    UINT32              uiStartIndex        ; /* ��ʼλ�� */
    BOOL                littleEndian        ;
}NackFrag;

/* NackFrag ��ر���빤�ߺ� */

#define SERIALIZE_NACKFRAG(nackFrag, memBlock)                              \
    do                                                                      \
    {                                                                       \
        nackFrag.uiStartIndex = memBlock.writeIndex;                        \
        SERIALIZE_ENTITYID(nackFrag.readerId, memBlock)                     \
        SERIALIZE_ENTITYID(nackFrag.writerId, memBlock)                     \
        SERIALIZE_SEQUENCENUMBER(nackFrag.writerSN, memBlock)               \
        SERIALIZE_FRAGMENTNUMBERSET(nackFrag.fragmentNumberState, memBlock) \
        PUT_UNSIGNED_INT(memBlock, nackFrag.count)                          \
    } while (0);


#define DESERIALIZE_NACKFRAG(nackFrag, memBlock,exchange)                                \
    do                                                                          \
    {                                                                           \
        DESERIALIZE_ENTITYID(nackFrag.readerId, memBlock)                       \
        DESERIALIZE_ENTITYID(nackFrag.writerId, memBlock)                       \
        DESERIALIZE_SEQUENCENUMBER(nackFrag.writerSN, memBlock,exchange)                 \
        DESERIALIZE_FRAGMENTNUMBERSET(nackFrag.fragmentNumberState, memBlock,exchange)   \
        GET_UNSIGNED_INT(memBlock, nackFrag.count,exchange)                              \
    } while (0);

#endif