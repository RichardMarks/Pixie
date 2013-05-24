//*** Platform_Win32_Sound_SoundStream_DSound.cpp ***

#include "Platform_Win32_Sound_SoundStream_DSound.h"
#include "Platform_Win32_Sound.h"
#include "Platform_OS.h"

//#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN

// DirectX version 3 is enough for sound
#define DIRECTSOUND_VERSION 0x0300
#include <dsound.h>
#include <math.h>

#define ErrMsg(expression,message) if (!(expression))  Platform::GetPlatform_OS()->OutputDebugText(message);

//*** Constructor ***

Platform_Win32_Sound_SoundStream_DSound::Platform_Win32_Sound_SoundStream_DSound(IDirectSound* directSound, int channels, int frequency, int bitsPerSample, int size):
	soundBuffer_(0),
	channels_(channels),
	frequency_(frequency),
	bitsPerSample_(bitsPerSample),
	size_(size),
	volume_(1.0f)
	{
    // Set up wave format structure. 
	WAVEFORMATEX format; 
    memset(&format, 0, sizeof(WAVEFORMATEX)); 
	format.wFormatTag=WAVE_FORMAT_PCM; 
    format.nChannels=(WORD)channels; 
    format.nSamplesPerSec=(DWORD)frequency; 
    format.nBlockAlign=(WORD)((channels*bitsPerSample)/8); 
    format.nAvgBytesPerSec=(DWORD)(frequency * format.nBlockAlign); 
    format.wBitsPerSample=(WORD)bitsPerSample; 
    format.cbSize=0;

	// Set up DSBUFFERDESC structure. 
    DSBUFFERDESC dsbdesc; 
    memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); // Zero it out. 
    dsbdesc.dwSize=sizeof(DSBUFFERDESC); 
    
	// Need default controls (pan, volume, frequency). 
    dsbdesc.dwFlags=DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS; 
    
    dsbdesc.dwBufferBytes=size; 
    dsbdesc.lpwfxFormat=&format; 
    
	// Create buffer. 
    HRESULT hr; 
    hr=directSound->CreateSoundBuffer(&dsbdesc, &soundBuffer_, NULL); 
	ErrMsg(SUCCEEDED(hr),"Couldn't create sound buffer\n");
	}


//*** Destructor ***

Platform_Win32_Sound_SoundStream_DSound::~Platform_Win32_Sound_SoundStream_DSound()
	{
	if (soundBuffer_)
		{
		soundBuffer_->Release();
		}
	}


void Platform_Win32_Sound_SoundStream_DSound::Play()
	{
	if (soundBuffer_)
		{
		soundBuffer_->Play(0,0,DSBPLAY_LOOPING);
		}
	}	


//*** Stop ***

void Platform_Win32_Sound_SoundStream_DSound::Stop()
	{
	if (soundBuffer_)
		{
		soundBuffer_->Stop();
		}
	}	


//*** GetPosition ***
	
int Platform_Win32_Sound_SoundStream_DSound::GetPosition()
	{
	DWORD position=0;
	if (soundBuffer_)
		{
		soundBuffer_->GetCurrentPosition(&position,0);
		}
	return (int)position;
	}


//*** SetPosition ***
	
void Platform_Win32_Sound_SoundStream_DSound::SetPosition(int position)
	{
	if (soundBuffer_)
		{
		soundBuffer_->SetCurrentPosition(position);
		}
	}


//*** CopySoundToBuffer ***

