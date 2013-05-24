//*** Platform_Win32_Time ***

#include "Platform_Win32_Time.h"


#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <windows.h>
#include <time.h>

//*** Constructor ***

Platform_Win32_Time::Platform_Win32_Time()
	{
	// Store the performance counter frequency
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	performanceFrequency_=(double)f.QuadPart;

	// Store the initial value of the performance counter, so that 
	// the time we returned is always measured from when the 
	// application was started, instead of from when the computer
	// was last rebooted.
	LARGE_INTEGER c;
	QueryPerformanceCounter(&c);
	initialValue_=(double)c.QuadPart;
	}


//*** GetTime ***

float Platform_Win32_Time::GetTime()
	{
	// Retrieve the current performance counter value
	LARGE_INTEGER c;
	QueryPerformanceCounter(&c);
	double performanceCount=(double)c.QuadPart;

	// Compensate for initial value (this will also improve precision of the float we return)
	performanceCount-=initialValue_;

	// Calculate the number of millisecons
	float result=(float)(performanceCount/performanceFrequency_);

	// And return it
	return result;
	}


//*** GetSystemTime ***

Platform_Time::SystemTime Platform_Win32_Time::GetSystemTime(bool utcTime)
	{
	SYSTEMTIME sysTime;
	if (utcTime)
		{
		::GetSystemTime(&sysTime);
		}
	else
		{
		GetLocalTime(&sysTime);
		}
	
	SystemTime result;
	result.year=(short)sysTime.wYear;
	result.month=(char)sysTime.wMonth;
	result.day=(char)sysTime.wDay;
	result.dayOfWeek=(char)sysTime.wDayOfWeek;
	result.hour=(char)sysTime.wHour;
	result.minute=(char)sysTime.wMinute;
	result.second=(char)sysTime.wSecond;

	return result;
	}