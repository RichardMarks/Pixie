/**
 * \class	Platform_Win32_FileSystem
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_Win32_FileSystem_H__
#define __Platform_Win32_FileSystem_H__

// Includes
#include "Platform_FileSystem.h"

// Forward declares

// Platform_Win32_FileSystem
class Platform_Win32_FileSystem:public Platform_FileSystem
	{

	public:
		Platform_Win32_FileSystem();

		virtual ~Platform_Win32_FileSystem();

		virtual void SetLogging(bool enabled);

		virtual Platform_FileSystem_Directory* CreateDirectoryObject(const char* path);
		virtual Platform_FileSystem_File* CreateFileObject(const char* path);
		
		virtual void RescanDevices();
		virtual int GetDeviceCount();
		virtual const Platform_FileSystem_Device* GetDevice(int index);

	private:
		void EmptyDeviceList();
	private:
		static const int MAX_DEVICES=64;
		int deviceCount_;
		Platform_FileSystem_Device* devices_[MAX_DEVICES]; 
		bool logging_;

	};

#endif /* __Platform_Win32_FileSystem_H__ */
