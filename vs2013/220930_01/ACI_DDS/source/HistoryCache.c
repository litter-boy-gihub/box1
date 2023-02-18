#include "../include/GlobalDefine.h"

/*---------------------------------------------------------------------------------
-   �� �� ��: InitHistoryData
-   ��������: ��ʼ����ʷ����
-   ��    ��: ������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��ʼ����ʷ����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\12       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID InitHistoryData(HistoryData* pstHistoryData)
{
    if (NULL != pstHistoryData)
    {
        pstHistoryData->bCancel = FALSE;
        pstHistoryData->data = NULL;
        pstHistoryData->seqNum = SEQUENCENUMBER_ZERO;
        pstHistoryData->stGuid = GUID_UNKNOWN;
        pstHistoryData->pNext = NULL;
        pstHistoryData->uiDataLen = 0;

        pstHistoryData->pstFragData = NULL;
        pstHistoryData->uiFragTotal = 0;
        pstHistoryData->uiFragNum = 0;
        pstHistoryData->uiFragLen = 0;
        pstHistoryData->FragDataStatus = NULL;
        pstHistoryData->bUseNackFrag = FALSE;
        pstHistoryData->pstNackReader = NULL;

		pstHistoryData->uiRecvFragNum = 0;

		pstHistoryData->bSend = TRUE;

#if defined QYDDS_MASSDATA
        /* Nephalem massdata ��ʼ�� */
        pstHistoryData->massDataBaseSeq = SEQUENCENUMBER_ZERO;
        pstHistoryData->massDataRecvNum = 0;
        pstHistoryData->massDataTotal = 0;
        pstHistoryData->massDataTotolSize = 0;
#endif 
    }
}

VOID CopyHistoryData(HistoryData* pstDestHistoryData, HistoryData* pstSourceHistory)
{
	pstDestHistoryData->uiFragTotal      = pstSourceHistory->uiFragTotal;
	pstDestHistoryData->uiFragNum        = pstSourceHistory->uiFragNum;
	pstDestHistoryData->uiFragLen        = pstSourceHistory->uiFragLen;
	pstDestHistoryData->uiDataLen        = pstSourceHistory->uiDataLen;
	pstDestHistoryData->timeInfo.sec     = pstSourceHistory->timeInfo.sec;
	pstDestHistoryData->timeInfo.nanosec = pstSourceHistory->timeInfo.nanosec;
	pstDestHistoryData->seqNum.high      = pstSourceHistory->seqNum.high;
	pstDestHistoryData->seqNum.low       = pstSourceHistory->seqNum.low;
	pstDestHistoryData->bUseNackFrag     = pstSourceHistory->bUseNackFrag;
	pstDestHistoryData->bCancel          = pstSourceHistory->bCancel;
	pstDestHistoryData->pNext            = pstSourceHistory->pNext;
	pstDestHistoryData->uiRecvFragNum    = pstSourceHistory->uiRecvFragNum;

	memcpy(&(pstDestHistoryData->stGuid),&(pstSourceHistory->stGuid),sizeof(pstDestHistoryData->stGuid));
	if (NULL != pstSourceHistory->pstNackReader)
	{
		pstDestHistoryData->pstNackReader = (NackFragReader*)malloc(sizeof(NackFragReader));
		memcpy(&(pstDestHistoryData->pstNackReader->stReaderID), &(pstSourceHistory->pstNackReader->stReaderID), sizeof(pstDestHistoryData->stGuid));
		pstDestHistoryData->pstNackReader->pNext = pstDestHistoryData->pstNackReader->pNext;
	}

	if (NULL != pstSourceHistory->FragDataStatus)
	{
		pstDestHistoryData->FragDataStatus = (CHAR*)malloc(pstDestHistoryData->uiFragTotal + 1);
		memcmp(pstDestHistoryData->FragDataStatus, pstSourceHistory->FragDataStatus, pstDestHistoryData->uiFragTotal + 1);
	}
	if (NULL != pstSourceHistory->data)
	{
		pstDestHistoryData->data = (CHAR*)malloc(pstDestHistoryData->uiDataLen);
		memcpy(pstDestHistoryData->data, pstSourceHistory->data, pstDestHistoryData->uiDataLen);
	}

	if (NULL != pstSourceHistory->pstFragData)
	{
		pstDestHistoryData->pstFragData = (FragData*)malloc(sizeof(FragData));
		memcpy(pstDestHistoryData->pstFragData, pstSourceHistory->pstFragData, sizeof(FragData));
		if (NULL != pstSourceHistory->pstFragData->data)
		{
			pstDestHistoryData->pstFragData->data = (CHAR*)malloc(pstDestHistoryData->pstFragData->usDataLen);
			memcpy(pstDestHistoryData->pstFragData->data, pstSourceHistory->pstFragData->data, pstDestHistoryData->pstFragData->usDataLen);
		}
	}
}

