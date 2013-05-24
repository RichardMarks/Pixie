//*** AudioFormat_OGG.cpp ***

#include "AudioFormat_OGG.h"
#include "Debug.h"
#include "Asset.h"
#include "StandardLibrary.h"


#include "ogg/codec.h"
#include "ogg/vorbisfile.h"

//*** File wrappers ***
size_t AudioFormat_OGG_read_func(void *ptr, size_t size, size_t nmemb, void *datasource);
int AudioFormat_OGG_seek_func(void *datasource, ogg_int64_t offset, int whence);
int AudioFormat_OGG_close_func(void *datasource);
long AudioFormat_OGG_tell_func(void *datasource);


//*** Register ***

void AudioFormat_OGG::Register()
	{
	AudioFormat::RegisterAudioFormat(TestAsset,Create);
	}


//*** Create ***

AudioFormat* AudioFormat_OGG::Create(const Asset& asset)
	{
	return new AudioFormat_OGG(asset);
	}


//*** TestAsset ***

bool AudioFormat_OGG::TestAsset(const Asset& asset)
	{
	if (asset.Open())
		{
		char buffer[5];
		asset.Read(buffer,5);
		asset.Close();
		if (StrCmp(buffer,"OggS")==0)
			{
			return true;
			}
		}
	return false;
	}


//*** Constructor ***

AudioFormat_OGG::AudioFormat_OGG(const Asset& asset)
	{
	// Create a copy of the asset (we need it to stream data from)
	asset_=new Asset(asset);

	// Open file (will be closed by OGG when it is done with it)
	bool res=asset_->Open();
	
	// Report missing file
	#ifdef _DEBUG
	if (!res)
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

	if (!res)
		{
		delete asset_;
		asset_=0;
		return;
		}
	
	// Set up OGG callbacks, to make it use our own file wrapper instead of fread etc.
	ov_callbacks callbacks;
	callbacks.read_func=AudioFormat_OGG_read_func;
	callbacks.seek_func=AudioFormat_OGG_seek_func;
	callbacks.close_func=AudioFormat_OGG_close_func;
	callbacks.tell_func=AudioFormat_OGG_tell_func;

	// Initialize OGG decoding of file
	oggFile_=new OggVorbis_File;
	int result=ov_open_callbacks(asset_, oggFile_, 0,0, callbacks);
	Assert(result==0,"Couldn't open stream");
	if (result!=0)
		{
		delete asset_;
		asset_=0;
		delete oggFile_;
		oggFile_=0;
		}
	}


//*** Destructor ***

AudioFormat_OGG::~AudioFormat_OGG()
	{
	ov_clear(oggFile_);
	delete oggFile_;
	delete asset_;
	}


//*** GetChannels ***

int AudioFormat_OGG::GetChannels()
	{
	vorbis_info* vi=ov_info(oggFile_,-1);
	return vi->channels;
	}


//*** GetFrequency ***

int AudioFormat_OGG::GetFrequency()
	{
	vorbis_info* vi=ov_info(oggFile_,-1);
	return vi->rate;
	}


//*** GetBitsPerSample ***

int AudioFormat_OGG::GetBitsPerSample()
	{
	return 16;
	}



//*** GetSize ***

int AudioFormat_OGG::GetSize()
	{
	Assert(oggFile_,"File not open");
	if (!oggFile_)
		{
		return 0;
		}

	unsigned int size=(unsigned int)ov_pcm_total(oggFile_,-1);
	Assert(size!=OV_EINVAL,"Couldn't get total length");
	return size*GetChannels()*(GetBitsPerSample()/8);
	}


//*** GetPosition ***

int AudioFormat_OGG::GetPosition()
	{
	return (int)(ov_pcm_tell(oggFile_)*GetChannels()*(GetBitsPerSample()/8));
	}


//*** SetPosition ***

void AudioFormat_OGG::SetPosition(int position)
	{
	ov_pcm_seek(oggFile_,position/(GetChannels()*(GetBitsPerSample()/8)));
	}


//*** CopySoundChunk ***

int AudioFormat_OGG::CopySoundChunk(void* buffer, int bytes)
	{
	int bytesToRead=bytes;
	unsigned int ret=0;
	while (bytesToRead>0)
		{
		int readSize=bytesToRead;
		int read=0;
		while (read<readSize)
			{
			ret=ov_read(oggFile_,static_cast<char*>(buffer)+read,readSize-read,0,2,1,0);
			read+=ret;
			if (ret==0)
				{
				return bytes-(bytesToRead-read);
				}			
			}
		bytesToRead-=readSize;
		buffer=static_cast<unsigned char*>(buffer)+readSize;
		}
	Assert(bytesToRead==0,"I'm sort of assuming bytesToRead will always equal 0 here...");
	return bytes;
	}


//*** File wrappers ***

size_t AudioFormat_OGG_read_func(void* ptr, size_t size, size_t nmemb, void* datasource)
	{
	Asset* asset=static_cast<Asset*>(datasource);
	return asset->Read(static_cast<unsigned char*>(ptr),(int)(size*nmemb));
	}

int AudioFormat_OGG_seek_func(void* datasource, ogg_int64_t offset, int whence)
	{
	Asset* asset=static_cast<Asset*>(datasource);
	Asset::SeekOrigin origin=Asset::SEEK_FROM_START;
	switch (whence)
	{
		case SEEK_SET:
			{
				origin=Asset::SEEK_FROM_START;
			}break;
		case SEEK_CUR:
			{
				origin=Asset::SEEK_FROM_CURRENT;
			}break;
		case SEEK_END:
			{
				origin=Asset::SEEK_FROM_END;
			}break;
	}

	return asset->Seek((int)offset,origin);
}

int AudioFormat_OGG_close_func(void* datasource)
	{
	Asset* asset=static_cast<Asset*>(datasource);
	asset->Close();
	return 0;
	}

long AudioFormat_OGG_tell_func(void* datasource)
	{
	Asset* asset=static_cast<Asset*>(datasource);
	return asset->Tell();
	}
