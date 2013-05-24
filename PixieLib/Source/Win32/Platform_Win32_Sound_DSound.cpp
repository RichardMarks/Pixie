//*** Platform_Win32_Sound_DSound.cpp ***

#include "Platform_Win32_Sound_DSound.h"
#include "Platform_Win32_Sound_SoundStream_DSound.h"
#include "Platform_OS.h"

//#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN


#define DIRECTSOUND_VERSION 0x0300
#include <dsound.h>


//*** Constructor ***

Platform_Win32_Sound_DSound::Platform_Win32_Sound_DSound(struct HWND__* windowHandle):
	windowHandle_(windowHandle),
	dsoundDLL_(0),
	directSound_(0)
	{

	}


//*** Setup ***

bool Platform_Win32_Sound_DSound::Setup()
	{
	dsoundDLL_=LoadLibrary("dsound.dll");

	if (dsoundDLL_)
		{
		typedef HRESULT (WINAPI *DirectSoundCreateDefinition)(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter);
		DirectSoundCreateDefinition DirectSoundCreate=(DirectSoundCreateDefinition)GetProcAddress((HMODULE)dsoundDLL_, "DirectSoundCreate"); 


		HRESULT hr;
		hr = DirectSoundCreate(NULL, &directSound_, NULL);
		if (!SUCCEEDED(hr))
			{
			Platform::GetPlatform_OS()->OutputDebugText("Couldn't create DirectSound object\n");
			return false;
			}
		
		hr = directSound_->SetCooperativeLevel(windowHandle_, /* DSSCL_EXCLUSIVE*/ DSSCL_NORMAL);
		if (!SUCCEEDED(hr))
			{
			Platform::GetPlatform_OS()->OutputDebugText("Couldn't set cooperative level for DirectSound object\n");
			return false;
			}
	
		return true;
		}

	Platform::GetPlatform_OS()->OutputDebugText("Couldn't load dsound.dll\n");
	return false;
	}


//*** Start ***

Platform_Win32_Sound_DSound::~Platform_Win32_Sound_DSound()
	{
	if (dsoundDLL_)
		{
		if (directSound_)
			{
			directSound_->Release();
			}
		FreeLibrary((HMODULE)dsoundDLL_);
		}
	}


//*** Update ***

void Platform_Win32_Sound_DSound::Update()
	{
	}


//*** CreateSoundStream ***

Platform_Sound_SoundStream* Platform_Win32_Sound_DSound::CreateSoundStream(int channels, int frequency, int bitsPerSample, int size)
	{
	return new Platform_Win32_Sound_SoundStream_DSound(directSound_,channels,frequency,bitsPerSample,size);
	}

