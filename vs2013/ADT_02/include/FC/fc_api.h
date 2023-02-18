/******************************************************************************
* API�ӿڶ���                                                                 *
* �汾��V2.00                                                               *
******************************************************************************/

#ifndef __FC_FH180_API_H__
#define __FC_FH180_API_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* ���Ͷ���                                                                     *
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
* �ص���������                                                                 *
*******************************************************************************/
/*ASM���ջص�*/
typedef FCUINT (*FC_ASM_FUNCPTR)(FCHANDLE hDev, FCUINT msgId, FCUINT iuLength);

/*�¼��ص�*/
typedef FCUINT (*FC_EVENT_FUNCPTR)(FCHANDLE hDev, FCUINT eventCode, FCUINT param1, FCUINT param2);

/*�ڴ濽���ص�*/
typedef unsigned int (*FC_MEMCPY_FUNCPTR)(void *dest, void *src, unsigned int size);

/******************************************************************************
* ��������                                                                     *
*******************************************************************************/
#define FC_PORT_LINK_DOWN                  0          /*�˿�linkdown*/
#define FC_PORT_LINK_UP                    1          /*�˿�linkup*/
#define FC_WDT_PERIOD_ENAB                 1          /*WDT��������ʹ��*/
#define FC_WDT_PERIOD_DISABLE              2          /*WDT�������ڽ�ֹ*/
#define FC_TM_SYNC_DISABLE                 0          /*ʱ��ͬ����ֹ*/
#define FC_TM_SYNC_ENAB                    1          /*ʱ��ͬ��ʹ��*/
#define FC_MAC_ON_LINE                     0          /*MAC����λ*/
#define FC_MAC_OFF_LINE                    1          /*MAC��λ*/
#define FC_SWITCH_PORT_MAX                 48         /*�������Ķ˿���*/
#define FC_NET_NODE_MAX                    48         /*����ڵ��������*/
#define FC_CONFIG_BLOCK_MAX                4         /*������ÿ����*/
#define FC_SCHEME_MAX                      4          /*��󷽰���*/
#define FC_CHANNEL_NUM_MAX                 512        /*ASMͨ����������*/

/*��ɫ*/
#define FC_NET_ROLE_NCT                    1          /*����������ڵ�*/
#define FC_NET_ROLE_BNCT                   2          /*���籸�ݿ������ڵ�*/
#define FC_NET_ROLE_NRT                    3          /*����Զ�̽ڵ�*/
#define FC_TIME_ROLE_CCS                   1          /*ʱ�ӷ�����*/
#define FC_TIME_ROLE_CCT                   2          /*ʱ�ӿͻ���*/

/*��Ϣ����*/
#define FC_MSG_TYPE_RAW                    0          /*RAW��Ϣ*/
#define FC_MSG_TYPE_URGNEY                 1          /*����*/
#define FC_MSG_TYPE_EVENT                  2          /*�¼�*/
#define FC_MSG_TYPE_STREAM                 3          /*����Ϣ*/

/*��Ϣ���շ�ʽ*/
#define FC_MSG_RECV_MODE_POLL              0          /*��ѯģʽ����*/
#define FC_MSG_RECV_MODE_INT               1          /*�ж�ģʽ����*/

/******************************************************************************
* �ж�����                                                                     *
*******************************************************************************/
#define IC_PORT_LINK_UP                    0x00000100 /*link״̬�Ӵ����Ϊ����.����0:�ж���;����1,ͨ��״̬:(1,ͨ��1����;2,ͨ��2����;3,ͨ��1��ͨ��2������)*/
#define IC_PORT_LINK_DOWN                  0x00000101 /*link״̬��������Ϊ����.����0:�ж���;����1,ͨ��״̬:(1,ͨ��1����;2,ͨ��2����;3,ͨ��1��ͨ��2������)*/
#define IC_VOLTAGE_GOOD                    0x00000102 /*��ѹ�Ӵ����Ϊ����.����0:�ж���;����1,��ǰ��ѹ;����2,��ѹ��ֵ;����3,��ѹ��ֵ*/
#define IC_VOLTAGE_FAILED                  0x00000103 /*��ѹ��������Ϊ����.����0:�ж���;����1,��ǰ��ѹ;����2,��ѹ��ֵ;����3,��ѹ��ֵ*/
#define IC_TEMPERATURE_GOOD                0x00000104 /*�¶ȴӴ����Ϊ����.����0:�ж���;����1,��ǰ�¶�;����2,������ֵ;����3,������ֵ*/
#define IC_TEMPERATURE_FAILED              0x00000105 /*�¶ȴ�������Ϊ����.����0:�ж���;����1,��ǰ�¶�;����2,������ֵ;����3,������ֵ*/
#define IC_RX_IU_DISCARD                   0x00000108 /*����IU��FPGA����.����0:�ж���;����1,MsgID;����2,������.*/
#define IC_RX_IU_TIMEOUT                   0x0000010A /*����IU����ʱ.����0:�ж���.*/
#define IC_RX_IU_LENGTH_INVALID            0x0000010B /*����IU�ĳ�����Ч.����0:�ж���.*/
#define IC_WDT_INIT_TIMEOUT                0x0000010E /*WDT��ʼֵ�ȴ���ʱ ����0���ж��� ����1��WDT��ʼֵ*/
#define IC_WDT_TIMEOUT                     0x0000010F /*WDT��ʱ���ڳ�ʱ�ж�.����0:�ж���;����1,���б�־(1���������У�0��δ��������) */
#define IC_NCT_ON_NET                      0x00000110 /*NCT��һ������*/
#define IC_NC_CHANGE                       0x00000112 /*NC�����л�.����0:�ж���;����1,�л����NC PortID*/
#define IC_NET_NODE_STATE_TABLE_CHANGED    0x00000114 /*�������仯.����0:�ж���;����1,bit0-bit31(ǰ32���豸);����2,bit0-bit15����16�豸��*/
#define IC_ON_LINE_INTR                    0x00000115 /*��������.����0:�ж���.*/
#define IC_OFF_LINE_INTR                   0x00000116 /*��������.����0:�ж���.*/
#define IC_ON_LINE_REQ_INTR                0x00000117 /*(NC��Ч)�ڵ�����������ж�.����0:�ж���;����1,�������߽ڵ�˿�ID.*/
#define IC_OFF_LINE_REQ_INTR               0x00000118 /*(NC��Ч)�ڵ�����������ж�.����0:�ж���;����1,�������߽ڵ�˿�ID.*/
#define IC_NC_CONFLICT_INTR                0x00000119 /*NC��ͻ�ж�.����0:�ж���;����1,ԭNC�˿�ID;��ͻNC�˿�ID.*/
#define IC_RTC_DELTA_OVER_INTR             0x0000011A /*RTC������ֵ�ж�.����0:�ж���.*/
#define IC_SW_LINK_CHANGE                  0x00000121 /*��������LINK�����仯������0:1-SW1 LINK 2-SW2 LINK 3-SW1/SW2 LINK ����1:1-SW1 LINK DOWN 2-SW2 LINK DOWN 3-SW1/SW2 LINK DOWN*/
#define IC_NC_CONSTRUCT_REQ                0x00000122 /*NT�ڵ��յ�NC���͵��ع����󣬲���0: �ع������� ����1�������ع���NRT�ڵ��FCID*/
#define IC_NT_CONSTRUCT_RESP               0x00000123 /*NC�ڵ��յ�NT���͵��ع�����Ļظ�������0: �ع������� ����1��NRT�ڵ���ع���� 0 �ɹ� ��0ʧ��*/

