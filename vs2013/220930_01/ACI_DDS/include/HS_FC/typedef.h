#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*类型定义*/
typedef short			  FCSHORT;
typedef int               FCINT;
typedef unsigned int      FCUINT;
typedef unsigned __int64  FCUINT64;
typedef int              *FCHANDLE;
typedef unsigned char     FCUCHAR;

/*ASM接收回调定义*/
typedef FCUINT (*FC_ASM_RECVMSG_FUNCPTR)(FCHANDLE hDev, FCUINT msgID);


/*中断回调定义*/
typedef FCUINT(*FC_INT_FUNCPTR)(FCHANDLE hDev, FCUINT intCode, FCUINT param0);

///////////////////////////////////////////////////////////////////////////////
// 常量定义
///////////////////////////////////////////////////////////////////////////////
#define FC_SUCCESS                      0x00000000
#define FC_FAILURE                      0x00000001
#define FC_INVALID_HANDLE               0x00000002
#define FC_INVALID_PARAMETER            0x00000003
#define FC_INVALID_CARD_STATE           0x00000004
#define FC_ENUM_DEVICE_FAILED           0x00000005
#define FC_OPEN_DEVICE_FAILED           0x00000006
#define FC_DEVICE_IO_FAILED             0x00000007
#define FC_READ_FILE_FAILED             0x00000008
#define FC_WRITE_FILE_FAILED            0x00000009
#define FC_WAIT_OBJECT_FAILED           0x0000000A
#define FC_IOCTRL_RESP_NG               0x0000000B
#define FC_SEND_TIMEOUT					0x0000000C
#define FC_RECV_TIMEOUT					0x0000000D
#define FC_LINK_DOWN                    0x0000000C /*端口linkdown*/
#define FC_SYNC_IO_WAIT_FAILED          0x0000000D /*发送ELS帧以后等待接收ELS帧超时*/
#define FC_ROLE_ILLE_OPERATION          0x0000000E /*角色非法操作*/
#define FC_ELS_SEND_FAILED              0x0000000F /*发送ELS帧失败*/

#define FC_NODE_STATE_ON_LINE			1		   /*节点在线*/
#define FC_NODE_STATE_OFF_LINE          0          /*节点不在线*/

#define SW_NUM                          2
#define SW_PORT_NUM                     48
#define NT_OFF_NET                      0
#define NT_ON_NET                       1
#define NT_OFF_LINE                     0
#define NT_ON_LINE                      1
#define MAJOR_VER                       1
#define MINOR_VER                       0
#define PORT_LINK_DOWN                  0
#define PORT_LINK_UP                    1
#define PORT_SPEED_1G                   1
#define PORT_SPEED_2G                   2
#define PORT_SPEED_4G                   3
#define RUN_STATUS_DISABLE              0
#define RUN_STATUS_ENAB                 1

#define CHANNEL_NUM_MAX                 256

// 中断子码定义
#define IC_VOLTAGE_SUCCESS              0x100      /*电压从错误值变为正常值*/
#define IC_VOLTAGE_FAILED               0x101      /*电压错误*/
#define IC_TEMP_SUCCESS                 0x102      /*温度从错误值变为正常值*/
#define IC_TEMP_FAILED                  0x103      /*温度错误*/
#define IC_LINK_STATUS_SUCCESS          0x104      /*link状态从错误值变为正常值*/
#define IC_LINK_STATUS_FAILED           0x105      /*link状态错误*/
#define IC_IO_BUF_WRITE_FAILED          0x106      /*收发缓冲区写错误*/
#define IC_IO_BUF_READ_FAILED           0x107      /*收发缓冲区读错误*/
#define IC_DISCARD_MSG           		0x108      /*消息丢弃*/
#define IC_COVER_MSG             		0x109      /*消息覆盖*/
#define IC_IU_TIMEOUT             		0x10A      /*IU超时*/
#define IC_IU_SEQ_ERR             		0x10B      /*IU超时*/
#define IC_IU_LENGTH_INVALID         	0x10C      /*IU长度无效*/
#define IC_ELS_RECV_RECONSTRUCT         0x200      /*重构*/
#define IC_ELS_RECV_NC_ON_NET           0x201      /*NC在网*/
#define IC_ELS_RECV_NC_TIMEOUT          0x202      /*WDT超时*/
#define IC_ELS_RECV_RECONSTRUCT_SUCCESS 0x203      /*重构完成*/
#define IC_ELS_RECV_RECONSTRUCT_FAIL    0x204      /*重构失败*/
#define IC_NC_CONFLICT                  0x208      /*NC冲突*/
#define IC_NC_CHANGED                   0x209      /*NC切换*/

#define CONFIG_MODE_PROJECT             0
#define CONFIG_MODE_FLASH               1
#define IU_LENGTH_MAX                   (16 * 1024 * 1024 + 4096) // 最大的IU字节数+4K
/*网络管理角色*/
#define ROLE_NC                         1            /*网络控制器*/
#define ROLE_BNC                        2            /*备份网络管理器*/
#define ROLE_NRT                        3            /*网络远程终端*/

#define CLK_SYNC_ROLE_SERVER_MASTER     1            /*网络时钟服务器，模块时钟客户端*/
#define CLK_SYNC_ROLE_CLIENT            2            /*网络时钟客户端*/

#define SYNC_MODE_SERVER                1            /*服务器-发送广播消息*/
#define SYNC_MODE_CLIENT                2            /*客户端-接收广播消息*/

#define FC_MINOR_SCHEME_SIZE            (32 * 1024) /*每个配置块的字节数*/
/*每个蓝图的字节数*/
#define FC_MAJOR_SCHEME_SIZE            (FC_MINOR_SCHEME_MAX * FC_MINOR_SCHEME_SIZE)
#define WDT_LOCAL_MODE                  1
#define WDT_NET_MODE                    2

#define FC_MAJOR_SCHEME_MAX             128			/*最大蓝图个数 */
#define FC_MINOR_SCHEME_MAX             10			/*每个蓝图的配置块个数 */
#define FC_CHANNEL_NUM_MAX              256			/*发送和接收通道的个数，最大256*/



#ifdef __cplusplus
}
#endif
