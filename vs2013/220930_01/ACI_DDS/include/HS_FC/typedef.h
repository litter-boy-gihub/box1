#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*���Ͷ���*/
typedef short			  FCSHORT;
typedef int               FCINT;
typedef unsigned int      FCUINT;
typedef unsigned __int64  FCUINT64;
typedef int              *FCHANDLE;
typedef unsigned char     FCUCHAR;

/*ASM���ջص�����*/
typedef FCUINT (*FC_ASM_RECVMSG_FUNCPTR)(FCHANDLE hDev, FCUINT msgID);


/*�жϻص�����*/
typedef FCUINT(*FC_INT_FUNCPTR)(FCHANDLE hDev, FCUINT intCode, FCUINT param0);

///////////////////////////////////////////////////////////////////////////////
// ��������
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
#define FC_LINK_DOWN                    0x0000000C /*�˿�linkdown*/
#define FC_SYNC_IO_WAIT_FAILED          0x0000000D /*����ELS֡�Ժ�ȴ�����ELS֡��ʱ*/
#define FC_ROLE_ILLE_OPERATION          0x0000000E /*��ɫ�Ƿ�����*/
#define FC_ELS_SEND_FAILED              0x0000000F /*����ELS֡ʧ��*/

#define FC_NODE_STATE_ON_LINE			1		   /*�ڵ�����*/
#define FC_NODE_STATE_OFF_LINE          0          /*�ڵ㲻����*/

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

// �ж����붨��
#define IC_VOLTAGE_SUCCESS              0x100      /*��ѹ�Ӵ���ֵ��Ϊ����ֵ*/
#define IC_VOLTAGE_FAILED               0x101      /*��ѹ����*/
#define IC_TEMP_SUCCESS                 0x102      /*�¶ȴӴ���ֵ��Ϊ����ֵ*/
#define IC_TEMP_FAILED                  0x103      /*�¶ȴ���*/
#define IC_LINK_STATUS_SUCCESS          0x104      /*link״̬�Ӵ���ֵ��Ϊ����ֵ*/
#define IC_LINK_STATUS_FAILED           0x105      /*link״̬����*/
#define IC_IO_BUF_WRITE_FAILED          0x106      /*�շ�������д����*/
#define IC_IO_BUF_READ_FAILED           0x107      /*�շ�������������*/
#define IC_DISCARD_MSG           		0x108      /*��Ϣ����*/
#define IC_COVER_MSG             		0x109      /*��Ϣ����*/
#define IC_IU_TIMEOUT             		0x10A      /*IU��ʱ*/
#define IC_IU_SEQ_ERR             		0x10B      /*IU��ʱ*/
#define IC_IU_LENGTH_INVALID         	0x10C      /*IU������Ч*/
#define IC_ELS_RECV_RECONSTRUCT         0x200      /*�ع�*/
#define IC_ELS_RECV_NC_ON_NET           0x201      /*NC����*/
#define IC_ELS_RECV_NC_TIMEOUT          0x202      /*WDT��ʱ*/
#define IC_ELS_RECV_RECONSTRUCT_SUCCESS 0x203      /*�ع����*/
#define IC_ELS_RECV_RECONSTRUCT_FAIL    0x204      /*�ع�ʧ��*/
#define IC_NC_CONFLICT                  0x208      /*NC��ͻ*/
#define IC_NC_CHANGED                   0x209      /*NC�л�*/

#define CONFIG_MODE_PROJECT             0
#define CONFIG_MODE_FLASH               1
#define IU_LENGTH_MAX                   (16 * 1024 * 1024 + 4096) // ����IU�ֽ���+4K
/*��������ɫ*/
#define ROLE_NC                         1            /*���������*/
#define ROLE_BNC                        2            /*�������������*/
#define ROLE_NRT                        3            /*����Զ���ն�*/

#define CLK_SYNC_ROLE_SERVER_MASTER     1            /*����ʱ�ӷ�������ģ��ʱ�ӿͻ���*/
#define CLK_SYNC_ROLE_CLIENT            2            /*����ʱ�ӿͻ���*/

#define SYNC_MODE_SERVER                1            /*������-���͹㲥��Ϣ*/
#define SYNC_MODE_CLIENT                2            /*�ͻ���-���չ㲥��Ϣ*/

#define FC_MINOR_SCHEME_SIZE            (32 * 1024) /*ÿ�����ÿ���ֽ���*/
/*ÿ����ͼ���ֽ���*/
#define FC_MAJOR_SCHEME_SIZE            (FC_MINOR_SCHEME_MAX * FC_MINOR_SCHEME_SIZE)
#define WDT_LOCAL_MODE                  1
#define WDT_NET_MODE                    2

#define FC_MAJOR_SCHEME_MAX             128			/*�����ͼ���� */
#define FC_MINOR_SCHEME_MAX             10			/*ÿ����ͼ�����ÿ���� */
#define FC_CHANNEL_NUM_MAX              256			/*���ͺͽ���ͨ���ĸ��������256*/



#ifdef __cplusplus
}
#endif
