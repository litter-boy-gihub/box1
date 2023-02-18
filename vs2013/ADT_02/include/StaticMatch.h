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
	UINT32		iStaticAddr;/* 静态节点的地址，唯一即可，FC版本可设置为FCid；以太网则为IP地址*/
	INT32		iProcessNum;/* 1~n，同一个网络地址上多个参与者，按顺序递增；标识的是同一个地址上不同进程，一个进程只允许一个参与者 */	  
	short		usPort[2];  /* 端口信息，以太网时，[0]表示sedp单播接收端口号，[1]表示用户单播接收端口号 */

	INT32		iWriteTopicNum;
	StaticTopic	WriteTopic[MAX_WTOPIC_PER_APP];
	INT32	    iReadTopicNum;
	StaticTopic	ReadTopic[MAX_RTOPIC_PER_APP];
}StaticParticipant;
//const int MAX_STATICAPP_NUM = 256;
#define MAX_STATICAPP_NUM 256

typedef struct _StaticInof
{
	INT32				iAppSize;     //应用总个数
	StaticParticipant   stStaticApps[MAX_STATICAPP_NUM];
}STATICINOF;

/********************************** 对外接口，静态订阅发布必须调用 ****************************************/
DDS_DLL void TestInitStaticConfig();
DDS_DLL void InitSTATICINOF(int localIndex); /* 设置当前应用下标，静态匹配必须调用，用于指定当前进程用的是哪个配置 */

RETSTATUS GenStaticPrefix(StaticParticipant appConfig, GuidPrefix_t *guidPrefix);/* 生成前缀 */

extern void CreateStaticLocalDataWriter(DDS_DomainParticipant* pstParticipant, const CHAR* pcTopicName, const CHAR* pcTopicType);
extern void CreateStaticLocalDataReader(DDS_DomainParticipant* pstParticipant, const CHAR* pcTopicName, const CHAR* pcTopicType);

extern int AddStaticDiscDomainParticipant(DDS_DomainParticipant* pstParticipant);
int AddStaticDiscWriter(DDS_DomainParticipant* pstParticipant, DDS_DiscoveredParticipant * pstDiscParticipant, StaticParticipant stStaticApps);
int AddStaticDiscReader(DDS_DomainParticipant* pstParticipant, DDS_DiscoveredParticipant * pstDiscParticipant, StaticParticipant stStaticApps);

#endif // ! _STATICMATCH_H

