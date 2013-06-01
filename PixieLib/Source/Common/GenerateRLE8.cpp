//*** GenerateRLE8.h ***

#include "GenerateRLE8.h"
#include "Image.h"
#include "ColorHelper.h"
#include "StandardLibrary.h"
#include "Bitmap_RLE8.h"
#include "MedianCutPalettizer.h"
#include "FloydSteinbergDither.h"

#include "HashTable.h"
#include "HashTableIterator.h"
#include "HashTableKey_Int.h"


//*** Constructor ***

GenerateRLE8::GenerateRLE8(const Image& image, Bitmap_RLE8* bitmap, bool dither):
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
	palette_(0)
	{
	if( image.GetWidth() > 0 && image.GetHeight() > 0 )
		{
		CreateFromImage(&image, dither);
		}

	bitmap->usesMask_=usesMask_;
	bitmap->xOffset_=xOffset_;
	bitmap->yOffset_=yOffset_;
	bitmap->activeWidth_=activeWidth_;
	bitmap->activeHeight_=activeHeight_;
	bitmap->width_=width_;
	bitmap->height_=height_;
	bitmap->opaqueSize_=opaqueSize_;
	bitmap->opaqueData_=opaqueData_;
	bitmap->alphaSize_=alphaSize_;
	bitmap->alphaData_=alphaData_;
	bitmap->colorCount_=colorCount_;
	bitmap->palette_=palette_;

	usesMask_=0;
	xOffset_=0;
	yOffset_=0;
	activeWidth_=0;
	activeHeight_=0;
	width_=0;
	height_=0;
	opaqueSize_=0;
	opaqueData_=0;
	alphaSize_=0;
	alphaData_=0;
	colorCount_=0;
	palette_=0;
	}




//*** CalculateDiff ***

int CalculateDiff(unsigned short a, unsigned short b)
	{
	unsigned char ar=(unsigned char)((a & 0xf800)>>11);
	unsigned char ag=(unsigned char)((a & 0x7e0)>>5);
	unsigned char ab=(unsigned char)((a & 0x1f));
	unsigned char br=(unsigned char)((b & 0xf800)>>11);
	unsigned char bg=(unsigned char)((b & 0x7e0)>>5);
	unsigned char bb=(unsigned char)((b & 0x1f));

	// Diff	
	int dr=Abs(ar-br);
	int dg=Abs(ab-bb);
	int db=Abs(ag-bg);
	
	return dr+dg+db;
	}




//*** CreateFromImage ***

