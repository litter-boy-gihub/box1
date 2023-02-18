/******************************************************************************
* API接口定义                                                                 *
* 版本：V2.00                                                               *
******************************************************************************/

#ifndef __FC_FH180_API_H__
#define __FC_FH180_API_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* 类型定义                                                                     *
*******************************************************************************/
typedef char               FCCHAR;
typedef short              FCSHORT;
typedef int                FCINT;
typedef unsigned char      FCUCHAR;
typedef unsigned short     FCUSHORT;
typedef unsigned int       FCUINT;
typedef unsigned long long FCUINT64;
typedef int *              FCHANDLE;

/******************************************************************************
* 回调函数声明                                                                 *
*******************************************************************************/
/*ASM接收回调*/
typedef FCUINT (*FC_ASM_FUNCPTR)(FCHANDLE hDev, FCUINT msgId, FCUINT iuLength);

/*事件回调*/
typedef FCUINT (*FC_EVENT_FUNCPTR)(FCHANDLE hDev, FCUINT eventCode, FCUINT param1, FCUINT param2);

/*内存拷贝回调*/
typedef unsigned int (*FC_MEMCPY_FUNCPTR)(void *dest, void *src, unsigned int size);

/******************************************************************************
* 常量定义                                                                     *
*******************************************************************************/
#define FC_PORT_LINK_DOWN                  0          /*端口linkdown*/
#define FC_PORT_LINK_UP                    1          /*端口linkup*/
#define FC_WDT_PERIOD_ENAB                 1          /*WDT加载周期使能*/
#define FC_WDT_PERIOD_DISABLE              2          /*WDT加载周期禁止*/
#define FC_TM_SYNC_DISABLE                 0          /*时钟同步禁止*/
#define FC_TM_SYNC_ENAB                    1          /*时钟同步使能*/
#define FC_MAC_ON_LINE                     0          /*MAC不复位*/
#define FC_MAC_OFF_LINE                    1          /*MAC复位*/
#define FC_SWITCH_PORT_MAX                 48         /*交换机的端口数*/
#define FC_NET_NODE_MAX                    48         /*网络节点的最大个数*/
#define FC_CONFIG_BLOCK_MAX                4         /*最大配置块个数*/
#define FC_SCHEME_MAX                      4          /*最大方案数*/
#define FC_CHANNEL_NUM_MAX                 512        /*ASM通道的最大个数*/

/*角色*/
#define FC_NET_ROLE_NCT                    1          /*网络控制器节点*/
#define FC_NET_ROLE_BNCT                   2          /*网络备份控制器节点*/
#define FC_NET_ROLE_NRT                    3          /*网络远程节点*/
#define FC_TIME_ROLE_CCS                   1          /*时钟服务器*/
#define FC_TIME_ROLE_CCT                   2          /*时钟客户端*/

/*消息类型*/
#define FC_MSG_TYPE_RAW                    0          /*RAW消息*/
#define FC_MSG_TYPE_URGNEY                 1          /*紧急*/
#define FC_MSG_TYPE_EVENT                  2          /*事件*/
#define FC_MSG_TYPE_STREAM                 3          /*长消息*/

/*消息接收方式*/
#define FC_MSG_RECV_MODE_POLL              0          /*查询模式接收*/
#define FC_MSG_RECV_MODE_INT               1          /*中断模式接收*/

/******************************************************************************
* 中断子码                                                                     *
*******************************************************************************/
#define IC_PORT_LINK_UP                    0x00000100 /*link状态从错误变为正常.参数0:中断码;参数1,通道状态:(1,通道1在线;2,通道2在线;3,通道1和通道2都在线)*/
#define IC_PORT_LINK_DOWN                  0x00000101 /*link状态从正常变为错误.参数0:中断码;参数1,通道状态:(1,通道1在线;2,通道2在线;3,通道1和通道2都在线)*/
#define IC_VOLTAGE_GOOD                    0x00000102 /*电压从错误变为正常.参数0:中断码;参数1,当前电压;参数2,低压阈值;参数3,高压阈值*/
#define IC_VOLTAGE_FAILED                  0x00000103 /*电压从正常变为错误.参数0:中断码;参数1,当前电压;参数2,低压阈值;参数3,高压阈值*/
#define IC_TEMPERATURE_GOOD                0x00000104 /*温度从错误变为正常.参数0:中断码;参数1,当前温度;参数2,低温阈值;参数3,高温阈值*/
#define IC_TEMPERATURE_FAILED              0x00000105 /*温度从正常变为错误.参数0:中断码;参数1,当前温度;参数2,低温阈值;参数3,高温阈值*/
#define IC_RX_IU_DISCARD                   0x00000108 /*接收IU，FPGA丢弃.参数0:中断码;参数1,MsgID;参数2,丢弃码.*/
#define IC_RX_IU_TIMEOUT                   0x0000010A /*接收IU，超时.参数0:中断码.*/
#define IC_RX_IU_LENGTH_INVALID            0x0000010B /*接收IU的长度无效.参数0:中断码.*/
#define IC_WDT_INIT_TIMEOUT                0x0000010E /*WDT初始值等待超时 参数0：中断码 参数1：WDT初始值*/
#define IC_WDT_TIMEOUT                     0x0000010F /*WDT定时周期超时中断.参数0:中断码;参数1,狗叫标志(1：发生狗叫，0：未发生狗叫) */
#define IC_NCT_ON_NET                      0x00000110 /*NCT第一次上线*/
#define IC_NC_CHANGE                       0x00000112 /*NC发生切换.参数0:中断码;参数1,切换后的NC PortID*/
#define IC_NET_NODE_STATE_TABLE_CHANGED    0x00000114 /*网表发生变化.参数0:中断码;参数1,bit0-bit31(前32个设备);参数2,bit0-bit15（后16设备）*/
#define IC_ON_LINE_INTR                    0x00000115 /*本地上网.参数0:中断码.*/
#define IC_OFF_LINE_INTR                   0x00000116 /*本地下网.参数0:中断码.*/
#define IC_ON_LINE_REQ_INTR                0x00000117 /*(NC有效)节点机上线请求中断.参数0:中断码;参数1,请求上线节点端口ID.*/
#define IC_OFF_LINE_REQ_INTR               0x00000118 /*(NC有效)节点机下线请求中断.参数0:中断码;参数1,请求下线节点端口ID.*/
#define IC_NC_CONFLICT_INTR                0x00000119 /*NC冲突中断.参数0:中断码;参数1,原NC端口ID;冲突NC端口ID.*/
#define IC_RTC_DELTA_OVER_INTR             0x0000011A /*RTC超门限值中断.参数0:中断码.*/
#define IC_SW_LINK_CHANGE                  0x00000121 /*交换机由LINK发生变化，参数0:1-SW1 LINK 2-SW2 LINK 3-SW1/SW2 LINK 参数1:1-SW1 LINK DOWN 2-SW2 LINK DOWN 3-SW1/SW2 LINK DOWN*/
#define IC_NC_CONSTRUCT_REQ                0x00000122 /*NT节点收到NC发送的重构请求，参数0: 重构方案号 参数1：请求重构的NRT节点的FCID*/
#define IC_NT_CONSTRUCT_RESP               0x00000123 /*NC节点收到NT发送的重构请求的回复，参数0: 重构方案号 参数1：NRT节点的重构结果 0 成功 非0失败*/

