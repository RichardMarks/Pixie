/**
 * \class	SpriteAction_SmoothArrival
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __SpriteAction_SmoothArrival_H__
#define __SpriteAction_SmoothArrival_H__

// Includes
#include "SpriteAction.h"

// Forward declares
class Sprite;

// SpriteAction_SmoothArrival
class SpriteAction_SmoothArrival:public SpriteAction
	{
	public:
		SpriteAction_SmoothArrival(float targetX, float targetY, float speed);

	private:
		virtual void Setup(SpriteController* controller, Sprite* sprite);
		virtual void Update(SpriteController* controller, Sprite* sprite, float deltaTime);

	private:
		float targetX_;
		float targetY_;
		float speed_;

		float velocityX_;
		float velocityY_;
	};

#endif /* __SpriteAction_SmoothArrival_H__ */