void Platform_Win32_Sound_SoundStream_DSound::CopySoundToBuffer(int bufferOffset, const void* soundData, int bytesToCopy)
	{
	if (!soundBuffer_)
		{
		return;
		}

	// Check the range of the bytesToCopy parameter
	ErrMsg((bufferOffset+bytesToCopy)<=size_,"Sound buffer out of range\n");

	// Obtain memory address of write block. This will be in two parts
    // if the block wraps around.
    LPVOID lpvPtr1; 
    DWORD dwBytes1; 
    LPVOID lpvPtr2; 
    DWORD dwBytes2; 
    HRESULT hr = soundBuffer_->Lock(bufferOffset, bytesToCopy, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0); 
 
    // If DSERR_BUFFERLOST is returned, restore and retry lock. 
    if (hr==DSERR_BUFFERLOST) 
		{ 
        soundBuffer_->Restore(); 
	    hr = soundBuffer_->Lock(bufferOffset, bytesToCopy, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0); 
		} 
    ErrMsg(SUCCEEDED(hr),"Couldn't lock sound buffer\n");

	// Write to pointers.      
	CopyMemory(lpvPtr1, soundData, dwBytes1);      
	if(lpvPtr2) 
		{ 
        CopyMemory(lpvPtr2, static_cast<const char*>(soundData)+dwBytes1, dwBytes2); 
	    } 

	// Release the data back to DirectSound. 
    hr=soundBuffer_->Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2); 
	ErrMsg(SUCCEEDED(hr),"Couldn't unlock sound buffer\n");		
	}


//*** ClearBuffer ***

void Platform_Win32_Sound_SoundStream_DSound::ClearBuffer(int bufferOffset, int bytesToClear)
	{
	if (!soundBuffer_)
		{
		}
		// Check the range of the bytesToClear parameter
	ErrMsg((bufferOffset+bytesToClear)<=size_,"Sound buffer out of range\n");

	// Obtain memory address of write block. This will be in two parts
    // if the block wraps around.
	LPVOID lpvPtr1; 
    DWORD dwBytes1; 
    LPVOID lpvPtr2; 
    DWORD dwBytes2; 
    HRESULT hr = soundBuffer_->Lock(bufferOffset, bytesToClear, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0); 
 
    // If DSERR_BUFFERLOST is returned, restore and retry lock. 
    if (hr==DSERR_BUFFERLOST) 
		{ 
        soundBuffer_->Restore(); 
	    hr = soundBuffer_->Lock(bufferOffset, bytesToClear, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0); 
		} 
    ErrMsg(SUCCEEDED(hr),"Couldn't lock sound buffer\n");

	// Write to pointers.      
	FillMemory(lpvPtr1, dwBytes1,0);      
	if(lpvPtr2) 
		{ 
        FillMemory(lpvPtr2, dwBytes2, 0); 
	    } 

	// Release the data back to DirectSound. 
    hr=soundBuffer_->Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2); 
	ErrMsg(SUCCEEDED(hr),"Couldn't unlock sound buffer\n");		
	}
	

//*** GetChannels ***

int Platform_Win32_Sound_SoundStream_DSound::GetChannels()
	{
	return channels_;
	}


//*** GetFrequency ***

int Platform_Win32_Sound_SoundStream_DSound::GetFrequency()
	{
	return frequency_;
	}


//*** GetBitsPerSample ***

int Platform_Win32_Sound_SoundStream_DSound::GetBitsPerSample()
	{
	return bitsPerSample_;
	}


//*** GetSize ***

int Platform_Win32_Sound_SoundStream_DSound::GetSize()
	{
	return size_;
	}


//*** ConvertLinearLevelToDirectSoundLevel ***

int Platform_Win32_Sound_SoundStream_DSound::ConvertLinearLevelToDirectSoundLevel(float level)
	{
	if (level<0.0001f)
		{
		return -DSBVOLUME_MIN;
		}
	return (int)(2000.0f*log10f(level));
	}




//*** SetVolume ***

void Platform_Win32_Sound_SoundStream_DSound::SetVolume(float level)
	{
	volume_=level;
	if (soundBuffer_)
		{
		LONG value=ConvertLinearLevelToDirectSoundLevel(level);
		soundBuffer_->SetVolume(value);
		}
	}


//*** GetVolume ***

float Platform_Win32_Sound_SoundStream_DSound::GetVolume()
	{
	return volume_;
	}
