//*** Platform_Win32_Sound_SoundStream_WMM.cpp ***

#include "Platform_OS.h"

#pragma warning( disable: 4201)
#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <windows.h>
#include <mmsystem.h>
#include <malloc.h>
#pragma comment (lib, "winmm.lib")

#include "Platform_Win32_Sound_SoundStream_WMM.h"
#include "Platform_Win32_Sound_WMM.h"
#include "Platform_Win32_Sound.h"



//*** waveCriticalSection ***

CRITICAL_SECTION Platform_Win32_Sound_SoundStream_WMM::waveCriticalSection_;


//*** waveOutProc ***

void CALLBACK Platform_Win32_Sound_SoundStream_WMM::waveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
    {
    // Get the pointer to free block counter
    int& firstUsedWaveBlock_=*(reinterpret_cast<int*>(dwInstance));

    // ignore calls that occur due to openining and closing the device.
    if (uMsg!=WOM_DONE)
		{
		return;
		}

	EnterCriticalSection(&waveCriticalSection_);
    firstUsedWaveBlock_++;
	firstUsedWaveBlock_%=WaveBlockCount;
    LeaveCriticalSection(&waveCriticalSection_);
	}


//*** Constructor ***

Platform_Win32_Sound_SoundStream_WMM::Platform_Win32_Sound_SoundStream_WMM(Platform_Win32_Sound_WMM* manager, int channels, int frequency, int bitsPerSample, int size):
	manager_(manager),
	channels_(channels),
	frequency_(frequency),
	bitsPerSample_(bitsPerSample),
	size_(size),
	firstUsedWaveBlock_(0),
	lastUsedWaveBlock_(0),
	waveBlocksBuffer_(0),
	soundBuffer_(0),
	soundPosition_(0),
	waveBlocks_(0),
	playing_(false)
	{
    InitializeCriticalSection(&waveCriticalSection_);
 
	soundBuffer_=malloc(size);

	// Allocate blocks
	waveBlocks_=new WAVEHDR[WaveBlockCount];
	waveBlocksBuffer_=malloc(WaveBlockSize*WaveBlockCount);
	for (int i=0; i<WaveBlockCount; i++)
		{
		memset(&waveBlocks_[i],0,sizeof(waveBlocks_[i]));
		waveBlocks_[i].dwBufferLength=WaveBlockSize;
        waveBlocks_[i].lpData=static_cast<char*>(waveBlocksBuffer_)+WaveBlockSize*i;
 		}


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
	
    // try to open the default wave device. WAVE_MAPPER is
    // a constant defined in mmsystem.h, it always points to the
    // default wave device on the system (some people have 2 or
    // more sound cards).   
    MMRESULT result=waveOutOpen(&hWaveOut_, WAVE_MAPPER, &format, (DWORD_PTR)waveOutProc, (DWORD_PTR)&firstUsedWaveBlock_, CALLBACK_FUNCTION);
	if(result!=MMSYSERR_NOERROR)
		{
		Platform::GetPlatform_OS()->OutputDebugText("Couldn't open wave device\n");
		}
	}


//*** Destructor ***

Platform_Win32_Sound_SoundStream_WMM::~Platform_Win32_Sound_SoundStream_WMM()
	{
	Stop();
    DeleteCriticalSection(&waveCriticalSection_);

	waveOutClose(hWaveOut_);
	free(waveBlocksBuffer_);
	delete[] waveBlocks_;
	free(soundBuffer_);
	manager_->SoundStreamDestroyed(this);
	}


void Platform_Win32_Sound_SoundStream_WMM::Play()
	{
	if (playing_)
		{
		return;
		}
	for (int i=0; i<WaveBlockCount; i++)
		{
        if(waveBlocks_[i].dwFlags & WHDR_PREPARED) 
			{
			waveOutUnprepareHeader(hWaveOut_, &waveBlocks_[i], sizeof(WAVEHDR));
			}
		int bytesToWrite=WaveBlockSize-(int)(waveBlocks_[i].dwUser);
		int remainder=0;
		if (soundPosition_+bytesToWrite>size_)
			{
			int roomLeft=size_-soundPosition_;
			remainder=bytesToWrite-roomLeft;
			bytesToWrite=roomLeft;
			}
        memcpy(waveBlocks_[i].lpData+waveBlocks_[i].dwUser, static_cast<char*>(soundBuffer_)+soundPosition_,bytesToWrite);
		soundPosition_+=bytesToWrite;
		soundPosition_%=size_;
		if (remainder>0)
			{
	        memcpy(waveBlocks_[i].lpData+waveBlocks_[i].dwUser+bytesToWrite, static_cast<char*>(soundBuffer_)+soundPosition_,remainder);
			soundPosition_+=remainder;
			soundPosition_%=size_;
			}
        waveBlocks_[i].dwBufferLength = WaveBlockSize;
        waveOutPrepareHeader(hWaveOut_, &waveBlocks_[i], sizeof(WAVEHDR));
        waveOutWrite(hWaveOut_, &waveBlocks_[i], sizeof(WAVEHDR));

		EnterCriticalSection(&waveCriticalSection_);
        firstUsedWaveBlock_=0;
        LeaveCriticalSection(&waveCriticalSection_);
        lastUsedWaveBlock_=WaveBlockCount-1;
		}
	playing_=true;
	}	


