//*** Bitmap_16bitAlphaCrop.cpp ***

#include "Bitmap_16bitAlphaCrop.h"
#include "ColorHelper.h"
#include "Image.h"
#include "Asset.h"
#include "FloydSteinbergDither.h"


//*** GetType ***

StringId Bitmap_16bitAlphaCrop::GetType()
	{
	static StringId idBitmap_16bitAlphaCrop("Bitmap_16bitAlphaCrop");
	return idBitmap_16bitAlphaCrop;
	}


//*** Constructor ***

Bitmap_16bitAlphaCrop::Bitmap_16bitAlphaCrop()
	{
	}


//*** Constructor ***

Bitmap_16bitAlphaCrop::Bitmap_16bitAlphaCrop(const Asset& asset)
	{
	Load(asset);
	}


//*** Constructor ***

Bitmap_16bitAlphaCrop::Bitmap_16bitAlphaCrop(const Image& image, bool dither)
	{
	width_=image.GetWidth();
	height_=image.GetHeight();

	int minX=image.GetWidth();
	int minY=image.GetHeight();
	int maxX=0;
	int maxY=0;
	for (int y=0; y<image.GetHeight(); y++)
		{
		for (int x=0; x<image.GetWidth(); x++)
			{
			unsigned int color=image.GetPixel(x,y);
			if (color>>24)
				{
				if (y<minY)
					minY=y;
				if (x<minX)
					minX=x;
				if (y>maxY)
					maxY=y;
				if (x>maxX)
					maxX=x;
				}
			}
		}
	if (maxX<minX || maxY<minY)
		{
		return;
		}

	hOffset_=(unsigned short)minX;
	vOffset_=(unsigned short)minY;
	hPitch_=(unsigned short)(maxX-minX+1);
	vPitch_=(unsigned short)(maxY-minY+1);


	color_=static_cast<unsigned short*>(Malloc(hPitch_*vPitch_*sizeof(unsigned short)));
	alpha_=static_cast<unsigned char*>(Malloc(hPitch_*vPitch_*sizeof(unsigned char)));

	///\todo **** FIX THIS SECTION - NO NEED TO DITHER ENTIRE IMAGE ***
	if (dither)
		{
		unsigned short* dithered=static_cast<unsigned short*>(Malloc(image.GetWidth()*image.GetHeight()*sizeof(unsigned short)));
		FloydSteinbergDither::DitherImage(image.GetPointer(),image.GetWidth(),image.GetHeight(),dithered);
		for (int y=0; y<vPitch_; y++)
			{
			for (int x=0; x<hPitch_; x++)
				{
				unsigned int color=image.GetPixel(x+hOffset_,y+vOffset_);
				color_[x+y*hPitch_]=dithered[x+hOffset_+(y+vOffset_)*image.GetWidth()];
				alpha_[x+y*hPitch_]=static_cast<unsigned char>(color>>24);
				}
			}
		Free(dithered);
		return;
		}

	for (int y=0; y<vPitch_; y++)
		{
		for (int x=0; x<hPitch_; x++)
			{
			unsigned int color=image.GetPixel(x+hOffset_,y+vOffset_);
			color_[x+y*hPitch_]=RGB32TO16(color);
			alpha_[x+y*hPitch_]=static_cast<unsigned char>(color>>24);
			}
		}
	}


//*** Constructor ***

Bitmap_16bitAlphaCrop::Bitmap_16bitAlphaCrop(const Image& color, const Image& alpha, bool dither)
	{
	Assert(color.GetWidth()==alpha.GetWidth() && color.GetHeight()==alpha.GetHeight(),"The size of color and alpha doesn't match");
	if (color.GetWidth()!=alpha.GetWidth() || color.GetHeight()!=alpha.GetHeight())
		{
		return;
		}

	width_=color.GetWidth();
	height_=color.GetHeight();
	int minX=alpha.GetWidth();
	int minY=alpha.GetHeight();
	int maxX=0;
	int maxY=0;
	for (int y=0; y<alpha.GetHeight(); y++)
		{
		for (int x=0; x<alpha.GetWidth(); x++)
			{
			unsigned int color=alpha.GetPixel(x,y);
			if (color&0xff)
				{	
				if (y<minY)
					minY=y;
				if (x<minX)
					minX=x;
				if (y>maxY)
					maxY=y;
				if (x>maxX)
					maxX=x;
				}
			}
		}

	if (maxX<minX || maxY<minY)
		{
		return;
		}

	hOffset_=(unsigned short)minX;
	vOffset_=(unsigned short)minY;
	hPitch_=(unsigned short)(maxX-minX+1);
	vPitch_=(unsigned short)(maxY-minY+1);

	color_=static_cast<unsigned short*>(Malloc(hPitch_*vPitch_*sizeof(unsigned short)));
	alpha_=static_cast<unsigned char*>(Malloc(hPitch_*vPitch_*sizeof(unsigned char)));

	//**** FIX THIS SECTION - NO NEED TO DITHER ENTIRE color ***
	if (dither)
		{
		unsigned short* dithered=static_cast<unsigned short*>(Malloc(color.GetWidth()*color.GetHeight()*sizeof(unsigned short)));
		FloydSteinbergDither::DitherImage(color.GetPointer(),color.GetWidth(),color.GetHeight(),dithered);
		for (int y=0; y<vPitch_; y++)
			{
			for (int x=0; x<hPitch_; x++)
				{
				color_[x+y*hPitch_]=dithered[x+hOffset_+(y+hOffset_)*color.GetWidth()];
				int a=(alpha.GetPixel(x+hOffset_,y+vOffset_)&0xff);
				alpha_[x+y*hPitch_]=static_cast<unsigned char>(a);
				}
			}
		Free(dithered);
		return;
		}

	for (int y=0; y<vPitch_; y++)
		{
		for (int x=0; x<hPitch_; x++)
			{
			color_[x+y*hPitch_]=RGB32TO16(color.GetPixel(x+hOffset_,y+vOffset_));
			int a=(alpha.GetPixel(x+hOffset_,y+vOffset_)&0xff);
			alpha_[x+y*hPitch_]=static_cast<unsigned char>(a);
			}
		}
	}


