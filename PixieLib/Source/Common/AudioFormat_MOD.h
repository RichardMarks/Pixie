/**
 * \class	AudioFormat_MOD
 * 
 * \ingroup	audio
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __AudioFormat_MOD_H__
#define __AudioFormat_MOD_H__

// Includes
#include "AudioFormat.h"

// Forward declares
struct _ModPlugFile;

// AudioFormat_MOD
class AudioFormat_MOD:public AudioFormat
	{
	public:
		static void Register();
		static bool TestAsset(const Asset& asset);
		static AudioFormat* Create(const Asset& asset);

		/**
		 * Constructor
		 */
		AudioFormat_MOD(
			const Asset& asset
			);
		
		/**
		 * Destructor
		 */
		virtual ~AudioFormat_MOD();

		/**
		 * Returns the number of channels this audio data is using
		 * 1=mono, 2=stereo
		 */
		virtual int GetChannels();

		/**
		 * Returns the frequency of the audio data in number of 
		 * samples per second
		 */
		virtual int GetFrequency();

		/**
		 * Returns the number of bits per sample, normally 8 or 16
		 */
		virtual int GetBitsPerSample();

		/**
		 * Returns the total size (in bytes) of the raw audio data. To calculate the length
		 * of the data in seconds, use this:
		 *	float length=((float)GetSize())/(float)(GetChannels()*GetFrequency()*GetBitsPerSample())
		 */
		virtual int GetSize();

		/**
		 * Returns the current position (in bytes) in the raw audio data stream, meaning the
		 * position from which the next call to CopySoundChunk will start outputting data.
		 * To convert this position to second, use the same formula as described for GetSize
		 */
		virtual int GetPosition();

		/**
		 * Sets the current position (in bytes) in the raw audio data stream, meaning the
		 * position from which the next call to CopySoundChunk will start outputting data.
		 */
		virtual void SetPosition(
			int position	///< Position in bytes from the start
			);

		/**
		 * Copy a chunk of the raw audio data to the specified memory buffer. The requested
		 * number of bytes will be written to the buffer, unless the end of the audio data
		 * is reached. The total number of bytes written is always returned.
		 */
		virtual int CopySoundChunk(
			void* buffer,	///< Buffer to write data to. Must be large enough to hold the requested number of bytes
			int bytes	///< Number of bytes to write. The actual number of bytes written may be less than this if the end of the data is reached.
			);

	private:
		_ModPlugFile* modFile_;
		int position_;
	};

#endif /* __AudioFormat_MOD_H__ */
