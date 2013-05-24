/**
 * \class	SpriteAction_ColorFade
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __SpriteAction_ColorFade_H__
#define __SpriteAction_ColorFade_H__

// Includes
#include "SpriteAction.h"

// Forward declares
class Sprite;

// SpriteAction_ColorFade
class SpriteAction_ColorFade:public SpriteAction
	{
	public:
		SpriteAction_ColorFade(unsigned short targetColor, float time);

	private:
		virtual void Setup(SpriteController* controller, Sprite* sprite);
		virtual void Update(SpriteController* controller, Sprite* sprite, float deltaTime);

	private:
		unsigned short targetColor_;
		float time_;
		float sourceR_;
		float sourceG_;
		float sourceB_;
		float targetR_;
		float targetG_;
		float targetB_;
		float deltaR_;
		float deltaG_;
		float deltaB_;
		float elapsedTime_;
	};

#endif /* __SpriteAction_ColorFade_H__ */

