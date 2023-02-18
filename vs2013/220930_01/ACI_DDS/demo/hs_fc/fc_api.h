/******************************************************************************
* 光纤子卡二次开发接口库头文件定义                                            *
******************************************************************************/
#ifndef __FC_API_H__
#define __FC_API_H__

#ifdef __cpluscplus
extern "C" {
#endif

#define FC_API_DLL  _declspec(dllexport)

/******************************************************************************
*类型定义                                                                     *
******************************************************************************/
typedef int                FCINT;
typedef unsigned int       FCUINT;
typedef unsigned long long FCUINT64;
typedef int               *FCHANDLE;
typedef unsigned char      FCUCHAR;

/*ASM接收回调定义*/
typedef FCUINT (*FC_ASM_RECVMSG_FUNCPTR)(FCHANDLE hDev, FCUINT msgID);

/*中断回调定义*/
typedef FCUINT(*FC_INT_FUNCPTR)(FCHANDLE hDev, FCUINT intCode, FCUINT param0);

/******************************************************************************
*常量定义                                                                     *
******************************************************************************/
#define FC_SUCCESS                      0x00000000 /*成功*/
#define FC_FAILURE                      0x00000001 /*失败*/
#define FC_INVALID_HANDLE               0x00000002 /*句柄无效*/
#define FC_INVALID_PARAMETER            0x00000003 /*参数无效*/
#define FC_INVALID_CARD_STATE           0x00000004 /*设备状态无效*/
#define FC_ENUM_DEVICE_FAILED           0x00000005 /*枚举设备失败*/
#define FC_OPEN_DEVICE_FAILED           0x00000006 /*打开设备失败*/
#define FC_DEVICE_IO_FAILED             0x00000007 /*IOCTRL失败*/
#define FC_READ_FILE_FAILED             0x00000008 /*读操作失败*/
#define FC_WRITE_FILE_FAILED            0x00000009 /*写操作失败*/
#define FC_WAIT_OBJECT_FAILED           0x0000000A /*等待信号量超时*/
#define FC_IOCTRL_RESP_NG               0x0000000B /*IOCTRL操作返回失败*/
#define FC_LINK_DOWN                    0x0000000C /*端口linkdown*/
#define FC_SYNC_IO_WAIT_FAILED          0x0000000D /*发送ELS帧以后等待接收ELS帧超时*/
#define FC_ROLE_ILLE_OPERATION          0x0000000E /*角色非法操作*/
#define FC_ELS_SEND_FAILED              0x0000000F /*发送ELS帧失败*/

#define SW_NUM                          2
#define SW_PORT_NUM                     48
#define NT_OFF_NET                      0
#define NT_ON_NET                       1
#define NT_OFF_LINE                     0
#define NT_ON_LINE                      1

#define PORT_LINK_DOWN                  0
#define PORT_LINK_UP                    1
#define RUN_STATUS_DISABLE              0
#define RUN_STATUS_ENAB                 1
#define CHANNEL_NUM_MAX                 256

#define CONFIG_MODE_PROJECT             0
#define CONFIG_MODE_FLASH               1

#define FC_SW_NUM_MAX                            2   /*交换机的数目*/
#define FC_SW_PORT_NUM_MAX                       48  /*36交换机的端口数目*/
#define SW_INFO_LENGH                            204 /*交换机信息长度*/


/*网络管理角色*/
#define ROLE_NC                         1            /*网络控制器*/
#define ROLE_BNC                        2            /*备份网络管理器*/
#define ROLE_NRT                        3            /*网络远程终端*/

#define CLK_SYNC_ROLE_SERVER_MASTER     1            /*网络时钟服务器，模块时钟客户端*/
#define CLK_SYNC_ROLE_CLIENT            2            /*网络时钟客户端*/

#define SYNC_MODE_SERVER                1            /*服务器-发送广播消息*/
#define SYNC_MODE_CLIENT                2            /*客户端-接收广播消息*/

#define FC_MAJOR_SCHEME_MAX             128          /*最大蓝图个数*/
#define FC_MINOR_SCHEME_MAX             4            /*每个蓝图的配置块个数*/

#define WDT_LOCAL_MODE                  1
#define WDT_NET_MODE                    2

/*手动自检结果*/
typedef struct
{
	FCUINT flash;        /*flash读写*/
	FCUINT buffer;		 /*收发缓冲区读写*/
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
	FCUINT els_loop;     /*ELS帧自环*/
	FCUINT asm_loop;     /*ASM自环*/
} FC_BIT_RESULT_POWER;

/*自检结果*/
typedef struct
{
	FCUINT memory;       /*内存测试*/
	FCUINT buffer;		 /*收发缓冲区读写*/
	FCUINT reg;          /*寄存器读写*/
	FCUINT temperature;  /*温度*/
	FCUINT voltage;      /*电压*/
	FCUINT flash;        /*flash读写*/
	FCUINT els_loop;     /*ELS帧自环*/
	FCUINT asm_loop;     /*ASM自环*/
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

/*上网状态表*/
typedef struct
{
	FCUCHAR Status[SW_PORT_NUM]; /*0: off net, 1: on net*/
} OnNetTbl;

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
	FCUCHAR Status[SW_NUM][SW_PORT_NUM]; /*0: off line, 1: on line*/
} SwLinkStatusTbl;

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


typedef struct
{
	FCUINT year;
	FCUINT month;
	FCUINT day;
	FCUINT spare;
}FC_SW_Date;

/*交换机link状态表*/
typedef struct
{
	FCUCHAR Status[FC_SW_PORT_NUM_MAX]; /*0: off line, 1: on line*/
} SwLinkStatusTbl_STR;

/******************************************************************************
*蓝图定义                                                                     *
******************************************************************************/
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
	FCUINT  wdt_loader_period;  /*WDT加载周期send els frame*/
	FCUINT  clk_sync_period;    /*时钟同步周期 set to fpga*/
	FCUINT  nonblock_txmsg_num; /*非数据块发送消息的总数*/
	FCUINT  nonblock_rxmsg_num; /*非数据块接收消息的总数*/
	FCUINT  block_txmsg_num;    /*数据块发送消息的总数*/
	FCUINT  block_rxmsg_num;    /*数据块接收消息的总数*/
} FC_PORT_CONFIG;

