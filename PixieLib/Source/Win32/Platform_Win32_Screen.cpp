//*** Platform_Win32_Screen.cpp ***

#include "Platform_Win32_Screen.h"
#include "Platform_Win32_Screen_Technology.h"
#include "Platform_Win32_Screen_D3D9.h"
#include "Platform_Win32_Screen_DDraw.h"
#include "Platform_Win32_Screen_GDI.h"
#include "Platform_Win32_OS.h"
#include "Platform_Time.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <string.h>
#include <malloc.h>
#include <stdio.h>


//*** Constructor ***

Platform_Win32_Screen::Platform_Win32_Screen(Platform_Win32_OS* os):
	windowHandle_(os->GetWindowHandle()),
	technology_(Technology_Undefined),
	technologyInstance_(0),
	fullscreen_(true),
	fullscreenWidth_(0),
	fullscreenHeight_(0),
	showfps_(false),
	interpolationMode_(true),
	disableOnWmSize_(0),
	ignoreNextOnWmSize_(false),
	firstTimeInitializeCalled_(false),
	forceddraw_(false),
	forcegdi_(false)
	{
	Platform::RegisterEventListener(this);

	// Check commandline flags
	if (os->GetCommandLineString())
		{
		char* cmdline=strdup(os->GetCommandLineString());
		char* token=strtok(cmdline," ");
		while (token)
			{
			if (stricmp(token,"-window")==0)
				{
				fullscreen_=false;
				}
			if (stricmp(token,"-showfps")==0)
				{
				showfps_=true;
				}
			if (stricmp(token,"-forceddraw")==0)
				{
				forceddraw_=true;
				}
			if (stricmp(token,"-forcegdi")==0)
				{
				forcegdi_=true;
				}

			token=strtok(0," ");
			}
		free(cmdline);
		}

	// Determine default screen size
	HWND desktopWindow=GetDesktopWindow();
	RECT desktopRect;
	BOOL result=GetWindowRect(desktopWindow,&desktopRect);
	if (result)
		{
		fullscreenWidth_=desktopRect.right-desktopRect.left;
		fullscreenHeight_=desktopRect.bottom-desktopRect.top;

		windowedWidth_=fullscreenWidth_-fullscreenWidth_/6;
		windowedHeight_=fullscreenHeight_-fullscreenHeight_/6; 
		windowedX_=fullscreenWidth_-windowedWidth_;
		windowedY_=fullscreenHeight_-windowedHeight_;
		windowedX_/=2;
		windowedY_/=2;
		}

	

	}


//*** Destructor ***

Platform_Win32_Screen::~Platform_Win32_Screen()
	{
	disableOnWmSize_++;

	if (technologyInstance_)
		{
		delete technologyInstance_;
		}

	disableOnWmSize_--;

	Platform::UnregisterEventListener(this);
	}


//*** FirstTimeInitialize ***

void Platform_Win32_Screen::FirstTimeInitialize()
	{
	disableOnWmSize_++;


	// Initialize window size
	if (!GetFullscreen())
		{
		SetWindowSize();
		}

	if (forceddraw_ || IsRunningOnWine())
		{
		SetTechnology(Technology_DDraw);
		}
	else if (forcegdi_)
		{
		SetTechnology(Technology_GDI);
		}
	else
		{
		SetTechnology(Technology_D3D9); 
		}	

	// Show window
	ShowWindow(windowHandle_,SW_SHOW);

	disableOnWmSize_--;
	}

//*** Present ***

