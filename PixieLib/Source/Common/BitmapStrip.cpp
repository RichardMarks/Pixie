//*** BitmapStrip.cpp **

#include "BitmapStrip.h"
#include "Image.h"
#include "StandardLibrary.h"
#include "Bitmap_RLE8.h"
#include "Bitmap_RLE16.h"
#include "Bitmap_16Bit.h"
#include "Bitmap_16BitAlpha.h"
#include "Bitmap_16BitAlphaCrop.h"
#include "Bitmap_Alpha.h"
#include "ColorHelper.h"
#include "Asset.h"


//*** Constructor ***

BitmapStrip::BitmapStrip()
	{
	}


//*** Constructor ***

BitmapStrip::BitmapStrip(const Asset& asset)
	{
	if (asset.Open())
		{
		ReadFromAsset( &asset );
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


//*** Constructor ***

BitmapStrip::BitmapStrip(const Image& image)
	{
	int width=image.GetWidth();
	int height=image.GetHeight();
	Image celImage(width,height);
	for (int i=0;  i<image.GetCelCount(); i++)
		{		

		for (int y=0; y<height; y++)
			{
			for (int x=0; x<width; x++)
				{
				celImage.SetPixel(x,y,image.GetPixel(i,x,y));	
				}
			}

		Bitmap* cel=new Bitmap_16bitAlpha(celImage);
		cels_.Add(cel);
		}
	}


//*** Constructor ***

BitmapStrip::BitmapStrip(const Bitmap* bitmap)
	{
	cels_.Add(bitmap);
	}


//*** Destructor ***

BitmapStrip::~BitmapStrip()
	{
	for (int i=0; i<cels_.GetItemCount(); i++)
		{
		const Bitmap* cel=cels_.Get(i);
		delete cel;
		}
	}


//*** GetCel ***

const Bitmap& BitmapStrip::GetCel(int index) const
	{
	Assert(cels_.GetItemCount()!=0,"Bitmap strip is empty");
	if (cels_.GetItemCount()==0)
		{
		static Bitmap_16bit defaultValue;
		return defaultValue;
		}

	int cel=index%cels_.GetItemCount();
	if (cel<0)
		{
		cel+=cels_.GetItemCount();
		}
	return *cels_.Get(cel);
	}


//*** GetCelCount ***

int BitmapStrip::GetCelCount() const
	{
	return cels_.GetItemCount();
	}


//*** operator Bitmap *** 

BitmapStrip::operator const Bitmap&() const
	{
	Assert(cels_.GetItemCount()!=0,"Bitmap strip is empty");
	if (cels_.GetItemCount()==0)
		{
		static Bitmap_16bit defaultValue;
		return defaultValue;
		}

	return *cels_.Get(0);
	}


//*** ReadFromAsset ***

void BitmapStrip::ReadFromAsset(const Asset* asset)
	{
	char header[8];
	asset->Read(header,8);

	// Is this an old-style pixie RLE file?
	if (StrNCmp(header,"PIXIE_RL",8)==0)
		{
		// Yes, so read the extra header byte
		char c;
		asset->Read(&c);
		Assert(c=='E',"Invalid header");
		if (c!='E')
			{
			asset->Close();
			return;
			}

		// Read the data
		int version=0;
		asset->Read(&version);
		if (version==0)
			{
			int celCount=0;
			asset->Read(&celCount);
			// Create and load a bitmap for each cel
			for (int i=0; i<Max(1,celCount); i++)
				{
				Bitmap* bitmap=new Bitmap_RLE8();
				bitmap->ReadFromAsset(asset);
				cels_.Add(bitmap);
				}
			}
		}

	// Is this an old-style pixie ABM file?
	else if (StrNCmp(header,"PIXIE_AB",8)==0)
		{
		// Yes, so read the extra header byte
		char c;
		asset->Read(&c);
		Assert(c=='M',"Invalid header");
		if (c!='M')
			{
			asset->Close();
			return;
			}

		// Read the data
		int version=0;
		asset->Read(&version);
		if (version==0)
			{
			Bitmap* bitmap=new Bitmap_16bitAlpha();
			bitmap->ReadFromAsset(asset);
			cels_.Add(bitmap);
			}
		}

	// Is this an old-style pixie BM file?
	else if (StrNCmp(header,"PIXIE_BM",8)==0)
		{
		// Read the data
		int version=0;
		asset->Read(&version);
		if (version==0)
			{
			Bitmap* bitmap=new Bitmap_16bit();
			bitmap->ReadFromAsset(asset);
			cels_.Add(bitmap);
			}
		}

	// Is this an old-style pixie AM file?
	else if (StrNCmp(header,"PIXIE_AM",8)==0)
		{
		// Read the data
		int version=0;
		asset->Read(&version);
		if (version==0)
			{
			Bitmap* bitmap=new Bitmap_Alpha();
			bitmap->ReadFromAsset(asset);
			cels_.Add(bitmap);
			}
		}

	// Is this a Bitmap_RLE8 file?
	else if (StrNCmp(header,"PIXRLE8B",8)==0)
		{
		// Read the data
		int version=0;
		asset->Read(&version);
		if (version==0)
			{
			int celCount=0;
			asset->Read(&celCount);
			// Create and load a bitmap for each cel
			for (int i=0; i<Max(1,celCount); i++)
				{
				Bitmap* bitmap=new Bitmap_RLE8();
				bitmap->ReadFromAsset(asset);
				cels_.Add(bitmap);
				}
			}
		}

	// Is this a Bitmap_RLE16 file?
	else if (StrNCmp(header,"PIXRLE16",8)==0)
		{
		// Read the data
		int version=0;
		asset->Read(&version);
		if (version==0)
			{
			int celCount=0;
			asset->Read(&celCount);
			// Create and load a bitmap for each cel
			for (int i=0; i<Max(1,celCount); i++)
				{
				Bitmap* bitmap=new Bitmap_RLE16();
				bitmap->ReadFromAsset(asset);
				cels_.Add(bitmap);
				}
			}
		}

	// Is this a Bitmap_16bit file?
	else if (StrNCmp(header,"PIX16BIT",8)==0)
		{
		int version=0;
		asset->Read(&version);
		if (version==0)
			{
			int celCount=0;
			asset->Read(&celCount);
			// Create and load a bitmap for each cel
			for (int i=0; i<Max(1,celCount); i++)
				{
				Bitmap* bitmap=new Bitmap_16bit();
				bitmap->ReadFromAsset(asset);
				cels_.Add(bitmap);
				}
			}
		}

	// Is this a Bitmap_16bitAlpha file?
	else if (StrNCmp(header,"PIX16BAL",8)==0)
		{
		int version=0;
		asset->Read(&version);
		if (version==0)
			{
			int celCount=0;
			asset->Read(&celCount);
			// Create and load a bitmap for each cel
			for (int i=0; i<Max(1,celCount); i++)
				{
				Bitmap* bitmap=new Bitmap_16bitAlpha();
				bitmap->ReadFromAsset(asset);
				cels_.Add(bitmap);
				}
			}
		}

	// Is this a Bitmap_16bitAlphaCrop file?
	else if (StrNCmp(header,"PIX16BAC",8)==0)
		{
		int version=0;
		asset->Read(&version);
		if (version==0)
			{
			int celCount=0;
			asset->Read(&celCount);
			// Create and load a bitmap for each cel
			for (int i=0; i<Max(1,celCount); i++)
				{
				Bitmap* bitmap=new Bitmap_16bitAlphaCrop();
				bitmap->ReadFromAsset(asset);
				cels_.Add(bitmap);
				}
			}
		}

	// Is this a Bitmap_Alpha file?
	else if (StrNCmp(header,"PIXALPHA",8)==0)
		{
		int version=0;
		asset->Read(&version);
		if (version==0)
			{
			int celCount=0;
			asset->Read(&celCount);
			// Create and load a bitmap for each cel
			for (int i=0; i<Max(1,celCount); i++)
				{
				Bitmap* bitmap=new Bitmap_Alpha();
				bitmap->ReadFromAsset(asset);
				cels_.Add(bitmap);
				}
			}
		}

	// Unknown file type
	else
		{
		Assert(false,"Invalid header");
		}

	}