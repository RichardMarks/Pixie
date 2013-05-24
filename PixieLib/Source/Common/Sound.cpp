//*** Sound.cpp **

#include "Sound.h"
#include "StandardLibrary.h"
#include "Asset.h"
#include "AudioFormat.h"
#include "Audio.h"


//*** Constructor ***

Sound::Sound(const Asset& asset):
	data_(0),
	channels_(0),
	frequency_(0),
	bitsPerSample_(0),
	size_(0)
	{
	AudioFormat* format=AudioFormat::CreateAudioFormat(asset);
	channels_=format->GetChannels();
	frequency_=format->GetFrequency();
	bitsPerSample_=format->GetBitsPerSample();
	size_=format->GetSize();
	data_=Malloc(size_);
	
	// Decode entire file to memory
	format->CopySoundChunk(data_,size_);
	
	delete format;
	}


//*** Destructor ***

Sound::~Sound()
	{
	Free(data_);
	}


//*** Play ***

void Sound::Play(float priority, bool looping) const
	{
	siAudio->PlaySound(*this,priority,looping);
	}


//*** GetData ***

const void* Sound::GetData() const
	{
	return data_;
	}


//*** GetChannels ***

int Sound::GetChannels() const
	{
	return channels_;
	}


//*** GetFrequency ***

int Sound::GetFrequency() const
	{
	return frequency_;
	}


//*** GetBitsPerSample ***

int Sound::GetBitsPerSample() const
	{
	return bitsPerSample_;
	}


//*** GetSize ***

int Sound::GetSize() const
	{
	return size_;
	}