/******************************************************************************
* ������                                                                       *
*******************************************************************************/
#define FC_SUCCESS                         0x00000000 /*�ɹ�*/
#define FC_FAILURE                         0xE3000000 /*ʧ��*/
#define FC_PARAMETER_INVALID               0xE3000001 /*��Ч�Ĳ���*/
#define FC_GUID_INVALID                    0xE3000002 /*��Ч��GUID*/
#define FC_INSUFFICIENT_RESOURCE           0xE3000003 /*��Դ����*/
#define FC_GET_INTERFACE_FAILED            0xE3000004 /*��ȡ�豸�ӿ�ʧ��*/
#define FC_TCSCPY_FAILED                   0xE3000005 /*����ʧ��*/
#define FC_NOT_FOUND_DEVICE                0xE3000006 /*û���ҵ��豸*/
#define FC_GET_READ_HANDLE_FAILED          0xE3000007 /*��ȡ�����ʧ��*/
#define FC_GET_WRITE_HANDLE_FAILED         0xE3000008 /*��ȡд���ʧ��*/
#define FC_GET_IOCTRL_HANDLE_FAILED        0xE3000009 /*��ȡ���ƾ��ʧ��*/
#define FC_OPEN_DEVICE_FAILED              0xE300000A /*���豸ʧ��*/
#define FC_DEVICE_STATE_INVALID            0xE300000B /*��Ч���豸״̬*/
#define FC_DEVICE_ID_INVALID               0xE300000C /*��Ч���豸ID*/
#define FC_CONFIG_MODE_INVALID             0xE300000D /*��Ч������ģʽ*/
#define FC_READ_FAILED                     0xE300000E /*������ʧ��*/
#define FC_WRITE_FAILED                    0xE300000F /*д����ʧ��*/
#define FC_IOCTRL_FAILED                   0xE3000010 /*IOCTRL����ʧ��*/
#define FC_EVT_HANDLER_REGED               0xE3000011 /*�¼��жϴ������Ѿ�ע��*/
#define FC_ASM_HANDLER_REGED               0xE3000012 /*ASM��Ϣ�����жϴ������Ѿ�ע��*/
#define FC_UNREG_EVT_HANDLER_FAILED        0xE3000013 /*ע���¼��жϴ�����ʧ��*/
#define FC_UNREG_ASM_HANDLER_FAILED        0xE3000014 /*ע��ASM��Ϣ�����жϴ�����ʧ��*/
#define FC_CREATE_TASK_FAILED              0xE3000015 /*��������ʧ��*/
#define FC_CREATE_EVENT_FAILED             0xE3000016 /*�����ź���ʧ��*/
#define FC_WAIT_EVENT_FAILED               0xE3000017 /*�ȴ��ź���ʧ��*/
#define FC_MSG_ID_INVALID                  0xE3000018 /*��Ч����ϢID*/
#define FC_RECV_MODE_INVALID               0xE3000019 /*��Ч�Ľ���ģʽ*/
#define FC_SEND_LENGTH_INVALID             0xE300001A /*��Ч�ķ��ͳ���*/
#define FC_SEND_BUF_FULL                   0xE300001B /*���ͻ�������*/
#define FC_SEND_TIMEOUT                    0xE300001C /*���ͳ�ʱ*/
#define FC_RECV_LENGTH_INVALID             0xE300001D /*��Ч�Ľ��ճ���*/
#define FC_RECV_NO_DATA                    0xE300001E /*����û������*/
#define FC_RECV_TIMEOUT                    0xE300001F /*���ճ�ʱ*/
#define FC_BUF_TOO_SMALL                   0xE3000020 /*������̫С*/
#define FC_IOCTRL_RESP_NG                  0xE3000021 /*IOCTRL�����ʧ��ֵ*/
#define FC_HANDLE_INVALID                  0xE3000022 /*��Ч�ľ��*/
#define FC_NM_ROLE_INVALID                 0xE3000023 /*��Ч����������ɫ*/
#define FC_TM_ROLE_INVALID                 0xE3000024 /*��Ч��ʱ�ӹ����ɫ*/
#define FC_IMAGE_FILE_OPEN_FAILED          0xE3000025 /*ӳ���ļ���ʧ��*/
#define FC_IMAGE_FILE_READ_FAILED          0xE3000026 /*ӳ���ļ���ȡʧ��*/
#define FC_READ_FILE_BUF_TOO_SMALL         0xE3000027 /*��ӳ���ļ���ȡ���ݵĻ�����̫С*/
#define FC_START_TASK_FAILURE              0xE3000028 /*��������ʧ��*/
#define FC_MSG_PRIORITY_INVALID            0xE3000029 /*��Ч����Ϣ���ȼ�*/
#define FC_BIT_FAILED                      0xE300002A /*�Լ�ʧ��*/
#define FC_GET_SEND_BUF_TIMEOUT            0xE300002B /*�ӷ�������ȡ�û�������ʱ*/
#define FC_NM_NODE_OFFLINE                 0xE300002C /*�ڵ�û������*/
#define FC_NM_NC_NODE_OFFLINE              0xE300002D /*�ڵ�NCû������*/
#define FC_NM_LENGTH_INVALID               0xE300002E /*�յ����������֡�ĳ��Ȳ���ȷ*/
#define FC_NM_ROLE_ILLE_OPERATION          0xE300002F /*��������ɫ����ȷ*/
#define FC_NM_WAIT_RESP_TIMEOUT            0xE3000030 /*�����������֡��ȴ��ظ���ʱ*/
#define FC_NM_LINK_DOWN                    0xE3000031 /*�����������֡ʱ�˿�linkdown*/
#define FC_NM_SEND_FAILED                  0xE3000032 /*�����������֡ʧ��*/
#define FC_TM_ROLE_ILLE_OPERATION          0xE3000033 /*ʱ�ӷ����ɫ����ȷ*/
#define FC_SPI_OP_BUSY                     0xE3000034 /*SPI����æ*/
#define FC_FLASH_READ_FAILED               0xE3000035 /*ͨ��SPI��FLASHʧ��*/
#define FC_FLASH_WRITE_FAILED              0xE3000036 /*ͨ��SPIдFLASHʧ��*/
#define FC_FTP_XFER_ERROR                  0xE3000037 /*FTP����ʧ��*/
#define FC_FTP_READ_ERROR                  0xE3000038 /*FTP���ļ�ʧ��*/