/******************************************************************************
* 错误码                                                                       *
*******************************************************************************/
#define FC_SUCCESS                         0x00000000 /*成功*/
#define FC_FAILURE                         0xE3000000 /*失败*/
#define FC_PARAMETER_INVALID               0xE3000001 /*无效的参数*/
#define FC_GUID_INVALID                    0xE3000002 /*无效的GUID*/
#define FC_INSUFFICIENT_RESOURCE           0xE3000003 /*资源不足*/
#define FC_GET_INTERFACE_FAILED            0xE3000004 /*获取设备接口失败*/
#define FC_TCSCPY_FAILED                   0xE3000005 /*拷贝失败*/
#define FC_NOT_FOUND_DEVICE                0xE3000006 /*没有找到设备*/
#define FC_GET_READ_HANDLE_FAILED          0xE3000007 /*获取读句柄失败*/
#define FC_GET_WRITE_HANDLE_FAILED         0xE3000008 /*获取写句柄失败*/
#define FC_GET_IOCTRL_HANDLE_FAILED        0xE3000009 /*获取控制句柄失败*/
#define FC_OPEN_DEVICE_FAILED              0xE300000A /*打开设备失败*/
#define FC_DEVICE_STATE_INVALID            0xE300000B /*无效的设备状态*/
#define FC_DEVICE_ID_INVALID               0xE300000C /*无效的设备ID*/
#define FC_CONFIG_MODE_INVALID             0xE300000D /*无效的配置模式*/
#define FC_READ_FAILED                     0xE300000E /*读命令失败*/
#define FC_WRITE_FAILED                    0xE300000F /*写命令失败*/
#define FC_IOCTRL_FAILED                   0xE3000010 /*IOCTRL命令失败*/
#define FC_EVT_HANDLER_REGED               0xE3000011 /*事件中断处理函数已经注册*/
#define FC_ASM_HANDLER_REGED               0xE3000012 /*ASM消息接收中断处理函数已经注册*/
#define FC_UNREG_EVT_HANDLER_FAILED        0xE3000013 /*注销事件中断处理函数失败*/
#define FC_UNREG_ASM_HANDLER_FAILED        0xE3000014 /*注销ASM消息接收中断处理函数失败*/
#define FC_CREATE_TASK_FAILED              0xE3000015 /*创建任务失败*/
#define FC_CREATE_EVENT_FAILED             0xE3000016 /*创建信号量失败*/
#define FC_WAIT_EVENT_FAILED               0xE3000017 /*等待信号量失败*/
#define FC_MSG_ID_INVALID                  0xE3000018 /*无效的消息ID*/
#define FC_RECV_MODE_INVALID               0xE3000019 /*无效的接收模式*/
#define FC_SEND_LENGTH_INVALID             0xE300001A /*无效的发送长度*/
#define FC_SEND_BUF_FULL                   0xE300001B /*发送缓冲区满*/
#define FC_SEND_TIMEOUT                    0xE300001C /*发送超时*/
#define FC_RECV_LENGTH_INVALID             0xE300001D /*无效的接收长度*/
#define FC_RECV_NO_DATA                    0xE300001E /*接收没有数据*/
#define FC_RECV_TIMEOUT                    0xE300001F /*接收超时*/
#define FC_BUF_TOO_SMALL                   0xE3000020 /*缓冲区太小*/
#define FC_IOCTRL_RESP_NG                  0xE3000021 /*IOCTRL命令返回失败值*/
#define FC_HANDLE_INVALID                  0xE3000022 /*无效的句柄*/
#define FC_NM_ROLE_INVALID                 0xE3000023 /*无效的网络管理角色*/
#define FC_TM_ROLE_INVALID                 0xE3000024 /*无效的时钟管理角色*/
#define FC_IMAGE_FILE_OPEN_FAILED          0xE3000025 /*映像文件打开失败*/
#define FC_IMAGE_FILE_READ_FAILED          0xE3000026 /*映像文件读取失败*/
#define FC_READ_FILE_BUF_TOO_SMALL         0xE3000027 /*从映像文件读取内容的缓冲区太小*/
#define FC_START_TASK_FAILURE              0xE3000028 /*启动任务失败*/
#define FC_MSG_PRIORITY_INVALID            0xE3000029 /*无效的消息优先级*/
#define FC_BIT_FAILED                      0xE300002A /*自检失败*/
#define FC_GET_SEND_BUF_TIMEOUT            0xE300002B /*从发送链表取得缓冲区超时*/
#define FC_NM_NODE_OFFLINE                 0xE300002C /*节点没有在线*/
#define FC_NM_NC_NODE_OFFLINE              0xE300002D /*节点NC没有在线*/
#define FC_NM_LENGTH_INVALID               0xE300002E /*收到的网络管理帧的长度不正确*/
#define FC_NM_ROLE_ILLE_OPERATION          0xE300002F /*网络管理角色不正确*/
#define FC_NM_WAIT_RESP_TIMEOUT            0xE3000030 /*发送网络管理帧后等待回复超时*/
#define FC_NM_LINK_DOWN                    0xE3000031 /*发送网络管理帧时端口linkdown*/
#define FC_NM_SEND_FAILED                  0xE3000032 /*发送网络管理帧失败*/
#define FC_TM_ROLE_ILLE_OPERATION          0xE3000033 /*时钟服务角色不正确*/
#define FC_SPI_OP_BUSY                     0xE3000034 /*SPI操作忙*/
#define FC_FLASH_READ_FAILED               0xE3000035 /*通过SPI读FLASH失败*/
#define FC_FLASH_WRITE_FAILED              0xE3000036 /*通过SPI写FLASH失败*/
#define FC_FTP_XFER_ERROR                  0xE3000037 /*FTP连接失败*/
#define FC_FTP_READ_ERROR                  0xE3000038 /*FTP读文件失败*/

