//*** Platform_Win32_FileSystem.cpp ***

#include "Platform_Win32_FileSystem.h"
#include "Platform_Win32_FileSystem_Device.h"
#include "Platform_Win32_FileSystem_Directory.h"
#include "Platform_OS.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <windows.h>

#include "Platform_Win32_FileSystem_File.h"


//*** Constructor ***

Platform_Win32_FileSystem::Platform_Win32_FileSystem():
	deviceCount_(0),
	logging_(false)
	{
	RescanDevices();
	}


//*** Destructor ***

Platform_Win32_FileSystem::~Platform_Win32_FileSystem()
	{
	EmptyDeviceList();
	}


//*** SetLogging ***

void Platform_Win32_FileSystem::SetLogging(bool enabled)
	{
	logging_=enabled;
	}

		
//*** GetDirectory ***

Platform_FileSystem_Directory* Platform_Win32_FileSystem::CreateDirectoryObject(const char* path)
	{
	if (logging_ && path)
		{
		Platform::GetPlatform_OS()->OutputDebugText("  >> \n");
		Platform::GetPlatform_OS()->OutputDebugText("  >> FileSystem - CreateDirectoryObject: %s\n",path);
		Platform::GetPlatform_OS()->OutputDebugText("  >> \n");
		}
	return new Platform_Win32_FileSystem_Directory(path);
	}


//*** GetFile ***

Platform_FileSystem_File* Platform_Win32_FileSystem::CreateFileObject(const char* path)
	{
	if (logging_ && path)
		{
		Platform::GetPlatform_OS()->OutputDebugText("  >> \n");
		Platform::GetPlatform_OS()->OutputDebugText("  >> FileSystem - CreateFileObject: %s\n",path);
		Platform::GetPlatform_OS()->OutputDebugText("  >> \n");
		}
	return new Platform_Win32_FileSystem_File(path);
	}


//*** RescanDevices ***

void Platform_Win32_FileSystem::RescanDevices()
	{
	if (logging_)
		{
		Platform::GetPlatform_OS()->OutputDebugText("  >> \n");
		Platform::GetPlatform_OS()->OutputDebugText("  >> FileSystem - RescanDevices\n");
		Platform::GetPlatform_OS()->OutputDebugText("  >> \n");
		}

	// Get rid of any existing devices
	EmptyDeviceList();

	// Get the list of logical drives from windows
	char logicalDrives[256];
	int len=GetLogicalDriveStrings(255, logicalDrives);
	
	// Parse through the list 
	char deviceName[10];
	int pos=0; // current position in deviceName

	for (int i=0; i<len; i++)
		{
		// Add next character to the current device name
		deviceName[pos]=logicalDrives[i];

		// If we reached the end of the current device name
		if (logicalDrives[i]==0)
			{
			// Remove trailing "\"
			//Assert(deviceName[pos-1]=='\\',"Invalid device name");
			deviceName[pos-1]=0;

			// Create new device object
			Platform_FileSystem_Device* device=new Platform_Win32_FileSystem_Device(deviceName);
			
			// Add the device to the internal list
			devices_[deviceCount_]=device;
			deviceCount_++;
			
			// Reset device name position, as we're now starting to parse a new device name
			pos=0;
			}
		else // We have NOT yet reached the end of the current device name
			{
			// Increase the position
			if (pos<10) // But make sure it don't increase too much (should only ever be 3 characters anyway, like "C:\")
				{
				pos++;			
				}
			}
		}
		
	}


//*** EmptyDeviceList ***

void Platform_Win32_FileSystem::EmptyDeviceList()
	{
	for (int i=0; i<deviceCount_; i++)
		{
		Platform_FileSystem_Device* device=devices_[i];
		delete device;
		}
	deviceCount_=0;
	}


//*** GetDeviceCount ***

int Platform_Win32_FileSystem::GetDeviceCount()
	{
	return deviceCount_;
	}


//*** GetDevice ***

const Platform_FileSystem_Device* Platform_Win32_FileSystem::GetDevice(int index)
	{
	if (index<0 || index>=deviceCount_)
		{
		return 0;
		}
	
	return devices_[index];
	}