/******************************************************************************
 ������                                                          *
*******************************************************************************/
#define FC_RX_IU_DISCARD_BUFOUT            0xE4000001 /* Ӧ�ý��ջ����� */
#define FC_RX_IU_DISCARD_PRIORITY		   0xE4000002 /* ���յ���ASM֡�����ȼ���ͨ���������ȼ���ƥ�䵼�¶��� */
#define FC_RX_IU_DISCARD_SEQ               0xE4000003 /* fc��Ϣ֡ seq ���� */
#define FC_RX_IU_DISCARD_OXID              0xE4000004 /* fc��Ϣ֡ oxid ���� */
#define FC_RX_IU_DISCARD_VALID             0xE4000005 /* fc��Ϣ֡ ��Ϣ��Դ����Ŀ����Ч */

/******************************************************************************
* ��ͼ���Ĵ����붨��                                                          *
*******************************************************************************/
/*��ͼ�Ĵ�����*/
#define FC_EB_HEADER_FILE_TYPE             0xEB000000 /*��Ч����ͼͷ���ļ�����*/
#define FC_EB_HEADER_LENGTH                0xEB000001 /*��Ч����ͼͷ�ĳ���*/
#define FC_EB_HEADER_VERSION               0xEB000002 /*��Ч����ͼͷ�İ汾*/
#define FC_EB_HEADER_DATE                  0xEB000003 /*��Ч����ͼͷ������*/
#define FC_EB_HEADER_CRC                   0xEB000004 /*��Ч����ͼͷ��CRC*/
#define FC_EB_HEADER_LRM_SIG               0xEB000005 /*��Ч����ͼͷ��LRM_SIG*/
#define FC_EB_HEADER_LRM_NUM               0xEB000006 /*��Ч����ͼͷ��LRM_NUM*/
#define FC_EB_HEADER_LRM_ID                0xEB000007 /*��Ч����ͼͷ��LRM_ID*/
#define FC_EB_HEADER_SOFT_VERSION          0xEB000008 /*��Ч����ͼͷ������汾*/
#define FC_EB_CONFIG_BLOCK_NUM             0xEB000009 /*��Ч�����ÿ����*/
#define FC_EB_CONFIG_BLOCK_NO              0xEB00000A /*��Ч�����ÿ�����*/
#define FC_EB_SCHEME_NUM                   0xEB00000B /*��Ч�ķ�������*/

/*�������Ĵ�����*/
#define FC_EI_CONFIG_BLOCK_NUM             0xE1000000 /*��Ч�����ÿ����*/
#define FC_EI_CONFIG_BLOCK_NO              0xE1000001 /*��Ч�����ÿ�����*/
#define FC_EI_CRC                          0xE1000002 /*��Ч��CRC*/
#define FC_EI_OFFSET                       0xE1000003 /*��Ч��ƫ��*/
#define FC_EI_LENGTH                       0xE1000004 /*��Ч�ĳ���*/
#define FC_EI_RANGE                        0xE1000005 /*��Ч�ķ�Χ*/
#define FC_EI_SCHEME_NUM                   0xE1000006 /*��Ч�ķ�������*/
#define FC_EI_SCHEME_NO                    0xE1000007 /*��Ч�ķ�������*/
#define FC_EI_SCHEME_CRC                   0xE1000008 /*��Ч�ķ���CRC*/
#define FC_EI_SCHEME_OFFSET                0xE1000009 /*��Ч�ķ���ƫ��*/
#define FC_EI_SCHEME_LENGTH                0xE100000A /*��Ч�ķ�������*/
#define FC_EI_SCHEME_RANGE                 0xE100000B /*��Ч�ķ�����Χ*/

