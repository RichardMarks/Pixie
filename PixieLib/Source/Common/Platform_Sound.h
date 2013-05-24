/**
 * \class	Platform_Sound
 * 
 * \ingroup	platform
 * \brief	Platform independent abstraction of sound playback
 * \author	Mattias Gustavsson	
 * 
 * The platform system is simple but powerful - all it provides
 * is functionality to create a SoundStream object, which can
 * be used to play streaming sounds. You can create several 
 * sound streams, and they can be played independently. The idea
 * is to create a few streams to use as sound channels, and then
 * keep streaming sounds to them as necessary, rather than creating
 * indidividual full sounds. The Audio module in the Pixie engine
 * implements a more user-friendly sound system built on top of 
 * this, which allows for playing of music and sound in a more
 * traditional way.
 */

#ifndef __Platform_Sound_H__
#define __Platform_Sound_H__

// Includes
#include "Platform.h"

// Forward declares
class Platform_Sound_SoundStream;

// Platform_Sound
class Platform_Sound
	{
	public:
		/**
		 * Destructor
		 */
		virtual ~Platform_Sound() { };


		/**
		 * Creates a Platform_Sound_SoundStream instance, with the specified number of channels (1=mono, 
		 * 2=stereo), frequency (number of samples per second) and bits per sample (8 or 16),
		 * and of the specified size in bytes. A sound stream provides basic functionality for streaming
		 * sound data to the sound card, and works by having the calling application fill and refill the
		 * sound-stream buffer with new sound data as needed. The returned instance should be destroyed 
		 * by the caller (using the delete keyword). when it is no longer needed.
		 *
		 * See the documentation for Platform_Sound_SoundStream for further information.
		 *
		 * \returns	A new sound stream object, or 0 if no more sound streams could be created. 
		 */
		virtual Platform_Sound_SoundStream* CreateSoundStream(
			int channels,	///< The number of channels. Use 1 for mono, 2 for stereo
			int frequency,	///< The frequency, in Hz (number of samples per second) 
			int bitsPerSample,	///< Number of bits per sample, typically 8 or 16
			int size	///< The size, in bytes, of the sound stream buffer
			) = 0;
	};


#endif /* __Platform_Sound_H__ */
