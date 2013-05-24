/**
 * \class	Platform_Win32_Input_MouseDevice
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_Win32_Input_MouseDevice_H__
#define __Platform_Win32_Input_MouseDevice_H__

// Includes
#include "Platform_Input_MouseDevice.h"

// Forward declares
struct IDirectInputDeviceA;

// Platform_Win32_Input_MouseDevice
class Platform_Win32_Input_MouseDevice: public Platform_Input_MouseDevice
	{
	public:
		Platform_Win32_Input_MouseDevice(struct HWND__* windowHandle);
		virtual ~Platform_Win32_Input_MouseDevice();

		virtual void GetPosition(float& x, float& y) const;
		virtual bool IsLeftButtonDown() const;
		virtual bool IsRightButtonDown() const;

	private:
		struct HWND__* windowHandle_;
	};

#endif /* __Platform_Win32_Input_MouseDevice_H__ */
