//*** TView.cpp ***

#include "TView.h"
#include "TCanvas.h"
#include "TDraw.h"


//** Constructor ***

TView::TView():
	background_(TUndefined),
	foreground_(TUndefined),
	highlight_(TUndefined),
	focusColor_(TUndefined),
	hasFocus_(false)
	{

	}


//*** Draw ***

void TView::Draw(TCanvas& canvas) const
	{
	TRect bounds = GetBounds();
	canvas.SetCursor(bounds.x_,bounds.y_);
	TDraw::Rect(canvas,bounds.w_,bounds.h_,' ',GetForeground(),GetBackground());
	}


//*** GetExtents ***

TRect TView::GetExtents() const
	{
	return TRect(0,0,size_.x_,size_.y_);
	}


//*** GetBounds ***

TRect TView::GetBounds() const
	{
	return TRect( position_, position_ + size_ );
	}


//*** SetBounds ***

void TView::SetBounds(TRect rect)
	{
	position_ = TPoint( rect.x_, rect.y_ );
	size_ = TPoint( rect.w_, rect.h_ );
	}


//*** SetPosition ***

void TView::SetPosition(int x, int y)
	{
	position_.x_ = x;
	position_.y_ = y;
	}


//*** SetSize ***

void TView::SetSize(int w, int h)
	{
	size_.x_ = w;
	size_.y_ = h;
	}


//*** SetBackground *** 

void TView::SetBackground(TColor color) 
	{ 
	background_ = color; 
	}


//*** GetBackground *** 

TColor TView::GetBackground() const 
	{ 
	return background_; 
	}


//*** SetForeground ***

void TView::SetForeground(TColor color) 
	{ 
	foreground_ = color; 
	}


//*** GetForeground ***

TColor TView::GetForeground() const 
	{ 
	return foreground_; 
	}


//*** SetHighlight ***

void TView::SetHighlight(TColor color) 
	{ 
	highlight_ = color; 
	}


//*** GetHighlight ***

TColor TView::GetHighlight() const 
	{ 
	return highlight_; 
	}


//*** SetFocusColor ***

void TView::SetFocusColor(TColor color) 
	{ 
	focusColor_ = color; 
	}


//*** GetFocusColor ***

TColor TView::GetFocusColor() const 
	{ 
	return focusColor_; 
	}


//*** EvaluateFocus ***

bool TView::EvaluateFocus(int x, int y) const
	{
	return false;
	}


//*** SetFocus ***

bool TView::SetFocus(int x, int y)
	{
	hasFocus_ = EvaluateFocus(x,y);

	// Code to call SetFocus on child views here

	return false;
	}


//*** HasFocus ***

bool TView::HasFocus() const
	{

	return hasFocus_;
	}
