/**
 * \class	Platform_Win32_Screen_DDraw
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_Win32_Screen_DDraw_H__
#define __Platform_Win32_Screen_DDraw_H__

// Includes
#include "Platform_Win32_Screen_Technology.h"

// Forward declares
struct IDirectDraw2;
struct IDirectDrawSurface;
struct IDirectDrawClipper;

// Platform_Win32_Screen_DDraw
class Platform_Win32_Screen_DDraw:public Platform_Win32_Screen_Technology
	{
	public:
		Platform_Win32_Screen_DDraw(struct HWND__* windowHandle, bool fullscreen, int screenWidth, int screenHeight);
		virtual ~Platform_Win32_Screen_DDraw();

		virtual bool Setup();


		virtual bool Present(
			unsigned short* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			unsigned short modulate,
			unsigned short backgroundColor
			);

		virtual void SetInterpolationMode(bool enabled);

		virtual void TransformCursorCoordinates(float& x, float& y);

	private:
		bool CopyBitmapToBackBufferInterpolation(
			unsigned short* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			unsigned short modulate
			);	

		bool CopyBitmapToBackBufferNoInterpolation(
			unsigned short* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			unsigned short modulate,
			unsigned short 	backgroundColor, 
			bool clearBorders
			);	

		bool CopyBackBufferToFrontBufferInterpolation(
			int width, 
			int height, 
			unsigned short backgroundColor, 
			bool clearBorder);
		
		bool CopyBackBufferToFrontBufferNoInterpolation();

		bool ClearBackBuffer(
			int x1, 
			int y1, 
			int x2, 
			int y2, 
			unsigned short color
			);

	private:
		struct HWND__* windowHandle_;
		bool interpolationMode_;
		bool fullscreen_;
		int screenWidth_;
		int screenHeight_;
		void* ddrawDLL_;
		IDirectDraw2* directDraw_;           
		IDirectDrawSurface* frontBuffer_; 
		IDirectDrawSurface* backBuffer_; 
		IDirectDrawClipper* clipper_;
		bool canUseAsyncBlt_;
		bool pixelFormat16_;
		unsigned short previousPresentColor_;
		int previousPresentWidth_;
		int previousPresentHeight_;
		int previousPresent_AppliedToBackBuffer_;
		int lastPresentWidth_;
		int lastPresentHeight_;
	};

#endif /* __Platform_Win32_Screen_DDraw_H__ */
