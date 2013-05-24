/**
 * \class	Platform_Win32_FileSystem_Directory
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_Win32_FileSystem_Directory_H__
#define __Platform_Win32_FileSystem_Directory_H__

// Includes
#include "Platform_FileSystem_Directory.h"

// Forward declares

// Platform_Win32_FileSystem_Directory
class Platform_Win32_FileSystem_Directory: public Platform_FileSystem_Directory
	{
	public:
		Platform_Win32_FileSystem_Directory(const char* path);
		virtual ~Platform_Win32_FileSystem_Directory();

		virtual const char* GetPath();

		virtual bool Exists();
		virtual void Create();
		virtual void Delete();

		virtual void Rescan();
		virtual int GetSubdirectoryCount();
		virtual const char* GetSubdirectory(int index);
		virtual int GetFileCount();
		virtual const char* GetFile(int index);

	private:
		void EmptySubdirectoryList();
		void EmptyFileList();
	private:
		char* path_;	///< The full path of the directory

		int subdirectoryCount_;
		int subdirectoryMaxCount_;
		const char** subdirectories_; ///< Array containing the names of all subdirectories
		
		int fileCount_;
		int fileMaxCount_;
		const char** files_; ///< Array containing the names of all files in this directory
	};

#endif /* __Platform_Win32_FileSystem_Directory_H__ */
