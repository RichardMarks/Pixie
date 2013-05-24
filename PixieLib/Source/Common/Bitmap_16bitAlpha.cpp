//*** Bitmap_16bitAlpha.cpp ***

#include "Bitmap_16bitAlpha.h"
#include "ColorHelper.h"
#include "Image.h"
#include "Asset.h"
#include "FloydSteinbergDither.h"


//*** GetType ***

StringId Bitmap_16bitAlpha::GetType()
	{
	static StringId idAlphaBitmap("Bitmap_16bitAlpha");
	return idAlphaBitmap;
	}


//*** Constructor ***

Bitmap_16bitAlpha::Bitmap_16bitAlpha()
	{
	}


//*** Constructor ***

Bitmap_16bitAlpha::Bitmap_16bitAlpha(const Asset& asset)
	{
	Load(asset);
	}


//*** Constructor ***

Bitmap_16bitAlpha::Bitmap_16bitAlpha(int width, int height)
	{
	width_=width;
	height_=height;
	hPitch_=width;
	vPitch_=height;
	color_=static_cast<unsigned short*>(Malloc(sizeof(unsigned short)*width_*height_));
	alpha_=static_cast<unsigned char*>(Malloc(sizeof(unsigned char)*width_*height_));
	}


//*** Constructor ***

Bitmap_16bitAlpha::Bitmap_16bitAlpha(const Image& image, bool dither):
	Bitmap()
	{
	width_=image.GetWidth();
	height_=image.GetHeight();
	hPitch_=width_;
	vPitch_=height_;
	color_=static_cast<unsigned short*>(Malloc(sizeof(unsigned short)*width_*height_));
	alpha_=static_cast<unsigned char*>(Malloc(sizeof(unsigned char)*width_*height_));
	
	if (dither)
		{
		FloydSteinbergDither::DitherImage(image.GetPointer(),image.GetWidth(),image.GetHeight(),color_);
		}

	for (int y=0; y<height_; y++)
		{
		for (int x=0; x<width_; x++)
			{
			unsigned int c=image.GetPixel(x,y);
			if (!dither)
				{
				color_[x+y*width_]=RGB32TO16(c);
				}
			unsigned char a=(unsigned char)(c>>24);
			alpha_[x+y*width_]=a;
			}	
		}
	}


//*** Destructor ***

Bitmap_16bitAlpha::~Bitmap_16bitAlpha()
	{
	if (color_)
		{
		Free(color_);
		}
	if (alpha_)
		{
		Free(alpha_);
		}
	}


//*** Save ***

void Bitmap_16bitAlpha::Save(Asset& asset) const
	{
	if (asset.Create())
		{
		asset.Write("PIX16BAL",8);
		int version=0;
		asset.Write(&version);
		int celCount=0;
		asset.Write(&celCount);
		WriteToAsset(&asset);
		asset.Close();
		}
	}


//*** Load ***

void Bitmap_16bitAlpha::Load(const Asset& asset)
	{
	if (asset.Open())
		{
		char header[8];
		asset.Read(header,8);

		if (StrNCmp(header,"PIX16BAL",8)==0)
			{
			int version=0;
			asset.Read(&version);
			int celCount=0;
			asset.Read(&celCount);
			if (version==0)
				{
				ReadFromAsset(&asset);
				}
			}

		// Check for old format too
		else if (StrNCmp(header,"PIXIE_AB",8)==0)
			{
			// Read the extra header byte (M)
			char c;
			asset.Read(&c);
			Assert(c=='M',"Invalid header");
			if (c!='M')
				{
				asset.Close();
				return;
				}
			
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

void Bitmap_16bitAlpha::ReadFromAsset(const Asset* asset)
	{
	if (color_)
		{
		Free(color_);
		color_=0;
		}
	if (alpha_)
		{
		Free(alpha_);
		alpha_=0;
		}

	asset->Read(&hPitch_);
	asset->Read(&vPitch_);
	width_=hPitch_;
	height_=vPitch_;
	if (hPitch_*vPitch_>0)
		{
		color_=static_cast<unsigned short*>(Malloc(sizeof(unsigned short)*hPitch_*vPitch_));
		asset->Read(color_,hPitch_*vPitch_);
		alpha_=static_cast<unsigned char*>(Malloc(sizeof(unsigned char)*hPitch_*vPitch_));
		asset->Read(alpha_,hPitch_*vPitch_);
		}
	}


//*** WriteToAsset ***

void Bitmap_16bitAlpha::WriteToAsset(Asset* asset) const
	{
	asset->Write(&hPitch_);
	asset->Write(&vPitch_);
	if (hPitch_*vPitch_>0)
		{
		asset->Write(color_,hPitch_*vPitch_);
		asset->Write(alpha_,hPitch_*vPitch_);
		}
	}
