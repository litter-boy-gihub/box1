/******************************************************************************
* FH130头文件定义                                                             *
* 版本                                                                        *
* 说明                                                                        *
******************************************************************************/

#ifndef __FH_130_API_H__
#define __FH_130_API_H__

#ifdef __cplusplus
extern "C" {
#endif  /*__cplusplus */

/*类型定义*/
typedef short                                    FCSHORT;
typedef int                                      FCINT;
typedef unsigned char                            FCUCHAR;
typedef unsigned short                           FCUSHORT;
typedef unsigned int                             FCUINT;
typedef unsigned long long                       FCUINT64;
typedef int                                     *FCHANDLE;

/*ASM接收回调定义*/
typedef FCUINT (* FC_ASM_RECVMSG_FUNCPTR)(FCHANDLE hDev, FCUINT msgID);

/*中断回调定义*/
typedef FCUINT (* FC_INT_FUNCPTR)(FCHANDLE hDev, FCUINT intCode, FCUINT param0);
typedef void (* FC_MEMCPY_FUNCPTR)(void *dest, void *src, unsigned int size);


/*自检返回值*/
#define BIT_SUCCESS                              0
#define BIT_FAILURE                              1


#define NC_OFF_NET                               0
#define NC_ON_NET                                1

#define NT_OFF_NET                               0
#define NT_ON_NET                                1

#define NT_OFF_LINE                              0
#define NT_ON_LINE                               1


#define CONFIG_MODE_PROJECT                      0
#define CONFIG_MODE_FLASH                        1

/*最大蓝图个数 */
#define FC_MAJOR_SCHEME_MAX                      128
/*每个蓝图的配置块个数 */
#define FC_MINOR_SCHEME_MAX                      12

/*角色*/
#define ROLE_NC                                  1
#define ROLE_BNC                                 2 
#define ROLE_NRT                                 3

/*网络时钟服务器，模块时钟客户端*/
#define CLK_SYNC_ROLE_SERVER_MASTER              1
/*网络时钟客户端*/
#define CLK_SYNC_ROLE_CLIENT                     2

/* RTC同步角色定义*/
/*1 服务器- 发送广播消息*/
#define SYNC_MODE_SERVER                         1
/*2 客户端- 接收广播消息*/
#define SYNC_MODE_CLIENT                         0

/*WDT周期禁止*/
#define WDT_LOADER_PERIOD_DISABLE                0
/*WDT周期使能*/
#define WDT_LOADER_PERIOD_ENAB                   1
/*没有收到ELS帧*/
#define RECV_ELS_INIT                            0
/*已经收到ELS帧*/
#define RECV_ELS_COMPLETE                        1

/* 网络角色定义 */
#define FC_NET_ROLE_NC                           1 /*网络控制器*/
#define FC_NET_ROLE_BNC                          2 /*备份网络控制器*/
#define FC_NET_ROLE_NRT                          3 /*网络远程终端*/

#define FC_NET_CLK_SYNC_ROLE_SERVER_MASTER       1 /*网络时钟服务器，模块时钟客户端*/
#define FC_NET_CLK_SYNC_ROLE_CLIENT              2 /*网络时钟客户端*/

#define FC_SYNC_MODE_SERVER                      1 /*发送RTC时钟*/
#define FC_SYNC_MODE_CLIENT                      2 /*接收RTC时钟*/

#define FC_WDT_LOADER_PERIOD_DISABLE             0 /*WDT加载周期禁止*/
#define FC_WDT_LOADER_PERIOD_ENAB                1 /*WDT加载周期使能*/

#define FC_TIME_WDT_LOADER_DISABLE               0/*时钟WDT加载周期禁止*/ 
#define FC_TIME_WDT_LOADER_ENAB                  1


#define FC_WDT_LOCAL_MODE                        1 /*本地模式*/
#define FC_WDT_NET_MODE                          2 /*网络模式*/

#define FC_NODE_STATE_OFF_LINE                   0 /*节点不在线*/
#define FC_NODE_STATE_ON_LINE                    1 /*节点在线*/

#define FC_PORT_STATUS_LINK_DOWN                 0 /* 端口链路DOWN */
#define FC_PORT_STATUS_LINK_UP                   1 /* 端口链路UP */

#define FC_CHANNEL_NUM_MAX                       256 /*发送和接收通道的个数，最大256*/

#define FC_SW_NUM_MAX                            2   /*交换机的数目*/
#define FC_SW_PORT_NUM_MAX                       48  /*36交换机的端口数目*/
#define SW_INFO_LENGH                            204 /*交换机信息长度*/

#define FC_CONFIG_MODE_PROJECT                   0   /*工程模式*/
#define FC_CONFIG_MODE_FLASH                     1   /*FLASH模式*/


#define FC_ASM_RUN_DISABLE                       0   /*禁止ASM发送接收*/
#define FC_ASM_RUN_ENAB                          1   /*允许ASM发送接收*/

#define FC_NC_BNC_MULTICAST_ADDR                 0x00FFFBFE  /*保留组播号，用于向NC和BNC节点发送组播消息*/

/* 发送/接收消息类型 */
#define MSG_TYPE_URGENCY_MSG				    1
#define MSG_TYPE_EVENT_MSG					    2
#define MSG_TYPE_ELS_MSG	                	3	/* 保留，不要使用 */
#define MSG_TYPE_STREAM_MSG					    4


/* 发送优先级 */
#define MAX_PRIORITY_LEVEL_NUM                  4
#define PRIORITY_LEVEL_0 	                    0 /* LOW , for ELS*/
#define PRIORITY_LEVEL_1  	                    1	
#define PRIORITY_LEVEL_2 	                    2
#define PRIORITY_LEVEL_3 	                    3 /* HIGH*/

/* 消息覆盖标记 */
#define MSG_NO_RECOVER		                    0	/* 当接收消息超过最大缓存数时，丢弃新来的消息 */
#define MSG_RECOVER			                    1	/* 当接收消息超过最大缓存数时，覆盖最老的一条消息，保留新来的消息 */
 

#define SW_FNU_1                                0  /* 交换机1 */
#define SW_FNU_2                                1  /* 交换机2 */


#define ENABLE_DOG_DAMK                         0
#define DISABLE_DOG_DAMK                        1


/******************************************************************************
* 中断子码定义                                                                *
******************************************************************************/

#define IC_VOLTAGE_SUCCESS                       0x100      /*电压从错误值变为正常值*/
#define IC_VOLTAGE_FAILED                        0x101      /*电压错误*/
#define IC_TEMP_SUCCESS                          0x102      /*温度从错误值变为正常值*/
#define IC_TEMP_FAILED                           0x103      /*温度错误*/
#define IC_LINK_STATUS_SUCCESS                   0x104      /*link状态从错误值变为正常值*/
#define IC_LINK_STATUS_FAILED                    0x105      /*link状态错误*/
#define IC_IO_BUF_WRITE_FAILED                   0x106      /*收发缓冲区写错误*/
#define IC_IO_BUF_READ_FAILED                    0x107      /*收发缓冲区读错误*/
#define IC_DISCARD_MSG           		         0x108      /*消息丢弃*/
#define IC_COVER_MSG             		         0x109      /*消息覆盖*/
#define IC_IU_TIMEOUT             		         0x10A      /*IU超时*/
#define IC_IU_SEQ_ERR             		         0x10B      /*IU超时*/
#define IC_IU_LENGTH_INVALID         	         0x10C      /*IU长度无效*/
#define IC_ELS_RECV_RECONSTRUCT                  0x200      /*重构*/
#define IC_ELS_RECV_NC_ON_NET                    0x201      /*NC在网*/
#define IC_ELS_RECV_NC_TIMEOUT                   0x202      /*WDT超时*/
#define IC_ELS_RECV_RECONSTRUCT_SUCCESS          0x203      /*重构完成*/
#define IC_ELS_RECV_RECONSTRUCT_FAIL             0x204      /*重构失败*/
#define IC_NC_CONFLICT                           0x208      /*NC冲突*/
#define IC_NC_CHANGED                            0x209      /*NC切换*/
#define TIME_SERVER_CHANGED                      0x210      /*时钟服务器冲突*/

/******************************************************************************
* 错误码
******************************************************************************/

/* 通用错误码 */
#define FC_SUCCESS                               0          /* 成功 */
#define FC_FAILURE                               0xE000FFFF /* 失败 */

#define FC_INVALID_HANDLE						 0xE0000001 /* 无效句柄 */
#define FC_PARAMS_INVALID   			         0xE0000002 /* 无效的参数  */
#define FC_INVALID_PORT_NUMBER					 0xE0000003 /* 无效的端口号*/	

/* 配置管理错误码 */
#define FC_DEVICE_ID_INVALID                     0xE1000001 /* 无效的设备ID */
#define FC_DEVICE_STATE_INVALID                  0xE1000002 /* 无效的设备状态 */
#define FC_DEVICE_NOT_FOUNT                      0xE1000003 /* 设备没有找到 */

/* 配置管理错误码 */
#define FC_GET_CONFIG_FAIL   			         0xE2000001 /* 获取配置数据失败 */
#define FC_INITIALISE_FAIL   			         0xE2000002 /* 初始化失败 */  
#define FC_SELF_CHECK_FAIL   			         0xE2000003 /* 自检失败 */
#define FC_HASH_CONFLICT                         0xE2000004 /* 哈希冲突 */
#define FC_DEV_GET_CONFIG_FAILED                 0xE2000005 /* 设备获取配置失败 */
#define FC_DEV_CB_INIT_FAILED                    0xE2000006 /* 设备上下文初始化失败 */
#define FC_DEV_START_FAILED                      0xE2000007 /* 设备启动失败 */
#define FC_INVALID_MAJOR_SCHEME					 0xE2000008 /* 无效的大版本号 */
#define FC_INVALID_MINOR_SCHEME					 0xE2000009 /* 无效的小版本号 */

#define FC_INVALID_FCID					 		 0xE2000011 /* FCID无效 */
#define FC_INVALID_NM_ROLE					 	 0xE2000012 /* 网络管理角色无效 */
#define FC_INVALID_TM_ROLE					 	 0xE2000013 /* 时钟同步角色无效 */
#define FC_INVALID_WDT_PERIOD				     0xE2000014 /* WDT定时器无效 */
#define FC_INVALID_CLK_SYNC_PERIOD				 0xE2000015 /* 时钟同步周期无效 */
#define FC_SEND_CHANNEL_BEYOND					 0xE2000016 /* 发送通道数超出范围 */	
#define FC_RECV_CHANNEL_BEYOND					 0xE2000017 /* 接收通道数超出范围 */
#define FC_TX_MSG_MAX_LEN_BEYOND				 0xE2000021 /* 无效的发送消息最大长度 */
#define FC_INVALID_TX_MSG_SRC_ID				 0xE2000022 /* 无效的发送消息源ID */
#define FC_INVALID_TX_MSG_DST_ID				 0xE2000023 /* 无效的发送消息目的ID */
#define FC_INVALID_TX_MSG_TYPE					 0xE2000024 /* 无效的发送消息类型 */
#define FC_INVALID_TX_MSG_PRIORITY               0xE2000025 /* 无效的发送消息优先级 */
#define FC_RX_MSG_MAX_LEN_BEYOND				 0xE2000031 /* 无效的接收消息最大长度 */
#define FC_INVALID_RX_MSG_TYPE					 0xE2000032 /* 无效的接收消息类型 */
#define FC_INVALID_RX_MSG_SRC_ID				 0xE2000033 /* 无效的接收消息源ID */
#define FC_INVALID_RX_MSG_RECOVER				 0xE2000034 /* 无效的接收消息覆盖标记 */

/* 数据通讯错误原因值 */
#define FC_MSG_ID_INVALID   			         0xE3000001 /* 无效的msgID  */
#define FC_ASM_RUN_INVALID                       0xE3000002 /* ASM运行控制是禁止 */
#define FC_DEV_CLOSED 					         0xE3000003 /* 设备已经关闭 */
#define FC_INVALID_RUN_CONTROL					 0xE3000004 /* 无效的运行控制值 */
#define FC_PORT_LINK_DOWN                        0xE3000005 /* 端口linkdown */

#define FC_SEND_INVALID_LENGTH                   0xE3000011 /* 无效的发送长度 */
#define FC_SEND_INVALID_BUFFER                   0xE3000012 /* 无效的发送缓存 */
#define FC_SEND_MSG_BUFFER_EMPTY 		         0xE3000013 /* 发送缓存用完 */
#define FC_SEND_MSG_NOT_ON_NET                   0xE3000014 /* 发送节点不在线 */

#define FC_RECV_INVALID_BUFFER                   0xE3000021 /* 无效的发送缓存 */
#define FC_RECV_LENGTH_IS_ZERO                   0xE3000022 /* 接收字节长度是零 */
#define FC_RECV_LENGTH_TOO_BIG                   0xE3000023 /* 接收字节长度超过ASM IU的最大字节长度 */
#define FC_RECV_LENGTH_TOO_SMALL                 0xE3000024 /* 接收字节长度超过ASM IU的最大字节长度 */
#define FC_RECV_NO_DATA             	         0xE3000025 /* 没有收到数据 */
#define FC_RECV_ASM_TIMEOUT						 0xE3000026 /* 接收超时 */
#define FC_RECV_MSG_NOT_ON_NET                   0xE3000027 /* 接收节点不在线 */

/* 时钟管理错误原因值 */
#define FC_TM_INVALID_YEAR_VALUE				 0xE4000001 /* 无效的年份值 */
#define FC_TM_INVALID_MONTH_VALUE				 0xE4000002 /* 无效的月份值 */
#define FC_TM_INVALID_DAY_VALUE					 0xE4000003 /* 无效的日期值 */
#define FC_TM_INVALID_HOUR_VALUE				 0xE4000004 /* 无效的时钟值 */
#define FC_TM_INVALID_MINUTE_VALUE				 0xE4000005 /* 无效的分钟值 */
#define FC_TM_INVALID_SECOND_VALUE				 0xE4000006 /* 无效的秒钟值 */
#define FC_TM_SET_DATE_FAIL					 	 0xE4000007 /* 设置日期错误 */
#define FC_TM_SET_RTC_FAIL					 	 0xE4000008 /* 设置RTC时间错误 */
#define FC_TM_ROLE_ILLE_OPERATION                0xE4000009 /* 时钟角色的非法操作 */
#define FC_TM_INVALID_CARRIER_RTC_SYN_MODE		 0xE400000A /* 无效的载板RTC同步模式 */	
#define FC_TM_INVALID_PLUSE_DUTY		 		 0xE400000B /* 无效的载板RTC同步PLUSE_DUTY */	

/* 网络管理错误原因值 */
#define FC_NM_ROLE_SAMED                         0xE5000001 /* 设置了相同的网络管理角色 */
#define FC_NM_ROLE_ILLE_OPERATION                0xE5000002 /* 网络管理角色的非法操作 */
#define FC_NM_NC_IS_OFF_LINE                     0xE5000003 /* NC节点没有在线 */
#define FC_NM_SYNC_IO_WAIT_FAILED                0xE5000004	/* 发送ELS帧以后等待接收ELS帧超时 */
#define FC_NM_FRAME_SEND_FAILED                  0xE5000005 /* 发送网络管理帧失败 */
#define FC_NM_INVALID_PORT_ID		 			 0xE5000006 /* 无效的port ID */
#define FC_NM_INVALID_PORT_COUNT				 0xE5000007 /* 无效的端口数目 */	
#define FC_NM_INVALID_NET_TBL_INDEX				 0xE5000008 /*无效的网表索引位*/
#define FC_TM_NM_ROLE_ILLE_OPERATION             0xE5000009 /*网络时钟管理角色非法*/

#define FC_SPI_OP_BUSY                           0xE3000034 /*SPI操作忙*/
#define FC_FLASH_READ_FAILED               	     0xE3000035 /*通过SPI读FLASH失败*/
#define FC_FLASH_WRITE_FAILED           		 0xE3000036 /*通过SPI写FLASH失败*/
#define FC_READ_FILE_ERROR                       0xE3000039 /*读文件失败*/
#define FC_HANDLE_GET_ERROR                      0xE3000040 /*获取设备的句柄未被赋值*/

/*手动自检结果*/
typedef struct
{
	FCUINT flash;        /*flash读写*/
	FCUINT temperature;  /*温度*/
	FCUINT voltage;      /*电压*/	
} FC_BIT_RESULT_MANUAL;

/*上电自检结果*/
typedef struct
{
	FCUINT memory;       /*内存测试*/
	FCUINT reg;          /*寄存器读写*/
	FCUINT temperature;  /*温度*/
	FCUINT voltage;      /*电压*/
	FCUINT flash;        /*flash读写*/
} FC_BIT_RESULT_POWER;

/*自检结果*/
typedef struct
{
	FCUINT memory;       /*内存测试*/
	FCUINT reg;          /*寄存器读写*/
	FCUINT temperature;  /*温度*/
	FCUINT voltage;      /*电压*/
	FCUINT flash;        /*flash读写*/
} FC_BIT_RESULT;

/*时间结构体*/
typedef struct
{
	FCUINT hour;         /*时*/
	FCUINT minute;       /*分*/
	FCUINT second;       /*秒*/
	FCUINT millisecond;  /*毫秒*/
	FCUINT microsecond;  /*微秒*/
	FCUINT nanosecond;   /*纳秒*/
} FCTime;

/*日期结构体*/
typedef struct
{
	FCUINT year;         /*年*/
	FCUINT month;        /*月*/
	FCUINT day;          /*日*/
} FCDate;

typedef struct
{
	FCUINT year;
	FCUINT month;
	FCUINT day;
	FCUINT spare;
}FC_SW_Date;

/*上网状态表*/
typedef struct
{
	FCUCHAR Status[FC_SW_PORT_NUM_MAX]; /*0: off net, 1: on net*/
} OnNetTbl;


/*上网状态表*/
typedef struct
{
	FCUCHAR AppSwInfoTable[SW_INFO_LENGH];
} AppSwInofWhole;


/*节点统计信息*/
typedef struct
{
    FCUINT RxMinFrameErrCnt[2]; /*Indicates the number of frames received that did not meet the minimum frame length requirement since this register was last read*/
    FCUINT RxMaxFrameErrCnt[2]; /*Indicates the number of frames received that exceeded the maximum frame length requirement since this register was last read*/
    FCUINT RxFrameCnt[2];       /*Indicates the number of frames received since this register was last read*/
    FCUINT RxWordCnt[2];        /*number of 32-bit data words received within FC frame(s) since this register was last read*/
    FCUINT RxPSMErrCnt[2];      /*Indicates the number of illegal PSM transactions that occurred since this register was last read*/
    FCUINT RxRRDYCnt[2];        /*Indicates the number of R_RDYs received since this register was last read*/
    FCUINT TxFrameCnt[2];       /*Indicates the number of frames transmitted since this register was last read*/
    FCUINT TxWordCnt[2];        /*Indicates the number of 32-bit data words transmitted within FC frame(s) since this register was last read*/
    FCUINT TxKErrCnt[2];        /*Indicates the number of 32-bit data words transmitted with an invalid K character since this register was last read*/
    FCUINT TxRRDYCnt[2];        /*Indicates the number of R_RDYs transmitted since this register was last read*/
} NetStatistics;

/*交换机link状态表*/
typedef struct
{
	FCUCHAR Status[FC_SW_PORT_NUM_MAX]; /*0: off line, 1: on line*/
} SwLinkStatusTbl_STR;


typedef struct 
{	
	/*软件版本号*/
	FCUSHORT VersionA;  /*A(0~9)(LSB=1)*/
	FCUSHORT VersionB;  /*B(00~99)(LSB=1)*/
	FCUSHORT VersionC;  /*C(00~25)软件技术状态标识(A~Z)*/

	/*软件版本生成日期*/
	FCUINT BuildYear; /*年(LSB=1)*/
	FCUINT BuildMonth;  /*月(LSB=1)*/
	FCUINT BuildDay;  /*日(LSB=1)*/

	FCUINT SoftwareCode;  /*软件编码*/
	/*软件版本生成时间*/
	FCUINT BuildHour;  /*时(LSB=1)*/
	FCUINT BuildMinute;  /*分(LSB=1)*/
	FCUINT BuildSecond;  /*秒(LSB=1)*/
	
}SoftWareVersion;



/*交换机加载配置*/
typedef struct
{
	FCUINT configMode;
	FCUINT configLength;
	FCUINT checksum;
} SwConfig;

/*交换机加载配置数据*/
typedef struct
{
	FCUINT endFlag;
	FCUINT length;
	FCUINT order;
	FCUCHAR configData[0];
} SwConfigData;

/*端口信息*/
typedef struct
{
	FCUCHAR port_name[16];      /*本地端口的名称*/
    FCUINT  port_id;            /*本地port_id*/
    FCUINT  port_type;          /*端口类型 1-ASM*/
    FCUINT  credit_num;         /*信用*/
    FCUINT  role;               /*端口的网络管理角色 1-网络管理器 2-备份网络管理器 3-网络终端*/
	FCUINT  clk_sync_role;      /*时钟同步角色 1-网络时钟服务器，模块时钟客户端 2-网络时钟客户端*/
    FCUINT  nc_wdt_period;      /*网络管理器的WDT定时周期*/
	FCUINT  wdt_loader_period;  /*WDT加载周期send els frame, 这个值必须比nc_wdt_period小 */
    FCUINT  clk_sync_period;    /*时钟同步周期 set to fpga*/
    FCUINT  nonblock_txmsg_num; /*非数据块发送消息的总数*/
    FCUINT  nonblock_rxmsg_num; /*非数据块接收消息的总数*/
    FCUINT  block_txmsg_num;    /*数据块发送消息的总数*/
    FCUINT  block_rxmsg_num;    /*数据块接收消息的总数*/
} FC_PORT_CONFIG;

/*发送通道*/
typedef struct 
{
	FCUCHAR msg_name[16];/*消息的名称*/
	FCUINT  msg_id;      /*发送消息ID*/
	FCUINT  msg_type;    /*消息类型 1-紧急 2-事件 3-保留(不要使用) 4-长消息*/
	FCUINT  msg_priority;/*消息优先级 0-3，0的优先级最低，3的优先级最高 */
	FCUINT  msg_src_id;  /*消息的源ASM端口port_id*/
	FCUINT  msg_dst_id;  /*消息的目的ASM端口port_id*/
} FC_MSG_SEND_ITEM;

/*接收通道*/
typedef struct
{
	FCUCHAR msg_name[16];/*消息的名称*/
	FCUINT  msg_id;      /*接收消息ID*/
	FCUINT  msg_max_len; /*最大消息长度*/
	FCUINT  msg_type;    /*消息类型 1-紧急 2-事件 3-保留(不要使用) 4-长消息*/
	FCUINT  msg_src_id;  /*消息的源ASM端口port_id*/
	FCUINT  msg_recover; /*消息覆盖控制标志0-不覆盖1-覆盖*/
} FC_MSG_RECV_ITEM;

/*配置块*/
typedef struct
{
    FC_PORT_CONFIG port_config;
	FC_MSG_SEND_ITEM send_item[FC_CHANNEL_NUM_MAX];
	FC_MSG_RECV_ITEM recv_item[FC_CHANNEL_NUM_MAX];
} CONFIG_BLOCK;

/*蓝图*/
typedef struct
{
    CONFIG_BLOCK config_block[FC_MINOR_SCHEME_MAX];
} BLUE_PRINT;


/*蓝图*/
typedef struct
{
	FCUINT minorScheme;
	FCUINT Length;
}Blue_node_num;




typedef struct
{
	FCUINT64 driver_sentBytes;
	FCUINT64 driver_recvBytes;
	FCUINT64 driver_sentIUs; 
	FCUINT64 driver_recvIUs;
	FCUINT64 driver_dicardIUs; 
	FCUINT64 driver_coverIUs; 
	FCUINT64 driver_exceptionIUs; 
	FCUINT64 driver_sentRawBuffers; 
	FCUINT64 driver_recvRawBuffers; 
	FCUINT64 driver_sendBuffers; 
	FCUINT64 driver_recvBuffers; 
	FCUINT64 driver_sendCmptBuffers; 
	
	FCUINT64 recv_Frame_Num;
	FCUINT64 recv_IU_Num;
	FCUINT64 recv_Error_IU_Num;
	FCUINT64 recv_Error_Frame_Num;
	FCUINT64 recv_Invalid_Frame_Num;
	FCUINT64 send_Frame_Num;
	FCUINT64 send_IU_Num;
}Device_Data_State;



#define FC_SW_PORT_MAX                           48                  /*交换机的端口数的最大值*/
#define FC_SW_FNU_UNIT_CONFIG_INFO_SOFTWARE_MAX  3                   /**/
#define FC_SW_OPTICAL_MODULE_MAX                 8                   /*广电收发器信息的最大值*/



/*FNU单元配置项版本*/
typedef struct {
    FCUINT  systemId ;      /*系统ID 任务系统=2*/
    FCUINT  subSystemID ;   /*分系统ID 惯性卫星导航分系统=0x01, 数据管理记录分系统=0x02, 武器悬挂物分系统=0x03, 综合处理分系统=0x04*/
    FCUINT  deviceId ;      /*综合图像处理单元=0x01, 综合射频运行平台=0x02, 核心处理计算平台=0x03, 独立软件产品OFP*/
    FCUINT  LRUID ;         /*根据具体设备编码 对于OFP软件产品此处填写0*/
    FCUINT  SoftWareID ;    /*软件ID 根据具体设备软件编码填写 对于OFP独立软件产品此处填写01*/
    

    FCUINT unitCode;         /*工作单元代码*/
    
    FCUINT versionA  ;    /*主板本号*/
    FCUINT versionB  ;    /*次板本号*/
    FCUINT versionC  ;    /*技术状态标识*/
    FCUINT BugFixNum ;    /*备用软件bug修订版本号*/

    FCUINT year     ;   /*版本生产 年*/
    FCUINT month    ;   /*月*/
    FCUINT day      ;   /*日*/

    FCUINT hour     ;   /*时*/
    FCUINT minute   ;   /*分*/
    FCUINT second   ;   /*秒*/   

} FC_SW_FNU_UNIT_CONFIG_SOFTWARE_INFO;

/*交换机上报 软件版本信息*/
typedef struct
{
	FC_SW_FNU_UNIT_CONFIG_SOFTWARE_INFO swSoftWareInfo[3]; 
} FC_SW_FNU_UNIT_CONFIG_SOFTWARE_INFO_STR;

typedef struct
{
	FCUSHORT direct;      /*表示光模块是发送还是接收 0-发送 1-接收*/
	FCUSHORT number;      /*光模块编号*/
	FCUSHORT temperature; /*光模块温度*/ 
	FCUSHORT voltage;     /*光模块实际电压，扩大1000倍*/
	FCUSHORT power[12];   /*每个光模块的12个无功率值*/
} FC_SW_OPTICAL_MODULE_INFO;

typedef struct
{
	FC_SW_OPTICAL_MODULE_INFO swOpticalModuleInfo[8];
} FC_SW_OPTICAL_MODULE_INFO_STR;

typedef struct
{
	FCUINT cpuTest;         /* cpuTest*/
	FCUINT ramTest;         /* ramTest*/
	FCUINT nvramTest;       /* nvramTest*/
	FCUINT fpgaTest;        /* fpgaTest*/
	FCUINT cpldTest;        /* cpldTest*/
	FCUINT port_01_12_Test; /* port_01_12_Test*/
	FCUINT port_13_24_Test; /* port_13_24_Test*/
	FCUINT port_25_36_Test; /* port_25_36_Test*/
	FCUINT port_37_48_Test; /* port_37_48_Test*/
	FCUINT port_01_48_Test; /* port_01_48_Test*/
	FCUINT invalidTest;     /* invalidTest*/
	FCUINT nmTest;          /* nmTest*/
}FC_SW_BIT_DETAIL_TABLE_STR;

/******************************************************************************
* 设备管理接口                                                                *
******************************************************************************/

/*打开FC节点机的N端口,为FC设备分配资源*/
FCUINT FC_NP_Open(FCHANDLE *hDev, FCUINT devId);

/*关闭FC节点机的N端口*/
FCUINT FC_NP_Close(FCHANDLE hDev);

/*获取FC节点机的N端口状态*/
FCUINT FC_NP_GetState(FCHANDLE hDev, FCUINT* fcState);

/*获取FC节点机的N端口版本号*/
FCUINT FC_NP_GetFpgaVersion(FCHANDLE hDev, SoftWareVersion* fpgaVerInfo);

/*软复位FC节点机的N端口*/
FCUINT FC_NP_SoftReset(FCHANDLE hDev);

/*启动FC节点机的N端口BIT操作*/
FCUINT FC_NP_IBit(FCHANDLE hDev, FC_BIT_RESULT_MANUAL *bitResult);

/*取得自检结果*/
FCUINT FC_NP_GetPuBitResult(FCHANDLE hDev, FC_BIT_RESULT_POWER *bitResult);

/*注册网络管理事件处理函数*/
FCUINT FC_NP_RegEvtHandler(FCHANDLE hDev, FC_INT_FUNCPTR func);

/*注销网络管理事件处理函数*/
FCUINT FC_NP_UnRegEvtHandler(FCHANDLE hDev);

/*配置下发后取得设置的portID*/
FCUINT FC_NP_GetLocalPortID(FCHANDLE hDev, FCUINT* LocalPortID);

/*获取光纤通道节点卡底层维护为DDS软件提供的识别码*/
FCUINT FC_NP_GetGUID(FCHANDLE hDev,FCUINT *pGUID);
/* 获取光纤设备的句柄 */
FCUINT FC_NP_GetDeviceHandle(FCHANDLE *hDev);


/******************************************************************************
* 通信管理接口                                                                *
******************************************************************************/

/*配置FC节点机的ASM通信模式*/
FCUINT FC_NP_ConfigMode(FCHANDLE hDev, FCUINT configMode);

/*加载FC节点机的ASM配置*/
FCUINT FC_NP_LoadConfig(FCHANDLE hDev, FCUINT majorScheme, FCUINT minorScheme);

/* 通过文件加载FC节点机的ASM配置 */
FCUINT FC_NP_LoadConfigFromFile(FCHANDLE hDev , FCUINT *pCfgTbl, FCUINT majorScheme, Blue_node_num *bluenode);

/*加载配置表版本配置表版本号上报*/
FCUINT FC_NP_LoadConfigSoftwareVersion(FCHANDLE hDev,SoftWareVersion *pLoadCfSoftwareVersion);

/*卸载FC节点机的ASM配置*/
FCUINT FC_NP_UnLoadConfig(FCHANDLE hDev);
    
/*对flash进行配置*/
FCUINT FC_NP_FlashConfig(FCHANDLE hDev);

/*发送指定的ASM消息*/
FCUINT FC_NP_SendMsg(FCHANDLE hDev, FCUINT msgID, FCUCHAR *pMsg, FCUINT nLen);

/*接收指定的ASM消息*/
FCUINT FC_NP_RecvMsg(FCHANDLE hDev, FCUINT msgID, FCUCHAR *pMsg, FCUINT *nLen);

/*注册ASM消息处理函数*/
FCUINT FC_NP_RegMsgHandler(FCHANDLE hDev, FC_ASM_RECVMSG_FUNCPTR func);

/*注销ASM消息处理函数*/
FCUINT FC_NP_UnRegMsgHandler(FCHANDLE hDev);

/*获取驱动工作状态是否正常*/
FCUINT FC_NP_GetWorkStatus(FCHANDLE hDev, FCUINT *status);

/*获取温度*/
FCUINT FC_NP_GetTemp(FCHANDLE hDev, FCINT *temperature);

/*获取端口的link状态*/
FCUINT FC_NP_GetPortLinkStatus(FCHANDLE hDev, FCUINT port, FCUINT *status);

/*控制ASM发送和接收
control 0-禁止ASM发送和接收
        1-允许ASM发送和接收
*/
FCUINT FC_NP_AsmIOControl(FCHANDLE hDev, FCUINT control);

/******************************************************************************
* 时统管理接口                                                                *
******************************************************************************/
/*获取FC节点机的时间*/
FCUINT FC_TM_GetRtc(FCHANDLE hDev, FCTime * pFCTime);

/*设置FC节点机的时间*/
FCUINT FC_TM_SetRtc(FCHANDLE hDev, FCTime * pFCTime);

/******************************************************************************
载板时钟同步配置
参数说明：
  hDev : 设备句柄
  carrier_rtc_enb : 0 禁止     1 - 允许
  mode : 0 - 客户端 1 - 服务器
  pluse_duty :
*******************************************************************************/
FCUINT FC_TM_SetCarrierRTCSyn(FCHANDLE hDev, FCUINT carrier_rtc_enb, FCUINT mode, FCUINT pluse_duty);

/*获取FC节点机的日期*/
FCUINT FC_TM_GetDateTime(FCHANDLE hDev, FCDate *pFCDate, FCTime *pFCTime);

/*设置FC节点机的日期*/
FCUINT FC_TM_SetDate(FCHANDLE hDev, FCDate * pFCDate);

/*设置节点的同步角色   FC_SYNC_MODE_SERVER   1  发送RTC时钟                  FC_SYNC_MODE_CLIENT    2    接收RTC时钟*/
FCUINT FC_TM_SyncModeSet(FCHANDLE hDev, FCUINT mode);

/*取得节点的同步角色*/
FCUINT FC_TM_SyncModeGet(FCHANDLE hDev, FCUINT *mode);

/*使能同步接收*/
FCUINT FC_TM_SyncEnable(FCHANDLE hDev);

/*禁止同步接收*/
FCUINT FC_TM_SyncDisable(FCHANDLE hDev);

/*获取网络广播时钟服务器端口号*/
FCUINT FC_TM_TimerServerPortID(FCHANDLE hDev,FCUINT *pPortID);

/******************************************************************************
* 网络管理接口                                                                *
******************************************************************************/

/*设置端口的网络管理角色*/
FCUINT FC_NM_RoleSet(FCHANDLE hDev,	FCUINT value);

/*取得端口的网络管理角色*/
FCUINT FC_NM_RoleGet(FCHANDLE hDev,	FCUINT *value);

/*取得上网状态表*/
FCUINT FC_NM_GetOnNetTable(FCHANDLE hDev, OnNetTbl *pOnNetTbl);

/*取得链路状态表*/
FCUINT FC_NM_GetLinkStateTable(FCHANDLE hDev,FCUINT port, SwLinkStatusTbl_STR *pLinkStateTbl, FCUINT swLocation);

/*取得本地上网状态*/
FCUINT FC_NM_GetOnNetState(FCHANDLE hDev, FCUINT *pState);

/*取得本地统计信息*/
FCUINT FC_NM_GetLocalStatistics(FCHANDLE hDev, NetStatistics * pNetStatistics);

/*取得交换机广播的应用信息*/
FCUINT FC_NM_GetSwInfo(FCHANDLE hDev, FCUINT port,AppSwInofWhole* pAppSwInofWhole );

/*上报软件版本*/
FCUINT FC_NM_GetFcSoftwareVersion(SoftWareVersion *pFcSoftwareVersion);

/*授权上网请求*/
FCUINT FC_NM_NC_GrantNrtOnNet(FCHANDLE hDev, FCUINT portId);

/*授权下网请求*/
FCUINT FC_NM_NC_GrantNrtOffNet(FCHANDLE hDev, FCUINT portId);

/*网络重构请求*/
FCUINT FC_NM_NC_NetRestruct(FCHANDLE hDev, FCUINT configNum);

/*判断网络节点是否在网*/
FCUINT FC_NM_NRT_NcIsOnNet(FCHANDLE hDev, FCUINT *pNcIsOnNet);

/*发起上网请求*/
FCUINT FC_NM_NRT_OnNetReq(FCHANDLE hDev);

/*发起下网请求*/
FCUINT FC_NM_NRT_OffNetReq(FCHANDLE hDev);

/*使能WDT*/
FCUINT FC_NM_WDTEnable(FCHANDLE hDev, FCUINT mode);

/*禁止WDT*/
FCUINT FC_NM_WDTDisable(FCHANDLE hDev, FCUINT mode);

/*使能时钟WDT*/
FCUINT FC_NM_DateWDTEnable(FCHANDLE hDev, FCUINT mode);

/*取得WDT信息*/
FCUINT FC_NM_GetWDTValue(FCHANDLE hDev, FCUINT *pWdt, FCUINT *pWdtLoader);

FCUINT FC_TM_BoardSyncRoleSet(FCHANDLE hDev, FCUINT role);

/*615调试函数 获取2个FC端口的link状态*/
FCUINT FC_DBG_GetAllPortStatus(FCHANDLE hDev, FCUINT *ports, FCUINT count);

/*注册内存拷贝函数*/
FCUINT FC_NP_RegMemCopyHandler(FC_MEMCPY_FUNCPTR func);

/*写FPGA映像文件*/
FCUINT FC_NM_NC_SendDatetoSw(FCHANDLE hDev,FC_SW_Date *pFCDate);

/*烧写本地FPGA映像文件*/
FCUINT FC_NP_BurnLocalFpgaImage(FCHANDLE hDev, FCUCHAR *filePath);

/*20210308*/

/*获取端口的链路状态变化的统计值*/
FCUINT FC_NP_GetPortLinkStatusChangeCount(FCHANDLE hDev, FCUINT *linkChangeCount, FCUINT port);

/*获取交换机的FPGA的温度*/
FCUINT FC_NM_GetSwFpgaTemperature(FCHANDLE hDev, FCUINT *fpgaTemperature,FCUINT swLocation);

/*获取交换机的光电收发器的信息*/
FCUINT FC_NM_GetSwOpticalModule(FCHANDLE hDev, FC_SW_OPTICAL_MODULE_INFO_STR *swOpticalModule,FCUINT swLocation);

/*获取交换机的FNU单元配置项的版本*/
FCUINT FC_NM_GetSwFnuUnitConfigInfo(FCHANDLE hDev, FC_SW_FNU_UNIT_CONFIG_SOFTWARE_INFO_STR *swFnuUnitConfigInfo,FCUINT swLocation);

/*获取交换机的BIT总结表*/
FCUINT FC_NM_GetSwBitResult(FCHANDLE hDev, FCUINT *bitResult,FCUINT swLocation);

/*获取交换机的BIT详细结果*/
FCUINT FC_NM_GetSwBitDetailTable(FCHANDLE hDev, FC_SW_BIT_DETAIL_TABLE_STR *swBitDetailTable,FCUINT swLocation);

/*获取交换机的设备编号*/
FCUINT FC_NM_GetSwDeviceNumberInfor(FCHANDLE hDev, FCUINT port ,FCUINT *pDevNumInfor);

/*获取交换机逻辑运行时间   */
FCUINT FC_NM_GetSwLogicRunTime(FCHANDLE hDev, FCUINT port ,FCUINT *pLogicRunTime);

/*获取子卡逻辑统计的错误值的总和*/
FCUINT FC_NM_GetDataStatisticsErrorCount(FCHANDLE hDev, Device_Data_State *pDevice_Data_State);

/*获取交换机逻辑运行时间*/
FCUINT FC_NM_GetSwMid(FCHANDLE hDev, FCUINT port ,FCUINT *pMid);


/*控制心跳收不到 是否响应中断*/
FCUINT FC_NP_ControlDogDank(FCHANDLE hDev, FCUINT dog_dank_state);



#ifdef __cplusplus
}
#endif  /*__cplusplus*/

#endif /*__FH_130_API_H__*/



