/**
 * \class	SpriteAction_SineWave
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __SpriteAction_SineWave_H__
#define __SpriteAction_SineWave_H__

// Includes
#include "SpriteAction.h"

// Forward declares
class Sprite;

// SpriteAction_SineWave
class SpriteAction_SineWave:public SpriteAction
	{
	public:
		SpriteAction_SineWave(float amplitudeX, float offsetX, float frequencyX, float amplitudeY, float offsetY, float frequencyY, float easeIn);
		SpriteAction_SineWave(float amplitudeX, float offsetX, float frequencyX, float amplitudeY, float offsetY, float frequencyY, float time, float easeIn, float easeOut);

	private:
		virtual void Setup(SpriteController* controller, Sprite* sprite);
		virtual void Update(SpriteController* controller, Sprite* sprite, float deltaTime);

	private:
		float amplitudeX_;
		float offsetX_;
		float frequencyX_;
		float amplitudeY_;
		float offsetY_;
		float frequencyY_;
		float easeIn_;
		bool perpetual_;
		float time_;
		float easeOut_;
		float elapsedTime_;

	};

#endif /* __SpriteAction_SineWave_H__ */

