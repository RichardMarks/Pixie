//*** TextWindow.cpp **

#include "TextWindow.h"
#include "BitmapStrip.h"
#include "Bitmap_16bit.h"
#include "ColorHelper.h"

//*** Constructor ***

TextWindow::TextWindow():
	width_(100),
	height_(100),
	windowBitmap_(0),
	redraw_(true),
	currentHighlightOption_(0),
	currentHighlightLink_(0),
	backgroundColor_(0xffff),
	borderColor_(0),
	borderSize_(2),
	padding_(7)
	{
	}


//*** Constructor ***

TextWindow::TextWindow(SpriteManager* spriteManager):
	Sprite(spriteManager),
	width_(100),
	height_(100),
	windowBitmap_(0),
	redraw_(true),
	currentHighlightOption_(0),
	currentHighlightLink_(0),
	backgroundColor_(0xffff),
	borderColor_(0),
	borderSize_(2),
	padding_(7)
	{
	}


//*** Destructor ***

TextWindow::~TextWindow()
	{
	if (windowBitmap_)
		{
		delete windowBitmap_;
		windowBitmap_=0;
		}
	}


//*** SetText ***

void TextWindow::SetText(const char* pmlString)
	{
	content_.SetString(pmlString);
	redraw_=true;
	}


//*** AddText ***

void TextWindow::AddText(const char* pmlString)
	{
	content_.AddString(pmlString);
	redraw_=true;
	}


//*** GetText ***

const char* TextWindow::GetText() const
	{
	return content_.GetString();
	}


//*** SetSize ***

void TextWindow::SetSize(float width, float height)
	{
	if (width==width_ && height==height_)
		{
		return;
		}
	width_=width;
	height_=height;
	if (windowBitmap_)
		{
		delete windowBitmap_;
		windowBitmap_=0;
		}
	redraw_=true;
	}


//*** SetWidth ***

void TextWindow::SetWidth(float width)
	{
	if (width==width_)
		{
		return;
		}
	width_=width;
	if (windowBitmap_)
		{
		delete windowBitmap_;
		windowBitmap_=0;
		}
	redraw_=true;
	}


//*** SetHeight ***

void TextWindow::SetHeight(float height)
	{
	if (height==height_)
		{
		return;
		}
	height_=height;
	if (windowBitmap_)
		{
		delete windowBitmap_;
		windowBitmap_=0;
		}
	redraw_=true;
	}


//*** GetWidth ***

float TextWindow::GetWidth() const
	{
	return width_;
	}


//*** GetHeight ***

float TextWindow::GetHeight() const
	{
	return height_;
	}


//*** SetBackgroundColor ***

void TextWindow::SetBackgroundColor(unsigned short color)
	{
	backgroundColor_=color;
	redraw_=true;
	}



//*** GetBackgroundColor ***

unsigned short TextWindow::GetBackgroundColor() const
	{
	return backgroundColor_;
	}


//*** SetBorderColor ***

void TextWindow::SetBorderColor(unsigned short color)
	{
	borderColor_=color;
	}


//*** GetBorderColor ***

unsigned short TextWindow::GetBorderColor() const
	{
	return borderColor_;
	}


//*** SetBorderSize ***

void TextWindow::SetBorderSize(float size)
	{
	borderSize_=size;
	redraw_=true;
	}


//*** GetBorderSize ***

float TextWindow::GetBorderSize() const
	{
	return borderSize_;
	}


//*** SetPadding ***

void TextWindow::SetPadding(float size)
	{
	padding_=size;
	redraw_=true;
	}


//*** GetPadding ***

float TextWindow::GetPadding() const
	{
	return padding_;
	}


//*** Render ***

void TextWindow::Render(Bitmap& bitmap)
	{
	Sprite::Render(bitmap);

	if (!IsVisible())
		{
		return;
		}

	if (GetAlpha()==0)
		{
		return;
		}

	if (!windowBitmap_)
		{
		windowBitmap_=new Bitmap_16bit((int)width_,(int)height_);
		windowBitmap_->Fill(backgroundColor_);
		}

	if (redraw_)
		{
		int bs=(int)borderSize_;
		int pd=(int)padding_;
		int x1=bs+pd;
		int y1=bs+pd;
		int x2=windowBitmap_->GetWidth()-bs-pd;
		int y2=windowBitmap_->GetHeight()-bs-1-pd;
		windowBitmap_->Fill(backgroundColor_);
		content_.Render(*windowBitmap_,x1,y1,x2,y2,0,0);
		redraw_=false;
		}

	int x1=(int)GetX();
	int y1=(int)GetY();
	int x2=(int)(GetX()+width_)-1;
	int y2=(int)(GetY()+height_)-1;
	int bs=(int)borderSize_;

	if (windowBitmap_)
		{
		windowBitmap_->Blit(bs,bs,((int)width_)-1-bs,((int)height_)-1-bs,bitmap,x1+bs,y1+bs,GetColor(),GetAlpha());
		}
	else
		{
		bitmap.Fill(x1+bs,y1+bs,x2-bs,y2-bs,RGBModulate16(backgroundColor_,GetColor()),GetAlpha());
		}
	if (bs>0)
		{
		bitmap.Fill(x1,y1,x2,y1+bs-1,RGBModulate16(borderColor_,GetColor()),GetAlpha());	
		bitmap.Fill(x1,y2-(bs-1),x2,y2,RGBModulate16(borderColor_,GetColor()),GetAlpha());	
		bitmap.Fill(x1,y1,x1+bs-1,y2,RGBModulate16(borderColor_,GetColor()),GetAlpha());	
		bitmap.Fill(x2-(bs-1),y1,x2,y2,RGBModulate16(borderColor_,GetColor()),GetAlpha());	
		}
	}


