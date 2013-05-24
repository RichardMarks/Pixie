//*** Platform_Win32_Sound_SoundStream_NoSound.cpp ***

#include "Platform_Win32_Sound_SoundStream_NoSound.h"


//*** Constructor ***

Platform_Win32_Sound_SoundStream_NoSound::Platform_Win32_Sound_SoundStream_NoSound(int channels, int frequency, int bitsPerSample, int size):
	channels_(channels),
	frequency_(frequency),
	bitsPerSample_(bitsPerSample),
	size_(size)
	{
	}


//*** Destructor ***

Platform_Win32_Sound_SoundStream_NoSound::~Platform_Win32_Sound_SoundStream_NoSound()
	{
	}


void Platform_Win32_Sound_SoundStream_NoSound::Play()
	{
	}	


//*** Stop ***

void Platform_Win32_Sound_SoundStream_NoSound::Stop()
	{
	}	


//*** GetPosition ***
	
int Platform_Win32_Sound_SoundStream_NoSound::GetPosition()
	{
	return 0;
	}


//*** SetPosition ***
	
void Platform_Win32_Sound_SoundStream_NoSound::SetPosition(int position)
	{
	}


//*** CopySoundToBuffer ***

void Platform_Win32_Sound_SoundStream_NoSound::CopySoundToBuffer(int bufferOffset, const void* soundData, int bytesToCopy)
	{
	}


//*** ClearBuffer ***

void Platform_Win32_Sound_SoundStream_NoSound::ClearBuffer(int bufferOffset, int bytesToClear)
	{
	}
	

//*** GetChannels ***

int Platform_Win32_Sound_SoundStream_NoSound::GetChannels()
	{
	return channels_;
	}


//*** GetFrequency ***

int Platform_Win32_Sound_SoundStream_NoSound::GetFrequency()
	{
	return frequency_;
	}


//*** GetBitsPerSample ***

int Platform_Win32_Sound_SoundStream_NoSound::GetBitsPerSample()
	{
	return bitsPerSample_;
	}


//*** GetSize ***

int Platform_Win32_Sound_SoundStream_NoSound::GetSize()
	{
	return size_;
	}


//*** SetVolume ***

void Platform_Win32_Sound_SoundStream_NoSound::SetVolume(float level)
	{

	}


//*** GetVolume ***

float Platform_Win32_Sound_SoundStream_NoSound::GetVolume()
	{
	return 1.0f;
	}
