/**
 * \class	SpriteAction_AlphaSine
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __SpriteAction_AlphaSine_H__
#define __SpriteAction_AlphaSine_H__

// Includes
#include "SpriteAction.h"

// Forward declares
class Sprite;

// SpriteAction_AlphaSine
class SpriteAction_AlphaSine:public SpriteAction
	{
	public:
		SpriteAction_AlphaSine(float amplitude, float offset, float frequency, float easeIn);
		SpriteAction_AlphaSine(float amplitude, float offset, float frequency, float time, float easeIn, float easeOut);

	private:
		virtual void Setup(SpriteController* controller, Sprite* sprite);
		virtual void Update(SpriteController* controller, Sprite* sprite, float deltaTime);

	private:
		float amplitude_;
		float offset_;
		float frequency_;
		float easeIn_;
		bool perpetual_;
		float time_;
		float easeOut_;
		float elapsedTime_;

	};

#endif /* __SpriteAction_AlphaSine_H__ */