void Platform_Win32_Screen::Present(unsigned short* bitmapData, int bitmapWidth, int bitmapHeight, unsigned short modulate, unsigned short backgroundColor)
	{
	if (!firstTimeInitializeCalled_)
		{
		FirstTimeInitialize();
		firstTimeInitializeCalled_=true;
		}

	if (!technologyInstance_)
		{
		return;
		}
	
	// Framerate counter
	if (showfps_ && Platform::GetPlatform_Time())
		{

		static float previousTime=Platform::GetPlatform_Time()->GetTime();
		static float accumulatedTime=0;
		static int currentFrameCount=0;
		static int frames=0;
		float newTime=Platform::GetPlatform_Time()->GetTime();
		float deltaTime=newTime-previousTime;
		accumulatedTime+=deltaTime;
		frames++;
		if (accumulatedTime>=1)
			{
			accumulatedTime-=1;
			currentFrameCount=frames;
			frames=0;
			}
		previousTime=newTime;
		static char fps[20];
		_snprintf(fps,20,"%02d",currentFrameCount);
		unsigned short color=0xffff; // White
		if (technology_==Technology_DDraw)
			{
			color=0xf81f; // Magenta
			}
		if (technology_==Technology_GDI)
			{
			color=0x07ff; // Cyan
			}
		DebugText(bitmapData,bitmapWidth,bitmapHeight,6,6,fps,0);
		DebugText(bitmapData,bitmapWidth,bitmapHeight,5,5,fps,color);

		}


	bool result=technologyInstance_->Present(bitmapData,bitmapWidth,bitmapHeight,modulate,backgroundColor);
	if (!result)
		{
		DowngradeTechnology();
		Present(bitmapData,bitmapWidth,bitmapHeight,modulate,backgroundColor);
		}
	}


//*** Present ***

void Platform_Win32_Screen::Present(unsigned int* bitmapData, int bitmapWidth, int bitmapHeight, unsigned int modulate, unsigned int backgroundColor)
	{
	if (!firstTimeInitializeCalled_)
		{
		FirstTimeInitialize();
		firstTimeInitializeCalled_=true;
		}

	if (!technologyInstance_)
		{
		return;
		}
	
	// Framerate counter
	if (showfps_ && Platform::GetPlatform_Time())
		{

		static float previousTime=Platform::GetPlatform_Time()->GetTime();
		static float accumulatedTime=0;
		static int currentFrameCount=0;
		static int frames=0;
		float newTime=Platform::GetPlatform_Time()->GetTime();
		float deltaTime=newTime-previousTime;
		accumulatedTime+=deltaTime;
		frames++;
		if (accumulatedTime>=1)
			{
			accumulatedTime-=1;
			currentFrameCount=frames;
			frames=0;
			}
		previousTime=newTime;
		static char fps[20];
		_snprintf(fps,20,"%02d",currentFrameCount);
		unsigned int color=0xffffffff; // White
		if (technology_==Technology_DDraw)
			{
			color=0xffff00ff; // Magenta
			}
		if (technology_==Technology_GDI)
			{
			color=0xff00ffff; // Cyan
			}
		DebugText(bitmapData,bitmapWidth,bitmapHeight,6,6,fps,0);
		DebugText(bitmapData,bitmapWidth,bitmapHeight,5,5,fps,color);

		}


	bool result=technologyInstance_->Present(bitmapData,bitmapWidth,bitmapHeight,modulate,backgroundColor);
	if (!result)
		{
		DowngradeTechnology();
		Present(bitmapData,bitmapWidth,bitmapHeight,modulate,backgroundColor);
		}
	}


//*** SetTechnology ***

void Platform_Win32_Screen::SetTechnology(Platform_Win32_Screen::Technology technology)
	{
	disableOnWmSize_++;

	if (technologyInstance_)
		{
		delete technologyInstance_;
		technologyInstance_=0;
		}

	technology_=technology;

	if (technology_>=Technology_Undefined)
		{
		return;
		}
	
	switch(technology_)
		{
		case Technology_D3D9:
			{
			technologyInstance_=new Platform_Win32_Screen_D3D9(windowHandle_, GetFullscreen(), GetWidth(), GetHeight(), GetInterpolationMode());
			} break;

		case Technology_DDraw:
			{
			technologyInstance_=new Platform_Win32_Screen_DDraw(windowHandle_, GetFullscreen(), GetWidth(), GetHeight());
			technologyInstance_->SetInterpolationMode(GetInterpolationMode());
			} break;

		case Technology_GDI:
			{
			technologyInstance_=new Platform_Win32_Screen_GDI(windowHandle_, GetFullscreen(), GetWidth(), GetHeight());
			technologyInstance_->SetInterpolationMode(GetInterpolationMode());
			} break;
		}

	if (!technologyInstance_->Setup())
		{
		DowngradeTechnology();
		}

	disableOnWmSize_--;
	}