typedef struct
{
	FCUCHAR msg_name[16];/*消息的名称*/
	FCUINT  msg_id;      /*发送消息ID*/
	FCUINT  msg_type;    /*消息类型 1-紧急2-事件3-长消息*/
	FCUINT  msg_priority;/*消息优先级0-长消息1-ELS消息2-事件消息3-紧急消息*/
	FCUINT  msg_src_id;  /*消息的源ASM端口port_id*/
	FCUINT  msg_dst_id;  /*消息的目的ASM端口port_id*/
} FC_MSG_SEND_ITEM;

typedef struct
{
	FCUCHAR msg_name[16];      /*消息的名称*/
	FCUINT  msg_id;            /*接收消息ID*/
	UINT32  msg_length;        /*最大消息的字节数*/
	FCUINT  msg_type;          /*消息类型 1-紧急2-事件3-长消息*/
	FCUINT  msg_src_id;        /*消息的源ASM端口port_id*/
	FCUINT  msg_recover;       /*消息覆盖控制标志0-不覆盖1-覆盖*/
} FC_MSG_RECV_ITEM;

typedef struct
{
	FC_PORT_CONFIG port_config;
	FC_MSG_SEND_ITEM send_item[CHANNEL_NUM_MAX];
	FC_MSG_RECV_ITEM recv_item[CHANNEL_NUM_MAX];
} CONFIG_BLOCK;

typedef struct
{
	CONFIG_BLOCK config_block[FC_MINOR_SCHEME_MAX];
} BLUE_PRINT;

//extern BLUE_PRINT gBluePrint[];

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
	FCUINT direct;      /*表示光模块是发送还是接收 0-发送 1-接收*/
	FCUINT number;      /*光模块编号*/
	FCUINT temperature; /*光模块温度*/ 
	FCUINT voltage;     /*光模块实际电压，扩大1000倍*/
	FCUINT power[12];   /*每个光模块的12个无功率值*/
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
*设备管理接口                                                                 *
******************************************************************************/
/*查询设备*/
FC_API_DLL FCUINT FC_NP_QueryDev(FCUINT *devCount);

/*打开FC节点机的N端口,为FC设备分配资源*/
FC_API_DLL FCUINT FC_NP_Open(FCHANDLE *hDev, FCUINT devId);

/*关闭FC节点机的N端口*/
FC_API_DLL FCUINT FC_NP_Close(FCHANDLE hDev);

/*获取FC节点机的N端口状态*/
FC_API_DLL FCUINT FC_NP_GetState(FCHANDLE hDev, FCUINT* fcState);

/*获取FC节点机的N端口版本号*/
FC_API_DLL FCUINT FC_NP_GetVersion(FCHANDLE hDev, FCUINT* version);

