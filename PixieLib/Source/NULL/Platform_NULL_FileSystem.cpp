//*** Platform_NULL_FileSystem.cpp ***

#include "Platform_NULL_FileSystem.h"
#include "Platform_NULL_FileSystem_Directory.h"
#include "Platform_NULL_FileSystem_File.h"


//*** Constructor ***

Platform_NULL_FileSystem::Platform_NULL_FileSystem()
	{
	}


//*** SetLogging ***

void Platform_NULL_FileSystem::SetLogging(bool enabled)
	{
	}

		
//*** GetDirectory ***

Platform_FileSystem_Directory* Platform_NULL_FileSystem::CreateDirectoryObject(const char* path)
	{
	return new Platform_NULL_FileSystem_Directory();
	}


//*** GetFile ***

Platform_FileSystem_File* Platform_NULL_FileSystem::CreateFileObject(const char* path)
	{
	return new Platform_NULL_FileSystem_File();
	}


//*** RescanDevices ***

void Platform_NULL_FileSystem::RescanDevices()
	{
	}


//*** GetDeviceCount ***

int Platform_NULL_FileSystem::GetDeviceCount()
	{
	return 0;
	}


//*** GetDevice ***

const Platform_FileSystem_Device* Platform_NULL_FileSystem::GetDevice(int index)
	{
	return 0;
	}