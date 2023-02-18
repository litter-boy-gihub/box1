#include "../include/GlobalDefine.h"

int getProcessId()
{
#if defined (_WIN32)
    int pid = _getpid();
    return pid;
#elif defined(__linux__)
    int pid = getpid();
    return pid;
#elif defined(_VXWORKS)

#if defined _TM1 || defined _TM3
    return 0xEFFF;
#else
    return taskIdSelf();
#endif
#endif // defined
}
