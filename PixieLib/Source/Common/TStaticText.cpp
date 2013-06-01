//*** TStaticText.cpp **
 
#include "TStaticText.h"
#include "TDraw.h"
#include "TCanvas.h"
#include "StandardLibrary.h"


//*** Constructor ***

TStaticText::TStaticText():
	text_(0)
	{

	}


//*** Destructor ***

TStaticText::~TStaticText()
	{
	if (text_)
		{
		Free((void*)text_);
		}
	}


//*** Draw ***

void TStaticText::Draw(TCanvas& canvas) const
	{
	TRect bounds = GetBounds();
	canvas.SetCursor(bounds.x_,bounds.y_);
	TDraw::Text(canvas,GetText(),GetForeground(),GetBackground(),GetHighlight());
	}


//*** SetText ***

void TStaticText::SetText(const char* text)
	{
	if (text_)
		{
		Free((void*)text_);
		text_ = 0;
		}

	if (text)
		{
		text_ = StrDup(text);
		TRect rect = GetBounds();
		rect.w_ = StrLen(text);
		rect.h_ = 1;
		SetBounds(rect);
		}
	}


//*** GetText ***

const char* TStaticText::GetText() const
	{
	return text_;
	}

