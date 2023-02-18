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
*  @file     Spdp.h
*  @brief    Spdp
*  Details.
*
*  @author   Hunter
*  @email    1028251654@qq.com
*  @version  1.0
*  @date     2019/08/01
*  @license  
*
*--------------------------------------------------------------------------------
*  Remark         : Description
*--------------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>  | <operation> | <Description>
*--------------------------------------------------------------------------------
*  2019/08/01 | 1.0       | Hunter    | Create file |
*--------------------------------------------------------------------------------
*
*********************************************************************************/

#ifndef HIPERD_SPDP_H
#define HIPERD_SPDP_H

/******************************** 组装SPDP发现报文 ******************************/
extern VOID JointSPDPDataMsg(RTPSMessageHeader* pstRTPSMsgHeader, HistoryData* pstHistoryData, BuiltinDataWriter* pstBuiltinDataWriter, MemoryBlock* pstMemBlock);

/********************************************************************************/
/* 处理SPDP数据报文 */
/********************************************************************************/
extern BOOL HandleSPDPDataMsg(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader*  pstSubMsgHeader, Data* pstDataMsg, MemoryBlock* pstMemBlock, DDS_DomainParticipant* pstParticipant);

/********************************************************************************/
/* 监控相关报文处理 */
/********************************************************************************/
/******************** 处理监控数据报文，打开监控配置时启用 **********************/
extern BOOL HandleMonitorDataMsg(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader*  pstSubMsgHeader, Data* pstDataMsg, MemoryBlock* pstMemBlock, DDS_DomainParticipant* pstParticipant);
/*************************** 处理主题数据报文，打开监控配置时启用 ***********************************/
extern BOOL HandleTopicDataMsg(RTPSMessageHeader* pstRTPSMsgHeader, SubMessageHeader*  pstSubMsgHeader, Data* pstDataMsg, MemoryBlock* pstMemBlock, DDS_DomainParticipant* pstParticipant);

extern VOID JointMonitorDataMsg(DDS_DomainParticipant* pstParticipant, DDS_Monitor*  pstMonitor, MemoryBlock* pstMemBlock);
#endif
