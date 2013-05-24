/**
 * \class	Platform_NULL_FileSystem
 * 
 * \ingroup	platform_null
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_NULL_FileSystem_H__
#define __Platform_NULL_FileSystem_H__

// Includes
#include "Platform_FileSystem.h"

// Forward declares

// Platform_NULL_FileSystem
class Platform_NULL_FileSystem:public Platform_FileSystem
	{

	public:
		Platform_NULL_FileSystem();

		virtual void SetLogging(bool enabled);

		virtual Platform_FileSystem_Directory* CreateDirectoryObject(const char* path);
		virtual Platform_FileSystem_File* CreateFileObject(const char* path);
		
		virtual void RescanDevices();
		virtual int GetDeviceCount();
		virtual const Platform_FileSystem_Device* GetDevice(int index);

	};

#endif /* __Platform_NULL_FileSystem_H__ */
