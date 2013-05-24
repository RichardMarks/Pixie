//*** Platform_NULL_Time ***

#include "Platform_NULL_Time.h"


//*** Constructor ***

Platform_NULL_Time::Platform_NULL_Time()
	{
	}


//*** GetTime ***

float Platform_NULL_Time::GetTime()
	{
	return 0;
	}


//*** GetSystemTime ***

Platform_Time::SystemTime Platform_NULL_Time::GetSystemTime(bool utcTime)
	{
	SystemTime result;
	result.day=0;
	result.dayOfWeek=0;
	result.hour=0;
	result.minute=0;
	result.month=0;
	result.second=0;
	result.year=0;
	return result;
	}