//*** Platform_Win32_Sound_WMM.cpp ***


#pragma warning( disable: 4201)
#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <windows.h>
#include <mmsystem.h>
#include <malloc.h>

#include "Platform_Win32_Sound_WMM.h"
#include "Platform_Win32_Sound_SoundStream_WMM.h"


#pragma comment (lib, "winmm.lib")


//*** Constructor ***

Platform_Win32_Sound_WMM::Platform_Win32_Sound_WMM():
	soundStreamCount_(0),
	soundStreamMaxCount_(0),
	soundStreams_(0)
	{
	}


//*** Setup ***

bool Platform_Win32_Sound_WMM::Setup()
	{
	return true;
	}


//*** Destructor ***

Platform_Win32_Sound_WMM::~Platform_Win32_Sound_WMM()
	{
	if (soundStreams_)
		{
		for (int i=0; i<soundStreamCount_; i++)
			{
			delete soundStreams_[i];
			}
		free(soundStreams_);
		}
	}
		

//*** Update ***

void Platform_Win32_Sound_WMM::Update()
	{
	for (int i=0; i<soundStreamCount_; i++)
		{
		soundStreams_[i]->Update();
		}
	}


//*** CreateSoundStream ***

Platform_Sound_SoundStream* Platform_Win32_Sound_WMM::CreateSoundStream(int channels, int frequency, int bitsPerSample, int size)
	{
	Platform_Win32_Sound_SoundStream_WMM* soundStream=new Platform_Win32_Sound_SoundStream_WMM(this,channels,frequency,bitsPerSample,size);

	if (!soundStreams_)
		{
		soundStreamMaxCount_=64;
		soundStreams_=static_cast<Platform_Win32_Sound_SoundStream_WMM**>(malloc(soundStreamMaxCount_*sizeof(Platform_Win32_Sound_SoundStream_WMM*)));
		}
	else if (soundStreamCount_>=soundStreamMaxCount_)
		{
		soundStreamMaxCount_*=2;
		soundStreams_=static_cast<Platform_Win32_Sound_SoundStream_WMM**>(realloc(soundStreams_,soundStreamMaxCount_*sizeof(Platform_Win32_Sound_SoundStream_WMM*)));
		}

	soundStreams_[soundStreamCount_]=soundStream;
	soundStreamCount_++;
	
	return soundStream;
	}



//*** SoundStreamDestroyed ***

void Platform_Win32_Sound_WMM::SoundStreamDestroyed(Platform_Win32_Sound_SoundStream_WMM* stream)
	{
	for (int i=0; i<soundStreamCount_; i++)
		{
		if (soundStreams_[i]==stream)
			{
			soundStreams_[i]=soundStreams_[soundStreamCount_-1];
			soundStreamCount_--;
			return;
			}
		}
	}
