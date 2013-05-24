/**
 * \class	TextWindow
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __TextWindow_H__
#define __TextWindow_H__

// Includes
#include "StringId.h"
#include "Sprite.h"
#include "MarkupTextBlock.h"

// Forward declares
class SpriteManager;

// TextWindow
class TextWindow:public Sprite
	{
	public:
		TextWindow();

		TextWindow(SpriteManager* spriteManager);

		~TextWindow();

		virtual void SetText(const char* pmlString);
		virtual void AddText(const char* pmlString);
		virtual const char* GetText() const;

		virtual void SetSize(float width, float height);
		virtual void SetWidth(float width);
		virtual void SetHeight(float height);
		virtual float GetWidth() const;
		virtual float GetHeight() const;

		virtual void SetBackgroundColor(unsigned short color);
		virtual unsigned short GetBackgroundColor() const;

		virtual void SetBorderColor(unsigned short color);
		virtual unsigned short GetBorderColor() const;

		virtual void SetBorderSize(float size);
		virtual float GetBorderSize() const;

		virtual void SetPadding(float padding);
		virtual float GetPadding() const;

	protected:
		virtual void Render(Bitmap& bitmap);

	private: 
		float width_;
		float height_;
		MarkupTextBlock content_;
		Bitmap* windowBitmap_;
		StringId currentHighlightOption_;
		StringId currentHighlightLink_;
		bool redraw_;
		unsigned short backgroundColor_;
		unsigned short borderColor_;
		float borderSize_;
		float padding_;
	};


#endif /* __TextWindow_H__ */

