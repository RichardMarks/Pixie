/**
 * \class	Music
 *
 * \ingroup	audio
 * \brief
 * \author	Mattias Gustavsson
 *
 */

#ifndef __Music_H__
#define __Music_H__

// Includes
namespace pixie {
// Forward declares
class Asset;
class Platform_Sound_SoundStream;
class AudioFormat;


// Music
class Music
	{
	public:
		/**
		 * Constructor
		 */
		Music(
			const Asset& asset
			);

		/**
		 * Destructor
		 */
		~Music();

		/**
		 * Specifies whether this track should be looping or not. A looping
		 * track will start over automatically when the end is reached.
		 * The default is non-looping
		 */
		void SetLooping(bool looping);

		/**
		 * Resumes (or starts) playback of the music from the current position
		 */
		void Play();

		/**
		 * Stops the playback of the music, and resets position to 0
		 */
		void Stop();

		/**
		 * Halts playback of the music, but doesn't reset position
		 */
		void Pause();


		/**
		 * Returns true if the music is currently playing, and false if it has been stopped
		 * (by calling the Stop method or by reaching the end if playing without looping)
		 */
		bool IsPlaying();

		/**
		 * Returns the length of the track in seconds
		 */
		float GetLength();

		/**
		 * Returns the current position of the track in seconds from the start
		 */
		float GetPosition();

		/**
		 * Sets the current position of the track in seconds from the start
		 */
		void SetPosition(float position);

		/**
		 * Returns the current volume level in the range 0.0 to 1.0
		 */
		float GetVolume();

		/**
		 * Sets the current volume level in the range 0.0 to 1.0
		 */
		void SetVolume(float level);

	private:
		friend class Audio;
		void Update();
		void CopySoundChunk(int bufferOffset,int maxBytes);

		int PositionFromTime(float time);
		float TimeFromPosition(int position);

	private:
		bool looping_;
		float storedPosition_;
		bool playing_;


		Platform_Sound_SoundStream* soundStream_;
		AudioFormat* format_;
		unsigned char* streamBuffer_;
		bool nextCopyToStart_;
		int lastCopySourceStart_;
		int lastCopyDestinationStart_;
		int lastCopyByteCount_;
		bool lastCopyReachedEnd_;
	};

}; // namespace pixie
#endif /* __Music_H__ */
