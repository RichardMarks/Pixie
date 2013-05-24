//*** SoundInstance.cpp **

#include "SoundInstance.h"
#include "Platform_Sound.h"
#include "Platform_Sound_SoundStream.h"
#include "Debug.h"
#include "Audio.h"
#include "Asset.h"
#include "Sound.h"
#include "StandardLibrary.h"



//*** Constructor ***

SoundInstance::SoundInstance(const Sound& sound, float priority, bool looping):
	sound_(&sound),
	priority_(priority),
	looping_(looping),
	storedPosition_(0),
	playing_(false),
	volume_(1),
	soundStream_(0),
	streamBuffer_(0),
	nextCopyToStart_(true),
	lastCopySourceStart_(0),
	lastCopyDestinationStart_(0),
	lastCopyByteCount_(0),
	lastCopyReachedEnd_(false)
	{
	streamBuffer_=static_cast<unsigned char*>(siAudio->GetStreamBuffer(Sound_BufferSize));
	}


//*** Destructor ***

SoundInstance::~SoundInstance()
	{
	streamBuffer_=0;
	}


//*** GetPriority ***

float SoundInstance::GetPriority()
	{
	return priority_;
	}


//*** GetSound ***

const Sound* SoundInstance::GetSound()
	{
	return sound_;
	}


//*** Play ***

void SoundInstance::Play()
	{
	soundStream_=siAudio->AcquireSoundStream(this);
	if (soundStream_)
		{
		soundDefinitionCopyPosition_=0;
		CopySoundChunk(0,Sound_BufferSize);
		nextCopyToStart_=true;
		soundStream_->SetPosition(0);
		soundStream_->Play();
		playing_=true;
		}
	}


//*** Stop ***

void SoundInstance::Stop()
	{
	Pause();
	storedPosition_=0;
	}


//*** Pause ***

void SoundInstance::Pause()
	{
	if (soundStream_)
		{
		storedPosition_=GetPosition();
		soundStream_->Stop();
		siAudio->ReleaseSoundStream(this);
		soundStream_=0;
		}
	}


//*** GetLength ***

float SoundInstance::GetLength()
	{
	return TimeFromPosition(sound_->GetSize());
	}


//*** SoundInstance ***

float SoundInstance::GetPosition()
	{
	if (soundStream_)
		{
		int streamPosition=soundStream_->GetPosition();
		int halfStreamSize=Music_BufferSize/2;
		if (nextCopyToStart_)
			{
			return TimeFromPosition(lastCopySourceStart_+streamPosition);
			}
		else
			{
			return TimeFromPosition(lastCopySourceStart_+(streamPosition-halfStreamSize));
			}
		}
	else
		{
		return storedPosition_;
		}
	}


//*** SetPosition ***

void SoundInstance::SetPosition(float position)
	{
	if (soundStream_)
		{
		Pause();
		storedPosition_=Clamp(position,0.0f,GetLength());
		Play();
		}
	else
		{
		storedPosition_=Clamp(position,0.0f,GetLength());
		}
	}



//***  Update ***

void SoundInstance::Update()
	{
	if (!soundStream_)
		{
		return;
		}

	// Get the current position of the sound stream
	int streamPosition=soundStream_->GetPosition();

	// Check if we've reached the end of the sound data
	if (lastCopyReachedEnd_)
		{
		int halfStreamSize=Sound_BufferSize/2;
		if (nextCopyToStart_)
			{
			if (streamPosition>=halfStreamSize+lastCopyByteCount_)
				{
				Stop();
				}
			}
		else
			{
			if (streamPosition<halfStreamSize && streamPosition>=lastCopyByteCount_)
				{
				Stop();
				}
			}
		}
	else	// Fill the next chunk of the buffer, as needed
		{
		int halfStreamSize=Sound_BufferSize/2;
		if (nextCopyToStart_)
			{
			if (streamPosition>=halfStreamSize)
				{
				CopySoundChunk(0,halfStreamSize);
				nextCopyToStart_=false;
				}
			}
		else
			{
			if (streamPosition<halfStreamSize)
				{
				CopySoundChunk(halfStreamSize,halfStreamSize);
				nextCopyToStart_=true;
				}
			}
		}
	}


//*** CopySoundChunk ***

void SoundInstance::CopySoundChunk(int bufferOffset,int maxBytes)
	{
	// Store the positions of format and stream buffers
	lastCopySourceStart_=soundDefinitionCopyPosition_;
	lastCopyDestinationStart_=bufferOffset;

	// Copy audio data to stream buffer
	lastCopyReachedEnd_=false;
	if (looping_)
		{
		// If in looping mode, keep copying until we've filled the whole buffer
		lastCopyByteCount_=0;
		while (lastCopyByteCount_<maxBytes)
			{
			int count=Min(maxBytes-lastCopyByteCount_,sound_->GetSize()-soundDefinitionCopyPosition_);
			MemCpy(streamBuffer_+lastCopyByteCount_,static_cast<const unsigned char*>(sound_->GetData())+soundDefinitionCopyPosition_,count);
			if (count<maxBytes)
				{
				soundDefinitionCopyPosition_=0;
				}
			lastCopyByteCount_+=count;
			}
		}
	else
		{
		lastCopyByteCount_=Min(maxBytes,sound_->GetSize()-soundDefinitionCopyPosition_);
		MemCpy(streamBuffer_,static_cast<const unsigned char*>(sound_->GetData())+soundDefinitionCopyPosition_,lastCopyByteCount_);

		if (lastCopyByteCount_<maxBytes)
			{
			lastCopyReachedEnd_=true;
			}
		}

	// Copy from the stream buffer to the actual sound stream
	int bytesToCopy=Min(maxBytes,lastCopyByteCount_);
	if (bytesToCopy>0)
		{
		soundStream_->CopySoundToBuffer(bufferOffset,streamBuffer_,bytesToCopy);
		}
	soundDefinitionCopyPosition_+=bytesToCopy;

	// If we've reached the end of the track, pad the rest of the buffer with silence to avoid any ticks
	if (lastCopyReachedEnd_)
		{
		MemSet(streamBuffer_,0,maxBytes);
		if (maxBytes-bytesToCopy>0)
			{
			soundStream_->CopySoundToBuffer(bufferOffset+bytesToCopy,streamBuffer_,maxBytes-bytesToCopy);
			}
		}
	}



//*** PositionFromTime ***

int SoundInstance::PositionFromTime(float time)
	{
	float sampleSize=(float)(sound_->GetChannels()*sound_->GetFrequency()*(sound_->GetBitsPerSample()/8));
	return (int)(time*sampleSize);
	}


//*** TimeFromPosition ***

float SoundInstance::TimeFromPosition(int position)
	{
	float sampleSize=(float)(sound_->GetChannels()*sound_->GetFrequency()*(sound_->GetBitsPerSample()/8));
	return position/sampleSize;
	}


//*** GetVolume ***

float SoundInstance::GetVolume()
	{
	return soundStream_->GetVolume();
	}


//*** SetVolume ***

void SoundInstance::SetVolume(float level)
	{
	soundStream_->SetVolume(level);
	}