//*** Stop ***

void Platform_Win32_Sound_SoundStream_WMM::Stop()
	{
	if (!playing_)
		{
		return;
		}


	// Wait for all blocks to finish
	bool done=false;
	while (!done)
		{
		done=true;
		for (int i=0; i<WaveBlockCount; i++)
			{
			if(waveBlocks_[i].dwFlags!=0 /* or it doesn't work with no sound card */ && !(waveBlocks_[i].dwFlags & WHDR_DONE))
				{
				done=false;
				break;
				}
			}
		if (!done)
			{
			Sleep(0);
			}
		}
		
    for (int i=0; i<WaveBlockCount; i++) 
		{
		if(waveBlocks_[i].dwFlags & WHDR_PREPARED)
			{
		    waveOutUnprepareHeader(hWaveOut_, &waveBlocks_[i], sizeof(WAVEHDR));
			}
		}
	playing_=false;
	}	


//*** GetPosition ***
	
int Platform_Win32_Sound_SoundStream_WMM::GetPosition()
	{
	return soundPosition_;
	}


//*** SetPosition ***
	
void Platform_Win32_Sound_SoundStream_WMM::SetPosition(int position)
	{
	soundPosition_=position;
	}


//*** CopySoundToBuffer ***

void Platform_Win32_Sound_SoundStream_WMM::CopySoundToBuffer(int bufferOffset, const void* soundData, int bytesToCopy)
	{
	memcpy(static_cast<unsigned char*>(soundBuffer_)+bufferOffset,soundData,bytesToCopy);
	}
	

//*** Update ***

void Platform_Win32_Sound_SoundStream_WMM::Update()
	{
	if (!playing_)
		{
		return;
		}

	// If all block are done, it means we've lost the buffers and need to refill from start
	bool allDone=true;
	for (int i=0; i<WaveBlockCount; i++)
		{
		if(!(waveBlocks_[i].dwFlags & WHDR_DONE))
			{
			allDone=false;
			break;
			}
		}
    if (allDone)
		{
		Stop();
		Play();
		return;
		}

	// If we haven't yet played half of the queued blocks, we don't do anything yet
	int lastBlockCorrected=lastUsedWaveBlock_%WaveBlockCount;
	if (lastBlockCorrected<firstUsedWaveBlock_)
		{
		lastBlockCorrected=(WaveBlockCount+lastBlockCorrected);
		}
	
	if ((lastBlockCorrected-firstUsedWaveBlock_+1)<WaveBlockCount)
		{
		lastUsedWaveBlock_=(lastUsedWaveBlock_+1)%WaveBlockCount;
        if(waveBlocks_[lastUsedWaveBlock_].dwFlags & WHDR_PREPARED) 
			{
			waveOutUnprepareHeader(hWaveOut_, &waveBlocks_[lastUsedWaveBlock_], sizeof(WAVEHDR));
			}

		int bytesToWrite=WaveBlockSize-((int)waveBlocks_[lastUsedWaveBlock_].dwUser);
		int remainder=0;
		if (soundPosition_+bytesToWrite>size_)
			{
			int roomLeft=size_-soundPosition_;
			remainder=bytesToWrite-roomLeft;
			bytesToWrite=roomLeft;
			}
        memcpy(waveBlocks_[lastUsedWaveBlock_].lpData+waveBlocks_[lastUsedWaveBlock_].dwUser, static_cast<char*>(soundBuffer_)+soundPosition_,bytesToWrite);
		soundPosition_+=bytesToWrite;
		soundPosition_%=size_;
		if (remainder>0)
			{
	        memcpy(waveBlocks_[lastUsedWaveBlock_].lpData+waveBlocks_[lastUsedWaveBlock_].dwUser+bytesToWrite, static_cast<char*>(soundBuffer_)+soundPosition_,remainder);
			soundPosition_+=remainder;
			soundPosition_%=size_;
			}
        waveBlocks_[lastUsedWaveBlock_].dwBufferLength = WaveBlockSize;
        waveOutPrepareHeader(hWaveOut_, &waveBlocks_[lastUsedWaveBlock_], sizeof(WAVEHDR));
        waveOutWrite(hWaveOut_, &waveBlocks_[lastUsedWaveBlock_], sizeof(WAVEHDR));
		}

	}


//*** ClearBuffer ***

void Platform_Win32_Sound_SoundStream_WMM::ClearBuffer(int bufferOffset, int bytesToClear)
	{
	memset(soundBuffer_,0,size_);
	}

	
//*** GetChannels ***

int Platform_Win32_Sound_SoundStream_WMM::GetChannels()
	{
	return channels_;
	}


//*** GetFrequency ***

int Platform_Win32_Sound_SoundStream_WMM::GetFrequency()
	{
	return frequency_;
	}


//*** GetBitsPerSample ***

int Platform_Win32_Sound_SoundStream_WMM::GetBitsPerSample()
	{
	return bitsPerSample_;
	}


//*** GetSize ***

int Platform_Win32_Sound_SoundStream_WMM::GetSize()
	{
	return size_;
	}


//*** SetVolume ***

void Platform_Win32_Sound_SoundStream_WMM::SetVolume(float level)
	{

	}


//*** GetVolume ***

float Platform_Win32_Sound_SoundStream_WMM::GetVolume()
	{
	return 1.0f;
	}
