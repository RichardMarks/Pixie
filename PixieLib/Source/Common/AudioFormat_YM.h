/**
 * \class	AudioFormat_YM
 * 
 * \ingroup	audio
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __AudioFormat_YM_H__
#define __AudioFormat_YM_H__

// Includes
#include "AudioFormat.h"

// Forward declares
class CYmMusic;

// AudioFormat_YM
class AudioFormat_YM:public AudioFormat
	{
	public:
		static void Register();
		static bool TestAsset(const Asset& asset);
		static AudioFormat* Create(const Asset& asset);

		/**
		 * Constructor
		 */
		AudioFormat_YM(
			const Asset& asset
			);
		
		/**
		 * Destructor
		 */
		virtual ~AudioFormat_YM();

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

		/**
		 * The GetRegisters() method is only available for the YM audio format, and returns
		 * the raw register values as recorded on the ST, and is useful for implementing
		 * oldschool VU-meters
		 */

		struct YmRegisters
			{
			int Channel_A_fine_tune; // 8 bits
			int Channel_A_coarse_tune; // 4 bits
			int Channel_B_fine_tune; // 8 bits
			int Channel_B_coarse_tune; // 4 bits
			int Channel_C_fine_tune; // 8 bits
			int Channel_C_coarse_tune; // 4 bits
			int Noise_generator_control; // 5 bits
			int Mixer_control_IO_enable; // 8 bits
			int Channel_A_amplitude; // 5 bits
			int Channel_B_amplitude; // 5 bits
			int Channel_C_amplitude; // 5 bits
			int Envelope_period_fine_tune; // 8 bits
			int Envelope_period_coarse_tune; // 8 bits
			int Envelope_shape; // 4 bits
			};

		YmRegisters GetRegisters(float position);

	private:
		CYmMusic* ymFile_;

		int chunkStart_;
		int chunkEnd_;
	};

#endif /* __AudioFormat_YM_H__ */
