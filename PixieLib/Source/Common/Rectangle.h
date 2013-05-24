/**
 * \class	Rectangle
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __Rectangle_H__
#define __Rectangle_H__

// Includes
#include "Sprite.h"

// Forward declares


// Rectangle
class Rectangle:public Sprite
	{
	public:
		Rectangle();

		Rectangle(SpriteManager* spriteManager);
		
		virtual ~Rectangle();

		virtual void SetSize(float width, float height);
		virtual void SetWidth(float width);
		virtual void SetHeight(float height);
		virtual float GetWidth() const;
		virtual float GetHeight() const;

	private:
		virtual void Render(Bitmap& bitmap);


	private:
		float width_;
		float height_;
		
	};

#endif /* __Rectangle_H__ */

