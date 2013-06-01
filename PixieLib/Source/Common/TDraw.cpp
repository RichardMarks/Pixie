//*** TDraw.cpp ***

#include "TDraw.h"
#include "TCanvas.h"
#include "StandardLibrary.h"

namespace TDraw
	{

	// FrameElements
	enum FrameElements
		{
		TopLeft,
		Horizontal,
		TopRight,
		Vertical,
		BottomLeft,
		BottomRight,
		};

	};


//*** Fill ***

void TDraw::Fill(TCanvas& canvas, unsigned char ascii, TColor foreground, TColor background)
	{
	TCell cell = { ascii, foreground, background };
	for (int yi=0; yi<canvas.GetHeight(); yi++)
		{
		for (int xi=0; xi<canvas.GetWidth(); xi++)
			{
			canvas.SetCell(xi,yi,cell);
			}
		}
	}


//*** Rect ***

void TDraw::Rect(TCanvas& canvas, int width, int height, unsigned char ascii, TColor foreground, TColor background)
	{
	int x = canvas.GetCursorX();
	int y = canvas.GetCursorY();
	TCell cell = { ascii, foreground, background };
	for (int yi=0; yi<height; yi++)
		{
		for (int xi=0; xi<width; xi++)
			{
			canvas.SetCell(x+xi,y+yi,cell);
			}
		}
	}


//** Text ***

void TDraw::Text(TCanvas& canvas, const char* text, TColor foreground, TColor background, TColor highlight)
	{
	if (!text)
		{
		return;
		}
	int x = canvas.GetCursorX();
	int y = canvas.GetCursorY();
	TCell cell = { 0, foreground, background };
	bool isHighlighting = false;
	while(*text)
		{
		if (*text=='~')
			{
			isHighlighting=!isHighlighting;
			if (isHighlighting)
				{
				cell.foreground = highlight;
				}
			else
				{
				cell.foreground = foreground;
				}
			}
		else
			{
			cell.ascii = *text;
			canvas.SetCell(x,y,cell);
			x++;
			}
		text++;
		}
	}


//*** Shadow ***

void TDraw::Shadow(TCanvas& canvas, int width, int height, unsigned char ascii, TColor foreground, TColor background)
	{
	int x = canvas.GetCursorX();
	int y = canvas.GetCursorY();
	canvas.SetCursor(x+2,y+height);
	HLine(canvas,width-2,ascii,foreground,background);
	canvas.SetCursor(x+width,y+1);
	Rect(canvas,2,height,ascii,foreground,background);
	}


//*** Frame ***

void TDraw::Frame(TCanvas& canvas, int width, int height, const char* ascii, TColor foreground, TColor background)
	{
	int originalX = canvas.GetCursorX();
	int originalY = canvas.GetCursorY();
	int x = originalX;
	int y = canvas.GetCursorY();
	TCell cell = { 0, foreground, background };

	// Top row
	cell.ascii = ascii[TDraw::TopLeft];
	canvas.SetCell(x,y,cell);
	x++;
	canvas.SetCursorX(x);
	HLine(canvas,width-2,ascii[TDraw::Horizontal],foreground,background);
	x+=width-2;
	canvas.SetCursorX(x);
	cell.ascii = ascii[TDraw::TopRight]; 
	canvas.SetCell(x,y,cell);
	x=originalX;
	y++;
	canvas.SetCursor(x,y);

	// Left side
	VLine(canvas,height-2,ascii[TDraw::Vertical],foreground,background);	

	// Internals
	canvas.SetCursor(originalX+1,originalY+1);
	Rect(canvas,width-2,height-2,' ',foreground,background);

	// Right side
	x=originalX;
	y++;
	canvas.SetCursor(originalX+width-1,originalY+1);
	VLine(canvas,height-2,ascii[TDraw::Vertical],foreground,background);	

	// Bottom row
	x=originalX;
	y=originalY+height-1;
	canvas.SetCursor(x,y);
	cell.ascii = ascii[TDraw::BottomLeft];
	canvas.SetCell(x,y,cell);
	x++;
	canvas.SetCursorX(x);
	HLine(canvas,width-2,ascii[TDraw::Horizontal],foreground,background);
	x+=width-2;
	canvas.SetCursorX(x);
	cell.ascii = ascii[TDraw::BottomRight]; 
	canvas.SetCell(x,y,cell);
	}


//*** DoubleFrame ***

void TDraw::DoubleFrame(TCanvas& canvas, int width, int height, TColor foreground, TColor background)
	{
	Frame(canvas,width,height,"\xc9\xcd\xbb\xba\xc8\xbc",foreground,background);
	}


//*** HLine ***

void TDraw::HLine(TCanvas& canvas, int width, unsigned char ascii, TColor foreground, TColor background)
	{
	int x = canvas.GetCursorX();
	int y = canvas.GetCursorY();
	TCell cell = { ascii, foreground, background };
	for (int xi=0; xi<width; xi++)
		{
		canvas.SetCell(x+xi,y,cell);
		}
	}


//*** VLine ***

void TDraw::VLine(TCanvas& canvas, int height, unsigned char ascii, TColor foreground, TColor background)
	{
	int x = canvas.GetCursorX();
	int y = canvas.GetCursorY();
	TCell cell = { ascii, foreground, background };
	for (int yi=0; yi<height; yi++)
		{
		canvas.SetCell(x,y+yi,cell);
		}
	}


//*** Button ***

void TDraw::Button(TCanvas& canvas, int width, const char* text, TColor foreground, TColor background, TColor highlight, TColor shadow)
	{
	int x = canvas.GetCursorX();
	int y = canvas.GetCursorY();
	HLine(canvas,width,' ',foreground,background);
	canvas.SetCursor(x+(width-StrLen(text))/2,y);
	TDraw::Text(canvas,text,foreground,background,highlight);
	canvas.SetCursor(x+1,y+1);
	HLine(canvas,width,0xDF,shadow);
	TCell cell = { 0xDC, shadow, TUndefined };
	canvas.SetCell(x+width,y,cell);
	}


//*** Checkbox ***

void TDraw::Checkbox(TCanvas& canvas, const char* text, bool enabled, TColor foreground, TColor background, TColor highlight)
	{
	int x = canvas.GetCursorX();
	int y = canvas.GetCursorY();
	if (enabled)
		{
		Text(canvas,"[X] ",foreground,background,highlight);
		}
	else
		{
		Text(canvas,"[ ] ",foreground,background,highlight);
		}
	canvas.SetCursor(x+4,y);
	Text(canvas,text,foreground,background,highlight);
	}


//*** RadioButton ***

void TDraw::RadioButton(TCanvas& canvas, const char* text, bool enabled, TColor foreground, TColor background, TColor highlight)
	{
	int x = canvas.GetCursorX();
	int y = canvas.GetCursorY();
	if (enabled)
		{
		Text(canvas,"(\x07) ",foreground,background,highlight);
		}
	else
		{
		Text(canvas,"( ) ",foreground,background,highlight);
		}
	canvas.SetCursor(x+4,y);
	Text(canvas,text,foreground,background,highlight);
	}
