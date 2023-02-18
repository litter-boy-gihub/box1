#include "HS_FC/precomp.h"
/*
0 �ڵ�0 FCID=0x010000 ����0 ��ΪNCT ����2��ͨ����BNC NRT�� ����2��ͨ����BNC NRT��
1 �ڵ�1 FCID=0x010001 ����1 ��ΪBNC ����2��ͨ����NC NRT�� ����2��ͨ����NC NRT��
2 �ڵ�2 FCID=0x010002 ����2 ��ΪNRT ����2��ͨ����NC BNC�� ����2��ͨ����NC BNC��
3 �ڵ�3 FCID=0x010003 ����3 ��ΪNCT �Ի���ͨ���� ����2��ͨ��������2��ͨ��
4 �ڵ�4 FCID=0x010004 ����4 ��ΪNCT �Ի�������1��ͨ�� ����1��ͨ��
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

    /* -----------------------------------��ͼ1---------------------------------- */
	{
		{
						/* ---------------------------���ÿ�MIU1------------------------- */
			{
				/*�˿�����*/
				{
					"MIU1",               /* ���ض˿ڵ����� */
					0x010001,           /* ����port_id */
					1,                  /* �˿����� 1-ASM */
					8,                  /* ���� */
					3,                  /* �˿ڵ���������ɫ 1-nc 2-bnc 3-Ϊ����Զ���ն�*/
					2,                  /* ʱ��ͬ����ɫ2-����ʱ�ӿͻ���*/
					1000,               /* WDT��ʱ����,���г�ʱ*/
					50,                /* WDT��������,ι������*/
					500,               /* ʱ��ͬ������ */
					0,                  /* �����ݿ鷢����Ϣ������ 1��615AЭ��*/
					0,                  /* �����ݿ������Ϣ������ 1��615AЭ��*/
					67,                  /* ���ݿ鷢����Ϣ������ */
					67,                  /* ���ݿ������Ϣ������ 1��615Aӳ���ļ�*/
				},
				/*����*/
				{
					{
						"MIU1toMIU1Heart", /*��Ϣ����*/
						0X010101,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x010001,/*��Ϣ��ԴASM�˿�port_id*/
						0x010001,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					{
						"MIU1toMIU1Topic", /*��Ϣ����*/
						0X010102,	 /*��ϢID*/
						2,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x010001,/*��Ϣ��ԴASM�˿�port_id*/
						0x010001,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"MIU1toMIU1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X010100,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x010001,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010001,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					
					{
						"MIU1toDPU1Heart", /*��Ϣ����*/
						0X010C01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x010001,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000C,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					{
						"MIU1toDPU1Topic", /*��Ϣ����*/
						0X010C02,	 /*��ϢID*/
						2,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x010001,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000C,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"MIU1toDPU1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X010C00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x010001,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000C,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"MIU1toDPU2Heart", /*��Ϣ����*/
						0X010D01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x010001,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000D,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					{
						"MIU1toDPU2Topic", /*��Ϣ����*/
						0X010D02,	 /*��ϢID*/
						2,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x010001,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000D,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"MIU1toDPU2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X010D00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x010001,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000D,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toDPU4Heart", /*��Ϣ����*/
						0X0C0F01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000F,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toDPU4Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0F02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000F,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toDPU4Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0F00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000F,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toDPU5Heart", /*��Ϣ����*/
						0X0C1001,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010010,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toDPU5Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1002,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010010,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toDPU5Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1000,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010010,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toDPU6Heart", /*��Ϣ����*/
						0X0C1101,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010011,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toDPU6Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1102,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010011,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toDPU6Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1100,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010011,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toDPU7Heart", /*��Ϣ����*/
						0X0C0001,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010000,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toDPU7Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0002,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010000,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toDPU7Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0000,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010000,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toMIU1Heart", /*��Ϣ����*/
						0X0C0101,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010001,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toMIU1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0102,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010001,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toMIU1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0100,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010001,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toMIU2Heart", /*��Ϣ����*/
						0x0C0201,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010002,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toMIU2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0x0C0202,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010002,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toMIU2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0x0C0200,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010002,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toIIPHeart", /*��Ϣ����*/
						0x0C0301,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010003,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toIIPTopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0302,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010003,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toIIPData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0300,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010003,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toISP1Heart", /*��Ϣ����*/
						0X0C0401,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010004,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toISP1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0402,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010004,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toISP1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0400,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010004,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toISP2Heart", /*��Ϣ����*/
						0X0C0501,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010005,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toISP2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0502,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010005,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toISP2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0500,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010005,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toOEDEHeart", /*��Ϣ����*/
						0X0C1801,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010018,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toOEDETopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1802,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010018,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toOEDEData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1800,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010018,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU1toINE1Heart", /*��Ϣ����*/
						0X0C1901,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010019,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toINE1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1902,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010019,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toINE1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1900,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010019,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toINE2Heart", /*��Ϣ����*/
						0X0C1A01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x01001A,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toINE2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1A02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001A,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toINE2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1A00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001A,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toAIU1Heart", /*��Ϣ����*/
						0X0C1B01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x01001B,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toAIU1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1B02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001B,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toAIU1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1B00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001B,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU1toAIU2Heart", /*��Ϣ����*/
						0X0C1C01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x01001C,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toAIU2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1C02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001C,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toAIU2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1C00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001C,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toMFD1Heart", /*��Ϣ����*/
						0X0C2401,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010024,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toMFD1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2402,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010024,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toMFD1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2400,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010024,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU1toMFD2Heart", /*��Ϣ����*/
						0X0C2501,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010025,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toMFD2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2502,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010025,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toMFD2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2500,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010025,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toUFCDHeart", /*��Ϣ����*/
						0X0C2601,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010026,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toUFCDTopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2602,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010026,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toUFCDData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2600,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010026,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toDEUHeart", /*��Ϣ����*/
						0X0C2701,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010027,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toDEUTopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2702,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010027,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toDEUData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2700,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010027,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toVIUHeart", /*��Ϣ����*/
						0X0C2801,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010028,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toVIUTopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2802,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010028,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toVIUData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2800,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010028,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					/*615A*/
					{
						"DPU1 protocol",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0179,             /* FCUINT msg_id       ������ϢID */
					  4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010001,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
				},
				/*����*/
				{

					{
						"MIU1toMIU1Heart",      /* ��Ϣ���� */
						0X010101,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},

					{
						"MIU1toMIU1Topic",      /* ��Ϣ���� */
						0X010102,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},

					{
						"MIU1toMIU1Data",      /* ��Ϣ���� */
						0X010100,        /* ��ϢID */
						66536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU1toMIU1Heart",      /* ��Ϣ���� */
						0X0C0101,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},

					{
						"DPU1toMIU1Topic",      /* ��Ϣ���� */
						0X0C0102,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x01000C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},

					{
						"DPU1toMIU1Data",      /* ��Ϣ���� */
						0X0C0100,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},

					{
						"DPU2toMIU1Heart",      /* ��Ϣ���� */
						0X0D0101,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000D,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},

					{
						"DPU2toMIU1Topic",      /* ��Ϣ���� */
						0X0D0102,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x01000D,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},

					{
						"DPU2toMIU1Data",      /* ��Ϣ���� */
						0X0D0100,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000D,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										
					{
						"DPU3toDPU1Heart",      /* ��Ϣ���� */
						0X0E0C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000E,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU3toDPU1Topic",      /* ��Ϣ���� */
						0X0E0C02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x01000E,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU3toDPU1Data",      /* ��Ϣ���� */
						0X0E0C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000E,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU4toDPU1Heart",      /* ��Ϣ���� */
						0X0F0C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000F,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU4toDPU1Topic",      /* ��Ϣ���� */
						0X0F0C02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x01000F,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU4toDPU1Data",      /* ��Ϣ���� */
						0X0F0C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000F,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU5toDPU1Heart",      /* ��Ϣ���� */
						0X100C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010010,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU5toDPU1Topic",      /* ��Ϣ���� */
						0X100C02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x010010,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU5toDPU1Data",      /* ��Ϣ���� */
						0X100C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010010,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU6toDPU1Heart",      /* ��Ϣ���� */
						0X110C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010011,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU6toDPU1Topic",      /* ��Ϣ���� */
						0X110C02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x010011,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU6toDPU1Data",      /* ��Ϣ���� */
						0X110C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010011,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU7toDPU1Heart",      /* ��Ϣ���� */
						0X000C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010000,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU7toDPU1Topic",      /* ��Ϣ���� */
						0X000C02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x010000,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU7toDPU1Data",      /* ��Ϣ���� */
						0X000C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010000,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"MIU1toDPU1Heart",      /* ��Ϣ���� */
						0X010C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MIU1toDPU1Topic",      /* ��Ϣ���� */
						0X010C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MIU1toDPU1Data",      /* ��Ϣ���� */
						0X010C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"MIU2toDPU1Heart",      /* ��Ϣ���� */
						0X020C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010002,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MIU2toDPU1Topic",      /* ��Ϣ���� */
						0X020C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010002,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MIU2toDPU1Data",      /* ��Ϣ���� */
						0X020C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010002,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"IIPtoDPU1Heart",      /* ��Ϣ���� */
						0X030C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010003,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"IIPtoDPU1Topic",      /* ��Ϣ���� */
						0X030C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010003,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"IIPtoDPU1Data",      /* ��Ϣ���� */
						0X030C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010003,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"ISP1toDPU1Heart",      /* ��Ϣ���� */
						0X040C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010004,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"ISP1toDPU1Topic",      /* ��Ϣ���� */
						0X040C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010004,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"ISP1toDPU1Data",      /* ��Ϣ���� */
						0X040C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010004,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"ISP2toDPU1Heart",      /* ��Ϣ���� */
						0X050C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010005,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"ISP2toDPU1Topic",      /* ��Ϣ���� */
						0X050C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010005,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"ISP2toDPU1Data",      /* ��Ϣ���� */
						0X050C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010005,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"OEDEtoDPU1Heart",      /* ��Ϣ���� */
						0X180C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010018,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"OEDEtoDPU1Topic",      /* ��Ϣ���� */
						0X180C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010018,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"OEDEtoDPU1Data",      /* ��Ϣ���� */
						0X180C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010018,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"INE1toDPU1Heart",      /* ��Ϣ���� */
						0X190C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010019,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"INE1toDPU1Topic",      /* ��Ϣ���� */
						0X190C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010019,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"INE1toDPU1Data",      /* ��Ϣ���� */
						0X190C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010019,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"INE2toDPU1Heart",      /* ��Ϣ���� */
						0X1A0C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001A,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"INE2toDPU1Topic",      /* ��Ϣ���� */
						0X1A0C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x01001A,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"INE2toDPU1Data",      /* ��Ϣ���� */
						0X1A0C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001A,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"AIU1toDPU1Heart",      /* ��Ϣ���� */
						0X1B0C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001B,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"AIU1toDPU1Topic",      /* ��Ϣ���� */
						0X1B0C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x01001B,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"AIU1toDPU1Data",      /* ��Ϣ���� */
						0X1B0C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001B,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"AIU2toDPU1Heart",      /* ��Ϣ���� */
						0X1C0C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"AIU2toDPU1Topic",      /* ��Ϣ���� */
						0X1C0C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x01001C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"AIU2toDPU1Data",      /* ��Ϣ���� */
						0X1C0C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"MFD1toDPU1Heart",      /* ��Ϣ���� */
						0X240C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010024,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MFD1toDPU1Topic",      /* ��Ϣ���� */
						0X240C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010024,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MFD1toDPU1Data",      /* ��Ϣ���� */
						0X240C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010024,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"MFD2toDPU1Heart",      /* ��Ϣ���� */
						0X250C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010025,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MFD2toDPU1Topic",      /* ��Ϣ���� */
						0X250C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010025,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MFD2toDPU1Data",      /* ��Ϣ���� */
						0X250C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010025,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"UFCDtoDPU1Heart",      /* ��Ϣ���� */
						0X260C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010026,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"UFCDtoDPU1Topic",      /* ��Ϣ���� */
						0X260C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010026,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"UFCDtoDPU1Data",      /* ��Ϣ���� */
						0X260C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010026,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DEUtoDPU1Heart",      /* ��Ϣ���� */
						0X270C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010027,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DEUtoDPU1Topic",      /* ��Ϣ���� */
						0X270C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010027,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DEUtoDPU1Data",      /* ��Ϣ���� */
						0X270C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010027,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"VIUtoDPU1Heart",      /* ��Ϣ���� */
						0X280C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010028,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"VIUtoDPU1Topic",      /* ��Ϣ���� */
						0X280C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010028,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"VIUtoDPU1Data",      /* ��Ϣ���� */
						0X280C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010028,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										/*615A*/
										{
						"DPU1 image",      /* ��Ϣ���� */
						0X010C78,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},

				},
			},
			/* ---------------------------���ÿ�1-1------------------------- */
			{
				/*�˿�����*/
				{
					"DPU1",               /* ���ض˿ڵ����� */
					0x01000C,           /* ����port_id */
					1,                  /* �˿����� 1-ASM */
					8,                  /* ���� */
					2,                  /* �˿ڵ���������ɫ 1-nc 2-bnc 3-Ϊ����Զ���ն�*/
					2,                  /* ʱ��ͬ����ɫ2-����ʱ�ӿͻ���*/
					1000,               /* WDT��ʱ����,���г�ʱ*/
					50,                /* WDT��������,ι������*/
					500,               /* ʱ��ͬ������ */
					0,                  /* �����ݿ鷢����Ϣ������ 1��615AЭ��*/
					0,                  /* �����ݿ������Ϣ������ 1��615AЭ��*/
					67,                  /* ���ݿ鷢����Ϣ������ */
					67,                  /* ���ݿ������Ϣ������ 1��615Aӳ���ļ�*/
				},
				/*����*/
				{
					{
						"DPU1toDPU1Heart", /*��Ϣ����*/
						0X0C0C01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000C,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					{
						"DPU1toDPU1Topic", /*��Ϣ����*/
						0X0C0C02,	 /*��ϢID*/
						2,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000C,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toDPU1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0C00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000C,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toDPU2Heart", /*��Ϣ����*/
						0X0C0D01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000D,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					{
						"DPU1toDPU2Topic", /*��Ϣ����*/
						0X0C0D02,	 /*��ϢID*/
						2,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000D,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toDPU2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0D00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000D,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toDPU3Heart", /*��Ϣ����*/
						0X0C0E01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000E,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toDPU3Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0E02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000E,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toDPU3Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0E00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000E,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toDPU4Heart", /*��Ϣ����*/
						0X0C0F01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000F,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toDPU4Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0F02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000F,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toDPU4Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0F00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000F,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toDPU5Heart", /*��Ϣ����*/
						0X0C1001,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010010,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toDPU5Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1002,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010010,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toDPU5Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1000,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010010,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toDPU6Heart", /*��Ϣ����*/
						0X0C1101,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010011,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toDPU6Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1102,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010011,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toDPU6Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1100,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010011,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toDPU7Heart", /*��Ϣ����*/
						0X0C0001,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010000,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toDPU7Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0002,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010000,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toDPU7Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0000,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010000,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toMIU1Heart", /*��Ϣ����*/
						0X0C0101,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010001,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toMIU1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0102,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010001,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toMIU1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0100,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010001,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toMIU2Heart", /*��Ϣ����*/
						0x0C0201,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010002,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toMIU2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0x0C0202,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010002,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toMIU2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0x0C0200,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010002,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toIIPHeart", /*��Ϣ����*/
						0x0C0301,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010003,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toIIPTopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0302,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010003,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toIIPData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0300,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010003,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toISP1Heart", /*��Ϣ����*/
						0X0C0401,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010004,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toISP1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0402,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010004,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toISP1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0400,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010004,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toISP2Heart", /*��Ϣ����*/
						0X0C0501,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010005,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toISP2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0502,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010005,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toISP2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0500,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010005,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toOEDEHeart", /*��Ϣ����*/
						0X0C1801,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010018,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toOEDETopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1802,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010018,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toOEDEData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1800,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010018,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU1toINE1Heart", /*��Ϣ����*/
						0X0C1901,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010019,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toINE1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1902,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010019,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toINE1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1900,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010019,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toINE2Heart", /*��Ϣ����*/
						0X0C1A01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x01001A,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toINE2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1A02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001A,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toINE2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1A00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001A,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toAIU1Heart", /*��Ϣ����*/
						0X0C1B01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x01001B,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toAIU1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1B02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001B,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toAIU1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1B00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001B,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU1toAIU2Heart", /*��Ϣ����*/
						0X0C1C01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x01001C,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toAIU2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1C02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001C,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toAIU2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C1C00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001C,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toMFD1Heart", /*��Ϣ����*/
						0X0C2401,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010024,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toMFD1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2402,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010024,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toMFD1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2400,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010024,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU1toMFD2Heart", /*��Ϣ����*/
						0X0C2501,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010025,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toMFD2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2502,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010025,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toMFD2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2500,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010025,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toUFCDHeart", /*��Ϣ����*/
						0X0C2601,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010026,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toUFCDTopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2602,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010026,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toUFCDData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2600,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010026,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toDEUHeart", /*��Ϣ����*/
						0X0C2701,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010027,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toDEUTopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2702,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010027,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toDEUData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2700,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010027,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU1toVIUHeart", /*��Ϣ����*/
						0X0C2801,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000C,/*��Ϣ��ԴASM�˿�port_id*/
						0x010028,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU1toVIUTopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2802,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010028,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU1toVIUData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C2800,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010028,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					/*615A*/
					{
						"DPU1 protocol",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0C0179,             /* FCUINT msg_id       ������ϢID */
					  4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000C,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010001,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
				},
				/*����*/
				{

					{
						"DPU1toDPU1Heart",      /* ��Ϣ���� */
						0X0C0C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},

					{
						"DPU1toDPU1Topic",      /* ��Ϣ���� */
						0X0C0C02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x01000C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},

					{
						"DPU1toDPU1Data",      /* ��Ϣ���� */
						0X0C0C00,        /* ��ϢID */
						66536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU2toDPU1Heart",      /* ��Ϣ���� */
						0X0D0C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000D,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},

					{
						"DPU2toDPU1Topic",      /* ��Ϣ���� */
						0X0D0C02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x01000D,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},

					{
						"DPU2toDPU1Data",      /* ��Ϣ���� */
						0X0D0C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000D,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU3toDPU1Heart",      /* ��Ϣ���� */
						0X0E0C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000E,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU3toDPU1Topic",      /* ��Ϣ���� */
						0X0E0C02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x01000E,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU3toDPU1Data",      /* ��Ϣ���� */
						0X0E0C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000E,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU4toDPU1Heart",      /* ��Ϣ���� */
						0X0F0C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000F,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU4toDPU1Topic",      /* ��Ϣ���� */
						0X0F0C02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x01000F,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU4toDPU1Data",      /* ��Ϣ���� */
						0X0F0C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000F,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU5toDPU1Heart",      /* ��Ϣ���� */
						0X100C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010010,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU5toDPU1Topic",      /* ��Ϣ���� */
						0X100C02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x010010,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU5toDPU1Data",      /* ��Ϣ���� */
						0X100C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010010,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU6toDPU1Heart",      /* ��Ϣ���� */
						0X110C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010011,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU6toDPU1Topic",      /* ��Ϣ���� */
						0X110C02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x010011,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU6toDPU1Data",      /* ��Ϣ���� */
						0X110C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010011,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU7toDPU1Heart",      /* ��Ϣ���� */
						0X000C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010000,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU7toDPU1Topic",      /* ��Ϣ���� */
						0X000C02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x010000,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU7toDPU1Data",      /* ��Ϣ���� */
						0X000C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010000,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"MIU1toDPU1Heart",      /* ��Ϣ���� */
						0X010C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MIU1toDPU1Topic",      /* ��Ϣ���� */
						0X010C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MIU1toDPU1Data",      /* ��Ϣ���� */
						0X010C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"MIU2toDPU1Heart",      /* ��Ϣ���� */
						0X020C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010002,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MIU2toDPU1Topic",      /* ��Ϣ���� */
						0X020C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010002,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MIU2toDPU1Data",      /* ��Ϣ���� */
						0X020C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010002,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"IIPtoDPU1Heart",      /* ��Ϣ���� */
						0X030C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010003,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"IIPtoDPU1Topic",      /* ��Ϣ���� */
						0X030C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010003,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"IIPtoDPU1Data",      /* ��Ϣ���� */
						0X030C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010003,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"ISP1toDPU1Heart",      /* ��Ϣ���� */
						0X040C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010004,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"ISP1toDPU1Topic",      /* ��Ϣ���� */
						0X040C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010004,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"ISP1toDPU1Data",      /* ��Ϣ���� */
						0X040C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010004,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"ISP2toDPU1Heart",      /* ��Ϣ���� */
						0X050C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010005,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"ISP2toDPU1Topic",      /* ��Ϣ���� */
						0X050C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010005,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"ISP2toDPU1Data",      /* ��Ϣ���� */
						0X050C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010005,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"OEDEtoDPU1Heart",      /* ��Ϣ���� */
						0X180C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010018,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"OEDEtoDPU1Topic",      /* ��Ϣ���� */
						0X180C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010018,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"OEDEtoDPU1Data",      /* ��Ϣ���� */
						0X180C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010018,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"INE1toDPU1Heart",      /* ��Ϣ���� */
						0X190C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010019,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"INE1toDPU1Topic",      /* ��Ϣ���� */
						0X190C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010019,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"INE1toDPU1Data",      /* ��Ϣ���� */
						0X190C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010019,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"INE2toDPU1Heart",      /* ��Ϣ���� */
						0X1A0C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001A,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"INE2toDPU1Topic",      /* ��Ϣ���� */
						0X1A0C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x01001A,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"INE2toDPU1Data",      /* ��Ϣ���� */
						0X1A0C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001A,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"AIU1toDPU1Heart",      /* ��Ϣ���� */
						0X1B0C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001B,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"AIU1toDPU1Topic",      /* ��Ϣ���� */
						0X1B0C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x01001B,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"AIU1toDPU1Data",      /* ��Ϣ���� */
						0X1B0C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001B,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"AIU2toDPU1Heart",      /* ��Ϣ���� */
						0X1C0C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"AIU2toDPU1Topic",      /* ��Ϣ���� */
						0X1C0C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x01001C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"AIU2toDPU1Data",      /* ��Ϣ���� */
						0X1C0C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"MFD1toDPU1Heart",      /* ��Ϣ���� */
						0X240C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010024,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MFD1toDPU1Topic",      /* ��Ϣ���� */
						0X240C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010024,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MFD1toDPU1Data",      /* ��Ϣ���� */
						0X240C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010024,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"MFD2toDPU1Heart",      /* ��Ϣ���� */
						0X250C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010025,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MFD2toDPU1Topic",      /* ��Ϣ���� */
						0X250C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010025,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MFD2toDPU1Data",      /* ��Ϣ���� */
						0X250C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010025,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"UFCDtoDPU1Heart",      /* ��Ϣ���� */
						0X260C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010026,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"UFCDtoDPU1Topic",      /* ��Ϣ���� */
						0X260C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010026,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"UFCDtoDPU1Data",      /* ��Ϣ���� */
						0X260C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010026,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DEUtoDPU1Heart",      /* ��Ϣ���� */
						0X270C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010027,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DEUtoDPU1Topic",      /* ��Ϣ���� */
						0X270C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010027,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DEUtoDPU1Data",      /* ��Ϣ���� */
						0X270C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010027,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"VIUtoDPU1Heart",      /* ��Ϣ���� */
						0X280C01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010028,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"VIUtoDPU1Topic",      /* ��Ϣ���� */
						0X280C02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010028,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"VIUtoDPU1Data",      /* ��Ϣ���� */
						0X280C00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010028,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										/*615A*/
										{
						"DPU1 image",      /* ��Ϣ���� */
						0X010C78,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},

					},
			},

			/* ---------------------------���ÿ�1-2------------------------- */
			{
				/*�˿�����*/
				{
					"DPU2",               /* ���ض˿ڵ����� */
					0x01000D,           /* ����port_id */
					1,                  /* �˿����� 1-ASM */
					8,                  /* ���� */
					2,                  /* �˿ڵ���������ɫ3-Ϊ����Զ���ն�*/
					2,                  /* ʱ��ͬ����ɫ2-����ʱ�ӿͻ���*/
					1500,               /* WDT��ʱ����*/
					50,               	/* WDT��������*/
					500,               	/* ʱ��ͬ������ */
					0,                  /* �����ݿ鷢����Ϣ������ 1��615AЭ��*/
					0,                  /* �����ݿ������Ϣ������ 1��615AЭ��*/
					67,                  /* ���ݿ鷢����Ϣ������ */
					67,                  /* ���ݿ������Ϣ������ 1��615Aӳ���ļ�*/
				},
				/*����*/
				{
					{
						"DPU2toDPU1Heart", /*��Ϣ����*/
						0X0D0C01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000C,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},

					 {
						"DPU2toDPU1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0C02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000C,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toDPU1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0C00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000C,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU2toDPU2Heart", /*��Ϣ����*/
						0X0D0D01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000D,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toDPU2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0D02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000D,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toDPU2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0D00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000D,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU2toDPU3Heart", /*��Ϣ����*/
						0X0D0E01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000E,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toDPU3Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0E02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000E,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toDPU3Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0E00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000E,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU2toDPU4Heart", /*��Ϣ����*/
						0X0D0F01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000F,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toDPU4Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0F02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000F,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toDPU4Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0F00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000F,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU2toDPU5Heart", /*��Ϣ����*/
						0X0D1001,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x010010,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toDPU5Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D1002,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010010,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toDPU5Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D1000,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010010,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU2toDPU6Heart", /*��Ϣ����*/
						0X0D1101,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x010011,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toDPU6Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D1102,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010011,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toDPU6Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D1100,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010011,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU2toDPU7Heart", /*��Ϣ����*/
						0X0D0001,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x010000,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toDPU7Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0002,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010000,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toDPU7Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0000,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010000,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU2toMIU1Heart", /*��Ϣ����*/
						0X0D0101,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x010001,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toMIU1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0102,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010001,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toMIU1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0100,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010001,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU2toMIU2Heart", /*��Ϣ����*/
						0x0D0201,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x010002,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toMIU2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0x0D0202,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010002,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toMIU2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0x0D0200,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010002,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU2toIIPHeart", /*��Ϣ����*/
						0x0D0301,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x010003,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toIIPTopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0302,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010003,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toIIPData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0300,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010003,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU2toISP1Heart", /*��Ϣ����*/
						0X0D0401,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x010004,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toISP1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0402,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010004,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toISP1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0400,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010004,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU2toISP2Heart", /*��Ϣ����*/
						0X0D0501,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x010005,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toISP2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0502,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010005,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toISP2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0500,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010005,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU2toOEDEHeart", /*��Ϣ����*/
						0X0D1801,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x010018,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toOEDETopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D1802,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010018,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toOEDEData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D1800,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010018,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU2toINE1Heart", /*��Ϣ����*/
						0X0D1901,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x010019,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toINE1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D1902,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010019,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toINE1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D1900,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010019,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU2toINE2Heart", /*��Ϣ����*/
						0X0D1A01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x01001A,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toINE2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D1A02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001A,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toINE2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D1A00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001A,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU2toAIU1Heart", /*��Ϣ����*/
						0X0D1B01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x01001B,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toAIU1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D1B02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001B,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toAIU1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D1B00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001B,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU2toAIU2Heart", /*��Ϣ����*/
						0X0D1C01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x01001C,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toAIU2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D1C02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001C,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toAIU2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D1C00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001C,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU2toMFD1Heart", /*��Ϣ����*/
						0X0D2401,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x010024,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toMFD1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D2402,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010024,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toMFD1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D2400,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010024,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU2toMFD2Heart", /*��Ϣ����*/
						0X0D2501,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x010025,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toMFD2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D2502,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010025,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toMFD2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D2500,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010025,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU2toUFCDHeart", /*��Ϣ����*/
						0X0D2601,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x010026,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toUFCDTopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D2602,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010026,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toUFCDData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D2600,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010026,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU2toDEUHeart", /*��Ϣ����*/
						0X0D2701,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x010027,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toDEUTopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D2702,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010027,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toDEUData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D2700,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010027,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU2toVIUHeart", /*��Ϣ����*/
						0X0D2801,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000D,/*��Ϣ��ԴASM�˿�port_id*/
						0x010028,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU2toVIUTopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D2802,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010028,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU2toVIUData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D2800,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010028,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
												/*615A*/
					{
						"DPU2 protocol",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0D0179,             /* FCUINT msg_id       ������ϢID */
					  4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000D,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010001,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
				},
				/*����*/
				{

					{
						"DPU1toDPU2Heart",      /* ��Ϣ���� */
						0X0C0D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU1toDPU2Topic",      /* ��Ϣ���� */
						0X0C0D02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x01000C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU1toDPU2Data",      /* ��Ϣ���� */
						0X0C0D00,        /* ��ϢID */
						66536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU2toDPU2Heart",      /* ��Ϣ���� */
						0X0D0D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000D,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU2toDPU2Topic",      /* ��Ϣ���� */
						0X0D0D02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x01000D,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU2toDPU2Data",      /* ��Ϣ���� */
						0X0D0D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000D,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU3toDPU2Heart",      /* ��Ϣ���� */
						0X0E0D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000E,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU3toDPU2Topic",      /* ��Ϣ���� */
						0X0E0D02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x01000E,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU3toDPU2Data",      /* ��Ϣ���� */
						0X0E0D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000E,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU4toDPU2Heart",      /* ��Ϣ���� */
						0X0F0D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000F,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU4toDPU2Topic",      /* ��Ϣ���� */
						0X0F0D02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x01000F,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU4toDPU2Data",      /* ��Ϣ���� */
						0X0F0D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000F,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU5toDPU2Heart",      /* ��Ϣ���� */
						0X100D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010010,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU5toDPU2Topic",      /* ��Ϣ���� */
						0X100D02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x010010,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU5toDPU2Data",      /* ��Ϣ���� */
						0X100D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010010,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU6toDPU2Heart",      /* ��Ϣ���� */
						0X110D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010011,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU6toDPU2Topic",      /* ��Ϣ���� */
						0X110D02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x010011,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU6toDPU2Data",      /* ��Ϣ���� */
						0X110D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010011,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU7toDPU2Heart",      /* ��Ϣ���� */
						0X000D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010000,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU7toDPU2Topic",      /* ��Ϣ���� */
						0X000D02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x010000,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU7toDPU2Data",      /* ��Ϣ���� */
						0X000D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010000,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"MIU1toDPU2Heart",      /* ��Ϣ���� */
						0X010D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MIU1toDPU2Topic",      /* ��Ϣ���� */
						0X010D02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MIU1toDPU2Data",      /* ��Ϣ���� */
						0X010D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"MIU2toDPU2Heart",      /* ��Ϣ���� */
						0X020D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010002,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MIU2toDPU2Topic",      /* ��Ϣ���� */
						0X020D02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010002,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MIU2toDPU2Data",      /* ��Ϣ���� */
						0X020D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010002,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"IIPtoDPU2Heart",      /* ��Ϣ���� */
						0X030D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010003,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"IIPtoDPU2Topic",      /* ��Ϣ���� */
						0X030D02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010003,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"IIPtoDPU2Data",      /* ��Ϣ���� */
						0X030D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010003,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"ISP1toDPU2Heart",      /* ��Ϣ���� */
						0X040D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010004,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"ISP1toDPU2Topic",      /* ��Ϣ���� */
						0X040D02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010004,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"ISP1toDPU2Data",      /* ��Ϣ���� */
						0X040D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010004,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"ISP2toDPU2Heart",      /* ��Ϣ���� */
						0X050D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010005,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"ISP2toDPU2Topic",      /* ��Ϣ���� */
						0X050D02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010005,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"ISP2toDPU2Data",      /* ��Ϣ���� */
						0X050D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010005,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"OEDEtoDPU2Heart",      /* ��Ϣ���� */
						0X180D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010018,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"OEDEtoDPU2Topic",      /* ��Ϣ���� */
						0X180D02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010018,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"OEDEtoDPU2Data",      /* ��Ϣ���� */
						0X180D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010018,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"INE1toDPU2Heart",      /* ��Ϣ���� */
						0X190D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010019,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"INE1toDPU2Topic",      /* ��Ϣ���� */
						0X190D02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010019,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"INE1toDPU2Data",      /* ��Ϣ���� */
						0X190D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010019,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"INE2toDPU2Heart",      /* ��Ϣ���� */
						0X1A0D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001A,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"INE2toDPU2Topic",      /* ��Ϣ���� */
						0X1A0D02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x01001A,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"INE2toDPU2Data",      /* ��Ϣ���� */
						0X1A0D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001A,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"AIU1toDPU2Heart",      /* ��Ϣ���� */
						0X1B0D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001B,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"AIU1toDPU2Topic",      /* ��Ϣ���� */
						0X1B0D02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x01001B,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"AIU1toDPU2Data",      /* ��Ϣ���� */
						0X1B0D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001B,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"AIU2toDPU2Heart",      /* ��Ϣ���� */
						0X1C0D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"AIU2toDPU2Topic",      /* ��Ϣ���� */
						0X1C0D02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x01001C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"AIU2toDPU2Data",      /* ��Ϣ���� */
						0X1C0D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},

										{
						"MFD1toDPU2Heart",      /* ��Ϣ���� */
						0X240D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010024,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MFD1toDPU2Topic",      /* ��Ϣ���� */
						0X240D02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010024,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MFD1toDPU2Data",      /* ��Ϣ���� */
						0X240D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010024,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"MFD2toDPU2Heart",      /* ��Ϣ���� */
						0X250D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010025,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MFD2toDPU2Topic",      /* ��Ϣ���� */
						0X250D02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010025,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MFD2toDPU2Data",      /* ��Ϣ���� */
						0X250D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010025,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"UFCDtoDPU2Heart",      /* ��Ϣ���� */
						0X260D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010026,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"UFCDtoDPU2Topic",      /* ��Ϣ���� */
						0X260D02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010026,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"UFCDtoDPU2Data",      /* ��Ϣ���� */
						0X260D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010026,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DEUtoDPU2Heart",      /* ��Ϣ���� */
						0X270D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010027,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DEUtoDPU2Topic",      /* ��Ϣ���� */
						0X270D02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010027,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DEUtoDPU2Data",      /* ��Ϣ���� */
						0X270D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010027,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"VIUtoDPU2Heart",      /* ��Ϣ���� */
						0X280D01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010028,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"VIUtoDPU2Topic",      /* ��Ϣ���� */
						0X280D02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010028,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"VIUtoDPU2Data",      /* ��Ϣ���� */
						0X280D00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010028,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										/*615A*/
					{
						"DPU2 image",      /* ��Ϣ���� */
						0X010D78,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					},
			},
			/* ---------------------------���ÿ�1-3------------------------- */
			{
				/*�˿�����*/
				{
					"DPU3",               /* ���ض˿ڵ����� */
					0x01000E,           /* ����port_id */
					1,                  /* �˿����� 1-ASM */
					8,                  /* ���� */
					3,                  /* �˿ڵ���������ɫ3-Ϊ����Զ���ն�*/
					2,                  /* ʱ��ͬ����ɫ2-����ʱ�ӿͻ���*/
					1000,               /* WDT��ʱ����*/
					400,                /* WDT��������*/
					500,               /* ʱ��ͬ������ */
					0,                  /* �����ݿ鷢����Ϣ������ 1��615AЭ��*/
					0,                  /* �����ݿ������Ϣ������ 1��615AЭ��*/
					67,                  /* ���ݿ鷢����Ϣ������ */
					67,                  /* ���ݿ������Ϣ������ 1��615Aӳ���ļ�*/
				},
				/*����*/
				{
					{
						"DPU3toDPU1Heart", /*��Ϣ����*/
						0X0E0C01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000C,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toDPU1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0C02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000C,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toDPU1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0C00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000C,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU3toDPU2Heart", /*��Ϣ����*/
						0X0E0D01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000D,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toDPU2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0D02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000D,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toDPU2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0D00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000D,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU3toDPU3Heart", /*��Ϣ����*/
						0X0E0E01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000E,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toDPU3Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0E02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000E,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toDPU3Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0E00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000E,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU3toDPU4Heart", /*��Ϣ����*/
						0X0E0F01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x01000F,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toDPU4Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0F02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000F,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toDPU4Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0F00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01000F,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU3toDPU5Heart", /*��Ϣ����*/
						0X0E1001,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x010010,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toDPU5Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E1002,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010010,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toDPU5Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E1000,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010010,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU3toDPU6Heart", /*��Ϣ����*/
						0X0E1101,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x010011,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toDPU6Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E1102,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010011,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toDPU6Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E1100,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010011,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU3toDPU7Heart", /*��Ϣ����*/
						0X0E0001,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x010000,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toDPU7Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0002,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010000,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toDPU7Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0000,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010000,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU3toMIU1Heart", /*��Ϣ����*/
						0X0E0101,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x010001,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toMIU1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0102,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010001,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toMIU1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0100,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010001,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU3toMIU2Heart", /*��Ϣ����*/
						0x0E0201,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x010002,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toMIU2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0x0E0202,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010002,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toMIU2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0x0E0200,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010002,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU3toIIPHeart", /*��Ϣ����*/
						0x0E0301,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x010003,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toIIPTopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0302,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010003,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toIIPData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0300,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010003,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU3toISP1Heart", /*��Ϣ����*/
						0X0E0401,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x010004,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toISP1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0402,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010004,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toISP1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0400,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010004,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU3toISP2Heart", /*��Ϣ����*/
						0X0E0501,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x010005,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toISP2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0502,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010005,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toISP2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0500,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010005,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU3toOEDEHeart", /*��Ϣ����*/
						0X0E1801,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x010018,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toOEDETopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E1802,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010018,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toOEDEData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E1800,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010018,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU3toINE1Heart", /*��Ϣ����*/
						0X0E1901,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x010019,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toINE1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E1902,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010019,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toINE1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E1900,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010019,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU3toINE2Heart", /*��Ϣ����*/
						0X0E1A01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x01001A,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toINE2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E1A02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001A,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toINE2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E1A00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001A,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU3toAIU1Heart", /*��Ϣ����*/
						0X0E1B01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x01001B,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toAIU1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E1B02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001B,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toAIU1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E1B00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001B,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU3toAIU2Heart", /*��Ϣ����*/
						0X0E1C01,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x01001C,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toAIU2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E1C02,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001C,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toAIU2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E1C00,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x01001C,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					{
						"DPU3toMFD1Heart", /*��Ϣ����*/
						0X0E2401,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x010024,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toMFD1Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E2402,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010024,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toMFD1Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E2400,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010024,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU3toMFD2Heart", /*��Ϣ����*/
						0X0E2501,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x010025,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toMFD2Topic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E2502,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010025,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toMFD2Data",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E2500,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010025,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU3toUFCDHeart", /*��Ϣ����*/
						0X0E2601,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x010026,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toUFCDTopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E2602,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010026,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toUFCDData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E2600,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010026,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU3toDEUHeart", /*��Ϣ����*/
						0X0E2701,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x010027,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toDEUTopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E2702,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010027,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toDEUData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E2700,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010027,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
										{
						"DPU3toVIUHeart", /*��Ϣ����*/
						0X0E2801,	 /*��ϢID*/
						4,       /*��Ϣ����*/
						1,       /*��Ϣ���ȼ�*/
						0x01000E,/*��Ϣ��ԴASM�˿�port_id*/
						0x010028,/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					 {
						"DPU3toVIUTopic",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E2802,             /* FCUINT msg_id       ������ϢID */
						2,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010028,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
					 {
						"DPU3toVIUData",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E2800,             /* FCUINT msg_id       ������ϢID */
						4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010028,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
												/*615A*/
					{
						"DPU3 protocol",  /* "send asmid=1",*/   /* ��Ϣ������ */
						0X0E0179,             /* FCUINT msg_id       ������ϢID */
					  4,                 /* FCINT  msg_type     ��Ϣ���� */
						1,               /* FCINT  msg_priority ��Ϣ���ȼ� */
						0x01000E,      /* FCINT  msg_src_id   ��Ϣ��ԴASM�˿�port_id */
						0x010001,    /* FCINT  msg_dst_id   ��Ϣ��Ŀ��ASM�˿�port_id */
					},
				},
				/*����*/
				{

					{
						"DPU1toDPU3Heart",      /* ��Ϣ���� */
						0X0C0E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU1toDPU3Topic",      /* ��Ϣ���� */
						0X0C0E02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x01000C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU1toDPU3Data",      /* ��Ϣ���� */
						0X0C0E00,        /* ��ϢID */
						66536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU2toDPU3Heart",      /* ��Ϣ���� */
						0X0D0E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000D,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU2toDPU3Topic",      /* ��Ϣ���� */
						0X0D0E02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x01000D,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU2toDPU3Data",      /* ��Ϣ���� */
						0X0D0E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000D,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU3toDPU3Heart",      /* ��Ϣ���� */
						0X0E0E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000E,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU3toDPU3Topic",      /* ��Ϣ���� */
						0X0E0E02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x01000E,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU3toDPU3Data",      /* ��Ϣ���� */
						0X0E0E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000E,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU4toDPU3Heart",      /* ��Ϣ���� */
						0X0F0E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000F,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU4toDPU3Topic",      /* ��Ϣ���� */
						0X0F0E02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x01000F,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU4toDPU3Data",      /* ��Ϣ���� */
						0X0F0E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01000F,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU5toDPU3Heart",      /* ��Ϣ���� */
						0X100E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010010,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU5toDPU3Topic",      /* ��Ϣ���� */
						0X100E02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x010010,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU5toDPU3Data",      /* ��Ϣ���� */
						0X100E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010010,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU6toDPU3Heart",      /* ��Ϣ���� */
						0X110E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010011,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU6toDPU3Topic",      /* ��Ϣ���� */
						0X110E02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x010011,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU6toDPU3Data",      /* ��Ϣ���� */
						0X110E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010011,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DPU7toDPU3Heart",      /* ��Ϣ���� */
						0X000E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010000,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU7toDPU3Topic",      /* ��Ϣ���� */
						0X000E02,        /* ��ϢID */
						2048, 		  /* */
						2,            /* ��Ϣ����  �¼� */
						0x010000,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DPU7toDPU3Data",      /* ��Ϣ���� */
						0X000E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010000,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"MIU1toDPU3Heart",      /* ��Ϣ���� */
						0X010E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MIU1toDPU3Topic",      /* ��Ϣ���� */
						0X010E02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MIU1toDPU3Data",      /* ��Ϣ���� */
						0X010E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"MIU2toDPU3Heart",      /* ��Ϣ���� */
						0X020E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010002,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MIU2toDPU3Topic",      /* ��Ϣ���� */
						0X020E02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010002,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MIU2toDPU3Data",      /* ��Ϣ���� */
						0X020E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010002,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"IIPtoDPU3Heart",      /* ��Ϣ���� */
						0X030E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010003,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"IIPtoDPU3Topic",      /* ��Ϣ���� */
						0X030E02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010003,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"IIPtoDPU3Data",      /* ��Ϣ���� */
						0X030E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010003,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"ISP1toDPU3Heart",      /* ��Ϣ���� */
						0X040E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010004,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"ISP1toDPU3Topic",      /* ��Ϣ���� */
						0X040E02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010004,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"ISP1toDPU3Data",      /* ��Ϣ���� */
						0X040E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010004,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"ISP2toDPU3Heart",      /* ��Ϣ���� */
						0X050E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010005,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"ISP2toDPU3Topic",      /* ��Ϣ���� */
						0X050E02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010005,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"ISP2toDPU3Data",      /* ��Ϣ���� */
						0X050E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010005,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"OEDEtoDPU3Heart",      /* ��Ϣ���� */
						0X180E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010018,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"OEDEtoDPU3Topic",      /* ��Ϣ���� */
						0X180E02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010018,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"OEDEtoDPU3Data",      /* ��Ϣ���� */
						0X180E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010018,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"INE1toDPU3Heart",      /* ��Ϣ���� */
						0X190E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010019,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"INE1toDPU3Topic",      /* ��Ϣ���� */
						0X190E02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010019,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"INE1toDPU3Data",      /* ��Ϣ���� */
						0X190E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010019,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"INE2toDPU3Heart",      /* ��Ϣ���� */
						0X1A0E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001A,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"INE2toDPU3Topic",      /* ��Ϣ���� */
						0X1A0E02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x01001A,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"INE2toDPU3Data",      /* ��Ϣ���� */
						0X1A0E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001A,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"AIU1toDPU3Heart",      /* ��Ϣ���� */
						0X1B0E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001B,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"AIU1toDPU3Topic",      /* ��Ϣ���� */
						0X1B0E02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x01001B,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"AIU1toDPU3Data",      /* ��Ϣ���� */
						0X1B0E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001B,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"AIU2toDPU3Heart",      /* ��Ϣ���� */
						0X1C0E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"AIU2toDPU3Topic",      /* ��Ϣ���� */
						0X1C0E02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x01001C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"AIU2toDPU3Data",      /* ��Ϣ���� */
						0X1C0E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x01001C,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"MFD1toDPU3Heart",      /* ��Ϣ���� */
						0X240E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010024,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MFD1toDPU3Topic",      /* ��Ϣ���� */
						0X240E02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010024,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MFD1toDPU3Data",      /* ��Ϣ���� */
						0X240E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010024,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"MFD2toDPU3Heart",      /* ��Ϣ���� */
						0X250E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010025,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MFD2toDPU3Topic",      /* ��Ϣ���� */
						0X250E02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010025,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"MFD2toDPU3Data",      /* ��Ϣ���� */
						0X250E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010025,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"UFCDtoDPU3Heart",      /* ��Ϣ���� */
						0X260E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010026,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"UFCDtoDPU3Topic",      /* ��Ϣ���� */
						0X260E02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010026,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"UFCDtoDPU3Data",      /* ��Ϣ���� */
						0X260E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010026,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"DEUtoDPU3Heart",      /* ��Ϣ���� */
						0X270E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010027,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DEUtoDPU3Topic",      /* ��Ϣ���� */
						0X270E02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010027,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"DEUtoDPU3Data",      /* ��Ϣ���� */
						0X270E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010027,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										{
						"VIUtoDPU3Heart",      /* ��Ϣ���� */
						0X280E01,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010028,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"VIUtoDPU3Topic",      /* ��Ϣ���� */
						0X280E02,        /* ��ϢID */
						2048, 		  /* 65536 sws*/
						2,            /* ��Ϣ����  �¼� */
						0x010028,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
					{
						"VIUtoDPU3Data",      /* ��Ϣ���� */
						0X280E00,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010028,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
										/*615A*/
					{
						"DPU3 image",      /* ��Ϣ���� */
						0X010E78,        /* ��ϢID */
						65536, 		  /* 65536 sws*/
						4,            /* ��Ϣ����  �¼� */
						0x010001,     /* ��Ϣ��ԴASM�˿�port_id */
						0             /* ��Ϣ��ˢ�������־ */
					},
				},
			},
			

		}
	},

};

/*���ñ�*/
BLUE_PRINT gBluePrint2[1] =
{

      {
		/*��[0]����ͼ*/
		  {			
			/*�ڵ�1 ����[3]�����ÿ�*/
			{
				/*�˿�����*/
				{
					"NC",               /* ���ض˿ڵ����� */
					MB_NODE1_FCID,      /* ����port_id */
					1,                  /* �˿����� 1-ASM */
					8,                  /* ���� */
					ROLE_NC,     /* �˿ڵ���������ɫ*/
					CLK_SYNC_ROLE_SERVER_MASTER,/* ʱ��ͬ����ɫ*/
					1500,               /* WDT��ʱ����*/
					500,                /* WDT��������*/
					100,                /* ʱ��ͬ������ */
					0,                  /* �����ݿ鷢����Ϣ������ */
					0,                  /* �����ݿ������Ϣ������ */
					1,                  /* ���ݿ鷢����Ϣ������ */
					1,                  /* ���ݿ������Ϣ������ */
				},
				{
					{
						"NC->NC",			/*��Ϣ����*/
						NC_MSG_ID_TO_NC_1,	/*��ϢID*/
						3,					/*��Ϣ����*/
						0,					/*��Ϣ���ȼ�*/
						MB_NODE1_FCID,		/*��Ϣ��ԴASM�˿�port_id*/
						MB_NODE1_FCID,		/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					{
						"NC->NC",			/*��Ϣ����*/
						NC_MSG_ID_TO_NC_1,	/*��ϢID*/
						3,					/*��Ϣ����*/
						0,					/*��Ϣ���ȼ�*/
						MB_NODE1_FCID,		/*��Ϣ��ԴASM�˿�port_id*/
						MB_NODE1_FCID,		/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
        
				},
				{
					{
						"NC->NC",			/*��Ϣ����*/
						NC_MSG_ID_TO_NC_1,  /*��ϢID*/
						FC_AMS_IU_LENGTH_RECV,				/*��Ϣ����*/
						3,					/*��Ϣ����  �¼�*/
						MB_NODE1_FCID,		/*��Ϣ��ԴASM�˿�port_id */
						0					/*��Ϣ��ˢ�������־*/
					},
					{
						"NC->NC",			/*��Ϣ����*/
						MSG_ID_12,  /*��ϢID*/
						FC_AMS_IU_LENGTH_RECV,				/*��Ϣ����*/
						3,					/*��Ϣ����  �¼�*/
						MB_NODE3_FCID,		/*��Ϣ��ԴASM�˿�port_id */
						0				    /*��Ϣ��ˢ�������־*/
					},	
				},
			},	
			/*�ڵ�2 ����[3]�����ÿ�*/
			{
				/*�˿�����*/
				{
					"NC",               /* ���ض˿ڵ����� */
					MB_NODE2_FCID,      /* ����port_id */
					1,                  /* �˿����� 1-ASM */
					8,                  /* ���� */
					ROLE_NC,     /* �˿ڵ���������ɫ*/
					CLK_SYNC_ROLE_SERVER_MASTER,/* ʱ��ͬ����ɫ*/
					1500,               /* WDT��ʱ����*/
					500,                /* WDT��������*/
					100,                /* ʱ��ͬ������ */
					0,                  /* �����ݿ鷢����Ϣ������ */
					0,                  /* �����ݿ������Ϣ������ */
					1,                  /* ���ݿ鷢����Ϣ������ */
					1,                  /* ���ݿ������Ϣ������ */
				},
				{
					{
						"NC->NC",			/*��Ϣ����*/
				 		MSG_ID_12,	/*��ϢID*/
						3,					/*��Ϣ����*/
						0,					/*��Ϣ���ȼ�*/
						MB_NODE2_FCID,		/*��Ϣ��ԴASM�˿�port_id*/
						MB_NODE2_FCID,		/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					{
						"NC->NC",			/*��Ϣ����*/
						MSG_ID_13,	/*��ϢID*/
						3,					/*��Ϣ����*/
						0,					/*��Ϣ���ȼ�*/
						MB_NODE2_FCID,		/*��Ϣ��ԴASM�˿�port_id*/
						MB_NODE2_FCID,		/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
        
				},
				{
					{
						"NC->NC",			/*��Ϣ����*/
						MSG_ID_12,  /*��ϢID*/
						FC_AMS_IU_LENGTH_RECV,				/*��Ϣ����*/
						3,					/*��Ϣ����  �¼�*/
						MB_NODE2_FCID,		/*��Ϣ��ԴASM�˿�port_id */
						0					/*��Ϣ��ˢ�������־*/
					},
					{
						"NC->NC",			/*��Ϣ����*/
						MSG_ID_13,  /*��ϢID*/
						FC_AMS_IU_LENGTH_RECV,				/*��Ϣ����*/
						3,					/*��Ϣ����  �¼�*/
						MB_NODE2_FCID,		/*��Ϣ��ԴASM�˿�port_id */
						0				    /*��Ϣ��ˢ�������־*/
					},	
				},
			},	
			
			/*�ڵ�3 ����[3]�����ÿ�*/
			{
				/*�˿�����*/
				{
					"NRT",               /* ���ض˿ڵ����� */
					MB_NODE3_FCID,      /* ����port_id */
					1,                  /* �˿����� 1-ASM */
					8,                  /* ���� */
					ROLE_NRT,     /* �˿ڵ���������ɫ*/
					CLK_SYNC_ROLE_CLIENT,/* ʱ��ͬ����ɫ*/

					1500,               /* WDT��ʱ����*/
					500,                /* WDT��������*/
					100,                /* ʱ��ͬ������ */
					0,                  /* �����ݿ鷢����Ϣ������ */
					0,                  /* �����ݿ������Ϣ������ */
					1,                  /* ���ݿ鷢����Ϣ������ */
					1,                  /* ���ݿ������Ϣ������ */
				},
				{
					{
						"NRT->NRT",			/*��Ϣ����*/
				 		NC_MSG_ID_TO_NC_2,	/*��ϢID*/
						3,					/*��Ϣ����*/
						0,					/*��Ϣ���ȼ�*/
						MB_NODE3_FCID,		/*��Ϣ��ԴASM�˿�port_id*/
						MB_NODE1_FCID,		/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
					{
						"NRT->NRT",			/*��Ϣ����*/
						NC_MSG_ID_TO_NC_2,	/*��ϢID*/
						3,					/*��Ϣ����*/
						0,					/*��Ϣ���ȼ�*/
						MB_NODE3_FCID,		/*��Ϣ��ԴASM�˿�port_id*/
						MB_NODE1_FCID,		/*��Ϣ��Ŀ��ASM�˿�port_id*/
					},
        
				},
				{
					{
						"NRT->NRT",			/*��Ϣ����*/
						NC_MSG_ID_TO_NC_1,  /*��ϢID*/
						FC_AMS_IU_LENGTH_RECV,				/*��Ϣ����*/
						3,					/*��Ϣ����  �¼�*/
						MB_NODE1_FCID,		/*��Ϣ��ԴASM�˿�port_id */
						0					/*��Ϣ��ˢ�������־*/
					},
					{
						"NRT->NRT",			/*��Ϣ����*/
						MSG_ID_11,  /*��ϢID*/
						FC_AMS_IU_LENGTH_RECV,				/*��Ϣ����*/
						3,					/*��Ϣ����  �¼�*/
						MB_NODE1_FCID,		/*��Ϣ��ԴASM�˿�port_id */
						0				    /*��Ϣ��ˢ�������־*/
					},	
				},
			},	


		 },

	},	
  

};
