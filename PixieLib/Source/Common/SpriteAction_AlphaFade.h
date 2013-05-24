/**
 * \class	SpriteAction_AlphaFade
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __SpriteAction_AlphaFade_H__
#define __SpriteAction_AlphaFade_H__

// Includes
#include "SpriteAction.h"

// Forward declares
class Sprite;

// SpriteAction_AlphaFade
class SpriteAction_AlphaFade:public SpriteAction
	{
	public:
		SpriteAction_AlphaFade(unsigned char targetAlpha, float time);

	private:
		virtual void Setup(SpriteController* controller, Sprite* sprite);
		virtual void Update(SpriteController* controller, Sprite* sprite, float deltaTime);

	private:
		unsigned char targetAlpha_;
		float time_;
		float sourceA_;
		float targetA_;
		float deltaA_;
		float elapsedTime_;
	};

#endif /* __SpriteAction_AlphaFade_H__ */

