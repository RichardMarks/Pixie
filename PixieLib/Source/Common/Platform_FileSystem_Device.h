/**
 * \class	Platform_FileSystem_Device
 *
 * \ingroup	platform
 * \brief	Platform independent abstraction of a file system device
 * \author	Mattias Gustavsson	
 * 
 * Provides access to the name of a file system device. This will be
 * platform specific.
 */

#ifndef __Platform_FileSYSTEM_Device_H__
#define __Platform_FileSYSTEM_Device_H__

// Includes

// Forward declares


// Platform_Files_Directory
class Platform_FileSystem_Device
	{
	public:

		/**
		 * Destructor
		 */
		virtual ~Platform_FileSystem_Device() { };
	

		/** 
		 * Retrieves the name of this device
		 *
		 * \returns	The name of the device
		 */
		virtual const char* GetName() const = 0;
		
	};

#endif /* __Platform_FileSYSTEM_Device_H__ */
