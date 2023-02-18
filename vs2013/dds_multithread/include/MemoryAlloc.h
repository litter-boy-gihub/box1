/*********************************************************************************
*  OpenST Basic tool library
*  Copyright (C) 2019 QiYuan
*
*
*  This program is security software
*  without permission you cannot MEMUSE or distribute the software
*  otherwise, legal liability will be investigated
*
*  If you want to MEMUSE the software, please contact the authorities to obtain a license
*  < http://www.iamqy.com.cn >.
*
*  @file     MemoryAlloc.h
*  @brief    
*  Details.
*
*  @author   Hunter
*  @email    1028251654@qq.com
*  @version  1.0
*  @date     2019/07/29
*  @license  GNU General Public License (GPL)
*
*--------------------------------------------------------------------------------
*  Remark         : Description
*--------------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>  | <operation> | <Description>
*--------------------------------------------------------------------------------
*  2019/07/29 | 1.0       | Hunter    | Create file |
*--------------------------------------------------------------------------------
*
*********************************************************************************/

#ifndef HIPERD_MEMORY_ALLOC_H
#define HIPERD_MEMORY_ALLOC_H

typedef struct _MemBlock
{
	/* ĩβ��ַ*/
	CHAR*  end;
	/* ��һ�� */
	struct _MemBlock* pNext;

}MemBlock;

typedef struct _MemBlockInfo
{
	/* ��ʼ��ַ*/
	CHAR* base;
	/* ĩβ��ַ*/
	CHAR*  end;
	/* ���ݿ���� */
	USHORT usBlockNum;
	/* ���ݿ��ܳ��� */
	UINT32 uiBlockTotal;

}MemBlockInfo;

#ifdef USE_STATIC_MALLOC

/* ���ͺ��ڴ������ */
#define MEMBLOCK_NUM_SIZE_16B     5120
#define MEMBLOCK_NUM_SIZE_32B     5120
#define MEMBLOCK_NUM_SIZE_64B     5120
#define MEMBLOCK_NUM_SIZE_128B    5120
#define MEMBLOCK_NUM_SIZE_256B    5120
#define MEMBLOCK_NUM_SIZE_512B    5120
#define MEMBLOCK_NUM_SIZE_1K      5120
#define MEMBLOCK_NUM_SIZE_2K      5120
#define MEMBLOCK_NUM_SIZE_4K      5120
#define MEMBLOCK_NUM_SIZE_8K      5120
#define MEMBLOCK_NUM_SIZE_16K     5120
#define MEMBLOCK_NUM_SIZE_32K     5120
#define MEMBLOCK_NUM_SIZE_64K     5120

/* ���ͺ��ڴ�鱣���� */
#define RESERVE_NUM               32

/* �ڴ���ͺ�ö�� */
typedef enum MemBlockType
{
    MEMBLOCK_SIZE_16B   = 0x10,
    MEMBLOCK_SIZE_32B   = 0x20,
    MEMBLOCK_SIZE_64B   = 0x40,
    MEMBLOCK_SIZE_128B  = 0x80,
    MEMBLOCK_SIZE_256B  = 0x100,
    MEMBLOCK_SIZE_512B  = 0x200,
    MEMBLOCK_SIZE_1K    = 0x400,
    MEMBLOCK_SIZE_2K    = 0x800,
    MEMBLOCK_SIZE_4K    = 0x1000,
    MEMBLOCK_SIZE_8K    = 0x2000,
    MEMBLOCK_SIZE_16K   = 0x4000,
    MEMBLOCK_SIZE_32K   = 0x8000,
    MEMBLOCK_SIZE_64K   = 0x10000,

}MemBlockType;

/* �ڴ���ͺ�ö�� */
typedef enum _MENUSE
{
    UNSE = 0x00,
    USED = 0x01,

}MEMUSE;


/****************************************************************************************************************************************/
//�ṹ�嶨��
/****************************************************************************************************************************************/

/* 16B�ֽ��ڴ��ṹ */
typedef struct _MEMBLOCK_16B
{
    struct _MEMBLOCK_16B* pNext;
    UINT32 uiSeqNum;
    MemBlockType memType;
    MEMUSE eumUse;
    char memBlock[MEMBLOCK_SIZE_16B];

}MEMBLOCK_16B;

/* 32B�ֽ��ڴ��ṹ */
typedef struct _MEMBLOCK_32B
{
    struct _MEMBLOCK_32B* pNext;
    UINT32 uiSeqNum;
    MemBlockType memType;
    MEMUSE eumUse;
    char memBlock[MEMBLOCK_SIZE_32B];

}MEMBLOCK_32B;

