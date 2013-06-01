/**
 * \class	TStaticText
 * 
 * \ingroup	
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	Use string pool to manage text
 */

#ifndef __TStaticText_H__
#define __TStaticText_H__

// Includes
#include "TView.h"

// External classes


// TStaticText
class TStaticText:public TView
	{
	public:
		TStaticText();

		virtual ~TStaticText();

		virtual void Draw(TCanvas& canvas) const;
	
		void SetText(const char* text);
		const char* GetText() const;
	
	private:
		const char* text_;
	};

#endif /* __TStaticText_H__ */
