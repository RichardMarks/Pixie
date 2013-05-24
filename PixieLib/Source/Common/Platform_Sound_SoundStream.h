/**
 * \class	Platform_Sound_SoundStream
 *
 * \ingroup	platform
 * \brief	Platform independent abstraction of a soundstream
 * \author	Mattias Gustavsson	
 * 
 * Implements a sound channel for streaming sound. The idea is that you
 * create a soundstream, fill it with the first bit of data from your 
 * sound, and then start playing it. You then monitor its current position,
 * and when it's gone past half the buffer size, you fill in the first half
 * of the buffer with more sound data, keep monitoring the buffer, and when
 * the position have wrapped around, you fill the second half with more 
 * data, and so on. This means you can use and reuse soundstreams, and 
 * play both short and long sounds, or even music, with any soundstream.
 */

#ifndef __Platform_Sound_Soundstream_h__
#define __Platform_Sound_Soundstream_h__

// Includes

// Forward declares


// Platform_Sound_SoundStream
class Platform_Sound_SoundStream
	{
	public:
		/**
		 * Destructor
		 */
		virtual ~Platform_Sound_SoundStream() { };
		

		/**
		 * Retrieves the number of channels this sound stream was created with.
		 *
		 * \returns	The number of channels, 1=mono, 2=stereo
		 */
		virtual int GetChannels() = 0;


		/**
		 * Retrieves the frequency (samples per second) this sound stream was 
		 * created with. Typical values for this is 11025 (11 kHz), 22050 (22 kHz)
		 * or 44100 (44 kHz, CD-quality).
		 *
		 * \returns	The number of samples per second
		 */
		virtual int GetFrequency() = 0;


		/**
		 * Retrieves the number of bits per sample which this sound stream was 
		 * created with. This is typically 8 or 16 bits.
		 *
		 * \returns	The number of bits per sample (typically 8 or 16)
		 */
		virtual int GetBitsPerSample() = 0;


		/**
		 * Retrieves the number of bytes allocated for this sound streams buffer.
		 *
		 * \returns	The size of the sound stream buffer in bytes
		 */
		virtual int GetSize() = 0;


		/**
		 * Starts playing the sound stream, from whatever position it is currently at.
		 * The stream will keep playing and loop when it's reached the end, until the
		 * Stop method is called. The GetPosition method can be called to monitor the
		 * current position in the stream, and if looping is not desired, Stop should
		 * be called when the position have reached the end of the stream buffer.
		 */
		virtual void Play() = 0;


		/**
		 * Pause the playback, and keeps the position so that the next call to Play
		 * will resume from the last point.
		 */
		virtual void Stop() = 0;


		/**
		 * Retrieves the current position in the sound stream buffer of the playback.
		 *
		 * \returns	The current position
		 */
		virtual int GetPosition() = 0;


		/**
		 * Sets the playback position. Must be in the range from 0 to the size of the
		 * sound stream buffer. There is no need to call Stop/Play for it to take effect.
		 */
		virtual void SetPosition(
			int position	///< The new position
			) = 0;


		/**
		 * Retrieves the current volume level, in the range 0 to 1. This is a linear volume
		 * level value, where 0 is silent and 1 is full volume.
		 *
		 * \returns	The current volume
		 */
		virtual float GetVolume() = 0;


		/**
		 * Sets the current volume level, in the range 0 to 1. This is a linear volume
		 * level value, where 0 is silent and 1 is full volume.
		 */
		virtual void SetVolume(
			float level
			) = 0;

		
		/**
		 * Clears the specified part of the sound streams buffer. bufferOffset + bytesToClear
		 * should be less than the value returned by GetSize
		 */

		virtual void ClearBuffer(
			int bufferOffset,	///< Where in the sound stream buffer to clearing 
			int bytesToClear	///< Number of bytes to clear
			) = 0;


		/**
		 * Copies raw sound data to the sound streams buffer, starting at bufferOffset. The 
		 * data is copied from the soundData pointer, for a total of bytesToCopy number of
		 * bytes. bufferOffset + bytesToClear should be less than the value returned by GetSize.
		 * The format of the data is interleaved raw sound samples, as specified by the 
		 * combination of channels, frequency and bits per sample.
		 */
		virtual void CopySoundToBuffer(
			int bufferOffset,	///< Where in the sound stream buffer to start copying data to
			const void* soundData,	///< Sound data to copy into the sound stream buffer
			int bytesToCopy	///< Number of bytes to copy
			) = 0;

	};

#endif /* __Platform_Sound_Soundstream_h__ */
