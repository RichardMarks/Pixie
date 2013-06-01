//*** Bitmap_RLE16.cpp ***

#include "Bitmap_RLE16.h"
#include "ColorHelper.h"
#include "Image.h"
#include "Asset.h"
#include "GenerateRLE16.h"



//*** GetType ***

StringId Bitmap_RLE16::GetType()
	{
	static StringId idBitmap_RLE16("Bitmap_RLE16");
	return idBitmap_RLE16;
	}


//*** Constructor ***

Bitmap_RLE16::Bitmap_RLE16():
	opaqueSize_(0),
	opaqueData_(0),
	alphaSize_(0),
	alphaData_(0)
	{
	}


//*** Constructor ***

Bitmap_RLE16::Bitmap_RLE16(const Asset& asset):
	opaqueSize_(0),
	opaqueData_(0),
	alphaSize_(0),
	alphaData_(0)
	{
	Load(asset);
	}


//*** Constructor ***

Bitmap_RLE16::Bitmap_RLE16(const Image& image, bool dither):
	opaqueSize_(0),
	opaqueData_(0),
	alphaSize_(0),
	alphaData_(0)
	{
	width_=image.GetWidth();
	height_=image.GetHeight();
	GenerateRLE16 generate(image,this,dither);
	}


//*** Destructor ***

Bitmap_RLE16::~Bitmap_RLE16()
	{
	if (opaqueData_)
		{
		Free(opaqueData_);
		}

	if (alphaData_)
		{
		Free(alphaData_);
		}
	}


//*** Save ***

void Bitmap_RLE16::Save(Asset& asset) const
	{
	if (asset.Create())
		{
		asset.Write("PIXRLE16",8);
		int version=0;
		asset.Write(&version);
		int celCount=0;
		asset.Write(&celCount);
		WriteToAsset(&asset);
		asset.Close();
		}

	}


//*** Load ***

