//*** Platform_Win32_Sound_NoSound.cpp ***

#include "Platform_Win32_Sound_NoSound.h"
#include "Platform_Win32_Sound_SoundStream_NoSound.h"

//*** Constructor ***

Platform_Win32_Sound_NoSound::Platform_Win32_Sound_NoSound()
	{

	}


//*** Setup ***

bool Platform_Win32_Sound_NoSound::Setup()
	{
	return true;
	}


//*** Start ***

Platform_Win32_Sound_NoSound::~Platform_Win32_Sound_NoSound()
	{
	}


//*** Update ***

void Platform_Win32_Sound_NoSound::Update()
	{
	}


//*** CreateSoundStream ***

Platform_Sound_SoundStream* Platform_Win32_Sound_NoSound::CreateSoundStream(int channels, int frequency, int bitsPerSample, int size)
	{
	return new Platform_Win32_Sound_SoundStream_NoSound(channels,frequency,bitsPerSample,size);
	}
