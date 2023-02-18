#include <iostream>

extern "C"
{
    #include "../include/GlobalDefine.h"
}

using namespace std;

//主题数据类型
enum QYTOPIC_TYPE
{
	YTOPIC_NET,			//网络级服务
	YTOPIC_PROCESS,		//进程级服务
	YTOPIC_THREAD,		//线程级服务
	YTOPIC_INVALID,		//无效类型
};

typedef struct  _QYMessageResult
{
	bool			m_success;					//是否成功
	unsigned short	m_resultLength;				//结果长度
	char *			m_result;					//结果数据

}QYMessageResult;

typedef struct  _YNetMessage
{
	bool				m_applySpace;			//是否申请的空间，如果不是的话，则表示是用户自己的空间，则不允许添加前后缀
	unsigned short		m_headIndex;			//头部下标
	char *				m_msgData;				//内部维护的数据
}QYNetMessage;

#include "QYTopicSender.h"
#include "QYTopicReceiver.h"