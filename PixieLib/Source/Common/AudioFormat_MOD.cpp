//*** AudioFormat_MOD.cpp ***

#include "AudioFormat_MOD.h"
#include "Debug.h"
#include "Asset.h"
#include "StandardLibrary.h"

#include "modplug/modplug.h"

namespace pixie {

//*** Register ***

void AudioFormat_MOD::Register()
	{
	AudioFormat::RegisterAudioFormat(TestAsset,Create);
	}


//*** Create ***

AudioFormat* AudioFormat_MOD::Create(const Asset& asset)
	{
	return new AudioFormat_MOD(asset);
	}


//*** TestAsset ***

bool AudioFormat_MOD::TestAsset(const Asset& asset)
	{
	if (asset.Open())
		{
		int size=asset.GetSize();
		unsigned char* buffer=static_cast<unsigned char*>(Malloc(size));
		asset.Open();
		asset.Read(buffer,size);
		asset.Close();

		ModPlugFile* mod=ModPlug_Load(buffer,size);
		Free(buffer);
		if (mod)
			{
			ModPlug_Unload(mod);
			return true;
			}
		}

	return false;
	}



//*** Constructor ***

AudioFormat_MOD::AudioFormat_MOD(const Asset& asset):
	modFile_(0),
	position_(0)
	{
	if (asset.Open())
		{
		int size=asset.GetSize();
		unsigned char* buffer=static_cast<unsigned char*>(Malloc(size));
		asset.Read(buffer,size);
		asset.Close();
		modFile_=ModPlug_Load(buffer,size);
		Free(buffer);
		}
	// Report missing file
	#ifdef _DEBUG
	else
		{
		const char* filename=asset.GetFilename().GetString();
		if (filename)
			{
			char errorMessage[1024];
			SNPrintF(errorMessage,1024,"File not found: %s",filename);
			Assert(false,errorMessage);
			}
		else
			{
			Assert(false,"An asset could not be accessed.");
			}
		}
	#endif
    }


//*** Destructor ***

AudioFormat_MOD::~AudioFormat_MOD()
	{
	if (modFile_)
		{
		ModPlug_Unload(modFile_);
		}
	}


//*** GetChannels ***

int AudioFormat_MOD::GetChannels()
	{
	return 2;
	}


//*** GetFrequency ***

int AudioFormat_MOD::GetFrequency()
	{
	return 44100;
	}


//*** GetBitsPerSample ***

int AudioFormat_MOD::GetBitsPerSample()
	{
	return 16;
	}


//*** GetSize ***

int AudioFormat_MOD::GetSize()
	{
	if (!modFile_)
		{
		return 0;
		}
	return (int)((ModPlug_GetLength(modFile_)/1000.0f)*GetChannels()*(GetBitsPerSample()/8)*GetFrequency());
	}


//*** GetPosition ***

int AudioFormat_MOD::GetPosition()
	{
	return position_;
	}


//*** SetPosition ***

void AudioFormat_MOD::SetPosition(int position)
	{
	position_=position;
	if (modFile_)
		{
		float sampleSize=(float)(GetChannels()*GetFrequency()*(GetBitsPerSample()/8));
		float time=position/sampleSize;
		ModPlug_Seek(modFile_,(int)(time*1000.0f));
		}
	}


//*** CopySoundChunk ***

int AudioFormat_MOD::CopySoundChunk(void* buffer,int bytes)
	{
	if (!modFile_)
		{
		return 0;
		}

	int read=ModPlug_Read(modFile_,buffer,bytes);
	position_+=read;
	return read;
	}


}; // namespace pixie
