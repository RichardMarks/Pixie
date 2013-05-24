//*** SpriteAction_AlphaPulse.cpp ***
 
#include "SpriteAction_AlphaPulse.h"
#include "ColorHelper.h"
#include "Sprite.h"
#include "StandardLibrary.h"


//*** Constructor ***

SpriteAction_AlphaPulse::SpriteAction_AlphaPulse(float speed):
	speed_(speed),
	currentSource_(0),
	currentTarget_(0)
	{
	if (speed_<=0)
		{
		speed_=FLOAT_EPSILON;
		}
	}


//*** Setup ***

void SpriteAction_AlphaPulse::Setup(SpriteController* controller, Sprite* sprite)
	{
	unsigned char sourceAlpha=sprite->GetAlpha();
	currentSource_=((float)sourceAlpha)/255.0f;
	currentTarget_=0.0f;
	}


//*** Update ***

void SpriteAction_AlphaPulse::Update(SpriteController* controller, Sprite* sprite, float deltaTime)
	{
	if (currentTarget_<currentSource_)
		{
		currentSource_-=deltaTime*speed_;
		if (currentSource_<=currentTarget_)
			{
			currentSource_=0.0f;
			currentTarget_=1.0f;
			}
		}
	else
		{
		currentSource_+=deltaTime*speed_;
		if (currentSource_>=currentTarget_)
			{
			currentSource_=1.0f;
			currentTarget_=0.0f;
			}
		}
	int alpha=Clamp((int)(currentSource_*255.0f),1,255);
	
	sprite->SetAlpha((char)alpha);
	}

