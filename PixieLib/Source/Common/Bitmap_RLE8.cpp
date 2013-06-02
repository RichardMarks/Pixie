//*** Bitmap_RLE8.cpp ***

#include "Bitmap_RLE8.h"
#include "Image.h"
#include "Asset.h"
#include "Bitmap_16bit.h"
#include "Bitmap_16bitAlpha.h"
#include "ColorHelper.h"
#include "GenerateRLE8.h"
#include "BlitterRLE8.h"
#include "CopperRLE8.h"
#include "MaxBlitterRLE8.h"

namespace pixie {

const char* Pixie_Rle_Header="PIXRLE8B";

//*** GetType ***

StringId Bitmap_RLE8::GetType()
	{
	static StringId idBitmap_RLE8("Bitmap_RLE8");
	return idBitmap_RLE8;
	}


//*** Constructor ***

Bitmap_RLE8::Bitmap_RLE8():
	usesMask_(0),
	xOffset_(0),
	yOffset_(0),
	activeWidth_(0),
	activeHeight_(0),
	width_(0),
	height_(0),
	opaqueSize_(0),
	opaqueData_(0),
	alphaSize_(0),
	alphaData_(0),
	colorCount_(0),
	palette_(0),
	modulatedPalette_(0),
	currentPalette_(0)
	{
	}


//*** Constructor ***

Bitmap_RLE8::Bitmap_RLE8(const Image& image, bool dither):
	usesMask_(0),
	xOffset_(0),
	yOffset_(0),
	activeWidth_(0),
	activeHeight_(0),
	width_(0),
	height_(0),
	opaqueSize_(0),
	opaqueData_(0),
	alphaSize_(0),
	alphaData_(0),
	colorCount_(0),
	palette_(0),
	modulatedPalette_(0),
	currentPalette_(0)
	{
	GenerateRLE8 generate(image,this,dither);
	}


//*** Constructor ***

Bitmap_RLE8::Bitmap_RLE8(const Asset& asset):
	usesMask_(0),
	xOffset_(0),
	yOffset_(0),
	activeWidth_(0),
	activeHeight_(0),
	width_(0),
	height_(0),
	opaqueSize_(0),
	opaqueData_(0),
	alphaSize_(0),
	alphaData_(0),
	colorCount_(0),
	palette_(0),
	modulatedPalette_(0),
	currentPalette_(0)
	{
	Load(asset);
	}


//*** Destructor ***

Bitmap_RLE8::~Bitmap_RLE8()
	{
	if (opaqueData_)
		{
		Free(opaqueData_);
		}

	if (alphaData_)
		{
		Free(alphaData_);
		}

	if (palette_)
		{
		Free(palette_);
		}

	if (modulatedPalette_)
		{
		Free(modulatedPalette_);
		}
	}

//*** GetHPitch ***

int Bitmap_RLE8::GetHPitch() const
    {
    return activeWidth_;
    }

//*** GetVPitch ***

int Bitmap_RLE8::GetVPitch() const
    {
    return activeHeight_;
    }

//*** GetHOffset ***

int Bitmap_RLE8::GetHOffset() const
    {
    return xOffset_;
    }

//*** GetVOffset ***

int Bitmap_RLE8::GetVOffset() const
    {
    return yOffset_;
    }

//*** GetWidth ***

int Bitmap_RLE8::GetWidth(Transformation transformation) const
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

int Bitmap_RLE8::GetHeight(Transformation transformation) const
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

unsigned short* Bitmap_RLE8::GetColorData()
	{
	return 0;
	}


//*** GetAlphaData ***

unsigned char* Bitmap_RLE8::GetAlphaData()
	{
	return 0;
	}


//*** Clear ***

void Bitmap_RLE8::Clear()
	{
	}


//*** Fill ***

void Bitmap_RLE8::Fill(int x1, int y1, int x2, int y2, unsigned short color, unsigned char alpha)
	{
	}


//*** Fill ***

void Bitmap_RLE8::Fill(unsigned short color, unsigned char alpha)
	{
	}



//*** SetPixelColor ***

void Bitmap_RLE8::SetPixelColor(int x, int y,unsigned short color, Transformation transformation)
	{
	}


//*** GetPixelColor ***

unsigned short Bitmap_RLE8::GetPixelColor(int x, int y, Transformation transformation) const
	{
	if (x>=0 && x<GetWidth(transformation) && y>=0 && y<GetHeight(transformation))
		{
		TransformCoordinates(x,y,transformation);
		return RLEGetPixelColor(x,y);
		}

	return 0;
	}


//*** SetPixelAlpha ***

void Bitmap_RLE8::SetPixelAlpha(int x, int y,unsigned char alpha, Transformation transformation)
	{
	}


//*** GetPixelAlpha ***

unsigned char Bitmap_RLE8::GetPixelAlpha(int x, int y, Transformation transformation) const
	{
	if (x>=0 && x<GetWidth(transformation) && y>=0 && y<GetHeight(transformation))
		{
		TransformCoordinates(x,y,transformation);
		return RLEGetPixelAlpha(x,y);
		}

	return 0;
	}


//*** BlendPixel ***

void Bitmap_RLE8::BlendPixel(int x, int y,unsigned short color, unsigned char alpha, Transformation transformation)
	{
	}


//*** Blit ***

void Bitmap_RLE8::Blit(Bitmap& target, int x, int y, unsigned short modulate, unsigned char alpha, Transformation transformation, bool maxBlit) const
	{
	Blit(0, 0, width_-1, height_-1, target, x, y, modulate, alpha, transformation, maxBlit );
	}


//*** Blit ***

void Bitmap_RLE8::Blit(int x1, int y1, int x2, int y2, Bitmap& target, int x, int y, unsigned short modulate, unsigned char alpha, Transformation transformation, bool maxBlit) const
	{
	if( !maxBlit )
		{
		BlitRLE(x1,y1,x2,y2,static_cast<Bitmap_16bit*>(&target),x,y,modulate,alpha);
		}
	else
		{
		MaxBlitRLE(x1,y1,x2,y2,static_cast<Bitmap_16bit*>(&target),x,y,modulate,alpha);
		}
	}


//*** Copy ***

void Bitmap_RLE8::Copy(Bitmap& target, int x, int y, unsigned short modulate, Transformation transformation) const
	{
	Copy(0, 0, width_-1, height_-1, target, x, y, modulate, transformation);
	}


//*** Copy ***

void Bitmap_RLE8::Copy(int x1, int y1, int x2, int y2, Bitmap& target, int x, int y, unsigned short modulate, Transformation transformation) const
	{
	BlitRLE(x1,y1,x2,y2,static_cast<Bitmap_16bitAlpha*>(&target),x,y,modulate);
	}


//*** ReadFromAsset ***

void Bitmap_RLE8::ReadFromAsset(const Asset* asset)
	{
	asset->Read(&width_);
	asset->Read(&height_);
	asset->Read(&activeWidth_);
	asset->Read(&activeHeight_);
	asset->Read(&colorCount_);
	palette_=static_cast<unsigned short*>(Malloc(sizeof(unsigned short)*colorCount_));
	asset->Read(palette_,colorCount_);
	asset->Read(&usesMask_);
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
	asset->Read(&xOffset_,1);
	asset->Read(&yOffset_,1);
	}


//*** WriteToAsset ***

void Bitmap_RLE8::WriteToAsset(Asset* asset) const
	{
	asset->Write(&width_);
	asset->Write(&height_);
	asset->Write(&activeWidth_);
	asset->Write(&activeHeight_);
	asset->Write(&colorCount_);
	asset->Write(palette_,colorCount_);
	asset->Write(&usesMask_);
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
	asset->Write(&xOffset_);
	asset->Write(&yOffset_);
	}


//*** BlitRLE ***

void Bitmap_RLE8::BlitRLE(Bitmap_16bit* target,int x, int y,unsigned short modulate, unsigned char alpha) const
	{
	Blit(0,0,activeWidth_-1,activeHeight_-1,*target,x,y,modulate,alpha);
	}


//*** Blit ***

void Bitmap_RLE8::BlitRLE(int x1, int y1, int x2, int y2, Bitmap_16bit* target,int x, int y, unsigned short modulate, unsigned char alpha) const
	{
	// Check for empty bitmap
	if (!opaqueData_ && !alphaData_)
		{
		return;
		}

	if (alpha==0)
		{
		return;
		}


	int clipX1=x+xOffset_;
	int clipY1=y+yOffset_;
	int clipX2=x+xOffset_+(x2-x1);
	int clipY2=y+yOffset_+(y2-y1);

	x-=x1;
	y-=y1;

	if (clipX1<0)
		clipX1=0;
	if (clipY1<0)
		clipY1=0;
	if (clipX2>=target->GetWidth())
		clipX2=target->GetWidth()-1;
	if (clipY2>=target->GetHeight())
		clipY2=target->GetHeight()-1;

	// Set up palette
	currentPalette_=palette_;
	if (modulate!=0xffff)
		{
		if (!modulatedPalette_)
			{
			modulatedPalette_=static_cast<unsigned short*>(Malloc(sizeof(unsigned short)*colorCount_));
			}
		for (int i=0; i<colorCount_; i++)
			{
			int c=palette_[i];
			unsigned int r=(c & 0xf800)>>11;
			unsigned int g=(c & 0x7e0)>>5;
			unsigned int b=(c & 0x1f);
			unsigned int mr=(modulate & 0xf800)>>11;
			unsigned int mg=(modulate & 0x7e0)>>5;
			unsigned int mb=(modulate & 0x1f);
			r*=mr;
			g*=mg;
			b*=mb;
			r>>=5;
			g>>=6;
			b>>=5;
			modulatedPalette_[i]=(unsigned short)((r<<11)|(g<<5)|(b));
			}
		currentPalette_=modulatedPalette_;
		}
	int targetDelta=target->GetWidth()-activeWidth_;
	int tx1=x+xOffset_;
	int ty1=y+yOffset_;
	int tx2=tx1+activeWidth_-1;
	int ty2=ty1+activeHeight_-1;
	unsigned short* data=&(target->GetColorData())[tx1+ty1*target->GetWidth()];

	// Do we need to clip?
	if (tx1>=clipX1 && ty1>=clipY1 && tx2<=clipX2 && ty2<=clipY2)
		{
		// Render opaque part
		if (opaqueData_)
			{
			if (usesMask_)
				{
				if (alpha==255)
					{
					BlitterRLE8::Opaque_Unclipped_Masked(opaqueData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1);
					}
				else
					{
					BlitterRLE8::Opaque_Unclipped_Masked_Transparent(opaqueData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,alpha);
					}
				}
			else
				{
				if (alpha==255)
					{
					BlitterRLE8::Opaque_Unclipped_Unmasked(opaqueData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1);
					}
				else
					{
					BlitterRLE8::Opaque_Unclipped_Unmasked_Transparent(opaqueData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,alpha);
					}
				}
			}

		// Render alpha part
		if (alphaData_)
			{
			if (alpha==255)
				{
				BlitterRLE8::Alpha_Unclipped(alphaData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1);
				}
			else
				{
				BlitterRLE8::Alpha_Unclipped_Transparent(alphaData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,alpha);
				}
			}
		}
	else	// Yes, clipping required
		{
		// Trivial rejection test
		if (tx2<clipX1 || ty2<clipY1 || tx1>clipX2 || ty1>clipY2)
			return;

		// Calculate visible part
		int xStart=0;
		int yStart=0;
		int xEnd=0;
		int yEnd=0;

		if (tx1<clipX1)
			{
			xStart=clipX1-tx1;
			}
		if (ty1<clipY1)
			{
			yStart=clipY1-ty1;
			}
		if (tx2>clipX2)
			{
			xEnd=tx2-clipX2+1;
			}
		if (ty2>clipY2)
			{
			yEnd=ty2-clipY2+1;
			}

		// Render opaque part
		if (opaqueData_)
			{
			if (usesMask_)
				{
				if (alpha==255)
					{
					BlitterRLE8::Opaque_Clipped_Masked(opaqueData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,xStart,yStart,xEnd,yEnd);
					}
				else
					{
					BlitterRLE8::Opaque_Clipped_Masked_Transparent(opaqueData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,xStart,yStart,xEnd,yEnd,alpha);
					}
				}
			else
				{
				if (alpha==255)
					{
					BlitterRLE8::Opaque_Clipped_Unmasked(opaqueData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,xStart,yStart,xEnd,yEnd);
					}
				else
					{
					BlitterRLE8::Opaque_Clipped_Unmasked_Transparent(opaqueData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,xStart,yStart,xEnd,yEnd,alpha);
					}
				}
			}

		// Render alpha part
		if (alphaData_)
			{
			if (alpha==255)
				{
				BlitterRLE8::Alpha_Clipped(alphaData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,xStart,yStart,xEnd,yEnd);
				}
			else
				{
				BlitterRLE8::Alpha_Clipped_Transparent(alphaData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,xStart,yStart,xEnd,yEnd,alpha);
				}
			}
		}
	}


//*** Blit ***

void Bitmap_RLE8::MaxBlitRLE(int x1, int y1, int x2, int y2, Bitmap_16bit* target,int x, int y, unsigned short modulate, unsigned char alpha) const
	{
	// Check for empty bitmap
	if (!opaqueData_ && !alphaData_)
		{
		return;
		}

	if (alpha==0)
		{
		return;
		}


	int clipX1=x+xOffset_;
	int clipY1=y+yOffset_;
	int clipX2=x+xOffset_+(x2-x1);
	int clipY2=y+yOffset_+(y2-y1);

	x-=x1;
	y-=y1;

	if (clipX1<0)
		clipX1=0;
	if (clipY1<0)
		clipY1=0;
	if (clipX2>=target->GetWidth())
		clipX2=target->GetWidth()-1;
	if (clipY2>=target->GetHeight())
		clipY2=target->GetHeight()-1;

	// Set up palette
	currentPalette_=palette_;
	if (modulate!=0xffff)
		{
		if (!modulatedPalette_)
			{
			modulatedPalette_=static_cast<unsigned short*>(Malloc(sizeof(unsigned short)*colorCount_));
			}
		for (int i=0; i<colorCount_; i++)
			{
			int c=palette_[i];
			unsigned int r=(c & 0xf800)>>11;
			unsigned int g=(c & 0x7e0)>>5;
			unsigned int b=(c & 0x1f);
			unsigned int mr=(modulate & 0xf800)>>11;
			unsigned int mg=(modulate & 0x7e0)>>5;
			unsigned int mb=(modulate & 0x1f);
			r*=mr;
			g*=mg;
			b*=mb;
			r>>=5;
			g>>=6;
			b>>=5;
			modulatedPalette_[i]=(unsigned short)((r<<11)|(g<<5)|(b));
			}
		currentPalette_=modulatedPalette_;
		}
	int targetDelta=target->GetWidth()-activeWidth_;
	int tx1=x+xOffset_;
	int ty1=y+yOffset_;
	int tx2=tx1+activeWidth_-1;
	int ty2=ty1+activeHeight_-1;
	unsigned short* data=&(target->GetColorData())[tx1+ty1*target->GetWidth()];

	// Do we need to clip?
	if (tx1>=clipX1 && ty1>=clipY1 && tx2<=clipX2 && ty2<=clipY2)
		{
		// Render opaque part
		if (opaqueData_)
			{
			if (usesMask_)
				{
				if (alpha==255)
					{
					MaxBlitterRLE8::Opaque_Unclipped_Masked(opaqueData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1);
					}
				else
					{
					MaxBlitterRLE8::Opaque_Unclipped_Masked_Transparent(opaqueData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,alpha);
					}
				}
			else
				{
				if (alpha==255)
					{
					MaxBlitterRLE8::Opaque_Unclipped_Unmasked(opaqueData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1);
					}
				else
					{
					MaxBlitterRLE8::Opaque_Unclipped_Unmasked_Transparent(opaqueData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,alpha);
					}
				}
			}

		// Render alpha part
		if (alphaData_)
			{
			if (alpha==255)
				{
				MaxBlitterRLE8::Alpha_Unclipped(alphaData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1);
				}
			else
				{
				MaxBlitterRLE8::Alpha_Unclipped_Transparent(alphaData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,alpha);
				}
			}
		}
	else	// Yes, clipping required
		{
		// Trivial rejection test
		if (tx2<clipX1 || ty2<clipY1 || tx1>clipX2 || ty1>clipY2)
			return;

		// Calculate visible part
		int xStart=0;
		int yStart=0;
		int xEnd=0;
		int yEnd=0;

		if (tx1<clipX1)
			{
			xStart=clipX1-tx1;
			}
		if (ty1<clipY1)
			{
			yStart=clipY1-ty1;
			}
		if (tx2>clipX2)
			{
			xEnd=tx2-clipX2+1;
			}
		if (ty2>clipY2)
			{
			yEnd=ty2-clipY2+1;
			}

		// Render opaque part
		if (opaqueData_)
			{
			if (usesMask_)
				{
				if (alpha==255)
					{
					MaxBlitterRLE8::Opaque_Clipped_Masked(opaqueData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,xStart,yStart,xEnd,yEnd);
					}
				else
					{
					MaxBlitterRLE8::Opaque_Clipped_Masked_Transparent(opaqueData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,xStart,yStart,xEnd,yEnd,alpha);
					}
				}
			else
				{
				if (alpha==255)
					{
					MaxBlitterRLE8::Opaque_Clipped_Unmasked(opaqueData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,xStart,yStart,xEnd,yEnd);
					}
				else
					{
					MaxBlitterRLE8::Opaque_Clipped_Unmasked_Transparent(opaqueData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,xStart,yStart,xEnd,yEnd,alpha);
					}
				}
			}

		// Render alpha part
		if (alphaData_)
			{
			if (alpha==255)
				{
				MaxBlitterRLE8::Alpha_Clipped(alphaData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,xStart,yStart,xEnd,yEnd);
				}
			else
				{
				MaxBlitterRLE8::Alpha_Clipped_Transparent(alphaData_,activeWidth_,activeHeight_,currentPalette_,data,targetDelta,tx1,ty1,xStart,yStart,xEnd,yEnd,alpha);
				}
			}
		}
	}

//*** Blit ***

void Bitmap_RLE8::BlitRLE(Bitmap_16bitAlpha* target,int x, int y, unsigned short modulate) const
	{
	Blit(0,0,activeWidth_-1,activeHeight_-1,*target,x,y,modulate);
	}


//*** Blit ***

void Bitmap_RLE8::BlitRLE(int x1, int y1, int x2, int y2, Bitmap_16bitAlpha* target, int x, int y, unsigned short modulate) const
	{
	// Check for empty bitmap
	if (!opaqueData_ && !alphaData_)
		{
		return;
		}

	x-=x1;
	y-=y1;


	int clipX1=x+xOffset_;
	int clipY1=y+yOffset_;
	int clipX2=x+xOffset_+(x2-x1);
	int clipY2=y+yOffset_+(y2-y1);


	if (clipX1<0)
		clipX1=0;
	if (clipY1<0)
		clipY1=0;
	if (clipX2>=target->GetWidth())
		clipX2=target->GetWidth()-1;
	if (clipY2>=target->GetHeight())
		clipY2=target->GetHeight()-1;

	// Set up palette
	currentPalette_=palette_;
	if (modulate!=0xffff)
		{
		if (!modulatedPalette_)
			{
			modulatedPalette_=static_cast<unsigned short*>(Malloc(sizeof(unsigned short)*colorCount_));
			}
		for (int i=0; i<colorCount_; i++)
			{
			int c=palette_[i];
			unsigned int r=(c & 0xf800)>>11;
			unsigned int g=(c & 0x7e0)>>5;
			unsigned int b=(c & 0x1f);
			unsigned int mr=(modulate & 0xf800)>>11;
			unsigned int mg=(modulate & 0x7e0)>>5;
			unsigned int mb=(modulate & 0x1f);
			r*=mr;
			g*=mg;
			b*=mb;
			r>>=5;
			g>>=6;
			b>>=5;
			modulatedPalette_[i]=(unsigned short)((r<<11)|(g<<5)|(b));
			}
		currentPalette_=modulatedPalette_;
		}

	int targetDelta=target->GetWidth()-activeWidth_;
	int tx1=x+xOffset_;
	int ty1=y+yOffset_;
	int tx2=tx1+activeWidth_-1;
	int ty2=ty1+activeHeight_-1;
	unsigned short* colorData=&(target->GetColorData())[tx1+ty1*target->GetWidth()];
	unsigned char* alphaData=&(target->GetAlphaData())[tx1+ty1*target->GetWidth()];

	// Do we need to clip?
	if (tx1>=clipX1 && ty1>=clipY1 && tx2<=clipX2 && ty2<=clipY2)
		{
		// Render alpha part
		if (alphaData_)
			{
			CopperRLE8::Alpha_Unclipped(alphaData_,activeWidth_,activeHeight_,currentPalette_,colorData, alphaData, targetDelta, tx1, ty1);
			}

		// Render opaque part
		if (opaqueData_)
			{
			if (usesMask_)
				{
				CopperRLE8::Opaque_Unclipped_Masked(opaqueData_,activeWidth_,activeHeight_,currentPalette_,colorData, alphaData, targetDelta, tx1, ty1);
				}
			else
				{
				CopperRLE8::Opaque_Unclipped_Unmasked(opaqueData_,activeWidth_,activeHeight_,currentPalette_,colorData, alphaData, targetDelta, tx1, ty1);
				}
			}

		}
	else	// Yes, clipping required
		{
		// Trivial rejection test
		if (tx2<clipX1 || ty2<clipY1 || tx1>clipX2 || ty1>clipY2)
			return;

		// Calculate visible part
		int xStart=0;
		int yStart=0;
		int xEnd=0;
		int yEnd=0;

		if (tx1<clipX1)
			{
			xStart=clipX1-tx1;
			}
		if (ty1<clipY1)
			{
			yStart=clipY1-ty1;
			}
		if (tx2>clipX2)
			{
			xEnd=tx2-clipX2+1;
			}
		if (ty2>clipY2)
			{
			yEnd=ty2-clipY2+1;
			}

		// Render opaque part
		if (opaqueData_)
			{
			if (usesMask_)
				{
				CopperRLE8::Opaque_Clipped_Masked(opaqueData_,activeWidth_,activeHeight_,currentPalette_,colorData, alphaData, targetDelta, tx1, ty1, xStart, yStart, xEnd, yEnd);
				}
			else
				{
				CopperRLE8::Opaque_Clipped_Unmasked(opaqueData_,activeWidth_,activeHeight_,currentPalette_,colorData, alphaData, targetDelta, tx1, ty1, xStart, yStart, xEnd, yEnd);
				}
			}

		// Render alpha part
		if (alphaData_)
			{
			CopperRLE8::Alpha_Clipped(alphaData_,activeWidth_,activeHeight_,currentPalette_,colorData, alphaData, targetDelta, tx1, ty1, xStart, yStart, xEnd, yEnd);
			}
		}
	}


//*** IgnoreOpaque ***

void IgnoreOpaque(int len, unsigned char** source)
	{
	unsigned char* dataRLE=*source;
	if (len&0x80)
		{
		len&=0x7f;
		dataRLE+=len;
		}
	else
		{
		++dataRLE;
		}
	*source=dataRLE;
	}


//*** IgnoreAlpha ***

void IgnoreAlpha(int len, unsigned char** source)
	{
	unsigned char* dataRLE=*source;
	if (len&0x80)
		{
		len&=0x7f;
		dataRLE+=len*2;
		}
	else
		{
		dataRLE+=2;
		}
	*source=dataRLE;
	}


//*** RLEGetPixelAlpha ***

unsigned char Bitmap_RLE8::RLEGetPixelAlpha(int x, int y) const
	{
	if (x<xOffset_ || x>=xOffset_+activeWidth_ || y<yOffset_ || y>=yOffset_+activeHeight_)
		{
		return 0;
		}

	// First, test opaque part
	unsigned char* dataRLE=opaqueData_;

	// Skip clipped rows
	for (int i=0; i<y-yOffset_; i++)
		{
		int xi=0;
		while (xi<activeWidth_)
			{
			// Get run length
			int len=*dataRLE;
			++dataRLE;

			IgnoreOpaque(len,&dataRLE);
			len&=0x7f;

			xi+=len;
			}
		}

	int xi=0;
	while (xi<=x-xOffset_)
		{

		// Get run length
		int len=*dataRLE;
		++dataRLE;
		if (len&0x80)
			{
			len&=0x7f;
			// Unique values
			for (int i=0; i<len; ++i)
				{
				int c=*dataRLE;
				if (xi==x-xOffset_)
					{
					if (c!=255)
						return 255;
					}
				++dataRLE;
				xi++;
				}
			}
		else
			{
			int c=*dataRLE;
			if (xi==x-xOffset_ || (xi+len)>=x-xOffset_)
				{
				if (c!=255)
					return 255;
				}

			++dataRLE;
			xi+=len;
			}
		}


	// Test alpha part
	dataRLE=alphaData_;

	// Skip clipped rows
	for (int i=0; i<y-yOffset_; i++)
		{
		int xi=0;
		while (xi<activeWidth_)
			{
			// Get run length
			int len=*dataRLE;
			++dataRLE;

			IgnoreAlpha(len,&dataRLE);
			len&=0x7f;

			xi+=len;
			}
		}

	xi=0;
	while (xi<=x-xOffset_)
		{

		// Get run length
		int len=*dataRLE;
		++dataRLE;
		if (len&0x80)
			{
			len&=0x7f;
			// Unique values
			for (int i=0; i<len; ++i)
				{
				int alpha=*dataRLE;
				if (xi==x-xOffset_)
					return (unsigned char)alpha;
				++dataRLE;
				++dataRLE;
				xi++;
				}
			}
		else
			{
			int alpha=*dataRLE;
			if (xi==x-xOffset_ || (xi+len)>=x-xOffset_)
				return (unsigned char)alpha;

			++dataRLE;
			++dataRLE;
			xi+=len;
			}
		}
	return 0;
	}


//*** RLEGetPixelColor ***

unsigned short Bitmap_RLE8::RLEGetPixelColor(int x, int y) const
	{
	return 0;
	}



//*** Save ***

void Bitmap_RLE8::Save(Asset& asset) const
	{
	if (asset.Create())
		{
		asset.Write(Pixie_Rle_Header,StrLen(Pixie_Rle_Header));
		int version=0;
		asset.Write(&version);
		int celCount=1;
		asset.Write(&celCount);
		WriteToAsset(&asset);
		asset.Close();
		}
	}


//*** Load ***

void Bitmap_RLE8::Load(const Asset& asset)
	{
	if (asset.Open())
		{
		char header[8];
		asset.Read(header,8);

		if (StrNCmp(header,"PIXRLE8B",8)==0)
			{
			int version=0;
			asset.Read(&version);
			if (version==0)
				{
				int celCount=0;
				asset.Read(&celCount);
				if (celCount>=1)
					{
					ReadFromAsset(&asset);
					}
				}
			}

		else if (StrNCmp(header,"PIXIE_RL",8)==0)
			{
			char c;
			asset.Read(&c);
			int version=0;
			asset.Read(&version);
			if (version==0)
				{
				int celCount=0;
				asset.Read(&celCount);
				if (celCount>=1)
					{
					ReadFromAsset(&asset);
					}
				}
			}
		else
			{
			Assert(false,"Invalid RLE header");
			}
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


//*** GetX1 ***

int Bitmap_RLE8::GetX1() const
	{
	return xOffset_;
	}


//*** GetY1 ***

int Bitmap_RLE8::GetY1() const
	{
	return yOffset_;
	}


//*** GetX2 ***

int Bitmap_RLE8::GetX2() const
	{
	return xOffset_+activeWidth_;
	}


//*** GetY2 ***

int Bitmap_RLE8::GetY2() const
	{
	return yOffset_+activeHeight_;
	}


//*** GetOpaqueSize ***

unsigned int Bitmap_RLE8::GetOpaqueSize() const
	{
	return opaqueSize_;
	}


//*** GetOpaqueData ***

unsigned char* Bitmap_RLE8::GetOpaqueData() const
	{
	return opaqueData_;
	}

}; // namespace pixie
