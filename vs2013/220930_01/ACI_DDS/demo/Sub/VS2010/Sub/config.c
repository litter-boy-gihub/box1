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

/*配置表*/
BLUE_PRINT gBluePrint[1] =
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
