//*** Bitmap_16bit.cpp ***

#include "Bitmap_16bit.h"
#include "ColorHelper.h"
#include "Image.h"
#include "Asset.h"
#include "FloydSteinbergDither.h"


//*** GetType ***

StringId Bitmap_16bit::GetType()
	{
	static StringId idBitmap_16bit("Bitmap_16bit");
	return idBitmap_16bit;
	}


//*** Constructor ***

Bitmap_16bit::Bitmap_16bit()
	{
	}


//*** Constructor ***

Bitmap_16bit::Bitmap_16bit(const Asset& asset)
	{
	Load(asset);
	}


//*** Constructor ***

Bitmap_16bit::Bitmap_16bit(int width, int height)
	{
	width_=width;
	height_=height;
	hPitch_=width;
	vPitch_=height;
	color_=static_cast<unsigned short*>(Malloc(sizeof(unsigned short)*width_*height_));
	}


//*** Constructor ***

Bitmap_16bit::Bitmap_16bit(const Image& image, bool dither)
	{
	width_=image.GetWidth();
	height_=image.GetHeight();
	hPitch_=width_;
	vPitch_=height_;
	color_=static_cast<unsigned short*>(Malloc(sizeof(unsigned short)*width_*height_));

	if (dither)
		{
		FloydSteinbergDither::DitherImage(image.GetPointer(),image.GetWidth(),image.GetHeight(),color_);
		return;
		}

	for (int y=0; y<height_; y++)
		{
		for (int x=0; x<width_; x++)
			{
			unsigned int c=image.GetPixel(x,y);
			color_[x+y*width_]=RGB32TO16(c);
			}	
		}

	}


//*** Destructor ***

Bitmap_16bit::~Bitmap_16bit()
	{
	if (color_)
		{
		Free(color_);
		}
	}


//*** Save ***

void Bitmap_16bit::Save(Asset& asset) const
	{
	if (asset.Create())
		{
		asset.Write("PIX16BIT",8);
		int version=0;
		asset.Write(&version);
		int celCount=0;
		asset.Write(&celCount);
		WriteToAsset(&asset);
		asset.Close();
		}
	}


//*** Load ***

void Bitmap_16bit::Load(const Asset& asset)
	{
	if (asset.Open())
		{
		char header[8];
		asset.Read(header,8);

		if (StrNCmp(header,"PIX16BIT",8)==0)
			{
			int version=0;
			asset.Read(&version);
			if (version==0)
				{
				int celCount=0;
				asset.Read(&celCount);
				ReadFromAsset(&asset);
				}
			}

		// Check for old format too
		else if (StrNCmp(header,"PIXIE_BM",8)==0)
			{
			int version=0;
			asset.Read(&version);
			if (version==0)
				{
				ReadFromAsset(&asset);
				}
			}

		else			
			{
			Assert(false,"Invalid header");
			}

		asset.Close();
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


//*** ReadFromAsset ***

void Bitmap_16bit::ReadFromAsset(const Asset* asset)
	{
	if (color_)
		{
		Free(color_);
		color_=0;
		}

	asset->Read(&hPitch_);
	asset->Read(&vPitch_);
	width_=hPitch_;
	height_=vPitch_;
	if (hPitch_*vPitch_>0)
		{
		color_=static_cast<unsigned short*>(Malloc(sizeof(unsigned short)*hPitch_*vPitch_));
		asset->Read(color_,hPitch_*vPitch_);
		}
	}


//*** WriteToAsset ***

void Bitmap_16bit::WriteToAsset(Asset* asset) const
	{
	asset->Write(&hPitch_);
	asset->Write(&vPitch_);
	if (hPitch_*vPitch_>0)
		{
		asset->Write(color_,hPitch_*vPitch_);
		}
	}
