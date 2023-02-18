#include "HS_FC/precomp.h"
/*
0 节点0 FCID=0x010000 配置0 作为NCT 发送2个通道（BNC NRT） 接收2个通道（BNC NRT）
1 节点1 FCID=0x010001 配置1 作为BNC 发送2个通道（NC NRT） 接收2个通道（NC NRT）
2 节点2 FCID=0x010002 配置2 作为NRT 发送2个通道（NC BNC） 接收2个通道（NC BNC）
3 节点3 FCID=0x010003 配置3 作为NCT 自环多通道， 发送2个通道，接收2个通道
4 节点4 FCID=0x010004 配置4 作为NCT 自环，发送1个通道 接收1个通道
*/

#define MB_NODE0_FCID			0x010000`

#define MB_NODE1_FCID			0x010002
#define MB_NODE2_FCID			0x010001
#define MB_NODE3_FCID			0x010001

#define MB_NODE4_FCID			0x010004

#define NC_MSG_ID_TO_BNC		1
#define NC_MSG_ID_TO_NRT		1
#define BNC_MSG_ID_TO_NC		3
#define BNC_MSG_ID_TO_NRT		4
#define NRT_MSG_ID_TO_NC		3
#define NRT_MSG_ID_TO_BNC		6
#define NC_MSG_ID_TO_NC_1		7
#define NC_MSG_ID_TO_NC_2		8
#define NC_MSG_ID_TO_NC_3		9
#define NC_MSG_ID_TO_NC_4		10



#define MSG_ID_10		(10)
#define MSG_ID_11		(11)
#define MSG_ID_12		(12)
#define MSG_ID_13		(13)
#define MSG_ID_14		(14)
#define MSG_ID_15		(15)
#define MSG_ID_16		(16)
#define MSG_ID_17		(17)
#define MSG_ID_18		(18)
#define MSG_ID_19		(19)

#define MB_NODE10_FCID			(0x010010)
#define MB_NODE11_FCID			(0x010011)
#define MB_NODE12_FCID			(0x010012)
#define MB_NODE13_FCID			(0x010013)
#define MB_NODE14_FCID			(0x010014)
#define MB_NODE15_FCID			(0x010015)
#define MB_NODE16_FCID			(0x010016)
#define MB_NODE17_FCID			(0x010017)
#define MB_NODE18_FCID			(0x010018)
#define MB_NODE19_FCID			(0x010019)


#define FC_AMS_IU_LENGTH_RECV	(16*1024*1024)


