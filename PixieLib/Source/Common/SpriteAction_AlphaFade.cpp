//*** SpriteAction_AlphaFade.cpp ***
 
#include "SpriteAction_AlphaFade.h"
#include "ColorHelper.h"
#include "Sprite.h"
#include "StandardLibrary.h"


//*** Constructor ***

SpriteAction_AlphaFade::SpriteAction_AlphaFade(unsigned char targetAlpha, float time):
	targetAlpha_(targetAlpha),
	time_(time),
	elapsedTime_(0),
	sourceA_(0),
	targetA_(0),
	deltaA_(0)
	{
	if (time_<=0)
		{
		time_=FLOAT_EPSILON;
		}
	}


//*** Setup ***

void SpriteAction_AlphaFade::Setup(SpriteController* controller, Sprite* sprite)
	{
	unsigned char sourceAlpha=sprite->GetAlpha();
	sourceA_=((float)sourceAlpha)/255.0f;

	targetA_=((float)targetAlpha_)/255.0f;

	deltaA_=(targetA_-sourceA_)/time_;
	}


//*** Update ***

void SpriteAction_AlphaFade::Update(SpriteController* controller, Sprite* sprite, float deltaTime)
	{
	elapsedTime_+=deltaTime;
	
	if (elapsedTime_>time_)
		{	
		sprite->SetAlpha(targetAlpha_);
		ActionCompleted();
		return;
		}

	float a=sourceA_+deltaA_*elapsedTime_;
	if (targetA_<sourceA_)
		{
		a=Max(a,targetA_);
		}
	else
		{
		a=Min(a,targetA_);
		}

	unsigned char alpha=(unsigned char)(a*255.0f);
	sprite->SetAlpha(alpha);
	}

