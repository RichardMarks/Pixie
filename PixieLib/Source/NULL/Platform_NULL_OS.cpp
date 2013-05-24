//*** Platform_NULL_OS ***

#include "Platform_NULL_OS.h"


//*** Constructor ***

Platform_NULL_OS::Platform_NULL_OS()
	{
	}

//*** OsYield ***

void Platform_NULL_OS::OsYield()
	{
	Platform::SendEvent_OsYield();
	}


//*** SetApplicationName ***

void Platform_NULL_OS::SetApplicationName(const char* applicationName)
	{
	}


//*** GetCommandLineString ***

const char* Platform_NULL_OS::GetCommandLineString()
	{
	return "";	
	}


//*** GetExecutablePath ***

const char* Platform_NULL_OS::GetExecutablePath()
	{
	return "";	
	}


//*** HasFocus ***

bool Platform_NULL_OS::HasFocus()
	{
	return true;	
	}


//*** ExitRequested ***

bool Platform_NULL_OS::ExitRequested()
	{
	return false;
	}


//*** DisplayAssertMessage ***

void Platform_NULL_OS::DisplayAssertMessage(const char* expression, const char* message, const char* file, int line)
	{
	}


//*** OutputDebugText ***

void Platform_NULL_OS::OutputDebugText(const char * formatstr, ...)
	{
	}


//*** ReportFatalError ***

void Platform_NULL_OS::ReportFatalError(const char* message, const char* file, int line)
	{
	}
