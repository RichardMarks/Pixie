/**
 * \class	SoundInstance
 * 
 * \ingroup	audio
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __SoundInstance_H__
#define __SoundInstance_H__

// Includes
#include "DynamicBuffer.h"

// Forward declares
class Sound;
class Platform_Sound_SoundStream;

// SoundInstance
class SoundInstance
	{
	public:
		/**
		 * Constructor
		 */
		SoundInstance(
			const Sound& sound,
			float priority,
			bool looping
			);

		/**
		 * Destructor
		 */
		~SoundInstance();


		/**
		 * Resumes (or starts) playback of the sound from the current position.
		 * Will request a sound stream from the Audio system, and it will receive
		 * one if there is one available. This might mean that a sound with lower
		 * or equal priority to this one will be cut off to make a sound stream 
		 * available, but no sounds with higher priority will be cut off. If the
		 * audio system can't find, or make, a sound stream available, the sound
		 * will not be played. When the sound reaches the end, the assigned sound
		 * stream will be released for use by other sounds.
		 */
		void Play();

		/**
		 * Retrieves the priority value of the sound
		 */
		float GetPriority();

		/**
		 * Stops the playback of the sound, and resets position to 0. This will
		 * also result in any assigned sound stream to be released for use by
		 * other sounds.
		 */
		void Stop();

		/**
		 * Halts playback of the music, but doesn't reset position. This will
		 * also result in any assigned sound stream to be released for use by
		 * other sounds.
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

		const Sound* GetSound();
	private:
		friend class Audio;
		void Update();
		void CopySoundChunk(int bufferOffset,int maxBytes);

		int PositionFromTime(float time);
		float TimeFromPosition(int position);

	private:
		const Sound* sound_;

		bool looping_;
		float storedPosition_;
		bool playing_;
		float priority_; ///< When all available sound streams are in use, sounds with higher priority will never be cut off by sounds with lower priority
		float volume_;

		Platform_Sound_SoundStream* soundStream_; ///< SoundInstance stream, as assigned by Audio system (may be 0 at any given time)

		unsigned char* streamBuffer_;
		bool nextCopyToStart_;
		int lastCopySourceStart_;
		int lastCopyDestinationStart_;
		int lastCopyByteCount_;
		bool lastCopyReachedEnd_;
		int soundDefinitionCopyPosition_;
	};

#endif /* __SoundInstance_H__ */

