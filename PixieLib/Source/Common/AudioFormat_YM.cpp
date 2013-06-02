//*** AudioFormat_YM.cpp ***

#include "AudioFormat_YM.h"
#include "Debug.h"
#include "Asset.h"
#include "StandardLibrary.h"

#include "stsound/YmMusic.h"

namespace pixie {
//*** Register ***

void AudioFormat_YM::Register()
	{
	AudioFormat::RegisterAudioFormat(TestAsset,Create);
	}


//*** Create ***

AudioFormat* AudioFormat_YM::Create(const Asset& asset)
	{
	return new AudioFormat_YM(asset);
	}


//*** TestAsset ***

bool AudioFormat_YM::TestAsset(const Asset& asset)
	{
	if (asset.Open())
		{
		char buffer[7];
		asset.Read(buffer,7);
		asset.Close();
		if (StrNCmp(&buffer[2],"-lh5-",5)==0 || StrNCmp(buffer,"YM3!",4)==0)
			{
			return true;
			}
		}

	return false;
	}


//*** Constructor ***

AudioFormat_YM::AudioFormat_YM(const Asset& asset):
	ymFile_(0),
	chunkStart_(0),
	chunkEnd_(0)
	{
	if (asset.Open())
		{
		int size=asset.GetSize();
		unsigned char* buffer=static_cast<unsigned char*>(Malloc(size));
		asset.Read(buffer,size);
		asset.Close();

		ymFile_=new CYmMusic();
		ymFile_->loadMemory(buffer,size);
		ymFile_->setLoopMode(true);
		ymFile_->play();
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

AudioFormat_YM::~AudioFormat_YM()
	{
	if (ymFile_)
		{
		ymFile_->unLoad();
		delete ymFile_;
		}
	}


//*** GetChannels ***

int AudioFormat_YM::GetChannels()
	{
	return 1;
	}


//*** GetFrequency ***

int AudioFormat_YM::GetFrequency()
	{
	return 44100;
	}


//*** GetBitsPerSample ***

int AudioFormat_YM::GetBitsPerSample()
	{
	return 16;
	}


//*** GetSize ***

int AudioFormat_YM::GetSize()
	{
	if (!ymFile_)
		{
		return 0;
		}
	return (int)((ymFile_->getMusicTime()/1000.0f)*GetChannels()*(GetBitsPerSample()/8)*GetFrequency());
	}


//*** GetPosition ***

int AudioFormat_YM::GetPosition()
	{
	if (!ymFile_)
		{
		return 0;
		}
	return (int)((ymFile_->getPos()/1000.0f)*GetChannels()*(GetBitsPerSample()/8)*GetFrequency());
	}


//*** SetPosition ***

void AudioFormat_YM::SetPosition(int position)
	{
	if (!ymFile_)
		{
		return;
		}
	float sampleSize=(float)(GetChannels()*GetFrequency()*(GetBitsPerSample()/8));
	float time=position/sampleSize;
	ymFile_->setMusicTime((int)(time*1000.0f));
	}


//*** CopySoundChunk ***

int AudioFormat_YM::CopySoundChunk(void* buffer,int bytes)
	{
	if (!ymFile_)
		{
		return 0;
		}
	chunkStart_=ymFile_->getCurrentFrame();
	ymbool notdone=ymFile_->update(static_cast<ymsample*>(buffer),bytes/2);
	chunkEnd_=ymFile_->getCurrentFrame();
	if (notdone)
		{
		return bytes;
		}

	return bytes-1;
	}


//*** GetRegisters ***

AudioFormat_YM::YmRegisters AudioFormat_YM::GetRegisters(float chunkPosition)
	{
	YmRegisters ym;
	MemSet(&ym,0,sizeof(ym));
	if (!ymFile_)
		{
		return ym;
		}
	int ymFrame=(int)(chunkStart_+(chunkEnd_-chunkStart_)*chunkPosition);
	int savedFrame=ymFile_->getCurrentFrame();
	ymFile_->setCurrentFrame(ymFrame);
	ymFile_->player();
	ym.Channel_A_fine_tune=ymFile_->readYmRegister(0);
	ym.Channel_A_coarse_tune=ymFile_->readYmRegister(1);
	ym.Channel_B_fine_tune=ymFile_->readYmRegister(2);
	ym.Channel_B_coarse_tune=ymFile_->readYmRegister(3);
	ym.Channel_C_fine_tune=ymFile_->readYmRegister(4);
	ym.Channel_C_coarse_tune=ymFile_->readYmRegister(5);
	ym.Noise_generator_control=ymFile_->readYmRegister(6);
	ym.Mixer_control_IO_enable=ymFile_->readYmRegister(7);
	ym.Channel_A_amplitude=ymFile_->readYmRegister(8);
	ym.Channel_B_amplitude=ymFile_->readYmRegister(9);
	ym.Channel_C_amplitude=ymFile_->readYmRegister(10);
	ym.Envelope_period_fine_tune=ymFile_->readYmRegister(11);
	ym.Envelope_period_coarse_tune=ymFile_->readYmRegister(12);
	ym.Envelope_shape=ymFile_->readYmRegister(13);
	ymFile_->setCurrentFrame(savedFrame);
	return ym;
	}

}; // namespace pixie