//*** DowngradeTechnology ***

void Platform_Win32_Screen::DowngradeTechnology()
	{
	disableOnWmSize_++;

	Platform::GetPlatform_OS()->OutputDebugText("Method failed for technology %d, falling back on technology %d\n",technology_,technology_+1);
	
	if (technologyInstance_)
		{
		delete technologyInstance_;
		technologyInstance_=0;
		}

	Technology newTechnology=(Technology)(technology_+1);
	
	if (newTechnology<Technology_Undefined)
		{
		SetTechnology(newTechnology);
		disableOnWmSize_--;
		return;
		}
	
	technology_=Technology_Undefined;
	technologyInstance_=0;

	disableOnWmSize_--;
	}


//*** SetInterpolationMode *** 

void Platform_Win32_Screen::SetInterpolationMode(bool enabled)
	{
	if (interpolationMode_==enabled)
		{
		return;
		}

	interpolationMode_=enabled;

	if (technologyInstance_)
		{
		technologyInstance_->SetInterpolationMode(interpolationMode_);
		}
	}


//*** GetInterpolationMode *** 

bool Platform_Win32_Screen::GetInterpolationMode()
	{
	return interpolationMode_;
	}


//*** SetFullscreen ***

void Platform_Win32_Screen::SetFullscreen(bool fullscreen)
	{
	if (fullscreen_==fullscreen)
		{
		return;
		}

	fullscreen_=fullscreen;
	
	if (technologyInstance_)
		{
		disableOnWmSize_++;

		delete technologyInstance_;
		technologyInstance_=0;
	
		if (!GetFullscreen())
			{
			SetWindowSize();
			}

		SetTechnology(technology_);

		if (technology_==Technology_DDraw && !GetFullscreen())
			{
			SetWindowSize();
			}

		disableOnWmSize_--;
		}
	}


//*** GetFullscreen ***

bool Platform_Win32_Screen::GetFullscreen()
	{
	return fullscreen_;
	}

		
//*** SetSize ***

void Platform_Win32_Screen::SetSize(int width, int height)
	{
	windowedWidth_=width;
	windowedHeight_=height;

	if (technologyInstance_)
		{
		disableOnWmSize_++;
	
		delete technologyInstance_;
		technologyInstance_=0;

		if (!GetFullscreen())	
			{
			SetWindowSize();
			}	

		SetTechnology(technology_);

		disableOnWmSize_--;

		}

	}


//*** GetWidth ***

int Platform_Win32_Screen::GetWidth()
	{
	if (fullscreen_)
		{
		return fullscreenWidth_;
		}
	
	return windowedWidth_;
	}


//*** GetHeight ***

int Platform_Win32_Screen::GetHeight()
	{
	if (fullscreen_)
		{
		return fullscreenHeight_;
		}
	
	return windowedHeight_;
	}


//*** TransformCursorCoordinates ***

void Platform_Win32_Screen::TransformCursorCoordinates(float& x, float& y)
	{
	if (technologyInstance_)
		{
		technologyInstance_->TransformCursorCoordinates(x,y);
		}
	}


//*** OnCustomEvent ***

void Platform_Win32_Screen::OnCustomEvent(const char* eventId, void* userData)
	{
	if (stricmp(eventId,"OnGainFocus")==0)
		{
		OnGainFocus();
		return;
		}

	if (stricmp(eventId,"OnLoseFocus")==0)
		{
		OnLoseFocus();
		return;
		}

	if (stricmp(eventId,"OnRestore")==0)
		{
		OnRestore();
		return;
		}

	if (stricmp(eventId,"OnMinimize")==0)
		{
		OnMinimize();
		return;
		}

	if (stricmp(eventId,"OnWmSize")==0)
		{
		int width=*(static_cast<int*>(userData));
		int height=*((static_cast<int*>(userData))+1);
		OnWmSize(width,height);
		return;
		}
	}


//*** OnRestore ***

