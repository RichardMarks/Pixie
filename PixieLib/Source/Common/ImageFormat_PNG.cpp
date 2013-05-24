//*** ImageFormat_PNG.cpp ***

#include "ImageFormat_PNG.h"
#include "Debug.h"
#include "Asset.h"
#include "StandardLibrary.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include "libpng/png.h"

//*** Register ***

void ImageFormat_PNG::Register()
	{
	ImageFormat::RegisterImageFormat(TestAsset,Create);
	}


//*** Create ***

ImageFormat* ImageFormat_PNG::Create(const Asset& asset)
	{
	return new ImageFormat_PNG(asset);
	}


//*** TestAsset ***

bool ImageFormat_PNG::TestAsset(const Asset& asset)
	{
	// Check if the header matches PNG files
	if (asset.Open())
		{
		unsigned char pngheader[]={137,80,78,71,13,10,26,10};
		unsigned char buffer[8];
		asset.Read(buffer,8);
		asset.Close();
		if (MemCmp(buffer,pngheader,8)==0)
			{
			return true;
			}
		}

	return false;
	}


//*** png_user_read_fn ***

void png_user_read_fn(png_structp assetptr, png_bytep buffer, png_size_t size)
	{
	Asset* asset=static_cast<Asset*>(assetptr->io_ptr);
	asset->Read(buffer,size);
	}


//*** Constructor ***

ImageFormat_PNG::ImageFormat_PNG(const Asset& asset):
	png_ptr_(0),
	info_ptr_(0)
	{
	if (asset.Open())
		{

		png_ptr_ = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		Assert(png_ptr_,"Couldn't create png read struct");
		if (!png_ptr_)
			{
			return;
			}

		info_ptr_ = png_create_info_struct((png_structp)png_ptr_);
		Assert(info_ptr_,"Couldn't create png info struct");
		if (!info_ptr_)
			{
			png_destroy_read_struct((png_structpp)&png_ptr_, 0,0);
			return;
			}

		png_set_read_fn(static_cast<png_structp>(png_ptr_), const_cast<Asset*>(&asset), png_user_read_fn);

		png_read_png(static_cast<png_structp>(png_ptr_), static_cast<png_infop>(info_ptr_), /*PNG_TRANSFORM_STRIP_16|*/PNG_TRANSFORM_EXPAND|PNG_TRANSFORM_BGR, 0);
		
		asset.Close();
 		}

   }


//*** Destructor ***

ImageFormat_PNG::~ImageFormat_PNG()
	{
    png_destroy_read_struct(reinterpret_cast<png_structpp>(&png_ptr_), reinterpret_cast<png_infopp>(&info_ptr_),0);
	}


//*** GetWidth ***

int ImageFormat_PNG::GetWidth()
	{
	if (info_ptr_)
		{
		return static_cast<png_infop>(info_ptr_)->width;
		}

	return 0;
	}


//*** GetHeight ***

int ImageFormat_PNG::GetHeight()
	{
	if (info_ptr_)
		{
		return static_cast<png_infop>(info_ptr_)->height;
		}

	return 0;
	}


//*** GetCelCount ***

int ImageFormat_PNG::GetCelCount()
	{
	return 1;
	}


//*** GetCelDelay ***

float ImageFormat_PNG::GetCelDelay(int celIndex)
	{
	return 0;
	}


//*** CopyPixels ***

void ImageFormat_PNG::CopyPixels(unsigned int* destination)
	{
	if (!info_ptr_ || !png_ptr_)
		{
		return;
		}

	png_infop info_ptr=static_cast<png_infop>(info_ptr_);
	png_structp png_ptr=static_cast<png_structp>(png_ptr_);

	Assert(info_ptr->color_type==PNG_COLOR_TYPE_RGB_ALPHA || info_ptr->color_type==PNG_COLOR_TYPE_RGB,"Unknown PNG format");
	if (info_ptr->color_type!=PNG_COLOR_TYPE_RGB_ALPHA && info_ptr->color_type!=PNG_COLOR_TYPE_RGB)
		{
		return;
		}

	if (info_ptr->color_type==PNG_COLOR_TYPE_RGB_ALPHA)
		{
		unsigned char* data=reinterpret_cast<unsigned char*>(destination);
		png_byte** row_pointers=png_get_rows(png_ptr, info_ptr);
		int rowSize=info_ptr->width*4;

		for (unsigned int y=0; y<info_ptr->height; y++)
			{
			for (unsigned int x=0; x<info_ptr->width; x++)
				{
				data[x*4+y*rowSize+0]=row_pointers[y][x*4+0];
				data[x*4+y*rowSize+1]=row_pointers[y][x*4+1];
				data[x*4+y*rowSize+2]=row_pointers[y][x*4+2];
				data[x*4+y*rowSize+3]=row_pointers[y][x*4+3];
				}
			}
		}
	else
		{
		unsigned char* data=reinterpret_cast<unsigned char*>(destination);
		png_byte** row_pointers=png_get_rows(png_ptr, info_ptr);
		int rowSize=info_ptr->width*4;

		for (unsigned int y=0; y<info_ptr->height; y++)
			{
			for (unsigned int x=0; x<info_ptr->width; x++)
				{
				data[x*4+y*rowSize+0]=row_pointers[y][x*3+0];
				data[x*4+y*rowSize+1]=row_pointers[y][x*3+1];
				data[x*4+y*rowSize+2]=row_pointers[y][x*3+2];
				data[x*4+y*rowSize+3]=0xff;
				}
			}
		}
	}


//*** Save ***

void ImageFormat_PNG::Save(const Filename& filename, int width, int height, void* data)
	{
	}
