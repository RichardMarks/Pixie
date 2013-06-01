/**
 * \class	TCanvas
 * 
 * \ingroup	
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __TCanvas_H__
#define __TCanvas_H__

// Includes
#include "TColor.h"
#include "TRect.h"

// External classes

// TFont
enum TFont
	{
	TFont8x8,
	TFont9x16,
	};


// TCell
struct TCell
	{
	unsigned char ascii;
	TColor foreground;
	TColor background;
	};

// TCanvas
class TCanvas
	{
	public:
		TCanvas(TFont font, int width, int height);

		~TCanvas();

		void Clear();

		TFont GetFont() const;

		int GetWidth() const;
		int GetHeight() const;
		unsigned short* GetData();
		const unsigned short* GetData() const;

		void SetCursor(int x, int y);
		void SetCursorX(int x);
		void SetCursorY(int y);
		int GetCursorX() const;
		int GetCursorY() const;
		void MoveCursor(int dx, int dy);

		TCell GetCell(int x, int y) const;
		TCell GetCell() const;
		void SetCell(int x, int y, TCell cell);
		void SetCell(TCell cell);

		unsigned char GetBackgroundAlpha() const;
		void SetBackgroundAlpha(unsigned char alpha);

		TRect GetExtents() const;
	
		TCell ToCell(unsigned short value) const;
		unsigned short ToUShort(TCell value) const;
	private:
		int cursorX_;
		int cursorY_;
		int width_;
		int height_;
		unsigned char backgroundAlpha_;
		unsigned short* cells_;
		TFont font_;
		mutable int offsetX_;
		mutable int offsetY_;
	};



#endif /* __TCanvas_H__ */