/*���ÿ�Ĵ�����*/
#define FC_EC_SCHEME_NO                    0xEC000000 /*��Ч�ķ�������*/
#define FC_EC_PORT_ID                      0xEC000001 /*��Ч�Ķ˿�ID*/
#define FC_EC_PORT_NAME                    0xEC000002 /*��Ч�Ķ˿�����*/
#define FC_EC_NM_ROLE                      0xEC000003 /*��Ч����������ɫ*/
#define FC_EC_TM_ROLE                      0xEC000004 /*��Ч��ʱ�ӷ����ɫ*/
#define FC_EC_WDT_INIT_VALUE               0xEC000005 /*��Ч��WDT��ʼֵ*/
#define FC_EC_WDT_PERIOD                   0xEC000006 /*��Ч��WDT��ʱ����ֵ*/
#define FC_EC_WDT_LOADER_PERIOD            0xEC000007 /*��Ч��WDT��������ֵ*/
#define FC_EC_CLK_SYNC_PERIOD              0xEC000008 /*��Ч��ʱ��ͬ������*/
#define FC_EC_BLOCK_TXMSG_NUM              0xEC000009 /*��Ч�Ŀ�ģʽ������Ϣ��*/
#define FC_EC_BLOCK_RXMSG_NUM              0xEC00000A /*��Ч�Ŀ�ģʽ������Ϣ��*/
#define FC_EC_STREAM_TXMSG_NUM             0xEC00000B /*��Ч����ģʽ������Ϣ��*/
#define FC_EC_STREAM_RXMSG_NUM             0xEC00000C /*��Ч����ģʽ������Ϣ��*/
#define FC_EC_TXMSG_NUM                    0xEC00000D /*��Ч�ķ�����Ϣ����*/
#define FC_EC_RXMSG_NUM                    0xEC00000E /*��Ч�Ľ�����Ϣ����*/
#define FC_EC_TX_MSG_ID                    0xEC00000F /*��Ч�ķ�����ϢID*/
#define FC_EC_TX_MSG_NAME                  0xEC000010 /*��Ч�ķ�����Ϣ����*/
#define FC_EC_TX_MSG_MAX_LEN               0xEC000011 /*��Ч�ķ�����Ϣ��󳤶�*/
#define FC_EC_TX_MSG_PRIORITY              0xEC000012 /*��Ч�ķ�����Ϣ���ȼ�*/
#define FC_EC_TX_MSG_SRC_ID                0xEC000013 /*��Ч�ķ�����ϢԴID*/
#define FC_EC_TX_MSG_DST_ID                0xEC000014 /*��Ч�ķ�����ϢĿ��ID*/
#define FC_EC_TX_MSG_TYPE                  0xEC000015 /*��Ч�ķ�����Ϣ����*/
#define FC_EC_TX_MSG_REFRESH               0xEC000016 /*��Ч�ķ�����Ϣˢ�±��*/
#define FC_EC_RX_MSG_ID                    0xEC000017 /*��Ч�Ľ�����ϢID*/
#define FC_EC_RX_MSG_NAME                  0xEC000018 /*��Ч�Ľ�����Ϣ����*/
#define FC_EC_RX_MSG_MAX_LEN               0xEC000019 /*��Ч�Ľ�����Ϣ��󳤶�*/
#define FC_EC_RX_MSG_TYPE                  0xEC00001A /*��Ч�Ľ�����Ϣ����*/
#define FC_EC_RX_MSG_RECV_MODE             0xEC00001B /*��Ч�Ľ�����Ϣ����ģʽ*/
#define FC_EC_RX_MSG_SRC_ID                0xEC00001C /*��Ч�Ľ�����ϢԴID*/
#define FC_EC_RX_MSG_RECOVER               0xEC00001D /*��Ч�Ľ�����Ϣ���Ǳ��*/
/******************************************************************************
* ���幫������ֵ                                                               *
*******************************************************************************/
#define FC_HANDLE_ERROR                    (0xE6310001)/*�豸�������*/
#define FC_IN_PARAM_ERROR                  (0xE6310002)/*�����������ָ�����*/
#define FC_OUT_PARAM_ERROR                 (0xE6310003)/*�����������ָ�����*/
#define FC_PORT_ID_ERROR                   (0xE6310004)/*�豸PORT ID����*/
#define FC_MSG_ID_ERROR                    (0xE6310005)/*�豸MSGID����*/
#define FC_MEM_MALLOC_ERROR                (0xE6310006)/*�豸�����ڴ����*/
#define FC_MEM_FREE_ERROR                  (0xE6310007)/*�豸�ͷ��ڴ����*/
#define FC_DEV_ID_ERROR                    (0xE6310008)/*�豸ID����*/
#define FC_DEV_STS_ERROR                   (0xE6310009)/*�豸״̬����*/
#define FC_OPTION_FAILURE                  (0xE631000A)/*ʧ��*/
/******************************************************************************
* �����豸������ֵ                                                            *
*******************************************************************************/
#define FC_DEV_RESET_ERROR                 (0xE6310101)/*�豸��λ����*/
#define FC_DEV_STS_SET_ERROR               (0xE6310102)/*�豸״̬���ô���*/
#define FC_DEV_OPEN_ERROR                  (0xE6310103)/*�豸�򿪴���*/
#define FC_DEV_CLOSE_ERROR                 (0xE6310104)/*�豸�رմ���*/
#define FC_DEV_ENABLE_ERROR                (0xE6310105)/*�豸ʹ�ܴ���*/
#define FC_DEV_VER_REG_ERROR               (0xE6310106)/*�豸�汾ע�����*/
#define FC_DEV_PORT_REG_ERROR              (0xE6310107)/*�豸�˿�ע�����*/
#define FC_DEV_MSG_TX_REG_ERROR            (0xE6310108)/*�豸��Ϣ����ע�����*/
#define FC_DEV_MSG_RX_REG_ERROR            (0xE6310109)/*�豸��Ϣ����ע�����*/
#define FC_DEV_PART_REG_ERROR              (0xE6310110)/*�豸�������ñ�ע�����*/
#define FC_DEV_RECFG_REG_ERROR             (0xE6310111)/*�豸�ع����ñ�ע�����*/
#define FC_DEV_VER_GET_ERROR               (0xE6310112)/*�豸�汾��ȡ����*/
#define FC_DEV_BUFF_CFG_ERROR              (0xE6310113)/*���������ô���*/
#define FC_DEV_REG_CALLBACK_ERROR          (0xE6310114)/*�豸ע���û��ص�����*/
#define FC_DEV_SET_INTRMASK_ERROR          (0xE6310115)/*�豸�ж��������ô���*/
#define FC_DEV_ENBLE_INTRMASK_ERROR        (0xE6310116)/*�豸�ж�ʹ�ܴ���*/
#define FC_DEV_HEART_CHECK_ERROR           (0xE6310117)/*�豸����������*/
#define FC_DEV_CFG_LOAD_ERROR              (0xE6310118)/*�豸���ü��ش���*/
#define FC_DEV_VL_CFG_ERROR                (0xE6310119)/*�豸����·���ô���*/
#define FC_DEV_FLASH_CHECK_ERROR           (0xE6310120)/*�豸FLASH ID������*/
#define FC_DEV_DISABLE_INTRMASK_ERROR      (0xE6310121)/*�豸�ж������������*/
#define FC_DEV_CFGTBL_BIT_ERROR            (0xE6310122)/*�豸���ñ�BIT����*/
#define FC_DEV_RTC_BIT_ERROR               (0xE6310123)/*FC���ñ�BIT������*/
#define FC_DEV_STATUS_BIT_ERROR            (0xE6310124)/*FC�豸״̬BIT������*/
#define FC_DEV_FLASH_BIT_ERROR             (0xE6310125)/*FC�豸FLASH BIT������*/
#define FC_DEV_CPU_BIT_ERROR               (0xE6310126)/*CPU BIT������*/
#define FC_DEV_RAM_BIT_ERROR               (0xE6310127)/*RAM BIT������*/
#define FC_DEV_CLK_CFG_ERROR               (0xE6310128)/*ʱ�����ô���*/
#define FC_DEV_NTK_CFG_ERROR               (0xE6310129)/*�������ô���*/
#define FC_DEV_TEMP_VALUE_ERROR            (0xE6310130)/*�¶�����ֵ����*/
#define FC_DEV_TEMP_SET_ERROR              (0xE6310131)/*�¶�����ֵ���ô���*/
#define FC_DEV_TEMP_GET_ERROR              (0xE6310132)/*�¶�����ֵ��ȡ����*/
#define FC_DEV_CFG_FILE_VER_GET_ERROR      (0xE6310133)/*FC�����ļ���ȡ����*/
#define FC_DEV_NVRAM_DATALEN_ERROR         (0xE6310134)/*FC�豸NVRAMͨ�ų��ȴ���*/
#define FC_DEV_HOST_CALLBAK_REG_ERROR      (0xE6310135)/*FC�豸�����ӿ������û��ص�ע��ʧ��*/
#define FC_DEV_HOST_CFG_ERROR              (0xE6310136)/*FC�豸�����ӿ����ô���*/
#define FC_DEV_DMA_ALLOC_ERROR             (0xE6310137)/*FC�豸ͨ�Ż������������*/
#define FC_DEV_MSG_QUEUE_CFG_ERROR         (0xE6310138)/*MSGID��������ʧ��*/
#define FC_DEV_NODE_NO_ERROR               (0xE6310139)/*�ڵ���������*/
#define FC_DEV_SCHEME_NO_ERROR             (0xE6310140)/*������������*/
#define FC_DEV_VOLTAGE_SET_ERROR           (0xE6310141)/*��ѹ����ֵ���ô���*/
#define FC_DEV_VOLTAGE_GET_ERROR           (0xE6310142)/*��ѹ����ֵ��ȡ����*/
#define FC_DEV_VOLTAGE_ERROR               (0xE6310143)/*��ǰ��ѹ����*/
#define FC_DEV_TEMP_ERROR                  (0xE6310144)/*��ǰ�¶ȴ���*/
/******************************************************************************
* ����ͨ�Ź�����ֵ                                                            *
*******************************************************************************/
#define FC_CMM_RUN_ERROR                   (0xE6310200)/*FCͨ��ʹ�ܴ���*/
#define FC_CMM_STOP_ERROR                  (0xE6310201)/*FCͨ�Ž�ֹ����*/
#define FC_CMM_MSGID_ERROR                 (0xE6310202)/*FCͨ��MSGID����*/
#define FC_CMM_CFG_MSG_ERROR               (0xE6310203)/*FCͨ�Ż�ȡ���õ�����Ϣ��Ϣ����*/
#define FC_CMM_BUFF_MSG_ERROR              (0xE6310204)/*FCͨ�Ż�ȡ��������Ԫ��ȡ����*/
#define FC_CMM_BUFF_ADDR_ERROR             (0xE6310205)/*FCͨ�Ż�ȡ��������ַ����*/
#define FC_CMM_MSGLEN_ERROR                (0xE6310206)/*FCͨ����Ϣ���ȴ���*/
#define FC_CMM_ADDR_ERROR                  (0xE6310207)/*FCͨ����Ϣ��ַ����*/
#define FC_CMM_PART_ORG_ERROR              (0xE6310208)   
#define FC_CMM_MIH_ORG_ERROR               (0xE6310209)   
#define FC_CMM_ASM_ORG_ERROR               (0xE6310210)/*FC ASM��Ϣ��֯����*/
#define FC_CMM_ASM_SEND_ERROR              (0xE6310211)/*FC ASM��Ϣ���ʹ���*/
#define FC_CMM_PART_PARSE_ERROR            (0xE6310212)   
#define FC_CMM_MIH_PARSE_ERROR             (0xE6310213)   
#define FC_CMM_ASM_PARSE_ERROR             (0xE6310214)/*FC ASM��Ϣ��������*/
#define FC_CMM_ASM_RECV_ERROR              (0xE6310215)/*FC ASM��Ϣ���մ���*/
#define FC_CMM_ASM_RECV_CPT_ERROR          (0xE6310216)/*FC ASM��Ϣ����������ô���*/
#define FC_CMM_ASM_ADDR_REG_ERROR          (0xE6310217)/*FCͨ�Ż���������ע�����*/
#define FC_CMM_SEND_STATE_ERROR            (0xE6310218)/*FC����Ϣ����״̬����*/ 
#define FC_CMM_LOCAL_STATE_ERROR           (0xE6310219) 
#define FC_CMM_DEST_STATE_ERROR            (0xE6310220)
/******************************************************************************
* ���幫ͨ�Ź�����ֵ                                                          *
*******************************************************************************/
#define FC_ELS_HANDLE_ERROR                (0xE6310300)/*ELS�������*/
#define FC_ELS_IN_PARAM_ERROR              (0xE6310301)/*�����������*/
#define FC_ELS_OUT_PARAM_ERROR             (0xE6310302)/*�����������*/
#define FC_ELS_OPT_ERROR                   (0xE6310303)/*���ô���*/
#define FC_ELS_MALLOC_ERROR                (0xE6310304)/*�ڴ��������*/
#define FC_ELS_SPAWN_TASK_ERROR            (0xE6310305)/*������������*/
#define FC_ELS_FUNCTION_ID_ERROR           (0xE6310306)/*����ID����*/
#define FC_ELS_DATA_LEN_ERROR              (0xE6310307)/*���ݳ��ȴ���*/
#define FC_ELS_MSG_LEN_ERROR               (0xE6310308)/*��Ϣ���ȴ���*/
#define FC_ELS_ADDR_ERROR                  (0xE6310309)/*���ݵ�ַ����*/
/******************************************************************************
* �������������ֵ                                                            *
*******************************************************************************/
#define FC_NTK_MSG_LEN_ERROR               (0xE6310400)/*��Ϣ���ȴ���*/
#define FC_NTK_PORT_ID_ERROR               (0xE6310401)/*�˿�ID����*/ 
#define FC_NTK_ROLE_ERROR                  (0xE6310402)/*�����ɫ����*/
#define FC_NTK_WDT_MODE_ERROR              (0xE6310403)/*WDTģʽ����*/
#define FC_NTK_CHANNEL_GET_ERROR           (0xE6310404)/*ͨ����ȡ����*/
#define FC_NTK_SW_MONI_CTRL_GET_ERROR      (0xE6310405) 
#define FC_NTK_SW_MONI_STATE_GET_ERROR     (0xE6310406)
#define FC_NTK_SW_BIT_GET_ERROR            (0xE6310407)/*������BIT��ȡ����*/
#define FC_NTK_SW_VER_GET_ERROR            (0xE6310408)/*�������汾��ȡ����*/
#define FC_NTK_SW_RESTRUCT_ERROR           (0xE6310409)/*�������ع�����*/
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
* ����ʱ�ӹ�����ֵ                                                            *
*******************************************************************************/
#define FC_CLK_ROLE_ERROR                  (0xE6310500)/*ʱ�ӽ�ɫ����*/

