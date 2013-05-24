//*** SpriteAction_AlphaSine.cpp ***
 
#include "SpriteAction_AlphaSine.h"
#include "ColorHelper.h"
#include "Sprite.h"
#include "SpriteController.h"


//*** Constructor ***

SpriteAction_AlphaSine::SpriteAction_AlphaSine(float amplitude, float offset, float frequency, float easeIn):
	amplitude_(amplitude),
	offset_(offset),
	frequency_(frequency),
	easeIn_(easeIn),
	perpetual_(true),
	time_(0),
	easeOut_(0),
	elapsedTime_(0)
	{
	}


//*** Constructor ***

SpriteAction_AlphaSine::SpriteAction_AlphaSine(float amplitude, float offset, float frequency, float time, float easeIn, float easeOut):
	amplitude_(amplitude),
	offset_(offset),
	frequency_(frequency),
	easeIn_(easeIn),
	perpetual_(false),
	time_(time),
	easeOut_(easeOut),
	elapsedTime_(0)
	{
	}


//*** Setup ***

void SpriteAction_AlphaSine::Setup(SpriteController* controller, Sprite* sprite)
	{
	if (time_==0)
		{
		perpetual_=true;
		}
	}


//*** Update ***

void SpriteAction_AlphaSine::Update(SpriteController* controller, Sprite* sprite, float deltaTime)
	{
	elapsedTime_+=deltaTime;
	
	if (!perpetual_ && elapsedTime_>time_)
		{	
		ActionCompleted();
		return;
		}

	float scale=1.0f;
	if (elapsedTime_<easeIn_)
		{
		scale=elapsedTime_/easeIn_;
		}
	else if (!perpetual_ && elapsedTime_>time_-easeOut_)
		{
		scale=1.0f-(elapsedTime_-(time_-easeOut_))/easeOut_;
		}

	float sine=Sin(elapsedTime_*frequency_+ToRadians(offset_))*amplitude_*scale;
	sine=(sine+1.0f)*0.5f;
	
	int alpha=Clamp((int)(sine*255.0f),1,255);	
	sprite->SetAlpha((char)alpha);
	}

