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
*  @file     HeartBeat.h
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

#ifndef HIPERD_HEARTBEAT_H
#define HIPERD_HEARTBEAT_H
    
/****************************************************************************************************************************************/
//结构体定义
/****************************************************************************************************************************************/

typedef struct _Heartbeat
{
    EntityId_t          readerId;
    EntityId_t          writerId;
    SequenceNumber_t    firstSN;
    SequenceNumber_t    lastSN;
    UINT32              count;
    UINT32              uiStartIndex; /* 起始位置 */
    CHAR*               pcReadId;
}Heartbeat;

BOOL Initial_Heartbeat(Heartbeat * curHeartbeat);

/* Heartbeat编解码工具宏 */

#define SERIALIZE_HEARTBEAT(heartbeat , memBlock)                       \
    do                                                                  \
    {                                                                   \
        heartbeat.pcReadId = memBlock.wrPtr;                            \
        heartbeat.uiStartIndex = memBlock.writeIndex;                   \
        SERIALIZE_ENTITYID(heartbeat.readerId, memBlock)                \
        SERIALIZE_ENTITYID(heartbeat.writerId, memBlock)                \
        SERIALIZE_SEQUENCENUMBER(heartbeat.firstSN, memBlock)           \
        SERIALIZE_SEQUENCENUMBER(heartbeat.lastSN, memBlock)            \
        PUT_UNSIGNED_INT(memBlock, heartbeat.count)                     \
    } while (0);

#define DESERIALIZE_HEARTBEAT(heartbeat , memBlock,exchange)                     \
    do                                                                  \
    {                                                                   \
        DESERIALIZE_ENTITYID(heartbeat.readerId, memBlock)              \
        DESERIALIZE_ENTITYID(heartbeat.writerId, memBlock)              \
        DESERIALIZE_SEQUENCENUMBER(heartbeat.firstSN, memBlock,exchange)         \
        DESERIALIZE_SEQUENCENUMBER(heartbeat.lastSN, memBlock,exchange)          \
        GET_UNSIGNED_INT(memBlock, heartbeat.count,exchange)                     \
    } while (0);


/* Nephalem 分片心跳结构*/
typedef unsigned long FragmentNumber;
typedef struct _Heartbeat_Frag
{
    EntityId_t          readerId;
    EntityId_t          writerId;
    SequenceNumber_t    writerSN;
    FragmentNumber      lastFragmentNum; /* 最后一个分片数据序号 */
    UINT32              count;
    UINT32              uiStartIndex; /* 起始位置 */
    CHAR*               pcReadId;
}HeartbeatFrag;


#define SERIALIZE_HEARTBEATFRAG(heartbeatFrag , memBlock)               \
    do                                                                  \
        {                                                               \
        heartbeatFrag.pcReadId = memBlock.wrPtr;                        \
        heartbeatFrag.uiStartIndex = memBlock.writeIndex;               \
        SERIALIZE_ENTITYID(heartbeatFrag.readerId, memBlock)            \
        SERIALIZE_ENTITYID(heartbeatFrag.writerId, memBlock)            \
        SERIALIZE_SEQUENCENUMBER(heartbeatFrag.writerSN, memBlock)      \
        PUT_UNSIGNED_INT(memBlock, heartbeatFrag.lastFragmentNum)       \
        PUT_UNSIGNED_INT(memBlock, heartbeatFrag.count)                 \
        } while (0);

#define DESERIALIZE_HEARTBEATFRAG(heartbeatFrag , memBlock,exchange)                 \
    do                                                                      \
        {                                                                   \
        DESERIALIZE_ENTITYID(heartbeatFrag.readerId, memBlock)              \
        DESERIALIZE_ENTITYID(heartbeatFrag.writerId, memBlock)              \
        DESERIALIZE_SEQUENCENUMBER(heartbeatFrag.writerSN, memBlock,exchange)        \
        GET_UNSIGNED_INT(memBlock, heartbeatFrag.lastFragmentNum,exchange)           \
        GET_UNSIGNED_INT(memBlock, heartbeatFrag.count,exchange)                     \
        } while (0);
/****************************************************************************************************************************************/
//函数声明
/****************************************************************************************************************************************/

#endif
