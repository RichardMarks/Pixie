//*** Platform_NULL_FileSystem_File.cpp ***

#include "Platform_NULL_FileSystem_File.h"

//*** Constructor ***

Platform_NULL_FileSystem_File::Platform_NULL_FileSystem_File()
	{	
	}


//*** GetPath ***

const char* Platform_NULL_FileSystem_File::GetPath()
	{
	return "";
	}


//*** Exists ***

bool Platform_NULL_FileSystem_File::Exists()
	{
	return false;
	}


//*** Create ***

bool Platform_NULL_FileSystem_File::Create()
	{
	return false;
	}


//*** Open ***

bool Platform_NULL_FileSystem_File::Open()
	{
	return false;
	}


//*** Close ***

bool Platform_NULL_FileSystem_File::Close()
	{
	return false;
	}


//*** Delete ***

void Platform_NULL_FileSystem_File::Delete()
	{
	}


//*** GetPosition ***

int Platform_NULL_FileSystem_File::GetPosition()
	{
	return 0;
	}


//*** SetPosition ***

int Platform_NULL_FileSystem_File::SetPosition(int position,SetPositionOrigin origin)
	{	
	return 0;
	}


//** Write methods */

int Platform_NULL_FileSystem_File::Write(const char* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Write(const short* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Write(const int* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Write(const long* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Write(const unsigned char* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Write(const unsigned short* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Write(const unsigned int* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Write(const unsigned long* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Write(const float* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Write(const double* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Write(const bool* value, int count)
	{
	return 0;
	}


//** Read methods */

int Platform_NULL_FileSystem_File::Read(char* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Read(short* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Read(int* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Read(long* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Read(unsigned char* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Read(unsigned short* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Read(unsigned int* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Read(unsigned long* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Read(float* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Read(double* value, int count)
	{
	return 0;
	}

int Platform_NULL_FileSystem_File::Read(bool* value, int count)
	{
	return 0;
	}

