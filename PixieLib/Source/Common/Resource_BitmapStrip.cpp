//*** Resource_BitmapStrip.cpp ***

#include "Resource_BitmapStrip.h"
#include "OldResourceManager.h"
#include "Filename.h"
#include "BitmapStrip.h"
#include "Bitmap_16bit.h"


//*** Constructor ***

Resource_BitmapStrip::Resource_BitmapStrip():
	bitmapStrip_(0)
	{
	}

//*** Constructor ***

Resource_BitmapStrip::Resource_BitmapStrip(const BitmapStrip* bitmapStrip):
	bitmapStrip_(bitmapStrip)
	{
	}


//*** Constructor ***

Resource_BitmapStrip::Resource_BitmapStrip(const Bitmap* bitmap):
	bitmapStrip_(0)
	{
	bitmapStrip_=siOldResourceManager->GetBitmapStrip(bitmap);
	if (bitmapStrip_)
		{
		siOldResourceManager->IncreaseReferenceCount(bitmapStrip_);
		}
	}


//*** Constructor ***

Resource_BitmapStrip::Resource_BitmapStrip(const Filename& filename, int celCount):
	bitmapStrip_(0)
	{
	bitmapStrip_=siOldResourceManager->GetBitmapStrip(filename.GetStringId(),celCount);
	if (bitmapStrip_)
		{
		siOldResourceManager->IncreaseReferenceCount(bitmapStrip_);
		}
	}


//*** Constructor ***

Resource_BitmapStrip::Resource_BitmapStrip(const char* filenameStr, int celCount):
	bitmapStrip_(0)
	{
	Filename filename(filenameStr);
	bitmapStrip_=siOldResourceManager->GetBitmapStrip(filename.GetStringId(),celCount);
	if (bitmapStrip_)
		{
		siOldResourceManager->IncreaseReferenceCount(bitmapStrip_);
		}
	}


//*** Constructor ***

Resource_BitmapStrip::Resource_BitmapStrip(StringId filenameId, int celCount):
	bitmapStrip_(0)
	{
	Filename filename(filenameId);
	bitmapStrip_=siOldResourceManager->GetBitmapStrip(filename.GetStringId(),celCount);
	if (bitmapStrip_)
		{
		siOldResourceManager->IncreaseReferenceCount(bitmapStrip_);
		}
	}


//*** Constructor ***

Resource_BitmapStrip::Resource_BitmapStrip(const Image& image):
	bitmapStrip_(0)
	{
	bitmapStrip_=siOldResourceManager->GetBitmapStrip(image);
	if (bitmapStrip_)
		{
		siOldResourceManager->IncreaseReferenceCount(bitmapStrip_);
		}
	}

		
//*** Destructor ***

Resource_BitmapStrip::~Resource_BitmapStrip() 
	{ 
	if (bitmapStrip_)
		{
		siOldResourceManager->DecreaseReferenceCount(bitmapStrip_);
		}
	}


///*** Copy Constructor ***

Resource_BitmapStrip::Resource_BitmapStrip(const Resource_BitmapStrip& resource):
	bitmapStrip_(resource.bitmapStrip_) 
	{ 
	if (bitmapStrip_)
		{
		siOldResourceManager->IncreaseReferenceCount(bitmapStrip_);
		}
	}


//*** Assignment operator ***

const Resource_BitmapStrip& Resource_BitmapStrip::operator =(const Resource_BitmapStrip& resource)
	{ 
	if (bitmapStrip_)
		{
		siOldResourceManager->DecreaseReferenceCount(bitmapStrip_);
		}
	bitmapStrip_=resource.bitmapStrip_;
	if (bitmapStrip_)
		{
		siOldResourceManager->IncreaseReferenceCount(bitmapStrip_);
		}
	return *this;
	}


//*** GetBitmapStrip ***

const BitmapStrip& Resource_BitmapStrip::GetBitmapStrip() const
	{
	if (!bitmapStrip_)
		{
		static BitmapStrip defaultValue;
		return defaultValue;
		}

	return *bitmapStrip_;
	}


//*** GetBitmap ***

