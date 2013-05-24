//*** Platform_NULL_Sound.cpp ***

#include "Platform_NULL_Sound.h"
#include "Platform_NULL_Sound_SoundStream.h"


//*** Constructor ***

Platform_NULL_Sound::Platform_NULL_Sound()
	{
	}

		
//*** CreateSoundStream ***

Platform_Sound_SoundStream* Platform_NULL_Sound::CreateSoundStream(int channels, int frequency, int bitsPerSample, int size)
	{
	return new Platform_NULL_Sound_SoundStream(channels,frequency,bitsPerSample,size);
	}

