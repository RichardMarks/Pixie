/**
 * \class	DragAndDropSprite
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	Make a proper implementation of this
 */
 
#ifndef __DragAndDropSprite_H__
#define __DragAndDropSprite_H__

// Includes
#include "StringId.h"
#include "Button.h"
#include "Array.h"
#include "Resource_BitmapStrip.h"

// Forward declares
class SpriteManager;

// DragAndDropSprite
class DragAndDropSprite:public Button
	{
	public:
		DragAndDropSprite();

		DragAndDropSprite(SpriteManager* spriteManager);

		virtual bool OnMouseOver(int x, int y, bool button, StringId& eventId, void*& userData);

	private:
		bool dragging_;
		float dragOffsetX_;
		float dragOffsetY_;
	};


#endif /* __DragAndDropSprite_H__ */

