/**
 * \class	Platform_Win32_FileSystem_File
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_Win32_FileSystem_File_H__
#define __Platform_Win32_FileSystem_File_H__

// Includes
#include "Platform_FileSystem_File.h"

// Forward declares

// Platform_Win32_FileSystem_File
class Platform_Win32_FileSystem_File: public Platform_FileSystem_File
	{
	public:
		Platform_Win32_FileSystem_File(const char* path);
		virtual ~Platform_Win32_FileSystem_File();

		virtual const char* GetPath();
		
		virtual bool Exists();
		virtual bool Create();
		virtual bool Open();
		virtual bool Close();
		virtual void Delete();
		virtual int GetPosition();
		virtual int SetPosition(int position,SetPositionOrigin origin);

		virtual int Write(const char* value, int count = 1);
		virtual int Write(const short* value, int count = 1);
		virtual int Write(const int* value, int count = 1);
		virtual int Write(const long* value, int count = 1);
		virtual int Write(const unsigned char* value, int count = 1);
		virtual int Write(const unsigned short* value, int count = 1);
		virtual int Write(const unsigned int* value, int count = 1);
		virtual int Write(const unsigned long* value, int count = 1);
		virtual int Write(const float* value, int count = 1);
		virtual int Write(const double* value, int count = 1);
		virtual int Write(const bool* value, int count = 1);

		virtual int Read(char* value, int count = 1);
		virtual int Read(short* value, int count = 1);
		virtual int Read(int* value, int count = 1);
		virtual int Read(long* value, int count = 1);
		virtual int Read(unsigned char* value, int count = 1);
		virtual int Read(unsigned short* value, int count = 1);
		virtual int Read(unsigned int* value, int count = 1);
		virtual int Read(unsigned long* value, int count = 1);
		virtual int Read(float* value, int count = 1);
		virtual int Read(double* value, int count = 1);
		virtual int Read(bool* value, int count = 1);

	private:
		char* path_;	///< The full path and filename for this file
		HANDLE fileHandle_; ///<
	};

#endif /* __Platform_Win32_FileSystem_File_H__ */
