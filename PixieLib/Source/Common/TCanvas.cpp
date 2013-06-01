//*** TCanvas.cpp ***

#include "TCanvas.h"
#include "TDraw.h"
#include "StandardLibrary.h"


//*** Constructor ***

TCanvas::TCanvas(TFont font, int width, int height):
	font_(font),
	width_(width),
	height_(height),
	cursorX_(0),
	cursorY_(0),
	backgroundAlpha_(255),
	offsetX_(0),
	offsetY_(0)
	{
	width_ = Max(1,width_);
	height_ = Max(1,height_);
	cells_ = new unsigned short[width_*height_];
	Clear();
	}


//*** Destructor ***

TCanvas::~TCanvas()
	{
	delete[] cells_;
	}


//*** Clear ***

void TCanvas::Clear()
	{
	TCell cell = { 0, TLightGrey, TBlack };
	unsigned short value = ToUShort(cell);
	for (int i=0; i<width_*height_; i++)
		{
		cells_[i] = value;
		}
	}


//*** GetFont **

TFont TCanvas::GetFont() const
	{
	return font_;
	}


//*** GetWidth ***

int TCanvas::GetWidth() const
	{
	return width_;
	}


//*** GetHeight ***

int TCanvas::GetHeight() const
	{
	return height_;
	}


//*** GetData ***

unsigned short* TCanvas::GetData()
	{
	return cells_;
	}


//*** GetData ***

const unsigned short* TCanvas::GetData() const
	{
	return cells_;
	}


//*** SetCursor ***

void TCanvas::SetCursor(int x, int y)
	{
	cursorX_=x%width_;
	cursorY_=y%height_;
	cursorX_=cursorX_<0?cursorX_+width_:cursorX_;
	cursorY_=cursorY_<0?cursorY_+height_:cursorY_;
	}


//*** SetCursorX ***

void TCanvas::SetCursorX(int x)
	{
	cursorX_=x%width_;
	cursorX_=cursorX_<0?cursorX_+width_:cursorX_;
	}


//*** SetCursorY ***

void TCanvas::SetCursorY(int y)
	{
	cursorY_=y%height_;
	cursorY_=cursorY_<0?cursorY_+height_:cursorY_;
	}


//*** GetCursorX ***

int TCanvas::GetCursorX() const
	{
	return cursorX_;
	}


//*** GetCursorY ***

int TCanvas::GetCursorY() const
	{
	return cursorY_;
	}


//*** MoveCursor ***

void TCanvas::MoveCursor(int dx, int dy)
	{
	cursorX_=(cursorX_+dx)%width_;
	cursorY_=(cursorY_+dy)%height_;
	cursorX_=cursorX_<0?cursorX_+width_:cursorX_;
	cursorY_=cursorY_<0?cursorY_+height_:cursorY_;
	}


////*** GetCell ***
//
//TCell TCanvas::GetCell(int x, int y) const
//	{
//	if (x<0 || x>=width_ || y<0 || y>=height_)
//		{
//		static TCell defaultCell = { ' ', TLightGrey, TBlack, };
//		return defaultCell;
//		}
//
//	return ToCell(cells_[x+y*width_]);
//	}


//*** GetCell ***

TCell TCanvas::GetCell() const
	{
	int x = GetCursorX();
	int y = GetCursorY();

	if (x<0 || x>=width_ || y<0 || y>=height_)
		{
		static TCell defaultCell = { ' ', TLightGrey, TBlack, };
		return defaultCell;
		}

	return ToCell(cells_[x+y*width_]);
	}


//*** SetCell ***

void TCanvas::SetCell(int x, int y, TCell cell)
	{
	if (x<0 || x>=width_ || y<0 || y>=height_)
		{
		return;
		}

	TCell targetCell = ToCell(cells_[x+y*width_]);
	if (cell.foreground!=TUndefined)
		{
		targetCell.foreground = cell.foreground;
		}
	if (cell.background!=TUndefined)
		{
		targetCell.background = cell.background;
		}
	if (cell.ascii!=0)
		{
		targetCell.ascii = cell.ascii;
		}
	cells_[x+y*width_] = ToUShort(targetCell);
	}


//*** SetCell ***

void TCanvas::SetCell(TCell cell)
	{
	int x = GetCursorX();
	int y = GetCursorY();
	if (x<0 || x>=width_ || y<0 || y>=height_)
		{
		return;
		}

	TCell targetCell = ToCell(cells_[x+y*width_]);
	if (cell.foreground!=TUndefined)
		{
		targetCell.foreground = cell.foreground;
		}
	if (cell.background!=TUndefined)
		{
		targetCell.background = cell.background;
		}
	if (cell.ascii!=0)
		{
		targetCell.ascii = cell.ascii;
		}
	cells_[x+y*width_] = ToUShort(targetCell);
	}


//*** GetBackgroundAlpha ***

unsigned char TCanvas::GetBackgroundAlpha() const
	{
	return backgroundAlpha_;
	}


//*** SetBackgroundAlpha ***

void TCanvas::SetBackgroundAlpha(unsigned char alpha)
	{
	backgroundAlpha_ = alpha;
	}


//*** GetExtents ***

TRect TCanvas::GetExtents() const
	{
	return TRect(0,0,width_,height_);
	}


//*** ToCell ***

TCell TCanvas::ToCell(unsigned short value) const
	{
	TCell cell;
	cell.ascii = (unsigned char)(value>>8);
	cell.foreground = (TColor)((value & 0xff) >> 4);
	cell.background = (TColor)((value & 0x0f));
	return cell;
	}


//*** ToUShort ***

unsigned short TCanvas::ToUShort(TCell value) const
	{
	return (unsigned short)((value.ascii << 8) | ((value.foreground & 0xf)<<4) | (value.background & 0xf));
	}