/******************************************************************************
 丢弃码                                                          *
*******************************************************************************/
#define FC_RX_IU_DISCARD_BUFOUT            0xE4000001 /* 应用接收缓冲满 */
#define FC_RX_IU_DISCARD_PRIORITY		   0xE4000002 /* 接收到的ASM帧的优先级与通道设置优先级不匹配导致丢包 */
#define FC_RX_IU_DISCARD_SEQ               0xE4000003 /* fc消息帧 seq 错误 */
#define FC_RX_IU_DISCARD_OXID              0xE4000004 /* fc消息帧 oxid 错误 */
#define FC_RX_IU_DISCARD_VALID             0xE4000005 /* fc消息帧 消息的源或者目的无效 */

/******************************************************************************
* 蓝图检查的错误码定义                                                          *
*******************************************************************************/
/*蓝图的错误码*/
#define FC_EB_HEADER_FILE_TYPE             0xEB000000 /*无效的蓝图头的文件类型*/
#define FC_EB_HEADER_LENGTH                0xEB000001 /*无效的蓝图头的长度*/
#define FC_EB_HEADER_VERSION               0xEB000002 /*无效的蓝图头的版本*/
#define FC_EB_HEADER_DATE                  0xEB000003 /*无效的蓝图头的日期*/
#define FC_EB_HEADER_CRC                   0xEB000004 /*无效的蓝图头的CRC*/
#define FC_EB_HEADER_LRM_SIG               0xEB000005 /*无效的蓝图头的LRM_SIG*/
#define FC_EB_HEADER_LRM_NUM               0xEB000006 /*无效的蓝图头的LRM_NUM*/
#define FC_EB_HEADER_LRM_ID                0xEB000007 /*无效的蓝图头的LRM_ID*/
#define FC_EB_HEADER_SOFT_VERSION          0xEB000008 /*无效的蓝图头的软件版本*/
#define FC_EB_CONFIG_BLOCK_NUM             0xEB000009 /*无效的配置块个数*/
#define FC_EB_CONFIG_BLOCK_NO              0xEB00000A /*无效的配置块索引*/
#define FC_EB_SCHEME_NUM                   0xEB00000B /*无效的方案个数*/

/*索引区的错误码*/
#define FC_EI_CONFIG_BLOCK_NUM             0xE1000000 /*无效的配置块个数*/
#define FC_EI_CONFIG_BLOCK_NO              0xE1000001 /*无效的配置块索引*/
#define FC_EI_CRC                          0xE1000002 /*无效的CRC*/
#define FC_EI_OFFSET                       0xE1000003 /*无效的偏移*/
#define FC_EI_LENGTH                       0xE1000004 /*无效的长度*/
#define FC_EI_RANGE                        0xE1000005 /*无效的范围*/
#define FC_EI_SCHEME_NUM                   0xE1000006 /*无效的方案个数*/
#define FC_EI_SCHEME_NO                    0xE1000007 /*无效的方案索引*/
#define FC_EI_SCHEME_CRC                   0xE1000008 /*无效的方案CRC*/
#define FC_EI_SCHEME_OFFSET                0xE1000009 /*无效的方案偏移*/
#define FC_EI_SCHEME_LENGTH                0xE100000A /*无效的方案长度*/
#define FC_EI_SCHEME_RANGE                 0xE100000B /*无效的方案范围*/

