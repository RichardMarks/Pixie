/**
 * \class	TView
 * 
 * \ingroup	
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __TView_H__
#define __TView_H__

// Includes
#include "TRect.h"
#include "TColor.h"

// External classes
class TCanvas;

// TView
class TView
	{
	public:
		TView();
		virtual ~TView() { };

		virtual void Draw(TCanvas& canvas) const;

		TRect GetExtents() const;
		TRect GetBounds() const;
	    void SetBounds( TRect bounds );

		void SetPosition(int x, int y);
		void SetSize(int w, int h);

		void SetBackground(TColor color);
		TColor GetBackground() const;

		void SetForeground(TColor color);
		TColor GetForeground() const;

		void SetHighlight(TColor color);
		TColor GetHighlight() const;

		void SetFocusColor(TColor color);
		TColor GetFocusColor() const;

		bool SetFocus(int x, int y);
		bool HasFocus() const;

	private:
		virtual bool EvaluateFocus(int x, int y) const;

	private:
		TPoint position_;
		TPoint size_;
		TColor background_;
		TColor foreground_;
		TColor highlight_;
		TColor focusColor_;
		bool hasFocus_;
	};


#endif /* __TView_H__ */
