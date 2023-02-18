#ifndef HIPERD_FCNETAPI_H
#define HIPERD_FCNETAPI_H

#define FC_MAX_NODE_NUM 32
#define FC_MAX_STREAM_DATA_SIZE 2096

typedef struct TagSYCCommFCOneConfig
{
	UINT32						m_FCPort;
	UINT32 						m_localDDSSendMsgID;
	UINT32						m_localDDSRecvMsgID;
	UINT32						m_localUserSendMsgID;
	UINT32						m_localUserRecvMsgID;
}YCCommFCOneConfig;

typedef struct TagSYCCommFCConfig
{
	int							m_nodeNum;
	int							m_thisNodeFCPort;
	int *						m_FCdev;
	YCCommFCOneConfig			m_configs[FC_MAX_NODE_NUM];
}YCCommFCConfig;

#endif // !HIPERD_FCNETAPI_H
