/**
 * \class	Platform_Win32_Screen_D3D9
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_Win32_Screen_D3D9_H__
#define __Platform_Win32_Screen_D3D9_H__

// Includes
#include "Platform_Win32_Screen_Technology.h"

// Forward declares
struct IDirect3D9;
struct IDirect3DDevice9;
struct IDirect3DTexture9;


// Platform_Win32_Screen_D3D9
class Platform_Win32_Screen_D3D9:public Platform_Win32_Screen_Technology
	{

	public:
		Platform_Win32_Screen_D3D9(struct HWND__* windowHandle, bool fullscreen, int screenWidth, int screenHeight, bool interpolationMode);
		virtual ~Platform_Win32_Screen_D3D9();

		virtual bool Setup();

		virtual bool Present(
			unsigned short* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			unsigned short modulate,
			unsigned short backgroundColor
			);
	
		virtual bool Present(
			unsigned int* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			unsigned int modulate,
			unsigned int backgroundColor
			);

		virtual void SetInterpolationMode(
			bool enabled		);

		virtual void TransformCursorCoordinates(float& x, float& y);

	private:
		bool CreateBackBuffers(int width, int height);

		bool CopyBitmapToBackBuffer(
			unsigned short* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			unsigned short backgroundColor
			);

		bool CopyBackBufferToFrontBuffer(
			int bitmapWidth, 
			int bitmapHeight, 
			unsigned short modulate);

		bool CopyBitmapToBackBuffer(
			unsigned int* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			unsigned int backgroundColor
			);

		bool CopyBackBufferToFrontBuffer(
			int bitmapWidth, 
			int bitmapHeight, 
			unsigned int modulate);

	private:
		struct HWND__* windowHandle_;
		bool interpolationMode_;
		bool fullscreen_;
		int screenWidth_;
		int screenHeight_;
		void* d3dDLL_;
		IDirect3D9* direct3D_;
		IDirect3DDevice9* device_;
		IDirect3DTexture9* backbuffer16a_;
		IDirect3DTexture9* backbuffer16b_;
		IDirect3DTexture9* backbuffer32a_;
		IDirect3DTexture9* backbuffer32b_;
		int backbufferWidth_;
		int backbufferHeight_;
		bool usingDynamicTexture_;
		bool buffer1Filled_;
		bool buffer2Filled_;

		int lastPresentWidth_;
		int lastPresentHeight_;
	};

		

#endif /* __Platform_Win32_Screen_D3D9_H__ */
