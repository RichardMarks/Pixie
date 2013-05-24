//*** Rectangle.cpp **

#include "Rectangle.h"


//*** Constructor ***

Rectangle::Rectangle():
	width_(10),
	height_(10)
	{
	}


//*** Constructor ***

Rectangle::Rectangle(SpriteManager* spriteManager):
	Sprite(spriteManager),
	width_(10),
	height_(10)
	{
	}


//*** Destructor ***

Rectangle::~Rectangle()
	{
	}



//*** SetSize ***

void Rectangle::SetSize(float width, float height)
	{
	width_=width;
	height_=height;
	}


//*** SetWidth ***

void Rectangle::SetWidth(float width)
	{
	width_=width;
	}


//*** SetHeight ***

void Rectangle::SetHeight(float height)
	{
	height_=height;
	}


//*** GetWidth ***

float Rectangle::GetWidth() const
	{
	return width_;
	}


//*** GetHeight ***

float Rectangle::GetHeight() const
	{
	return height_;
	}


//*** Render ***

void Rectangle::Render(Bitmap& bitmap)
	{
	if (IsVisible() && width_>0 && height_>0)
		{
		int x1=(int)(GetX()-GetOriginX());
		int y1=(int)(GetY()-GetOriginY());
		int x2=x1+(int)GetWidth()-1;
		int y2=y1+(int)GetHeight()-1;

		if (x1==0 && y1==0 && x2==bitmap.GetWidth()-1 && y2==bitmap.GetHeight()-1 && GetColor()==0 && GetAlpha()==255)
			{
			bitmap.Clear();
			}
		else
			{
			bitmap.Fill(x1,y1,x2,y2,GetColor(),GetAlpha());
			}
		}

	}
