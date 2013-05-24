/**
 * \class	Platform_NULL_FileSystem_Directory
 * 
 * \ingroup	platform_null
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_NULL_FileSystem_Directory_H__
#define __Platform_NULL_FileSystem_Directory_H__

// Includes
#include "Platform_FileSystem_Directory.h"

// Forward declares

// Platform_NULL_FileSystem_Directory
class Platform_NULL_FileSystem_Directory: public Platform_FileSystem_Directory
	{
	public:
		Platform_NULL_FileSystem_Directory();

		virtual const char* GetPath();

		virtual bool Exists();
		virtual void Create();
		virtual void Delete();

		virtual void Rescan();
		virtual int GetSubdirectoryCount();
		virtual const char* GetSubdirectory(int index);
		virtual int GetFileCount();
		virtual const char* GetFile(int index);
	};

#endif /* __Platform_NULL_FileSystem_Directory_H__ */