/*配置块的错误码*/
#define FC_EC_SCHEME_NO                    0xEC000000 /*无效的方案索引*/
#define FC_EC_PORT_ID                      0xEC000001 /*无效的端口ID*/
#define FC_EC_PORT_NAME                    0xEC000002 /*无效的端口名称*/
#define FC_EC_NM_ROLE                      0xEC000003 /*无效的网络管理角色*/
#define FC_EC_TM_ROLE                      0xEC000004 /*无效的时钟服务角色*/
#define FC_EC_WDT_INIT_VALUE               0xEC000005 /*无效的WDT初始值*/
#define FC_EC_WDT_PERIOD                   0xEC000006 /*无效的WDT定时周期值*/
#define FC_EC_WDT_LOADER_PERIOD            0xEC000007 /*无效的WDT加载周期值*/
#define FC_EC_CLK_SYNC_PERIOD              0xEC000008 /*无效的时钟同步周期*/
#define FC_EC_BLOCK_TXMSG_NUM              0xEC000009 /*无效的块模式发送消息数*/
#define FC_EC_BLOCK_RXMSG_NUM              0xEC00000A /*无效的块模式接收消息数*/
#define FC_EC_STREAM_TXMSG_NUM             0xEC00000B /*无效的流模式发送消息数*/
#define FC_EC_STREAM_RXMSG_NUM             0xEC00000C /*无效的流模式接收消息数*/
#define FC_EC_TXMSG_NUM                    0xEC00000D /*无效的发送消息总数*/
#define FC_EC_RXMSG_NUM                    0xEC00000E /*无效的接收消息总数*/
#define FC_EC_TX_MSG_ID                    0xEC00000F /*无效的发送消息ID*/
#define FC_EC_TX_MSG_NAME                  0xEC000010 /*无效的发送消息名称*/
#define FC_EC_TX_MSG_MAX_LEN               0xEC000011 /*无效的发送消息最大长度*/
#define FC_EC_TX_MSG_PRIORITY              0xEC000012 /*无效的发送消息优先级*/
#define FC_EC_TX_MSG_SRC_ID                0xEC000013 /*无效的发送消息源ID*/
#define FC_EC_TX_MSG_DST_ID                0xEC000014 /*无效的发送消息目的ID*/
#define FC_EC_TX_MSG_TYPE                  0xEC000015 /*无效的发送消息类型*/
#define FC_EC_TX_MSG_REFRESH               0xEC000016 /*无效的发送消息刷新标记*/
#define FC_EC_RX_MSG_ID                    0xEC000017 /*无效的接收消息ID*/
#define FC_EC_RX_MSG_NAME                  0xEC000018 /*无效的接收消息名称*/
#define FC_EC_RX_MSG_MAX_LEN               0xEC000019 /*无效的接收消息最大长度*/
#define FC_EC_RX_MSG_TYPE                  0xEC00001A /*无效的接收消息类型*/
#define FC_EC_RX_MSG_RECV_MODE             0xEC00001B /*无效的接收消息接收模式*/
#define FC_EC_RX_MSG_SRC_ID                0xEC00001C /*无效的接收消息源ID*/
#define FC_EC_RX_MSG_RECOVER               0xEC00001D /*无效的接收消息覆盖标记*/
/******************************************************************************
* 定义公共返回值                                                               *
*******************************************************************************/
#define FC_HANDLE_ERROR                    (0xE6310001)/*设备句柄错误*/
#define FC_IN_PARAM_ERROR                  (0xE6310002)/*输入参数错误：指针错误*/
#define FC_OUT_PARAM_ERROR                 (0xE6310003)/*输出参数错误：指针错误*/
#define FC_PORT_ID_ERROR                   (0xE6310004)/*设备PORT ID错误*/
#define FC_MSG_ID_ERROR                    (0xE6310005)/*设备MSGID错误*/
#define FC_MEM_MALLOC_ERROR                (0xE6310006)/*设备申请内存错误*/
#define FC_MEM_FREE_ERROR                  (0xE6310007)/*设备释放内存错误*/
#define FC_DEV_ID_ERROR                    (0xE6310008)/*设备ID错误*/
#define FC_DEV_STS_ERROR                   (0xE6310009)/*设备状态错误*/
#define FC_OPTION_FAILURE                  (0xE631000A)/*失败*/
/******************************************************************************
* 定义设备管理返回值                                                            *
*******************************************************************************/
#define FC_DEV_RESET_ERROR                 (0xE6310101)/*设备复位错误*/
#define FC_DEV_STS_SET_ERROR               (0xE6310102)/*设备状态设置错误*/
#define FC_DEV_OPEN_ERROR                  (0xE6310103)/*设备打开错误*/
#define FC_DEV_CLOSE_ERROR                 (0xE6310104)/*设备关闭错误*/
#define FC_DEV_ENABLE_ERROR                (0xE6310105)/*设备使能错误*/
#define FC_DEV_VER_REG_ERROR               (0xE6310106)/*设备版本注册错误*/
#define FC_DEV_PORT_REG_ERROR              (0xE6310107)/*设备端口注册错误*/
#define FC_DEV_MSG_TX_REG_ERROR            (0xE6310108)/*设备消息发送注册错误*/
#define FC_DEV_MSG_RX_REG_ERROR            (0xE6310109)/*设备消息接收注册错误*/
#define FC_DEV_PART_REG_ERROR              (0xE6310110)/*设备分区配置表注册错误*/
#define FC_DEV_RECFG_REG_ERROR             (0xE6310111)/*设备重构配置表注册错误*/
#define FC_DEV_VER_GET_ERROR               (0xE6310112)/*设备版本获取错误*/
#define FC_DEV_BUFF_CFG_ERROR              (0xE6310113)/*缓冲区配置错误*/
#define FC_DEV_REG_CALLBACK_ERROR          (0xE6310114)/*设备注册用户回调错误*/
#define FC_DEV_SET_INTRMASK_ERROR          (0xE6310115)/*设备中断掩码设置错误*/
#define FC_DEV_ENBLE_INTRMASK_ERROR        (0xE6310116)/*设备中断使能错误*/
#define FC_DEV_HEART_CHECK_ERROR           (0xE6310117)/*设备心跳检测错误*/
#define FC_DEV_CFG_LOAD_ERROR              (0xE6310118)/*设备配置加载错误*/
#define FC_DEV_VL_CFG_ERROR                (0xE6310119)/*设备虚链路配置错误*/
#define FC_DEV_FLASH_CHECK_ERROR           (0xE6310120)/*设备FLASH ID检测错误*/
#define FC_DEV_DISABLE_INTRMASK_ERROR      (0xE6310121)/*设备中断掩码清除错误*/
#define FC_DEV_CFGTBL_BIT_ERROR            (0xE6310122)/*设备配置表BIT错误*/
#define FC_DEV_RTC_BIT_ERROR               (0xE6310123)/*FC配置表BIT检测错误*/
#define FC_DEV_STATUS_BIT_ERROR            (0xE6310124)/*FC设备状态BIT检测错误*/
#define FC_DEV_FLASH_BIT_ERROR             (0xE6310125)/*FC设备FLASH BIT检测错误*/
#define FC_DEV_CPU_BIT_ERROR               (0xE6310126)/*CPU BIT检测错误*/
#define FC_DEV_RAM_BIT_ERROR               (0xE6310127)/*RAM BIT检测错误*/
#define FC_DEV_CLK_CFG_ERROR               (0xE6310128)/*时钟配置错误*/
#define FC_DEV_NTK_CFG_ERROR               (0xE6310129)/*网络配置错误*/
#define FC_DEV_TEMP_VALUE_ERROR            (0xE6310130)/*温度门限值错误*/
#define FC_DEV_TEMP_SET_ERROR              (0xE6310131)/*温度门限值设置错误*/
#define FC_DEV_TEMP_GET_ERROR              (0xE6310132)/*温度门限值获取错误*/
#define FC_DEV_CFG_FILE_VER_GET_ERROR      (0xE6310133)/*FC配置文件获取错误*/
#define FC_DEV_NVRAM_DATALEN_ERROR         (0xE6310134)/*FC设备NVRAM通信长度错误*/
#define FC_DEV_HOST_CALLBAK_REG_ERROR      (0xE6310135)/*FC设备主机接口配置用户回调注册失败*/
#define FC_DEV_HOST_CFG_ERROR              (0xE6310136)/*FC设备主机接口配置错误*/
#define FC_DEV_DMA_ALLOC_ERROR             (0xE6310137)/*FC设备通信缓冲区申请错误*/
#define FC_DEV_MSG_QUEUE_CFG_ERROR         (0xE6310138)/*MSGID队列配置失败*/
#define FC_DEV_NODE_NO_ERROR               (0xE6310139)/*节点索引错误*/
#define FC_DEV_SCHEME_NO_ERROR             (0xE6310140)/*方案索引错误*/
#define FC_DEV_VOLTAGE_SET_ERROR           (0xE6310141)/*电压门限值设置错误*/
#define FC_DEV_VOLTAGE_GET_ERROR           (0xE6310142)/*电压门限值获取错误*/
#define FC_DEV_VOLTAGE_ERROR               (0xE6310143)/*当前电压错误*/
#define FC_DEV_TEMP_ERROR                  (0xE6310144)/*当前温度错误*/
/******************************************************************************
* 定义通信管理返回值                                                            *
*******************************************************************************/
#define FC_CMM_RUN_ERROR                   (0xE6310200)/*FC通信使能错误*/
#define FC_CMM_STOP_ERROR                  (0xE6310201)/*FC通信禁止错误*/
#define FC_CMM_MSGID_ERROR                 (0xE6310202)/*FC通信MSGID错误*/
#define FC_CMM_CFG_MSG_ERROR               (0xE6310203)/*FC通信获取配置的流消息信息错误*/
#define FC_CMM_BUFF_MSG_ERROR              (0xE6310204)/*FC通信获取缓冲区单元获取错误*/
#define FC_CMM_BUFF_ADDR_ERROR             (0xE6310205)/*FC通信获取缓冲区地址错误*/
#define FC_CMM_MSGLEN_ERROR                (0xE6310206)/*FC通信消息长度错误*/
#define FC_CMM_ADDR_ERROR                  (0xE6310207)/*FC通信消息地址错误*/
#define FC_CMM_PART_ORG_ERROR              (0xE6310208)   
#define FC_CMM_MIH_ORG_ERROR               (0xE6310209)   
#define FC_CMM_ASM_ORG_ERROR               (0xE6310210)/*FC ASM消息组织错误*/
#define FC_CMM_ASM_SEND_ERROR              (0xE6310211)/*FC ASM消息发送错误*/
#define FC_CMM_PART_PARSE_ERROR            (0xE6310212)   
#define FC_CMM_MIH_PARSE_ERROR             (0xE6310213)   
#define FC_CMM_ASM_PARSE_ERROR             (0xE6310214)/*FC ASM消息解析错误*/
#define FC_CMM_ASM_RECV_ERROR              (0xE6310215)/*FC ASM消息接收错误*/
#define FC_CMM_ASM_RECV_CPT_ERROR          (0xE6310216)/*FC ASM消息接收完成设置错误*/
#define FC_CMM_ASM_ADDR_REG_ERROR          (0xE6310217)/*FC通信缓冲区接收注册错误*/
#define FC_CMM_SEND_STATE_ERROR            (0xE6310218)/*FC流消息发送状态错误*/ 
#define FC_CMM_LOCAL_STATE_ERROR           (0xE6310219) 
#define FC_CMM_DEST_STATE_ERROR            (0xE6310220)
/******************************************************************************
* 定义公通信管理返回值                                                          *
*******************************************************************************/
#define FC_ELS_HANDLE_ERROR                (0xE6310300)/*ELS句柄错误*/
#define FC_ELS_IN_PARAM_ERROR              (0xE6310301)/*输入参数错误*/
#define FC_ELS_OUT_PARAM_ERROR             (0xE6310302)/*输出参数错误*/
#define FC_ELS_OPT_ERROR                   (0xE6310303)/*设置错误*/
#define FC_ELS_MALLOC_ERROR                (0xE6310304)/*内存申请错误*/
#define FC_ELS_SPAWN_TASK_ERROR            (0xE6310305)/*任务启动错误*/
#define FC_ELS_FUNCTION_ID_ERROR           (0xE6310306)/*功能ID错误*/
#define FC_ELS_DATA_LEN_ERROR              (0xE6310307)/*数据长度错误*/
#define FC_ELS_MSG_LEN_ERROR               (0xE6310308)/*消息长度错误*/
#define FC_ELS_ADDR_ERROR                  (0xE6310309)/*数据地址错误*/
/******************************************************************************
* 定义网络管理返回值                                                            *
*******************************************************************************/
#define FC_NTK_MSG_LEN_ERROR               (0xE6310400)/*消息长度错误*/
#define FC_NTK_PORT_ID_ERROR               (0xE6310401)/*端口ID错误*/ 
#define FC_NTK_ROLE_ERROR                  (0xE6310402)/*网络角色错误*/
#define FC_NTK_WDT_MODE_ERROR              (0xE6310403)/*WDT模式错误*/
#define FC_NTK_CHANNEL_GET_ERROR           (0xE6310404)/*通道获取错误*/
#define FC_NTK_SW_MONI_CTRL_GET_ERROR      (0xE6310405) 
#define FC_NTK_SW_MONI_STATE_GET_ERROR     (0xE6310406)
#define FC_NTK_SW_BIT_GET_ERROR            (0xE6310407)/*交换机BIT获取错误*/
#define FC_NTK_SW_VER_GET_ERROR            (0xE6310408)/*交换机版本获取错误*/
#define FC_NTK_SW_RESTRUCT_ERROR           (0xE6310409)/*交换机重构错误*/
#define FC_NTK_SW_STATISINFO_GET_ERROR     (0xE6310410)
#define FC_NTK_SW_IBIT_GET_ERR0R           (0xE6310411)
#define FC_NTK_SW_PBIT_GET_ERR0R           (0xE6310412)
#define FC_NTK_SW_CBIT_GET_ERR0R           (0xE6310413)
#define FC_NTK_IBIT_GET_ERR0R              (0xE6310414)
#define FC_NTK_PBIT_GET_ERR0R              (0xE6310415)
#define FC_NTK_CBIT_GET_ERR0R              (0xE6310415)
#define FC_NTK_SW_LINK_STATE_GET_ERR0R     (0xE6310417)
#define FC_NTK_WDT_SET_ERR0R               (0xE6310418)
#define FC_NTK_ONLINE_REQ_ERROR            (0xE6310419)
#define FC_NTK_FORCE_OFFLINE_ERROR         (0xE6310420)
/******************************************************************************
* 定义时钟管理返回值                                                            *
*******************************************************************************/
#define FC_CLK_ROLE_ERROR                  (0xE6310500)/*时钟角色错误*/

