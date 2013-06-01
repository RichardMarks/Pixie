/**
 * \class	TCheckbox
 * 
 * \ingroup	
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __TCheckbox_H__
#define __TCheckbox_H__

// Includes
#include "TStaticText.h"

// External classes


// TCheckbox
class TCheckbox:public TStaticText
	{
	public:
		TCheckbox();

		virtual void Draw(TCanvas& canvas) const;

		virtual bool EvaluateFocus(int x, int y) const;

		bool IsChecked() const;
		void SetChecked(bool checked);

	private:
		bool checked_;
	};

#endif /* __TCheckbox_H__ */