void GenerateRLE8::CreateFromImage(const Image* image, bool dither)
	{
	width_=(unsigned short)image->GetWidth();
	height_=(unsigned short)image->GetHeight();
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

	activeWidth_=(unsigned short)(maxX-minX+1);
	activeHeight_=(unsigned short)(maxY-minY+1);
	
	xOffset_=(unsigned short)minX;
	yOffset_=(unsigned short)minY;

	// First, create palettized version
	unsigned char* data=0;
	unsigned char* mask=0;
	int alphaCount = 0;
	Palettize(image,&data,&mask,&palette_,&colorCount_,&alphaCount,dither);
	
	if (mask)
		usesMask_=1;
	else
		usesMask_=0;

	// Create RLE data
	opaqueSize_=GetRLESize(data,mask);
	if (opaqueSize_>0)
		{
		opaqueData_=static_cast<unsigned char*>(Malloc(opaqueSize_));
		CreateRLE(data,mask,opaqueData_);
		}

	// Create RLE mask
	if (mask && alphaCount )
		{
		alphaSize_=GetRLESize_Alpha(data,mask);
		if (alphaSize_>0)
			{
			alphaData_=static_cast<unsigned char*>(Malloc(alphaSize_));
			CreateRLE_Alpha(data,mask,alphaData_);
			}
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


//*** GetRunLength ***

int	GenerateRLE8::GetRunLength(unsigned char* data, unsigned char* mask,int size)
	{
	int length=0;
	int currCol=*data;
	if (mask && *mask<255)
		{
		currCol=255;
		}
	int c=currCol;
	while ((currCol==c) && size>0)
		{
		length++;

		data++;
		if (mask)
			{
			mask++;
			}
		size--;

		if (size>0)
			{
			currCol=*data;
			if (mask && *mask<255)
				{
				currCol=255;
				}
			}
		}
	return length;
	}


//*** GetUniquesLength ***

int	GenerateRLE8::GetUniquesLength(unsigned char* data, unsigned char* mask,int size)
	{
	int length=0;
	while (size>0 && GetRunLength(data,mask,size)<=2)
		{
		length++;
		data++;
		if (mask)
			{
			mask++;
			}
		size--;
		}
	return length;
	}


//*** GetRLESize ***

int GenerateRLE8::GetRLESize(unsigned char* data,unsigned char* mask)
	{
	return CreateRLE(data,mask,0);
	}


//*** CreateRLE ***

int GenerateRLE8::CreateRLE(unsigned char* source, unsigned char* mask, unsigned char* destination)
	{
	int size=activeWidth_*activeHeight_;
	int result=0;	
	int xleft=activeWidth_;
	while (size>0)
		{
		unsigned char len=(unsigned char)GetRunLength(source,mask,Min(127,Min(size,xleft)));
		if (len<=2)
			{
			unsigned char len=(unsigned char)GetUniquesLength(source,mask,Min(127,Min(size,xleft)));

			result++; // Count value
			if (destination)
				{
				*destination=(len | 0x80); // Top bit set to indicate run of unique colors
				destination++;
				}
			
			result+=len; // Unique colors
			if (destination)
				{
				unsigned char* colorsource=source;
				unsigned char* maskvalue=mask;
				for (int i=0; i<len; i++)
					{
					if (mask && *maskvalue<255)
						*destination=255;	// Transparent or semitransparent pixel
					else
						*destination=*colorsource;
					destination++;
					colorsource++;
					if (mask)
						maskvalue++;
					}
				}
		
			if (mask)
				mask+=len;
			source+=len;
			xleft-=len;
			Assert(xleft>=0,"Inavlid run length");
			if (xleft==0)
				xleft=activeWidth_;
			size-=len;
			}
		else
			{
			result++; // Count value
			if (destination)
				{
				*destination=len; // Top bit not set to indicate run of same color
				destination++;
				}
		
			result++; // Repeated color
			if (destination)
				{
				if (mask && *mask<255)
					*destination=255;	// Transparent or semitransparent pixel
				else
					*destination=*source;
				destination++;
				}
				
			source+=len;
			xleft-=len;
			Assert(xleft>=0,"Inavlid run length");
			if (xleft==0)
				xleft=activeWidth_;
			if (mask)
				mask+=len;
			size-=len;
			}
		}
		
	return result;
	}	
	


//*** GetRunLength_Alpha ***

int	GenerateRLE8::GetRunLength_Alpha(unsigned char* data, unsigned char* mask,int size)
	{
	int length=0;
	int currCol=*data;
	int currMask=*mask;
	if (currMask==0 || currMask==255)
		{
		currMask=0;
		currCol=255;
		}
	int c=currCol;
	int m=currMask;
	while ((currCol==c) && (currMask==m) && size>0)
		{
		length++;
		data++;
		mask++;
		size--;
		if (size>0)
			{
			currCol=*data;
			currMask=*mask;
			if (currMask==0 || currMask==255)
				{
				currMask=0;
				currCol=255;
				}
			}
		}
	return length;
	}


//*** GetUniquesLength_Alpha ***

int	GenerateRLE8::GetUniquesLength_Alpha(unsigned char* data, unsigned char* mask,int size)
	{
	int length=0;
	while (size>0 && GetRunLength_Alpha(data,mask,size)<=2)
		{
		length++;
		data++;
		mask++;
		size--;
		}
	return length;
	}


//*** GetRLESize_Alpha ***

int GenerateRLE8::GetRLESize_Alpha(unsigned char* data,unsigned char* mask)
	{
	return CreateRLE_Alpha(data,mask,0);
	}


//*** CreateRLE_Alpha ***

int GenerateRLE8::CreateRLE_Alpha(unsigned char* source, unsigned char* mask, unsigned char* destination)
	{
	int size=activeWidth_*activeHeight_;
	int result=0;	
	int xleft=activeWidth_;
	while (size>0)
		{
		unsigned char len=(unsigned char)GetRunLength_Alpha(source,mask,Min(127,Min(size,xleft)));
		if (len<=2)
			{
			unsigned char len=(unsigned char)GetUniquesLength_Alpha(source,mask,Min(127,Min(size,xleft)));

			result++; // Count value
			if (destination)
				{
				*destination=(len | 0x80); // Top bit set to indicate run of unique colors
				destination++;
				}
			
			// Unique colors
			unsigned char* colorsource=source;
			unsigned char* maskvalue=mask;
			for (int i=0; i<len; i++)
				{
				if (*maskvalue==0 || *maskvalue==255)
					{
					if (destination)
						{
						*destination=0;	// Fully transparent pixel
						destination++;
						*destination=0;	
						destination++;
						}
					result+=2;
					}
				else
					{
					if (destination)
						{
						*destination=*maskvalue;	// Semi-transparent pixel
						destination++;
						*destination=*colorsource;
						destination++;
						}
					result+=2;
					}
				colorsource++;
				maskvalue++;
				}
				
			mask+=len;
			source+=len;
			xleft-=len;
			Assert(xleft>=0,"Invalid run length");
			if (xleft==0)
				xleft=activeWidth_;
			size-=len;
			}
		else
			{
			result++; // Count value
			if (destination)
				{
				*destination=len; // Top bit not set to indicate run of same color
				destination++;
				}
		
			// Repeated color

			if (*mask==0 || *mask==255)
				{
				if (destination)
					{
					*destination=0;	// Fully transparent pixel
					destination++;
					*destination=0;
					destination++;
					}
				result+=2;
				}
			else
				{
				if (destination)
					{
					*destination=*mask;
					destination++;
					*destination=*source;
					destination++;
					}
				result+=2;
				}
							
			source+=len;
			xleft-=len;
			Assert(xleft>=0,"Invalid run length");
			if (xleft==0)
				xleft=activeWidth_;
			mask+=len;
			size-=len;
			}
		}
		
	return result;
	}	





//*** Palettize ***

void GenerateRLE8::Palettize(const Image* image,unsigned char** data, unsigned char** mask, unsigned short** palette, unsigned char* colorCount, int* alphaCount, bool dither)	
	{
	int usedPaletteEntries=0;
	*data=static_cast<unsigned char*>(Malloc(sizeof(unsigned char)*activeWidth_*activeHeight_));
	*mask=static_cast<unsigned char*>(Malloc(sizeof(unsigned char)*activeWidth_*activeHeight_));
	unsigned char* img=*data;
	unsigned char* maskimg=*mask;
	bool maskUsed=false;
	bool maskUsedTrans=false;

	*alphaCount = 0;

	// Create palette
	unsigned short pal16[256];
	usedPaletteEntries=MedianCutPalettizer::GeneratePalette(image->GetPointer(),image->GetWidth(),image->GetHeight(),pal16,255);

	// Dither
	unsigned int pal[256];
	for (int i=0; i<usedPaletteEntries; i++)
		{
		pal[i]=RGB16TO32(pal16[i]);
		}
	unsigned char* palettized=(unsigned char*)Malloc(image->GetWidth()*image->GetHeight());

	if (dither)
		{
		FloydSteinbergDither::DitherImage(image->GetPointer(),image->GetWidth(), image->GetHeight(),pal,usedPaletteEntries,palettized);
		}
	else
		{
		MedianCutPalettizer::PalettizeImage(image->GetPointer(),image->GetWidth(), image->GetHeight(),pal,usedPaletteEntries,palettized);
		}

	int emptyCount = 0;
	int fullCount = 0;
	for (int y=0; y<activeHeight_; y++)
		{
		for (int x=0; x<activeWidth_; x++)
			{
			unsigned int color=image->GetPixel(xOffset_+x,yOffset_+y);
			unsigned char a=((unsigned char)(((color)&0xff000000)>>24));
			unsigned char colIndex=palettized[xOffset_+x+image->GetWidth()*(y+yOffset_)];
			*img=colIndex;
			img++;
			*maskimg=a;
			maskimg++;

			if (a<255)
				{
				maskUsed=true;
				if (a>0)
					{
					(*alphaCount)++;
					maskUsedTrans=true;
					}
				else
					{
					emptyCount ++;
					}
				}
			else
				{
				fullCount++;
				}
			}
		}

	Free(palettized);

	if (!maskUsed)
		{
		Free(*mask);
		*mask=0;
		}
	
	*palette=static_cast<unsigned short*>(Malloc(sizeof(unsigned short)*usedPaletteEntries));
//	MemCpy(*palette,pal,sizeof(unsigned short)*usedPaletteEntries);
	for (int i=0; i<usedPaletteEntries; i++)
		{
//		unsigned short c=RGB32TO16(pal32[i]);
		unsigned short c=pal16[i];
		(*palette)[i]=c;
		}
	*colorCount=(unsigned char)usedPaletteEntries;
	}	
	
	
//*** Crop ***

void GenerateRLE8::Crop(const Image* image, int* minX, int* minY, int* maxX, int* maxY)
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

