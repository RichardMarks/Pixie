/**
 * \class	Platform_Win32_Sound_DSound
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_Win32_Sound_DSound_H__
#define __Platform_Win32_Sound_DSound_H__

// Includes
#include "Platform_Win32_Sound_Technology.h"

// Forward declares
struct IDirectSound;

// Platform_Win32_Sound_DSound
class Platform_Win32_Sound_DSound:public Platform_Win32_Sound_Technology
	{

	public:
		Platform_Win32_Sound_DSound(struct HWND__* windowHandle);
		virtual ~Platform_Win32_Sound_DSound();

		virtual bool Setup();

		virtual void Update();

		virtual Platform_Sound_SoundStream* CreateSoundStream(int channels, int frequency, int bitsPerSample, int size);
		
	private:
		struct HWND__* windowHandle_;
		void* dsoundDLL_;
		IDirectSound* directSound_; 

	};

#endif /* __Platform_Win32_Sound_DSound_H__ */