/******************************************************************************
* 配置块定义                                                                   *
*******************************************************************************/
/* 蓝图头信息*/
typedef struct
{
	FCUINT  file_type;       /*蓝图类型*/
	FCINT   length;          /*所有方案有效数据的字节长度，不包括蓝图头信息*/
	FCUCHAR version[16];     /*版本信息*/
	FCUINT  date;            /*蓝图日期*/
	FCUINT  crc;             /*所有方案有效数据的字节和，不包括蓝图头信息*/
	FCUINT  lrm_sig;         /*同类型LRM模块设备标识(端口地址 PORT_ID) 保留*/
	FCSHORT lrm_num;         /*同类型LRM模块设备总个数(最大4个) 保留*/
	FCSHORT lrm_id;          /*本配置在同类型LRM模块设备中的编号 保留*/
	FCUCHAR soft_version[24];/*软件配置工具的版本 保留*/
} FC_BLUE_PRINT_HEADER;

/*端口信息*/
typedef struct
{
	FCUINT  port_id;          /*端口FCID*/
	FCUCHAR port_name[16];    /*端口名称*/
	FCUINT  nm_role;          /*网络管理角色*/
	FCUINT  tm_role;          /*时钟同步角色*/
	FCUINT  wdt_init_value;   /*网络管理器启动初始值 单位毫秒*/
	FCUINT  wdt_period;       /*WDT狗叫值 单位毫秒*/
	FCUINT  wdt_loader_period;/*网络管理器WDT定时周期 单位毫秒*/
	FCUINT  clk_sync_period;  /*时钟同步周期 单位毫秒 一般设置为50ms*/
	FCUINT  block_txmsg_num;  /*数据块发送消息总数   0～512*/
	FCUINT  block_rxmsg_num;  /*数据块接收消息总数   0～512*/
	FCUINT  stream_txmsg_num; /*流数据块发送消息总数 0～512*/
	FCUINT  stream_rxmsg_num; /*流数据块接收消息总数 0～512*/
} FC_PORT_CONFIG;

