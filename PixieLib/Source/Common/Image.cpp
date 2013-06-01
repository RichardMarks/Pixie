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
	celCount_(0)
	{
	}


//*** Constructor ***

Image::Image(const Asset& asset):
	width_(0),
	height_(0),
	data_(0),
	celCount_(1)
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

Image::Image(const Asset& asset, int celCount):
	width_(0),
	height_(0),
	data_(0),
	celCount_(celCount)
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
	celCount_(1)

	{
	data_=new unsigned int[width_*height_];
	MemSet(data_,0,width_*height_*sizeof(unsigned int));
	}


//*** Constructor ***

Image::Image(int width, int height, int celCount):
	width_(width),
	height_(height),
	data_(0),
	celCount_(celCount)
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
	celCount_(image.celCount_)
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
	celCount_(1)
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
		delete[] data_;
		}
	}


//*** Copy operator ***

const Image& Image::operator=(const Image& image)
	{
	if (data_)
		{
		delete[] data_;
		}

	width_=image.width_;
	height_=image.height_;
	data_=0;
	celCount_=image.celCount_;

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

//*** GetWidth ***

int Image::GetWidth() const
	{
	return width_;	
	}


//*** GetHeight ***

int Image::GetHeight() const
	{
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

	if (!data_)
		{
		return 0;
		}

	return data_[x+y*width_];
	}


//*** SetPixel ***

void Image::SetPixel(int x, int y, unsigned int color)
	{
	Assert(x>=0 && x<GetWidth() && y>=0 && y<GetHeight(),"Coordinates out of range");
	if (!(x>=0 && x<GetWidth() && y>=0 && y<GetHeight()))
		{
		return;
		}

	if (data_)
		{
		data_[x+y*width_]=color;
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

	if (!data_)
		{
		return 0;
		}

	return data_[width_*height_*cel+x+y*width_];
	}


//*** SetPixel ***
void Image::SetPixel(int cel, int x, int y, unsigned int color)
	{
	Assert(x>=0 && x<GetWidth() && y>=0 && y<GetHeight(),"Coordinates out of range");
	if (!(x>=0 && x<GetWidth() && y>=0 && y<GetHeight()))
		{
		return;
		}

	if (data_)
		{
		data_[width_*height_*cel+x+y*width_]=color;
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


//*** GetPointer ***

unsigned int* Image::GetPointer() const
	{
	return data_;
	}


//*** Brightness ****

void Image::Brightness(float value)
	{
	for (int cel=0; cel<celCount_; cel++)
		{
		for (int y=0; y<height_; y++)
			{
			for (int x=0; x<width_; x++)
				{
				unsigned int c = data_[width_*height_*cel+x+y*width_];
				float a = RGB32TOFLOATA( c );
				float r = RGB32TOFLOATR( c );
				float g = RGB32TOFLOATG( c );
				float b = RGB32TOFLOATB( c );
				r += value;
				g += value;
				b += value;
				c = FLOATTORGBA32( r, g, b, a );
				data_[width_*height_*cel+x+y*width_] = c;
				}
			}
		}
	}


//*** Contrast ****

void Image::Contrast(float value)
	{
	for (int cel=0; cel<celCount_; cel++)
		{
		for (int y=0; y<height_; y++)
			{
			for (int x=0; x<width_; x++)
				{
				unsigned int c = data_[width_*height_*cel+x+y*width_];
				float a = RGB32TOFLOATA( c );
				float r = RGB32TOFLOATR( c );
				float g = RGB32TOFLOATG( c );
				float b = RGB32TOFLOATB( c );
				r -= 0.5f;
				g -= 0.5f;
				b -= 0.5f;
				r *= value;
				g *= value;
				b *= value;
				r += 0.5f;
				g += 0.5f;
				b += 0.5f;
				c = FLOATTORGBA32( r, g, b, a );
				data_[width_*height_*cel+x+y*width_] = c;
				}
			}
		}
	}


//*** BilinearSample ***

void Image::BilinearSample( int c, float x, float y, float& r, float& g, float& b, float& a ) const
	{
	int x1 = Clamp( (int)x, 0, GetWidth() - 1 );
	int y1 = Clamp( (int)y, 0, GetHeight() - 1 );
	int x2 = Clamp( x1 + 1, 0, GetWidth() - 1 );
	int y2 = Clamp( y1 + 1, 0, GetHeight() - 1 );
	float dx = (x - x1);
	float dy = (y - y1);

	unsigned cx1 = GetPixel( c, x1, y1 );
	unsigned cx2 = GetPixel( c, x2, y1 );
	unsigned cy1 = GetPixel( c, x1, y2 );
	unsigned cy2 = GetPixel( c, x2, y2 );

	float rx1 = RGB32TOFLOATR( cx1 );
	float gx1 = RGB32TOFLOATG( cx1 );
	float bx1 = RGB32TOFLOATB( cx1 );
	float ax1 = RGB32TOFLOATA( cx1 );

	float rx2 = RGB32TOFLOATR( cx2 );
	float gx2 = RGB32TOFLOATG( cx2 );
	float bx2 = RGB32TOFLOATB( cx2 );
	float ax2 = RGB32TOFLOATA( cx2 );

	if( ax1 < FLOAT_EPSILON )
		{
		rx1 = rx2;
		gx1 = gx2;
		bx1 = bx2;
		ax1 = ax2;
		}

	if( ax2 < FLOAT_EPSILON )
		{
		rx2 = rx1;
		gx2 = gx1;
		bx2 = bx1;
		ax2 = ax1;
		}

	float ry1 = RGB32TOFLOATR( cy1 );
	float gy1 = RGB32TOFLOATG( cy1 );
	float by1 = RGB32TOFLOATB( cy1 );
	float ay1 = RGB32TOFLOATA( cy1 );

	float ry2 = RGB32TOFLOATR( cy2 );
	float gy2 = RGB32TOFLOATG( cy2 );
	float by2 = RGB32TOFLOATB( cy2 );
	float ay2 = RGB32TOFLOATA( cy2 );

	if( ay1 < FLOAT_EPSILON )
		{
		ry1 = ry2;
		gy1 = gy2;
		by1 = by2;
		ay1 = ay2;
		}

	if( ay2 < FLOAT_EPSILON )
		{
		ry2 = ry1;
		gy2 = gy1;
		by2 = by1;
		ay2 = ay1;
		}

	float rx = ( rx1 * ( 1.0f - dx ) ) + ( rx2 * dx );
	float gx = ( gx1 * ( 1.0f - dx ) ) + ( gx2 * dx );
	float bx = ( bx1 * ( 1.0f - dx ) ) + ( bx2 * dx );
	float ax = ( ax1 * ( 1.0f - dx ) ) + ( ax2 * dx );
	
	float ry = ( ry1 * ( 1.0f - dx ) ) + ( ry2 * dx );
	float gy = ( gy1 * ( 1.0f - dx ) ) + ( gy2 * dx );
	float by = ( by1 * ( 1.0f - dx ) ) + ( by2 * dx );
	float ay = ( ay1 * ( 1.0f - dx ) ) + ( ay2 * dx );

	if( ax < FLOAT_EPSILON )
		{
		rx = ry;
		gx = gy;
		bx = by;
		ax = ay;
		}

	if( ay < FLOAT_EPSILON )
		{
		ry = rx;
		gy = gx;
		by = bx;
		ay = ax;
		}

	r = ( rx * ( 1.0f - dy ) ) + ( ry * dy );
	g = ( gx * ( 1.0f - dy ) ) + ( gy * dy );
	b = ( bx * ( 1.0f - dy ) ) + ( by * dy );
	a = ( ax * ( 1.0f - dy ) ) + ( ay * dy );
	}

//*** Sharpen ***

void Image::Sharpen( float radius )
	{
	const int filterWidth = 3;
	const int filterHeight = 3;

	int filter[ filterWidth ] [ filterHeight ] = 
		{
		{ -1, -1, -1, },
		{ -1,  9, -1, },
		{ -1, -1, -1, },
		};

	unsigned int* result = new unsigned int[width_*height_*celCount_];
    
	for( int c = 0; c < celCount_; c++ )
		{
		for( int x = 0; x < width_; x++ )
			{
			for( int y = 0; y < height_; y++ )
				{
				float alpha = RGB32TOFLOATA( data_[width_*height_*c+x+y*width_] );
				if( alpha > 0)
					{
					float or = RGB32TOFLOATR( data_[width_*height_*c+x+y*width_] );
					float og = RGB32TOFLOATG( data_[width_*height_*c+x+y*width_] );
					float ob = RGB32TOFLOATB( data_[width_*height_*c+x+y*width_] );

					float red = 0;
					float green = 0;
					float blue = 0;

					for( int fx = -1; fx <= 1; fx++ )
						{
						for( int fy = -1; fy <= 1; fy++ )
							{
							float ix = x + fx * radius;
							float iy = y + fy * radius;

							float ir = 0;
							float ig = 0;
							float ib = 0;
							float ia = 0;
							BilinearSample( c, ix, iy, ir, ig, ib, ia );
							if( ia < FLOAT_EPSILON )
								{
								ir = or;
								ig = og;
								ib = ob;
								ia = alpha;
								}
							red   += ir * filter[ 1 + fx ] [ 1 + fy ];
							green += ig * filter[ 1 + fx ] [ 1 + fy ];
							blue  += ib * filter[ 1 + fx ] [ 1 + fy ];
							}
						}

					result[width_*height_*c+x+y*width_] = FLOATTORGBA32( red, green, blue, alpha );
					}
				else
					{
					result[width_*height_*c+x+y*width_] = data_[width_*height_*c+x+y*width_];
					}
				}
			}
		}

	delete[] data_;
	data_ = result;
	}


//*** RGBToHSV ***

void RGBToHSV(float r, float g, float b, float& h, float& s, float& v)
	{
	float RedUnit = r;
	float GreenUnit = g;
	float BlueUnit = b;
	float MaxUnit = Max(Max(RedUnit,GreenUnit),BlueUnit);
	float MinUnit = Min(Min(RedUnit,GreenUnit),BlueUnit);

	float th = 0;
	float ts = 0;
	float tv = 0;

	if(MaxUnit == MinUnit)
		th = 0;
	else
		{
		if(RedUnit == MaxUnit)
			{
			th = (GreenUnit-BlueUnit)/(MaxUnit-MinUnit) * 60.0f;
			}
		else if(GreenUnit == MaxUnit)
			{
			th = (2 + (BlueUnit-RedUnit)/(MaxUnit-MinUnit)) * 60.0f;
			}
		else
			{
			th = (4 + (RedUnit-GreenUnit)/(MaxUnit-MinUnit)) * 60.0f;
			}
		}
	while(th < 0)
		th += 360;
	while(th > 360)
		th -= 360;

	ts = (MaxUnit - MinUnit) / MaxUnit;
	ts = Min(ts,1.0f);
	ts = Max(ts,0.0f);
	tv = MaxUnit;

	//Normalize
	h = th / 360.0f;
	s = ts;
	v = tv;
	}


//*** HSVToRGB ***

void HSVToRGB( float h, float s, float v, float& r, float& g, float& b )
	{
	int i;

	float hue = h * 6.0f;
	float sat = s;
	float bri = v;

	i = (int)Floor(hue);

	float f = (hue) - i;
	if(!(i & 1)) //If i is even (i.e. last bit of 'i' isn't set
		f = 1 - f;

	float m = bri * (1 - sat);
	float n = bri * (1 - sat * f);

	switch (i)
		{
		case 6:
		case 0: 
			r = bri;
			g = n;
			b = m;
			break;
		case 1: 
			r = n;
			g = bri;
			b = m;
			break;
		case 2: 
			r = m;
			g = bri;
			b = n;
			break;
		case 3: 
			r = m;
			g = n;
			b = bri;
			break;
		case 4: 
			r = n;
			g = m;
			b = bri;
			break;
		case 5: 
			r = bri;
			g = m;
			b = n;
		}
	}


//*** Saturation ****

void Image::Saturation(float value)
	{
	for (int cel=0; cel<celCount_; cel++)
		{
		for (int y=0; y<height_; y++)
			{
			for (int x=0; x<width_; x++)
				{
				unsigned int c = data_[width_*height_*cel+x+y*width_];
				float a = RGB32TOFLOATA( c );
				float r = RGB32TOFLOATR( c );
				float g = RGB32TOFLOATG( c );
				float b = RGB32TOFLOATB( c );
				float h;
				float s;
				float v;
				RGBToHSV( r, g, b, h, s, v );
				s += value;
				HSVToRGB( h, s, v, r, g, b );
				c = FLOATTORGBA32( r, g, b, a );
				data_[width_*height_*cel+x+y*width_] = c;
				}
			}
		}
	}


//*** Hue ****

void Image::Hue(float value)
	{
	for (int cel=0; cel<celCount_; cel++)
		{
		for (int y=0; y<height_; y++)
			{
			for (int x=0; x<width_; x++)
				{
				unsigned int c = data_[width_*height_*cel+x+y*width_];
				float a = RGB32TOFLOATA( c );
				float r = RGB32TOFLOATR( c );
				float g = RGB32TOFLOATG( c );
				float b = RGB32TOFLOATB( c );
				float h;
				float s;
				float v;
				RGBToHSV( r, g, b, h, s, v );
				h += value;
				h = Mod( h, 1.0f );
				h = h < 0 ? 1.0f + h : h;
				HSVToRGB( h, s, v, r, g, b );
				c = FLOATTORGBA32( r, g, b, a );
				data_[width_*height_*cel+x+y*width_] = c;
				}
			}
		}
	}


//*** Bleed ***

void Image::Bleed( int iterations )
	{
	Image newImage( width_, height_ );
	for( int i = 0; i < iterations; i++ )
		{
		for (int cel=0; cel<celCount_; cel++)
			{
			for (int y=0; y<height_; y++)
				{
				for (int x=0; x<width_; x++)
					{
					unsigned int c = GetPixel( cel, x, y );
					if( (c & 0xff000000) == 0)
						{
						c = GetPixel( cel, Clamp( x - 1, 0, width_ -1 ), y );
						}
					if( (c & 0xff000000) == 0)
						{
						c = GetPixel( cel, Clamp( x + 1, 0, width_ -1 ), y );
						}
					if( (c & 0xff000000) == 0)
						{
						c = GetPixel( cel, x, Clamp( y - 1, 0, height_ -1 ) );
						}
					if( (c & 0xff000000) == 0)
						{
						c = GetPixel( cel, x, Clamp( y + 1, 0, height_ -1 ) );
						}
					if( (c & 0xff000000) == 0)
						{
						c = GetPixel( cel, x, y );
						}
					newImage.SetPixel( cel, x, y, c );
					}
				}
			for (int y=0; y<height_; y++)
				{
				for (int x=0; x<width_; x++)
					{
					SetPixel( cel, x, y, newImage.GetPixel( x, y) );
					}
				}
			}
		}
	}
