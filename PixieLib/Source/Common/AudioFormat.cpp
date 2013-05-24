//*** AudioFormat.cpp ***

#include "AudioFormat.h"
#include "Debug.h"
#include "Asset.h"
#include "StandardLibrary.h"

Array<AudioFormat::AudioFormatEntry> AudioFormat::audioFormats_;

//*** RegisterAudioFormat ***

void AudioFormat::RegisterAudioFormat(bool (*TestFunction)(const Asset&),AudioFormat* (*CreateFunction)(const Asset&))
	{
	AudioFormatEntry entry;
	entry.TestFunction=TestFunction;
	entry.CreateFunction=CreateFunction;
	audioFormats_.Add(entry);
	}


//*** CreateAudioFormat ***

AudioFormat* AudioFormat::CreateAudioFormat(const Asset& asset)
	{
	// Make sure asset exists
	#ifdef _DEBUG
	if (!asset.Open())
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

		return 0;
		}
	asset.Close();
	#endif

	for (int i=0; i<audioFormats_.GetItemCount(); i++)
		{
		AudioFormatEntry entry=audioFormats_.Get(i);
		if (entry.TestFunction(asset))
			{
			return entry.CreateFunction(asset);
			}
		}

	// Report unknown audio format
	#ifdef _DEBUG
	const char* filename=asset.GetFilename().GetString();
	if (filename)
		{
		char errorMessage[1024];
		SNPrintF(errorMessage,1024,"Unknown audio format: %s",filename);
		Assert(false,errorMessage);
		}
	else
		{
		Assert(false,"Unknown audio format.");
		}
	#endif

	return 0;
	}