/*发送块*/
typedef struct
{
	FCUINT  msg_id;           /*消息ID*/
	FCUCHAR msg_name[16];     /*消息名称*/
	FCUINT  msg_max_len;      /*发送的最大字节数*/
	FCUINT  msg_src_id;       /*消息的源FCID*/
	FCUINT  msg_dst_id;       /*消息的目的FCID*/
	FCUINT  msg_type;         /*消息类型 1-紧急 2-事件 3-长消息*/
} FC_MSG_SEND_ITEM;

/*接收块*/
typedef struct
{
	FCUINT  msg_id;           /*消息ID*/
	FCUCHAR msg_name[16];     /*消息名称*/
	FCUINT  msg_max_len;      /*接收的最大字节数*/
	FCUINT  msg_type;         /*消息类型 1-紧急 2-事件 3-长消息*/
	FCUINT  msg_recv_mode;    /*消息接收方式 1-查询 2-中断*/
	FCUINT  msg_src_id;       /*消息的源FCID*/
	FCUINT  msg_recover;      /*消息覆盖标志 0-不覆盖 1-覆盖*/
} FC_MSG_RECV_ITEM;

/*方案*/
typedef struct
{
	FCUINT scheme_no;                              /*方案索引号 从0开始*/
	FC_PORT_CONFIG port_config;                    /*端口信息*/
	FC_MSG_SEND_ITEM send_item[FC_CHANNEL_NUM_MAX];/*发送通道*/
	FC_MSG_RECV_ITEM recv_item[FC_CHANNEL_NUM_MAX];/*接收通道*/
} FC_SCHEME;

/*节点配置块*/
typedef struct
{
	FCUINT    config_block_no;      /*配置块索引号 从0开始*/
	FCUINT    scheme_num;           /*方案个数*/
	FC_SCHEME scheme[FC_SCHEME_MAX];/*方案*/
} FC_CONFIG_BLOCK;

/*蓝图*/
typedef struct
{
	FC_BLUE_PRINT_HEADER blue_print_header;                /*蓝图头信息*/
	FCUINT               config_block_num;                 /*节点个数*/
	FC_CONFIG_BLOCK      config_block[FC_CONFIG_BLOCK_MAX];/*节点配置*/
} FC_BLUE_PRINT;

/*节点上电自检*/
typedef struct
{
	FCUCHAR temperature;/*温度的状态 0-正常 1-错误*/
	FCUCHAR voltage;    /*电压的状态 0-正常 1-错误*/
	FCUCHAR flash;      /*flash读写的状态 0-正常 1-错误*/
	FCUCHAR reg;        /*寄存器读写的状态 0-正常 1-错误*/ 
	FCUCHAR els_loop;   /*ELS帧自环的状态 0-正常 1-错误*/
	FCUCHAR asm_loop;   /*ASM自环的状态 0-正常 1-错误*/
	FCUCHAR reserve[2]; /*对齐8字节*/
} FC_PBIT_RESULT;

/*节点周期自检*/
typedef struct
{
	FCUCHAR temperature;/*温度的状态 0-正常 1-错误*/
	FCUCHAR voltage;    /*电压的状态 0-正常 1-错误*/
	FCUCHAR reserve[6]; /*对齐8字节*/
} FC_CBIT_RESULT;

/*节点维护自检*/
typedef struct
{
	FCUCHAR temperature;/*温度的状态 0-正常 1-错误*/
	FCUCHAR voltage;    /*电压的状态 0-正常 1-错误*/
	FCUCHAR flash;      /*flash读写的状态 0-正常 1-错误*/
	FCUCHAR reg;        /*寄存器读写的状态 0-正常 1-错误*/ 
	FCUCHAR reserve[4]; /*对齐8字节*/
} FC_MBIT_RESULT;

/*日期*/
typedef struct
{
	FCUINT year; /*年*/
	FCUINT month;/*月*/
	FCUINT day;  /*日*/
} FC_DATE;

/*时间*/
typedef struct
{
	FCUINT hour;       /*时*/
	FCUINT minute;     /*分*/
	FCUINT second;     /*秒*/
	FCUINT millisecond;/*毫秒*/
	FCUINT microsecond;/*微秒*/
	FCUINT nanosecond; /*纳秒*/
} FC_TIME;

/*FTP参数*/
typedef struct
{
	FCUCHAR hostIp[16];/*主机的IP地址，最大15个字符以\0结尾*/
	FCUCHAR user[16];  /*用户名，最大15个字符以\0结尾*/
	FCUCHAR pwd[16];   /*密码，最大15个字符以\0结尾*/
} FC_FTP_PARAM;

/*版本*/
typedef struct
{
	FCUCHAR fpga[16];           /*FPGA版本号，最大15个字符以\0结尾*/
	FCUCHAR fpga_date[20];		/*逻辑版本生成的日期，最大18个字符以\0结尾*/
	FCUCHAR api[16];            /*API版本号，最大15个字符以\0结尾*/
	FCUCHAR api_date[20];		/*驱动版本日期，最大15个字符以\0结尾*/
	FCUCHAR blue_print_ver[16]; /*蓝图的版本号，最大15个字符以\0结尾*/
	FCUCHAR blue_print_date[20];/*蓝图版本生成日期，最大18个字符以\0结尾*/
} FC_VERSION;

/*端口统计信息*/
typedef struct
{
	FCUINT64 RxUndersizeFrameCnt;
	FCUINT64 RxOversizeFrameErrCnt;
	FCUINT64 RxAllFrameCnt;
	FCUINT64 RxValidFrameCnt;
	FCUINT64 RxWordCnt;
	FCUINT64 RxRRDYCnt;
	FCUINT64 RxKErrCnt;
	FCUINT64 LinkChgUp2DownCnt;
	FCUINT64 TxAllFrameCnt;
	FCUINT64 TxValidFrameCnt;
	FCUINT64 TxWordCnt;
	FCUINT64 TxRRDYCnt;
	FCUINT64 TxKErrCnt;
} FC_PORT_STATISTICS_INFO;

/*网络节点的统计信息*/
typedef struct
{
	FC_PORT_STATISTICS_INFO port_statistics_info[2];
} FC_NET_STATISTICS_INFO;

/*网络状态表*/
typedef struct
{
	struct
	{
		FCUINT fcid;  /*FCID*/
		FCUINT state; /*端口的状态 0-下网 1-上网*/
	} netNodeState[FC_NET_NODE_MAX];
} FC_NET_NODE_STATE_TABLE;

/*交换机版本信息*/
typedef struct
{
	FCUINT manufactureInfo;/*设备厂商信息*/
	FCUINT fpgaVersion;    /*FPGA逻辑版本*/
	FCUINT softVersion;    /*软件版本*/
	FCUINT firmwareVersion;/*固件版本*/
} FC_SW_VERSION_INFO;

