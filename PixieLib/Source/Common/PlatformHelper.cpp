/**
 * \file	PlatformHelper.cpp
 * 
 * \ingroup	helpers
 * \brief	
 * \author	Mattias Gustavsson	
 *  
 *
 */

#include "PlatformHelper.h"


//**********************
//*** WINDOWS 32-BIT ***
//**********************

#ifdef WIN32

// Need to have memory-leak checking
#include <crtdbg.h>

//*** DefaultWinMain ***

void DefaultWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
	// Enable windows memory leak detection (will report leaks in the Output window)
	#ifdef _DEBUG
		int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
		flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
		_CrtSetDbgFlag(flag); // Set flag to the new value

//		_CrtSetBreakAlloc(1624); // Can be manually commented back in to break at a certain allocation
	#endif
	
	// Platform abstraction systems - This is where we choose which platform components we want to use
	Platform_Win32_OS* os=new Platform_Win32_OS(hInstance,lpCmdLine);
	Platform::SetPlatform_OS(os);
	Platform::SetPlatform_Time(new Platform_Win32_Time());
	Platform::SetPlatform_Sound(new Platform_Win32_Sound(os));
	Platform::SetPlatform_Input(new Platform_Win32_Input(os));
	Platform::SetPlatform_Screen(new Platform_Win32_Screen(os));
	Platform::SetPlatform_FileSystem(new Platform_Win32_FileSystem());
	Platform::SetPlatform_Network(new Platform_Win32_Network());
//	Platform::SetPlatform_3D(new Platform_Win32_3D(os)); // Don't use both Screen and 3D - only one or the other at a time

	}

//*** DefaultWinMain3D ***

void DefaultWinMain3D(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
	// Enable windows memory leak detection (will report leaks in the Output window)
	#ifdef _DEBUG
		int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
		flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
		_CrtSetDbgFlag(flag); // Set flag to the new value

//		_CrtSetBreakAlloc(1624); // Can be manually commented back in to break at a certain allocation
	#endif
	
	// Platform abstraction systems - This is where we choose which platform components we want to use
	Platform_Win32_OS* os=new Platform_Win32_OS(hInstance,lpCmdLine);
	Platform::SetPlatform_OS(os);
	Platform::SetPlatform_Time(new Platform_Win32_Time());
	Platform::SetPlatform_Sound(new Platform_Win32_Sound(os));
	Platform::SetPlatform_Input(new Platform_Win32_Input(os));
	Platform::SetPlatform_3D(new Platform_Win32_3D(os)); 
	Platform::SetPlatform_FileSystem(new Platform_Win32_FileSystem());
	Platform::SetPlatform_Network(new Platform_Win32_Network());
//	Platform::SetPlatform_Screen(new Platform_Win32_Screen(os)); // Don't use both Screen and 3D - only one or the other at a time
	}

#endif /* #ifdef WIN32 */

//**************************
//*** END WINDOWS 32-BIT ***
//**************************

