/**
 * \class	Platform_Win32_Input
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_Win32_Input_H__
#define __Platform_Win32_Input_H__

// Includes
#include "Platform_Input.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <windows.h>

// Forward declares
class Platform_Win32_OS;
class Platform_Win32_Input_KeyboardDevice;
class Platform_Win32_Input_MouseDevice;
struct IDirectInputA;

// Platform_Win32_Input
class Platform_Win32_Input:public Platform_Input,PlatformEventListener
	{

	public:
		Platform_Win32_Input(Platform_Win32_OS* os);
		virtual ~Platform_Win32_Input();

		virtual const Platform_Input_KeyboardDevice* GetKeyboardDevice();
		virtual const Platform_Input_MouseDevice* GetMouseDevice();

		virtual int CreateMouseCursor(int width, int height, int hotspotX, int hotspotY, unsigned short* colorData, unsigned char* alphaData);

		virtual void SetMouseCursor(int handle);

		virtual void SetDefaultMouseCursor();

		virtual void OnCustomEvent(const char* eventId,void* userData);

	private:
		struct HWND__* windowHandle_;
		Platform_Win32_Input_KeyboardDevice* keyboardDevice_; 
		Platform_Win32_Input_MouseDevice* mouseDevice_; 
		
		static const int MaxCursors = 256;
		int cursorCount_;
		HCURSOR cursors_[MaxCursors];

		HCURSOR defaultCursor_;
	};

#endif /* __Platform_Win32_Input_H__ */