/*软复位FC节点机的N端口*/
FC_API_DLL FCUINT FC_NP_SoftReset(FCHANDLE hDev);

/*启动FC节点机的N端口BIT操作*/
FC_API_DLL FCUINT FC_NP_IBit(FCHANDLE hDev, FC_BIT_RESULT_MANUAL *bitResult);

/*取得自检结果*/
FC_API_DLL FCUINT FC_NP_GetPuBitResult(FCHANDLE hDev, FC_BIT_RESULT_POWER *bitResult);

/*注册网络管理事件处理函数*/
FC_API_DLL FCUINT FC_NP_RegEvtHandler(FCHANDLE hDev, FC_INT_FUNCPTR func);

/*注销网络管理事件处理函数*/
FC_API_DLL FCUINT FC_NP_UnRegEvtHandler(FCHANDLE hDev);

/*配置下发后取得设置的portID*/
FCUINT FC_NP_GetLocalPortID(FCHANDLE hDev, FCUINT* LocalPortID);

/*获取光纤通道节点卡底层维护为DDS软件提供的识别码*/
FCUINT FC_NP_GetGUID(FCHANDLE hDev,FCUINT *pGUID);
/* 获取光纤设备的句柄 */
FCUINT FC_NP_GetDeviceHandle(FCHANDLE *hDev);
/******************************************************************************
*通信管理接口                                                                 *
******************************************************************************/
/*配置FC节点机的ASM通信模式*/
FC_API_DLL FCUINT FC_NP_ConfigMode(FCHANDLE hDev, FCUINT configMode);

/*加载FC节点机的ASM配置*/
FC_API_DLL FCUINT FC_NP_LoadConfig(FCHANDLE hDev,BLUE_PRINT *bluePrint, FCUINT majorScheme, FCUINT minorScheme);

/*卸载FC节点机的ASM配置*/
FC_API_DLL FCUINT FC_NP_UnLoadConfig(FCHANDLE hDev);

/*对flash进行配置*/
FC_API_DLL FCUINT FC_NP_FlashConfig(FCHANDLE hDev);

/*发送指定的ASM消息*/
FC_API_DLL FCUINT FC_NP_SendMsg(FCHANDLE hDev, FCUINT msgID, FCUCHAR *pMsg, FCUINT nLen);

/*接收指定的ASM消息*/
FC_API_DLL FCUINT FC_NP_RecvMsg(FCHANDLE hDev, FCUINT msgID, FCUCHAR *pMsg, FCUINT *nLen);

/*注册ASM消息处理函数*/
FC_API_DLL FCUINT FC_NP_RegMsgHandler(FCHANDLE hDev, FC_ASM_RECVMSG_FUNCPTR func);

/*注销ASM消息处理函数*/
FC_API_DLL FCUINT FC_NP_UnRegMsgHandler(FCHANDLE hDev);

/******************************************************************************
*时统管理接口                                                                 *
******************************************************************************/
/*获取驱动工作状态是否正常*/
FC_API_DLL FCUINT FC_NP_GetWorkStatus(FCHANDLE hDev, FCUINT *status);

/*获取温度*/
FC_API_DLL FCUINT FC_NP_GetTemp(FCHANDLE hDev, FCUINT *temperature);

/*获取端口的link状态*/
FC_API_DLL FCUINT FC_NP_GetPortLinkStatus(FCHANDLE hDev, int port, FCUINT *status);

/*设置消息发送控制*/
FC_API_DLL FCUINT FC_NP_Run(FCHANDLE hDev, FCUINT runStatus);

/*获取FC节点机的时间 highRTC的高10位low RTC的低32位*/
FC_API_DLL FCUINT FC_TM_GetRtc(FCHANDLE hDev, FCTime *time);

/*设置FC节点机的时间*/
FC_API_DLL FCUINT FC_TM_SetRtc(FCHANDLE hDev, FCTime time);

/*获取FC节点机的日期*/
FC_API_DLL FCUINT FC_TM_GetDateTime(FCHANDLE hDev, FCDate *date, FCTime *time);

/*设置FC节点机的日期*/
FC_API_DLL FCUINT FC_TM_SetDate(FCHANDLE hDev, FCDate date);

/*设置节点的同步角色*/
FC_API_DLL FCUINT FC_TM_SyncModeSet(FCHANDLE hDev, FCUINT mode);

/*取得节点的同步角色*/
FC_API_DLL FCUINT FC_TM_SyncModeGet(FCHANDLE hDev, FCUINT *mode);

/*使能同步接收*/
FC_API_DLL FCUINT FC_TM_SyncEnable(FCHANDLE hDev);

