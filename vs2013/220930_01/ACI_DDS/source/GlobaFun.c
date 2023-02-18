#include "../include/GlobalDefine.h"
void QY_Sleep(UINT sleepTime)
{
#if defined(_WIN32)
    Sleep(sleepTime);
#elif defined(__linux__)
/*TODO：sleep接口，vxWorks的usleep可能无法使用*/
	   struct timespec tc;
       tc.tv_sec = sleepTime / 1000;
       tc.tv_nsec = (sleepTime % 1000) *  1,000,000;
       nanosleep(&tc,NULL);
#elif defined(_VXWORKS)
 
#if defined _TM1 || defined _TM3
       RETURN_CODE_TYPE returnCode;
       TIMED_WAIT(sleepTime * 1000000, &returnCode);
#else
       taskDelay(sysClkRateGet() / 1000  * sleepTime );
#endif
#endif
}
