//*** Platform_Win32_Sound.cpp ***

#include "Platform_Win32_Sound.h"
#include "Platform_Win32_Sound_Technology.h"
#include "Platform_Win32_Sound_DSound.h"
#include "Platform_Win32_Sound_WMM.h"
#include "Platform_Win32_Sound_NoSound.h"
#include "Platform_Win32_OS.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <malloc.h>

//*** Constructor ***

Platform_Win32_Sound::Platform_Win32_Sound(Platform_Win32_OS* os):
	windowHandle_(os->GetWindowHandle()),
	technology_(Technology_Undefined),
	technologyInstance_(0)
	{
	Platform::RegisterEventListener(this);

	// Check commandline flags
	bool nosound=false;
	bool forcewmm=false;
	if (os->GetCommandLineString())
		{
		char* cmdline=strdup(os->GetCommandLineString());
		char* token=strtok(cmdline," ");
		while (token)
			{
			if (stricmp(token,"-nosound")==0)
				{
				nosound=true;
				}
			if (stricmp(token,"-forcewmm")==0)
				{
				forcewmm=true;
				}

			token=strtok(0," ");
			}
		free(cmdline);
		}

	// Choose default technology based on commandline flags
	if (nosound)
		{
		SetTechnology(Technology_NoSound);
		}
	else if (forcewmm)
		{
		SetTechnology(Technology_WMM);
		}
	else
		{
		SetTechnology(Technology_DSound); 
		}	
	}


//*** Destructor ***

Platform_Win32_Sound::~Platform_Win32_Sound()
	{
	if (technologyInstance_)
		{
		delete technologyInstance_;
		technologyInstance_=0;
		technology_=Technology_Undefined;
		}

	Platform::UnregisterEventListener(this);
	}


//*** OnOsYield ***

void Platform_Win32_Sound::OnOsYield()
	{
	if (!technologyInstance_)
		{
		return;
		}

	technologyInstance_->Update();
	}

		
//*** CreateSoundStream ***

Platform_Sound_SoundStream* Platform_Win32_Sound::CreateSoundStream(int channels, int frequency, int bitsPerSample, int size)
	{
	if (!technologyInstance_)
		{
		return 0;
		}

	return technologyInstance_->CreateSoundStream(channels,frequency,bitsPerSample,size);
	}


//*** SetTechnology ***

void Platform_Win32_Sound::SetTechnology(Platform_Win32_Sound::Technology technology)
	{
	if (technologyInstance_)
		{
		delete technologyInstance_;
		technologyInstance_=0;
		}

	technology_=technology;

	if (technology_>=Technology_Undefined)
		{
		return;
		}
	
	switch(technology_)
		{
		case Technology_DSound:
			{
			technologyInstance_=new Platform_Win32_Sound_DSound(windowHandle_);
			} break;

		case Technology_WMM:
			{
			technologyInstance_=new Platform_Win32_Sound_WMM();
			} break;
		case Technology_NoSound:
			{
			technologyInstance_=new Platform_Win32_Sound_NoSound();
			} break;
		}

	if (!technologyInstance_->Setup())
		{
		DowngradeTechnology();
		}
	}


//*** DowngradeTechnology ***

void Platform_Win32_Sound::DowngradeTechnology()
	{
	Platform::GetPlatform_OS()->OutputDebugText("Method failed for technology %d, falling back on technology %d\n",technology_,technology_+1);
	
	if (technologyInstance_)
		{
		delete technologyInstance_;
		technologyInstance_=0;
		}

	Technology newTechnology=(Technology)(technology_+1);
	
	if (newTechnology<Technology_Undefined)
		{
		SetTechnology(newTechnology);
		return;
		}
	
	technology_=Technology_Undefined;
	technologyInstance_=0;
	}


