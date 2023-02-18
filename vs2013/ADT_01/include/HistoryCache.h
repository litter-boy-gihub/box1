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

// ��Ϣ����
typedef enum DCPSMESSAGEID
{
    SAMPLE_DATA                 = 0x00,    /* ��ͨ���������� */
    REGISTER_INSTANCE           = 0x01,    /* ע��ʵ�� */
    UNREGISTER_INSTANCE         = 0x02,    /* ע��ʵ�� */
    DISPOSE_INSTANCE            = 0x03,    /* ɾ��ʵ�� */
    DISPOSE_UNREGISTER_INSTANCE = 0x04,    /* ע����ɾ��ʵ�� */
    END_COHERENT_CHANGES        = 0x05,	   /* ����Կ��� */
    MESSAGE_INVALID             = 0x06     /* ��Ч����Ϣ���� */
}DCPSMESSAGEID;

typedef struct _FragData
{
    USHORT              usFragIndex;    /*��Ƭ���*/
    USHORT              usDataLen;      /*�û����ݳ���*/
    CHAR*               data;           /*�û�����*/
}FragData;

typedef struct _NackFragReader
{
    GUID_t stReaderID;
    struct _NackFragReader* pNext;
}NackFragReader;

typedef struct _HistoryData
{
	BOOL                 bCancel;			/* �Ƿ�ȡ���������� */
	SequenceNumber_t     seqNum;			/* ��Ϣ���к� */
	GUID_t               stGuid;			/* ʵ��guid */
	Time_t               timeInfo;			/* ����ʱ��� */
	UINT32               uiDataLen;			/* �û����ݳ��� */
	CHAR*                data;				/* �û����� */

    USHORT               uiFragTotal;		/*��Ƭ����*/
    USHORT               uiFragNum;			/*��ǰ��Ƭ��*/
    UINT32               uiFragLen;			/*���õķ�Ƭ���ݴ�С*/
    FragData*            pstFragData;		/*��Ƭ����*/
    CHAR*                FragDataStatus;	/* ��Ƭ���ݽ���״̬ 1:δ�յ� */
    BOOL                 bUseNackFrag;		/* �ж��������ش� ���ǵ���Ƭ�ش� */
    NackFragReader*      pstNackReader;		/* Nephalem ��ǰ������Ҫͨ��NackFrag�����ش���reader */

	BOOL				 bSend;				/* Nephalem �����Ƿ���Ҫ���ͣ������ڸ������ʱ�ж��Ƿ�Ҫ�ϰ� */

	USHORT				 uiRecvFragNum;		/* JDXCZ 2021-11-26���յ��ķ�Ƭ���ݵĸ����������жϷ�Ƭ�����Ƿ�������*/

#if defined QYDDS_MASSDATA
    SequenceNumber_t     massDataBaseSeq;	/* ��Ƭ���ݻ���ַ */
    USHORT               massDataTotal;		/* ��Ƭ���� */
    UINT32               massDataTotolSize;	/* ��Ƭ�����ܴ�С */
    UINT32               massDataRecvNum;	/* Ŀǰ����װ���������*/
#endif 

    struct _HistoryData*  pNext;			/* ��һ���ڵ� */

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


/* ��ʼ����ʷ���� */
extern VOID InitHistoryData(HistoryData* pstHistoryData);

/* JD ������ʷ����*/
extern VOID CopyHistoryData(HistoryData* pstDestHistoryData, HistoryData* pstSourceHistory);

/* ������ʷ���� */
extern VOID UninitHistoryData(HistoryData* pstHistoryData);

/* ��ʼ����ʷ�������� */
extern VOID InitHistoryCache(HistoryCache* pstHistoryCache);

/* ������ʷ�������� */
extern VOID UninitHistoryCache(HistoryCache* pstHistoryCache);

/* ��������β�建���� */
extern VOID InsertHistoryCacheTail(HistoryCache* pstHistoryCache, HistoryData* pstHistoryData);

/* ����seqNum˳���С���뻺���� */
extern BOOL InsertHistoryCacheOrder(HistoryCache* pstHistoryCache, HistoryData* pstHistoryData);

#endif
