/**
 * \class	AudioFormat
 * 
 * \ingroup	audio
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __AudioFormat_H__
#define __AudioFormat_H__

// Includes
#include "Array.h"

// Forward declares
class Asset;


// AudioFormat
class AudioFormat
	{
	public:
		/**
		 * Destructor
		 */
		virtual ~AudioFormat() { } 

		/**
		 * Returns the number of channels this audio data is using
		 * 1=mono, 2=stereo
		 */
		virtual int GetChannels() = 0;

		/**
		 * Returns the frequency of the audio data in number of 
		 * samples per second
		 */
		virtual int GetFrequency() = 0;

		/**
		 * Returns the number of bits per sample, normally 8 or 16
		 */
		virtual int GetBitsPerSample() = 0;

		/**
		 * Returns the total size (in bytes) of the raw audio data. To calculate the length
		 * of the data in seconds, use this:
		 *	float length=((float)GetSize())/(float)(GetChannels()*GetFrequency()*GetBitsPerSample())
		 */
		virtual int GetSize() = 0;

		/**
		 * Returns the current position (in bytes) in the raw audio data stream, meaning the
		 * position from which the next call to CopySoundChunk will start outputting data.
		 * To convert this position to second, use the same formula as described for GetSize
		 */
		virtual int GetPosition() = 0;

		/**
		 * Sets the current position (in bytes) in the raw audio data stream, meaning the
		 * position from which the next call to CopySoundChunk will start outputting data.
		 */
		virtual void SetPosition(
			int position	///< Position in bytes from the start
			) = 0;

		/**
		 * Copy a chunk of the raw audio data to the specified memory buffer. The requested
		 * number of bytes will be written to the buffer, unless the end of the audio data
		 * is reached. The total number of bytes written is always returned.
		 */
		virtual int CopySoundChunk(
			void* buffer,	///< Buffer to write data to. Must be large enough to hold the requested number of bytes
			int bytes	///< Number of bytes to write. The actual number of bytes written may be less than this if the end of the data is reached.
			) = 0;


		static AudioFormat* CreateAudioFormat(const Asset& asset);

	protected:
		static void RegisterAudioFormat(bool (*TestFunction)(const Asset&),AudioFormat* (*CreateFunction)(const Asset&));

	private:
		struct AudioFormatEntry
			{
			bool (*TestFunction)(const Asset&);
			AudioFormat* (*CreateFunction)(const Asset&);
			};
		static Array<AudioFormatEntry> audioFormats_;
	
	};

#endif /* __AudioFormat_H__ */