/******************************************************************************
* ���ÿ鶨��                                                                   *
*******************************************************************************/
/* ��ͼͷ��Ϣ*/
typedef struct
{
	FCUINT  file_type;       /*��ͼ����*/
	FCINT   length;          /*���з�����Ч���ݵ��ֽڳ��ȣ���������ͼͷ��Ϣ*/
	FCUCHAR version[16];     /*�汾��Ϣ*/
	FCUINT  date;            /*��ͼ����*/
	FCUINT  crc;             /*���з�����Ч���ݵ��ֽںͣ���������ͼͷ��Ϣ*/
	FCUINT  lrm_sig;         /*ͬ����LRMģ���豸��ʶ(�˿ڵ�ַ PORT_ID) ����*/
	FCSHORT lrm_num;         /*ͬ����LRMģ���豸�ܸ���(���4��) ����*/
	FCSHORT lrm_id;          /*��������ͬ����LRMģ���豸�еı�� ����*/
	FCUCHAR soft_version[24];/*������ù��ߵİ汾 ����*/
} FC_BLUE_PRINT_HEADER;

/*�˿���Ϣ*/
typedef struct
{
	FCUINT  port_id;          /*�˿�FCID*/
	FCUCHAR port_name[16];    /*�˿�����*/
	FCUINT  nm_role;          /*��������ɫ*/
	FCUINT  tm_role;          /*ʱ��ͬ����ɫ*/
	FCUINT  wdt_init_value;   /*���������������ʼֵ ��λ����*/
	FCUINT  wdt_period;       /*WDT����ֵ ��λ����*/
	FCUINT  wdt_loader_period;/*���������WDT��ʱ���� ��λ����*/
	FCUINT  clk_sync_period;  /*ʱ��ͬ������ ��λ���� һ������Ϊ50ms*/
	FCUINT  block_txmsg_num;  /*���ݿ鷢����Ϣ����   0��512*/
	FCUINT  block_rxmsg_num;  /*���ݿ������Ϣ����   0��512*/
	FCUINT  stream_txmsg_num; /*�����ݿ鷢����Ϣ���� 0��512*/
	FCUINT  stream_rxmsg_num; /*�����ݿ������Ϣ���� 0��512*/
} FC_PORT_CONFIG;

