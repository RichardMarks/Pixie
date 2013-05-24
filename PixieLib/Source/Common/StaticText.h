/**
 * \class	StaticText
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __StaticText_H__
#define __StaticText_H__

// Includes
#include "StringId.h"
#include "Sprite.h"
#include "Array.h"
#include "Resource_Font.h"

// Forward declares
class SpriteManager;

// StaticText
class StaticText:public Sprite
	{
	public:
		StaticText();
		StaticText(SpriteManager* spriteManager);
		virtual ~StaticText();

		virtual const Resource_Font& GetFont() const;
		virtual void SetFont(const Resource_Font& font);

		virtual const char* GetText() const;
		virtual void SetText(const char* text);

		virtual int GetSpacing() const;
		virtual void SetSpacing(int spacing);

		virtual int GetWidth() const;
		virtual int GetVSpacing() const;
		virtual void SetWidth(int width, int vspacing);

	protected:
		virtual void Render(Bitmap& bitmap);

	private: 
		Resource_Font font_;
		char* text_;
		int spacing_;
		int width_;
		int vspacing_;
	};


#endif /* __StaticText_H__ */

