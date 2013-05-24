/**
 * \class	SpriteAction_ButtonStateCrossFade
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __SpriteAction_ButtonStateCrossFade_H__
#define __SpriteAction_ButtonStateCrossFade_H__

// Includes
#include "SpriteAction.h"

// Forward declares
class Sprite;

// SpriteAction_ButtonStateCrossFade
class SpriteAction_ButtonStateCrossFade:public SpriteAction
	{
	public:
		SpriteAction_ButtonStateCrossFade(float time);

	private:
		virtual void Setup(SpriteController* controller, Sprite* sprite);
		virtual void Update(SpriteController* controller, Sprite* sprite, float deltaTime);

	private:
		float time_;
		float elapsedTime_;
	};

#endif /* __SpriteAction_ButtonStateCrossFade_H__ */