/*禁止同步接收*/
FC_API_DLL FCUINT FC_TM_SyncDisable(FCHANDLE hDev);

/*获取网络广播时钟服务器端口号*/
FCUINT FC_TM_TimerServerPortID(FCHANDLE hDev,FCUINT *pPortID);
/******************************************************************************
*网络管理接口                                                                 *
*******************************************************************************/
/*设置端口的网络管理角色*/
FC_API_DLL FCUINT FC_NM_RoleSet(FCHANDLE hDev, FCUINT value);

/*取得端口的网络管理角色*/
FC_API_DLL FCUINT FC_NM_RoleGet(FCHANDLE hDev, FCUINT *value);

/*取得上网状态表*/
FC_API_DLL FCUINT FC_NM_GetOnNetTable(FCHANDLE hDev, OnNetTbl *pOnNetTbl);

/*取得本地上网状态*/
FC_API_DLL FCUINT FC_NM_GetOnNetState(FCHANDLE hDev, FCUINT *pState);

/*取得本地统计信息*/
FC_API_DLL FCUINT FC_NM_GetLocalStatistics(FCHANDLE hDev, NetStatistics *pNetStatistics);

/*加载交换机配置*/
FC_API_DLL FCUINT FC_NM_NC_LoadSwConfig(FCHANDLE hDev, FCUINT portId, SwConfig *pSwConfig);

/*加载交换机配置数据*/
FC_API_DLL FCUINT FC_NM_NC_LoadSwConfigData(FCHANDLE hDev, FCUINT portId, SwConfigData *pSwConfigData);

/*取得交换机link状态*/
FC_API_DLL FCUINT FC_NM_GetSwLinkStatus(FCHANDLE hDev, SwLinkStatusTbl *pSwLinkStatusTbl);

/*取得交换机自检结果*/
FC_API_DLL FCUINT FC_NM_NC_GetSwBitResult(FCHANDLE hDev, FCINT port, FCUINT64 *pSwBitResult);

/*设置交换机方案号*/
FC_API_DLL FCUINT FC_NM_NC_SetMonitorScheme(FCHANDLE hDev, FCINT port, FCINT SchemeNo);

/*授权上网请求*/
FC_API_DLL FCUINT FC_NM_NC_GrantNrtOnNet(FCHANDLE hDev, FCUINT portId);

/*授权下网请求*/
FC_API_DLL FCUINT FC_NM_NC_GrantNrtOffNet(FCHANDLE hDev, FCUINT portId);

/*网络重构请求*/
FC_API_DLL FCUINT FC_NM_NC_NetRestruct(FCHANDLE hDev, FCUINT configNum);

/*取得网络节点自检结果*/
FC_API_DLL FCUINT FC_NM_NC_GetNrtBitResult(FCHANDLE hDev, FCUINT portId, FCUINT *pNrtBitResult);

/*取得网络节点统计信息*/
FC_API_DLL FCUINT FC_NM_NC_GetNetStatistics(FCHANDLE hDev, FCUINT portId, NetStatistics *pNetStatistics);

/*判断网络节点是否在网*/
FC_API_DLL FCUINT FC_NM_NRT_NcIsOnNet(FCHANDLE hDev, FCUINT *pNcIsOnNet);

/*发起上网请求*/
FC_API_DLL FCUINT FC_NM_NRT_OnNetReq(FCHANDLE hDev);

/*发起下网请求*/
FC_API_DLL FCUINT FC_NM_NRT_OffNetReq(FCHANDLE hDev);

/*使能WDT*/
FC_API_DLL FCUINT FC_NM_WDTEnable(FCHANDLE hDev, FCUINT mode);

/*禁止WDT*/
FC_API_DLL FCUINT FC_NM_WDTDisable(FCHANDLE hDev, FCUINT mode);

/*使能时钟WDT*/
FC_API_DLL FCUINT FC_NM_DateWDTEnable(FCHANDLE hDev, FCUINT mode);
/*取得WDT信息*/
FC_API_DLL FCUINT FC_NM_GetWDTValue(FCHANDLE hDev, FCUINT *pWdt, FCUINT *pWdtLoader);

/*写FPGA映像文件*/
FC_API_DLL FCUINT FC_NM_NC_SendDatetoSw(FCHANDLE hDev,FC_SW_Date *pFCDate);
FC_API_DLL FCUINT FC_SimElsSend(FCHANDLE hDev, FCUINT key);


#ifdef __cpluscplus
}
#endif

#endif /*__FC_API_H__*/
