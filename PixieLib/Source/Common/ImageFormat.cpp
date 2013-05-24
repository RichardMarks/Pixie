//*** ImageFormat.cpp ***

#include "ImageFormat.h"
#include "Debug.h"
#include "Asset.h"
#include "StandardLibrary.h"

Array<ImageFormat::ImageFormatEntry> ImageFormat::imageFormats_;

//*** RegisterImageFormat ***

void ImageFormat::RegisterImageFormat(bool (*TestFunction)(const Asset&),ImageFormat* (*CreateFunction)(const Asset&))
	{
	ImageFormatEntry entry;
	entry.TestFunction=TestFunction;
	entry.CreateFunction=CreateFunction;
	imageFormats_.Add(entry);
	}


//*** CreateImageFormat ***

ImageFormat* ImageFormat::CreateImageFormat(const Asset& asset)
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

	for (int i=0; i<imageFormats_.GetItemCount(); i++)
		{
		ImageFormatEntry entry=imageFormats_.Get(i);
		if (entry.TestFunction(asset))
			{
			return entry.CreateFunction(asset);
			}
		}

	// Report unknown image format
	#ifdef _DEBUG
	const char* filename=asset.GetFilename().GetString();
	if (filename)
		{
		char errorMessage[1024];
		SNPrintF(errorMessage,1024,"Unknown image format: %s",filename);
		Assert(false,errorMessage);
		}
	else
		{
		Assert(false,"Unknown image format.");
		}
	#endif

	return 0;
	}