/* 64B�ֽ��ڴ��ṹ */
typedef struct _MEMBLOCK_64B
{
    struct _MEMBLOCK_64B* pNext;
    UINT32 uiSeqNum;
    MemBlockType memType;
    MEMUSE eumUse;
    char memBlock[MEMBLOCK_SIZE_64B];

}MEMBLOCK_64B;

/* 128B�ֽ��ڴ��ṹ */
typedef struct _MEMBLOCK_128B
{
    struct _MEMBLOCK_128B* pNext;
    UINT32 uiSeqNum;
    MemBlockType memType;
    MEMUSE eumUse;
    char memBlock[MEMBLOCK_SIZE_128B];

}MEMBLOCK_128B;

/* 256B�ֽ��ڴ��ṹ */
typedef struct _MEMBLOCK_256B
{
    struct _MEMBLOCK_256B* pNext;
    UINT32 uiSeqNum;
    MemBlockType memType;
    MEMUSE eumUse;
    char memBlock[MEMBLOCK_SIZE_256B];

}MEMBLOCK_256B;

/* 512B�ֽ��ڴ��ṹ */
typedef struct _MEMBLOCK_512B
{
    struct _MEMBLOCK_512B* pNext;
    UINT32 uiSeqNum;
    MemBlockType memType;
    MEMUSE eumUse;
    char memBlock[MEMBLOCK_SIZE_512B];

}MEMBLOCK_512B;

/* 1K�ֽ��ڴ��ṹ */
typedef struct _MEMBLOCK_1K
{
    struct _MEMBLOCK_1K* pNext;
    UINT32 uiSeqNum;
    MemBlockType memType;
    MEMUSE eumUse;
    char memBlock[MEMBLOCK_SIZE_1K];

}MEMBLOCK_1K;

/* 2K�ֽ��ڴ��ṹ */
typedef struct _MEMBLOCK_2K
{
    struct _MEMBLOCK_2K* pNext;
    UINT32 uiSeqNum;
    MemBlockType memType;
    MEMUSE eumUse;
    char memBlock[MEMBLOCK_SIZE_2K];

}MEMBLOCK_2K;

/* 4K�ֽ��ڴ��ṹ */
typedef struct _MEMBLOCK_4K
{
    struct _MEMBLOCK_4K* pNext;
    UINT32 uiSeqNum;
    MemBlockType memType;
    MEMUSE eumUse;
    char memBlock[MEMBLOCK_SIZE_4K];

}MEMBLOCK_4K;

/* 8K�ֽ��ڴ��ṹ */
typedef struct _MEMBLOCK_8K
{
    struct _MEMBLOCK_8K* pNext;
    UINT32 uiSeqNum;
    MemBlockType memType;
    MEMUSE eumUse;
    char memBlock[MEMBLOCK_SIZE_8K];

}MEMBLOCK_8K;

/* 16K�ֽ��ڴ��ṹ */
typedef struct _MEMBLOCK_16K
{
    struct _MEMBLOCK_16K* pNext;
    UINT32 uiSeqNum;
    MemBlockType memType;
    MEMUSE eumUse;
    char memBlock[MEMBLOCK_SIZE_16K];

}MEMBLOCK_16K;

/* 32K�ֽ��ڴ��ṹ */
typedef struct _MEMBLOCK_32K
{
    struct _MEMBLOCK_32K* pNext;
    UINT32 uiSeqNum;
    MemBlockType memType;
    MEMUSE eumUse;
    char memBlock[MEMBLOCK_SIZE_32K];

}MEMBLOCK_32K;

/* 64K�ֽ��ڴ��ṹ */
typedef struct _MEMBLOCK_64K
{
    struct _MEMBLOCK_64K* pNext;
    UINT32 uiSeqNum;
    MemBlockType memType;
    MEMUSE eumUse;
    char memBlock[MEMBLOCK_SIZE_64K];

}MEMBLOCK_64K;

#endif

/****************************************************************************************************************************************/
//��������
/****************************************************************************************************************************************/

#define STATIC_MEM_LEN  1024*1024*100

/* ��̬�ڴ��ʼ�� */
extern VOID InitMemBlock();

/* ��̬�ڴ���� */
extern DDS_DLL VOID* DDS_STATIC_MALLOC(UINT32 uiLen);

/* ��̬�ڴ���� */
extern DDS_DLL VOID* DDS_STATIC_MALLOC_T(UINT32 uiLen);

/* ��̬�ڴ��ͷ� */
extern DDS_DLL VOID DDS_STATIC_FREE(VOID* pMemBlock);

/* ��̬�ڴ��� */
extern DDS_DLL VOID DDS_MemoryUsage();

#ifdef USE_STATIC_MALLOC_T
#define DDS_STATIC_MALLOC_T DDS_STATIC_MALLOC
#endif

extern VOID PrintMemBlockInfo();

#endif