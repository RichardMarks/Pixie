/**
 * \class	Platform_NULL_Sound_SoundStream
 * 
 * \ingroup	platform_null
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_NULL_Sound_SoundStream_H__
#define __Platform_NULL_Sound_SoundStream_H__

// Includes
#include "Platform_Sound_SoundStream.h"

// Forward declares


// Platform_NULL_Sound_SoundStream
class Platform_NULL_Sound_SoundStream : public Platform_Sound_SoundStream
	{
	public:
		Platform_NULL_Sound_SoundStream(
			int channels, 
			int frequency, 
			int bitsPerSample, 
			int size
			);
	
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

		virtual void Update();

		virtual void SetVolume(float level);
		virtual float GetVolume();

		int channels_;
		int frequency_;
		int bitsPerSample_;
		int size_;
	};

#endif /* __Platform_NULL_Sound_SoundStream_H__ */