void Platform_Win32_Screen::OnRestore()
	{
	ignoreNextOnWmSize_=true;
	}


//*** OnWmSize ***

void Platform_Win32_Screen::OnWmSize(int width, int height)
	{
	if (disableOnWmSize_ || fullscreen_ || ignoreNextOnWmSize_)
		{
		Platform::GetPlatform_OS()->OutputDebugText("OnWmSize(%d,%d) - DISABLED\n",width,height);
		ignoreNextOnWmSize_=false;
		return;
		}

	disableOnWmSize_++;

	Platform::GetPlatform_OS()->OutputDebugText("OnWmSize(%d,%d) fullscreen_=%d\n",width,height,fullscreen_);

	if (technology_==Technology_DDraw && !technologyInstance_)
		{
		SetTechnology(technology_);
		}
	
	windowedWidth_=width;
	windowedHeight_=height;

	if (!fullscreen_ && technologyInstance_)
		{
		delete technologyInstance_;
		technologyInstance_=0;

		SetTechnology(technology_);
		}

	disableOnWmSize_--;
	}



//*** OnMinimize ***

void Platform_Win32_Screen::OnMinimize()
	{
/*	DebugPrint(("OnMinimize\n"));
	if (technologyInstance_)
		{
		delete technologyInstance_;
		technologyInstance_=0;
		}
*/	}


//*** OnLoseFocus ***

void Platform_Win32_Screen::OnLoseFocus()
	{
	disableOnWmSize_++;

	if (fullscreen_ && technologyInstance_)
		{
		disableOnWmSize_++;
		Platform::GetPlatform_OS()->OutputDebugText("OnLoseFocus\n");
		delete technologyInstance_;
		technologyInstance_=0;
		disableOnWmSize_--;
		}
	}


//*** OnGainFocus ***

void Platform_Win32_Screen::OnGainFocus()
	{
	if (fullscreen_ && !technologyInstance_)
		{
		disableOnWmSize_++;
		Platform::GetPlatform_OS()->OutputDebugText("OnGainFocus\n");
		SetTechnology(technology_);
		disableOnWmSize_--;
		}

	disableOnWmSize_--;
	}


//*** SetWindowSize ***

void Platform_Win32_Screen::SetWindowSize()
	{
	Platform::GetPlatform_OS()->OutputDebugText("SetWindowSize\n");

	// Get the whole window area
	RECT rect1;
	BOOL ret=GetWindowRect(windowHandle_,&rect1);
	rect1.right-=rect1.left;
	rect1.bottom-=rect1.top;

	// Get the client area
	RECT rect2;
	ret=GetClientRect(windowHandle_,&rect2);
	rect2.right-=rect2.left;
	rect2.bottom-=rect2.top;

	// Calculate the size of the windows borders/title bar etc.
	int sx=rect1.right-rect2.right;
	int sy=rect1.bottom-rect2.bottom;

	// Set the window to the required dimensions
	MoveWindow(windowHandle_,windowedX_,windowedY_,windowedWidth_+sx,windowedHeight_+sy,TRUE);
	}


//*** IsRunningOnWine ***

bool Platform_Win32_Screen::IsRunningOnWine()
	{
    HMODULE module=LoadLibrary("ntdll");
    if (!module)
		{
        return false;
		}

	FARPROC proc=GetProcAddress(module, "wine_get_unix_file_name");
    FreeLibrary(module);

    return proc!=0;
	}


//*** DebugText ***

void Platform_Win32_Screen::DebugText(unsigned short* colorData, int hPitch, int vPitch, int x, int y, const char* text, unsigned short color)
	{
	// Draw text
	int xp=x;
	for (unsigned int i=0; i<strlen(text); i++)
		{
		char c=text[i];
		if (c>=32 && c<=192)
			{
			c-=32;

			// Source coordinates
			int sx=((c%14)*9);
			int sy=((c/14)*16);

			// Destination coordinates
			int dx=xp;
			int dy=y;

			// Render
			BlitCharacter(colorData,hPitch,vPitch,sx,sy,dx,dy,color);

			// Increase draw position
			xp=xp+8;
			}
		}
	}


//*** BlitCharacter ***

