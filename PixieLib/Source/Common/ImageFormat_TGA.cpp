//*** ImageFormat_TGA.cpp ***

#include "ImageFormat_TGA.h"
#include "Debug.h"
#include "Asset.h"
#include "StandardLibrary.h"
#include "Filename.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <stdio.h>
#include "targa/targa.h"

//*** Register ***

void ImageFormat_TGA::Register()
	{
	ImageFormat::RegisterImageFormat(TestAsset,Create);
	}


//*** Create ***

ImageFormat* ImageFormat_TGA::Create(const Asset& asset)
	{
	return new ImageFormat_TGA(asset);
	}


//*** TestAsset ***

bool ImageFormat_TGA::TestAsset(const Asset& asset)
	{
	// Check if the footer says that this is a TGA file
	if (asset.Open())
		{
		asset.Seek(-18,Asset::SEEK_FROM_END);
		char buffer[16];
		asset.Read(buffer,16);
		asset.Close();
		if (StrNCmp(buffer,"TRUEVISION-XFILE",16)==0)
			{
			return true;
			}
		}

	// If the footer doesn't match, this might still be a tga file (version < 2)
	// so try and load it as a tga and see if it works...
	int size=asset.GetSize();
	char* buffer=new char[size];
	asset.Open();
	asset.Read(buffer,size);
    asset.Close();

	tga_image image;
	tga_read_from_Buffer(&image,buffer);

	delete[] buffer;

	// Check to see if the header data makes sense...
	if (image.width<32767 && image.height<32767 && image.image_data && (image.pixel_depth==8 || image.pixel_depth==24 || image.pixel_depth==16 || image.pixel_depth==32))
		{
		// Yeah, sure, this looks like proper data, so give thumbs up and hope for the best
		return true;
		}

	tga_free_buffers(&image);

	// Nope, not likely to be a TGA
	return false;
	}


//*** Constructor ***

ImageFormat_TGA::ImageFormat_TGA(const Asset& asset):
	image_(0)
	{
	int size=asset.GetSize();
	char* buffer=new char[size];
	asset.Open();
	asset.Read(buffer,size);
    asset.Close();

	image_=new tga_image;
	tga_read_from_Buffer(static_cast<tga_image*>(image_),buffer);

	delete[] buffer;
    }


//*** Destructor ***

ImageFormat_TGA::~ImageFormat_TGA()
	{
	tga_free_buffers(static_cast<tga_image*>(image_));
	delete static_cast<tga_image*>(image_);
	}


//*** GetWidth ***

int ImageFormat_TGA::GetWidth()
	{
	return static_cast<tga_image*>(image_)->width;
	}


//*** GetHeight ***

int ImageFormat_TGA::GetHeight()
	{
	return static_cast<tga_image*>(image_)->height;
	}


//*** GetCelCount ***

int ImageFormat_TGA::GetCelCount()
	{
	return 1;
	}


//*** GetCelDelay ***

float ImageFormat_TGA::GetCelDelay(int celIndex)
	{
	return 0;
	}


//*** CopyPixels ***

void ImageFormat_TGA::CopyPixels(unsigned int* destination)
	{
	tga_image* image=static_cast<tga_image*>(image_);
	bool upsidedown=!tga_is_top_to_bottom(image);
	int width=image->width;
	int height=image->height;
	unsigned char* data=image->image_data;
	
	if (image->pixel_depth==24)
		{
		for (int y=0; y<height; y++)
			{
			for (int x=0; x<width; x++)
				{
				unsigned int c=0xff000000;
				c|=data[2]<<16;
				c|=data[1]<<8;
				c|=data[0];
				int yp=y;
				if (upsidedown)
					{
					yp=height-y-1;
					}
				destination[x+yp*width]=c;
				data+=3;
				}
			}
		return;
		}

	if (image->pixel_depth==32)
		{
		for (int y=0; y<height; y++)
			{
			for (int x=0; x<width; x++)
				{
				unsigned int c=0x00000000;
				c|=data[3]<<24;
				c|=data[2]<<16;
				c|=data[1]<<8;
				c|=data[0];
				int yp=y;
				if (upsidedown)
					{
					yp=height-y-1;
					}
				destination[x+yp*width]=c;
				data+=4;
				}
			}
		return;
		}

	if (image->pixel_depth==16)
		{
		for (int y=0; y<height; y++)
			{
			for (int x=0; x<width; x++)
				{
				unsigned int s=data[1]<<8 | data[0];
				unsigned int c=0xff000000;
				unsigned int b=((s&0x1f));
				unsigned int g=(((s>>5)&0x1f));
				unsigned int r=(((s>>10)&0x1f));
				float fr=((float)r)/31.0f;
				float fg=((float)g)/31.0f;
				float fb=((float)b)/31.0f;

				r=(unsigned int)(255.0f*fr);
				g=(unsigned int)(255.0f*fg);
				b=(unsigned int)(255.0f*fb);
				c|=r<<16;
				c|=g<<8;
				c|=b;
				int yp=y;
				if (upsidedown)
					{
					yp=height-y-1;
					}
				destination[x+yp*width]=c;
				data+=2;
				}
			}
		return;
		}


	Assert(false,"Unsupported TGA file");
	}


//*** Save ***

void ImageFormat_TGA::Save(const Filename& filename, int width, int height, void* data)
	{
	tga_image tga;
	MemSet(&tga,0,sizeof(tga));
    tga.image_id_length=0;
    tga.color_map_type=0;
    tga.image_type=TGA_IMAGE_TYPE_BGR;
    tga.color_map_origin=0;
    tga.color_map_length=0;
    tga.color_map_depth=0;
    tga.origin_x=0;
    tga.origin_y=0;
    tga.width=(unsigned short)width;
    tga.height=(unsigned short)height;
    tga.pixel_depth=32;
    tga.image_descriptor=TGA_T_TO_B_BIT;
    tga.image_id=0;
    tga.color_map_data=0;
    tga.image_data=static_cast<unsigned char*>(data);

	tga_write(filename.GetString(),&tga);
	}
