//*** Platform_Win32_Screen_GDI.cpp ***

#include "Platform_Win32_Screen_GDI.h"

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

namespace pixie {

 //*** Constructor ***

Platform_Win32_Screen_GDI::Platform_Win32_Screen_GDI(struct HWND__* windowHandle, bool fullscreen, int screenWidth, int screenHeight):
	windowHandle_(windowHandle),
	fullscreen_(fullscreen),
	screenWidth_(screenWidth),
	screenHeight_(screenHeight),
	backBuffer_(0)
	{
	}


//*** Setup ***

bool Platform_Win32_Screen_GDI::Setup()
	{
	if (fullscreen_)
		{
		SetWindowLong(windowHandle_,GWL_STYLE,WS_POPUP|WS_VISIBLE);

		MoveWindow(windowHandle_,0,0,screenWidth_,screenHeight_,TRUE);
		}

	if (fullscreen_)
		{
		RECT client;
		GetClientRect(GetDesktopWindow(),&client);
		screenWidth_=client.right-client.left;
		screenHeight_=client.bottom-client.top;
		}

	HDC dc = GetDC(windowHandle_);
	BitBlt(dc,0,0,screenWidth_,screenHeight_,0,0,0,BLACKNESS);
	ReleaseDC (windowHandle_, dc);

	backBuffer_=new unsigned short[screenWidth_*screenHeight_];
	return true;
	}


//*** Destructor ***

Platform_Win32_Screen_GDI::~Platform_Win32_Screen_GDI()
	{
	if (backBuffer_)
		{
		delete[] backBuffer_;
		}
	if (fullscreen_)
		{
		SetWindowLong(windowHandle_,GWL_STYLE,WS_OVERLAPPEDWINDOW | WS_VISIBLE );
		}
	}


//*** Present ***

bool Platform_Win32_Screen_GDI::Present(unsigned short* bitmapData, int bitmapWidth, int bitmapHeight, unsigned short modulate, unsigned short backgroundColor)
	{
	lastPresentWidth_=bitmapWidth;
	lastPresentHeight_=bitmapHeight;

	int dx=screenWidth_-bitmapWidth;
	int dy=screenHeight_-bitmapHeight;
	int scaleX=screenWidth_/bitmapWidth;
	int scaleY=screenHeight_/bitmapHeight;
	int scale=min(scaleX,scaleY);

	// Straight copy
	if (dx==0 && dy==0)
		{
		if (modulate==0xffff)
			{
			SetDIBits(bitmapData,bitmapWidth,bitmapHeight);
			}
		else
			{
			ModulateBitmap(bitmapData,bitmapWidth,bitmapHeight,modulate);
			SetDIBits(backBuffer_,bitmapWidth,bitmapHeight);
			}
		}

	// Center bitmap
	else if (dx>=0 && dy>=0 && scale==1)
		{
		if (modulate==0xffff)
			{
			SetDIBits(bitmapData,bitmapWidth,bitmapHeight,dx/2,dy/2);
			}
		else
			{
			ModulateBitmap(bitmapData,bitmapWidth,bitmapHeight,modulate);
			SetDIBits(backBuffer_,bitmapWidth,bitmapHeight,dx/2,dy/2);
			}
		}

	// Crop bitmap
	else if (dx<0 || dy<0)
		{
		if (modulate==0xffff)
			{
			CropBitmap(bitmapData,bitmapWidth,bitmapHeight);
			}
		else
			{
			ModulateAndCropBitmap(bitmapData,bitmapWidth,bitmapHeight,modulate);
			}
		SetDIBits(backBuffer_,min(screenWidth_,bitmapWidth),min(screenHeight_,bitmapHeight));
		}

	// Scale bitmap
	else
		{
		if (modulate==0xffff)
			{
			ScaleBitmap(bitmapData,bitmapWidth,bitmapHeight,scale);
			}
		else
			{
			ModulateAndScaleBitmap(bitmapData,bitmapWidth,bitmapHeight,modulate,scale);
			}
		SetDIBits(backBuffer_,bitmapWidth*scale,bitmapHeight*scale);
		}
	return true;
	}


//*** Present ***

bool Platform_Win32_Screen_GDI::Present(unsigned int* bitmapData, int bitmapWidth, int bitmapHeight, unsigned int modulate, unsigned int backgroundColor)
	{
	return true;
	}


//*** ModulateBitmap ***

void Platform_Win32_Screen_GDI::ModulateBitmap(unsigned short* bitmapData, int bitmapWidth, int bitmapHeight, unsigned short modulate)
	{
	unsigned short* source=bitmapData;
	unsigned short* destination=backBuffer_;

	int pixels=bitmapWidth*bitmapHeight;
	for (int i=0; i<pixels; i++)
		{
		*destination=RGBModulate16(*source,modulate);
		source++;
		destination++;
		}
	}


//*** ScaleBitmap ***

void Platform_Win32_Screen_GDI::ScaleBitmap(unsigned short* bitmapData, int bitmapWidth, int bitmapHeight, int scale)
	{
	unsigned short* source=bitmapData;
	unsigned short* destination=backBuffer_;

	int lineSize=bitmapWidth*scale;

	for (int y=0; y<bitmapHeight; y++)
		{
		switch (scale)
			{
			case 2:
				{
				for (int x=0; x<bitmapWidth; x++)
					{
					unsigned short pixel=*source;
					source++;

					unsigned int pixel32=pixel<<16 | pixel;

					*reinterpret_cast<unsigned int*>(destination)=pixel32;
					destination+=2;
					}
				} break;
			case 3:
				{
				for (int x=0; x<bitmapWidth; x++)
					{
					unsigned short pixel=*source;
					source++;

					unsigned int pixel32=pixel<<16 | pixel;

					*reinterpret_cast<unsigned int*>(destination)=pixel32;
					destination+=2;
					*destination=pixel;
					destination++;
					}
				} break;
			case 4:
				{
				for (int x=0; x<bitmapWidth; x++)
					{
					unsigned short pixel=*source;
					source++;

					unsigned int pixel32=pixel<<16 | pixel;

					*reinterpret_cast<unsigned int*>(destination)=pixel32;
					destination+=2;
					*reinterpret_cast<unsigned int*>(destination)=pixel32;
					destination+=2;
					}
				} break;
			default:
				{
				for (int x=0; x<bitmapWidth; x++)
					{
					unsigned short pixel=*source;
					source++;
					for (int i=0; i<scale; i++)
						{
						*destination=pixel;
						destination++;
						}
					}
				} break;
			}

		for (int i=1; i<scale; i++)
			{
			memcpy(destination,destination-lineSize,lineSize*sizeof(unsigned short));
			destination+=lineSize;
			}
		}
	}


//*** ModulateAndScaleBitmap ***

void Platform_Win32_Screen_GDI::ModulateAndScaleBitmap(unsigned short* bitmapData, int bitmapWidth, int bitmapHeight, unsigned short modulate, int scale)
	{
	unsigned short* source=bitmapData;
	unsigned short* destination=backBuffer_;

	int lineSize=bitmapWidth*scale;

	for (int y=0; y<bitmapHeight; y++)
		{
		switch (scale)
			{
			case 2:
				{
				for (int x=0; x<bitmapWidth; x++)
					{
					unsigned short pixel=RGBModulate16(*source,modulate);
					source++;

					unsigned int pixel32=pixel<<16 | pixel;

					*reinterpret_cast<unsigned int*>(destination)=pixel32;
					destination+=2;
					}
				} break;
			case 3:
				{
				for (int x=0; x<bitmapWidth; x++)
					{
					unsigned short pixel=RGBModulate16(*source,modulate);
					source++;

					unsigned int pixel32=pixel<<16 | pixel;

					*reinterpret_cast<unsigned int*>(destination)=pixel32;
					destination+=2;
					*destination=pixel;
					destination++;
					}
				} break;
			case 4:
				{
				for (int x=0; x<bitmapWidth; x++)
					{
					unsigned short pixel=RGBModulate16(*source,modulate);
					source++;

					unsigned int pixel32=pixel<<16 | pixel;

					*reinterpret_cast<unsigned int*>(destination)=pixel32;
					destination+=2;
					*reinterpret_cast<unsigned int*>(destination)=pixel32;
					destination+=2;
					}
				} break;
			default:
				{
				for (int x=0; x<bitmapWidth; x++)
					{
					unsigned short pixel=RGBModulate16(*source,modulate);
					source++;
					for (int i=0; i<scale; i++)
						{
						*destination=pixel;
						destination++;
						}
					}
				} break;
			}

		for (int i=1; i<scale; i++)
			{
			memcpy(destination,destination-lineSize,lineSize*sizeof(unsigned short));
			destination+=lineSize;
			}
		}
	}


//*** CropBitmap ***

void Platform_Win32_Screen_GDI::CropBitmap(unsigned short* bitmapData, int bitmapWidth, int bitmapHeight )
	{
	unsigned short* source=bitmapData;
	unsigned short* destination=backBuffer_;

	int lineSize=min(screenWidth_,bitmapWidth)*sizeof(unsigned short);

	int lines=min(screenHeight_,bitmapHeight);
	for (int y=0; y<lines; y++)
		{
		memcpy(destination,source,lineSize);
		source+=bitmapWidth;
		destination+=screenWidth_;
		}
	}


//*** ModulateAndCropBitmap ***

void Platform_Win32_Screen_GDI::ModulateAndCropBitmap(unsigned short* bitmapData, int bitmapWidth, int bitmapHeight, unsigned short modulate)
	{
	unsigned short* source=bitmapData;
	unsigned short* destination=backBuffer_;

	int width=min(screenWidth_,bitmapWidth);
	int sourceWidth=bitmapWidth-width;
	int destinationWidth=screenWidth_-width;

	int lines=min(screenHeight_,bitmapHeight);
	for (int y=0; y<lines; y++)
		{
		for (int x=0; x<width; x++)
			{
			unsigned short pixel=RGBModulate16(*source,modulate);
			*destination=pixel;
			source++;
			destination++;
			}
		source+=sourceWidth;
		destination+=destinationWidth;
		}
	}


//*** SetInterpolationMode ***

void Platform_Win32_Screen_GDI::SetInterpolationMode(bool enabled)
	{
	// Can't do interpolation mode with GDI
	}


//*** SetDIBits ***

void Platform_Win32_Screen_GDI::SetDIBits(unsigned short* data, int width, int height, int x, int y)
	{
	HDC dc = GetDC(windowHandle_);

	BITMAPV4HEADER bmi;
	memset (&bmi, 0, sizeof (bmi));
	bmi.bV4Size          = sizeof (BITMAPV4HEADER);
	bmi.bV4Width         = width;
	bmi.bV4Height        = -height;
	bmi.bV4Planes        = 1;
	bmi.bV4V4Compression = BI_BITFIELDS;
	bmi.bV4XPelsPerMeter = 72;
	bmi.bV4YPelsPerMeter = 72;
	bmi.bV4BitCount      = 16;
	bmi.bV4SizeImage     = width*height*2;
	bmi.bV4RedMask       = 31<<11;
	bmi.bV4GreenMask     = 63<<5;
	bmi.bV4BlueMask      = 31<<0;
	SetDIBitsToDevice(dc, x, y, width, height,0,0,0, height, data, reinterpret_cast<BITMAPINFO*>(&bmi), DIB_RGB_COLORS);

	ReleaseDC (windowHandle_, dc);
	}


//*** TransformCursorCoordinates ***

void Platform_Win32_Screen_GDI::TransformCursorCoordinates(float& x, float& y)
	{
	if (lastPresentWidth_==0 || lastPresentHeight_==0)
		{
		return;
		}

	int hscale=screenWidth_/lastPresentWidth_;
	int vscale=screenHeight_/lastPresentHeight_;
	int pixelScale=max(1,min(hscale,vscale));
	int hborder=max(0,(screenWidth_-pixelScale*lastPresentWidth_)/2);
	int vborder=max(0,(screenHeight_-pixelScale*lastPresentHeight_)/2);

	x=(x-hborder)/pixelScale;
	y=(y-vborder)/pixelScale;
	}

}; // namespace pixie
