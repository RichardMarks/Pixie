//*** Music.cpp ***

#include "Music.h"
#include "Platform_Sound.h"
#include "Platform_Sound_SoundStream.h"
#include "Debug.h"
#include "Audio.h"
#include "Asset.h"
#include "AudioFormat.h"
#include "StandardLibrary.h"



//*** Constructor ***

Music::Music(const Asset& asset):
	soundStream_(0),
	format_(0),
	streamBuffer_(0),
	nextCopyToStart_(true),
	looping_(false),
	playing_(false),
	storedPosition_(0),
	lastCopySourceStart_(0),
	lastCopyDestinationStart_(0),
	lastCopyByteCount_(0),
	lastCopyReachedEnd_(false)
	{
	// Create music format
	format_=AudioFormat::CreateAudioFormat(asset);

	if (!format_)
		{
		return;
		}

	// Create sound buffer 
	if (Platform::GetPlatform_Sound())
		{
		soundStream_=Platform::GetPlatform_Sound()->CreateSoundStream(format_->GetChannels(),format_->GetFrequency(),format_->GetBitsPerSample(),Music_BufferSize);
		}
	streamBuffer_=static_cast<unsigned char*>(siAudio->GetStreamBuffer(Music_BufferSize));

	siAudio->Register(this);
	}


//*** Destructor ***

Music::~Music()
	{
	if (format_)
		{
		delete format_;
		format_=0;
		if (Audio::IsInstanceCreated())
			{
			siAudio->Unregister(this);
			}
		delete soundStream_;
		}
	}


//*** SetLooping ***

void Music::SetLooping(bool looping)
	{
	looping_=looping;
	}


//*** Play ***

void Music::Play()
	{
	if (!format_)
		{
		return;
		}

	if (IsPlaying())
		{
		Stop();
		}

	format_->SetPosition(PositionFromTime(storedPosition_));
	CopySoundChunk(0,Music_BufferSize);
	nextCopyToStart_=true;
	soundStream_->SetPosition(0);
	soundStream_->Play();
	playing_=true;
	}


//*** Stop ***

void Music::Stop()
	{
	if (!format_)
		{
		return;
		}

	Pause();
	storedPosition_=0;
	}


//*** Pause ***

void Music::Pause()
	{
	if (!format_)
		{
		return;
		}

	if (IsPlaying())
		{
		storedPosition_=GetPosition();
		soundStream_->Stop();
		playing_=false;
		}
	}


//*** IsPlaying ***

bool Music::IsPlaying()
	{
	return playing_;
	}


//*** GetLength ***

float Music::GetLength()
	{
	if (!format_)
		{
		return 0;
		}

	return TimeFromPosition(format_->GetSize());
	}


//*** GetPosition ***

float Music::GetPosition()
	{
	if (!format_)
		{
		return 0;
		}

	if (IsPlaying())
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

void Music::SetPosition(float position)
	{
	if (!format_)
		{
		return;
		}

	if (IsPlaying())
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

void Music::Update()
	{
	if (!format_)
		{
		return;
		}

	if (!IsPlaying())
		{
		return;
		}

	// Get the current position of the sound stream
	int streamPosition=soundStream_->GetPosition();

	// Check if we've reached the end of the sound data
	if (lastCopyReachedEnd_)
		{
		int halfStreamSize=Music_BufferSize/2;
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
		int halfStreamSize=Music_BufferSize/2;
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

void Music::CopySoundChunk(int bufferOffset,int maxBytes)
	{
	if (!format_)
		{
		return;
		}

	// Store the positions of format and stream buffers
	lastCopySourceStart_=format_->GetPosition();
	lastCopyDestinationStart_=bufferOffset;

	// Copy audio data to stream buffer
	lastCopyReachedEnd_=false;
	if (looping_)
		{
		// If in looping mode, keep copying until we've filled the whole buffer
		lastCopyByteCount_=0;
		while (lastCopyByteCount_<maxBytes)
			{
			int count=format_->CopySoundChunk(streamBuffer_+lastCopyByteCount_,maxBytes-lastCopyByteCount_);
			if (count<maxBytes)
				{
				format_->SetPosition(0);
				}
			lastCopyByteCount_+=count;
			}
		}
	else
		{
		lastCopyByteCount_=format_->CopySoundChunk(streamBuffer_,maxBytes);
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

int Music::PositionFromTime(float time)
	{
	if (!format_)
		{
		return 0;
		}
	float sampleSize=(float)(format_->GetChannels()*format_->GetFrequency()*(format_->GetBitsPerSample()/8));
	return (int)(time*sampleSize);
	}


//*** TimeFromPosition ***

float Music::TimeFromPosition(int position)
	{
	if (!format_)
		{
		return 0;
		}

	float sampleSize=(float)(format_->GetChannels()*format_->GetFrequency()*(format_->GetBitsPerSample()/8));
	return position/sampleSize;
	}


//*** GetVolume ***

float Music::GetVolume()
	{
	if (!format_)
		{
		return 0;
		}

	return soundStream_->GetVolume();
	}


//*** SetVolume ***

void Music::SetVolume(float level)
	{
	if (!format_)
		{
		return;
		}

	soundStream_->SetVolume(level);
	}

