//*** FloydSteinbergDither.h ***

#include "FloydSteinbergDither.h"

#include "StandardLibrary.h"
#include "Debug.h"
#include "ColorHelper.h"

#pragma warning (disable:4244)

typedef struct {
    unsigned char R, G, B, A;
} RGBTriple;

typedef struct {
    int size;
    RGBTriple* table;
} RGBPalette;

typedef struct {
    int width, height;
    RGBTriple* pixels;
} RGBImage;

typedef struct {
    int width, height;
    unsigned char* pixels;
} PalettizedImage;

#define plus_truncate_uchar(a, b) \
    if (((int)(a)) + (b) < 0) \
        (a) = 0; \
    else if (((int)(a)) + (b) > 255) \
        (a) = 255; \
    else \
        (a) += (b);


static
unsigned char FindNearestColor(RGBTriple color, RGBPalette palette) {
    int i, distanceSquared, minDistanceSquared, bestIndex = 0;
    minDistanceSquared = 255*255 + 255*255 + 255*255 + 1;
    for (i=0; i<palette.size; i++) {
        int Rdiff = ((int)color.R) - palette.table[i].R;
        int Gdiff = ((int)color.G) - palette.table[i].G;
        int Bdiff = ((int)color.B) - palette.table[i].B;
        distanceSquared = Rdiff*Rdiff + Gdiff*Gdiff + Bdiff*Bdiff;
        if (distanceSquared < minDistanceSquared) {
            minDistanceSquared = distanceSquared;
            bestIndex = i;
        }
    }
    return bestIndex;
}


#define compute_disperse(channel) \
error = ((int)(currentPixel->channel)) - palette.table[index].channel; \
if (x + 1 < image.width && image.pixels[(x+1) + (y+0)*image.width].A>0) { \
    plus_truncate_uchar(image.pixels[(x+1) + (y+0)*image.width].channel, (error*7) >> 4); \
} \
if (y + 1 < image.height) { \
    if (x - 1 > 0 && image.pixels[(x-1) + (y+1)*image.width].A>0) { \
        plus_truncate_uchar(image.pixels[(x-1) + (y+1)*image.width].channel, (error*3) >> 4); \
    } \
	if (image.pixels[(x+0) + (y+1)*image.width].A>0) { \
		plus_truncate_uchar(image.pixels[(x+0) + (y+1)*image.width].channel, (error*5) >> 4); \
	}\
    if (x + 1 < image.width && image.pixels[(x+1) + (y+1)*image.width].A>0) { \
        plus_truncate_uchar(image.pixels[(x+1) + (y+1)*image.width].channel, (error*1) >> 4); \
    } \
}


PalettizedImage FloydSteinbergDitherer(RGBImage image, RGBPalette palette)
	{
    PalettizedImage result;
    result.width = image.width;
    result.height = image.height;
    result.pixels = (unsigned char*)Malloc(sizeof(unsigned char) * result.width * result.height);

	int x, y;
    for(y = 0; y < image.height; y++) 
		{
        for(x = 0; x < image.width; x++) 
			{
			RGBTriple* currentPixel = &(image.pixels[x + y*image.width]);
			unsigned char index = FindNearestColor(*currentPixel, palette);
			result.pixels[x + y*result.width] = index;
			if (currentPixel->A==0)
				{
				result.pixels[x + y*result.width]=0;
				continue;
				}

			int error;
			compute_disperse(R);
			compute_disperse(G);
			compute_disperse(B);
			}
		}

    return result;
}


//*** DitherImage ***

void FloydSteinbergDither::DitherImage(unsigned int* image, int imageWidth, int imageHeight, unsigned int* palette, int paletteCount, unsigned char* outputData)
	{
	RGBPalette pal;
	pal.size=paletteCount;
	pal.table=new RGBTriple[paletteCount];
	for (int i=0; i<paletteCount; i++)
		{
		unsigned int color=palette[i];
		unsigned char a=((unsigned char)((color&0xff000000)>>24));
		unsigned char r=((unsigned char)((color&0x00ff0000)>>16));
		unsigned char g=((unsigned char)((color&0x0000ff00)>>8 ));
		unsigned char b=((unsigned char)((color&0x000000ff)    ));
		
		pal.table[i].A=a;
		pal.table[i].R=r;
		pal.table[i].G=g;
		pal.table[i].B=b;
		}

	RGBImage img;
	img.width=imageWidth;
	img.height=imageHeight;
	img.pixels=new RGBTriple[imageWidth*imageHeight];
	for (int y=0; y<imageHeight; y++)
		{
		for (int x=0; x<imageWidth; x++)
			{
			unsigned int color=image[x+y*imageWidth];
			unsigned char a=((unsigned char)((color&0xff000000)>>24));
			unsigned char r=((unsigned char)((color&0x00ff0000)>>16));
			unsigned char g=((unsigned char)((color&0x0000ff00)>>8 ));
			unsigned char b=((unsigned char)((color&0x000000ff)    ));
			RGBTriple t;
			t.A=a;
			t.R=r;
			t.G=g;
			t.B=b;
			img.pixels[x+y*imageWidth]=t;
			}
		}

	PalettizedImage palImg=FloydSteinbergDitherer(img,pal);
	delete[] pal.table;
	delete[] img.pixels;

	Assert(palImg.width==imageWidth && palImg.height==imageHeight,"Image sizes doesn't match");

	for (int y=0; y<palImg.height; y++)
		{
		for (int x=0; x<palImg.width; x++)
			{
			outputData[x+palImg.width*y]=palImg.pixels[x+palImg.width*y];
			}
		}
	Free(palImg.pixels);
	}


