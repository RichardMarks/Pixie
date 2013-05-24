//*** SpriteAction_ButtonStateCrossFade.cpp ***
 
#include "SpriteAction_ButtonStateCrossFade.h"
#include "ColorHelper.h"
#include "Sprite.h"
#include "Button.h"


//*** Constructor ***

SpriteAction_ButtonStateCrossFade::SpriteAction_ButtonStateCrossFade(float time):
	time_(time),
	elapsedTime_(0)
	{
	if (time_<=0)
		{
		time_=FLOAT_EPSILON;
		}
	}


//*** Setup ***

void SpriteAction_ButtonStateCrossFade::Setup(SpriteController* controller, Sprite* sprite)
	{
	Button* button=static_cast<Button*>(sprite);
	button->SetStateCrossFadeAlpha(255,0);
	}


//*** Update ***

void SpriteAction_ButtonStateCrossFade::Update(SpriteController* controller, Sprite* sprite, float deltaTime)
	{
	Button* button=static_cast<Button*>(sprite);

	elapsedTime_+=deltaTime;
	
	if (elapsedTime_>time_)
		{	
		button->SetStateCrossFadeAlpha(0,255);
		ActionCompleted();
		return;
		}

	// Handle state crossfade
	if (elapsedTime_<=time_*0.5f)
		{
		float i=elapsedTime_/(time_*0.5f);
		button->SetStateCrossFadeAlpha(255,(unsigned char)(255*i))			;
		}
	else
		{
		float i=(elapsedTime_-time_*0.5f)/(time_*0.5f);
		button->SetStateCrossFadeAlpha(255-(unsigned char)(255*i),255);
		}
	}

