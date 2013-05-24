//*** GenerateRLE16.h ***

#include "GenerateRLE16.h"
#include "Image.h"
#include "ColorHelper.h"
#include "StandardLibrary.h"
#include "Bitmap_RLE16.h"
#include "FloydSteinbergDither.h"
#include "Debug.h"

//*** Constructor ***

GenerateRLE16::GenerateRLE16(const Image& image, Bitmap_RLE16* bitmap, bool dither):
	opaqueSize_(0),
	opaqueData_(0),	
	alphaSize_(0),
	alphaData_(0),
	hPitch_(0),
	vPitch_(0),
	hOffset_(0),
	vOffset_(0)
	{
	CreateFromImage(&image, dither);

	bitmap->opaqueSize_=opaqueSize_;
	bitmap->opaqueData_=opaqueData_;
	bitmap->alphaSize_=alphaSize_;
	bitmap->alphaData_=alphaData_;
	bitmap->hPitch_=hPitch_;
	bitmap->vPitch_=vPitch_;
	bitmap->hOffset_=hOffset_;
	bitmap->vOffset_=vOffset_;

	opaqueSize_=0;
	opaqueData_=0;	
	alphaSize_=0;
	alphaData_=0;
	hPitch_=0;
	vPitch_=0;
	hOffset_=0;
	vOffset_=0;
	}




//*** Crop ***

void GenerateRLE16::Crop(const Image* image, int* minX, int* minY, int* maxX, int* maxY)
	{
	*minX=image->GetWidth();
	*minY=image->GetHeight();
	*maxX=0;
	*maxY=0;
	for (int y=0; y<image->GetHeight(); y++)
		{
		for (int x=0; x<image->GetWidth(); x++)
			{
			unsigned int color=image->GetPixel(x,y);
			if (color>>24)
				{
				if (y<*minY)
					*minY=y;
				if (x<*minX)
					*minX=x;
				if (y>*maxY)
					*maxY=y;
				if (x>*maxX)
					*maxX=x;
				}
			}
		}
	}


//*** CreateFromImage ***

void GenerateRLE16::CreateFromImage(const Image* image, bool dither)
	{
	int minX=0;
	int minY=0;
	int maxX=0;
	int maxY=0;
	
	// Crop image
	Crop(image,&minX,&minY,&maxX,&maxY);
	if (maxX<minX || maxY<minY)
		{
		return;
		}

	hPitch_=(unsigned short)(maxX-minX+1);
	vPitch_=(unsigned short)(maxY-minY+1);
	
	hOffset_=(unsigned short)minX;
	vOffset_=(unsigned short)minY;

	// First, create 16-bit version
	unsigned short* data=0;
	unsigned char* mask=0;
	Palettize(image,&data,&mask,dither);
	
	// Create RLE data
	opaqueSize_=GetRLESize(data,mask);
	if (opaqueSize_>0)
		{
		opaqueData_=static_cast<unsigned char*>(Malloc(opaqueSize_));
		CreateRLE(data,mask,opaqueData_);
		}

	// Create RLE mask
	alphaSize_=GetRLESize_Alpha(data,mask);
	if (alphaSize_>0)
		{
		alphaData_=static_cast<unsigned char*>(Malloc(alphaSize_));
		CreateRLE_Alpha(data,mask,alphaData_);
		}
		

	// Release temp stuff
	if (data)
		{
		Free(data);
		}
	if (mask)
		{
		Free(mask);
		}
	}



//*** Palettize ***

