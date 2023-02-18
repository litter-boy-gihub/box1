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
高精度时间相关的数据结构和处理机制
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
 高精度时标结构
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
//初始化一个时标结构
#define HPTB_INIT( BLOCK ) QueryPerformanceFrequency(&(BLOCK.nFreq))
//开始计时
#define HPTB_BEGIN( BLOCK ) QueryPerformanceCounter(&(BLOCK.nBeginTime))
//结束计时
#define HPTB_END( BLOCK ) QueryPerformanceCounter(&(BLOCK.nEndTime))
//获取时标结构中对应的微秒时间间隔
#define HPTB_INTERVAL_MICROSEC( BLOCK ) (long)((double)( BLOCK.nEndTime.QuadPart- BLOCK.nBeginTime.QuadPart)*1000000.0/(double) BLOCK.nFreq.QuadPart);
#elif defined (__linux__)
/*TODO：修复linux下bug，添加vxworks下支持*/

#define HPTB_INIT( BLOCK ) gettimeofday(&(BLOCK.nBeginTime),NULL)
//开始计时
#define HPTB_BEGIN( BLOCK ) gettimeofday(&(BLOCK.nBeginTime),NULL)
//结束计时
#define HPTB_END( BLOCK ) gettimeofday(&(BLOCK.nEndTime),NULL)
#define HPTB_INTERVAL_MICROSEC( BLOCK ) (unsigned long)((double)(BLOCK.nEndTime.tv_sec - BLOCK.nBeginTime.tv_sec) * 1000000 + (BLOCK.nEndTime.tv_usec - BLOCK.nBeginTime.tv_usec))
#elif defined (_VXWORKS)
#define HPTB_INIT( BLOCK ) clock_gettime(CLOCK_REALTIME,&(BLOCK.nBeginTime));BLOCK.nBeginTime.tv_usec /= 1000
#define HPTB_BEGIN( BLOCK ) clock_gettime(CLOCK_REALTIME,&(BLOCK.nBeginTime));BLOCK.nBeginTime.tv_usec /= 1000
#define HPTB_END( BLOCK ) clock_gettime(CLOCK_REALTIME,&(BLOCK.nEndTime));BLOCK.nEndTime.tv_usec /= 1000
#define HPTB_INTERVAL_MICROSEC( BLOCK ) (unsigned long)((double)(BLOCK.nEndTime.tv_sec - BLOCK.nBeginTime.tv_sec) * 1000000 + (BLOCK.nEndTime.tv_usec - BLOCK.nBeginTime.tv_usec))
#endif
//获取时标结构中对应的微秒时间间隔

#endif