const Bitmap& Resource_BitmapStrip::GetBitmap() const
	{
	if (!bitmapStrip_ || bitmapStrip_->GetCelCount()==0)
		{
		static Bitmap_16bit defaultValue;
		return defaultValue;
		}

	return bitmapStrip_->GetCel(0);
	}


//*** operator const BitmapStrip& ***

Resource_BitmapStrip::operator const BitmapStrip&() const
	{
	return GetBitmapStrip();
	}


//*** operator const Bitmap& ***

Resource_BitmapStrip::operator const Bitmap&() const
	{
	return GetBitmap();
	}

int Resource_BitmapStrip::GetCelCount() const
	{
	return GetBitmapStrip().GetCelCount();
	}


const Bitmap& Resource_BitmapStrip::GetCel(int index) const
	{
	return GetBitmapStrip().GetCel(index);
	}



int Resource_BitmapStrip::GetWidth(int cel) const
	{
	const BitmapStrip& strip=GetBitmapStrip();
	if (strip.GetCelCount()==0)
		{
		return 0 ;
		}

	return strip.GetCel(cel).GetWidth();
	}

int Resource_BitmapStrip::GetHeight(int cel) const
	{
	const BitmapStrip& strip=GetBitmapStrip();
	if (strip.GetCelCount()==0)
		{
		return 0;
		}

	return strip.GetCel(cel).GetHeight();
	}

void Resource_BitmapStrip::Blit(Bitmap& target, int x, int y, unsigned short modulate, unsigned char alpha, Bitmap::Transformation transformation) const
	{
	GetBitmap().Blit(target,x,y,modulate,alpha,transformation);
	}

void Resource_BitmapStrip::Blit(int x1, int y1,	int x2,	int y2,	Bitmap& target, int x,int y, unsigned short modulate, unsigned char alpha, Bitmap::Transformation transformation) const
	{
	GetBitmap().Blit(x1,y1,x2,y2,target,x,y,modulate,alpha,transformation);
	}

void Resource_BitmapStrip::Copy(Bitmap& target, int x, int y, unsigned short modulate, Bitmap::Transformation transformation) const
	{
	GetBitmap().Copy(target,x,y,modulate,transformation);
	}

void Resource_BitmapStrip::Copy(int x1, int y1, int x2, int y2,	Bitmap& target,	int x, int y, unsigned short modulate, Bitmap::Transformation transformation) const
	{
	GetBitmap().Copy(x1,y1,x2,y2,target,x,y,modulate,transformation);
	}


void Resource_BitmapStrip::Blit(int cel, Bitmap& target, int x, int y, unsigned short modulate, unsigned char alpha, Bitmap::Transformation transformation) const
	{
	const BitmapStrip& strip=GetBitmapStrip();
	if (strip.GetCelCount()==0)
		{
		return;
		}
	strip.GetCel(cel).Blit(target,x,y,modulate,alpha,transformation);
	}

void Resource_BitmapStrip::Blit(int cel, int x1, int y1,	int x2,	int y2,	Bitmap& target, int x,int y, unsigned short modulate, unsigned char alpha, Bitmap::Transformation transformation) const
	{
	const BitmapStrip& strip=GetBitmapStrip();
	if (strip.GetCelCount()==0)
		{
		return;
		}
	strip.GetCel(cel).Blit(x1,y1,x2,y2,target,x,y,modulate,alpha,transformation);
	}

void Resource_BitmapStrip::Copy(int cel, Bitmap& target, int x, int y, unsigned short modulate, Bitmap::Transformation transformation) const
	{
	const BitmapStrip& strip=GetBitmapStrip();
	if (strip.GetCelCount()==0)
		{
		return;
		}
	strip.GetCel(cel).Copy(target,x,y,modulate,transformation);
	}

void Resource_BitmapStrip::Copy(int cel, int x1, int y1, int x2, int y2, Bitmap& target, int x, int y, unsigned short modulate, Bitmap::Transformation transformation) const
	{
	const BitmapStrip& strip=GetBitmapStrip();
	if (strip.GetCelCount()==0)
		{
		return;
		}
	strip.GetCel(cel).Copy(x1,y1,x2,y2,target,x,y,modulate,transformation);
	}