void Platform_Win32_Screen::BlitCharacter(unsigned short* colorData, int hPitch, int vPitch, int sx, int sy, int dx, int dy, unsigned short color)
	{
	for (int y=0; y<16; y++)
		{
		for (int x=0; x<8; x++)
			{
			int u=(sx+x)/32;
			int l=(sx+x)-u*32;
			unsigned int h=DebugFontData[u+(sy+y)*4];
			if (h & (1<<l))
				{
				int xp=dx+x;
				int yp=dy+y;
				if (xp>=0 && xp<hPitch && yp>=0 && yp<vPitch)
					{
					colorData[xp+yp*hPitch]=color;
					}
				}
			}
		}	
	}

//*** DebugText ***

void Platform_Win32_Screen::DebugText(unsigned int* colorData, int hPitch, int vPitch, int x, int y, const char* text, unsigned int color)
	{
	// Draw text
	int xp=x;
	for (unsigned int i=0; i<strlen(text); i++)
		{
		char c=text[i];
		if (c>=32 && c<=192)
			{
			c-=32;

			// Source coordinates
			int sx=((c%14)*9);
			int sy=((c/14)*16);

			// Destination coordinates
			int dx=xp;
			int dy=y;

			// Render
			BlitCharacter(colorData,hPitch,vPitch,sx,sy,dx,dy,color);

			// Increase draw position
			xp=xp+8;
			}
		}
	}


//*** BlitCharacter ***

void Platform_Win32_Screen::BlitCharacter(unsigned int* colorData, int hPitch, int vPitch, int sx, int sy, int dx, int dy, unsigned int color)
	{
	for (int y=0; y<16; y++)
		{
		for (int x=0; x<8; x++)
			{
			int u=(sx+x)/32;
			int l=(sx+x)-u*32;
			unsigned int h=DebugFontData[u+(sy+y)*4];
			if (h & (1<<l))
				{
				int xp=dx+x;
				int yp=dy+y;
				if (xp>=0 && xp<hPitch && yp>=0 && yp<vPitch)
					{
					colorData[xp+yp*hPitch]=color;
					}
				}
			}
		}	
	}

//*** DebugFontData ***

