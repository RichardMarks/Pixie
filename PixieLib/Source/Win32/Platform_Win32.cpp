//*** Platform_Win32.cpp ***

#include "Platform_Win32_OS.h"
#include "Platform_Win32_Time.h"
#include "Platform_Win32_Input.h"
#include "Platform_Win32_FileSystem.h"
#include "Platform_Win32_Screen.h"
#include "Platform_Win32_Network.h"
#include "Platform_Win32_3D.h"
#include "Platform_Win32_Sound.h"


void Platform_Win32()
	{
	Platform_Win32_OS(0,0);
	Platform_Win32_Time();
	Platform_Win32_Input(0);
	Platform_Win32_FileSystem();
	Platform_Win32_Screen(0);
	Platform_Win32_Network();
	Platform_Win32_3D(0);
	Platform_Win32_Sound(0);
	}