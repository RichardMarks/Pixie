//*** Resource_Font.cpp ***

#include "Resource_Font.h"
#include "OldResourceManager.h"
#include "Filename.h"
#include "Font.h"


//*** Constructor ***

Resource_Font::Resource_Font():
	font_(0)
	{
	}


//*** Constructor ***

Resource_Font::Resource_Font(const Filename& filename)
	{
	font_=siOldResourceManager->GetFont(filename.GetStringId());
	siOldResourceManager->IncreaseReferenceCount(font_);
	}


//*** Constructor ***

Resource_Font::Resource_Font(const char* filenameStr)
	{
	Filename filename(filenameStr);
	font_=siOldResourceManager->GetFont(filename.GetStringId());
	siOldResourceManager->IncreaseReferenceCount(font_);
	}
		

//*** Constructor ***

Resource_Font::Resource_Font(StringId filenameId)
	{
	Filename filename(filenameId);
	font_=siOldResourceManager->GetFont(filename.GetStringId());
	siOldResourceManager->IncreaseReferenceCount(font_);
	}
		

//*** Destructor ***

Resource_Font::~Resource_Font() 
	{ 
	if (font_)
		{
		siOldResourceManager->DecreaseReferenceCount(font_);
		}
	}


///*** Copy Constructor ***

Resource_Font::Resource_Font(const Resource_Font& resource):
	font_(resource.font_) 
	{ 
	siOldResourceManager->IncreaseReferenceCount(font_);
	}


//*** Assignment operator ***

const Resource_Font& Resource_Font::operator =(const Resource_Font& resource)
	{ 
	font_=resource.font_;
	siOldResourceManager->IncreaseReferenceCount(font_);
	return *this;
	}


//*** GetFont ***

const Font& Resource_Font::GetFont() const
	{
	if (font_)
		{
		return *font_;
		}

	static Font defaultValue;
	return defaultValue;
	}


//*** operator const Font& ***

Resource_Font::operator const Font&() const
	{
	return GetFont();
	}


void Resource_Font::Blit(Bitmap& bitmap, int x, int y, const char* text, int spacing, unsigned short modulate, unsigned char alpha) const
	{
	if (!font_)
		{
		return;
		}

	font_->Blit(bitmap,x,y,text,spacing,modulate,alpha);
	}

void Resource_Font::BlitWrap(Bitmap& bitmap, int x, int y, const char* text, int width, int hspacing, int vspacing, unsigned short modulate, unsigned char alpha) const
	{
	if (!font_)
		{
		return;
		}

	font_->BlitWrap(bitmap,x,y,text,width,hspacing,vspacing,modulate,alpha);
	}