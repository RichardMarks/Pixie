//*** Image.cpp ***

#include "Image.h"
#include "Debug.h"
#include "StandardLibrary.h"
#include "ImageFormat.h"
#include "Bitmap.h"
#include "ColorHelper.h"


//*** Constructor ***

Image::Image():
	width_(0),
	height_(0),
	data_(0),
	celCount_(0),
	orientation_(Rotate_0)
	{
	}


//*** Constructor ***

Image::Image(const Asset& asset, Orientation orientation):
	width_(0),
	height_(0),
	data_(0),
	celCount_(1),
	orientation_(orientation)
	{
	ImageFormat* format=ImageFormat::CreateImageFormat(asset);

	Assert(format,"Unknown image format");

	if (format)
		{
		width_=format->GetWidth();
		height_=format->GetHeight();
		celCount_=format->GetCelCount();
		data_=new unsigned int[width_*height_*celCount_];
		format->CopyPixels(data_);
		for (int i=0; i<format->GetCelCount(); i++)
			{
			float delay=format->GetCelDelay(i);
			celDelay_.Add(delay);
			}
		delete format;
		}
	}


//*** Constructor ***

Image::Image(const Asset& asset, int celCount, Orientation orientation):
	width_(0),
	height_(0),
	data_(0),
	celCount_(celCount),
	orientation_(orientation)
	{
	ImageFormat* format=ImageFormat::CreateImageFormat(asset);

	Assert(format,"Unknown image format");

	if (format)
		{
		if (celCount_==1) 
			{
			celCount_=format->GetCelCount();
			}
		
		width_=format->GetWidth();
		height_=format->GetHeight()/celCount_;
		data_=new unsigned int[width_*height_*celCount_];
		format->CopyPixels(data_);
		delete format;
		}
	}


//*** Constructor ***

Image::Image(int width, int height):
	width_(width),
	height_(height),
	data_(0),
	celCount_(1),
	orientation_(Rotate_0)

	{
	data_=new unsigned int[width_*height_];
	MemSet(data_,0,width_*height_*sizeof(unsigned int));
	}


//*** Constructor ***

Image::Image(int width, int height, int celCount):
	width_(width),
	height_(height),
	data_(0),
	celCount_(celCount),
	orientation_(Rotate_0)
	{
	Assert(celCount>0, "Invalid cel count");
	if (celCount==0)
		{
		return;
		}
	data_=new unsigned int[width_*height_*celCount_];
	MemSet(data_,0,width_*height_*celCount_*sizeof(unsigned int));
	}


//*** Constructor ***

Image::Image(const Image& image):
	width_(image.width_),
	height_(image.height_),
	data_(0),
	celCount_(image.celCount_),
	orientation_(image.orientation_)
	{
	if (width_*height_*celCount_>0)
		{
		data_=new unsigned int[width_*height_*celCount_];
		MemCpy(data_,image.data_,width_*height_*celCount_*sizeof(unsigned int));
		for (int i=0; i<image.celDelay_.GetItemCount(); i++)
			{
			celDelay_.Add(image.celDelay_.Get(i));
			}
		}
	}


//*** Constructor ***

Image::Image(const Bitmap& bitmap):
	width_(bitmap.GetWidth()),
	height_(bitmap.GetHeight()),
	data_(0),
	celCount_(1),
	orientation_(Rotate_0)
	{
	if (width_*height_*celCount_>0)
		{
		data_=new unsigned int[width_*height_*celCount_];
		for (int y=0; y<height_; y++)
			{
			for (int x=0; x<width_; x++)
				{
				data_[x+y*width_]=RGB16TO32(bitmap.GetPixelColor(x,y),bitmap.GetPixelAlpha(x,y));
				}
			}
		}
	}


//*** Destructor ***

Image::~Image()
	{
	if (data_)
		{
		delete data_;
		}
	}


//*** Copy operator ***

const Image& Image::operator=(const Image& image)
	{
	if (data_)
		{
		delete data_;
		}

	width_=image.width_;
	height_=image.height_;
	data_=0;
	celCount_=image.celCount_;
	orientation_=image.orientation_;

	if (width_*height_*celCount_>0)
		{
		data_=new unsigned int[width_*height_*celCount_];
		MemCpy(data_,image.data_,width_*height_*celCount_*sizeof(unsigned int));
		for (int i=0; i<image.celDelay_.GetItemCount(); i++)
			{
			celDelay_.Add(image.celDelay_.Get(i));
			}
		}

	return *this;
	}

//*** SetOrientation ***

void Image::SetOrientation(Orientation orientation)
	{
	orientation_=orientation;
	}

//*** GetWidth ***