/*---------------------------------------------------------------------------------
-   �� �� ��: UninitHistoryData
-   ��������: ������ʷ����
-   ��    ��: ������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ������ʷ����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\12       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID UninitHistoryData(HistoryData* pstHistoryData)
{
    if (NULL != pstHistoryData)
    {
        pstHistoryData->bCancel = FALSE;

        pstHistoryData->seqNum = SEQUENCENUMBER_ZERO;
        pstHistoryData->stGuid = GUID_UNKNOWN;
        pstHistoryData->pNext = NULL;

        if (NULL != pstHistoryData->FragDataStatus)
        {
            DDS_STATIC_FREE(pstHistoryData->FragDataStatus);
            pstHistoryData->FragDataStatus = NULL;
        }

        if (NULL != pstHistoryData->pstFragData)
        {
            if (NULL != pstHistoryData->pstFragData->data)
            {

                DDS_STATIC_FREE(pstHistoryData->pstFragData->data);
                pstHistoryData->pstFragData->data = NULL;
            }

            DDS_STATIC_FREE(pstHistoryData->pstFragData);
            pstHistoryData->pstFragData = NULL;

        }
        
		if (NULL != pstHistoryData->data)
        {
            DDS_STATIC_FREE(pstHistoryData->data);
            pstHistoryData->data = NULL;
        }

        /* Nephalem ����ɾ�� */
        while (NULL != pstHistoryData->pstNackReader)
        {
            NackFragReader* pstDel = pstHistoryData->pstNackReader;
            pstHistoryData->pstNackReader = pstHistoryData->pstNackReader->pNext;
            DDS_STATIC_FREE(pstDel);
        }

        pstHistoryData->uiDataLen = 0;
		pstHistoryData->uiRecvFragNum = 0;
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: InitHistoryCache
-   ��������: ��ʼ����ʷ����
-   ��    ��: ������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ��ʼ����ʷ��������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\12       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID InitHistoryCache(HistoryCache* pstHistoryCache)
{
    if (NULL != pstHistoryCache)
    {
        pstHistoryCache->pstHead = NULL;
        pstHistoryCache->pstTail = NULL;
        pstHistoryCache->uiHistoryDataNum = 0;
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: InitHistoryCache
-   ��������: ������ʷ����
-   ��    ��: ������
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ������ʷ����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\12       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID UninitHistoryCache(HistoryCache* pstHistoryCache)
{
	HistoryData* pstHistoryData = NULL;
    if (NULL != pstHistoryCache)
    {
        pstHistoryData = pstHistoryCache->pstHead;
        while (NULL != pstHistoryData)
        {
            pstHistoryCache->pstHead = pstHistoryData->pNext;
            UninitHistoryData(pstHistoryData);
            DDS_STATIC_FREE(pstHistoryData);

            pstHistoryData = pstHistoryCache->pstHead;
        }
        DDS_STATIC_FREE(pstHistoryCache->pstHead);
        pstHistoryCache->uiHistoryDataNum = 0;
        pstHistoryCache->pstTail = NULL;
    }
}

/*---------------------------------------------------------------------------------
-   �� �� ��: InsertHistoryCacheTail
-   ��������: ��������β�建����
-   ��    ��: ���������������ݽṹ
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ����β�建����
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\12       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID InsertHistoryCacheTail(HistoryCache* pstHistoryCache, HistoryData* pstHistoryData)
{
//    printf("%d\n", pstHistoryCache->uiHistoryDataNum);
    if (NULL != pstHistoryCache->pstTail && pstHistoryCache->uiHistoryDataNum != 0)
    {
        pstHistoryCache->pstTail->pNext = pstHistoryData;
        pstHistoryCache->pstTail = pstHistoryData;
        pstHistoryData->pNext = NULL;
    }
    else
    {
        pstHistoryCache->pstHead = pstHistoryData;
        pstHistoryCache->pstTail = pstHistoryData;
        pstHistoryData->pNext = NULL;
    }

    pstHistoryCache->uiHistoryDataNum++;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: InsertHistoryCacheOrder
-   ��������: ����seqNum˳���С���뻺����
-   ��    ��: ���������������ݽṹ
-   ��    ��:
-   ȫ�ֱ���:
-   ע    ��: ����seqNum˳���С�ȴ�β���Ƚϲ���,�ٴ�ͷ����������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2019\8\12       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL InsertHistoryCacheOrder(HistoryCache* pstHistoryCache, HistoryData* pstNewHistoryData)
{
    HistoryData* pstTempHistoryData = NULL;
    HistoryData* pstHistoryData  = pstHistoryCache->pstHead;
    while (NULL != pstHistoryData)
    {
        /* �Ѵ��ڹ��� */
        if (ISEQUAL_SEQUENCENUMBER(pstHistoryData->seqNum, pstNewHistoryData->seqNum))
        {
			/* Nephalem �Ѵ��ڵ�HistoryData����Ҫ�����HistoryData������Ϊ����ɹ� */
			if (pstHistoryData == pstNewHistoryData)//����ʲôʱ�������أ��ظ�������ô��Ҫnum++
			{
				pstHistoryCache->uiHistoryDataNum++;
				return TRUE;
			}
				
            return FALSE;
        }

        /* ���ȿ���β�����룬��߲���Ч�� */
		if (ISLESSTHEN_SEQUENCENUMBER(pstHistoryCache->pstTail->seqNum, pstNewHistoryData->seqNum))/* ����������ѭ�������� */
        {
            DOUBLE_INSERT_TAIL(pstHistoryCache->pstHead, pstHistoryCache->pstTail, pstNewHistoryData);
            pstHistoryCache->uiHistoryDataNum++;
            //printf("reader historyDataNum: %d\n", pstHistoryCache->uiHistoryDataNum);
            return TRUE;
        }

        if (ISLESSTHEN_SEQUENCENUMBER(pstNewHistoryData->seqNum, pstHistoryData->seqNum))
        {
            if (NULL == pstTempHistoryData)
            {
                pstNewHistoryData->pNext = pstHistoryCache->pstHead;
                pstHistoryCache->pstHead = pstNewHistoryData;
                break;
            }
            else
            {
                pstNewHistoryData->pNext = pstTempHistoryData->pNext;
                pstTempHistoryData->pNext = pstNewHistoryData;
                break;
            }
        }
        else 
        {
            pstTempHistoryData = pstHistoryData;  //pstTempHistoryData��ʾǰһ���ڵ�
            pstHistoryData = pstHistoryData->pNext; //�����ǰ��ţ����ڵ���Ҫ�����������ţ���ǰ��������ƶ�
        }
    }

    if (NULL == pstHistoryData)
    {
        DOUBLE_INSERT_TAIL(pstHistoryCache->pstHead, pstHistoryCache->pstTail, pstNewHistoryData);
    }

    pstHistoryCache->uiHistoryDataNum++;

    return TRUE;
}
