//*** Platform_Win32_FileSystem_File.cpp ***

#include "Platform_OS.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <windows.h>
#include <malloc.h>

#include "Platform_Win32_FileSystem_File.h"

//*** Constructor ***

Platform_Win32_FileSystem_File::Platform_Win32_FileSystem_File(const char* path):
	path_(0),
	fileHandle_(INVALID_HANDLE_VALUE)
	{
	if (!path)
		{
		Platform::GetPlatform_OS()->OutputDebugText("No path specified!\n");
		}

	if (path)
		{
		path_=strdup(path);
		}
	
	}


//*** Destructor ***

Platform_Win32_FileSystem_File::~Platform_Win32_FileSystem_File()
	{
	if (path_)
		{
		free(path_);
		}
	
	if (fileHandle_!=INVALID_HANDLE_VALUE)
		{
		CloseHandle(fileHandle_);
		}
	}


//*** GetPath ***

const char* Platform_Win32_FileSystem_File::GetPath()
	{
	return path_;
	}


//*** Exists ***

bool Platform_Win32_FileSystem_File::Exists()
	{
	if (!path_)
		{
		return false;
		}

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(GetPath(), &findFileData);
	if (hFind!=INVALID_HANDLE_VALUE && !(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
		FindClose(hFind);		
		return true;
		}

	FindClose(hFind);
	return false;
	}


//*** Create ***

bool Platform_Win32_FileSystem_File::Create()
	{
	if (fileHandle_!=INVALID_HANDLE_VALUE)
		{
		Platform::GetPlatform_OS()->OutputDebugText("File already open\n");
		return false;
		}

	fileHandle_=CreateFile(path_,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,0);
	return fileHandle_!=INVALID_HANDLE_VALUE;
	}


//*** Open ***

bool Platform_Win32_FileSystem_File::Open()
	{
	if (fileHandle_!=INVALID_HANDLE_VALUE)
		{
		Platform::GetPlatform_OS()->OutputDebugText("File already open\n");
		return false;
		}

	fileHandle_=CreateFile(path_,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	return fileHandle_!=INVALID_HANDLE_VALUE;
	}


//*** Close ***

bool Platform_Win32_FileSystem_File::Close()
	{
	if (fileHandle_==INVALID_HANDLE_VALUE)
		{
		Platform::GetPlatform_OS()->OutputDebugText("File not open\n");
		return false;
		}

	int result=CloseHandle(fileHandle_);
	fileHandle_=INVALID_HANDLE_VALUE;
	return result!=0;
	}


//*** Delete ***

void Platform_Win32_FileSystem_File::Delete()
	{
	if (path_)
		{
		DeleteFile(path_);
		}
	}


//*** GetPosition ***

int Platform_Win32_FileSystem_File::GetPosition()
	{
	if (fileHandle_==INVALID_HANDLE_VALUE)
		{
		Platform::GetPlatform_OS()->OutputDebugText("File not open\n");
		return 0;
		}
	return SetFilePointer(fileHandle_,0,0,FILE_CURRENT);
	}


//*** SetPosition ***

int Platform_Win32_FileSystem_File::SetPosition(int position,SetPositionOrigin origin)
	{	
	if (fileHandle_==INVALID_HANDLE_VALUE)
		{
		Platform::GetPlatform_OS()->OutputDebugText("File not open\n");
		return 0;
		}

	DWORD startingPoint=0;
	switch (origin)
		{
		case ORIGIN_START:
			{
			startingPoint=FILE_BEGIN;
			}break;
		case ORIGIN_CURRENT:
			{
			startingPoint=FILE_CURRENT;
			}break;
		case ORIGIN_END:
			{
			startingPoint=FILE_END;
			}break;
		}
	int result=SetFilePointer(fileHandle_,position,0,startingPoint);
	if (result==INVALID_SET_FILE_POINTER)
		{
		return -1;
		}

	return result;
	}


//*** WRITEMACRO ***

// This macro defines the code that is to be executed for all of the overloaded Write methods
// The code is identical for all the different Write methods, only the input parameters change,
// so it makes more sense to use a macro rather than copy-paste. This macro is undefined as
// soon as it is not needed anymore

#define WRITEMACRO()																						\
	if (fileHandle_==INVALID_HANDLE_VALUE)																	\
		{																									\
		Platform::GetPlatform_OS()->OutputDebugText("Tried to write to file that is not open for writing\n");	\
		return 0;																							\
		}																									\
																											\
	unsigned long numberOfBytesWritten=0; /* Stores the number of bytes read */								\
	/* Writes the data to the file */																		\
	BOOL result=WriteFile(fileHandle_,value,sizeof(*value)*count,&numberOfBytesWritten,0);					\
	if (!result)																							\
		{																									\
		Platform::GetPlatform_OS()->OutputDebugText("Write operation failed\n");									\
		return 0;																							\
		}																									\
																											\
	/* Return number of elements written, rather than the number of bytes */								\
	return numberOfBytesWritten/sizeof(*value);																\


//** Write methods */

int Platform_Win32_FileSystem_File::Write(const char* value, int count)
	{
	WRITEMACRO(); // The code for all the write methods are the same, so a macro is used instead of copy-paste
	}

int Platform_Win32_FileSystem_File::Write(const short* value, int count)
	{
	WRITEMACRO(); // The code for all the write methods are the same, so a macro is used instead of copy-paste
	}

int Platform_Win32_FileSystem_File::Write(const int* value, int count)
	{
	WRITEMACRO(); // The code for all the write methods are the same, so a macro is used instead of copy-paste
	}

int Platform_Win32_FileSystem_File::Write(const long* value, int count)
	{
	WRITEMACRO(); // The code for all the write methods are the same, so a macro is used instead of copy-paste
	}

int Platform_Win32_FileSystem_File::Write(const unsigned char* value, int count)
	{
	WRITEMACRO(); // The code for all the write methods are the same, so a macro is used instead of copy-paste
	}

int Platform_Win32_FileSystem_File::Write(const unsigned short* value, int count)
	{
	WRITEMACRO(); // The code for all the write methods are the same, so a macro is used instead of copy-paste
	}

int Platform_Win32_FileSystem_File::Write(const unsigned int* value, int count)
	{
	WRITEMACRO(); // The code for all the write methods are the same, so a macro is used instead of copy-paste
	}

int Platform_Win32_FileSystem_File::Write(const unsigned long* value, int count)
	{
	WRITEMACRO(); // The code for all the write methods are the same, so a macro is used instead of copy-paste
	}

int Platform_Win32_FileSystem_File::Write(const float* value, int count)
	{
	WRITEMACRO(); // The code for all the write methods are the same, so a macro is used instead of copy-paste
	}

int Platform_Win32_FileSystem_File::Write(const double* value, int count)
	{
	WRITEMACRO(); // The code for all the write methods are the same, so a macro is used instead of copy-paste
	}

int Platform_Win32_FileSystem_File::Write(const bool* value, int count)
	{
	WRITEMACRO(); // The code for all the write methods are the same, so a macro is used instead of copy-paste
	}

// No need for the write macro anymore, so undefine it
#undef WRITEMACRO 

//*** READMACRO ***

// This macro defines the code that is to be executed for all of the overloaded Read methods
// The code is identical for all the different Read methods, only the input parameters change,
// so it makes more sense to use a macro rather than copy-paste. This macro is undefined as
// soon as it is not needed anymore

#define READMACRO()																							\
	if (fileHandle_==INVALID_HANDLE_VALUE)																	\
		{																									\
		Platform::GetPlatform_OS()->OutputDebugText("Tried to read from file that is not open for reading\n");	\
		return 0;																							\
		}																									\
																											\
	unsigned long numberOfBytesRead=0; /* Stores the number of bytes read */								\
	/* Reads the data from the file */																		\
	BOOL result=ReadFile(fileHandle_,value,sizeof(*value)*count,&numberOfBytesRead,0);						\
	if (!result)																							\
		{																									\
		Platform::GetPlatform_OS()->OutputDebugText("Read operation failed\n");									\
		return 0;																							\
		}																									\
																											\
	/* Return number of elements read, rather than the number of bytes */									\
	return numberOfBytesRead/sizeof(*value);																\


//** Read methods */

int Platform_Win32_FileSystem_File::Read(char* value, int count)
	{
	READMACRO();
	}

int Platform_Win32_FileSystem_File::Read(short* value, int count)
	{
	READMACRO();
	}

int Platform_Win32_FileSystem_File::Read(int* value, int count)
	{
	READMACRO();
	}

int Platform_Win32_FileSystem_File::Read(long* value, int count)
	{
	READMACRO();
	}

int Platform_Win32_FileSystem_File::Read(unsigned char* value, int count)
	{
	READMACRO();
	}

int Platform_Win32_FileSystem_File::Read(unsigned short* value, int count)
	{
	READMACRO();
	}

int Platform_Win32_FileSystem_File::Read(unsigned int* value, int count)
	{
	READMACRO();
	}

int Platform_Win32_FileSystem_File::Read(unsigned long* value, int count)
	{
	READMACRO();
	}

int Platform_Win32_FileSystem_File::Read(float* value, int count)
	{
	READMACRO();
	}

int Platform_Win32_FileSystem_File::Read(double* value, int count)
	{
	READMACRO();
	}

int Platform_Win32_FileSystem_File::Read(bool* value, int count)
	{
	READMACRO();
	}

// No need for the read macro anymore, so undefine it
#undef READMACRO

