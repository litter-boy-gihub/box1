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
*  @file     Data.h
*  @brief    Data
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

#ifndef HIPERD_DATA_H
#define HIPERD_DATA_H

typedef struct Data
{
	/********************* �������� ***************************/
	EX_UINT8            extraFlags[2];     //[0]�ֽ���[1]0000
	EX_UINT16           octetsToInlineQos; //InlineQosFlag���Ƿ�ȡ����������
	EntityId_t          readerId;
	EntityId_t          writerId;
	SequenceNumber_t    writerSN;
	SerializedData      serializedPayload; //��С�˱��

	/********************* �������� ***************************/
	ParameterList       inlineQos;    /* ��¼�����б��������л���serializedData���棻Ŀǰѹ��û�õ� */
	CHAR*               pcReaderId;   /* �������readidλ��*/
	UINT32              uiStartIndex; /* ��ʼλ�� */
}Data;

#define SERIALIZE_DATA(curData, memBlock)                                       \
    do                                                                          \
    {                                                                           \
        curData.uiStartIndex = memBlock.writeIndex;                             \
        PUT_BYTE_ARRAY(memBlock, curData.extraFlags, 2)                         \
        PUT_UNSIGNED_SHORT(memBlock, curData.octetsToInlineQos)                 \
        curData.pcReaderId = memBlock.wrPtr;                                    \
        SERIALIZE_ENTITYID(curData.readerId, memBlock)                          \
        SERIALIZE_ENTITYID(curData.writerId, memBlock)                          \
        SERIALIZE_SEQUENCENUMBER(curData.writerSN, memBlock)                    \
    } while (0);

#define DESERIALIZE_DATA(curData, memBlock,exchange)                                     \
    do                                                                          \
    {                                                                           \
        GET_BYTE_ARRAY(memBlock, curData.extraFlags, 2)                         \
        GET_UNSIGNED_SHORT(memBlock, curData.octetsToInlineQos,exchange)                 \
        DESERIALIZE_ENTITYID(curData.readerId, memBlock)                        \
        DESERIALIZE_ENTITYID(curData.writerId, memBlock)                        \
        DESERIALIZE_SEQUENCENUMBER(curData.writerSN, memBlock,exchange)                  \
    } while (0);

#endif
