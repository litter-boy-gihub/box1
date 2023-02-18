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

/*���ñ�*/
BLUE_PRINT gBluePrint[1] =
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
