/**
 * \file	Main.cpp
 * 
 * \ingroup	pixiepvt
 * \brief	
 * \author	Mattias Gustavsson	
 *  
 *
 */

#include "PixiePVT.h"


//**********************
//*** WINDOWS 32-BIT ***
//**********************

#ifdef WIN32

// Win32 Platform Includes
#include "../Win32/Platform_Win32_OS.h"
#include "../Win32/Platform_Win32_Time.h"
#include "../Win32/Platform_Win32_Sound.h"
#include "../Win32/Platform_Win32_Input.h"
#include "../Win32/Platform_Win32_Screen.h"
#include "../Win32/Platform_Win32_FileSystem.h"
#include "../Win32/Platform_Win32_Network.h"
#include "../Win32/Platform_Win32_3D.h"
#include "StandardLibrary.h"

// Need to have memory-leak checking
#include <crtdbg.h>

//*** WinMain ***

using namespace pixie;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
	// Enable windows memory leak detection (will report leaks in the Output window)
	#ifdef _DEBUG
		int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
		flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
		_CrtSetDbgFlag(flag); // Set flag to the new value

//		_CrtSetBreakAlloc(1624); // Can be manually commented back in to break at a certain allocation
	#endif
	
	// Platform abstraction systems - This is where we choose which platform components we want to use
	char cmdline[4096];
	StrCpy(cmdline,lpCmdLine);
	StrCat(cmdline," -forceddraw");
	Platform_Win32_OS* os=new Platform_Win32_OS(hInstance,cmdline);
	Platform::SetPlatform_OS(os);
//	Platform::SetPlatform_Time(new Platform_Win32_Time());
//	Platform::SetPlatform_Sound(new Platform_Win32_Sound(os));
	Platform::SetPlatform_Input(new Platform_Win32_Input(os));
	Platform::SetPlatform_Screen(new Platform_Win32_Screen(os));
	Platform::SetPlatform_FileSystem(new Platform_Win32_FileSystem());
//	Platform::SetPlatform_Network(new Platform_Win32_Network());  // This system isn't quite done yet - and not needed for this game anyway
//	Platform::SetPlatform_3D(new Platform_Win32_3D(os)); // Don't use both Screen and 3D - only one or the other at a time

	// Start the game
	PixiePVTMain();

	// Return the result (always 0, as this was a controlled shutdown)
	return 0;
	}

#endif /* #ifdef WIN32 */

//**************************
//*** END WINDOWS 32-BIT ***
//**************************