/*���Ϳ�*/
typedef struct
{
	FCUINT  msg_id;           /*��ϢID*/
	FCUCHAR msg_name[16];     /*��Ϣ����*/
	FCUINT  msg_max_len;      /*���͵�����ֽ���*/
	FCUINT  msg_src_id;       /*��Ϣ��ԴFCID*/
	FCUINT  msg_dst_id;       /*��Ϣ��Ŀ��FCID*/
	FCUINT  msg_type;         /*��Ϣ���� 1-���� 2-�¼� 3-����Ϣ*/
} FC_MSG_SEND_ITEM;

/*���տ�*/
typedef struct
{
	FCUINT  msg_id;           /*��ϢID*/
	FCUCHAR msg_name[16];     /*��Ϣ����*/
	FCUINT  msg_max_len;      /*���յ�����ֽ���*/
	FCUINT  msg_type;         /*��Ϣ���� 1-���� 2-�¼� 3-����Ϣ*/
	FCUINT  msg_recv_mode;    /*��Ϣ���շ�ʽ 1-��ѯ 2-�ж�*/
	FCUINT  msg_src_id;       /*��Ϣ��ԴFCID*/
	FCUINT  msg_recover;      /*��Ϣ���Ǳ�־ 0-������ 1-����*/
} FC_MSG_RECV_ITEM;

/*����*/
typedef struct
{
	FCUINT scheme_no;                              /*���������� ��0��ʼ*/
	FC_PORT_CONFIG port_config;                    /*�˿���Ϣ*/
	FC_MSG_SEND_ITEM send_item[FC_CHANNEL_NUM_MAX];/*����ͨ��*/
	FC_MSG_RECV_ITEM recv_item[FC_CHANNEL_NUM_MAX];/*����ͨ��*/
} FC_SCHEME;

/*�ڵ����ÿ�*/
typedef struct
{
	FCUINT    config_block_no;      /*���ÿ������� ��0��ʼ*/
	FCUINT    scheme_num;           /*��������*/
	FC_SCHEME scheme[FC_SCHEME_MAX];/*����*/
} FC_CONFIG_BLOCK;

/*��ͼ*/
typedef struct
{
	FC_BLUE_PRINT_HEADER blue_print_header;                /*��ͼͷ��Ϣ*/
	FCUINT               config_block_num;                 /*�ڵ����*/
	FC_CONFIG_BLOCK      config_block[FC_CONFIG_BLOCK_MAX];/*�ڵ�����*/
} FC_BLUE_PRINT;

/*�ڵ��ϵ��Լ�*/
typedef struct
{
	FCUCHAR temperature;/*�¶ȵ�״̬ 0-���� 1-����*/
	FCUCHAR voltage;    /*��ѹ��״̬ 0-���� 1-����*/
	FCUCHAR flash;      /*flash��д��״̬ 0-���� 1-����*/
	FCUCHAR reg;        /*�Ĵ�����д��״̬ 0-���� 1-����*/ 
	FCUCHAR els_loop;   /*ELS֡�Ի���״̬ 0-���� 1-����*/
	FCUCHAR asm_loop;   /*ASM�Ի���״̬ 0-���� 1-����*/
	FCUCHAR reserve[2]; /*����8�ֽ�*/
} FC_PBIT_RESULT;

/*�ڵ������Լ�*/
typedef struct
{
	FCUCHAR temperature;/*�¶ȵ�״̬ 0-���� 1-����*/
	FCUCHAR voltage;    /*��ѹ��״̬ 0-���� 1-����*/
	FCUCHAR reserve[6]; /*����8�ֽ�*/
} FC_CBIT_RESULT;

/*�ڵ�ά���Լ�*/
typedef struct
{
	FCUCHAR temperature;/*�¶ȵ�״̬ 0-���� 1-����*/
	FCUCHAR voltage;    /*��ѹ��״̬ 0-���� 1-����*/
	FCUCHAR flash;      /*flash��д��״̬ 0-���� 1-����*/
	FCUCHAR reg;        /*�Ĵ�����д��״̬ 0-���� 1-����*/ 
	FCUCHAR reserve[4]; /*����8�ֽ�*/
} FC_MBIT_RESULT;

/*����*/
typedef struct
{
	FCUINT year; /*��*/
	FCUINT month;/*��*/
	FCUINT day;  /*��*/
} FC_DATE;

/*ʱ��*/
typedef struct
{
	FCUINT hour;       /*ʱ*/
	FCUINT minute;     /*��*/
	FCUINT second;     /*��*/
	FCUINT millisecond;/*����*/
	FCUINT microsecond;/*΢��*/
	FCUINT nanosecond; /*����*/
} FC_TIME;

/*FTP����*/
typedef struct
{
	FCUCHAR hostIp[16];/*������IP��ַ�����15���ַ���\0��β*/
	FCUCHAR user[16];  /*�û��������15���ַ���\0��β*/
	FCUCHAR pwd[16];   /*���룬���15���ַ���\0��β*/
} FC_FTP_PARAM;

/*�汾*/
typedef struct
{
	FCUCHAR fpga[16];           /*FPGA�汾�ţ����15���ַ���\0��β*/
	FCUCHAR fpga_date[20];		/*�߼��汾���ɵ����ڣ����18���ַ���\0��β*/
	FCUCHAR api[16];            /*API�汾�ţ����15���ַ���\0��β*/
	FCUCHAR api_date[20];		/*�����汾���ڣ����15���ַ���\0��β*/
	FCUCHAR blue_print_ver[16]; /*��ͼ�İ汾�ţ����15���ַ���\0��β*/
	FCUCHAR blue_print_date[20];/*��ͼ�汾�������ڣ����18���ַ���\0��β*/
} FC_VERSION;

/*�˿�ͳ����Ϣ*/
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

/*����ڵ��ͳ����Ϣ*/
typedef struct
{
	FC_PORT_STATISTICS_INFO port_statistics_info[2];
} FC_NET_STATISTICS_INFO;

/*����״̬��*/
typedef struct
{
	struct
	{
		FCUINT fcid;  /*FCID*/
		FCUINT state; /*�˿ڵ�״̬ 0-���� 1-����*/
	} netNodeState[FC_NET_NODE_MAX];
} FC_NET_NODE_STATE_TABLE;

/*�������汾��Ϣ*/
typedef struct
{
	FCUINT manufactureInfo;/*�豸������Ϣ*/
	FCUINT fpgaVersion;    /*FPGA�߼��汾*/
	FCUINT softVersion;    /*����汾*/
	FCUINT firmwareVersion;/*�̼��汾*/
} FC_SW_VERSION_INFO;

/*�������˿���·״̬��*/
typedef struct
{
	struct
	{
		FCUINT fcid;  /*FCID*/
		FCUINT state; /*�˿ڵ�״̬ 0-��·down 1-��·up*/
	} swPortLink[FC_SWITCH_PORT_MAX];/*ֻȡǰ46���ڵ�*/
} FC_SW_PORT_LINK_TABLE;

