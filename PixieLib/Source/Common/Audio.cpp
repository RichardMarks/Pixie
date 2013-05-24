//*** Audio.cpp ***

#include "Audio.h"
#include "Debug.h"
#include "Platform_Sound_SoundStream.h"
#include "Platform_Sound.h"
#include "Music.h"
#include "Asset.h"
#include "SoundInstance.h"
#include "Sound.h"
#include "ArrayIterator.h"
#include "StandardLibrary.h"


//*** Constructor ***

Audio::Audio():
	streamBuffer_(0),
	streamBufferSize_(0),
	currentSoundInstanceHandle_(0)
	{
	}


//*** Destructor ***

Audio::~Audio()
	{
	for (int i=0; i<freeSoundStreams_.GetItemCount(); i++)
		{
		delete freeSoundStreams_.Get(i);
		}

	for (int i=0; i<assignedSoundStreams_.GetItemCount(); i++)
		{
		delete assignedSoundStreams_.Get(i).stream;
		}

	if (streamBuffer_)
		{
		Free(streamBuffer_);
		streamBuffer_=0;
		streamBufferSize_=0;
		}
	}


//*** Update ***

void Audio::Update()
	{
	// Update all music
	for (int i=0; i<music_.GetItemCount(); i++)
		{
		Music* music=music_.Get(i);
		music->Update();
		}

	for (int i=0; i<soundInstances_.GetItemCount(); i++)
		{
		SoundInstance* instance=soundInstances_.Get(i).instance;
		instance->Update();
		}
	}


//*** Register ***

void Audio::Register(Music* music)
	{
	music_.Add(music);
	}

	
//*** Unregister ***

void Audio::Unregister(Music* music)
	{
	ArrayIterator<Music*> it(music_);
	if (it.Find(music))
		{
		music_.Remove(it);
		}
	}


//*** PlaySound ***

unsigned int Audio::PlaySound(const Sound& sound, float priority, bool looping)
	{
	SoundInstanceEntry entry;
	entry.instance=new SoundInstance(sound,priority,looping);
	entry.handle=currentSoundInstanceHandle_;
	currentSoundInstanceHandle_++;
	soundInstances_.Add(entry);
	entry.instance->Play();
	return entry.handle;
	}


//*** GetSoundInstance ***

SoundInstance* Audio::GetSoundInstance(unsigned int handle)
	{
	for (int i=0; i<soundInstances_.GetItemCount(); i++)
		{
		if (soundInstances_.Get(i).handle==handle)
			{
			return soundInstances_.Get(i).instance;
			}
		}

	return 0;
	}


//*** GetStreamBuffer ***

void* Audio::GetStreamBuffer(int minSize)
	{
	if (!streamBuffer_ || streamBufferSize_<minSize)
		{
		if (streamBuffer_)
			{
			Free(streamBuffer_);
			streamBuffer_=0;
			}
		streamBufferSize_=minSize;
		streamBuffer_=Malloc(streamBufferSize_);
		}

	return streamBuffer_;
	}


//*** AcquireSoundStream ***

Platform_Sound_SoundStream* Audio::AcquireSoundStream(SoundInstance* instance)
	{
	const Sound* definition=instance->GetSound();
	int channels=definition->GetChannels();
	int frequency=definition->GetFrequency();
	int bitsPerSample=definition->GetBitsPerSample();

	// If there's a free stream, just use that
	if (freeSoundStreams_.GetItemCount()>0)
		{
		Platform_Sound_SoundStream* stream=freeSoundStreams_.Get(freeSoundStreams_.GetItemCount()-1);
		freeSoundStreams_.RemoveLast();

		if (stream->GetChannels()!=channels || stream->GetFrequency()!=frequency || stream->GetBitsPerSample()!=bitsPerSample)
			{
			delete stream;
			stream=Platform::GetPlatform_Sound()->CreateSoundStream(channels,frequency,bitsPerSample,Sound_BufferSize);
			}
		
		SoundStreamEntry entry;
		entry.instanceAssignedTo=instance;
		entry.stream=stream;
		assignedSoundStreams_.Add(entry);
		return stream;
		}

	// If we haven't reached the maximum yet, create a new one and use it
	if (assignedSoundStreams_.GetItemCount()<MaxSimultaneousSounds)
		{
		Platform_Sound_SoundStream* stream=Platform::GetPlatform_Sound()->CreateSoundStream(channels,frequency,bitsPerSample,Sound_BufferSize);
		
		SoundStreamEntry entry;
		entry.instanceAssignedTo=instance;
		entry.stream=stream;
		assignedSoundStreams_.Add(entry);
		return stream;
		}

	// Kill off a sound with lower or equal priority
	for (int i=0; i<assignedSoundStreams_.GetItemCount(); i++)
		{
		if (assignedSoundStreams_.Get(i).instanceAssignedTo->GetPriority()<=instance->GetPriority())
			{
			// Stop the sound, and release the sound stream it was using
			instance->Stop();
			
			// And return that stream
			Platform_Sound_SoundStream* stream=freeSoundStreams_.Get(freeSoundStreams_.GetItemCount()-1);
			freeSoundStreams_.RemoveLast();
			
			if (stream->GetChannels()!=channels || stream->GetFrequency()!=frequency || stream->GetBitsPerSample()!=bitsPerSample)
				{
				delete stream;
				stream=Platform::GetPlatform_Sound()->CreateSoundStream(channels,frequency,bitsPerSample,Sound_BufferSize);
				}

			SoundStreamEntry entry;
			entry.instanceAssignedTo=instance;
			entry.stream=stream;
			assignedSoundStreams_.Add(entry);
			return stream;
			}
		}

	// No stream available - that's fine too, just means the sound won't play
	return 0;
	}


//*** ReleaseSoundStream ***

void Audio::ReleaseSoundStream(SoundInstance* instance)
	{
	for (int i=0; i<assignedSoundStreams_.GetItemCount(); i++)
		{
		if (assignedSoundStreams_.Get(i).instanceAssignedTo==instance)
			{
			Platform_Sound_SoundStream* stream=assignedSoundStreams_.Get(i).stream;
			stream->Stop();
			assignedSoundStreams_.Remove(i);
			freeSoundStreams_.Add(stream);
			return;
			}
		}
	}
