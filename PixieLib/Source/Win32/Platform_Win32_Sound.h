/**
 * \class	Platform_Win32_Sound
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_Win32_Sound_H__
#define __Platform_Win32_Sound_H__

// Includes
#include "Platform_Sound.h"

// Forward declares
class Platform_Win32_OS;
class Platform_Win32_Sound_Technology;

// Platform_Win32_Sound
class Platform_Win32_Sound:public Platform_Sound,PlatformEventListener
	{

	public:
		Platform_Win32_Sound(Platform_Win32_OS* os);
		virtual ~Platform_Win32_Sound();

		virtual void OnOsYield();

		virtual Platform_Sound_SoundStream* CreateSoundStream(int channels, int frequency, int bitsPerSample, int size);
		
	private:
		enum Technology
			{
			Technology_DSound,
			Technology_WMM,
			Technology_NoSound,
			Technology_Undefined,
			};

		void SetTechnology(
			Technology technology
			);

		void DowngradeTechnology();

	private:
		struct HWND__* windowHandle_;
		Technology technology_;
		Platform_Win32_Sound_Technology* technologyInstance_;
	};

#endif /* __Platform_Win32_SOUND_H__ */
