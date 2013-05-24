//*** Platform_NULL.cpp ***

#include "Platform_NULL_OS.h"
#include "Platform_NULL_Time.h"
#include "Platform_NULL_Input.h"
#include "Platform_NULL_FileSystem.h"
#include "Platform_NULL_Screen.h"
//#include "Platform_NULL_Network.h"
//#include "Platform_NULL_3D.h"
#include "Platform_NULL_Sound.h"


void Platform_NULL()
	{
	Platform_NULL_OS();
	Platform_NULL_Time();
	Platform_NULL_Input();
	Platform_NULL_FileSystem();
	Platform_NULL_Screen();
//	Platform_NULL_Network();
//	Platform_NULL_3D();
	Platform_NULL_Sound();
	}