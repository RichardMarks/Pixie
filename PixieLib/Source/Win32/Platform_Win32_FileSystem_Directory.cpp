//*** Platform_Win32_FileSystem_Directory.cpp ***

#include "Platform_OS.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <windows.h>
#include <string.h>
#include <malloc.h>

#include "Platform_Win32_FileSystem_Directory.h"



//*** Constructor ***

Platform_Win32_FileSystem_Directory::Platform_Win32_FileSystem_Directory(const char* path):
	path_(0),
	subdirectoryCount_(0),
	subdirectories_(0),
	fileCount_(0),
	files_(0)
	{
	if (!path)
		{
		Platform::GetPlatform_OS()->OutputDebugText("No path specified!\n");
		}

	if (path)
		{
		path_=strdup(path);
		}
	
	Rescan();
	}


//*** Destructor ***

Platform_Win32_FileSystem_Directory::~Platform_Win32_FileSystem_Directory()
	{
	if (path_)
		{
		free(path_);
		}

	EmptySubdirectoryList();
	if (subdirectories_)
		{
		free(subdirectories_);
		}

	EmptyFileList();
	if (files_)
		{
		free(files_);
		}
	}


//*** GetPath ***

const char* Platform_Win32_FileSystem_Directory::GetPath()
	{
	return path_;
	}


//*** Exists ***

bool Platform_Win32_FileSystem_Directory::Exists()
	{
	if (!path_)
		{
		return false;
		}

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(GetPath(), &findFileData);
	if (hFind!=INVALID_HANDLE_VALUE && (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
		FindClose(hFind);		
		return true;
		}

	FindClose(hFind);
	return false;
	}


//*** Create ***

void Platform_Win32_FileSystem_Directory::Create()
	{
	if (path_)
		{
		CreateDirectory(path_,0);
		}
	}


//*** Delete ***

void Platform_Win32_FileSystem_Directory::Delete()
	{
	if (path_)
		{
		RemoveDirectory(path_);
		}
	}


//*** Rescan ***

void Platform_Win32_FileSystem_Directory::Rescan()
	{
	if (!path_)
		{
		return;
		}

	// Get rid of any existing subdirectories
	EmptySubdirectoryList();

	// Create search string
	const char* searchPattern="*";
	char* absoluteSearchPattern=new char[strlen(path_) + strlen(searchPattern)+2];
	strcpy(absoluteSearchPattern, path_);
	strcat(absoluteSearchPattern, "/");
	strcat(absoluteSearchPattern, searchPattern);

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(absoluteSearchPattern, &findFileData);
	if (hFind!=INVALID_HANDLE_VALUE)
		{
		// List files and subdirs
		BOOL loop=true;
		while (loop)
			{
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
				if ((stricmp(findFileData.cFileName,".")!=0) && (stricmp(findFileData.cFileName,"..")!=0))
					{
					if (!subdirectories_)
						{
						subdirectoryMaxCount_=64;
						subdirectories_=static_cast<const char**>(malloc(subdirectoryMaxCount_*sizeof(const char*)));
						}
					else if (subdirectoryCount_>=subdirectoryMaxCount_)
						{
						subdirectoryMaxCount_*=2;
						subdirectories_=static_cast<const char**>(realloc(subdirectories_,subdirectoryMaxCount_*sizeof(const char*)));
						
						}
					subdirectories_[subdirectoryCount_]=strdup(findFileData.cFileName);
					subdirectoryCount_++;
					}
				}
			else
				{
				if (!files_)
					{
					fileMaxCount_=64;
					files_=static_cast<const char**>(malloc(fileMaxCount_*sizeof(const char*)));
					}
				else if (fileCount_>=fileMaxCount_)
					{
					fileMaxCount_*=2;
					files_=static_cast<const char**>(realloc(files_,fileMaxCount_*sizeof(const char*)));
					
					}
				files_[fileCount_]=strdup(findFileData.cFileName);
				fileCount_++;
				}

			loop=FindNextFile(hFind,&findFileData);
			}
		}

	delete absoluteSearchPattern;
	FindClose(hFind);
	}



//*** EmptySubdirectoryList ***

void Platform_Win32_FileSystem_Directory::EmptySubdirectoryList()
	{
	for (int i=0; i<subdirectoryCount_; i++)
		{
		char* name=const_cast<char*>(subdirectories_[i]);
		free(name);
		}
	subdirectoryCount_=0;
	}


//*** EmptyFileList ***

void Platform_Win32_FileSystem_Directory::EmptyFileList()
	{
	for (int i=0; i<fileCount_; i++)
		{
		char* name=const_cast<char*>(files_[i]);
		free(name);
		}
	fileCount_=0;
	}


//*** GetSubdirectoryCount ***

int Platform_Win32_FileSystem_Directory::GetSubdirectoryCount()
	{
	return subdirectoryCount_;
	}


//*** GetSubdirectory ***

const char* Platform_Win32_FileSystem_Directory::GetSubdirectory(int index)
	{	
	if (index<0 || index>=subdirectoryCount_)
		{
		return 0;
		}
	
	return subdirectories_[index];
	}


//*** GetFileCount ***

int Platform_Win32_FileSystem_Directory::GetFileCount()
	{
	return fileCount_;
	}


//*** GetFile***

const char* Platform_Win32_FileSystem_Directory::GetFile(int index)
	{	
	if (index<0 || index>=fileCount_)
		{
		return 0;
		}
	
	return files_[index];
	}
