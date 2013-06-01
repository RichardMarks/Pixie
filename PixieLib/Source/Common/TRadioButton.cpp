//*** TRadioButton.cpp **
 
#include "TRadioButton.h"
#include "TDraw.h"
#include "TCanvas.h"
#include "StandardLibrary.h"


//*** Constructor ***

TRadioButton::TRadioButton():
	selected_(false)
	{

	}


//*** Draw ***

void TRadioButton::Draw(TCanvas& canvas) const
	{
	TRect bounds = GetBounds();
	canvas.SetCursor(bounds.x_,bounds.y_);
	TColor foreground = GetForeground();
	if (HasFocus())
		{
		foreground = GetFocusColor();
		}
	TDraw::RadioButton(canvas,GetText(),IsSelected(),foreground,GetBackground(),GetHighlight());
	}


//*** EvaluateFocus ***

bool TRadioButton::EvaluateFocus(int x, int y) const
	{
	TRect bounds = GetBounds();
	if (bounds.Contains(TPoint(x,y)))
		{
		return true;
		}

	return false;
	}


//*** IsSelected ***

bool TRadioButton::IsSelected() const
	{
	return selected_;
	}


//*** SetSelected ***

void TRadioButton::SetSelected(bool selected)
	{
	selected_ = selected;
	}
