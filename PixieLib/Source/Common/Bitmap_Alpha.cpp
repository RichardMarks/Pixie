//*** Bitmap_Alpha.cpp ***

#include "Bitmap_Alpha.h"
#include "ColorHelper.h"
#include "Image.h"
#include "Asset.h"


//*** GetType ***

StringId Bitmap_Alpha::GetType()
	{
	static StringId idBitmap_Alpha("Bitmap_Alpha");
	return idBitmap_Alpha;
	}


//*** Constructor ***

Bitmap_Alpha::Bitmap_Alpha()
	{
	}


//*** Constructor ***

Bitmap_Alpha::Bitmap_Alpha(const Asset& asset)
	{
	Load(asset);
	}


//*** Constructor ***

Bitmap_Alpha::Bitmap_Alpha(int width, int height)
	{
	width_=width;
	height_=height;
	hPitch_=width;
	vPitch_=height;
	alpha_=static_cast<unsigned char*>(Malloc(sizeof(unsigned char)*width_*height_));
	}


//*** Constructor ***

Bitmap_Alpha::Bitmap_Alpha(const Image& image, bool useGrayscaleInsteadOfAlpha)
	{
	width_=image.GetWidth();
	height_=image.GetHeight();
	hPitch_=width_;
	vPitch_=height_;
	alpha_=static_cast<unsigned char*>(Malloc(sizeof(unsigned char)*width_*height_));
	
	if (!useGrayscaleInsteadOfAlpha)
		{
		for (int y=0; y<height_; y++)
			{
			for (int x=0; x<width_; x++)
				{
				unsigned int c=image.GetPixel(x,y);
				unsigned char a=(unsigned char)(c>>24);
				alpha_[x+y*width_]=a;
				}	
			}
		}
	else
		{
		for (int y=0; y<height_; y++)
			{
			for (int x=0; x<width_; x++)
				{
				unsigned int c=image.GetPixel(x,y);
				unsigned int acc=(unsigned char)((c&0xff0000)>>16);
				acc+=(unsigned char)((c&0xff00)>>8);
				acc+=(unsigned char)((c&0xff));
				acc/=3;
				alpha_[x+y*width_]=(unsigned char)acc;
				}	
			}
		}
	}


//*** Destructor ***

Bitmap_Alpha::~Bitmap_Alpha()
	{
	if (alpha_)
		{
		Free(alpha_);
		}
	}


//*** Save ***

void Bitmap_Alpha::Save(Asset& asset) const
	{
	if (asset.Create())
		{
		asset.Write("PIXALPHA",8);
		int version=0;
		asset.Write(&version);
		int celCount=0;
		asset.Write(&celCount);
		WriteToAsset(&asset);
		asset.Close();
		}
	}


//*** Load ***

void Bitmap_Alpha::Load(const Asset& asset)
	{
	if (asset.Open())
		{
		char header[8];
		asset.Read(header,8);

		if (StrNCmp(header,"PIXALPHA",8)==0)
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
		else if (StrNCmp(header,"PIXIE_AM",8)==0)
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

void Bitmap_Alpha::ReadFromAsset(const Asset* asset)
	{
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
		alpha_=static_cast<unsigned char*>(Malloc(sizeof(unsigned char)*hPitch_*vPitch_));
		asset->Read(alpha_,hPitch_*vPitch_);
		}
	}


//*** WriteToAsset ***

void Bitmap_Alpha::WriteToAsset(Asset* asset) const
	{
	asset->Write(&hPitch_);
	asset->Write(&vPitch_);
	if (hPitch_*vPitch_>0)
		{
		asset->Write(alpha_,hPitch_*vPitch_);
		}
	}
