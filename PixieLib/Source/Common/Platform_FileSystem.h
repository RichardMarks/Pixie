/**
 * \class	Platform_FileSystem
 * 
 * \ingroup	platform
 * \brief	Platform independent abstraction of the file system
 * \author	Mattias Gustavsson	
 * 
 * Handles access to files, folders and device names. This system
 * gives to advantages over the standard c-runtime functions: you
 * can enable logging of file access (to find out where in the
 * game your loading occurs, which can sometimes be non-obvious
 * with several layers of abstraction and automated systems), but
 * more importantlu, it handles endian-ness automatically, making
 * sure all data is converted to little-endian when written, and
 * converted from little-endian to whatever is used by the current 
 * platform when read.
 */

#ifndef __Platform_FileSystem_H__
#define __Platform_FileSystem_H__

// Includes
#include "Platform.h"

// Forward declares
class Platform_FileSystem_Directory;
class Platform_FileSystem_File;
class Platform_FileSystem_Device;

// Platform_FileSystem
class Platform_FileSystem
	{
	public:
		/**
		 * Destructor
		 */
		virtual ~Platform_FileSystem() { };


		/**
		 * Sometimes, for debug purposes, it might be useful to find out where and when
		 * things are loaded. If logging is enabled (it is disabled by default) a debug
		 * text string will be written every time a file or directory object is requested 
		 * through the Platform_FileSystem, including the full path of the file/directory. 
		 */
		virtual void SetLogging(
			bool enabled	///< True if you want file access to be logged, false if you don't
			) = 0;


		/**
		 * Creates a platform independent directory object corresponding to the specified
		 * path. The directory may or may not exist - the directory object is merely 
		 * representational, and can be queried for whether the directory exists or not,
		 * and used to create/delete it as necessary.
		 *
		 * The returned object needs to be destroyed by the caller (using the delete keyword) 
		 * when it is no longer needed.
		 *
		 * The path can be a full or relative path, with directories separated by a
		 * forward slash (/). It is up to the platform specific implementation to
		 * replace these separators with whatever is used on that platform.
		 *
		 * \returns	A new Platform_FileSystem_Directory object representing the directory
		 */
		virtual Platform_FileSystem_Directory* CreateDirectoryObject(
			const char* path	///< Path to create a directory object for
			) = 0;
		

		/**
		 * Creates a platform independent file object corresponding to the specified
		 * path. The file may or may not exist - the file object is merely  representational, 
		 * and can be queried for whether the file exists or not,  and used to create,  open 
		 * or delete the file as necessary.
		 *
		 * The returned object needs to be destroyed by the caller (using the delete keyword) 
		 * when it is no longer needed.
		 *
		 * The path can be a full or relative path, with directories separated by a
		 * forward slash (/). It is up to the platform specific implementation to
		 * replace these separators with whatever is used on that platform.
		 *
		 * \returns	A new Platform_FileSystem_File object representing the file
		 */
		virtual Platform_FileSystem_File* CreateFileObject(
			const char* path	///< Path (including filename) to create a file object for
			) = 0;


		/**
		 * When Platform_FileSystem is created, it makes an internal list of the devices
		 * that are present in the system. This list can be updated by a call to this
		 * method, if you have reason to believe that the available devices have changed.
		 */
		virtual void RescanDevices() = 0;


		/**
		 * Retrieves the number of available devices for the system
		 *
		 * \returns The number of available devices
		 */
		virtual int GetDeviceCount() = 0;


		/**
		 * Retrieves the device with the specified index, from 0 to the value returned by
		 * GetDeviceCount. The object returned is owned and controlled by Platform_FileSystem,
		 * and shouldn't be deleted by the caller
		 *
		 * \returns	A device object representing the device at the specified index
		 */
		virtual const Platform_FileSystem_Device* GetDevice(
			int index	///> Index of the device
			) = 0;

	};


#endif /* __Platform_FileSystem_H__ */