//*** DitherImage ***

void FloydSteinbergDither::DitherImage(unsigned int* image, int imageWidth, int imageHeight, unsigned short* outputData)
	{
	for (int y=0; y<imageHeight; y++)
		{
		for (int x=0; x<imageWidth; x++)
			{
			unsigned int color=image[x+y*imageWidth];
			unsigned char r=((unsigned char)((color&0x00ff0000)>>16));
			unsigned char g=((unsigned char)((color&0x0000ff00)>>8 ));
			unsigned char b=((unsigned char)((color&0x000000ff)    ));
			unsigned short color16 = RGB32TO16(color);
			unsigned char r16=((unsigned char)((RGB16TO32(color16)&0x00ff0000)>>16));
			unsigned char g16=((unsigned char)((RGB16TO32(color16)&0x0000ff00)>>8 ));
			unsigned char b16=((unsigned char)((RGB16TO32(color16)&0x000000ff)    ));
			outputData[x+y*imageWidth]=color16;
			
			int error = ((int)r) - r16;
			if (x + 1 < imageWidth) 
				{ 
				unsigned char& r=*(((unsigned char*)(&image[x+1+y*imageWidth]))+2);
				plus_truncate_uchar(r, (error*7) >> 4); 
				} 
			if (y + 1 < imageHeight) 
				{ 
				if (x - 1 > 0) 
					{ 
					unsigned char& r=*(((unsigned char*)(&image[x-1+(y+1)*imageWidth]))+2);
					plus_truncate_uchar(r, (error*3) >> 4); 
					} 
				unsigned char& r=*(((unsigned char*)(&image[x+0+(y+1)*imageWidth]))+2);
				plus_truncate_uchar(r, (error*5) >> 4); 
				if (x + 1 < imageWidth) 
					{ 
					unsigned char& r=*(((unsigned char*)(&image[x+1+(y+1)*imageWidth]))+2);
					plus_truncate_uchar(r, (error*1) >> 4); 
					} 
				}


			error = ((int)g) - g16;
			if (x + 1 < imageWidth) 
				{ 
				unsigned char& g=*(((unsigned char*)(&image[x+1+y*imageWidth]))+1);
				plus_truncate_uchar(g, (error*7) >> 4); 
				} 
			if (y + 1 < imageHeight) 
				{ 
				if (x - 1 > 0) 
					{ 
					unsigned char& g=*(((unsigned char*)(&image[x-1+(y+1)*imageWidth]))+1);
					plus_truncate_uchar(g, (error*3) >> 4); 
					} 
				unsigned char& g=*(((unsigned char*)(&image[x+0+(y+1)*imageWidth]))+1);
				plus_truncate_uchar(g, (error*5) >> 4); 
				if (x + 1 < imageWidth) 
					{ 
					unsigned char& g=*(((unsigned char*)(&image[x+1+(y+1)*imageWidth]))+1);
					plus_truncate_uchar(g, (error*1) >> 4); 
					} 
				}

			error = ((int)b) - b16;
			if (x + 1 < imageWidth) 
				{ 
				unsigned char& b=*(((unsigned char*)(&image[x+1+y*imageWidth]))+0);
				plus_truncate_uchar(b, (error*7) >> 4); 
				} 
			if (y + 1 < imageHeight) 
				{ 
				if (x - 1 > 0) 
					{ 
					unsigned char& b=*(((unsigned char*)(&image[x-1+(y+1)*imageWidth]))+0);
					plus_truncate_uchar(b, (error*3) >> 4); 
					} 
				unsigned char& b=*(((unsigned char*)(&image[x+0+(y+1)*imageWidth]))+0);
				plus_truncate_uchar(b, (error*5) >> 4); 
				if (x + 1 < imageWidth) 
					{ 
					unsigned char& b=*(((unsigned char*)(&image[x+1+(y+1)*imageWidth]))+0);
					plus_truncate_uchar(b, (error*1) >> 4); 
					} 
				}

			}

		}

	}
