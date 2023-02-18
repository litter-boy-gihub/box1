/******************************************************************************
* �����ӿ����ο����ӿڿ�ͷ�ļ�����                                            *
******************************************************************************/
#ifndef __FC_API_H__
#define __FC_API_H__

#ifdef __cpluscplus
extern "C" {
#endif

#define FC_API_DLL  _declspec(dllexport)

/******************************************************************************
*���Ͷ���                                                                     *
******************************************************************************/
typedef int                FCINT;
typedef unsigned int       FCUINT;
typedef unsigned long long FCUINT64;
typedef int               *FCHANDLE;
typedef unsigned char      FCUCHAR;

/*ASM���ջص�����*/
typedef FCUINT (*FC_ASM_RECVMSG_FUNCPTR)(FCHANDLE hDev, FCUINT msgID);

/*�жϻص�����*/
typedef FCUINT(*FC_INT_FUNCPTR)(FCHANDLE hDev, FCUINT intCode, FCUINT param0);

/******************************************************************************
*��������                                                                     *
******************************************************************************/
#define FC_SUCCESS                      0x00000000 /*�ɹ�*/
#define FC_FAILURE                      0x00000001 /*ʧ��*/
#define FC_INVALID_HANDLE               0x00000002 /*�����Ч*/
#define FC_INVALID_PARAMETER            0x00000003 /*������Ч*/
#define FC_INVALID_CARD_STATE           0x00000004 /*�豸״̬��Ч*/
#define FC_ENUM_DEVICE_FAILED           0x00000005 /*ö���豸ʧ��*/
#define FC_OPEN_DEVICE_FAILED           0x00000006 /*���豸ʧ��*/
#define FC_DEVICE_IO_FAILED             0x00000007 /*IOCTRLʧ��*/
#define FC_READ_FILE_FAILED             0x00000008 /*������ʧ��*/
#define FC_WRITE_FILE_FAILED            0x00000009 /*д����ʧ��*/
#define FC_WAIT_OBJECT_FAILED           0x0000000A /*�ȴ��ź�����ʱ*/
#define FC_IOCTRL_RESP_NG               0x0000000B /*IOCTRL��������ʧ��*/
#define FC_LINK_DOWN                    0x0000000C /*�˿�linkdown*/
#define FC_SYNC_IO_WAIT_FAILED          0x0000000D /*����ELS֡�Ժ�ȴ�����ELS֡��ʱ*/
#define FC_ROLE_ILLE_OPERATION          0x0000000E /*��ɫ�Ƿ�����*/
#define FC_ELS_SEND_FAILED              0x0000000F /*����ELS֡ʧ��*/

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

#define FC_SW_NUM_MAX                            2   /*����������Ŀ*/
#define FC_SW_PORT_NUM_MAX                       48  /*36�������Ķ˿���Ŀ*/
#define SW_INFO_LENGH                            204 /*��������Ϣ����*/


/*��������ɫ*/
#define ROLE_NC                         1            /*���������*/
#define ROLE_BNC                        2            /*�������������*/
#define ROLE_NRT                        3            /*����Զ���ն�*/

#define CLK_SYNC_ROLE_SERVER_MASTER     1            /*����ʱ�ӷ�������ģ��ʱ�ӿͻ���*/
#define CLK_SYNC_ROLE_CLIENT            2            /*����ʱ�ӿͻ���*/

#define SYNC_MODE_SERVER                1            /*������-���͹㲥��Ϣ*/
#define SYNC_MODE_CLIENT                2            /*�ͻ���-���չ㲥��Ϣ*/

#define FC_MAJOR_SCHEME_MAX             128          /*�����ͼ����*/
#define FC_MINOR_SCHEME_MAX             4            /*ÿ����ͼ�����ÿ����*/

#define WDT_LOCAL_MODE                  1
#define WDT_NET_MODE                    2

/*�ֶ��Լ���*/
typedef struct
{
	FCUINT flash;        /*flash��д*/
	FCUINT buffer;		 /*�շ���������д*/
	FCUINT temperature;  /*�¶�*/
	FCUINT voltage;      /*��ѹ*/
} FC_BIT_RESULT_MANUAL;

/*�ϵ��Լ���*/
typedef struct
{
	FCUINT memory;       /*�ڴ����*/
	FCUINT reg;          /*�Ĵ�����д*/
	FCUINT temperature;  /*�¶�*/
	FCUINT voltage;      /*��ѹ*/
	FCUINT flash;        /*flash��д*/
	FCUINT els_loop;     /*ELS֡�Ի�*/
	FCUINT asm_loop;     /*ASM�Ի�*/
} FC_BIT_RESULT_POWER;

/*�Լ���*/
typedef struct
{
	FCUINT memory;       /*�ڴ����*/
	FCUINT buffer;		 /*�շ���������д*/
	FCUINT reg;          /*�Ĵ�����д*/
	FCUINT temperature;  /*�¶�*/
	FCUINT voltage;      /*��ѹ*/
	FCUINT flash;        /*flash��д*/
	FCUINT els_loop;     /*ELS֡�Ի�*/
	FCUINT asm_loop;     /*ASM�Ի�*/
} FC_BIT_RESULT;

/*ʱ��ṹ��*/
typedef struct
{
	FCUINT hour;         /*ʱ*/
	FCUINT minute;       /*��*/
	FCUINT second;       /*��*/
	FCUINT millisecond;  /*����*/
	FCUINT microsecond;  /*΢��*/
	FCUINT nanosecond;   /*����*/
} FCTime;

/*���ڽṹ��*/
typedef struct
{
	FCUINT year;         /*��*/
	FCUINT month;        /*��*/
	FCUINT day;          /*��*/
} FCDate;

/*����״̬��*/
typedef struct
{
	FCUCHAR Status[SW_PORT_NUM]; /*0: off net, 1: on net*/
} OnNetTbl;

/*�ڵ�ͳ����Ϣ*/
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

/*������link״̬��*/
typedef struct
{
	FCUCHAR Status[SW_NUM][SW_PORT_NUM]; /*0: off line, 1: on line*/
} SwLinkStatusTbl;

/*��������������*/
typedef struct
{
    FCUINT configMode;
	FCUINT configLength;
	FCUINT checksum;
} SwConfig;

/*������������������*/
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

/*������link״̬��*/
typedef struct
{
	FCUCHAR Status[FC_SW_PORT_NUM_MAX]; /*0: off line, 1: on line*/
} SwLinkStatusTbl_STR;

/******************************************************************************
*��ͼ����                                                                     *
******************************************************************************/
/*�˿���Ϣ*/
typedef struct
{
	FCUCHAR port_name[16];      /*���ض˿ڵ�����*/
	FCUINT  port_id;            /*����port_id*/
	FCUINT  port_type;          /*�˿����� 1-ASM*/
	FCUINT  credit_num;         /*����*/
	FCUINT  role;               /*�˿ڵ���������ɫ 1-��������� 2-������������� 3-�����ն�*/
	FCUINT  clk_sync_role;      /*ʱ��ͬ����ɫ 1-����ʱ�ӷ�������ģ��ʱ�ӿͻ��� 2-����ʱ�ӿͻ���*/
	FCUINT  nc_wdt_period;      /*�����������WDT��ʱ����*/
	FCUINT  wdt_loader_period;  /*WDT��������send els frame*/
	FCUINT  clk_sync_period;    /*ʱ��ͬ������ set to fpga*/
	FCUINT  nonblock_txmsg_num; /*�����ݿ鷢����Ϣ������*/
	FCUINT  nonblock_rxmsg_num; /*�����ݿ������Ϣ������*/
	FCUINT  block_txmsg_num;    /*���ݿ鷢����Ϣ������*/
	FCUINT  block_rxmsg_num;    /*���ݿ������Ϣ������*/
} FC_PORT_CONFIG;

typedef struct
{
	FCUCHAR msg_name[16];/*��Ϣ������*/
	FCUINT  msg_id;      /*������ϢID*/
	FCUINT  msg_type;    /*��Ϣ���� 1-����2-�¼�3-����Ϣ*/
	FCUINT  msg_priority;/*��Ϣ���ȼ�0-����Ϣ1-ELS��Ϣ2-�¼���Ϣ3-������Ϣ*/
	FCUINT  msg_src_id;  /*��Ϣ��ԴASM�˿�port_id*/
	FCUINT  msg_dst_id;  /*��Ϣ��Ŀ��ASM�˿�port_id*/
} FC_MSG_SEND_ITEM;

typedef struct
{
	FCUCHAR msg_name[16];      /*��Ϣ������*/
	FCUINT  msg_id;            /*������ϢID*/
	UINT32  msg_length;        /*�����Ϣ���ֽ���*/
	FCUINT  msg_type;          /*��Ϣ���� 1-����2-�¼�3-����Ϣ*/
	FCUINT  msg_src_id;        /*��Ϣ��ԴASM�˿�port_id*/
	FCUINT  msg_recover;       /*��Ϣ���ǿ��Ʊ�־0-������1-����*/
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



#define FC_SW_PORT_MAX                           48                  /*�������Ķ˿��������ֵ*/
#define FC_SW_FNU_UNIT_CONFIG_INFO_SOFTWARE_MAX  3                   /**/
#define FC_SW_OPTICAL_MODULE_MAX                 8                   /*����շ�����Ϣ�����ֵ*/



/*FNU��Ԫ������汾*/
typedef struct {
    FCUINT  systemId ;      /*ϵͳID ����ϵͳ=2*/
    FCUINT  subSystemID ;   /*��ϵͳID �������ǵ�����ϵͳ=0x01, ���ݹ����¼��ϵͳ=0x02, �����������ϵͳ=0x03, �ۺϴ����ϵͳ=0x04*/
    FCUINT  deviceId ;      /*�ۺ�ͼ����Ԫ=0x01, �ۺ���Ƶ����ƽ̨=0x02, ���Ĵ������ƽ̨=0x03, ���������ƷOFP*/
    FCUINT  LRUID ;         /*���ݾ����豸���� ����OFP�����Ʒ�˴���д0*/
    FCUINT  SoftWareID ;    /*���ID ���ݾ����豸���������д ����OFP���������Ʒ�˴���д01*/
    

    FCUINT unitCode;         /*������Ԫ����*/
    
    FCUINT versionA  ;    /*���屾��*/
    FCUINT versionB  ;    /*�ΰ屾��*/
    FCUINT versionC  ;    /*����״̬��ʶ*/
    FCUINT BugFixNum ;    /*�������bug�޶��汾��*/

    FCUINT year     ;   /*�汾���� ��*/
    FCUINT month    ;   /*��*/
    FCUINT day      ;   /*��*/

    FCUINT hour     ;   /*ʱ*/
    FCUINT minute   ;   /*��*/
    FCUINT second   ;   /*��*/   

} FC_SW_FNU_UNIT_CONFIG_SOFTWARE_INFO;

/*�������ϱ� ����汾��Ϣ*/
typedef struct
{
	FC_SW_FNU_UNIT_CONFIG_SOFTWARE_INFO swSoftWareInfo[3]; 
} FC_SW_FNU_UNIT_CONFIG_SOFTWARE_INFO_STR;

typedef struct
{
	FCUINT direct;      /*��ʾ��ģ���Ƿ��ͻ��ǽ��� 0-���� 1-����*/
	FCUINT number;      /*��ģ����*/
	FCUINT temperature; /*��ģ���¶�*/ 
	FCUINT voltage;     /*��ģ��ʵ�ʵ�ѹ������1000��*/
	FCUINT power[12];   /*ÿ����ģ���12���޹���ֵ*/
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
*�豸����ӿ�                                                                 *
******************************************************************************/
/*��ѯ�豸*/
FC_API_DLL FCUINT FC_NP_QueryDev(FCUINT *devCount);

/*��FC�ڵ����N�˿�,ΪFC�豸������Դ*/
FC_API_DLL FCUINT FC_NP_Open(FCHANDLE *hDev, FCUINT devId);

/*�ر�FC�ڵ����N�˿�*/
FC_API_DLL FCUINT FC_NP_Close(FCHANDLE hDev);

/*��ȡFC�ڵ����N�˿�״̬*/
FC_API_DLL FCUINT FC_NP_GetState(FCHANDLE hDev, FCUINT* fcState);

/*��ȡFC�ڵ����N�˿ڰ汾��*/
FC_API_DLL FCUINT FC_NP_GetVersion(FCHANDLE hDev, FCUINT* version);

/*��λFC�ڵ����N�˿�*/
FC_API_DLL FCUINT FC_NP_SoftReset(FCHANDLE hDev);

/*����FC�ڵ����N�˿�BIT����*/
FC_API_DLL FCUINT FC_NP_IBit(FCHANDLE hDev, FC_BIT_RESULT_MANUAL *bitResult);

/*ȡ���Լ���*/
FC_API_DLL FCUINT FC_NP_GetPuBitResult(FCHANDLE hDev, FC_BIT_RESULT_POWER *bitResult);

/*ע����������¼�������*/
FC_API_DLL FCUINT FC_NP_RegEvtHandler(FCHANDLE hDev, FC_INT_FUNCPTR func);

/*ע����������¼�������*/
FC_API_DLL FCUINT FC_NP_UnRegEvtHandler(FCHANDLE hDev);

/*�����·���ȡ�����õ�portID*/
FCUINT FC_NP_GetLocalPortID(FCHANDLE hDev, FCUINT* LocalPortID);

/*��ȡ����ͨ���ڵ㿨�ײ�ά��ΪDDS����ṩ��ʶ����*/
FCUINT FC_NP_GetGUID(FCHANDLE hDev,FCUINT *pGUID);
/* ��ȡ�����豸�ľ�� */
FCUINT FC_NP_GetDeviceHandle(FCHANDLE *hDev);
/******************************************************************************
*ͨ�Ź���ӿ�                                                                 *
******************************************************************************/
/*����FC�ڵ����ASMͨ��ģʽ*/
FC_API_DLL FCUINT FC_NP_ConfigMode(FCHANDLE hDev, FCUINT configMode);

/*����FC�ڵ����ASM����*/
FC_API_DLL FCUINT FC_NP_LoadConfig(FCHANDLE hDev,BLUE_PRINT *bluePrint, FCUINT majorScheme, FCUINT minorScheme);

/*ж��FC�ڵ����ASM����*/
FC_API_DLL FCUINT FC_NP_UnLoadConfig(FCHANDLE hDev);

/*��flash��������*/
FC_API_DLL FCUINT FC_NP_FlashConfig(FCHANDLE hDev);

/*����ָ����ASM��Ϣ*/
FC_API_DLL FCUINT FC_NP_SendMsg(FCHANDLE hDev, FCUINT msgID, FCUCHAR *pMsg, FCUINT nLen);

/*����ָ����ASM��Ϣ*/
FC_API_DLL FCUINT FC_NP_RecvMsg(FCHANDLE hDev, FCUINT msgID, FCUCHAR *pMsg, FCUINT *nLen);

/*ע��ASM��Ϣ������*/
FC_API_DLL FCUINT FC_NP_RegMsgHandler(FCHANDLE hDev, FC_ASM_RECVMSG_FUNCPTR func);

/*ע��ASM��Ϣ������*/
FC_API_DLL FCUINT FC_NP_UnRegMsgHandler(FCHANDLE hDev);

/******************************************************************************
*ʱͳ����ӿ�                                                                 *
******************************************************************************/
/*��ȡ��������״̬�Ƿ�����*/
FC_API_DLL FCUINT FC_NP_GetWorkStatus(FCHANDLE hDev, FCUINT *status);

/*��ȡ�¶�*/
FC_API_DLL FCUINT FC_NP_GetTemp(FCHANDLE hDev, FCUINT *temperature);

/*��ȡ�˿ڵ�link״̬*/
FC_API_DLL FCUINT FC_NP_GetPortLinkStatus(FCHANDLE hDev, int port, FCUINT *status);

/*������Ϣ���Ϳ���*/
FC_API_DLL FCUINT FC_NP_Run(FCHANDLE hDev, FCUINT runStatus);

/*��ȡFC�ڵ����ʱ�� highRTC�ĸ�10λlow RTC�ĵ�32λ*/
FC_API_DLL FCUINT FC_TM_GetRtc(FCHANDLE hDev, FCTime *time);

/*����FC�ڵ����ʱ��*/
FC_API_DLL FCUINT FC_TM_SetRtc(FCHANDLE hDev, FCTime time);

/*��ȡFC�ڵ��������*/
FC_API_DLL FCUINT FC_TM_GetDateTime(FCHANDLE hDev, FCDate *date, FCTime *time);

/*����FC�ڵ��������*/
FC_API_DLL FCUINT FC_TM_SetDate(FCHANDLE hDev, FCDate date);

/*���ýڵ��ͬ����ɫ*/
FC_API_DLL FCUINT FC_TM_SyncModeSet(FCHANDLE hDev, FCUINT mode);

/*ȡ�ýڵ��ͬ����ɫ*/
FC_API_DLL FCUINT FC_TM_SyncModeGet(FCHANDLE hDev, FCUINT *mode);

/*ʹ��ͬ������*/
FC_API_DLL FCUINT FC_TM_SyncEnable(FCHANDLE hDev);

/*��ֹͬ������*/
FC_API_DLL FCUINT FC_TM_SyncDisable(FCHANDLE hDev);

/*��ȡ����㲥ʱ�ӷ������˿ں�*/
FCUINT FC_TM_TimerServerPortID(FCHANDLE hDev,FCUINT *pPortID);
/******************************************************************************
*�������ӿ�                                                                 *
*******************************************************************************/
/*���ö˿ڵ���������ɫ*/
FC_API_DLL FCUINT FC_NM_RoleSet(FCHANDLE hDev, FCUINT value);

/*ȡ�ö˿ڵ���������ɫ*/
FC_API_DLL FCUINT FC_NM_RoleGet(FCHANDLE hDev, FCUINT *value);

/*ȡ������״̬��*/
FC_API_DLL FCUINT FC_NM_GetOnNetTable(FCHANDLE hDev, OnNetTbl *pOnNetTbl);

/*ȡ�ñ�������״̬*/
FC_API_DLL FCUINT FC_NM_GetOnNetState(FCHANDLE hDev, FCUINT *pState);

/*ȡ�ñ���ͳ����Ϣ*/
FC_API_DLL FCUINT FC_NM_GetLocalStatistics(FCHANDLE hDev, NetStatistics *pNetStatistics);

/*���ؽ���������*/
FC_API_DLL FCUINT FC_NM_NC_LoadSwConfig(FCHANDLE hDev, FCUINT portId, SwConfig *pSwConfig);

/*���ؽ�������������*/
FC_API_DLL FCUINT FC_NM_NC_LoadSwConfigData(FCHANDLE hDev, FCUINT portId, SwConfigData *pSwConfigData);

/*ȡ�ý�����link״̬*/
FC_API_DLL FCUINT FC_NM_GetSwLinkStatus(FCHANDLE hDev, SwLinkStatusTbl *pSwLinkStatusTbl);

/*ȡ�ý������Լ���*/
FC_API_DLL FCUINT FC_NM_NC_GetSwBitResult(FCHANDLE hDev, FCINT port, FCUINT64 *pSwBitResult);

/*���ý�����������*/
FC_API_DLL FCUINT FC_NM_NC_SetMonitorScheme(FCHANDLE hDev, FCINT port, FCINT SchemeNo);

/*��Ȩ��������*/
FC_API_DLL FCUINT FC_NM_NC_GrantNrtOnNet(FCHANDLE hDev, FCUINT portId);

/*��Ȩ��������*/
FC_API_DLL FCUINT FC_NM_NC_GrantNrtOffNet(FCHANDLE hDev, FCUINT portId);

/*�����ع�����*/
FC_API_DLL FCUINT FC_NM_NC_NetRestruct(FCHANDLE hDev, FCUINT configNum);

/*ȡ������ڵ��Լ���*/
FC_API_DLL FCUINT FC_NM_NC_GetNrtBitResult(FCHANDLE hDev, FCUINT portId, FCUINT *pNrtBitResult);

/*ȡ������ڵ�ͳ����Ϣ*/
FC_API_DLL FCUINT FC_NM_NC_GetNetStatistics(FCHANDLE hDev, FCUINT portId, NetStatistics *pNetStatistics);

/*�ж�����ڵ��Ƿ�����*/
FC_API_DLL FCUINT FC_NM_NRT_NcIsOnNet(FCHANDLE hDev, FCUINT *pNcIsOnNet);

/*������������*/
FC_API_DLL FCUINT FC_NM_NRT_OnNetReq(FCHANDLE hDev);

/*������������*/
FC_API_DLL FCUINT FC_NM_NRT_OffNetReq(FCHANDLE hDev);

/*ʹ��WDT*/
FC_API_DLL FCUINT FC_NM_WDTEnable(FCHANDLE hDev, FCUINT mode);

/*��ֹWDT*/
FC_API_DLL FCUINT FC_NM_WDTDisable(FCHANDLE hDev, FCUINT mode);

/*ʹ��ʱ��WDT*/
FC_API_DLL FCUINT FC_NM_DateWDTEnable(FCHANDLE hDev, FCUINT mode);
/*ȡ��WDT��Ϣ*/
FC_API_DLL FCUINT FC_NM_GetWDTValue(FCHANDLE hDev, FCUINT *pWdt, FCUINT *pWdtLoader);

/*дFPGAӳ���ļ�*/
FC_API_DLL FCUINT FC_NM_NC_SendDatetoSw(FCHANDLE hDev,FC_SW_Date *pFCDate);
FC_API_DLL FCUINT FC_SimElsSend(FCHANDLE hDev, FCUINT key);


#ifdef __cpluscplus
}
#endif

#endif /*__FC_API_H__*/