/*�������������ݼ��ӿ���*/
typedef struct
{
	FCUINT mport;    /*��ض˿ڱ�� 0-1*/
	FCUINT monitorID;/*��ط������� 0-127*/
	FCUINT command;  /*��ؿ���ָ�� 0-ֹͣ��� 1-��ʼ���*/
} FC_SW_MONITOR_CTRL_REQ;

typedef struct
{
	FCUINT state;    /*��ǰ�ļ��״̬ 0-δ��� 1-�����*/
	FCUINT monitorID;/*��ǰ��ط������� 0-127*/
	FCUINT domainID; /*ָ����������ID 1-2*/
} FC_SW_MONITOR_CTRL_RESP;

/*��ȡ���������״̬*/
typedef struct
{
	FCUINT mport; /*��ض˿ڱ�� 0-1*/
} FC_SW_MONITOR_STATE_REQ;

typedef struct
{
	FCUINT state;    /*��ǰ�ļ��״̬ 0-δ��� 1-�����*/
	FCUINT monitorID;/*��ǰ��ط������� 0-127*/
	FCUINT domainID; /*ָ����������ID 1-2*/
} FC_SW_MONITOR_STATE_RESP;

/*�������ϵ��Լ�*/
typedef struct
{
	FCUINT reserve;
} FC_SW_GET_PBIT_REQ;

typedef struct
{
	FCUCHAR cpuBit;    /*CPU�Լ��� 0-���� 1-����*/
	FCUCHAR flashBit;  /*FLASH�Լ��� 0-���� 1-����*/
	FCUCHAR ramBit;    /*RAM�Լ��� 0-���� 1-����*/	
	FCUCHAR reserve[5];/*����8�ֽ�*/
} FC_SW_GET_PBIT_RESP;

/*�����������Լ�*/
typedef struct
{
	FCUINT reserve;
} FC_SW_GET_CBIT_REQ;

typedef struct
{
	FCUCHAR temperatureBit;/*�¶� 0-���� 1-����*/
	FCUCHAR voltageBit;    /*��ѹ�Լ��� 0-���� 1-����*/	
	FCUCHAR reserve[6];    /*����8�ֽ�*/
} FC_SW_GET_CBIT_RESP;

/*������ά���Լ�*/
typedef struct
{
	FCUINT reserve;
} FC_SW_GET_MBIT_REQ;

typedef struct
{
	FCUCHAR cpuBit;        /*CPU�Լ��� 0-���� 1-����*/
	FCUCHAR nvramBit;      /*NVRAM�Լ��� 0-���� 1-����*/
	FCUCHAR flashBit;      /*FLASH�Լ��� 0-���� 1-����*/
	FCUCHAR dramBit;       /*DRAM�Լ��� 0-���� 1-����*/
	FCUCHAR temperatureBit;/*FPGA�ں��¶��Լ��� 0-���� 1-����*/
	FCUCHAR voltageBit;    /*��ѹ�Լ��� 0-���� 1-����*/
	FCUCHAR reserve[2];    /*����8�ֽ�*/
} FC_SW_GET_MBIT_RESP;

/*���󽻻����ع�*/
typedef struct
{
	FCUINT schemeID;/*�ع������� 0-255*/
} FC_SW_RESTRUCT_REQ;
typedef struct
{
	FCUINT schemeID;/*�ع������� 0-255*/
	FCUINT result;  /*�ع���� 0-�ɹ� 1-ʧ��*/
} FC_SW_RESTRUCT_RESP;

/*����NT�ڵ��ع�*/
typedef struct
{
	FCUINT schemeID;/*�ع������� 0-7*/
	FCUINT fcid;    /*�����ع��Ľڵ��FCID*/
} FC_NODE_RESTRUCT_REQ;
typedef struct
{
	FCUINT schemeID;/*�ع������� 0-7*/
	FCUINT result;  /*�ع���� 0-�ɹ� 1-ʧ��*/
} FC_NODE_RESTRUCT_RESP;

/******************************************************************************
* �豸����ӿ�                                                                 *
*******************************************************************************/
/*��ѯ�豸*/
FCUINT FC_NP_DevQuery(FCUINT *count);

/*���豸*/
FCUINT FC_NP_DevOpen(FCHANDLE *hDev, FCUINT devID);

/* ��ȡ��� */
FCUINT FC_NP_GetHandle(FCHANDLE *hDev, FCUINT devID);

/*�ر��豸*/
FCUINT FC_NP_DevClose(FCHANDLE hDev);

/*��ȡ�汾*/
FCUINT FC_NP_GetVersion(FCHANDLE hDev, FC_VERSION *version);

/*��ȡ�˿�״̬*/
FCUINT FC_NP_GetPortStatus(FCHANDLE hDev, FCUINT *status);

/*��ȡ�¶�*/
FCUINT FC_NP_GetTemperature(FCHANDLE hDev, FCINT *temperature);

/*��ȡ��ѹ*/
FCUINT FC_NP_GetVoltage(FCHANDLE hDev, FCUINT *voltage);

/*��ȡ����״̬*/
FCUINT FC_NP_GetWorkState(FCHANDLE hDev, FCUINT *state);

/*��λ*/
FCUINT FC_NP_DevReset(FCHANDLE hDev, FCUINT macOffline);

/*ע���¼��ص�����*/
FCUINT FC_NP_RegEvtHandler(FCHANDLE hDev, FC_EVENT_FUNCPTR func);

/*ע���¼��ص�����*/
FCUINT FC_NP_UnRegEvtHandler(FCHANDLE hDev);

/*������ͼ config.cģʽ��ʹ�ã�bin�ļ�ģʽ�²���*/
FCUINT FC_NP_SetBluePrint(FCHANDLE hDev, FC_BLUE_PRINT *bluePrint);

/*�������ÿ�*/
/*ע��configBlockNo---�ڵ������ñ������λ��0��ʼ��(FCID=0x10006,��configBlockNo=6),schemeNo-�̶�Ϊ0*/
FCUINT FC_NP_LoadConfig(FCHANDLE hDev, FCUINT configBlockNo, FCUINT schemeNo);

FCUINT FC_NP_GetLocalPortID(FCHANDLE hDev, FCUINT* LocalPortID);

/*����ģʽ������ͼ��FLASH*/
#if 0
FCUINT FC_NP_SaveBluePrint(FCHANDLE hDev);
#else
FCUINT FC_NP_SaveBluePrint(FCHANDLE hDev, FC_FTP_PARAM *ftpParam);
#endif

/*615Aģʽ������ͼ��FLASH*/
FCUINT FC_NP_615ASaveBluePrint(FCHANDLE hDev, FCUCHAR *binBuf, FCUINT length);

/*����ģʽдFPGAӳ���ļ�*/
#if 0 
FCUINT FC_NP_BurnFpgaImage(FCHANDLE hDev, FCUCHAR *fileName);
#else
FCUINT FC_NP_BurnFpgaImage(FCHANDLE hDev, FC_FTP_PARAM *ftpParam, FCCHAR *fileName);
#endif

