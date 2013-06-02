//*** Platform_Win32_Screen_DDraw.cpp ***

#include "Platform_Win32_Screen_DDraw.h"


#include "Platform_OS.h"
#define Assert(expression,message) if (!(expression))  Platform::GetPlatform_OS()->OutputDebugText(message);
#define DebugPrint(x) Platform::GetPlatform_OS()->OutputDebugText x;

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#define DIRECTDRAW_VERSION 0x0300
#include <ddraw.h>

#pragma comment (lib, "dxguid.lib")

namespace pixie {
//*** Constructor ***

Platform_Win32_Screen_DDraw::Platform_Win32_Screen_DDraw(struct HWND__* windowHandle, bool fullscreen, int screenWidth, int screenHeight):
	windowHandle_(windowHandle),
	interpolationMode_(true),
	fullscreen_(fullscreen),
	screenWidth_(screenWidth),
	screenHeight_(screenHeight),
	ddrawDLL_(0),
	directDraw_(0),
	frontBuffer_(0),
	backBuffer_(0),
	clipper_(0),
	canUseAsyncBlt_(false),
	pixelFormat16_(true),
	previousPresentColor_(0),
	previousPresentWidth_(-1),
	previousPresentHeight_(-1),
	previousPresent_AppliedToBackBuffer_(false),
	lastPresentWidth_(0),
	lastPresentHeight_(0)
	{
	}


//*** Setup ***

bool Platform_Win32_Screen_DDraw::Setup()
	{
	// Create DirectDraw object
	ddrawDLL_=LoadLibrary("ddraw.dll");
	if (!ddrawDLL_)
		{
		DebugPrint(("Couldn't find ddraw.dll\n"));
		return false;
		}

	// Create DirectDraw object
	typedef HRESULT (WINAPI *DirectDrawCreateDefinition)( GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter );
	DirectDrawCreateDefinition DirectDrawCreate=(DirectDrawCreateDefinition)GetProcAddress((HMODULE)ddrawDLL_, "DirectDrawCreate");
	if (DirectDrawCreate==0)
		{
		DebugPrint(("Failed to find function entry point in ddraw.dll\n"));
		return false;
		}

	IDirectDraw* ddrawObject=0;
	HRESULT ddrval = DirectDrawCreate( 0, &ddrawObject, 0 );
	if (FAILED(ddrval))
		{
		DebugPrint(("Failed to create DirectDraw object\n"));
		return false;
		}

	ddrval=ddrawObject->QueryInterface(IID_IDirectDraw2,reinterpret_cast<void**>(&directDraw_));
	ddrawObject->Release();
	if (FAILED(ddrval))
		{
		DebugPrint(("Failed to get DirectDraw2 object through QueryInterface\n"));
		return false;
		}


	if (fullscreen_)
		{
		SetWindowLong(windowHandle_,GWL_STYLE,WS_POPUPWINDOW|WS_VISIBLE);

		// Set cooperative level
		ddrval = directDraw_->SetCooperativeLevel(windowHandle_,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
		if (FAILED(ddrval))
			{
			DebugPrint(("Failed to set cooperative level\n"));
			return false;
			}

		// Set display mode to 16 bits, but keep the same screen resolution
		ddrval = directDraw_->SetDisplayMode(screenWidth_, screenHeight_, 16, 60, 0);
		if (FAILED(ddrval))
			{
			DebugPrint(("Failed to set display mode\n"));
			return false;
			}
		}
	else
		{
		// Set cooperative level
		ddrval = directDraw_->SetCooperativeLevel(windowHandle_,DDSCL_NORMAL);
		if (FAILED(ddrval))
			{
			DebugPrint(("Failed to set cooperative level\n"));
			return false;
			}
		}

	// Get pixelformat of screen
	DDSURFACEDESC displayMode;
	memset(&displayMode,0,sizeof(DDSURFACEDESC));
	displayMode.dwSize=sizeof(DDSURFACEDESC);
	ddrval=directDraw_->GetDisplayMode(&displayMode);
	if (FAILED(ddrval))
		{
		DebugPrint(("Failed to get display mode\n"));
		return false;
		}

	// Check that pixel format is supported
	if ((displayMode.ddpfPixelFormat.dwFlags & DDPF_RGB) && displayMode.ddpfPixelFormat.dwRGBBitCount==32)
		{
		pixelFormat16_=false;
		DebugPrint(("32 bit color mode\n"));
		if (displayMode.ddpfPixelFormat.dwRBitMask!=0x00ff0000 ||
			displayMode.ddpfPixelFormat.dwGBitMask!=0x0000ff00 ||
			displayMode.ddpfPixelFormat.dwBBitMask!=0x000000ff)
			{
			DebugPrint(("Pixel format is 32 bit, but the bit masks are invalid\n"));
			return false;
			}
		}
	else if ((displayMode.ddpfPixelFormat.dwFlags & DDPF_RGB) && displayMode.ddpfPixelFormat.dwRGBBitCount==16)
		{
		DebugPrint(("16 bit color mode\n"));
		pixelFormat16_=true;
		if (displayMode.ddpfPixelFormat.dwRBitMask!=0xf800 ||
			displayMode.ddpfPixelFormat.dwGBitMask!=0x07e0 ||
			displayMode.ddpfPixelFormat.dwBBitMask!=0x001f)
			{
			DebugPrint(("Pixel format is 16 bit, but the bit masks are invalid\n"));
			return false;
			}
		}
	else
		{
		DebugPrint(("Pixel format is not 16 ot 32 bit\n"));
		return false;
		}

	// Check if asynchronous blits are supported
	DDCAPS ddcaps;
	memset(&ddcaps,0,sizeof(ddcaps));
	ddcaps.dwSize=sizeof(ddcaps);
	canUseAsyncBlt_=false;
	ddrval=directDraw_->GetCaps(&ddcaps,0);
	if (SUCCEEDED(ddrval))
		{
		if ((ddcaps.dwCaps & DDCAPS_BLTQUEUE)>0)
			{
			canUseAsyncBlt_=true;
			}
		}

	// Create the primary surface
	DDSURFACEDESC       ddsd;
	memset(&ddsd,0,sizeof(DDSURFACEDESC));
	ddsd.dwSize = sizeof(ddsd);
/*	if (fullscreen_)
		{
		// Create the primary surface with one back buffer.
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
		ddsd.dwBackBufferCount = 1;
		}
	else
*/		{
		// Create simple primary surface
		ddsd.dwFlags = DDSD_CAPS ;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		}

	ddrval = directDraw_->CreateSurface(&ddsd, &frontBuffer_, 0);
	if (FAILED(ddrval))
		{
		DebugPrint(("Failed to create direct draw front buffer\n"));
		return false;
		}


	// Set up clipper for primary surface
	if (!fullscreen_)
		{
		ddrval = directDraw_->CreateClipper( 0, &clipper_, 0 );
		if (FAILED(ddrval))
			{
			DebugPrint(("Failed to create clipper\n"));
			return false;
			}

		ddrval = clipper_->SetHWnd( 0, windowHandle_);
		if (FAILED(ddrval))
			{
			DebugPrint(("Failed to set clipper window\n"));
			return false;
			}

		ddrval = frontBuffer_->SetClipper(clipper_);
		if (FAILED(ddrval))
			{
			DebugPrint(("Failed to assign clipper to primary surface\n"));
			return false;
			}
		}


	// Create back buffer
/*	if (fullscreen_)
		{
		DDSCAPS ddscaps;
		MemSet(&ddscaps,0,sizeof(DDSCAPS));
		ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
		ddrval = frontBuffer_->GetAttachedSurface(&ddscaps, &backBuffer_);
		if (FAILED(ddrval))
			{
			DebugPrint(("Failed to retrieve back buffer\n"));
			Terminate();
			return false;
			}
		}
	else
*/		{
		DWORD capsFlags=DDSCAPS_OFFSCREENPLAIN;
		ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
		ddsd.ddsCaps.dwCaps = capsFlags;
		ddsd.dwWidth=screenWidth_;
		ddsd.dwHeight=screenHeight_;
		ddsd.ddpfPixelFormat=displayMode.ddpfPixelFormat;
		ddrval = directDraw_->CreateSurface( &ddsd, &backBuffer_, 0 );
		if (FAILED(ddrval))
			{
			DebugPrint(("Failed to create back buffer\n"));
			return false;
			}
		}

	bool result=ClearBackBuffer(0,0,screenWidth_,screenHeight_,0);
	if (!result)
		{
		return false;
		}

	return true;
	}


//*** Destructor ***

Platform_Win32_Screen_DDraw::~Platform_Win32_Screen_DDraw()
	{
	if (clipper_)
		{
		clipper_->Release();
		}
	if (backBuffer_)
		{
		backBuffer_->Release();
		}
	if (frontBuffer_)
		{
		frontBuffer_->Release();
		}

	if (directDraw_)
		{
		directDraw_->Release();
		}

	if (ddrawDLL_)
		{
		FreeLibrary((HMODULE)ddrawDLL_);
		}

	if (fullscreen_)
		{
		SetWindowLong(windowHandle_,GWL_STYLE,WS_OVERLAPPEDWINDOW | WS_VISIBLE );
		}
	}


//*** Present ***

bool Platform_Win32_Screen_DDraw::Present(unsigned short* bitmapData, int bitmapWidth, int bitmapHeight, unsigned short modulate, unsigned short backgroundColor)
	{
	lastPresentWidth_=bitmapWidth;
	lastPresentHeight_=bitmapHeight;

	// Check if we need to clear the borders
	bool clearBorders=false;
	if (previousPresentColor_!=backgroundColor || previousPresentWidth_==-1 || previousPresentHeight_==-1 ||
		bitmapWidth<previousPresentWidth_ || bitmapHeight<previousPresentHeight_)
		{
		clearBorders=true;
		}

	previousPresentColor_=backgroundColor;
	previousPresentWidth_=bitmapWidth;
	previousPresentHeight_=bitmapHeight;

	// Copy data from Bitmap to video memory
	if (interpolationMode_)
		{
		bool result=CopyBitmapToBackBufferInterpolation(bitmapData,bitmapWidth,bitmapHeight,modulate);
		if (!result)
			{
			return false;
			}
		}
	else
		{
		bool result=CopyBitmapToBackBufferNoInterpolation(bitmapData,bitmapWidth,bitmapHeight,modulate,backgroundColor,clearBorders);
		if (!result)
			{
			return false;
			}
		}

	//if (fullscreen_)
		{
		// Wait for vertical blank
		HRESULT ddrval = directDraw_->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN,0);
		if (FAILED(ddrval))
			{
			DebugPrint(("WaitForVerticalBlank failed\n"));
			return false;
			}
		}

	// Copy data from offscreen surface to screen
	if (interpolationMode_)
		{
		bool result=CopyBackBufferToFrontBufferInterpolation(min(bitmapWidth,screenWidth_),min(bitmapHeight,screenHeight_),backgroundColor,clearBorders);
		if (!result)
			{
			return false;
			}
		}
	else
		{
		bool result=CopyBackBufferToFrontBufferNoInterpolation();
		if (!result)
			{
			return false;
			}
		}


	return true;
	}


//*** Present ***

bool Platform_Win32_Screen_DDraw::Present(unsigned int* bitmapData, int bitmapWidth, int bitmapHeight, unsigned int modulate, unsigned int backgroundColor)
	{
	return true;
	}


//*** CopyBitmapToBackBufferNoInterpolation ***

bool Platform_Win32_Screen_DDraw::CopyBitmapToBackBufferNoInterpolation(unsigned short* bitmapData, int bitmapWidth, int bitmapHeight, unsigned short modulate, unsigned short backgroundColor, bool clearBorders)
	{
	int width=bitmapWidth;
	int height=bitmapHeight;

	unsigned int destWidth=screenWidth_;
	unsigned int destHeight=screenHeight_;
	int rowSize=sizeof(unsigned short)*width;

	int hscale=destWidth/width;
	int vscale=destHeight/height;
	int pixelScale=min(hscale,vscale);

	int hborder=destWidth-pixelScale*width;
	int vborder=destHeight-pixelScale*height;
	int x1=(destWidth-pixelScale*width)/2;
	int y1=(destHeight-pixelScale*height)/2;
	int x2=x1+pixelScale*width;
	int y2=y1+pixelScale*height;

	// clear out and reset surface description
	DDSURFACEDESC ddsd;
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	RECT rect;
	rect.left=x1;
	rect.top=y1;
	rect.right=x2;
	rect.bottom=y2;

	HRESULT ddrval=backBuffer_->Lock(&rect,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS | DDLOCK_WAIT,0);
	while (ddrval==DDERR_SURFACELOST)
		{
		ddrval=backBuffer_->Restore();
		if (FAILED(ddrval))
			{
			DebugPrint(("Couldn't restore back buffer\n"));
			return false;
			}
		ddrval=backBuffer_->Lock(&rect,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS | DDLOCK_WAIT,0);
		}
	if (FAILED(ddrval))
		{
		DebugPrint(("Couldn't lock back buffer\n"));
		return false;
		}
	if (!(ddsd.dwFlags&(DDSD_LPSURFACE|DDSD_PITCH)))
		{
		DebugPrint(("Back buffer not valid\n"));
		return false;
		}

	if (pixelFormat16_)
		{
		unsigned short* data=static_cast<unsigned short*>(ddsd.lpSurface);
		unsigned int pitch=ddsd.lPitch;
		unsigned short* src=static_cast<unsigned short*>(bitmapData);
		int conversionBufferPitchDelta=pitch/2-width;

		if (pixelScale==1 && pixelFormat16_ && modulate==0xffff)
			{
			if (conversionBufferPitchDelta!=0)
				{
				conversionBufferPitchDelta+=width;
				for (int y=0; y<height; y++)
					{
					memcpy(data,src,rowSize);
					src+=width;
					data+=conversionBufferPitchDelta;
					}
				}
			else
				{
				memcpy(data,src,sizeof(unsigned short)*width*height);
				}
			}
		else
			{
			for (int y=0; y<height; y++)
				{
				for (int x=0; x<width; x++)
					{
					unsigned short pixel=*src;
					if (modulate!=0xffff)
						{
						pixel=RGBModulate16(pixel,modulate);
						}
					src++;
					for (int py=0; py<pixelScale; py++)
						{
						for (int px=0; px<pixelScale; px++)
							{
							data[px+py*(conversionBufferPitchDelta+width)]=pixel;
							}
						}
					data+=pixelScale;
					}
				data+=conversionBufferPitchDelta*pixelScale;
				}
			}
		}
	else if (!pixelFormat16_)
		{
		unsigned int* data=static_cast<unsigned int*>(ddsd.lpSurface);
		unsigned int pitch=ddsd.lPitch;
		unsigned short* src=static_cast<unsigned short*>(bitmapData);
		int conversionBufferPitchDelta=pitch/4-width;
		for (int y=0; y<height; y++)
			{
			for (int x=0; x<width; x++)
				{
				unsigned short pixel=*src;
				if (modulate!=0xffff)
					{
					pixel=RGBModulate16(pixel,modulate);
					}
				src++;
				for (int py=0; py<pixelScale; py++)
					{
					for (int px=0; px<pixelScale; px++)
						{
						data[px+py*(conversionBufferPitchDelta+width)]=RGB16TO32(pixel);
						}
					}
				data+=pixelScale;
				}
			data+=conversionBufferPitchDelta*pixelScale;
			}
		}
	ddrval=backBuffer_->Unlock(ddsd.lpSurface);
	if (FAILED(ddrval))
		{
		DebugPrint(("Failed to unlock back buffer\n"));
		return false;
		}


	if (clearBorders || !previousPresent_AppliedToBackBuffer_)
		{
		if (vborder)
			{
			bool result=ClearBackBuffer(0,0,destWidth,vborder/2,backgroundColor);
			if (!result)
				{
				return false;
				}
			result=ClearBackBuffer(0,screenHeight_-vborder/2-1,destWidth,screenHeight_,backgroundColor);
			if (!result)
				{
				return false;
				}
			}
		if (hborder)
			{
			bool result=ClearBackBuffer(0,vborder/2,hborder/2,screenHeight_-vborder/2,backgroundColor);
			if (!result)
				{
				return false;
				}
			result=ClearBackBuffer(screenWidth_-hborder/2,vborder/2,screenWidth_,screenHeight_-vborder/2,backgroundColor);
			if (!result)
				{
				return false;
				}
			}
		if (clearBorders)
			{
			previousPresent_AppliedToBackBuffer_=false;
			}
		else
			{
			previousPresent_AppliedToBackBuffer_=true;
			}
		}

	return true;
	}


//*** CopyBitmapToBackBufferInterpolation ***

bool Platform_Win32_Screen_DDraw::CopyBitmapToBackBufferInterpolation(unsigned short* bitmapData, int bitmapWidth, int bitmapHeight, unsigned short modulate)
	{
	int width=bitmapWidth;
	int height=bitmapHeight;

	int x1=0;
	int y1=0;
	int x2=min(width,screenWidth_);
	int y2=min(height,screenHeight_);

	// clear out and reset surface description
	DDSURFACEDESC ddsd;
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	RECT rect;
	rect.left=x1;
	rect.top=y1;
	rect.right=x2;
	rect.bottom=y2;

	HRESULT ddrval=backBuffer_->Lock(&rect,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS | DDLOCK_WAIT,0);
	while (ddrval==DDERR_SURFACELOST)
		{
		ddrval=backBuffer_->Restore();
		if (FAILED(ddrval))
			{
			DebugPrint(("Couldn't restore back buffer\n"));
			return false;
			}
		ddrval=backBuffer_->Lock(&rect,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS | DDLOCK_WAIT,0);
		}
	if (FAILED(ddrval))
		{
		DebugPrint(("Couldn't lock back buffer\n"));
		return false;
		}
	if (!(ddsd.dwFlags&(DDSD_LPSURFACE|DDSD_PITCH)))
		{
		DebugPrint(("Back buffer not valid\n"));
		return false;
		}

	if (pixelFormat16_)
		{
		unsigned short* dst=static_cast<unsigned short*>(ddsd.lpSurface);

		unsigned int pitch=ddsd.lPitch;
		unsigned short* src=static_cast<unsigned short*>(bitmapData);
		int conversionBufferPitchDelta=pitch/2-min(width,screenWidth_);

		if (modulate==0xffff)
			{
			if (conversionBufferPitchDelta!=0)
				{
				int rowSize=sizeof(unsigned short)*min(width,screenWidth_);
				conversionBufferPitchDelta+=min(width,screenWidth_);
				for (int y=0; y<min(height,screenHeight_); y++)
					{
					memcpy(dst,src,rowSize);
					src+=width;
					dst+=conversionBufferPitchDelta;
					}
				}
			else
				{
				memcpy(dst,src,sizeof(unsigned short)*width*height);
				}
			}
		else
			{
			int sourceDelta=0;
			if (width>screenWidth_)
				{
				sourceDelta=width-screenWidth_;
				}
			for (int y=0; y<min(height,screenHeight_); y++)
				{
				for (int x=0; x<min(width,screenWidth_); x++)
					{
					unsigned short pixel=*src;
					pixel=RGBModulate16(pixel,modulate);
					*dst=pixel;
					src++;
					dst++;
					}
				dst+=conversionBufferPitchDelta;
				src+=sourceDelta;
				}
			}
		}
	else
		{
		unsigned int* dst=static_cast<unsigned int*>(ddsd.lpSurface);
		unsigned int pitch=ddsd.lPitch;
		unsigned short* src=static_cast<unsigned short*>(bitmapData);
		int conversionBufferPitchDelta=pitch/4-min(width,screenWidth_);
		int sourceDelta=0;
		if (width>screenWidth_)
			{
			sourceDelta=width-screenWidth_;
			}
		for (int y=0; y<min(height,screenHeight_); y++)
			{
			for (int x=0; x<min(width,screenWidth_); x++)
				{
				unsigned short pixel=*src;
				if (modulate!=0xffff)
					{
					pixel=RGBModulate16(pixel,modulate);
					}
				*dst=RGB16TO32(pixel);
				src++;
				dst++;
				}
			dst+=conversionBufferPitchDelta;
			src+=sourceDelta;
			}
		}
	ddrval=backBuffer_->Unlock(ddsd.lpSurface);
	if (FAILED(ddrval))
		{
		DebugPrint(("Failed to unlock back buffer\n"));
		return false;
		}


	return true;
	}


//*** CopyBackBufferToFrontBufferNoInterpolation ***

bool Platform_Win32_Screen_DDraw::CopyBackBufferToFrontBufferNoInterpolation()
	{
	RECT rect;
	BOOL retval=GetClientRect(windowHandle_,&rect);
	Assert(retval,"Failed to get client rect");
	POINT pt1;
	POINT pt2;
	pt1.x=rect.left;
	pt1.y=rect.top;
	pt2.x=rect.right;
	pt2.y=rect.bottom;
	retval=ClientToScreen(windowHandle_, &pt1 );
	Assert(retval,"ClientToScreen failed!");
	retval=ClientToScreen(windowHandle_, &pt2 );
	Assert(retval,"ClientToScreen failed!");
	rect.left=pt1.x;
	rect.top=pt1.y;
	rect.right=pt2.x;
	rect.bottom=pt2.y;

	int sourceWidth=screenWidth_;
	int sourceHeight=screenHeight_;
	int destWidth=rect.right-rect.left;
	int destHeight=rect.bottom-rect.top;

	if (sourceWidth>destWidth)
		{
		sourceWidth=destWidth;
		}

	if (sourceHeight>destHeight)
		{
		sourceHeight=destHeight;
		}

	int hscale=destWidth/sourceWidth;
	int vscale=destHeight/sourceHeight;
	int pixelScale=hscale;
	if (vscale<hscale)
		{
		pixelScale=vscale;
		}
	int hborder=destWidth-pixelScale*sourceWidth;
	int vborder=destHeight-pixelScale*sourceHeight;

	RECT dest=rect;
	dest.left+=hborder/2;
	dest.top+=vborder/2;
	dest.right-=hborder/2;
	dest.bottom-=vborder/2;

	RECT source;
	source.left=0;
	source.top=0;
	source.right=sourceWidth;
	source.bottom=sourceHeight;

	// Blit offscreen surface to primary surface

	// use DDCAPS_BLTQUEUE to check if we can use DDBLT_ASYNC
	DWORD blitFlags= DDBLT_WAIT;
	if (canUseAsyncBlt_)
		{
		blitFlags|=DDBLT_ASYNC;
		}

	HRESULT ddrval=frontBuffer_->Blt(&dest,backBuffer_,&source, blitFlags,0);
	while (ddrval==DDERR_SURFACELOST)
		{
		ddrval=frontBuffer_->Restore();
		if (FAILED(ddrval))
			{
			DebugPrint(("Failed to restore front buffer\n"));
			return false;
			}
		ddrval=backBuffer_->Restore();
		if (FAILED(ddrval))
			{
			DebugPrint(("Failed to restore back buffer\n"));
			return false;
			}
		ddrval=frontBuffer_->Blt(&rect,backBuffer_,&source, blitFlags,0);
		}

	if (FAILED(ddrval))
		{
		DebugPrint(("Blt to front buffer failed\n"));
		return false;
		}

	return true;
	}



//*** CopyBackBufferToFrontBufferInterpolation ***

bool Platform_Win32_Screen_DDraw::CopyBackBufferToFrontBufferInterpolation(int width, int height, unsigned short backgroundColor, bool clearBorders)
	{
	float hscale=screenWidth_/(float)width;
	float vscale=screenHeight_/(float)height;
	float pixelScale=min(hscale,vscale);

	float hborder=(screenWidth_-pixelScale*width)/2;
	float vborder=(screenHeight_-pixelScale*height)/2;

	RECT rect;
	BOOL retval=GetClientRect(windowHandle_,&rect);
	Assert(retval,"Failed to get client rect");
	POINT pt1;
	POINT pt2;
	pt1.x=rect.left;
	pt1.y=rect.top;
	pt2.x=rect.right;
	pt2.y=rect.bottom;
	retval=ClientToScreen(windowHandle_, &pt1 );
	Assert(retval,"ClientToScreen failed!");
	retval=ClientToScreen(windowHandle_, &pt2 );
	Assert(retval,"ClientToScreen failed!");
	rect.left=pt1.x;
	rect.top=pt1.y;
	rect.right=pt2.x;
	rect.bottom=pt2.y;

	RECT dest=rect;
	dest.left+=(int)hborder;
	dest.top+=(int)vborder;
	dest.right-=(int)hborder;
	dest.bottom-=(int)vborder;

	RECT source;
	source.left=0;
	source.top=0;
	source.right=width;
	source.bottom=height;

	// Blit offscreen surface to primary surface

	// use DDCAPS_BLTQUEUE to check if we can use DDBLT_ASYNC
	DWORD blitFlags= DDBLT_WAIT;
	if (canUseAsyncBlt_)
		{
		blitFlags|=DDBLT_ASYNC;
		}

	HRESULT ddrval=frontBuffer_->Blt(&dest,backBuffer_,&source, blitFlags,0);
	while (ddrval==DDERR_SURFACELOST)
		{
		ddrval=frontBuffer_->Restore();
		if (FAILED(ddrval))
			{
			DebugPrint(("Failed to restore front buffer\n"));
			return false;
			}
		ddrval=backBuffer_->Restore();
		if (FAILED(ddrval))
			{
			DebugPrint(("Failed to restore back buffer\n"));
			return false;
			}
		ddrval=frontBuffer_->Blt(&rect,backBuffer_,&source, blitFlags,0);
		}

	if (FAILED(ddrval))
		{
		DebugPrint(("Blt to front buffer failed\n"));
		return false;
		}

	DDBLTFX fx;
	ZeroMemory(&fx,sizeof(DDBLTFX));
	fx.dwSize=sizeof(DDBLTFX);
	if (pixelFormat16_)
		{
		fx.dwFillColor=backgroundColor;
		}
	else
		{
		fx.dwFillColor=RGB16TO32(backgroundColor);
		}

	if (((int)vborder)>0)
		{
		RECT dest=rect;
		dest.bottom=dest.top+(int)vborder;
		frontBuffer_->Blt(&dest,0,0,blitFlags|DDBLT_COLORFILL,&fx);
		dest=rect;
		dest.top=dest.bottom-(int)vborder;
		frontBuffer_->Blt(&dest,0,0,blitFlags|DDBLT_COLORFILL,&fx);
		}
	if (((int)hborder)>0)
		{
		RECT dest=rect;
		dest.right=dest.left+(int)hborder;
		frontBuffer_->Blt(&dest,0,0,blitFlags|DDBLT_COLORFILL,&fx);
		dest=rect;
		dest.left=dest.right-(int)hborder;
		frontBuffer_->Blt(&dest,0,0,blitFlags|DDBLT_COLORFILL,&fx);
		}


	return true;
	}


//*** ClearBackBuffer ***

bool Platform_Win32_Screen_DDraw::ClearBackBuffer(int x1, int y1, int x2, int y2, unsigned short color)
	{
	RECT rect;
	rect.left=x1;
	rect.top=y1;
	rect.right=x2;
	rect.bottom=y2;

	DDBLTFX bltfx;
	bltfx.dwSize=sizeof(DDBLTFX);
	if (pixelFormat16_)
		{
		bltfx.dwFillColor=color;
		}
	else
		{
		bltfx.dwFillColor=RGB16TO32(color);
		}

	DWORD blitFlags=DDBLT_WAIT | DDBLT_DDFX | DDBLT_COLORFILL;
	if (canUseAsyncBlt_)
		{
		blitFlags|=DDBLT_ASYNC;
		}

	HRESULT ddrval=backBuffer_->Blt(&rect,0,0,blitFlags,&bltfx);
	while (ddrval==DDERR_SURFACELOST)
		{
		ddrval=backBuffer_->Restore();
		if (FAILED(ddrval))
			{
			DebugPrint(("Failed to restore front buffer\n"));
			return false;
			}
		ddrval=backBuffer_->Blt(&rect,0,0,blitFlags,&bltfx);
		}
	if (FAILED(ddrval))
		{
		DebugPrint(("ColorBlt to front buffer failed\n"));
		return false;
		}

	return true;
	}



//*** SetInterpolationMode ***

void Platform_Win32_Screen_DDraw::SetInterpolationMode(bool enabled)
	{
	interpolationMode_=enabled;
	}


//*** TransformCursorCoordinates ***

void Platform_Win32_Screen_DDraw::TransformCursorCoordinates(float& x, float& y)
	{
	if (lastPresentWidth_==0 || lastPresentHeight_==0)
		{
		return;
		}

	if (!interpolationMode_)
		{
		int hscale=screenWidth_/lastPresentWidth_;
		int vscale=screenHeight_/lastPresentHeight_;
		int pixelScale=max(1,min(hscale,vscale));
		int hborder=max(0,(screenWidth_-pixelScale*lastPresentWidth_)/2);
		int vborder=max(0,(screenHeight_-pixelScale*lastPresentHeight_)/2);

		x=(x-hborder)/pixelScale;
		y=(y-vborder)/pixelScale;
		return;
		}

	float hscale=screenWidth_/(float)lastPresentWidth_;
	float vscale=screenHeight_/(float)lastPresentHeight_;
	float pixelScale=min(hscale,vscale);

	float hborder=(screenWidth_-pixelScale*lastPresentWidth_)/2;
	float vborder=(screenHeight_-pixelScale*lastPresentHeight_)/2;

	x=(x-hborder)/pixelScale;
	y=(y-vborder)/pixelScale;
	}


}; // namespace pixie
