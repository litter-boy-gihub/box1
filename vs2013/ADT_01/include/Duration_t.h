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
*  @file     Duration.h
*  @brief    Duration
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

#ifndef HIPERD_DURATION_T_H
#define HIPERD_DURATION_T_H

/**
 * ������Duration_tֵ
 */
extern const int            DURATION_INFINITE_SEC   ; /* ������    */
extern const int            DURATION_ZERO_SEC       ; /* 0��       */
extern const unsigned int   DURATION_INFINITE_NSEC  ; /* ��������  */
extern const unsigned int   DURATION_ZERO_NSEC      ; /* 0����     */

/* Duration_t �ṹ�����л���Parameterʱռ�ÿռ�Ĵ�С ��Ҫ�����ֽڶ���*/
#define SIZE_OF_DURATION 8

typedef struct Duration_t
{
    EX_INT32    sec     ; /* ���ֵ        */
    EX_UINT32   nanosec ; /* ��΢���ֵ    */
}Duration_t; 

/* �Ӽ������ͱȽϺ� */
Duration_t Add_Duration(Duration_t left, Duration_t right);
Duration_t Sub_Duration(Duration_t left, Duration_t right);
BOOL Compare_Duration(Duration_t left, Duration_t  right);
BOOL Duration_IsEqual(Duration_t left, Duration_t  right);

#define ISEQUAL_DURATION(left,right) ((left.sec == right.sec)&&(left.nanosec == right.nanosec))

#define ISLESSTHEN_DURATION(left,right) ((left.sec < right.sec)||(left.sec == right.sec && left.nanosec < right.nanosec))

#define ISEQUAL_OR_LESSTHEN_DURATION(left,right) (ISEQUAL_DURATION(left,right) || ISLESSTHEN_DURATION(left,right))

#define ISGRATERTHEN_DURATION(left,right) (!ISEQUAL_OR_LESSTHEN_DURATION(left,right))

#define ISEQUAL_OR_GRATERTHEN_DURATION(left,right) (!ISLESSTHEN_DURATION(left,right))

/*
 ����Ϊ�ýṹ����빤�ߺ�
*/
#define SERIALIZE_DURATION(duration , memBlock)      \
    do                                               \
    {                                                \
        PUT_INT(memBlock, duration.sec)              \
        PUT_UNSIGNED_INT(memBlock, duration.nanosec) \
    } while (0);

#define DESERIALIZE_DURATION(duration , memBlock,exchange)    \
    do                                               \
    {                                                \
        GET_INT(memBlock, duration.sec,exchange)              \
        GET_UNSIGNED_INT(memBlock, duration.nanosec,exchange) \
    } while (0);


#define DURATION_IS_EQUAL(leftDuration , rightDuration) \
    (## leftDuration ##.sec == ## rightDuration ##.sec) && \
    (## leftDuration ##.nanosec == ## rightDuration ##.nanosec)




// RTPSЭ���õ���һЩDuration_t��Ĭ��ֵ
extern const Duration_t INVALID_DURATION                ; /* ��Ч��ֵ */
extern const Duration_t DEFALUT_HB_RESP_DELAY           ; /* HB�ӳ���Ӧʱ�䣨����ACKNACK����100ms */
extern const Duration_t DEFALUT_HB_SUPPRESSION          ; /* HB���ƣ���ֹ������յ�HB, 0s */
extern const Duration_t DEFALUT_MAX_BLOCKING_TIME       ; /* д��HistoryCache������ӳ�ʱ��,100ms */
extern const Duration_t DEFALUT_HB_PERIOD               ; /* ����HB������ */
extern const Duration_t DEFALUT_NACK_RESP_DELAY         ; /* ACKNACK����Ӧ�ӳ�ʱ��,200ms */
extern const Duration_t DEFALUT_NACK_SUPPRESSION        ; /* NACK���ƣ�SENT���Unacknowledged״̬(Э��������)��10ms */
extern const Duration_t DEFALUT_UPDATE_PARTICIPANT      ; /* ����Participant����ʱ�� */
extern const Duration_t DEFALUT_PARTICIPANT_LEASE       ; /* Participant����ʱ��,15s */
extern const Duration_t DEFALUT_CHECK_PARTICIPANT_STALE ; /* ParticipantReader������Participant�����Ϣ�ļ�� */
extern const Duration_t DEFALUT_REFRESH_UNACK           ; /* Writerˢ����Ӧ��ʱ���� */

// �ο�IETF NTP���壬time = seconds+[fraction/(2^32)]
typedef Duration_t Time_t;
// ����
extern const Time_t TIME_ZERO;
extern const Time_t TIME_INVALID;
extern const Time_t TIME_INFINITE;

// ��̬�ĺ��������ص��øú���ʱ������ʱ��
Time_t GetNowTime();


#endif
