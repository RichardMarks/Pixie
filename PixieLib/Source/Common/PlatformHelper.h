/**
 * \file	PlatformHelper.h
 * 
 * \ingroup	helpers
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __PlatformHelper_H__
#define __PlatformHelper_H__


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

void DefaultWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

void DefaultWinMain3D(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

#endif /* #ifdef WIN32 */

//**************************
//*** END WINDOWS 32-BIT ***
//**************************

#endif /* __PlatformHelper_H__ */
