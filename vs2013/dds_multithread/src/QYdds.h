#include <iostream>

extern "C"
{
    #include "../include/GlobalDefine.h"
}

using namespace std;

//������������
enum QYTOPIC_TYPE
{
	YTOPIC_NET,			//���缶����
	YTOPIC_PROCESS,		//���̼�����
	YTOPIC_THREAD,		//�̼߳�����
	YTOPIC_INVALID,		//��Ч����
};

typedef struct  _QYMessageResult
{
	bool			m_success;					//�Ƿ�ɹ�
	unsigned short	m_resultLength;				//�������
	char *			m_result;					//�������

}QYMessageResult;

typedef struct  _YNetMessage
{
	bool				m_applySpace;			//�Ƿ�����Ŀռ䣬������ǵĻ������ʾ���û��Լ��Ŀռ䣬���������ǰ��׺
	unsigned short		m_headIndex;			//ͷ���±�
	char *				m_msgData;				//�ڲ�ά��������
}QYNetMessage;

#include "QYTopicSender.h"
#include "QYTopicReceiver.h"