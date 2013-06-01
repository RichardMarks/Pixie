//*** TButton.cpp **
 
#include "TButton.h"
#include "TDraw.h"
#include "TCanvas.h"
#include "StandardLibrary.h"


//*** Draw ***

void TButton::Draw(TCanvas& canvas) const
	{
	TRect bounds = GetBounds();
	canvas.SetCursor(bounds.x_,bounds.y_);
	TColor foreground = GetForeground();
	if (HasFocus())
		{
		foreground = GetFocusColor();
		}
	TDraw::Button(canvas,bounds.w_,GetText(),foreground,GetBackground(),GetHighlight());
	}


//*** EvaluateFocus ***

bool TButton::EvaluateFocus(int x, int y) const
	{
	TRect bounds = GetBounds();
	if (bounds.Contains(TPoint(x,y)))
		{
		return true;
		}

	return false;
	}