void Bitmap_RLE16::Load(const Asset& asset)
	{
	if (asset.Open())
		{
		char header[8];
		asset.Read(header,8);

		if (StrNCmp(header,"PIXRLE16",8)==0)
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
		else if (StrNCmp(header,"PIXIE_RL",8)==0)
			{
			// Read the extra header bytes (BM)
			char c[3];
			asset.Read(c,3);
			Assert(StrNCmp(c,"E16",3)==0,"Invalid header");
			if (StrNCmp(c,"E16",3)!=0)
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

void Bitmap_RLE16::ReadFromAsset(const Asset* asset)
	{
	asset->Read(&width_);
	asset->Read(&height_);
	asset->Read(&hPitch_);
	asset->Read(&vPitch_);
	asset->Read(&opaqueSize_);
	if (opaqueSize_>0)
		{
		opaqueData_=static_cast<unsigned char*>(Malloc(opaqueSize_));
		asset->Read(opaqueData_,opaqueSize_);
		}
	asset->Read(&alphaSize_);
	if (alphaSize_>0)
		{
		alphaData_=static_cast<unsigned char*>(Malloc(alphaSize_));
		asset->Read(alphaData_,alphaSize_);
		}
	asset->Read(&hOffset_,1);
	asset->Read(&vOffset_,1);
	}


//*** WriteToAsset ***

void Bitmap_RLE16::WriteToAsset(Asset* asset) const
	{
	asset->Write(&width_);
	asset->Write(&height_);
	asset->Write(&hPitch_);
	asset->Write(&vPitch_);
	asset->Write(&opaqueSize_);
	if (opaqueSize_>0 && opaqueData_)
		{
		asset->Write(opaqueData_,opaqueSize_);
		}
	asset->Write(&alphaSize_);
	if (alphaSize_>0 && alphaData_)
		{
		asset->Write(alphaData_,alphaSize_);
		}
	asset->Write(&hOffset_);
	asset->Write(&vOffset_);
	}


//*** GetWidth ***

int Bitmap_RLE16::GetWidth(Transformation transformation) const
	{
	switch (transformation)
		{
		case NoTransformation:
		case Rotate_180:
		case Mirror_X:
		case Mirror_Y:
			{
			return width_;
			} break;

		case Rotate_90:
		case Rotate_270:
			{
			return height_;
			} break;
		}
	
	return width_;		
	}


//*** GetHeight ***

int Bitmap_RLE16::GetHeight(Transformation transformation) const
	{
	switch (transformation)
		{
		case NoTransformation:
		case Rotate_180:
		case Mirror_X:
		case Mirror_Y:
			{
			return height_;
			} break;

		case Rotate_90:
		case Rotate_270:
			{
			return width_;
			} break;
		}
	
	return height_;
	}


//*** GetColorData ***

unsigned short* Bitmap_RLE16::GetColorData()
	{
	return 0;
	}


//*** GetAlphaData ***

unsigned char* Bitmap_RLE16::GetAlphaData()
	{
	return 0;
	}


//*** Clear ***

void Bitmap_RLE16::Clear()
	{
	}


//*** Fill ***

void Bitmap_RLE16::Fill(int x1, int y1, int x2, int y2, unsigned short color, unsigned char alpha)
	{
	}


//*** Fill ***

void Bitmap_RLE16::Fill(unsigned short color, unsigned char alpha)
	{
	}



//*** SetPixelColor ***

void Bitmap_RLE16::SetPixelColor(int x, int y,unsigned short color, Transformation transformation)
	{
	}


//*** GetPixelColor ***

unsigned short Bitmap_RLE16::GetPixelColor(int x, int y, Transformation transformation) const
	{
//	if (x>=0 && x<GetWidth(transformation) && y>=0 && y<GetHeight(transformation))
//		{
//		TransformCoordinates(x,y,transformation);
//		return RLEGetPixelColor(x,y);
//		}

	return 0;
	}


//*** SetPixelAlpha ***

void Bitmap_RLE16::SetPixelAlpha(int x, int y,unsigned char alpha, Transformation transformation)
	{
	}


//*** GetPixelAlpha ***

unsigned char Bitmap_RLE16::GetPixelAlpha(int x, int y, Transformation transformation) const
	{
	if (x>=0 && x<GetWidth(transformation) && y>=0 && y<GetHeight(transformation))
		{
		TransformCoordinates(x,y,transformation);
		return RLEGetPixelAlpha(x,y);
		}

	return 0;
	}


//*** BlendPixel ***

void Bitmap_RLE16::BlendPixel(int x, int y,unsigned short color, unsigned char alpha, Transformation transformation)
	{
	}


//*** Blit ***

void Bitmap_RLE16::Blit(Bitmap& target, int x, int y, unsigned short modulate, unsigned char alpha, Transformation transformation, bool maxBlit) const
	{
	Blit(0, 0, width_-1, height_-1, target, x, y, modulate, alpha, transformation, maxBlit);
	}


//*** Blit ***

void Bitmap_RLE16::Blit(int x1, int y1, int x2, int y2, Bitmap& target, int x, int y, unsigned short modulate, unsigned char alpha, Transformation transformation, bool maxBlit) const
	{
	BlitRLE(target,x,y);
	}


//*** Copy ***

void Bitmap_RLE16::Copy(Bitmap& target, int x, int y, unsigned short modulate, Transformation transformation) const
	{
	}


//*** Copy ***

void Bitmap_RLE16::Copy(int x1, int y1, int x2, int y2, Bitmap& target, int x, int y, unsigned short modulate, Transformation transformation) const
	{
	}


//*** Blit ***

void Bitmap_RLE16::BlitRLE(Bitmap& target, int xp, int yp) const
	{
	unsigned short* data=reinterpret_cast<unsigned short*>(opaqueData_);
	int size=opaqueSize_;
	unsigned short* dest=target.GetColorData();
	dest+=hOffset_+target.GetWidth()*(vOffset_+yp)+xp;
	int xdelta=target.GetWidth()-hPitch_;
	int x=0;
	while (size>0)
		{
		unsigned short blank=*data;
		data++;
		size-=2;
		dest+=blank;
		x+=blank;
		if (x>=hPitch_)
			{
			x-=hPitch_;
			dest+=xdelta;
			}

		unsigned short nonblank=*data;
		data++;
		size-=2;
		while (nonblank>0)
			{
			nonblank--;
			unsigned short col=*data;
			data++;
			size-=2;
			*dest=col;
			dest++;
			x++;
			if (x>=hPitch_)
				{
				x-=hPitch_;
				dest+=xdelta;
				}
			}
		}

	unsigned char* alphadata=alphaData_;
	size=alphaSize_;
	dest=target.GetColorData();
	dest+=hOffset_+target.GetWidth()*(vOffset_+yp)+xp;
	x=0;
	while (size>0)
		{
		unsigned short blank=*reinterpret_cast<unsigned short*>(alphadata);
		alphadata+=2;
		size-=2;
		dest+=blank;
		x+=blank;
		if (x>=hPitch_)
			{
			x-=hPitch_;
			dest+=xdelta;
			}

		unsigned short nonblank=*reinterpret_cast<unsigned short*>(alphadata);
		alphadata+=2;
		size-=2;
		while (nonblank>0)
			{
			nonblank--;
			unsigned short col=*reinterpret_cast<unsigned short*>(alphadata);
			alphadata+=2;
			size-=2;
			unsigned char alpha=*static_cast<unsigned char*>(alphadata);
			alphadata+=1;
			size-=1;
			*dest=AlphaBlend16(*dest,col,alpha);
			dest++;
			x++;
			if (x>=hPitch_)
				{
				x-=hPitch_;
				dest+=xdelta;
				}
			}
		}
	}


//*** RLEGetPixelAlpha ***

unsigned char Bitmap_RLE16::RLEGetPixelAlpha(int xp, int yp) const
	{
	xp-=hOffset_;
	yp-=vOffset_;

	unsigned short* data=reinterpret_cast<unsigned short*>(opaqueData_);
	int size=opaqueSize_;
	int x=0;
	int y=0;
	while (size>0)
		{
		unsigned short blank=*data;
		data++;
		size-=2;
		x+=blank;
		if (x>=hPitch_)
			{
			x-=hPitch_;
			y++;
			}

		unsigned short nonblank=*data;
		data++;
		size-=2;
		while (nonblank>0)
			{
			nonblank--;
			unsigned short col=*data;
			data++;
			size-=2;
			if (x==xp && y==yp)
				{
				return 255;
				}
			x++;
			if (x>=hPitch_)
				{
				x-=hPitch_;
				y++;
				}
			}
		}

	unsigned char* alphadata=alphaData_;
	size=alphaSize_;
	x=0;
	y=0;
	while (size>0)
		{
		unsigned short blank=*reinterpret_cast<unsigned short*>(alphadata);
		alphadata+=2;
		size-=2;
		x+=blank;
		if (x>=hPitch_)
			{
			x-=hPitch_;
			y++;
			}

		unsigned short nonblank=*reinterpret_cast<unsigned short*>(alphadata);
		alphadata+=2;
		size-=2;
		while (nonblank>0)
			{
			nonblank--;
			unsigned short col=*reinterpret_cast<unsigned short*>(alphadata);
			alphadata+=2;
			size-=2;
			unsigned char alpha=*static_cast<unsigned char*>(alphadata);
			if (x==xp && y==yp)
				{
				return alpha;
				}
			alphadata+=1;
			size-=1;
			x++;
			if (x>=hPitch_)
				{
				x-=hPitch_;
				y++;
				}
			}
		}
	return 0;
	}