int Image::GetWidth() const
	{
	switch (orientation_)
		{
		case Rotate_0:
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

int Image::GetHeight() const
	{
	switch (orientation_)
		{
		case Rotate_0:
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




//*** GetPixel ***

unsigned int Image::GetPixel(int x, int y) const
	{
	Assert(x>=0 && x<GetWidth() && y>=0 && y<GetHeight(),"Coordinates out of range");
	if (!(x>=0 && x<GetWidth() && y>=0 && y<GetHeight()))
		{
		return 0;
		}

	int xp=x;
	int yp=y;

	switch (orientation_)
		{
		case Rotate_0:
			{
			} break;

		case Rotate_90:
			{
			xp=y;
			yp=(height_-1)-x;
			} break;

		case Rotate_180:
			{
			xp=(width_-1)-x;
			yp=(height_-1)-y;
			} break;

		case Rotate_270:
			{
			xp=(width_-1)-y;
			yp=x;
			} break;

		case Mirror_X:
			{
			xp=(width_-1)-x;
			} break;

		case Mirror_Y:
			{
			yp=(height_-1)-y;
			} break;

		}



	if (!data_)
		{
		return 0;
		}

	return data_[xp+yp*width_];
	}


//*** SetPixel ***

void Image::SetPixel(int x, int y, unsigned int color)
	{
	Assert(x>=0 && x<GetWidth() && y>=0 && y<GetHeight(),"Coordinates out of range");
	if (!(x>=0 && x<GetWidth() && y>=0 && y<GetHeight()))
		{
		return;
		}

	int xp=x;
	int yp=y;

	switch (orientation_)
		{
		case Rotate_0:
			{
			} break;

		case Rotate_90:
			{
			xp=y;
			yp=(height_-1)-x;
			} break;

		case Rotate_180:
			{
			xp=(width_-1)-x;
			yp=(height_-1)-y;
			} break;

		case Rotate_270:
			{
			xp=(width_-1)-y;
			yp=x;
			} break;

		case Mirror_X:
			{
			xp=(width_-1)-x;
			} break;

		case Mirror_Y:
			{
			yp=(height_-1)-y;
			} break;

		}

	if (data_)
		{
		data_[xp+yp*width_]=color;
		}
	}


//*** GetPixel ***

unsigned int Image::GetPixel(int cel, int x, int y) const
	{
	Assert(x>=0 && x<GetWidth() && y>=0 && y<GetHeight(),"Coordinates out of range");
	if (!(x>=0 && x<GetWidth() && y>=0 && y<GetHeight()))
		{
		return 0;
		}

	int xp=x;
	int yp=y;

	switch (orientation_)
		{
		case Rotate_0:
			{
			} break;

		case Rotate_90:
			{
			xp=y;
			yp=(height_-1)-x;
			} break;

		case Rotate_180:
			{
			xp=(width_-1)-x;
			yp=(height_-1)-y;
			} break;

		case Rotate_270:
			{
			xp=(width_-1)-y;
			yp=x;
			} break;

		case Mirror_X:
			{
			xp=(width_-1)-x;
			} break;

		case Mirror_Y:
			{
			yp=(height_-1)-y;
			} break;

		}

	if (!data_)
		{
		return 0;
		}

	return data_[width_*height_*cel+xp+yp*width_];
	}


//*** SetPixel ***
void Image::SetPixel(int cel, int x, int y, unsigned int color)
	{
	Assert(x>=0 && x<GetWidth() && y>=0 && y<GetHeight(),"Coordinates out of range");
	if (!(x>=0 && x<GetWidth() && y>=0 && y<GetHeight()))
		{
		return;
		}

	int xp=x;
	int yp=y;

	switch (orientation_)
		{
		case Rotate_0:
			{
			} break;

		case Rotate_90:
			{
			xp=y;
			yp=(height_-1)-x;
			} break;

		case Rotate_180:
			{
			xp=(width_-1)-x;
			yp=(height_-1)-y;
			} break;

		case Rotate_270:
			{
			xp=(width_-1)-y;
			yp=x;
			} break;

		case Mirror_X:
			{
			xp=(width_-1)-x;
			} break;

		case Mirror_Y:
			{
			yp=(height_-1)-y;
			} break;

		}
	if (data_)
		{
		data_[width_*height_*cel+xp+yp*width_]=color;
		}
	}


//*** GetCelCount ****

int Image::GetCelCount() const
	{
	return celCount_;
	}


//*** GetCelDelay ****

float Image::GetCelDelay(int cel) const
	{
	Assert(cel>=0 && cel<celCount_ ,"Cel index out of range");
	if (cel<0 || cel>=celCount_)
		{
		return 0.0f;
		}

	return celDelay_.Get(cel);
	}


//*** AdjustBrightness  ****

void Image::AdjustBrightness(float redMultiplier, float greenMultiplier, float blueMultiplier)
	{
	for (int cel=0; cel<celCount_; cel++)
		{
		for (int y=0; y<height_; y++)
			{
			for (int x=0; x<width_; x++)
				{
				unsigned int c=GetPixel(cel,x,y);
				if (c&0xff000000)
					{
					unsigned int r=(c&0x00ff0000)>>16;
					unsigned int g=(c&0x0000ff00)>>8;
					unsigned int b=(c&0x000000ff);
					r=Clamp((int)(r*redMultiplier),0,0xff);
					g=Clamp((int)(g*greenMultiplier),0,0xff);
					b=Clamp((int)(b*blueMultiplier),0,0xff);
					c&=0xff000000;
					c|=(r<<16) | (g<<8) | b;
					SetPixel(cel,x,y,c);
					}
				}
			}
		}
	}


//*** GetPointer ***

unsigned int* Image::GetPointer() const
	{
	return data_;
	}
