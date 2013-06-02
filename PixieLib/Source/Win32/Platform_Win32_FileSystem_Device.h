/**
 * \class	Platform_Win32_FileSystem_Device
 *
 * \ingroup	win32
 * \brief
 * \author	Mattias Gustavsson
 *
 *
 */

#ifndef __Platform_Win32_FileSystem_Device_H__
#define __Platform_Win32_FileSystem_Device_H__

// Includes
#include "Platform_FileSystem_Device.h"
namespace pixie {
// Forward declares

// Platform_Win32_FileSystem_Device
class Platform_Win32_FileSystem_Device: public Platform_FileSystem_Device
	{
	public:
		Platform_Win32_FileSystem_Device(const char* deviceName);
		virtual ~Platform_Win32_FileSystem_Device();

		virtual const char* GetName() const;

	private:
		char* name_;
	};
}; // namespace pixie
#endif /* __Platform_Win32_FileSystem_Device_H__ */
