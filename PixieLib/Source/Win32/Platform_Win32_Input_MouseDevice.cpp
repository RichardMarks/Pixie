//*** Platform_Win32_Input_MouseDevice.cpp ***

#include "Platform_Win32_Input_MouseDevice.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <windows.h>

#include "Platform_Win32_Input.h"
#include "Platform_OS.h"

//*** Constructor ***

Platform_Win32_Input_MouseDevice::Platform_Win32_Input_MouseDevice(struct HWND__* windowHandle):
	windowHandle_(windowHandle)
	{
	}


//*** Destructor ***

Platform_Win32_Input_MouseDevice::~Platform_Win32_Input_MouseDevice()
	{
	}



//*** GetPosition ***

void Platform_Win32_Input_MouseDevice::GetPosition(float& x, float& y) const
	{
	// Retrieve new mouse position
	POINT point;
	point.x=0;
	point.y=0;
	BOOL ret=GetCursorPos(&point);
	if (!ret)
		{
		Platform::GetPlatform_OS()->OutputDebugText("Couldn't get mouse cursor\n");
		}

	ScreenToClient(windowHandle_, &point);
	x=(float)point.x;
	y=(float)point.y;
	}


//*** IsLeftButtonDown ***

bool Platform_Win32_Input_MouseDevice::IsLeftButtonDown() const
	{
	if ((Platform::GetPlatform_OS() && Platform::GetPlatform_OS()->HasFocus()) && GetAsyncKeyState(0x01)!=0)
		{
		return true;
		}

	return false;
	}


//*** IsRightButtonDown ***

bool Platform_Win32_Input_MouseDevice::IsRightButtonDown() const
	{
	if ((Platform::GetPlatform_OS() && Platform::GetPlatform_OS()->HasFocus()) && GetAsyncKeyState(0x02)!=0)
		{
		return true;
		}

	return false;
	}

