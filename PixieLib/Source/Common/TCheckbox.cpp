//*** TCheckbox.cpp **
 
#include "TCheckbox.h"
#include "TDraw.h"
#include "TCanvas.h"
#include "StandardLibrary.h"

//*** Constructor ***

TCheckbox::TCheckbox():
	checked_(false)
	{

	}


//*** Draw ***

void TCheckbox::Draw(TCanvas& canvas) const
	{
	TRect bounds = GetBounds();
	canvas.SetCursor(bounds.x_,bounds.y_);
	TColor foreground = GetForeground();
	if (HasFocus())
		{
		foreground = GetFocusColor();
		}
	TDraw::Checkbox(canvas,GetText(),IsChecked(),foreground,GetBackground(),GetHighlight());
	}


//*** EvaluateFocus ***

bool TCheckbox::EvaluateFocus(int x, int y) const
	{
	TRect bounds = GetBounds();
	if (bounds.Contains(TPoint(x,y)))
		{
		return true;
		}

	return false;
	}


//*** IsChecked ***

bool TCheckbox::IsChecked() const
	{
	return checked_;
	}


//*** SetChecked ***

void TCheckbox::SetChecked(bool checked)
	{
	checked_ = checked;
	}
