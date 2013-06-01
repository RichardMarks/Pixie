/**
 * \file	TDraw.h
 * 
 * \ingroup	
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __TDraw_H__
#define __TDraw_H__

// Includes
#include "TColor.h"

// External classes
class TCanvas;

// TDraw
namespace TDraw
	{
	void Fill(TCanvas& canvas, unsigned char ascii = ' ', TColor foreground = TUndefined, TColor background = TUndefined);
	void Rect(TCanvas& canvas, int width, int height, unsigned char ascii = ' ', TColor foreground = TUndefined, TColor background = TUndefined);
	void Text(TCanvas& canvas, const char* text, TColor foreground = TUndefined, TColor background = TUndefined, TColor highlight = TUndefined);
	void Shadow(TCanvas& canvas, int width, int height, unsigned char ascii, TColor foreground = TUndefined, TColor background = TUndefined);
	void Frame(TCanvas& canvas, int width, int height, const char* ascii = "\xda\xc4\xbf\xb3\xc0\xd9", TColor foreground = TUndefined, TColor background = TUndefined);
	void DoubleFrame(TCanvas& canvas, int width, int height, TColor foreground = TUndefined, TColor background = TUndefined);
	void HLine(TCanvas& canvas, int width, unsigned char ascii = ' ', TColor foreground = TUndefined, TColor background = TUndefined);
	void VLine(TCanvas& canvas, int height, unsigned char ascii = ' ', TColor foreground = TUndefined, TColor background = TUndefined);
	void Button(TCanvas& canvas, int width, const char* text, TColor foreground = TBlack, TColor background = TGreen, TColor highlight = TYellow, TColor shadow = TBlack);
	void Checkbox(TCanvas& canvas, const char* text, bool enabled, TColor foreground = TUndefined, TColor background = TUndefined, TColor highlight = TUndefined);
	void RadioButton(TCanvas& canvas, const char* text, bool enabled, TColor foreground = TUndefined, TColor background = TUndefined, TColor highlight = TUndefined);
	};



#endif /* __TDraw_H__ */
