//*** SpriteAction_SineWave.cpp ***
 
#include "SpriteAction_SineWave.h"
#include "ColorHelper.h"
#include "Sprite.h"
#include "SpriteController.h"


//*** Constructor ***

SpriteAction_SineWave::SpriteAction_SineWave(float amplitudeX, float offsetX, float frequencyX, float amplitudeY, float offsetY, float frequencyY, float easeIn):
	amplitudeX_(amplitudeX),
	offsetX_(offsetX),
	frequencyX_(frequencyX),
	amplitudeY_(amplitudeY),
	offsetY_(offsetY),
	frequencyY_(frequencyY),
	easeIn_(easeIn),
	perpetual_(true),
	time_(0),
	easeOut_(0),
	elapsedTime_(0)
	{
	}


//*** Constructor ***

SpriteAction_SineWave::SpriteAction_SineWave(float amplitudeX, float offsetX, float frequencyX, float amplitudeY, float offsetY, float frequencyY, float time, float easeIn, float easeOut):
	amplitudeX_(amplitudeX),
	offsetX_(offsetX),
	frequencyX_(frequencyX),
	amplitudeY_(amplitudeY),
	offsetY_(offsetY),
	frequencyY_(frequencyY),
	easeIn_(easeIn),
	perpetual_(false),
	time_(time),
	easeOut_(easeOut),
	elapsedTime_(0)
	{
	}


//*** Setup ***

void SpriteAction_SineWave::Setup(SpriteController* controller, Sprite* sprite)
	{
	if (time_==0)
		{
		perpetual_=true;
		}
	}


//*** Update ***

void SpriteAction_SineWave::Update(SpriteController* controller, Sprite* sprite, float deltaTime)
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

	float xOffset=Sin(elapsedTime_*frequencyX_+ToRadians(offsetX_))*amplitudeX_*scale;
	float yOffset=Sin(elapsedTime_*frequencyY_+ToRadians(offsetY_))*amplitudeY_*scale;

	controller->AddOffset(xOffset,yOffset);
	}

