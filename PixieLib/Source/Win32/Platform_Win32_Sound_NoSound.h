/**
 * \class	Platform_Win32_Sound_NoSound
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_Win32_Sound_NoSound_H__
#define __Platform_Win32_Sound_NoSound_H__

// Includes
#include "Platform_Win32_Sound_Technology.h"

// Forward declares


// Platform_Win32_Sound_NoSound
class Platform_Win32_Sound_NoSound:public Platform_Win32_Sound_Technology
	{

	public:
		Platform_Win32_Sound_NoSound();
		virtual ~Platform_Win32_Sound_NoSound();

		virtual bool Setup();

		virtual void Update();

		virtual Platform_Sound_SoundStream* CreateSoundStream(int channels, int frequency, int bitsPerSample, int size);
		
	private:

	};

#endif /* __Platform_Win32_Sound_NoSound_H__ */
