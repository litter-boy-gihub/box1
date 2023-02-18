#include "../include/GlobalDefine.h"
#pragma warning(disable:4996)
char *GetVersionInfo()
{
#if defined(_WIN32)
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (!GetVersionEx(&osvi)) {
		return NULL;
	}

	if (osvi.dwMajorVersion == 5) {

		switch (osvi.dwMinorVersion) {
		case 0:
			return "Windows 2000";
		case 1:
			return "Windows XP";
		case 2:
			return "Windows Server 2003";
		default:
			return "Unknown System";
		}
	}
	else if (osvi.dwMajorVersion == 6) {

		switch (osvi.dwMinorVersion) {
		case 0:
			return "Windows Vista";
		case 1:
			return "Windows 7";
		case 2:
			return "Windows 8";
		default:
			return "Unknown System";
		}
	}
	else if (osvi.dwMajorVersion == 10) {

		switch (osvi.dwMinorVersion) {
		case 0:
			return "Windows 10";
		default:
			return "Unknown System";
		}
	}
	else {
		return "Unknown System";
	}
#elif defined (__linux__)
    return "Linux";
#elif defined _VXWORKS
    return "VxWorks";
#endif
}


VOID SetPropertyList(PropertyList* pstPropertyList)
{
	char* buf ;
#ifdef _WIN32
	WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 1), &wsaData);
#endif

	pstPropertyList->PropertySize = 4;

	strcpy(pstPropertyList->listBuf[0].name.value, "dds.sys_info.hostname");
	pstPropertyList->listBuf[0].name.length = 32;
#if defined _TM1 ||  defined _TM3
	strcpy(pstPropertyList->listBuf[0].value.value, "admin");
#else
	gethostname(pstPropertyList->listBuf[0].value.value, 64);
#endif
	pstPropertyList->listBuf[0].value.length = 64;

	strcpy(pstPropertyList->listBuf[1].name.value, "dds.sys_info.system");
	pstPropertyList->listBuf[1].name.length = 32;
	buf = GetVersionInfo();
	if (buf != NULL)
	{
		strcpy(pstPropertyList->listBuf[1].value.value, buf);
	}
	pstPropertyList->listBuf[1].value.length = 32;

	strcpy(pstPropertyList->listBuf[2].name.value, "dds.sys_info.process_id");
	pstPropertyList->listBuf[2].name.length = 32;
	sprintf(pstPropertyList->listBuf[2].value.value,"%d", getProcessId());
	pstPropertyList->listBuf[2].value.length = 16;

	strcpy(pstPropertyList->listBuf[3].name.value, "dds.sys_info.host_ip");
	pstPropertyList->listBuf[3].name.length = 32;
	getHostIPv4AddrList(&pstPropertyList->listBuf[3].value);
	pstPropertyList->listBuf[3].value.length = 32;
}
