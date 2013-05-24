//*** Platform_NULL_FileSystem_Directory.cpp ***

#include "Platform_NULL_FileSystem_Directory.h"


//*** Constructor ***

Platform_NULL_FileSystem_Directory::Platform_NULL_FileSystem_Directory()
	{
	}


//*** GetPath ***

const char* Platform_NULL_FileSystem_Directory::GetPath()
	{
	return "";
	}


//*** Exists ***

bool Platform_NULL_FileSystem_Directory::Exists()
	{
	return false;
	}


//*** Create ***

void Platform_NULL_FileSystem_Directory::Create()
	{
	}


//*** Delete ***

void Platform_NULL_FileSystem_Directory::Delete()
	{
	}


//*** Rescan ***

void Platform_NULL_FileSystem_Directory::Rescan()
	{
	}


//*** GetSubdirectoryCount ***

int Platform_NULL_FileSystem_Directory::GetSubdirectoryCount()
	{
	return 0;
	}


//*** GetSubdirectory ***

const char* Platform_NULL_FileSystem_Directory::GetSubdirectory(int index)
	{	
	return 0;
	}


//*** GetFileCount ***

int Platform_NULL_FileSystem_Directory::GetFileCount()
	{
	return 0;
	}


//*** GetFile***

const char* Platform_NULL_FileSystem_Directory::GetFile(int index)
	{	
	return 0;
	}
