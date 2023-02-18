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
*  @file     RTPSFragInfo.h
*  @brief    RTPSFragInfo
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
RTPSFragInfo.h
�洢ĳ����·�ķ�Ƭ�յ�����Ϣ
********************************************************************/

#ifndef HIPERD_RTPSFRAGINFO_H
#define HIPERD_RTPSFRAGINFO_H

#define RTPSFRAGINFO_MEMSIZE 16

/* TODO �����ʼ��*/
typedef struct RTPSFragInfo
{
    unsigned int        expectTotalFragNum                  ; /* ��Ƭ����                                           */
 //   ReceivedDataSample  deliverRecvSample                   ; /* �ύ������ͷ                                       */
    GUID_t              writerId                            ; /* �Զ�writer                                         */ 
    DisjointSequence    recvFragNum                         ; /* �յ���FragmentNumber,�˴�����DisjointSequence      */
    BOOL                fragDataValid[RTPSFRAGINFO_MEMSIZE] ; /* ����ķ�Ƭ���� ,ֱ����FragmentNumber_t�������±�   */
    MemoryBlock   fragData[RTPSFRAGINFO_MEMSIZE]      ; /* FragmentNumber_t fragDataFN[RTPSFRAGINFO_MEMSIZE]; */ 
    SequenceNumber_t    tmpSeq                              ; /* �����sequenceNumber,����ÿ�ζ�����һ����ʱ����    */
    SequenceNumber_t    upBoundSeq                          ; 
    DisjointSequence    exclude                             ; /* ����Ŀյ�Disjoint,��ԶΪ��                        */
}RTPSFragInfo;

BOOL Initial_RTPSFragInfo(RTPSFragInfo * curInfo);

BOOL returnHeldData_RTPSFragInfo(RTPSFragInfo * curInfo);

BOOL insertNewFrag_RTPSFragInfo(RTPSFragInfo * curInfo, FragmentNumber_t fragNum, MemoryBlock * head);

BOOL isDeliverable_RTPSFragInfo(RTPSFragInfo * curInfo);

#endif
