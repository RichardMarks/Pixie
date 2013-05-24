//*** ImageFormat_GIF.cpp ***

#include "ImageFormat_GIF.h"
#include "Debug.h"
#include "Asset.h"
#include "StandardLibrary.h"
#include "ColorHelper.h"
#include "gif/gif.h"



//*** Register ***

void ImageFormat_GIF::Register()
	{
	ImageFormat::RegisterImageFormat(TestAsset,Create);
	}


//*** Create ***

ImageFormat* ImageFormat_GIF::Create(const Asset& asset)
	{
	return new ImageFormat_GIF(asset);
	}


//*** TestAsset ***

bool ImageFormat_GIF::TestAsset(const Asset& asset)
	{
	if (asset.Open())
		{
		char buffer[3];
		asset.Read(buffer,3);
		asset.Close();
		if (StrNCmp(buffer,"GIF",3)==0)
			{
			return true;
			}
		}

	return false;
	}


//*** Constructor ***

ImageFormat_GIF::ImageFormat_GIF(const Asset& asset):
	image_(0)
	{
	GifLoader* image=new GifLoader();
	image_=image;
	bool result=image->Load(asset);
	Assert(result,"Failed to load gif image");
	if (!result)
		{
		delete image_;
		image_=0;
		}
	}


//*** Destructor ***

ImageFormat_GIF::~ImageFormat_GIF()
	{
	if (image_)
		{
		delete image_;
		}
	}


//*** GetWidth ***

int ImageFormat_GIF::GetWidth()
	{
	if (!image_)
		{
		return 0;
		}
	return image_->width_;
	}


//*** GetHeight ***

int ImageFormat_GIF::GetHeight()
	{
	if (!image_)
		{
		return 0;
		}
	return image_->height_;
	}


//*** GetCelCount ***

int ImageFormat_GIF::GetCelCount()
	{
	if (!image_)
		{
		return 0;
		}

	return image_->cels_.GetItemCount();
	}


//*** GetCelDelay ***

float ImageFormat_GIF::GetCelDelay(int celIndex)
	{
	if (!image_)
		{
		return 0;
		}
	GifLoader::Cel* cel=image_->cels_.Get(celIndex);
	return cel->delay/100.0f;
	}


//*** CopyPixels ***

void ImageFormat_GIF::CopyPixels(unsigned int* destination)
	{
	if (!image_)
		{
		return;
		}

	MemSet(destination,0,GetWidth()*GetHeight()*GetCelCount()*sizeof(unsigned int));
	for (int i=0; i<GetCelCount(); i++)
		{
		GifLoader::Cel* cel=image_->cels_.Get(i);
		for (int y=0; y<cel->height; y++)
			{
			for (int x=0; x<cel->width; x++)
				{
				unsigned char p=cel->pixels[x+y*cel->width];
				if (cel->transparency!=p)
					{
					unsigned int c=cel->palette[p];
					unsigned char r=(unsigned char)((c&0x00ff0000)>>16);
					unsigned char g=(unsigned char)((c&0x0000ff00)>>8);
					unsigned char b=(unsigned char)((c&0x000000ff));
					destination[GetWidth()*GetHeight()*i+(x+cel->x)+(y+cel->y)*GetWidth()]=RGB32(r,g,b);
					}
				}
			}
		}

	}


//*** Save ***

void ImageFormat_GIF::Save(const Filename& filename, int width, int height, void* data)
	{

	}
