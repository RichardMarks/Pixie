/**
 * \class	Platform_Win32_Sound_WMM
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_Win32_Sound_WMM_H__
#define __Platform_Win32_Sound_WMM_H__

// Includes
#include "Platform_Win32_Sound_Technology.h"

// Forward declares
class Platform_Win32_Sound_SoundStream_WMM;

// Platform_Win32_Sound_WMM
class Platform_Win32_Sound_WMM:public Platform_Win32_Sound_Technology
	{

	public:
		Platform_Win32_Sound_WMM();
		virtual ~Platform_Win32_Sound_WMM();

		virtual bool Setup();


		virtual void Update();

		virtual Platform_Sound_SoundStream* CreateSoundStream(int channels, int frequency, int bitsPerSample, int size);
		
		void SoundStreamDestroyed(Platform_Win32_Sound_SoundStream_WMM* stream);
	private:
		int soundStreamCount_;
		int soundStreamMaxCount_;
		Platform_Win32_Sound_SoundStream_WMM** soundStreams_;
	
	};

#endif /* __Platform_Win32_Sound_WMM_H__ */
