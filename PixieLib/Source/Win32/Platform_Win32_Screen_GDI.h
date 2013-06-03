/**
 * \class	Platform_Win32_Screen_GDI
 *
 * \ingroup	win32
 * \brief
 * \author	Mattias Gustavsson
 *
 */

#ifndef __Platform_Win32_Screen_GDI_H__
#define __Platform_Win32_Screen_GDI_H__

// Includes
#include "Platform_Win32_Screen_Technology.h"

// Forward declares
struct HDC__;
struct HWND__;

namespace pixie {

// Platform_Win32_Screen_GDI
class Platform_Win32_Screen_GDI:public Platform_Win32_Screen_Technology
	{

	public:
		Platform_Win32_Screen_GDI(struct HWND__* windowHandle, bool fullscreen, int screenWidth, int screenHeight);
		virtual ~Platform_Win32_Screen_GDI();

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

		virtual void SetInterpolationMode(bool enabled);

		virtual void TransformCursorCoordinates(float& x, float& y);
	private:
		bool CopyBitmapData(
			unsigned short* source, int sourceWidth, int sourceHeight, int sourcePitch,
			unsigned short* destination, int destinationWidth, int destinationHeight, int destinationPitch,
			unsigned short modulate
			);

		void ModulateBitmap(
			unsigned short* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			unsigned short modulate
			);

		void ScaleBitmap(
			unsigned short* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			int scale
			);

		void ModulateAndScaleBitmap
			(
			unsigned short* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			unsigned short modulate,
			int scale);

		void CropBitmap(
			unsigned short* bitmapData,
			int bitmapWidth,
			int bitmapHeight
			);

		void ModulateAndCropBitmap(
			unsigned short* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			unsigned short modulate
			);

		void SetDIBits(
			unsigned short* data,
			int width,
			int height,
			int x = 0,
			int y = 0
			);

	private:
		HWND__* windowHandle_;
		bool fullscreen_;
		int screenWidth_;
		int screenHeight_;
		unsigned short* backBuffer_;
		int lastPresentWidth_;
		int lastPresentHeight_;
	};

}; // namespace pixie

#endif /* __Platform_Win32_Screen_GDI_H__ */
