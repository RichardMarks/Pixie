/**
 * \class	Platform_Win32_Sound_SoundStream_DSound
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_Win32_Sound_SoundStream_DSound_H__
#define __Platform_Win32_Sound_SoundStream_DSound_H__

// Includes
#include "Platform_Sound_SoundStream.h"

// Forward declares
struct IDirectSound;
struct IDirectSoundBuffer;

// Platform_Win32_Sound_SoundStream_DSound
class Platform_Win32_Sound_SoundStream_DSound : public Platform_Sound_SoundStream
	{
	public:
		Platform_Win32_Sound_SoundStream_DSound(
			IDirectSound* directSound,
			int channels, 
			int frequency, 
			int bitsPerSample, 
			int size
			);
		virtual ~Platform_Win32_Sound_SoundStream_DSound();

	
		virtual void Play();
		virtual void Stop();

		virtual int GetPosition();
		virtual void SetPosition(int position);

		virtual void CopySoundToBuffer(int bufferOffset, const void* soundData, int bytesToCopy);
		virtual void ClearBuffer(int bufferOffset, int bytesToClear);

		virtual int GetChannels();
		virtual int GetFrequency();
		virtual int GetBitsPerSample();
		virtual int GetSize();

		virtual void SetVolume(float level);
		virtual float GetVolume();
	
	private:
		int ConvertLinearLevelToDirectSoundLevel(float level);

	private:
	    IDirectSoundBuffer* soundBuffer_;	
		int channels_;
		int frequency_;
		int bitsPerSample_;
		int size_;
		float volume_;
	};

#endif /* __Platform_Win32_Sound_SoundStream_DSound_H__ */
