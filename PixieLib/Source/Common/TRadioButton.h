/**
 * \class	TRadioButton
 * 
 * \ingroup	
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __TRadioButton_H__
#define __TRadioButton_H__

// Includes
#include "TStaticText.h"

// External classes


// TRadioButton
class TRadioButton:public TStaticText
	{
	public:
		TRadioButton();
		virtual void Draw(TCanvas& canvas) const;

		virtual bool EvaluateFocus(int x, int y) const;
	
		bool IsSelected() const;
		void SetSelected(bool selected);

	private:
		bool selected_;
	};

#endif /* __TRadioButton_H__ */