unsigned int Platform_Win32_Screen::DebugFontData[] = 
	{
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000180,0x00000000,0x00000000,0x00000000,0x0001c180,0x00000000,0x00000000,0x61983000,0x070363c3,0x0018300c,0x00000000,
	0x61987800,0x0d8b6663,0x0030180c,0x00000000,0xf1987800,0x0d8dc067,0xb030180c,0x000000c1,0x60007800,0x070600c3,0xe0600c00,0x000000c0,0x60003000,0x01830183,0xf8600c00,0x0fc003f3,
	0x60003000,0x3d818303,0xe0600c00,0x000000c0,0xf0000000,0x198ec607,0xb0600c00,0x000000c1,0x60003000,0x199b4663,0x00600c00,0x00038000,0x60003000,0x371b03c3,0x00301800,0x00038000,
	0x00000000,0x000e0180,0x00301800,0x00030000,0x00000000,0x00000180,0x00183000,0x00018000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x81e0c000,0x030783c1,0xf0fc383f,0x000001e0,
	0xc330c000,0x030cc661,0x98c01803,0x00000331,0xf3b06000,0x1b0cc661,0x98600c03,0x07038331,0x83b06000,0x1b0c0601,0xb8603e03,0x07038331,0x83303000,0x1b070301,0xf030661f,0x00000330,
	0x83703000,0x198c0181,0xd8306630,0x000003e1,0x83701800,0x3f8cc0c1,0x98186630,0x00000181,0x83301838,0x180cc061,0x98186618,0x070380c1,0x81e00c38,0x180787e1,0xf0183c0f,0x070380e0,
	0x00000c00,0x00000000,0x00000000,0x06000000,0x00000000,0x00000000,0x00000000,0x03000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0xe0180060,0x0f8307e1,0xf8fc1e1e,0x078661e1,
	0x30300030,0x19878c33,0x180c3633,0x03066330,0x30600018,0x198ccc33,0x180c6633,0x03066330,0x80c0fc0c,0x198ccf31,0x180c6603,0x03066030,0xc1800006,0x0f8ccdb0,0xf87c6603,0x0307e030,
	0xc0c0fc0c,0x198fcdb0,0x180c6603,0x030663b0,0x00600018,0x198ccf30,0x180c6633,0x03066330,0xc0300030,0x198cc030,0x180c3633,0x03066330,0xc0180060,0x0f8ccfe0,0x18fc1e1e,0x078663e0,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x3018cc60,0x0f878c66,0xf8783e1e,0x18c66331,
	0x3018cc60,0x198ccc66,0x60cc6633,0x18c66330,0x70186c60,0x198ccce7,0x600c6633,0x18c66330,0xb0186c60,0x198ccde6,0x60186633,0x1ac66330,0xb0183c60,0x0f8ccf66,0x60303e33,0x1ac66330,
	0xb0186c60,0x018cce66,0x60603633,0x1ac66330,0x30186c66,0x018ccc66,0x60c06633,0x0d866330,0x3018cc66,0x018ccc66,0x60cc6633,0x0d83c330,0x31f8cc3c,0x01878c66,0x6078661e,0x0d8181e0,
	0x00000000,0x00000000,0x00000018,0x00000000,0x00000000,0x00000000,0x00000030,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x06000000,0x00001c00,0x00000000,0x00000000,0x0f000000,0x00001800,0x00000000,0xe1f8cc66,0x19878061,0x18003000,0x00060000,
	0x6180cc66,0x00060060,0x18000000,0x00060000,0x6180cc2c,0x000600c0,0xf8780000,0x0787c1e0,0x60c0cc18,0x000600c0,0x98c00000,0x0cc66331,0x60607818,0x00060180,0x98c00000,0x0cc66031,
	0x60303034,0x00060180,0x98f80000,0x0fc66031,0x60183066,0x00060300,0x98cc0000,0x00c66031,0x60183066,0x00060300,0x98cc0000,0x00c66331,0x61f83066,0x00060600,0xf8f80000,0x0787c1e0,
	0x60000000,0x00060600,0x00000000,0x00000000,0x60000000,0x00060000,0x00000000,0x00000000,0xe0000000,0x80078001,0x0000007f,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0xc0000000,0x00000300,0x00000000,0x00000000,0xc0180078,0x0780c300,0x00000000,0x00000000,
	0x0018000c,0x0600c000,0x00000000,0x00000000,0xf0f8f80c,0x060cc3c0,0xf8783e3f,0x0f8663e0,0xc198cc0c,0x060cc300,0x98cc666b,0x00c76331,0xc198cc7e,0x0606c300,0x98cc666b,0x00c0e331,
	0xc198cc0c,0x0603c300,0x98cc666b,0x07806331,0xc198cc0c,0x0606c300,0x98cc666b,0x0c006331,0xc198cc0c,0x060cc300,0x98cc666b,0x0c006331,0xf198f80c,0x1f8cc303,0xf8786663,0x07c063e0,
	0x0000c000,0x00000300,0x18000000,0x00000300,0x0000c000,0x00000300,0x18000000,0x00000300,0x00007c00,0x000001e0,0x18000000,0x00000300,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x0000000c,0x00000000,0x38181818,0x000003f2,
	0x0000000c,0x00000000,0x6c30180c,0x000003f3,0x3198cc7e,0x1f8cc666,0xc430180c,0x000003f1,0xb198cc0c,0x180cc666,0x0030180c,0x000003f0,0xb198cc0c,0x0c0cc3c6,0x00601806,0x000003f0,
	0xb198cc0c,0x060cc186,0x00c01803,0x000003f0,0xb198cc0c,0x030cc3c6,0x00601806,0x000003f0,0x60f0cc0c,0x018cc663,0x0030180c,0x000003f0,0x6060f878,0x1f878663,0x0030180c,0x000003f0,
	0x00000000,0x00060000,0x0030180c,0x00000000,0x00000000,0x00030000,0x00181818,0x00000000,0x00000000,0x0001e000,0x00001800,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	};
