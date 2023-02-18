#include "../include/GlobalDefine.h"

SubTaskControlBlock g_sub_task_ctrl_blocks[SUB_TASK_COUNT];

/*
�����е����������ṹ���г�ʼ��������ϵͳ������ͼ��ϵͳ�е�����ͨ�Ź�ϵ
����ȷ���Եģ���˿�����ÿһ���豸��ϵͳ��ʼ���׶ν����е�������ɳ�ʼ��
*/
void InitializeSubTasks()
{
    int sub_task_index;
    for (sub_task_index = 0; sub_task_index<SUB_TASK_COUNT; sub_task_index++)
    {
        g_sub_task_ctrl_blocks[sub_task_index].active = FALSE;
    }
    //�Բ��������������
    g_sub_task_ctrl_blocks[SUB_TASK_TEST].active = TRUE;
    g_sub_task_ctrl_blocks[SUB_TASK_TEST].miliSecPeriod = 5 * 1000;
    g_sub_task_ctrl_blocks[SUB_TASK_TEST].microSecDelayBound = 0x0fffffff;
    HPTB_INIT(g_sub_task_ctrl_blocks[SUB_TASK_TEST].timeBlock);

    //��SPDP���ݷ��������������
    g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].active = TRUE;
    g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].miliSecPeriod = 1 * 1000;
    g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].microSecDelayBound = 10 * 1000;
    HPTB_INIT(g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_SEND].timeBlock);

    //��SPDP���ݽ��������������
    g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_RECV].active = TRUE;
    g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_RECV].miliSecPeriod = 0;
    g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_RECV].microSecDelayBound = 0x0fffffff;
    HPTB_INIT(g_sub_task_ctrl_blocks[SUB_TASK_SPDP_PARTICIPANT_DATA_RECV].timeBlock);

    /* ��SEDP���ݷ�������������� */
    g_sub_task_ctrl_blocks[SUB_TASK_SEDP_DATA_SEND].active = TRUE;
    g_sub_task_ctrl_blocks[SUB_TASK_SEDP_DATA_SEND].miliSecPeriod = 0;
    g_sub_task_ctrl_blocks[SUB_TASK_SEDP_DATA_SEND].microSecDelayBound = 0x0fffffff;
    HPTB_INIT(g_sub_task_ctrl_blocks[SUB_TASK_SEDP_DATA_SEND].timeBlock);

    /* ��SEDP���ݽ�������������� */
    g_sub_task_ctrl_blocks[SUB_TASK_SEDP_DATA_RECV].active = TRUE;
    g_sub_task_ctrl_blocks[SUB_TASK_SEDP_DATA_RECV].miliSecPeriod = 0;
    g_sub_task_ctrl_blocks[SUB_TASK_SEDP_DATA_RECV].microSecDelayBound = 0x0fffffff;
    HPTB_INIT(g_sub_task_ctrl_blocks[SUB_TASK_SEDP_DATA_RECV].timeBlock);

    /* ����������������� */
    g_sub_task_ctrl_blocks[SUB_TASK_HEARTBEAT].active = TRUE;
    g_sub_task_ctrl_blocks[SUB_TASK_HEARTBEAT].miliSecPeriod = 0;
    g_sub_task_ctrl_blocks[SUB_TASK_HEARTBEAT].microSecDelayBound = 0x0fffffff;
    HPTB_INIT(g_sub_task_ctrl_blocks[SUB_TASK_HEARTBEAT].timeBlock);

    /* ���û����ݽ�������������� */
    g_sub_task_ctrl_blocks[SUB_TASK_USER_DATA_RECV].active = TRUE;
    g_sub_task_ctrl_blocks[SUB_TASK_USER_DATA_RECV].miliSecPeriod = 0;
    g_sub_task_ctrl_blocks[SUB_TASK_USER_DATA_RECV].microSecDelayBound = 0x0fffffff;
    HPTB_INIT(g_sub_task_ctrl_blocks[SUB_TASK_USER_DATA_RECV].timeBlock);

    //����ϵͳ��ͼ������Ϣ�����е����緢�ͺͽ��������������
    //ϵͳ��ʼ���׶Σ���Ȼ�������Щ������ĳ�ʼ�������ǲ�û�н��뼤��״̬
    //��������ҵ��Ӧ�ö��ķ����ӿڵĵ��ã����������շ��������𲽱�����
}

