#include "stdafx.h"

namespace  Core {

b32  TimePeriodInit;

unsigned __int64 g_qpc_freq;
unsigned __int64 g_qpc_start;

void		
Time_Init()
{
	TimePeriodInit = (timeBeginPeriod(1) == TIMERR_NOERROR);

    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    g_qpc_freq = freq.QuadPart;

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    g_qpc_start = counter.QuadPart;

}

void		
Time_Free()
{
	if(TimePeriodInit) timeEndPeriod(1);
}


namespace  Time {


u32			
GetMS() 
{
	return timeGetTime();
}

u64			
GetMicroS() 
{
	LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return ((counter.QuadPart - g_qpc_start)*1000000 / g_qpc_freq);
}


}
}