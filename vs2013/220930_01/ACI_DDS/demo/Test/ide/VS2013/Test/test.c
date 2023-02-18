#include "GlobalDefine.h"

//#include "YMd5/YMd5.h"
//#include "vld.h"
#include "crtdbg.h"
#include <Psapi.h>
#include "WinBase.h"
#pragma comment(lib,"Psapi.lib")

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//
////验证#337 循环执行创建和删除参与者的操作导致程序崩溃并有内存泄露
//
int get_memory_usage(UINT64 * mem, UINT64 * vmem)
{
	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
	{
		if (mem)
		{
			*mem = pmc.WorkingSetSize;
		}
		if (vmem)
		{
			*vmem = pmc.PagefileUsage;
		}
		return 0;
	}
	return -1;
}

void fun_Participant()
{
	DDS_ParticipantFactory_init();
	DDS_DomainParticipantQos stDomainQos;
	DDS_DomainParticipantFactory_get_defaul_participant_qos(&stDomainQos);
	
	DDS_DomainParticipant* pstParticipant = DDS_DomainParticipantFactory_create_participant(70, &stDomainQos);
	if (pstParticipant)
	{
		DDS_DomainParticipantFactory_delete_participant(pstParticipant);
	}
	Sleep(100);
}

int main()
{
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	int i = 0;
	while (1)
	{
		fun_Participant();
		UINT64 mem, vmem;
		get_memory_usage(&mem, &vmem);
		printf("-------------------- i = %d mem = %u, vmem = %d\n", ++i, mem, vmem);

		if (i == 100)
			break;
	}
	return 0;
}
