#include "../include/GlobalDefine.h"

ConfData* g_pstHead = NULL;

/*---------------------------------------------------------------------------------
-   �� �� ��: ReadIniFile
-   ��������: ��ȡini�����ļ�
-   ��    ��: �ļ�·����
-   ��    ��: ����
-   ȫ�ֱ���:
-   ע    ��: ��ȡini�����ļ�
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\04\28       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
BOOL ReadIniFile(const CHAR* filename)
{
#if defined(_WIN32)
    FILE * pFile;
    CHAR line[INI_LINE_LEN];
    UINT32 i, j;
	ConfData* pstConfData = NULL;
	KeyValue* pstKeyValue = NULL;

    fopen_s(&pFile, filename, "rb");
    if (pFile == NULL)
    {
        printf("open file fail !\n");
        return FALSE;
    }

    /* ����������ݣ������ظ� */
    DeleteIniFile();

    while (!feof(pFile))
    {
        memset(line, 0, INI_LINE_LEN);
        fgets(line, INI_LINE_LEN, pFile);

        if ('#' == line[0])
        {
            continue;
        }

        if ('\r' == line[(strlen(line) - 2)] && '\n' == line[(strlen(line) - 1)])
        {
            line[strlen(line) - 2] = '\0';
        }

        if ('[' == line[0] && ']' == line[strlen(line)-1])
        {
            pstConfData = (ConfData*)DDS_STATIC_MALLOC(sizeof(ConfData));
            if (NULL == pstConfData)
            {
                return FALSE;
            }

            pstConfData->pstKeyValue = NULL;
            pstConfData->pNext = NULL;

            memcpy(pstConfData->label, line + 1, strlen(line)-2);
            pstConfData->label[strlen(line) - 2] = '\0';

            while (!feof(pFile))
            {
                memset(line, 0, INI_LINE_LEN);
                fgets(line, INI_LINE_LEN, pFile);

                if ('#' == line[0])
                {
                    continue;
                }

                if (' ' == line[0] || '\r' == line[0])
                {
                    break;
                }

                if ('\r' == line[(strlen(line) - 2)] && '\n' == line[(strlen(line) - 1)])
                {
                    line[strlen(line) - 2] = '\0';
                }

                pstKeyValue = (KeyValue*)DDS_STATIC_MALLOC(sizeof(KeyValue));
                if (NULL == pstKeyValue)
                {
                    DDS_STATIC_FREE(pstConfData);
                    return FALSE;
                }

                for (i = 0; i < strlen(line); i++)
                {
                    if ('=' == line[i])
                    {
                        pstKeyValue->key[i] = '\0';

                        for (j = 0; j < strlen(line) - i;j++)
                        {
                            pstKeyValue->value[j] = line[i+j+1];
                        }

                        break;
                    }

                    pstKeyValue->key[i] = line[i];

                }

                LIST_INSERT_HEAD(pstConfData->pstKeyValue, pstKeyValue);
            }

            LIST_INSERT_HEAD(g_pstHead, pstConfData);
        }
    }

    fclose(pFile);
    return TRUE;
#endif
}

