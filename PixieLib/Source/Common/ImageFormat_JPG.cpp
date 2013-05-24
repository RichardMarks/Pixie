//*** ImageFormat_JPG.cpp ***

#include "ImageFormat_JPG.h"
#include "Debug.h"
#include "Asset.h"
#include "StandardLibrary.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <stdio.h>
#include "jpeglib/jpeglib.h"

//*** Register ***

void ImageFormat_JPG::Register()
	{
	ImageFormat::RegisterImageFormat(TestAsset,Create);
	}


//*** Create ***

ImageFormat* ImageFormat_JPG::Create(const Asset& asset)
	{
	return new ImageFormat_JPG(asset);
	}


//*** TestAsset ***

bool ImageFormat_JPG::TestAsset(const Asset& asset)
	{
	// Check if the header matches JPG (JFIF) files
	if (asset.Open())
		{
		unsigned char jpgheader[]={0xff,0xd8,0xff,0xe0};
		unsigned char buffer[4];
		asset.Read(buffer,4);
		asset.Close();
		if (MemCmp(buffer,jpgheader,4)==0)
			{
			return true;
			}
		}

	return false;
	}




//*** Constructor ***

ImageFormat_JPG::ImageFormat_JPG(const Asset& asset):
	width_(0),
	height_(0),
	image_(0)
	{
	int size=asset.GetSize();
	char* data=new char[size];
	asset.Open();
	asset.Read(data,size);
    asset.Close();

  /* This struct contains the JPEG decompression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   */
  struct jpeg_decompress_struct cinfo;

  /* More stuff */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */


  /* Step 1: allocate and initialize JPEG decompression object */

  /* We set up the normal JPEG error routines, then override error_exit. */
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);

  /* Now we can initialize the JPEG decompression object. */
  jpeg_create_decompress(&cinfo);

  /* Step 2: specify data source (eg, a file) */

  jpeg_memory_src(&cinfo, reinterpret_cast<JOCTET*>(data), size);

  /* Step 3: read file parameters with jpeg_read_header() */

  (void) jpeg_read_header(&cinfo, TRUE);
  /* We can ignore the return value from jpeg_read_header since
   *   (a) suspension is not possible with the stdio data source, and
   *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
   * See libjpeg.txt for more info.
   */

  /* Step 4: set parameters for decompression */

  /* In this example, we don't need to change any of the defaults set by
   * jpeg_read_header(), so we do nothing here.
   */

  /* Step 5: Start decompressor */

  (void) jpeg_start_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  width_= cinfo.output_width;
  height_=cinfo.output_height;
  image_=new unsigned int[width_*height_];

  /* We may need to do some setup of our own at this point before reading
   * the data.  After jpeg_start_decompress() we have the correct scaled
   * output image dimensions available, as well as the output colormap
   * if we asked for color quantization.
   * In this example, we need to make an output work buffer of the right size.
   */ 
  /* JSAMPLEs per row in output buffer */
  row_stride = cinfo.output_width * cinfo.output_components;
  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  /* Step 6: while (scan lines remain to be read) */
  /*           jpeg_read_scanlines(...); */

  /* Here we use the library's state variable cinfo.output_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   */
  int y=0;
  while (cinfo.output_scanline < cinfo.output_height) {
    /* jpeg_read_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could ask for
     * more than one scanline at a time if that's more convenient.
     */
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    /* Assume put_scanline_someplace wants a pointer and sample count. */
    //put_scanline_someplace(buffer[0], row_stride);
	for (int x=0; x<row_stride/3; x++)
		{
		unsigned char r=(unsigned char)(*buffer)[x*3+0];
		unsigned char g=(unsigned char)(*buffer)[x*3+1];
		unsigned char b=(unsigned char)(*buffer)[x*3+2];
		image_[x+y*width_]=0xff000000 | r<<16 | g<<8 | b;
		}
	y++;
  }

  /* Step 7: Finish decompression */

  (void) jpeg_finish_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* Step 8: Release JPEG decompression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&cinfo);

	delete[] data;
   }


//*** Destructor ***

ImageFormat_JPG::~ImageFormat_JPG()
	{
	if (image_)
		{
		delete[] image_;
		}
	}


//*** GetWidth ***

int ImageFormat_JPG::GetWidth()
	{
	return width_;
	}


//*** GetHeight ***

int ImageFormat_JPG::GetHeight()
	{
	return height_;
	}


//*** GetCelCount ***

int ImageFormat_JPG::GetCelCount()
	{
	return 1;
	}


//*** GetCelDelay ***

float ImageFormat_JPG::GetCelDelay(int celIndex)
	{
	return 0;
	}


//*** CopyPixels ***

void ImageFormat_JPG::CopyPixels(unsigned int* destination)
	{
	if (!image_)
		{
		return;
		}

	MemCpy(destination,image_,width_*height_*sizeof(unsigned int));
	}


//*** Save ***

void ImageFormat_JPG::Save(const Filename& filename, int width, int height, void* data)
	{
	}