/*交换机端口链路状态表*/
typedef struct
{
	struct
	{
		FCUINT fcid;  /*FCID*/
		FCUINT state; /*端口的状态 0-链路down 1-链路up*/
	} swPortLink[FC_SWITCH_PORT_MAX];/*只取前46个节点*/
} FC_SW_PORT_LINK_TABLE;

/*交换机网络数据监视控制*/
typedef struct
{
	FCUINT mport;    /*监控端口编号 0-1*/
	FCUINT monitorID;/*监控方案索引 0-127*/
	FCUINT command;  /*监控控制指令 0-停止监控 1-开始监控*/
} FC_SW_MONITOR_CTRL_REQ;

typedef struct
{
	FCUINT state;    /*当前的监控状态 0-未监控 1-监控中*/
	FCUINT monitorID;/*当前监控方案索引 0-127*/
	FCUINT domainID; /*指定交换机域ID 1-2*/
} FC_SW_MONITOR_CTRL_RESP;

/*获取交换机监控状态*/
typedef struct
{
	FCUINT mport; /*监控端口编号 0-1*/
} FC_SW_MONITOR_STATE_REQ;

typedef struct
{
	FCUINT state;    /*当前的监控状态 0-未监控 1-监控中*/
	FCUINT monitorID;/*当前监控方案索引 0-127*/
	FCUINT domainID; /*指定交换机域ID 1-2*/
} FC_SW_MONITOR_STATE_RESP;

/*交换机上电自检*/
typedef struct
{
	FCUINT reserve;
} FC_SW_GET_PBIT_REQ;

typedef struct
{
	FCUCHAR cpuBit;    /*CPU自检结果 0-正常 1-错误*/
	FCUCHAR flashBit;  /*FLASH自检结果 0-正常 1-错误*/
	FCUCHAR ramBit;    /*RAM自检结果 0-正常 1-错误*/	
	FCUCHAR reserve[5];/*对齐8字节*/
} FC_SW_GET_PBIT_RESP;

/*交换机周期自检*/
typedef struct
{
	FCUINT reserve;
} FC_SW_GET_CBIT_REQ;

typedef struct
{
	FCUCHAR temperatureBit;/*温度 0-正常 1-错误*/
	FCUCHAR voltageBit;    /*电压自检结果 0-正常 1-错误*/	
	FCUCHAR reserve[6];    /*对齐8字节*/
} FC_SW_GET_CBIT_RESP;

/*交换机维护自检*/
typedef struct
{
	FCUINT reserve;
} FC_SW_GET_MBIT_REQ;

typedef struct
{
	FCUCHAR cpuBit;        /*CPU自检结果 0-正常 1-错误*/
	FCUCHAR nvramBit;      /*NVRAM自检结果 0-正常 1-错误*/
	FCUCHAR flashBit;      /*FLASH自检结果 0-正常 1-错误*/
	FCUCHAR dramBit;       /*DRAM自检结果 0-正常 1-错误*/
	FCUCHAR temperatureBit;/*FPGA内核温度自检结果 0-正常 1-错误*/
	FCUCHAR voltageBit;    /*电压自检结果 0-正常 1-错误*/
	FCUCHAR reserve[2];    /*对齐8字节*/
} FC_SW_GET_MBIT_RESP;

/*请求交换机重构*/
typedef struct
{
	FCUINT schemeID;/*重构方案号 0-255*/
} FC_SW_RESTRUCT_REQ;
typedef struct
{
	FCUINT schemeID;/*重构方案号 0-255*/
	FCUINT result;  /*重构结果 0-成功 1-失败*/
} FC_SW_RESTRUCT_RESP;

/*请求NT节点重构*/
typedef struct
{
	FCUINT schemeID;/*重构方案号 0-7*/
	FCUINT fcid;    /*请求重构的节点的FCID*/
} FC_NODE_RESTRUCT_REQ;
typedef struct
{
	FCUINT schemeID;/*重构方案号 0-7*/
	FCUINT result;  /*重构结果 0-成功 1-失败*/
} FC_NODE_RESTRUCT_RESP;

/******************************************************************************
* 设备管理接口                                                                 *
*******************************************************************************/
/*查询设备*/
FCUINT FC_NP_DevQuery(FCUINT *count);

/*打开设备*/
FCUINT FC_NP_DevOpen(FCHANDLE *hDev, FCUINT devID);

/* 获取句柄 */
FCUINT FC_NP_GetHandle(FCHANDLE *hDev, FCUINT devID);

/*关闭设备*/
FCUINT FC_NP_DevClose(FCHANDLE hDev);

/*获取版本*/
FCUINT FC_NP_GetVersion(FCHANDLE hDev, FC_VERSION *version);

/*获取端口状态*/
FCUINT FC_NP_GetPortStatus(FCHANDLE hDev, FCUINT *status);

/*获取温度*/
FCUINT FC_NP_GetTemperature(FCHANDLE hDev, FCINT *temperature);

/*获取电压*/
FCUINT FC_NP_GetVoltage(FCHANDLE hDev, FCUINT *voltage);

/*获取工作状态*/
FCUINT FC_NP_GetWorkState(FCHANDLE hDev, FCUINT *state);

/*复位*/
FCUINT FC_NP_DevReset(FCHANDLE hDev, FCUINT macOffline);

/*注册事件回调函数*/
FCUINT FC_NP_RegEvtHandler(FCHANDLE hDev, FC_EVENT_FUNCPTR func);

/*注销事件回调函数*/
FCUINT FC_NP_UnRegEvtHandler(FCHANDLE hDev);

/*设置蓝图 config.c模式下使用，bin文件模式下不用*/
FCUINT FC_NP_SetBluePrint(FCHANDLE hDev, FC_BLUE_PRINT *bluePrint);

/*加载配置块*/
/*注：configBlockNo---节点在配置表的索引位从0开始例(FCID=0x10006,则configBlockNo=6),schemeNo-固定为0*/
FCUINT FC_NP_LoadConfig(FCHANDLE hDev, FCUINT configBlockNo, FCUINT schemeNo);

FCUINT FC_NP_GetLocalPortID(FCHANDLE hDev, FCUINT* LocalPortID);

/*地面模式保存蓝图到FLASH*/
#if 0
FCUINT FC_NP_SaveBluePrint(FCHANDLE hDev);
#else
FCUINT FC_NP_SaveBluePrint(FCHANDLE hDev, FC_FTP_PARAM *ftpParam);
#endif

/*615A模式保存蓝图到FLASH*/
FCUINT FC_NP_615ASaveBluePrint(FCHANDLE hDev, FCUCHAR *binBuf, FCUINT length);

