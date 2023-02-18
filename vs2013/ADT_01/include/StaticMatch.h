#ifndef  _STATICMATCH_H
#define _STATICMATCH_H

typedef struct _StaticTopic
{
	//BoundedString256 sTopicName;
	const char * sTopicName;
	const char * sTopicType;
	//BoundedString32  sTopicType;
}StaticTopic;
#define MAX_WTOPIC_PER_APP 256
//const int MAX_WTOPIC_PER_APP = 256;
#define MAX_RTOPIC_PER_APP 256
//const int MAX_RTOPIC_PER_APP = 256;

typedef struct _StaticParticipant
{
	UINT32		iStaticAddr;/* ��̬�ڵ�ĵ�ַ��Ψһ���ɣ�FC�汾������ΪFCid����̫����ΪIP��ַ*/
	INT32		iProcessNum;/* 1~n��ͬһ�������ַ�϶�������ߣ���˳���������ʶ����ͬһ����ַ�ϲ�ͬ���̣�һ������ֻ����һ�������� */	  
	short		usPort[2];  /* �˿���Ϣ����̫��ʱ��[0]��ʾsedp�������ն˿ںţ�[1]��ʾ�û��������ն˿ں� */

	INT32		iWriteTopicNum;
	StaticTopic	WriteTopic[MAX_WTOPIC_PER_APP];
	INT32	    iReadTopicNum;
	StaticTopic	ReadTopic[MAX_RTOPIC_PER_APP];
}StaticParticipant;
//const int MAX_STATICAPP_NUM = 256;
#define MAX_STATICAPP_NUM 256

typedef struct _StaticInof
{
	INT32				iAppSize;     //Ӧ���ܸ���
	StaticParticipant   stStaticApps[MAX_STATICAPP_NUM];
}STATICINOF;

/********************************** ����ӿڣ���̬���ķ���������� ****************************************/
DDS_DLL void TestInitStaticConfig();
DDS_DLL void InitSTATICINOF(int localIndex); /* ���õ�ǰӦ���±꣬��̬ƥ�������ã�����ָ����ǰ�����õ����ĸ����� */

RETSTATUS GenStaticPrefix(StaticParticipant appConfig, GuidPrefix_t *guidPrefix);/* ����ǰ׺ */

extern void CreateStaticLocalDataWriter(DDS_DomainParticipant* pstParticipant, const CHAR* pcTopicName, const CHAR* pcTopicType);
extern void CreateStaticLocalDataReader(DDS_DomainParticipant* pstParticipant, const CHAR* pcTopicName, const CHAR* pcTopicType);

extern int AddStaticDiscDomainParticipant(DDS_DomainParticipant* pstParticipant);
int AddStaticDiscWriter(DDS_DomainParticipant* pstParticipant, DDS_DiscoveredParticipant * pstDiscParticipant, StaticParticipant stStaticApps);
int AddStaticDiscReader(DDS_DomainParticipant* pstParticipant, DDS_DiscoveredParticipant * pstDiscParticipant, StaticParticipant stStaticApps);

#endif // ! _STATICMATCH_H

