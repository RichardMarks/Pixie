//*** Platform_NULL_Sound_SoundStream.cpp ***

#include "Platform_NULL_Sound_SoundStream.h"


//*** Constructor ***

Platform_NULL_Sound_SoundStream::Platform_NULL_Sound_SoundStream(int channels, int frequency, int bitsPerSample, int size):
	channels_(channels),
	frequency_(frequency),
	bitsPerSample_(bitsPerSample),
	size_(size)
	{
	}



void Platform_NULL_Sound_SoundStream::Play()
	{
	}	


//*** Stop ***

void Platform_NULL_Sound_SoundStream::Stop()
	{
	}	


//*** GetPosition ***
	
int Platform_NULL_Sound_SoundStream::GetPosition()
	{
	return 0;
	}


//*** SetPosition ***
	
void Platform_NULL_Sound_SoundStream::SetPosition(int position)
	{
	}


//*** CopySoundToBuffer ***

void Platform_NULL_Sound_SoundStream::CopySoundToBuffer(int bufferOffset, const void* soundData, int bytesToCopy)
	{
	}
	

//*** ClearBuffer ***

void Platform_NULL_Sound_SoundStream::ClearBuffer(int bufferOffset, int bytesToClear)
	{
	}

	
//*** GetChannels ***

int Platform_NULL_Sound_SoundStream::GetChannels()
	{
	return channels_;
	}


//*** GetFrequency ***

int Platform_NULL_Sound_SoundStream::GetFrequency()
	{
	return frequency_;
	}


//*** GetBitsPerSample ***

int Platform_NULL_Sound_SoundStream::GetBitsPerSample()
	{
	return bitsPerSample_;
	}


//*** GetSize ***

int Platform_NULL_Sound_SoundStream::GetSize()
	{
	return size_;
	}


//*** SetVolume ***

void Platform_NULL_Sound_SoundStream::SetVolume(float level)
	{
	}


//*** GetVolume ***

float Platform_NULL_Sound_SoundStream::GetVolume()
	{
	return 0;
	}