/*地面模式写FPGA映像文件*/
#if 0 
FCUINT FC_NP_BurnFpgaImage(FCHANDLE hDev, FCUCHAR *fileName);
#else
FCUINT FC_NP_BurnFpgaImage(FCHANDLE hDev, FC_FTP_PARAM *ftpParam, FCCHAR *fileName);
#endif

/*615A模式写FPGA映像文件*/
FCUINT FC_NP_615ABurnFpgaImage(FCHANDLE hDev, FCUCHAR *binBuf, FCUINT length);

/*注册中断接收ASM消息的回调函数*/
FCUINT FC_NP_RegMsgHandler(FCHANDLE hDev, FC_ASM_FUNCPTR func);

/*注销中断接收ASM消息的回调函数*/
FCUINT FC_NP_UnRegMsgHandler(FCHANDLE hDev);

/*获取端口统计信息*/
FCUINT FC_NP_GetPortStatisticsInfo(FCHANDLE hDev, FC_PORT_STATISTICS_INFO *psi);

/******************************************************************************
* 通信管理接口                                                                 *
*******************************************************************************/
/*发送ASM消息*/
FCUINT FC_NP_SendMsg(FCHANDLE hDev, FCUINT msgID, FCUCHAR *pMsg, FCUINT nLen);


/*接收ASM消息*/
/*注：nLen---即是输入也是输出*/
FCUINT FC_NP_RecvMsg(FCHANDLE hDev, FCUINT msgID, FCUCHAR *pMsg, FCUINT *nLen);

/******************************************************************************
* 时统管理接口                                                                 *
*******************************************************************************/
/*设置日期*/
FCUINT FC_TM_SetDate(FCHANDLE hDev, FC_DATE *date);

/*设置时间*/
FCUINT FC_TM_SetTime(FCHANDLE hDev, FC_TIME *time);

/*获取RTC*/
FCUINT FC_TM_GetRtc(FCHANDLE hDev, FC_DATE *date, FC_TIME *time);

/*设置时钟同步使能*/
FCUINT FC_TM_SetSyncEnab(FCHANDLE hDev, FCUINT enab);

/*设置时钟管理角色*/
FCUINT FC_TM_SetTmRole(FCHANDLE hDev, FCUINT role);

/*获取时钟管理角色*/
FCUINT FC_TM_GetTmRole(FCHANDLE hDev, FCUINT *role);

/******************************************************************************
* 网络管理接口                                                                 *
*******************************************************************************/
/*查询NCT是否在线*/
FCUINT FC_NM_NctIsOnLine(FCHANDLE hDev, FCUINT *state);

/*上网请求*/
FCUINT FC_NM_OnLineRequest(FCHANDLE hDev);

/*下网请求*/
FCUINT FC_NM_OffLineRequest(FCHANDLE hDev);

/*强制指定节点下线*/
FCUINT FC_NM_ForceOffLine(FCHANDLE hDev, FCUINT fcid);

/*获取所有节点的网络状态*/
FCUINT FC_NM_GetNetStateTable(FCHANDLE hDev, FC_NET_NODE_STATE_TABLE *netNodeStateTable);

/*获取指定节点的网络状态*/
FCUINT FC_NM_GetNodeState(FCHANDLE hDev, FCUINT fcid, FCUINT *netNodeState);

/*获取节点上电自检信息*/
FCUINT FC_NM_GetPBitResult(FCHANDLE hDev, FCUINT fcid, FC_PBIT_RESULT *bitResult);

/*获取节点周期自检信息*/
FCUINT FC_NM_GetCBitResult(FCHANDLE hDev, FCUINT fcid, FC_CBIT_RESULT *bitResult);

/*获取节点维护自检信息*/
FCUINT FC_NM_GetMBitResult(FCHANDLE hDev, FCUINT fcid, FC_MBIT_RESULT *bitResult);

/*获取指定节点的统计信息*/
FCUINT FC_NM_GetNetStatisticsInfo(FCHANDLE hDev, FCUINT fcid, FC_NET_STATISTICS_INFO *nsi);

/*设置网络管理角色*/
FCUINT FC_NM_SetNmRole(FCHANDLE hDev, FCUINT role);

/*获取网络管理角色*/
FCUINT FC_NM_GetNmRole(FCHANDLE hDev, FCUINT *role);

/*设置WDT定时器使能或禁止*/
FCUINT FC_NM_SetWdtPeriodEnab(FCHANDLE hDev, FCUINT enab);

/*获取WDT定时器的状态*/
FCUINT FC_NM_GetWdtPeriodEnab(FCHANDLE hDev, FCUINT *enab);
#if 0
/*获取交换机的端口链路状态表*/
FCUINT FC_NM_GetSwPortLinkTable(FCHANDLE hDev, FC_SW_PORT_LINK_TABLE *swPortLinkTable);
#endif
/*交换机网络数据监视控制*/
FCUINT FC_NM_SwMonitorCtrl(FCHANDLE hDev, FCUINT swIndex, FC_SW_MONITOR_CTRL_REQ *req, FC_SW_MONITOR_CTRL_RESP *resp);

/*获取交换机监控状态*/
FCUINT FC_NM_GetSwMonitorState(FCHANDLE hDev, FCUINT swIndex, FC_SW_MONITOR_STATE_REQ *req, FC_SW_MONITOR_STATE_RESP *resp);

/*获取交换机上电自检信息*/
FCUINT FC_NM_GetSwPbitResult(FCHANDLE hDev, FCUINT swIndex, FC_SW_GET_PBIT_REQ *req, FC_SW_GET_PBIT_RESP *resp);

/*获取交换机周期自检信息*/
FCUINT FC_NM_GetSwCbitResult(FCHANDLE hDev, FCUINT swIndex, FC_SW_GET_CBIT_REQ *req, FC_SW_GET_CBIT_RESP *resp);

/*获取交换机维护自检信息*/
FCUINT FC_NM_GetSwMbitResult(FCHANDLE hDev, FCUINT swIndex, FC_SW_GET_MBIT_REQ *req, FC_SW_GET_MBIT_RESP *resp);

/*获取交换机版本*/
FCUINT FC_NM_GetSwVersion(FCHANDLE hDev, FCUINT swIndex, FC_SW_VERSION_INFO *swVersionInfo);

/*请求交换机重构*/
FCUINT FC_NM_SwRestruct(FCHANDLE hDev, FC_SW_RESTRUCT_REQ *req, FC_SW_RESTRUCT_RESP *resp);

/*NC节点发送重构请求*/
FCUINT FC_NM_NtRestruct(FCHANDLE hDev, FC_NODE_RESTRUCT_REQ *req);

/*NT节点发送重构请求的回复*/
FCUINT FC_NM_NtRestructAck(FCHANDLE hDev, FC_NODE_RESTRUCT_RESP *resp);

#ifdef __cplusplus
}
#endif

#endif /*__FC_FH180_API_H__*/
