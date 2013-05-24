/**
 * \class	Sound
 * 
 * \ingroup	audio
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __Sound_H__
#define __Sound_H__

// Includes

// Forward declares
class Asset;

// Sound
class Sound
	{
	public:
		/**
		 * Constructor
		 */
		Sound(
			const Asset& asset
			);

		/**
		 * Destructor
		 */
		~Sound();



		void Play(float priority = 0, bool looping = false) const;

		const void* GetData() const;

		/**
		 * Returns the number of channels this sound is using
		 * 1=mono, 2=stereo
		 */
		int GetChannels() const;

		/**
		 * Returns the frequency of the sound data in number of 
		 * samples per second
		 */
		int GetFrequency() const;

		/**
		 * Returns the number of bits per sample, normally 8 or 16
		 */
		int GetBitsPerSample() const;

		/**
		 * Returns the total size (in bytes) of the sound data. To calculate the length
		 * of the data in seconds, use this:
		 *	float length=((float)GetSize())/(float)(GetChannels()*GetFrequency()*GetBitsPerSample())
		 */
		int GetSize() const;

	private:
		void* data_;
		int channels_;
		int frequency_;
		int bitsPerSample_;
		int size_;

	};

#endif /* __Sound_H__ */

