/**
 * \class	SpriteAction_AlphaPulse
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __SpriteAction_AlphaPulse_H__
#define __SpriteAction_AlphaPulse_H__

// Includes
#include "SpriteAction.h"

// Forward declares
class Sprite;

// SpriteAction_AlphaPulse
class SpriteAction_AlphaPulse:public SpriteAction
	{
	public:
		SpriteAction_AlphaPulse(float speed);

	private:
		virtual void Setup(SpriteController* controller, Sprite* sprite);
		virtual void Update(SpriteController* controller, Sprite* sprite, float deltaTime);

	private:
		unsigned char targetAlpha_;
		float speed_;
		float currentSource_;
		float currentTarget_;
	};		

#endif /* __SpriteAction_AlphaPulse_H__ */

