//*** Platform_Win32_FileSystem_Device.cpp ***

#include "Platform_Win32_FileSystem_Device.h"
#include "Platform_OS.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <string.h>
#include <malloc.h>


//*** Constructor ***

Platform_Win32_FileSystem_Device::Platform_Win32_FileSystem_Device(const char* deviceName):
	name_(0)
	{
	if (!deviceName)
		{
		Platform::GetPlatform_OS()->OutputDebugText("No device name specified\n");
		}

	if (deviceName)
		{
		name_=strdup(deviceName);
		}
	}


//*** Destructor ***

Platform_Win32_FileSystem_Device::~Platform_Win32_FileSystem_Device()
	{
	if (name_)
		{
		free(name_);
		}
	}


//*** GetName ***

const char* Platform_Win32_FileSystem_Device::GetName() const
	{
	return name_;
	}


