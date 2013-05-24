//*** Platform_Win32_Input ***

#include "Platform_Win32_Input.h"
#include "Platform_Win32_Input_KeyboardDevice.h"
#include "Platform_Win32_Input_MouseDevice.h"
#include "Platform_Win32_OS.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN

#define DIRECTINPUT_VERSION 0x0300
#include <dinput.h>


//*** Constructor ***

Platform_Win32_Input::Platform_Win32_Input(Platform_Win32_OS* os):
	windowHandle_(os->GetWindowHandle()),
	keyboardDevice_(0),
	mouseDevice_(0),
	cursorCount_(0)
	{
	Platform::RegisterEventListener(this);
	defaultCursor_=LoadCursor(NULL, IDC_ARROW); 
	}


//*** Destructor ***

Platform_Win32_Input::~Platform_Win32_Input()
	{
	if (keyboardDevice_)
		{
		delete keyboardDevice_;
		keyboardDevice_=0;
		}

	if (mouseDevice_)
		{
		delete mouseDevice_;
		mouseDevice_=0;
		}
	Platform::UnregisterEventListener(this);

	for (int i=0; i<cursorCount_; i++)
		{
		DeleteObject(cursors_[i]);
		}
	DeleteObject(defaultCursor_);
	}


//*** OnCustomEvent ***

void Platform_Win32_Input::OnCustomEvent(const char* eventId,void* userData)
	{
	if (stricmp(eventId,"OnWmChar")==0)
		{
		int ascii=*(static_cast<int*>(userData));
		int keycode=*((static_cast<int*>(userData))+1);
		if (keyboardDevice_)
			{
			keyboardDevice_->SetCharPressed((unsigned char)ascii,(unsigned char)keycode);
			}
		return;
		}

	if (stricmp(eventId,"OnWmKeyUp")==0)
		{
		int keycode=*(static_cast<int*>(userData));
		if (keyboardDevice_)
			{
			keyboardDevice_->SetCharReleased((unsigned char)keycode);
			}
		return;
		}
	}


//*** GetKeyboardDevice ***

const Platform_Input_KeyboardDevice* Platform_Win32_Input::GetKeyboardDevice()
	{
	if (!keyboardDevice_)
		{
		keyboardDevice_=new Platform_Win32_Input_KeyboardDevice();
		}

	return keyboardDevice_;
	}


//*** GetMouseDevice ***

const Platform_Input_MouseDevice* Platform_Win32_Input::GetMouseDevice()
	{
	if (!mouseDevice_)
		{
		mouseDevice_=new Platform_Win32_Input_MouseDevice(windowHandle_);
		}

	return mouseDevice_;
	}



//*** CreateMouseCursor ***

int Platform_Win32_Input::CreateMouseCursor(int width, int height, int hotspotX, int hotspotY, unsigned short* colorData, unsigned char* alphaData)
	{
	if (cursorCount_>=MaxCursors)
		{
		return 0;
		}

	//Create the AND and XOR masks for the bitmap

	HDC hDC					= ::GetDC(NULL);
	HDC hAndMaskDC			= ::CreateCompatibleDC(hDC); 
	HDC hXorMaskDC			= ::CreateCompatibleDC(hDC); 
	
	HBITMAP hAndMask = ::CreateCompatibleBitmap(hDC,width,height);
	HBITMAP hXorMask = ::CreateCompatibleBitmap(hDC,width,height);

	//Select the bitmaps to DC
	HBITMAP hOldAndMaskBitmap	= (HBITMAP)::SelectObject(hAndMaskDC,hAndMask);
	HBITMAP hOldXorMaskBitmap	= (HBITMAP)::SelectObject(hXorMaskDC,hXorMask);

	//Scan each pixel of the source bitmap and create the masks
	for (int y=0; y<height; y++)
		{
		for (int x=0; x<width; x++)
			{
			unsigned short c=colorData[x+y*width];
			unsigned char r=(unsigned char)((c & 0xf800)>>8);
			unsigned char g=(unsigned char)((c & 0x07e0)>>3);
			unsigned char b=(unsigned char)((c & 0x001f)<<3);
			unsigned char a=alphaData[x+y*width];

			if (a<128)
				{
				::SetPixel(hAndMaskDC,x,y,RGB(255,255,255));
				::SetPixel(hXorMaskDC,x,y,RGB(0,0,0));
				}
			else
				{
				::SetPixel(hAndMaskDC,x,y,RGB(0,0,0));
				::SetPixel(hXorMaskDC,x,y,RGB(r,g,b));
				}

			}
		}
	
	::SelectObject(hAndMaskDC,hOldAndMaskBitmap);
	::SelectObject(hXorMaskDC,hOldXorMaskBitmap);

	::DeleteDC(hXorMaskDC);
	::DeleteDC(hAndMaskDC);

	::ReleaseDC(NULL,hDC);


	
	//Create the cursor using the masks and the hotspot values provided
	ICONINFO iconinfo = {0};
	iconinfo.fIcon		= FALSE;
	iconinfo.xHotspot	= hotspotX;
	iconinfo.yHotspot	= hotspotY;
	iconinfo.hbmMask	= hAndMask;
	iconinfo.hbmColor	= hXorMask;

	cursors_[cursorCount_] = CreateIconIndirect(&iconinfo);
	cursorCount_++;

	return cursorCount_;
	}


//*** SetMouseCursor ***

void Platform_Win32_Input::SetMouseCursor(int handle)
	{
	if (handle<=0 || handle>cursorCount_)
		{
		SetCursor(0);
		Platform::SendEvent_CustomEvent("SetCursor",0);
		}
	else
		{
		SetCursor(cursors_[handle-1]);
		Platform::SendEvent_CustomEvent("SetCursor",cursors_[handle-1]);
		}
	}


//*** SetDefaultMouseCursor ***

void Platform_Win32_Input::SetDefaultMouseCursor()
	{
	SetCursor(defaultCursor_);
	Platform::SendEvent_CustomEvent("SetCursor",defaultCursor_);
	}

