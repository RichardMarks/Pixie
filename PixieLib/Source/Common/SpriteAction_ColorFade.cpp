//*** SpriteAction_ColorFade.cpp ***
 
#include "SpriteAction_ColorFade.h"
#include "ColorHelper.h"
#include "Sprite.h"
#include "StandardLibrary.h"


//*** Constructor ***

SpriteAction_ColorFade::SpriteAction_ColorFade(unsigned short targetColor, float time):
	targetColor_(targetColor),
	time_(time),
	elapsedTime_(0),
	sourceR_(0),
	sourceG_(0),
	sourceB_(0),
	targetR_(0),
	targetG_(0),
	targetB_(0),
	deltaR_(0),
	deltaG_(0),
	deltaB_(0)
	{
	if (time_<=0)
		{
		time_=FLOAT_EPSILON;
		}
	}


//*** Setup ***

void SpriteAction_ColorFade::Setup(SpriteController* controller, Sprite* sprite)
	{
	unsigned short sourceColor=sprite->GetColor();
	sourceR_=RGB16TOFLOATR(sourceColor);
	sourceG_=RGB16TOFLOATG(sourceColor);
	sourceB_=RGB16TOFLOATB(sourceColor);

	targetR_=RGB16TOFLOATR(targetColor_);
	targetG_=RGB16TOFLOATG(targetColor_);
	targetB_=RGB16TOFLOATB(targetColor_);

	deltaR_=(targetR_-sourceR_)/time_;
	deltaG_=(targetG_-sourceG_)/time_;	
	deltaB_=(targetB_-sourceB_)/time_;
	}


//*** Update ***

void SpriteAction_ColorFade::Update(SpriteController* controller, Sprite* sprite, float deltaTime)
	{
	elapsedTime_+=deltaTime;
	
	if (elapsedTime_>time_)
		{	
		sprite->SetColor(targetColor_);
		ActionCompleted();
		return;
		}

	float r=sourceR_+deltaR_*elapsedTime_;
	float g=sourceG_+deltaG_*elapsedTime_;
	float b=sourceB_+deltaB_*elapsedTime_;

	unsigned short color=FLOATTORGB16(r,g,b);
	sprite->SetColor(color);
	}

