/**
 * \class	Platform_NULL_Screen
 * 
 * \ingroup	platform_null
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_NULL_Screen_H__
#define __Platform_NULL_Screen_H__

// Includes
#include "Platform_Screen.h"

// Forward declares


// Platform_NULL_Screen
class Platform_NULL_Screen:public Platform_Screen
	{

	public:
		Platform_NULL_Screen();

		virtual void Present(
			unsigned short* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			unsigned short modulate = 0xffff,
			unsigned short backgroundColor = 0x0000
			);

		virtual void Present(
			unsigned int* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			unsigned int modulate = 0xffffffff,
			unsigned int backgroundColor = 0x00000000
			);

		virtual void SetInterpolationMode(bool enabled);

		virtual bool GetInterpolationMode();

		virtual void SetFullscreen(bool fullscreen);

		virtual bool GetFullscreen();

		virtual void SetSize(int width, int height);
		
		virtual int GetWidth();

		virtual int GetHeight();

		virtual void TransformCursorCoordinates(float& x, float& y);

	};


#endif /* __Platform_NULL_Screen_H__ */