/*---------------------------------------------------------------------------------
-   �� �� ��: DeleteIniConf
-   ��������: ���������ļ�
-   ��    ��: �ļ�·����
-   ��    ��: ����
-   ȫ�ֱ���:
-   ע    ��: ��ȡini�����ļ�
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\04\28       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
VOID DeleteIniFile()
{
    ConfData* pstConfData = g_pstHead;
    KeyValue* pstKeyValue = NULL;

    while (NULL != pstConfData)
    {
        LIST_DELETE_ALL(pstConfData->pstKeyValue, pstKeyValue);

        pstConfData = pstConfData->pNext;
    }

    LIST_DELETE_ALL(g_pstHead, pstConfData);
}

/*---------------------------------------------------------------------------------
-   �� �� ��: GetStrValue
-   ��������: ��ȡ��������
-   ��    ��: ��ǩ��keyֵ
-   ��    ��: ����
-   ȫ�ֱ���:
-   ע    ��: ��ȡ��������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\04\28       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL BOOL GetStrValue(const CHAR*label, const CHAR* key, CHAR* value)
{
	KeyValue* pstKeyValue = NULL;
    ConfData* pstConfData = g_pstHead;
    while (NULL != pstConfData)
    {
        if (!strcmp(pstConfData->label, label))
        {
            pstKeyValue = pstConfData->pstKeyValue;
            while (NULL != pstKeyValue)
            {
                if (!strcmp(pstKeyValue->key, key))
                {
					strncpy(value, pstKeyValue->value, strlen(value));
                    return TRUE;
                }

                pstKeyValue = pstKeyValue->pNext;
            }
        }
        pstConfData = pstConfData->pNext;
    }

    return FALSE;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: GetIntValue
-   ��������: ��ȡ��������
-   ��    ��: ��ǩ��keyֵ
-   ��    ��: ����
-   ȫ�ֱ���:
-   ע    ��: ��ȡ��������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\04\28       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL BOOL GetIntValue(const CHAR*label, const CHAR* key, INT* value)
{
    CHAR tmpStr[1024];
	KeyValue* pstKeyValue = NULL;
	CHAR* tmp = NULL;
    ConfData* pstConfData = g_pstHead;
    while (NULL != pstConfData)
    {
        if (!strcmp(pstConfData->label, label))
        {
            pstKeyValue = pstConfData->pstKeyValue;
            while (NULL != pstKeyValue)
            {
                if (!strcmp(pstKeyValue->key, key))
                {
                    *value = 1;
					strncpy(tmpStr, pstKeyValue->value, strlen(pstKeyValue->value));

                    tmp = strtok(tmpStr, "*");
                    while (tmp)
                    {
                        *value = *value * atoi(tmp);
                        tmp = strtok(NULL, "*");
                    }
                    return TRUE;
                }

                pstKeyValue = pstKeyValue->pNext;
            }
        }
        pstConfData = pstConfData->pNext;
    }

    return FALSE;
}

/*---------------------------------------------------------------------------------
-   �� �� ��: GetLongValue
-   ��������: ��ȡ��������
-   ��    ��: ��ǩ��keyֵ
-   ��    ��: ����
-   ȫ�ֱ���:
-   ע    ��: ��ȡ��������
-   �޸ļ�¼:
-   �޸�����        �汾        �޸���      �޸�ԭ������
==================================================================================
-   2020\04\28       1.0         Hunter      Created
----------------------------------------------------------------------------------*/
DDS_DLL BOOL GetLongValue(const CHAR*label, const CHAR* key, LONG* value)
{
    CHAR tmpStr[1024];
	KeyValue* pstKeyValue = NULL;
    ConfData* pstConfData = g_pstHead;
	CHAR* tmp = NULL;
    while (NULL != pstConfData)
    {
        if (!strcmp(pstConfData->label, label))
        {
            pstKeyValue = pstConfData->pstKeyValue;
            while (NULL != pstKeyValue)
            {
                if (!strcmp(pstKeyValue->key, key))
                {
                    *value = 1;
					strncpy(tmpStr, pstKeyValue->value, strlen(pstKeyValue->value));

                    tmp = strtok(tmpStr, "*");
                    while (tmp)
                    {
                        *value = *value * atol(tmp);
                        tmp = strtok(NULL, "*");
                    }
                    return TRUE;
                }

                pstKeyValue = pstKeyValue->pNext;
            }
        }
        pstConfData = pstConfData->pNext;
    }

    return FALSE;
}

DDS_DLL BOOL GetIntListValue(const CHAR*label, const CHAR* key, int * rArrValue, int *rSize)
{
    CHAR tmpStr[1024];
	KeyValue* pstKeyValue = NULL;
	CHAR* tmp = NULL;
    ConfData* pstConfData = g_pstHead;
    while (NULL != pstConfData)
    {
        if (!strcmp(pstConfData->label, label))
        {
            pstKeyValue = pstConfData->pstKeyValue;
            while (NULL != pstKeyValue)
            {
                if (!strcmp(pstKeyValue->key, key))
                {
					strncpy(tmpStr, pstKeyValue->value, strlen(pstKeyValue->value));

                    *rSize = 0;
                    tmp = strtok(tmpStr, ",");
                    while (tmp)
                    {
                        rArrValue[*rSize] = atoi(tmp);
                        tmp = strtok(NULL, ",");
                        *rSize = *rSize + 1;
                    }
                    return TRUE;
                }

                pstKeyValue = pstKeyValue->pNext;
            }
        }
        pstConfData = pstConfData->pNext;
    }

    return FALSE;
}