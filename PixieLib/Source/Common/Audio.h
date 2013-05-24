/**
 * \class	Audio
 * 
 * \ingroup	audio
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	Add WAV format
 * \todo	Convert sound type (freq,chan,bps) in realtime (all sounds (not music) play as a single, specifiable format. default is 44khz 2ch 16bps)
 * \todo	Shared temp buffers for streaming music and sound data
 * \todo	Support built-in cross-fading of music
 * \todo	Clean up and finish off sound/soundinstance and priorities
 */

#ifndef __Audio_H__
#define __Audio_H__

// Includes
#include "Singleton.h"
#include "Array.h"

// Forward declares
class Music;
class Platform_Sound_SoundStream;
class Sound;
class SoundInstance;
class AudioFormat;
class Asset;

const int MaxSimultaneousSounds = 16;
const int Sound_BufferSize = 16*1024;
const int Music_BufferSize=1024*256; // 256k sound buffer

// Audio
class Audio:public Singleton<Audio>
	{
	public:
		Audio();
		virtual ~Audio();	

		void Update();
		void Register(Music* stream);
		void Unregister(Music* stream);

		unsigned int PlaySound(const Sound& sound, float priority = 0, bool looping = false);
		SoundInstance* GetSoundInstance(unsigned int handle);

		Platform_Sound_SoundStream* AcquireSoundStream(SoundInstance* instance);
		void ReleaseSoundStream(SoundInstance* instance);

		void* GetStreamBuffer(int minSize);
	private:
		Array<Music*> music_;

		struct SoundInstanceEntry
			{
			unsigned int handle;
			SoundInstance* instance;
			};
		Array<SoundInstanceEntry> soundInstances_;
		unsigned int currentSoundInstanceHandle_;

		Array<Platform_Sound_SoundStream*> freeSoundStreams_;
		struct SoundStreamEntry
			{
			SoundInstance* instanceAssignedTo;
			Platform_Sound_SoundStream* stream;
			};
		Array<SoundStreamEntry> assignedSoundStreams_;

		void* streamBuffer_;
		int streamBufferSize_;

	};

#define siAudio Audio::GetInstance()

#endif /* __Audio_H__ */

