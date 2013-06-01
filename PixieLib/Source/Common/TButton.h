/**
 * \class	TButton
 * 
 * \ingroup	
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __TButton_H__
#define __TButton_H__

// Includes
#include "TStaticText.h"

// External classes


// TButton
class TButton:public TStaticText
	{
	public:
		virtual void Draw(TCanvas& canvas) const;

		virtual bool EvaluateFocus(int x, int y) const;
	
	private:
	};

#endif /* __TButton_H__ */