/*615AģʽдFPGAӳ���ļ�*/
FCUINT FC_NP_615ABurnFpgaImage(FCHANDLE hDev, FCUCHAR *binBuf, FCUINT length);

/*ע���жϽ���ASM��Ϣ�Ļص�����*/
FCUINT FC_NP_RegMsgHandler(FCHANDLE hDev, FC_ASM_FUNCPTR func);

/*ע���жϽ���ASM��Ϣ�Ļص�����*/
FCUINT FC_NP_UnRegMsgHandler(FCHANDLE hDev);

/*��ȡ�˿�ͳ����Ϣ*/
FCUINT FC_NP_GetPortStatisticsInfo(FCHANDLE hDev, FC_PORT_STATISTICS_INFO *psi);

/******************************************************************************
* ͨ�Ź���ӿ�                                                                 *
*******************************************************************************/
/*����ASM��Ϣ*/
FCUINT FC_NP_SendMsg(FCHANDLE hDev, FCUINT msgID, FCUCHAR *pMsg, FCUINT nLen);


/*����ASM��Ϣ*/
/*ע��nLen---��������Ҳ�����*/
FCUINT FC_NP_RecvMsg(FCHANDLE hDev, FCUINT msgID, FCUCHAR *pMsg, FCUINT *nLen);

/******************************************************************************
* ʱͳ����ӿ�                                                                 *
*******************************************************************************/
/*��������*/
FCUINT FC_TM_SetDate(FCHANDLE hDev, FC_DATE *date);

/*����ʱ��*/
FCUINT FC_TM_SetTime(FCHANDLE hDev, FC_TIME *time);

/*��ȡRTC*/
FCUINT FC_TM_GetRtc(FCHANDLE hDev, FC_DATE *date, FC_TIME *time);

/*����ʱ��ͬ��ʹ��*/
FCUINT FC_TM_SetSyncEnab(FCHANDLE hDev, FCUINT enab);

/*����ʱ�ӹ����ɫ*/
FCUINT FC_TM_SetTmRole(FCHANDLE hDev, FCUINT role);

/*��ȡʱ�ӹ����ɫ*/
FCUINT FC_TM_GetTmRole(FCHANDLE hDev, FCUINT *role);

/******************************************************************************
* �������ӿ�                                                                 *
*******************************************************************************/
/*��ѯNCT�Ƿ�����*/
FCUINT FC_NM_NctIsOnLine(FCHANDLE hDev, FCUINT *state);

/*��������*/
FCUINT FC_NM_OnLineRequest(FCHANDLE hDev);

/*��������*/
FCUINT FC_NM_OffLineRequest(FCHANDLE hDev);

/*ǿ��ָ���ڵ�����*/
FCUINT FC_NM_ForceOffLine(FCHANDLE hDev, FCUINT fcid);

/*��ȡ���нڵ������״̬*/
FCUINT FC_NM_GetNetStateTable(FCHANDLE hDev, FC_NET_NODE_STATE_TABLE *netNodeStateTable);

/*��ȡָ���ڵ������״̬*/
FCUINT FC_NM_GetNodeState(FCHANDLE hDev, FCUINT fcid, FCUINT *netNodeState);

/*��ȡ�ڵ��ϵ��Լ���Ϣ*/
FCUINT FC_NM_GetPBitResult(FCHANDLE hDev, FCUINT fcid, FC_PBIT_RESULT *bitResult);

/*��ȡ�ڵ������Լ���Ϣ*/
FCUINT FC_NM_GetCBitResult(FCHANDLE hDev, FCUINT fcid, FC_CBIT_RESULT *bitResult);

/*��ȡ�ڵ�ά���Լ���Ϣ*/
FCUINT FC_NM_GetMBitResult(FCHANDLE hDev, FCUINT fcid, FC_MBIT_RESULT *bitResult);

/*��ȡָ���ڵ��ͳ����Ϣ*/
FCUINT FC_NM_GetNetStatisticsInfo(FCHANDLE hDev, FCUINT fcid, FC_NET_STATISTICS_INFO *nsi);

/*������������ɫ*/
FCUINT FC_NM_SetNmRole(FCHANDLE hDev, FCUINT role);

/*��ȡ��������ɫ*/
FCUINT FC_NM_GetNmRole(FCHANDLE hDev, FCUINT *role);

/*����WDT��ʱ��ʹ�ܻ��ֹ*/
FCUINT FC_NM_SetWdtPeriodEnab(FCHANDLE hDev, FCUINT enab);

/*��ȡWDT��ʱ����״̬*/
FCUINT FC_NM_GetWdtPeriodEnab(FCHANDLE hDev, FCUINT *enab);
#if 0
/*��ȡ�������Ķ˿���·״̬��*/
FCUINT FC_NM_GetSwPortLinkTable(FCHANDLE hDev, FC_SW_PORT_LINK_TABLE *swPortLinkTable);
#endif
/*�������������ݼ��ӿ���*/
FCUINT FC_NM_SwMonitorCtrl(FCHANDLE hDev, FCUINT swIndex, FC_SW_MONITOR_CTRL_REQ *req, FC_SW_MONITOR_CTRL_RESP *resp);

/*��ȡ���������״̬*/
FCUINT FC_NM_GetSwMonitorState(FCHANDLE hDev, FCUINT swIndex, FC_SW_MONITOR_STATE_REQ *req, FC_SW_MONITOR_STATE_RESP *resp);

/*��ȡ�������ϵ��Լ���Ϣ*/
FCUINT FC_NM_GetSwPbitResult(FCHANDLE hDev, FCUINT swIndex, FC_SW_GET_PBIT_REQ *req, FC_SW_GET_PBIT_RESP *resp);

/*��ȡ�����������Լ���Ϣ*/
FCUINT FC_NM_GetSwCbitResult(FCHANDLE hDev, FCUINT swIndex, FC_SW_GET_CBIT_REQ *req, FC_SW_GET_CBIT_RESP *resp);

/*��ȡ������ά���Լ���Ϣ*/
FCUINT FC_NM_GetSwMbitResult(FCHANDLE hDev, FCUINT swIndex, FC_SW_GET_MBIT_REQ *req, FC_SW_GET_MBIT_RESP *resp);

/*��ȡ�������汾*/
FCUINT FC_NM_GetSwVersion(FCHANDLE hDev, FCUINT swIndex, FC_SW_VERSION_INFO *swVersionInfo);

/*���󽻻����ع�*/
FCUINT FC_NM_SwRestruct(FCHANDLE hDev, FC_SW_RESTRUCT_REQ *req, FC_SW_RESTRUCT_RESP *resp);

/*NC�ڵ㷢���ع�����*/
FCUINT FC_NM_NtRestruct(FCHANDLE hDev, FC_NODE_RESTRUCT_REQ *req);

/*NT�ڵ㷢���ع�����Ļظ�*/
FCUINT FC_NM_NtRestructAck(FCHANDLE hDev, FC_NODE_RESTRUCT_RESP *resp);

#ifdef __cplusplus
}
#endif

#endif /*__FC_FH180_API_H__*/
