/**
 * \class	RenderCall
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __RenderCall_H__
#define __RenderCall_H__

// Includes
#include "Sprite.h"

// Forward declares


// RenderCall
class RenderCall:public Sprite
	{
	public:
		RenderCall();

		RenderCall(SpriteManager* spriteManager);
		
		virtual ~RenderCall();

		typedef void (*CallbackFunction)(Bitmap& bitmap, RenderCall* sprite, void* userData);

		void SetCallback(CallbackFunction callbackFunction, void* userData);

	private:
		virtual void Render(Bitmap& bitmap);


	private:
		CallbackFunction callbackFunction_;
		void* userData_;
		
	};

#endif /* __RenderCall_H__ */

