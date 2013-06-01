/**
 * \class	Platform_Win32_Screen
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_Win32_Screen_H__
#define __Platform_Win32_Screen_H__

// Includes
#include "Platform_Screen.h"

// Forward declares
class Platform_Win32_OS;
class Platform_Win32_Screen_Technology;


// Platform_Win32_Screen
class Platform_Win32_Screen:public Platform_Screen, PlatformEventListener
	{

	public:
	
		Platform_Win32_Screen(Platform_Win32_OS* os);
		virtual ~Platform_Win32_Screen();

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

		void OnWmSize(int width, int height);
		
		void OnMinimize();
		
		void OnRestore();

		void OnLoseFocus();

		void OnGainFocus();

		virtual void OnCustomEvent(const char* eventId, void* userData);

	private:
		void SetWindowSize();

		enum Technology
			{
			Technology_D3D9,
			Technology_DDraw,
			Technology_GDI,
			Technology_Undefined,
			};

		void SetTechnology(
			Technology technology
			);

		void FirstTimeInitialize();

		void DowngradeTechnology();

		bool IsRunningOnWine();

		void DebugText(unsigned short* colorData, int hPitch, int vPitch, int x, int y, const char* text, unsigned short color);
		void BlitCharacter(unsigned short* colorData, int hPitch, int vPitch, int sx, int sy, int dx, int dy, unsigned short color);

		void DebugText(unsigned int* colorData, int hPitch, int vPitch, int x, int y, const char* text, unsigned int color);
		void BlitCharacter(unsigned int* colorData, int hPitch, int vPitch, int sx, int sy, int dx, int dy, unsigned int color);
	private:
		static unsigned int Platform_Win32_Screen::DebugFontData[];
		struct HWND__* windowHandle_;
		Technology technology_;
		Platform_Win32_Screen_Technology* technologyInstance_;
		bool fullscreen_;
		int windowedX_;
		int windowedY_;
		int windowedWidth_;
		int windowedHeight_;
		int fullscreenWidth_;
		int fullscreenHeight_;
		int disableOnWmSize_;
		bool showfps_;
		bool interpolationMode_;
		bool ignoreNextOnWmSize_;
		bool forceddraw_;
		bool forcegdi_;
		bool firstTimeInitializeCalled_;
	};


#endif /* __Platform_Win32_Screen_H__ */
