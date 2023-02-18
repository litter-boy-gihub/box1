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
*  @file     TimeTool.h
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

/********************************************************************
TimeTool.h
�߾���ʱ����ص����ݽṹ�ʹ������
********************************************************************/
#ifndef HIPERD_TIMETOOL_H
#define HIPERD_TIMETOOL_H

#if defined(__linux__)
typedef struct  _LARGE_INTEGER
{
    struct {
        DWORD LowPart;
        LONG HighPart;
    } DUMMYSTRUCTNAME;
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
    LONGLONG QuadPart;
}LARGE_INTEGER;
#endif

/*
 �߾���ʱ��ṹ
*/
#if defined(_WIN32)
typedef struct _HighPrecisionTimeBlock
{
    LARGE_INTEGER nBeginTime;
    LARGE_INTEGER nEndTime  ;
    LARGE_INTEGER nFreq     ;
}HighPrecisionTimeBlock;
#elif defined(__linux__) || defined (_VXWORKS)
#if !defined(__cplusplus)
typedef struct _timeval{
    long int tv_sec;
    long int tv_usec;
}timeval;
#endif
typedef struct _HighPrecisionTimeBlock
{
    timeval nBeginTime;
    timeval nEndTime;
}HighPrecisionTimeBlock;
#endif

#if defined (_WIN32)
//��ʼ��һ��ʱ��ṹ
#define HPTB_INIT( BLOCK ) QueryPerformanceFrequency(&(BLOCK.nFreq))
//��ʼ��ʱ
#define HPTB_BEGIN( BLOCK ) QueryPerformanceCounter(&(BLOCK.nBeginTime))
//������ʱ
#define HPTB_END( BLOCK ) QueryPerformanceCounter(&(BLOCK.nEndTime))
//��ȡʱ��ṹ�ж�Ӧ��΢��ʱ����
#define HPTB_INTERVAL_MICROSEC( BLOCK ) (long)((double)( BLOCK.nEndTime.QuadPart- BLOCK.nBeginTime.QuadPart)*1000000.0/(double) BLOCK.nFreq.QuadPart);
#elif defined (__linux__)
/*TODO���޸�linux��bug�����vxworks��֧��*/

#define HPTB_INIT( BLOCK ) gettimeofday(&(BLOCK.nBeginTime),NULL)
//��ʼ��ʱ
#define HPTB_BEGIN( BLOCK ) gettimeofday(&(BLOCK.nBeginTime),NULL)
//������ʱ
#define HPTB_END( BLOCK ) gettimeofday(&(BLOCK.nEndTime),NULL)
#define HPTB_INTERVAL_MICROSEC( BLOCK ) (unsigned long)((double)(BLOCK.nEndTime.tv_sec - BLOCK.nBeginTime.tv_sec) * 1000000 + (BLOCK.nEndTime.tv_usec - BLOCK.nBeginTime.tv_usec))
#elif defined (_VXWORKS)
#define HPTB_INIT( BLOCK ) clock_gettime(CLOCK_REALTIME,&(BLOCK.nBeginTime));BLOCK.nBeginTime.tv_usec /= 1000
#define HPTB_BEGIN( BLOCK ) clock_gettime(CLOCK_REALTIME,&(BLOCK.nBeginTime));BLOCK.nBeginTime.tv_usec /= 1000
#define HPTB_END( BLOCK ) clock_gettime(CLOCK_REALTIME,&(BLOCK.nEndTime));BLOCK.nEndTime.tv_usec /= 1000
#define HPTB_INTERVAL_MICROSEC( BLOCK ) (unsigned long)((double)(BLOCK.nEndTime.tv_sec - BLOCK.nBeginTime.tv_sec) * 1000000 + (BLOCK.nEndTime.tv_usec - BLOCK.nBeginTime.tv_usec))
#endif
//��ȡʱ��ṹ�ж�Ӧ��΢��ʱ����

#endif