void GenerateRLE16::Palettize(const Image* image,unsigned short** data, unsigned char** mask, bool dither)	
	{
	*data=static_cast<unsigned short*>(Malloc(sizeof(unsigned short)*hPitch_*vPitch_));
	*mask=static_cast<unsigned char*>(Malloc(sizeof(unsigned char)*hPitch_*vPitch_));
	unsigned short* img=*data;
	unsigned char* maskimg=*mask;
	unsigned short* dithered=0;

	if (dither)
		{
		dithered=static_cast<unsigned short*>(Malloc(sizeof(unsigned short)*image->GetWidth()*image->GetHeight()));
		FloydSteinbergDither::DitherImage(image->GetPointer(),image->GetWidth(), image->GetHeight(),dithered);
		}


	for (int y=0; y<vPitch_; y++)
		{
		for (int x=0; x<hPitch_; x++)
			{
			unsigned int color=image->GetPixel(hOffset_+x,vOffset_+y);
			unsigned short color16=RGB32TO16(color);
			if (dithered)
				{
				color16=dithered[(x+hOffset_)+(y+vOffset_)*image->GetWidth()];
				}
			*img=color16;
			img++;

			unsigned char a=((unsigned char)((color&0xff000000)>>24));
			*maskimg=a;
			maskimg++;
			}
		}

	if (dithered)
		{
		Free(dithered);
		}
	}	


//*** GetRLESize ***

int GenerateRLE16::GetRLESize(unsigned short* data,unsigned char* mask)
	{
	return CreateRLE(data,mask,0);
	}


//*** CreateRLE ***

int GenerateRLE16::CreateRLE(unsigned short* source, unsigned char* mask, unsigned char* destination)
	{
	int result=0;
	int size=hPitch_*vPitch_;
	while (size>0)
		{
		unsigned int blank=0;
		while (size>0 && *mask<255)
			{
			blank++;
			size--;
			source++;
			mask++;
			}
		
		if (destination)
			{
			Assert(blank<=0xffff,"RLE run is too long - need code here to break it up");
			*reinterpret_cast<unsigned short*>(destination)=(unsigned short)blank;
			destination+=2;
			}
		result+=2;

		unsigned short* nonblankptr=reinterpret_cast<unsigned short*>(destination);
		if (destination)
			{
			destination+=2;
			}
		result+=2;

		unsigned int nonblank=0;
		while (size>0 && *mask==255)
			{
			if (destination)
				{
				*reinterpret_cast<unsigned short*>(destination)=*source;
				destination+=2;
				}
			result+=2;
			nonblank++;
			size--;
			source++;
			mask++;
			}
		
		if (destination)
			{
			Assert(nonblank<=0xffff,"RLE run is too long - need code here to break it up");
			*nonblankptr=(unsigned short)nonblank;
			}
		}
		
	return result;
	}	


//*** GetRLESize_Alpha ***

int GenerateRLE16::GetRLESize_Alpha(unsigned short* data,unsigned char* mask)
	{
	return CreateRLE_Alpha(data,mask,0);
	}


//*** CreateRLE_Alpha ***

int GenerateRLE16::CreateRLE_Alpha(unsigned short* source, unsigned char* mask, unsigned char* destination)
	{
	int result=0;
	int size=hPitch_*vPitch_;
	while (size>0)
		{
		unsigned int blank=0;
		while (size>0 && (*mask==0 || *mask==255))
			{
			blank++;
			size--;
			source++;
			mask++;
			}
		
		if (destination)
			{
			Assert(blank<=0xffff,"RLE run is too long - need code here to break it up");
			*reinterpret_cast<unsigned short*>(destination)=(unsigned short)blank;
			destination+=2;
			}
		result+=2;

		unsigned short* nonblankptr=reinterpret_cast<unsigned short*>(destination);
		if (destination)
			{
			destination+=2;
			}
		result+=2;
		unsigned int nonblank=0;
		while (size>0 && *mask>0 && *mask<255)
			{
			if (destination)
				{
				*reinterpret_cast<unsigned short*>(destination)=*static_cast<unsigned short*>(source);
				destination+=2;
				*static_cast<unsigned char*>(destination)=*static_cast<unsigned char*>(mask);
				destination+=1;
				}
			result+=3;
			nonblank++;
			size--;
			source++;
			mask++;
			}
		
		if (destination)
			{
			Assert(nonblank<=0xffff,"RLE run is too long - need code here to break it up");
			*nonblankptr=(unsigned short)nonblank;
			}
		}
		
	return result;
	}	


