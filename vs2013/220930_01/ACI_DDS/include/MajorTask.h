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
*  @file     VendorId_t.h
*  @brief    VendorId_t
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
VendorId_t.h
RTPS�淶��VendorId_t��Ӧ���������ͼ���ش������
********************************************************************/

#ifndef HIPERD_MAJORTASK_H
#define HIPERD_MAJORTASK_H

/*
������Ĺ���״̬���������ֿ��ܵĳ������
*/
typedef enum MainTaskState
{
    MTS_IDLE = 0,
    MTS_TO_EXIT = 1,
    MTS_SEND_SPDP = 2,
    MTS_RECV_SPDP = 3,
}MainTaskState;

typedef enum SUB_TASK_ID
{
    SUB_TASK_INVALID = -1,
    SUB_TASK_TEST = 0,
    SUB_TASK_SPDP_PARTICIPANT_DATA_SEND = 1,
    SUB_TASK_SPDP_PARTICIPANT_DATA_RECV = 2,
    SUB_TASK_SEDP_DATA_SEND = 3,
    SUB_TASK_SEDP_DATA_RECV = 4,
    SUB_TASK_HEARTBEAT = 5,
    SUB_TASK_USER_DATA_RECV = 6,
    SUB_TASK_COUNT = 32,
}SUB_TASK_ID;

/*
���������ṹ
����������ѭ��ִ�и���������
*/
typedef struct MainTaskControlBlock
{
    DDS_THREAD_ID taskID;
    DDS_THREAD_HANDLE taskHandle;
    MainTaskState taskState;
    char udpRecvBuf[MAX_UDP_MSG_SIZE];
}MainTaskControlBlock;


/*
���������ṹ
*/
typedef struct SubTaskControlBlock
{
    //���������Ƿ�ִ��
    BOOL active;

    //������ִ�н��й����ʱ��ṹ
    HighPrecisionTimeBlock timeBlock;

    //�Ժ���Ϊ��λ������
    unsigned long miliSecPeriod;

    //��΢��Ϊ��λ���ӳ����ޣ�Ӳʵʱ�����������ӳ�������ʧ��
    unsigned long microSecDelayBound;

}SubTaskControlBlock;

extern SubTaskControlBlock g_sub_task_ctrl_blocks[SUB_TASK_COUNT];

extern void InitializeSubTasks();

#endif
