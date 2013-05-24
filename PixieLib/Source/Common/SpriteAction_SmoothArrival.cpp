//*** SpriteAction_SmoothArrival.cpp ***
 
#include "SpriteAction_SmoothArrival.h"
#include "ColorHelper.h"
#include "Sprite.h"
#include "SpriteController.h"


//*** Constructor ***

SpriteAction_SmoothArrival::SpriteAction_SmoothArrival(float targetX, float targetY, float speed):
	targetX_(targetX),
	targetY_(targetY),
	speed_(speed),
	velocityX_(0),
	velocityY_(0)
	{
	}


//*** Setup ***

void SpriteAction_SmoothArrival::Setup(SpriteController* controller, Sprite* sprite)
	{
	}


//*** Update ***

void SpriteAction_SmoothArrival::Update(SpriteController* controller, Sprite* sprite, float deltaTime)
	{
	float diffX=targetX_-sprite->GetX();
	float diffY=targetY_-sprite->GetY();
	
	if (Abs(diffX)<2.0f && Abs(diffY)<2.0f)
		{
		sprite->SetPosition(targetX_,targetY_);
		ActionCompleted();
		return;
		}

	controller->AddVelocity(diffX*speed_,diffY*speed_);
	}
