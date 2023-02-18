/*
 * FILE: FC_Adapter.h - Exported FC functions
 *
 * (c) Copyright, Real-Time Innovations, 2008-2014
 *
 * All rights reserved.
 *
 * No duplications, whole or partial, manual or electronic, may be made
 * without express written permission.  Any such copies, or
 * revisions thereof, must display this notice unaltered.
 * This code contains trade secrets of Real-Time Innovations, Inc.
 *
 * Modification History
 * --------------------
 */
 /*ci
  * \addtogroup NETIO_FCInterfaceClass
  * @{
  */

#ifndef FC_Adapter_h
#define FC_Adapter_h

#ifdef __cplusplus
	extern "C" {
#endif

/*用于DDS配置的发送定义结构体*/	
struct FCSendMsgIDType
{
	int destfcid;		/*目的节点标识，自定义 不允许重复*/
	int msgid;			/*消息ID*/
	int ifmc;			/*广播标识 0-单播  1-广播*/
};
extern struct FCSendMsgIDType  fc_send_msgid_table_all[][60];
//extern struct FCSendMsgIDType  fc_send_msgid_table[]; /*The Table of Send FC Msgid */
//extern int sendmsgnum; /*The Num of Send FC MsgID*/

struct FCRecvMsgIDType
{
	int srcfcid;		/*目的节点标识，自定义 不允许重复*/
	int msgid;			/*消息ID*/
	int ifmc;			/*广播标识 0-单播  1-广播*/
};
//extern struct FCRecvMsgIDType fc_recv_msgid_table[]; /*The Table of Recv FC Msgid */
extern struct FCRecvMsgIDType fc_recv_msgid_table_all[][60];
extern int recvmsgnum;  /*The Num of Recv FC MsgID*/


/****************FC配置相关*******************/

/*ASM接收回调定义*/
typedef unsigned int (* FC_ASM_RECVMSG_FUN)(int hDev, unsigned int msgID);/*20191017zzt*/

typedef unsigned int (* FC_NPOpen_FUNCPTR)(int hDev, unsigned int devId);

typedef unsigned int (* FC_NPClose_FUNCPTR)(int hDev);

typedef unsigned int (* FC_SENDMSG_FUNCPTR)(int hDev, unsigned int msgID, unsigned char *pMsg, unsigned int nLen);

typedef unsigned int (* FC_RECVMSG_FUNCPTR)(int hDev, unsigned int msgID, unsigned char *pMsg, unsigned int *nLen);

typedef unsigned int  (* FC_GETGUID_FUNCPTR)(int hDev, unsigned int * pGUID);

typedef unsigned int  (* FC_GETPORTID_FUNCPTR)(int hDev, unsigned int * LocalPortID);

typedef unsigned int  (* FC_NPASMIOCONTROL_FUNCPTR)(int hDev, unsigned int control);

/*注册ASM消息处理函数*/
typedef unsigned int (* FC_REGMSGHANDLE_FUNCPTR)(int hDev, FC_ASM_RECVMSG_FUN func);/*20191017zzt*/

typedef struct{
   int FC_Handle; /*20210107zzt*/
	FC_NPOpen_FUNCPTR   FC_Open;
	FC_NPClose_FUNCPTR  FC_Close;
	FC_SENDMSG_FUNCPTR  FC_SendMsg;
	FC_RECVMSG_FUNCPTR  FC_RecvMsg;
	FC_REGMSGHANDLE_FUNCPTR  FC_RegHandleMsg;/*20191017zzt*/
	FC_GETGUID_FUNCPTR FC_GetGUID;/*20210320 zzt*/
	FC_GETPORTID_FUNCPTR FC_GetLocalPortID;/*20210320 zzt*/
   FC_NPASMIOCONTROL_FUNCPTR FC_AsmIOControl;/*20210419 zzt*/
}FC_DEVICE_DRIVER;
extern FC_DEVICE_DRIVER fc_driver;
/*DevID of FC Card*/
extern int FC_DEVID ;

extern int FC_Interface_Register();

#ifdef __cplusplus
	}                               /* extern "C" */
#endif

#endif
