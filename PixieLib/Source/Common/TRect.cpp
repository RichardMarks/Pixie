//*** TRect.cpp ***

#include "TRect.h"
#include "StandardLibrary.h"

//*** Default Constructor ***

TRect::TRect():
	x_(0),
	y_(0),
	w_(0),
	h_(0)
	{

	}


//*** Constructor ***

TRect::TRect( int x, int y, int w, int h ):
	x_(x),
	y_(y),
	w_(w),
	h_(h)
	{

	}


//*** Constructor ***

TRect::TRect( TPoint p1, TPoint p2 )
	{
	x_ = Min( p1.x_, p2.x_ );
	y_ = Min( p1.y_, p2.y_ );
	w_ = Abs( p2.x_ - p1.x_ );
	h_ = Abs( p2.y_ - p1.y_ );
	}


//*** Move ***

void TRect::Move( int dx, int dy )
	{
	x_ += dx;
	y_ += dy;
	}


//*** Grow ***

void TRect::Grow( int dx, int dy )
	{
    x_ -= dx;
    y_ -= dy;
    w_ += 2*dx;
    h_ += 2*dy;
	}


//*** Intersect ***

TRect TRect::Intersect( TRect rect ) const
	{
	return TRect( 
		TPoint( Max( x_, rect.x_ ), Max( y_, rect.y_ ) ),
		TPoint( Min( x_+w_, rect.x_+rect.w_ ), Min( y_+h_, rect.y_+rect.h_ ) )
		);
	}


//*** Union ***

TRect TRect::Union( TRect rect ) const
	{
	if (IsEmpty())
		{
		return rect;
		}
	
	if (rect.IsEmpty())
		{
		return *this;
		}

	return TRect( 
		TPoint( Min( x_, rect.x_ ), Min( y_, rect.y_ ) ),
		TPoint( Max( x_+w_, rect.x_+rect.w_ ), Max( y_+h_, rect.y_+rect.h_ ) )
		);
	}


//*** Contains ***

bool TRect::Contains( TPoint point ) const
	{
	return ( point.x_ >= x_ && point.x_ < x_+w_ && point.y_ >= y_ && point.y_ < y_+h_ );
	}


//*** Equality operator ***

bool TRect::operator == ( TRect rect ) const
	{
	return x_==rect.x_ && y_==rect.y_ && w_==rect.w_ && h_==rect.h_;
	}


//*** Inequality operator ***

bool TRect::operator != ( TRect rect ) const
	{
	return !(*this == rect);
	}


//*** IsEmpty ***

bool TRect::IsEmpty() const
	{
	return w_<=0 || h_<=0;
	}