//*** Destructor ***

Bitmap_16bitAlphaCrop::~Bitmap_16bitAlphaCrop()
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

void Bitmap_16bitAlphaCrop::Save(Asset& asset) const
	{
	if (asset.Create())
		{
		asset.Write("PIX16BAC",8);
		int version=0;
		asset.Write(&version);
		int celCount=0;
		asset.Write(&celCount);
		WriteToAsset(&asset);
		asset.Close();
		}
	}


//*** Load ***

void Bitmap_16bitAlphaCrop::Load(const Asset& asset)
	{
	if (asset.Open())
		{
		char header[8];
		asset.Read(header,8);

		if (StrNCmp(header,"PIX16BAC",8)==0)
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
		else if (StrNCmp(header,"PIXIE_CA",8)==0)
			{
			// Read the extra header bytes (BM)
			char c[2];
			asset.Read(c,2);
			Assert(StrNCmp(c,"BM",2)==0,"Invalid header");
			if (StrNCmp(c,"BM",2)!=0)
				{
				asset.Close();
				return;
				}
			
			int version=0;
			asset.Read(&version);
			if (version==0)
				{
				ReadFromAsset_Old(&asset);
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

void Bitmap_16bitAlphaCrop::ReadFromAsset(const Asset* asset)
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

	asset->Read(&width_);
	asset->Read(&height_);
	asset->Read(&hOffset_);
	asset->Read(&vOffset_);
	asset->Read(&hPitch_);
	asset->Read(&vPitch_);

	if (hPitch_*vPitch_>0)
		{
		color_=static_cast<unsigned short*>(Malloc(sizeof(unsigned short)*hPitch_*vPitch_));
		asset->Read(color_,hPitch_*vPitch_);
		alpha_=static_cast<unsigned char*>(Malloc(sizeof(unsigned char)*hPitch_*vPitch_));
		asset->Read(alpha_,hPitch_*vPitch_);
		}
	}


//*** ReadFromAsset_Old ***

void Bitmap_16bitAlphaCrop::ReadFromAsset_Old(const Asset* asset)
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

	asset->Read(&width_);
	asset->Read(&height_);

	unsigned short xOffset;
	unsigned short yOffset;
	unsigned short activeWidth;
	unsigned short activeHeight;
	asset->Read(&xOffset);
	asset->Read(&yOffset);
	asset->Read(&activeWidth);
	asset->Read(&activeHeight);
	hOffset_=xOffset;
	vOffset_=yOffset;
	hPitch_=activeWidth;
	vPitch_=activeHeight;

	if (hPitch_*vPitch_>0)
		{
		color_=static_cast<unsigned short*>(Malloc(sizeof(unsigned short)*hPitch_*vPitch_));
		asset->Read(color_,hPitch_*vPitch_);
		alpha_=static_cast<unsigned char*>(Malloc(sizeof(unsigned char)*hPitch_*vPitch_));
		asset->Read(alpha_,hPitch_*vPitch_);
		}
	}


//*** WriteToAsset ***

void Bitmap_16bitAlphaCrop::WriteToAsset(Asset* asset) const
	{
	asset->Write(&width_);
	asset->Write(&height_);
	asset->Write(&hOffset_);
	asset->Write(&vOffset_);
	asset->Write(&hPitch_);
	asset->Write(&vPitch_);
	if (hPitch_*vPitch_>0)
		{
		asset->Write(color_,hPitch_*vPitch_);
		asset->Write(alpha_,hPitch_*vPitch_);
		}
	}

