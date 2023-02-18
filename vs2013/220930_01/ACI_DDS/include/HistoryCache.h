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
*  @file     HistoryCache.h
*  @brief    HistoryCache
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
HistoryCache.h
********************************************************************/

#ifndef HIPERD_HISTORYCACHE_H
#define HIPERD_HISTORYCACHE_H

#define HISTORYCACHE_MAX 512

// 消息类型
typedef enum DCPSMESSAGEID
{
    SAMPLE_DATA                 = 0x00,    /* 普通的样本数据 */
    REGISTER_INSTANCE           = 0x01,    /* 注册实例 */
    UNREGISTER_INSTANCE         = 0x02,    /* 注销实例 */
    DISPOSE_INSTANCE            = 0x03,    /* 删除实例 */
    DISPOSE_UNREGISTER_INSTANCE = 0x04,    /* 注销并删除实例 */
    END_COHERENT_CHANGES        = 0x05,	   /* 相干性控制 */
    MESSAGE_INVALID             = 0x06     /* 无效的消息类型 */
}DCPSMESSAGEID;

typedef struct _FragData
{
    USHORT              usFragIndex;    /*分片编号*/
    USHORT              usDataLen;      /*用户数据长度*/
    CHAR*               data;           /*用户数据*/
}FragData;

typedef struct _NackFragReader
{
    GUID_t stReaderID;
    struct _NackFragReader* pNext;
}NackFragReader;

typedef struct _HistoryData
{
	BOOL                 bCancel;			/* 是否取消发布订阅 */
	SequenceNumber_t     seqNum;			/* 消息序列号 */
	GUID_t               stGuid;			/* 实体guid */
	Time_t               timeInfo;			/* 数据时间戳 */
	UINT32               uiDataLen;			/* 用户数据长度 */
	CHAR*                data;				/* 用户数据 */

    USHORT               uiFragTotal;		/*分片总数*/
    USHORT               uiFragNum;			/*当前分片数*/
    UINT32               uiFragLen;			/*设置的分片数据大小*/
    FragData*            pstFragData;		/*分片数据*/
    CHAR*                FragDataStatus;	/* 分片数据接收状态 1:未收到 */
    BOOL                 bUseNackFrag;		/* 判断是整包重传 还是单分片重传 */
    NackFragReader*      pstNackReader;		/* Nephalem 当前数据需要通过NackFrag进行重传的reader */

	BOOL				 bSend;				/* Nephalem 数据是否需要发送，用于在复用组包时判断是否要合包 */

	USHORT				 uiRecvFragNum;		/* JDXCZ 2021-11-26接收到的分片数据的个数，用于判断分片数据是否接收完成*/

#if defined QYDDS_MASSDATA
    SequenceNumber_t     massDataBaseSeq;	/* 分片数据基地址 */
    USHORT               massDataTotal;		/* 分片总数 */
    UINT32               massDataTotolSize;	/* 分片数据总大小 */
    UINT32               massDataRecvNum;	/* 目前已组装的数据序号*/
#endif 

    struct _HistoryData*  pNext;			/* 下一个节点 */

}HistoryData;

typedef struct HistoryCache
{
    HistoryData* pstHead;
    HistoryData* pstTail;
    UINT32       uiHistoryDataNum;

}HistoryCache;

#define SERIALIZE_HISTORYDATA(historyData, memBlock)                    \
    do                                                                  \
    {                                                                   \
        PUT_UNSIGNED_INT(memBlock, historyData.uiDataLen)               \
        PUT_CHAR_ARRAY(memBlock,historyData.data,historyData.uiDataLen) \
    } while (0);


#define DESERIALIZE_HISTORYDATA(serData, memBlock,exchange)                      \
    do                                                                  \
    {                                                                   \
        GET_UNSIGNED_INT(memBlock, historyData.uiDataLen,exchange)               \
        GET_CHAR_ARRAY(memBlock,historyData.data,historyData.uiDataLen) \
    } while (0);


/* 初始化历史数据 */
extern VOID InitHistoryData(HistoryData* pstHistoryData);

/* JD 拷贝历史数据*/
extern VOID CopyHistoryData(HistoryData* pstDestHistoryData, HistoryData* pstSourceHistory);

/* 析构历史数据 */
extern VOID UninitHistoryData(HistoryData* pstHistoryData);

/* 初始化历史缓存数据 */
extern VOID InitHistoryCache(HistoryCache* pstHistoryCache);

/* 析构历史缓存数据 */
extern VOID UninitHistoryCache(HistoryCache* pstHistoryCache);

/* 缓存数据尾插缓存器 */
extern VOID InsertHistoryCacheTail(HistoryCache* pstHistoryCache, HistoryData* pstHistoryData);

/* 按照seqNum顺序大小插入缓存器 */
extern BOOL InsertHistoryCacheOrder(HistoryCache* pstHistoryCache, HistoryData* pstHistoryData);

#endif