BLUE_PRINT gBluePrint[] =
{

    /* -----------------------------------蓝图1---------------------------------- */
	{
		{
						/* ---------------------------配置块MIU1------------------------- */
			{
				/*端口配置*/
				{
					"MIU1",               /* 本地端口的名称 */
					0x010001,           /* 本地port_id */
					1,                  /* 端口类型 1-ASM */
					8,                  /* 信用 */
					3,                  /* 端口的网络管理角色 1-nc 2-bnc 3-为网络远程终端*/
					2,                  /* 时钟同步角色2-网络时钟客户端*/
					1000,               /* WDT定时周期,狗叫超时*/
					50,                /* WDT加载周期,喂狗周期*/
					500,               /* 时钟同步周期 */
					0,                  /* 非数据块发送消息的总数 1条615A协议*/
					0,                  /* 非数据块接收消息的总数 1条615A协议*/
					67,                  /* 数据块发送消息的总数 */
					67,                  /* 数据块接收消息的总数 1条615A映像文件*/
				},
				/*发送*/
				{
					{
						"MIU1toMIU1Heart", /*消息名称*/
						0X010101,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x010001,/*消息的源ASM端口port_id*/
						0x010001,/*消息的目的ASM端口port_id*/
					},
					{
						"MIU1toMIU1Topic", /*消息名称*/
						0X010102,	 /*消息ID*/
						2,       /*消息类型*/
						1,       /*消息优先级*/
						0x010001,/*消息的源ASM端口port_id*/
						0x010001,/*消息的目的ASM端口port_id*/
					},
					 {
						"MIU1toMIU1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X010100,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x010001,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010001,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					
					{
						"MIU1toDPU1Heart", /*消息名称*/
						0X010C01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x010001,/*消息的源ASM端口port_id*/
						0x01000C,/*消息的目的ASM端口port_id*/
					},
					{
						"MIU1toDPU1Topic", /*消息名称*/
						0X010C02,	 /*消息ID*/
						2,       /*消息类型*/
						1,       /*消息优先级*/
						0x010001,/*消息的源ASM端口port_id*/
						0x01000C,/*消息的目的ASM端口port_id*/
					},
					 {
						"MIU1toDPU1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X010C00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x010001,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000C,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"MIU1toDPU2Heart", /*消息名称*/
						0X010D01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x010001,/*消息的源ASM端口port_id*/
						0x01000D,/*消息的目的ASM端口port_id*/
					},
					{
						"MIU1toDPU2Topic", /*消息名称*/
						0X010D02,	 /*消息ID*/
						2,       /*消息类型*/
						1,       /*消息优先级*/
						0x010001,/*消息的源ASM端口port_id*/
						0x01000D,/*消息的目的ASM端口port_id*/
					},
					 {
						"MIU1toDPU2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X010D00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x010001,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000D,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toDPU4Heart", /*消息名称*/
						0X0C0F01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x01000F,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toDPU4Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0F02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000F,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toDPU4Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0F00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000F,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toDPU5Heart", /*消息名称*/
						0X0C1001,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010010,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toDPU5Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1002,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010010,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toDPU5Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1000,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010010,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toDPU6Heart", /*消息名称*/
						0X0C1101,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010011,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toDPU6Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1102,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010011,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toDPU6Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1100,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010011,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toDPU7Heart", /*消息名称*/
						0X0C0001,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010000,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toDPU7Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0002,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010000,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toDPU7Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0000,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010000,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toMIU1Heart", /*消息名称*/
						0X0C0101,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010001,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toMIU1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0102,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010001,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toMIU1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0100,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010001,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toMIU2Heart", /*消息名称*/
						0x0C0201,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010002,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toMIU2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0x0C0202,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010002,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toMIU2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0x0C0200,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010002,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toIIPHeart", /*消息名称*/
						0x0C0301,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010003,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toIIPTopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0302,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010003,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toIIPData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0300,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010003,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toISP1Heart", /*消息名称*/
						0X0C0401,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010004,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toISP1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0402,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010004,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toISP1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0400,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010004,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toISP2Heart", /*消息名称*/
						0X0C0501,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010005,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toISP2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0502,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010005,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toISP2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0500,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010005,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toOEDEHeart", /*消息名称*/
						0X0C1801,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010018,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toOEDETopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1802,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010018,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toOEDEData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1800,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010018,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU1toINE1Heart", /*消息名称*/
						0X0C1901,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010019,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toINE1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1902,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010019,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toINE1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1900,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010019,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toINE2Heart", /*消息名称*/
						0X0C1A01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x01001A,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toINE2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1A02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001A,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toINE2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1A00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001A,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toAIU1Heart", /*消息名称*/
						0X0C1B01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x01001B,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toAIU1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1B02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001B,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toAIU1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1B00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001B,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU1toAIU2Heart", /*消息名称*/
						0X0C1C01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x01001C,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toAIU2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1C02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001C,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toAIU2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1C00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001C,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toMFD1Heart", /*消息名称*/
						0X0C2401,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010024,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toMFD1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2402,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010024,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toMFD1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2400,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010024,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU1toMFD2Heart", /*消息名称*/
						0X0C2501,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010025,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toMFD2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2502,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010025,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toMFD2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2500,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010025,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toUFCDHeart", /*消息名称*/
						0X0C2601,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010026,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toUFCDTopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2602,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010026,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toUFCDData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2600,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010026,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toDEUHeart", /*消息名称*/
						0X0C2701,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010027,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toDEUTopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2702,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010027,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toDEUData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2700,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010027,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toVIUHeart", /*消息名称*/
						0X0C2801,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010028,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toVIUTopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2802,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010028,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toVIUData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2800,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010028,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					/*615A*/
					{
						"DPU1 protocol",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0179,             /* FCUINT msg_id       发送消息ID */
					  4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010001,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
				},
				/*接收*/
				{

					{
						"MIU1toMIU1Heart",      /* 消息名称 */
						0X010101,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},

					{
						"MIU1toMIU1Topic",      /* 消息名称 */
						0X010102,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},

					{
						"MIU1toMIU1Data",      /* 消息名称 */
						0X010100,        /* 消息ID */
						66536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU1toMIU1Heart",      /* 消息名称 */
						0X0C0101,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},

					{
						"DPU1toMIU1Topic",      /* 消息名称 */
						0X0C0102,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x01000C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},

					{
						"DPU1toMIU1Data",      /* 消息名称 */
						0X0C0100,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},

					{
						"DPU2toMIU1Heart",      /* 消息名称 */
						0X0D0101,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000D,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},

					{
						"DPU2toMIU1Topic",      /* 消息名称 */
						0X0D0102,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x01000D,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},

					{
						"DPU2toMIU1Data",      /* 消息名称 */
						0X0D0100,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000D,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										
					{
						"DPU3toDPU1Heart",      /* 消息名称 */
						0X0E0C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000E,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU3toDPU1Topic",      /* 消息名称 */
						0X0E0C02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x01000E,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU3toDPU1Data",      /* 消息名称 */
						0X0E0C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000E,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU4toDPU1Heart",      /* 消息名称 */
						0X0F0C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000F,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU4toDPU1Topic",      /* 消息名称 */
						0X0F0C02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x01000F,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU4toDPU1Data",      /* 消息名称 */
						0X0F0C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000F,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU5toDPU1Heart",      /* 消息名称 */
						0X100C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010010,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU5toDPU1Topic",      /* 消息名称 */
						0X100C02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x010010,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU5toDPU1Data",      /* 消息名称 */
						0X100C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010010,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU6toDPU1Heart",      /* 消息名称 */
						0X110C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010011,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU6toDPU1Topic",      /* 消息名称 */
						0X110C02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x010011,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU6toDPU1Data",      /* 消息名称 */
						0X110C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010011,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU7toDPU1Heart",      /* 消息名称 */
						0X000C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010000,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU7toDPU1Topic",      /* 消息名称 */
						0X000C02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x010000,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU7toDPU1Data",      /* 消息名称 */
						0X000C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010000,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"MIU1toDPU1Heart",      /* 消息名称 */
						0X010C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MIU1toDPU1Topic",      /* 消息名称 */
						0X010C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MIU1toDPU1Data",      /* 消息名称 */
						0X010C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"MIU2toDPU1Heart",      /* 消息名称 */
						0X020C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010002,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MIU2toDPU1Topic",      /* 消息名称 */
						0X020C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010002,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MIU2toDPU1Data",      /* 消息名称 */
						0X020C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010002,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"IIPtoDPU1Heart",      /* 消息名称 */
						0X030C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010003,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"IIPtoDPU1Topic",      /* 消息名称 */
						0X030C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010003,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"IIPtoDPU1Data",      /* 消息名称 */
						0X030C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010003,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"ISP1toDPU1Heart",      /* 消息名称 */
						0X040C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010004,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"ISP1toDPU1Topic",      /* 消息名称 */
						0X040C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010004,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"ISP1toDPU1Data",      /* 消息名称 */
						0X040C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010004,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"ISP2toDPU1Heart",      /* 消息名称 */
						0X050C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010005,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"ISP2toDPU1Topic",      /* 消息名称 */
						0X050C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010005,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"ISP2toDPU1Data",      /* 消息名称 */
						0X050C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010005,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"OEDEtoDPU1Heart",      /* 消息名称 */
						0X180C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010018,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"OEDEtoDPU1Topic",      /* 消息名称 */
						0X180C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010018,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"OEDEtoDPU1Data",      /* 消息名称 */
						0X180C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010018,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"INE1toDPU1Heart",      /* 消息名称 */
						0X190C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010019,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"INE1toDPU1Topic",      /* 消息名称 */
						0X190C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010019,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"INE1toDPU1Data",      /* 消息名称 */
						0X190C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010019,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"INE2toDPU1Heart",      /* 消息名称 */
						0X1A0C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001A,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"INE2toDPU1Topic",      /* 消息名称 */
						0X1A0C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x01001A,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"INE2toDPU1Data",      /* 消息名称 */
						0X1A0C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001A,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"AIU1toDPU1Heart",      /* 消息名称 */
						0X1B0C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001B,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"AIU1toDPU1Topic",      /* 消息名称 */
						0X1B0C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x01001B,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"AIU1toDPU1Data",      /* 消息名称 */
						0X1B0C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001B,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"AIU2toDPU1Heart",      /* 消息名称 */
						0X1C0C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"AIU2toDPU1Topic",      /* 消息名称 */
						0X1C0C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x01001C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"AIU2toDPU1Data",      /* 消息名称 */
						0X1C0C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"MFD1toDPU1Heart",      /* 消息名称 */
						0X240C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010024,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MFD1toDPU1Topic",      /* 消息名称 */
						0X240C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010024,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MFD1toDPU1Data",      /* 消息名称 */
						0X240C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010024,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"MFD2toDPU1Heart",      /* 消息名称 */
						0X250C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010025,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MFD2toDPU1Topic",      /* 消息名称 */
						0X250C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010025,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MFD2toDPU1Data",      /* 消息名称 */
						0X250C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010025,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"UFCDtoDPU1Heart",      /* 消息名称 */
						0X260C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010026,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"UFCDtoDPU1Topic",      /* 消息名称 */
						0X260C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010026,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"UFCDtoDPU1Data",      /* 消息名称 */
						0X260C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010026,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DEUtoDPU1Heart",      /* 消息名称 */
						0X270C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010027,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DEUtoDPU1Topic",      /* 消息名称 */
						0X270C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010027,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DEUtoDPU1Data",      /* 消息名称 */
						0X270C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010027,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"VIUtoDPU1Heart",      /* 消息名称 */
						0X280C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010028,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"VIUtoDPU1Topic",      /* 消息名称 */
						0X280C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010028,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"VIUtoDPU1Data",      /* 消息名称 */
						0X280C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010028,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										/*615A*/
										{
						"DPU1 image",      /* 消息名称 */
						0X010C78,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},

				},
			},
			/* ---------------------------配置块1-1------------------------- */
			{
				/*端口配置*/
				{
					"DPU1",               /* 本地端口的名称 */
					0x01000C,           /* 本地port_id */
					1,                  /* 端口类型 1-ASM */
					8,                  /* 信用 */
					2,                  /* 端口的网络管理角色 1-nc 2-bnc 3-为网络远程终端*/
					2,                  /* 时钟同步角色2-网络时钟客户端*/
					1000,               /* WDT定时周期,狗叫超时*/
					50,                /* WDT加载周期,喂狗周期*/
					500,               /* 时钟同步周期 */
					0,                  /* 非数据块发送消息的总数 1条615A协议*/
					0,                  /* 非数据块接收消息的总数 1条615A协议*/
					67,                  /* 数据块发送消息的总数 */
					67,                  /* 数据块接收消息的总数 1条615A映像文件*/
				},
				/*发送*/
				{
					{
						"DPU1toDPU1Heart", /*消息名称*/
						0X0C0C01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x01000C,/*消息的目的ASM端口port_id*/
					},
					{
						"DPU1toDPU1Topic", /*消息名称*/
						0X0C0C02,	 /*消息ID*/
						2,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x01000C,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toDPU1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0C00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000C,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toDPU2Heart", /*消息名称*/
						0X0C0D01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x01000D,/*消息的目的ASM端口port_id*/
					},
					{
						"DPU1toDPU2Topic", /*消息名称*/
						0X0C0D02,	 /*消息ID*/
						2,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x01000D,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toDPU2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0D00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000D,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toDPU3Heart", /*消息名称*/
						0X0C0E01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x01000E,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toDPU3Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0E02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000E,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toDPU3Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0E00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000E,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toDPU4Heart", /*消息名称*/
						0X0C0F01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x01000F,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toDPU4Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0F02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000F,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toDPU4Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0F00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000F,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toDPU5Heart", /*消息名称*/
						0X0C1001,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010010,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toDPU5Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1002,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010010,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toDPU5Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1000,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010010,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toDPU6Heart", /*消息名称*/
						0X0C1101,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010011,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toDPU6Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1102,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010011,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toDPU6Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1100,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010011,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toDPU7Heart", /*消息名称*/
						0X0C0001,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010000,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toDPU7Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0002,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010000,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toDPU7Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0000,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010000,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toMIU1Heart", /*消息名称*/
						0X0C0101,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010001,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toMIU1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0102,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010001,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toMIU1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0100,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010001,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toMIU2Heart", /*消息名称*/
						0x0C0201,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010002,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toMIU2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0x0C0202,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010002,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toMIU2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0x0C0200,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010002,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toIIPHeart", /*消息名称*/
						0x0C0301,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010003,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toIIPTopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0302,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010003,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toIIPData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0300,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010003,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toISP1Heart", /*消息名称*/
						0X0C0401,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010004,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toISP1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0402,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010004,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toISP1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0400,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010004,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toISP2Heart", /*消息名称*/
						0X0C0501,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010005,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toISP2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0502,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010005,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toISP2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0500,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010005,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toOEDEHeart", /*消息名称*/
						0X0C1801,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010018,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toOEDETopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1802,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010018,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toOEDEData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1800,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010018,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU1toINE1Heart", /*消息名称*/
						0X0C1901,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010019,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toINE1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1902,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010019,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toINE1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1900,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010019,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toINE2Heart", /*消息名称*/
						0X0C1A01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x01001A,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toINE2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1A02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001A,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toINE2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1A00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001A,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toAIU1Heart", /*消息名称*/
						0X0C1B01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x01001B,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toAIU1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1B02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001B,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toAIU1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1B00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001B,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU1toAIU2Heart", /*消息名称*/
						0X0C1C01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x01001C,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toAIU2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1C02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001C,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toAIU2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C1C00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001C,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toMFD1Heart", /*消息名称*/
						0X0C2401,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010024,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toMFD1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2402,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010024,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toMFD1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2400,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010024,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU1toMFD2Heart", /*消息名称*/
						0X0C2501,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010025,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toMFD2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2502,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010025,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toMFD2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2500,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010025,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toUFCDHeart", /*消息名称*/
						0X0C2601,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010026,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toUFCDTopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2602,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010026,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toUFCDData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2600,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010026,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toDEUHeart", /*消息名称*/
						0X0C2701,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010027,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toDEUTopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2702,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010027,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toDEUData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2700,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010027,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU1toVIUHeart", /*消息名称*/
						0X0C2801,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000C,/*消息的源ASM端口port_id*/
						0x010028,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU1toVIUTopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2802,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010028,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU1toVIUData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C2800,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010028,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					/*615A*/
					{
						"DPU1 protocol",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0C0179,             /* FCUINT msg_id       发送消息ID */
					  4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000C,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010001,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
				},
				/*接收*/
				{

					{
						"DPU1toDPU1Heart",      /* 消息名称 */
						0X0C0C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},

					{
						"DPU1toDPU1Topic",      /* 消息名称 */
						0X0C0C02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x01000C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},

					{
						"DPU1toDPU1Data",      /* 消息名称 */
						0X0C0C00,        /* 消息ID */
						66536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU2toDPU1Heart",      /* 消息名称 */
						0X0D0C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000D,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},

					{
						"DPU2toDPU1Topic",      /* 消息名称 */
						0X0D0C02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x01000D,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},

					{
						"DPU2toDPU1Data",      /* 消息名称 */
						0X0D0C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000D,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU3toDPU1Heart",      /* 消息名称 */
						0X0E0C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000E,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU3toDPU1Topic",      /* 消息名称 */
						0X0E0C02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x01000E,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU3toDPU1Data",      /* 消息名称 */
						0X0E0C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000E,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU4toDPU1Heart",      /* 消息名称 */
						0X0F0C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000F,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU4toDPU1Topic",      /* 消息名称 */
						0X0F0C02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x01000F,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU4toDPU1Data",      /* 消息名称 */
						0X0F0C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000F,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU5toDPU1Heart",      /* 消息名称 */
						0X100C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010010,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU5toDPU1Topic",      /* 消息名称 */
						0X100C02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x010010,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU5toDPU1Data",      /* 消息名称 */
						0X100C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010010,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU6toDPU1Heart",      /* 消息名称 */
						0X110C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010011,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU6toDPU1Topic",      /* 消息名称 */
						0X110C02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x010011,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU6toDPU1Data",      /* 消息名称 */
						0X110C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010011,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU7toDPU1Heart",      /* 消息名称 */
						0X000C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010000,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU7toDPU1Topic",      /* 消息名称 */
						0X000C02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x010000,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU7toDPU1Data",      /* 消息名称 */
						0X000C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010000,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"MIU1toDPU1Heart",      /* 消息名称 */
						0X010C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MIU1toDPU1Topic",      /* 消息名称 */
						0X010C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MIU1toDPU1Data",      /* 消息名称 */
						0X010C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"MIU2toDPU1Heart",      /* 消息名称 */
						0X020C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010002,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MIU2toDPU1Topic",      /* 消息名称 */
						0X020C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010002,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MIU2toDPU1Data",      /* 消息名称 */
						0X020C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010002,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"IIPtoDPU1Heart",      /* 消息名称 */
						0X030C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010003,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"IIPtoDPU1Topic",      /* 消息名称 */
						0X030C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010003,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"IIPtoDPU1Data",      /* 消息名称 */
						0X030C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010003,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"ISP1toDPU1Heart",      /* 消息名称 */
						0X040C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010004,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"ISP1toDPU1Topic",      /* 消息名称 */
						0X040C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010004,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"ISP1toDPU1Data",      /* 消息名称 */
						0X040C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010004,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"ISP2toDPU1Heart",      /* 消息名称 */
						0X050C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010005,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"ISP2toDPU1Topic",      /* 消息名称 */
						0X050C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010005,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"ISP2toDPU1Data",      /* 消息名称 */
						0X050C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010005,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"OEDEtoDPU1Heart",      /* 消息名称 */
						0X180C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010018,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"OEDEtoDPU1Topic",      /* 消息名称 */
						0X180C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010018,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"OEDEtoDPU1Data",      /* 消息名称 */
						0X180C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010018,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"INE1toDPU1Heart",      /* 消息名称 */
						0X190C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010019,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"INE1toDPU1Topic",      /* 消息名称 */
						0X190C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010019,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"INE1toDPU1Data",      /* 消息名称 */
						0X190C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010019,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"INE2toDPU1Heart",      /* 消息名称 */
						0X1A0C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001A,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"INE2toDPU1Topic",      /* 消息名称 */
						0X1A0C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x01001A,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"INE2toDPU1Data",      /* 消息名称 */
						0X1A0C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001A,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"AIU1toDPU1Heart",      /* 消息名称 */
						0X1B0C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001B,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"AIU1toDPU1Topic",      /* 消息名称 */
						0X1B0C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x01001B,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"AIU1toDPU1Data",      /* 消息名称 */
						0X1B0C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001B,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"AIU2toDPU1Heart",      /* 消息名称 */
						0X1C0C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"AIU2toDPU1Topic",      /* 消息名称 */
						0X1C0C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x01001C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"AIU2toDPU1Data",      /* 消息名称 */
						0X1C0C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"MFD1toDPU1Heart",      /* 消息名称 */
						0X240C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010024,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MFD1toDPU1Topic",      /* 消息名称 */
						0X240C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010024,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MFD1toDPU1Data",      /* 消息名称 */
						0X240C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010024,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"MFD2toDPU1Heart",      /* 消息名称 */
						0X250C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010025,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MFD2toDPU1Topic",      /* 消息名称 */
						0X250C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010025,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MFD2toDPU1Data",      /* 消息名称 */
						0X250C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010025,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"UFCDtoDPU1Heart",      /* 消息名称 */
						0X260C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010026,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"UFCDtoDPU1Topic",      /* 消息名称 */
						0X260C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010026,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"UFCDtoDPU1Data",      /* 消息名称 */
						0X260C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010026,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DEUtoDPU1Heart",      /* 消息名称 */
						0X270C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010027,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DEUtoDPU1Topic",      /* 消息名称 */
						0X270C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010027,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DEUtoDPU1Data",      /* 消息名称 */
						0X270C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010027,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"VIUtoDPU1Heart",      /* 消息名称 */
						0X280C01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010028,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"VIUtoDPU1Topic",      /* 消息名称 */
						0X280C02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010028,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"VIUtoDPU1Data",      /* 消息名称 */
						0X280C00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010028,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										/*615A*/
										{
						"DPU1 image",      /* 消息名称 */
						0X010C78,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},

					},
			},

			/* ---------------------------配置块1-2------------------------- */
			{
				/*端口配置*/
				{
					"DPU2",               /* 本地端口的名称 */
					0x01000D,           /* 本地port_id */
					1,                  /* 端口类型 1-ASM */
					8,                  /* 信用 */
					2,                  /* 端口的网络管理角色3-为网络远程终端*/
					2,                  /* 时钟同步角色2-网络时钟客户端*/
					1500,               /* WDT定时周期*/
					50,               	/* WDT加载周期*/
					500,               	/* 时钟同步周期 */
					0,                  /* 非数据块发送消息的总数 1条615A协议*/
					0,                  /* 非数据块接收消息的总数 1条615A协议*/
					67,                  /* 数据块发送消息的总数 */
					67,                  /* 数据块接收消息的总数 1条615A映像文件*/
				},
				/*发送*/
				{
					{
						"DPU2toDPU1Heart", /*消息名称*/
						0X0D0C01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x01000C,/*消息的目的ASM端口port_id*/
					},

					 {
						"DPU2toDPU1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0C02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000C,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toDPU1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0C00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000C,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU2toDPU2Heart", /*消息名称*/
						0X0D0D01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x01000D,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toDPU2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0D02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000D,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toDPU2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0D00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000D,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU2toDPU3Heart", /*消息名称*/
						0X0D0E01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x01000E,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toDPU3Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0E02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000E,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toDPU3Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0E00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000E,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU2toDPU4Heart", /*消息名称*/
						0X0D0F01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x01000F,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toDPU4Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0F02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000F,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toDPU4Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0F00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000F,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU2toDPU5Heart", /*消息名称*/
						0X0D1001,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x010010,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toDPU5Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D1002,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010010,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toDPU5Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D1000,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010010,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU2toDPU6Heart", /*消息名称*/
						0X0D1101,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x010011,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toDPU6Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D1102,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010011,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toDPU6Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D1100,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010011,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU2toDPU7Heart", /*消息名称*/
						0X0D0001,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x010000,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toDPU7Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0002,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010000,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toDPU7Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0000,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010000,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU2toMIU1Heart", /*消息名称*/
						0X0D0101,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x010001,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toMIU1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0102,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010001,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toMIU1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0100,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010001,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU2toMIU2Heart", /*消息名称*/
						0x0D0201,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x010002,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toMIU2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0x0D0202,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010002,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toMIU2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0x0D0200,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010002,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU2toIIPHeart", /*消息名称*/
						0x0D0301,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x010003,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toIIPTopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0302,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010003,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toIIPData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0300,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010003,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU2toISP1Heart", /*消息名称*/
						0X0D0401,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x010004,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toISP1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0402,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010004,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toISP1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0400,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010004,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU2toISP2Heart", /*消息名称*/
						0X0D0501,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x010005,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toISP2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0502,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010005,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toISP2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0500,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010005,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU2toOEDEHeart", /*消息名称*/
						0X0D1801,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x010018,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toOEDETopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D1802,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010018,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toOEDEData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D1800,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010018,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU2toINE1Heart", /*消息名称*/
						0X0D1901,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x010019,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toINE1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D1902,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010019,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toINE1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D1900,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010019,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU2toINE2Heart", /*消息名称*/
						0X0D1A01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x01001A,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toINE2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D1A02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001A,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toINE2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D1A00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001A,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU2toAIU1Heart", /*消息名称*/
						0X0D1B01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x01001B,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toAIU1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D1B02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001B,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toAIU1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D1B00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001B,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU2toAIU2Heart", /*消息名称*/
						0X0D1C01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x01001C,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toAIU2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D1C02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001C,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toAIU2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D1C00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001C,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU2toMFD1Heart", /*消息名称*/
						0X0D2401,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x010024,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toMFD1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D2402,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010024,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toMFD1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D2400,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010024,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU2toMFD2Heart", /*消息名称*/
						0X0D2501,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x010025,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toMFD2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D2502,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010025,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toMFD2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D2500,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010025,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU2toUFCDHeart", /*消息名称*/
						0X0D2601,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x010026,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toUFCDTopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D2602,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010026,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toUFCDData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D2600,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010026,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU2toDEUHeart", /*消息名称*/
						0X0D2701,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x010027,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toDEUTopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D2702,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010027,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toDEUData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D2700,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010027,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU2toVIUHeart", /*消息名称*/
						0X0D2801,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000D,/*消息的源ASM端口port_id*/
						0x010028,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU2toVIUTopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D2802,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010028,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU2toVIUData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D2800,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010028,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
												/*615A*/
					{
						"DPU2 protocol",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0D0179,             /* FCUINT msg_id       发送消息ID */
					  4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000D,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010001,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
				},
				/*接收*/
				{

					{
						"DPU1toDPU2Heart",      /* 消息名称 */
						0X0C0D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU1toDPU2Topic",      /* 消息名称 */
						0X0C0D02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x01000C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU1toDPU2Data",      /* 消息名称 */
						0X0C0D00,        /* 消息ID */
						66536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU2toDPU2Heart",      /* 消息名称 */
						0X0D0D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000D,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU2toDPU2Topic",      /* 消息名称 */
						0X0D0D02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x01000D,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU2toDPU2Data",      /* 消息名称 */
						0X0D0D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000D,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU3toDPU2Heart",      /* 消息名称 */
						0X0E0D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000E,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU3toDPU2Topic",      /* 消息名称 */
						0X0E0D02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x01000E,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU3toDPU2Data",      /* 消息名称 */
						0X0E0D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000E,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU4toDPU2Heart",      /* 消息名称 */
						0X0F0D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000F,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU4toDPU2Topic",      /* 消息名称 */
						0X0F0D02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x01000F,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU4toDPU2Data",      /* 消息名称 */
						0X0F0D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000F,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU5toDPU2Heart",      /* 消息名称 */
						0X100D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010010,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU5toDPU2Topic",      /* 消息名称 */
						0X100D02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x010010,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU5toDPU2Data",      /* 消息名称 */
						0X100D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010010,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU6toDPU2Heart",      /* 消息名称 */
						0X110D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010011,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU6toDPU2Topic",      /* 消息名称 */
						0X110D02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x010011,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU6toDPU2Data",      /* 消息名称 */
						0X110D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010011,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU7toDPU2Heart",      /* 消息名称 */
						0X000D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010000,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU7toDPU2Topic",      /* 消息名称 */
						0X000D02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x010000,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU7toDPU2Data",      /* 消息名称 */
						0X000D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010000,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"MIU1toDPU2Heart",      /* 消息名称 */
						0X010D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MIU1toDPU2Topic",      /* 消息名称 */
						0X010D02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MIU1toDPU2Data",      /* 消息名称 */
						0X010D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"MIU2toDPU2Heart",      /* 消息名称 */
						0X020D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010002,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MIU2toDPU2Topic",      /* 消息名称 */
						0X020D02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010002,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MIU2toDPU2Data",      /* 消息名称 */
						0X020D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010002,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"IIPtoDPU2Heart",      /* 消息名称 */
						0X030D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010003,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"IIPtoDPU2Topic",      /* 消息名称 */
						0X030D02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010003,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"IIPtoDPU2Data",      /* 消息名称 */
						0X030D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010003,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"ISP1toDPU2Heart",      /* 消息名称 */
						0X040D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010004,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"ISP1toDPU2Topic",      /* 消息名称 */
						0X040D02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010004,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"ISP1toDPU2Data",      /* 消息名称 */
						0X040D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010004,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"ISP2toDPU2Heart",      /* 消息名称 */
						0X050D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010005,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"ISP2toDPU2Topic",      /* 消息名称 */
						0X050D02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010005,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"ISP2toDPU2Data",      /* 消息名称 */
						0X050D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010005,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"OEDEtoDPU2Heart",      /* 消息名称 */
						0X180D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010018,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"OEDEtoDPU2Topic",      /* 消息名称 */
						0X180D02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010018,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"OEDEtoDPU2Data",      /* 消息名称 */
						0X180D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010018,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"INE1toDPU2Heart",      /* 消息名称 */
						0X190D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010019,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"INE1toDPU2Topic",      /* 消息名称 */
						0X190D02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010019,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"INE1toDPU2Data",      /* 消息名称 */
						0X190D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010019,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"INE2toDPU2Heart",      /* 消息名称 */
						0X1A0D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001A,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"INE2toDPU2Topic",      /* 消息名称 */
						0X1A0D02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x01001A,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"INE2toDPU2Data",      /* 消息名称 */
						0X1A0D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001A,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"AIU1toDPU2Heart",      /* 消息名称 */
						0X1B0D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001B,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"AIU1toDPU2Topic",      /* 消息名称 */
						0X1B0D02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x01001B,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"AIU1toDPU2Data",      /* 消息名称 */
						0X1B0D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001B,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"AIU2toDPU2Heart",      /* 消息名称 */
						0X1C0D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"AIU2toDPU2Topic",      /* 消息名称 */
						0X1C0D02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x01001C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"AIU2toDPU2Data",      /* 消息名称 */
						0X1C0D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},

										{
						"MFD1toDPU2Heart",      /* 消息名称 */
						0X240D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010024,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MFD1toDPU2Topic",      /* 消息名称 */
						0X240D02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010024,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MFD1toDPU2Data",      /* 消息名称 */
						0X240D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010024,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"MFD2toDPU2Heart",      /* 消息名称 */
						0X250D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010025,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MFD2toDPU2Topic",      /* 消息名称 */
						0X250D02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010025,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MFD2toDPU2Data",      /* 消息名称 */
						0X250D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010025,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"UFCDtoDPU2Heart",      /* 消息名称 */
						0X260D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010026,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"UFCDtoDPU2Topic",      /* 消息名称 */
						0X260D02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010026,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"UFCDtoDPU2Data",      /* 消息名称 */
						0X260D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010026,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DEUtoDPU2Heart",      /* 消息名称 */
						0X270D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010027,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DEUtoDPU2Topic",      /* 消息名称 */
						0X270D02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010027,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DEUtoDPU2Data",      /* 消息名称 */
						0X270D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010027,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"VIUtoDPU2Heart",      /* 消息名称 */
						0X280D01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010028,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"VIUtoDPU2Topic",      /* 消息名称 */
						0X280D02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010028,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"VIUtoDPU2Data",      /* 消息名称 */
						0X280D00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010028,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										/*615A*/
					{
						"DPU2 image",      /* 消息名称 */
						0X010D78,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					},
			},
			/* ---------------------------配置块1-3------------------------- */
			{
				/*端口配置*/
				{
					"DPU3",               /* 本地端口的名称 */
					0x01000E,           /* 本地port_id */
					1,                  /* 端口类型 1-ASM */
					8,                  /* 信用 */
					3,                  /* 端口的网络管理角色3-为网络远程终端*/
					2,                  /* 时钟同步角色2-网络时钟客户端*/
					1000,               /* WDT定时周期*/
					400,                /* WDT加载周期*/
					500,               /* 时钟同步周期 */
					0,                  /* 非数据块发送消息的总数 1条615A协议*/
					0,                  /* 非数据块接收消息的总数 1条615A协议*/
					67,                  /* 数据块发送消息的总数 */
					67,                  /* 数据块接收消息的总数 1条615A映像文件*/
				},
				/*发送*/
				{
					{
						"DPU3toDPU1Heart", /*消息名称*/
						0X0E0C01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x01000C,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toDPU1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0C02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000C,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toDPU1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0C00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000C,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU3toDPU2Heart", /*消息名称*/
						0X0E0D01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x01000D,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toDPU2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0D02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000D,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toDPU2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0D00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000D,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU3toDPU3Heart", /*消息名称*/
						0X0E0E01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x01000E,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toDPU3Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0E02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000E,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toDPU3Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0E00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000E,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU3toDPU4Heart", /*消息名称*/
						0X0E0F01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x01000F,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toDPU4Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0F02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000F,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toDPU4Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0F00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01000F,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU3toDPU5Heart", /*消息名称*/
						0X0E1001,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x010010,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toDPU5Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E1002,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010010,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toDPU5Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E1000,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010010,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU3toDPU6Heart", /*消息名称*/
						0X0E1101,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x010011,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toDPU6Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E1102,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010011,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toDPU6Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E1100,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010011,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU3toDPU7Heart", /*消息名称*/
						0X0E0001,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x010000,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toDPU7Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0002,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010000,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toDPU7Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0000,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010000,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU3toMIU1Heart", /*消息名称*/
						0X0E0101,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x010001,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toMIU1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0102,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010001,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toMIU1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0100,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010001,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU3toMIU2Heart", /*消息名称*/
						0x0E0201,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x010002,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toMIU2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0x0E0202,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010002,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toMIU2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0x0E0200,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010002,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU3toIIPHeart", /*消息名称*/
						0x0E0301,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x010003,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toIIPTopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0302,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010003,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toIIPData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0300,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010003,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU3toISP1Heart", /*消息名称*/
						0X0E0401,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x010004,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toISP1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0402,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010004,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toISP1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0400,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010004,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU3toISP2Heart", /*消息名称*/
						0X0E0501,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x010005,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toISP2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0502,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010005,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toISP2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0500,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010005,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU3toOEDEHeart", /*消息名称*/
						0X0E1801,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x010018,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toOEDETopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E1802,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010018,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toOEDEData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E1800,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010018,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU3toINE1Heart", /*消息名称*/
						0X0E1901,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x010019,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toINE1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E1902,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010019,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toINE1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E1900,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010019,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU3toINE2Heart", /*消息名称*/
						0X0E1A01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x01001A,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toINE2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E1A02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001A,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toINE2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E1A00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001A,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU3toAIU1Heart", /*消息名称*/
						0X0E1B01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x01001B,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toAIU1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E1B02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001B,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toAIU1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E1B00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001B,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU3toAIU2Heart", /*消息名称*/
						0X0E1C01,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x01001C,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toAIU2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E1C02,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001C,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toAIU2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E1C00,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x01001C,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					{
						"DPU3toMFD1Heart", /*消息名称*/
						0X0E2401,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x010024,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toMFD1Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E2402,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010024,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toMFD1Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E2400,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010024,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU3toMFD2Heart", /*消息名称*/
						0X0E2501,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x010025,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toMFD2Topic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E2502,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010025,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toMFD2Data",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E2500,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010025,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU3toUFCDHeart", /*消息名称*/
						0X0E2601,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x010026,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toUFCDTopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E2602,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010026,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toUFCDData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E2600,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010026,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU3toDEUHeart", /*消息名称*/
						0X0E2701,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x010027,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toDEUTopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E2702,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010027,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toDEUData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E2700,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010027,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
										{
						"DPU3toVIUHeart", /*消息名称*/
						0X0E2801,	 /*消息ID*/
						4,       /*消息类型*/
						1,       /*消息优先级*/
						0x01000E,/*消息的源ASM端口port_id*/
						0x010028,/*消息的目的ASM端口port_id*/
					},
					 {
						"DPU3toVIUTopic",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E2802,             /* FCUINT msg_id       发送消息ID */
						2,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010028,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
					 {
						"DPU3toVIUData",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E2800,             /* FCUINT msg_id       发送消息ID */
						4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010028,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
												/*615A*/
					{
						"DPU3 protocol",  /* "send asmid=1",*/   /* 消息的名称 */
						0X0E0179,             /* FCUINT msg_id       发送消息ID */
					  4,                 /* FCINT  msg_type     消息类型 */
						1,               /* FCINT  msg_priority 消息优先级 */
						0x01000E,      /* FCINT  msg_src_id   消息的源ASM端口port_id */
						0x010001,    /* FCINT  msg_dst_id   消息的目的ASM端口port_id */
					},
				},
				/*接收*/
				{

					{
						"DPU1toDPU3Heart",      /* 消息名称 */
						0X0C0E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU1toDPU3Topic",      /* 消息名称 */
						0X0C0E02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x01000C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU1toDPU3Data",      /* 消息名称 */
						0X0C0E00,        /* 消息ID */
						66536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU2toDPU3Heart",      /* 消息名称 */
						0X0D0E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000D,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU2toDPU3Topic",      /* 消息名称 */
						0X0D0E02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x01000D,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU2toDPU3Data",      /* 消息名称 */
						0X0D0E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000D,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU3toDPU3Heart",      /* 消息名称 */
						0X0E0E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000E,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU3toDPU3Topic",      /* 消息名称 */
						0X0E0E02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x01000E,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU3toDPU3Data",      /* 消息名称 */
						0X0E0E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000E,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU4toDPU3Heart",      /* 消息名称 */
						0X0F0E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000F,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU4toDPU3Topic",      /* 消息名称 */
						0X0F0E02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x01000F,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU4toDPU3Data",      /* 消息名称 */
						0X0F0E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01000F,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU5toDPU3Heart",      /* 消息名称 */
						0X100E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010010,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU5toDPU3Topic",      /* 消息名称 */
						0X100E02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x010010,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU5toDPU3Data",      /* 消息名称 */
						0X100E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010010,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU6toDPU3Heart",      /* 消息名称 */
						0X110E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010011,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU6toDPU3Topic",      /* 消息名称 */
						0X110E02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x010011,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU6toDPU3Data",      /* 消息名称 */
						0X110E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010011,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DPU7toDPU3Heart",      /* 消息名称 */
						0X000E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010000,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU7toDPU3Topic",      /* 消息名称 */
						0X000E02,        /* 消息ID */
						2048, 		  /* */
						2,            /* 消息类型  事件 */
						0x010000,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DPU7toDPU3Data",      /* 消息名称 */
						0X000E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010000,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"MIU1toDPU3Heart",      /* 消息名称 */
						0X010E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MIU1toDPU3Topic",      /* 消息名称 */
						0X010E02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MIU1toDPU3Data",      /* 消息名称 */
						0X010E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"MIU2toDPU3Heart",      /* 消息名称 */
						0X020E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010002,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MIU2toDPU3Topic",      /* 消息名称 */
						0X020E02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010002,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MIU2toDPU3Data",      /* 消息名称 */
						0X020E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010002,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"IIPtoDPU3Heart",      /* 消息名称 */
						0X030E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010003,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"IIPtoDPU3Topic",      /* 消息名称 */
						0X030E02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010003,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"IIPtoDPU3Data",      /* 消息名称 */
						0X030E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010003,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"ISP1toDPU3Heart",      /* 消息名称 */
						0X040E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010004,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"ISP1toDPU3Topic",      /* 消息名称 */
						0X040E02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010004,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"ISP1toDPU3Data",      /* 消息名称 */
						0X040E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010004,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"ISP2toDPU3Heart",      /* 消息名称 */
						0X050E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010005,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"ISP2toDPU3Topic",      /* 消息名称 */
						0X050E02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010005,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"ISP2toDPU3Data",      /* 消息名称 */
						0X050E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010005,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"OEDEtoDPU3Heart",      /* 消息名称 */
						0X180E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010018,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"OEDEtoDPU3Topic",      /* 消息名称 */
						0X180E02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010018,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"OEDEtoDPU3Data",      /* 消息名称 */
						0X180E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010018,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"INE1toDPU3Heart",      /* 消息名称 */
						0X190E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010019,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"INE1toDPU3Topic",      /* 消息名称 */
						0X190E02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010019,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"INE1toDPU3Data",      /* 消息名称 */
						0X190E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010019,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"INE2toDPU3Heart",      /* 消息名称 */
						0X1A0E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001A,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"INE2toDPU3Topic",      /* 消息名称 */
						0X1A0E02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x01001A,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"INE2toDPU3Data",      /* 消息名称 */
						0X1A0E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001A,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"AIU1toDPU3Heart",      /* 消息名称 */
						0X1B0E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001B,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"AIU1toDPU3Topic",      /* 消息名称 */
						0X1B0E02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x01001B,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"AIU1toDPU3Data",      /* 消息名称 */
						0X1B0E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001B,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"AIU2toDPU3Heart",      /* 消息名称 */
						0X1C0E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"AIU2toDPU3Topic",      /* 消息名称 */
						0X1C0E02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x01001C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"AIU2toDPU3Data",      /* 消息名称 */
						0X1C0E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x01001C,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"MFD1toDPU3Heart",      /* 消息名称 */
						0X240E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010024,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MFD1toDPU3Topic",      /* 消息名称 */
						0X240E02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010024,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MFD1toDPU3Data",      /* 消息名称 */
						0X240E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010024,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"MFD2toDPU3Heart",      /* 消息名称 */
						0X250E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010025,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MFD2toDPU3Topic",      /* 消息名称 */
						0X250E02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010025,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"MFD2toDPU3Data",      /* 消息名称 */
						0X250E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010025,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"UFCDtoDPU3Heart",      /* 消息名称 */
						0X260E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010026,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"UFCDtoDPU3Topic",      /* 消息名称 */
						0X260E02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010026,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"UFCDtoDPU3Data",      /* 消息名称 */
						0X260E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010026,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"DEUtoDPU3Heart",      /* 消息名称 */
						0X270E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010027,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DEUtoDPU3Topic",      /* 消息名称 */
						0X270E02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010027,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"DEUtoDPU3Data",      /* 消息名称 */
						0X270E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010027,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										{
						"VIUtoDPU3Heart",      /* 消息名称 */
						0X280E01,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010028,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"VIUtoDPU3Topic",      /* 消息名称 */
						0X280E02,        /* 消息ID */
						2048, 		  /* 65536 sws*/
						2,            /* 消息类型  事件 */
						0x010028,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
					{
						"VIUtoDPU3Data",      /* 消息名称 */
						0X280E00,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010028,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
										/*615A*/
					{
						"DPU3 image",      /* 消息名称 */
						0X010E78,        /* 消息ID */
						65536, 		  /* 65536 sws*/
						4,            /* 消息类型  事件 */
						0x010001,     /* 消息的源ASM端口port_id */
						0             /* 消息非刷新输出标志 */
					},
				},
			},
			

		}
	},

};

/*配置表*/
BLUE_PRINT gBluePrint2[1] =
{

      {
		/*第[0]个蓝图*/
		  {			
			/*节点1 ，第[3]个配置块*/
			{
				/*端口配置*/
				{
					"NC",               /* 本地端口的名称 */
					MB_NODE1_FCID,      /* 本地port_id */
					1,                  /* 端口类型 1-ASM */
					8,                  /* 信用 */
					ROLE_NC,     /* 端口的网络管理角色*/
					CLK_SYNC_ROLE_SERVER_MASTER,/* 时钟同步角色*/
					1500,               /* WDT定时周期*/
					500,                /* WDT加载周期*/
					100,                /* 时钟同步周期 */
					0,                  /* 非数据块发送消息的总数 */
					0,                  /* 非数据块接收消息的总数 */
					1,                  /* 数据块发送消息的总数 */
					1,                  /* 数据块接收消息的总数 */
				},
				{
					{
						"NC->NC",			/*消息名称*/
						NC_MSG_ID_TO_NC_1,	/*消息ID*/
						3,					/*消息类型*/
						0,					/*消息优先级*/
						MB_NODE1_FCID,		/*消息的源ASM端口port_id*/
						MB_NODE1_FCID,		/*消息的目的ASM端口port_id*/
					},
					{
						"NC->NC",			/*消息名称*/
						NC_MSG_ID_TO_NC_1,	/*消息ID*/
						3,					/*消息类型*/
						0,					/*消息优先级*/
						MB_NODE1_FCID,		/*消息的源ASM端口port_id*/
						MB_NODE1_FCID,		/*消息的目的ASM端口port_id*/
					},
        
				},
				{
					{
						"NC->NC",			/*消息名称*/
						NC_MSG_ID_TO_NC_1,  /*消息ID*/
						FC_AMS_IU_LENGTH_RECV,				/*消息长度*/
						3,					/*消息类型  事件*/
						MB_NODE1_FCID,		/*消息的源ASM端口port_id */
						0					/*消息非刷新输出标志*/
					},
					{
						"NC->NC",			/*消息名称*/
						MSG_ID_12,  /*消息ID*/
						FC_AMS_IU_LENGTH_RECV,				/*消息长度*/
						3,					/*消息类型  事件*/
						MB_NODE3_FCID,		/*消息的源ASM端口port_id */
						0				    /*消息非刷新输出标志*/
					},	
				},
			},	
			/*节点2 ，第[3]个配置块*/
			{
				/*端口配置*/
				{
					"NC",               /* 本地端口的名称 */
					MB_NODE2_FCID,      /* 本地port_id */
					1,                  /* 端口类型 1-ASM */
					8,                  /* 信用 */
					ROLE_NC,     /* 端口的网络管理角色*/
					CLK_SYNC_ROLE_SERVER_MASTER,/* 时钟同步角色*/
					1500,               /* WDT定时周期*/
					500,                /* WDT加载周期*/
					100,                /* 时钟同步周期 */
					0,                  /* 非数据块发送消息的总数 */
					0,                  /* 非数据块接收消息的总数 */
					1,                  /* 数据块发送消息的总数 */
					1,                  /* 数据块接收消息的总数 */
				},
				{
					{
						"NC->NC",			/*消息名称*/
				 		MSG_ID_12,	/*消息ID*/
						3,					/*消息类型*/
						0,					/*消息优先级*/
						MB_NODE2_FCID,		/*消息的源ASM端口port_id*/
						MB_NODE2_FCID,		/*消息的目的ASM端口port_id*/
					},
					{
						"NC->NC",			/*消息名称*/
						MSG_ID_13,	/*消息ID*/
						3,					/*消息类型*/
						0,					/*消息优先级*/
						MB_NODE2_FCID,		/*消息的源ASM端口port_id*/
						MB_NODE2_FCID,		/*消息的目的ASM端口port_id*/
					},
        
				},
				{
					{
						"NC->NC",			/*消息名称*/
						MSG_ID_12,  /*消息ID*/
						FC_AMS_IU_LENGTH_RECV,				/*消息长度*/
						3,					/*消息类型  事件*/
						MB_NODE2_FCID,		/*消息的源ASM端口port_id */
						0					/*消息非刷新输出标志*/
					},
					{
						"NC->NC",			/*消息名称*/
						MSG_ID_13,  /*消息ID*/
						FC_AMS_IU_LENGTH_RECV,				/*消息长度*/
						3,					/*消息类型  事件*/
						MB_NODE2_FCID,		/*消息的源ASM端口port_id */
						0				    /*消息非刷新输出标志*/
					},	
				},
			},	
			
			/*节点3 ，第[3]个配置块*/
			{
				/*端口配置*/
				{
					"NRT",               /* 本地端口的名称 */
					MB_NODE3_FCID,      /* 本地port_id */
					1,                  /* 端口类型 1-ASM */
					8,                  /* 信用 */
					ROLE_NRT,     /* 端口的网络管理角色*/
					CLK_SYNC_ROLE_CLIENT,/* 时钟同步角色*/

					1500,               /* WDT定时周期*/
					500,                /* WDT加载周期*/
					100,                /* 时钟同步周期 */
					0,                  /* 非数据块发送消息的总数 */
					0,                  /* 非数据块接收消息的总数 */
					1,                  /* 数据块发送消息的总数 */
					1,                  /* 数据块接收消息的总数 */
				},
				{
					{
						"NRT->NRT",			/*消息名称*/
				 		NC_MSG_ID_TO_NC_2,	/*消息ID*/
						3,					/*消息类型*/
						0,					/*消息优先级*/
						MB_NODE3_FCID,		/*消息的源ASM端口port_id*/
						MB_NODE1_FCID,		/*消息的目的ASM端口port_id*/
					},
					{
						"NRT->NRT",			/*消息名称*/
						NC_MSG_ID_TO_NC_2,	/*消息ID*/
						3,					/*消息类型*/
						0,					/*消息优先级*/
						MB_NODE3_FCID,		/*消息的源ASM端口port_id*/
						MB_NODE1_FCID,		/*消息的目的ASM端口port_id*/
					},
        
				},
				{
					{
						"NRT->NRT",			/*消息名称*/
						NC_MSG_ID_TO_NC_1,  /*消息ID*/
						FC_AMS_IU_LENGTH_RECV,				/*消息长度*/
						3,					/*消息类型  事件*/
						MB_NODE1_FCID,		/*消息的源ASM端口port_id */
						0					/*消息非刷新输出标志*/
					},
					{
						"NRT->NRT",			/*消息名称*/
						MSG_ID_11,  /*消息ID*/
						FC_AMS_IU_LENGTH_RECV,				/*消息长度*/
						3,					/*消息类型  事件*/
						MB_NODE1_FCID,		/*消息的源ASM端口port_id */
						0				    /*消息非刷新输出标志*/
					},	
				},
			},	


		 },

	},	
  

};
