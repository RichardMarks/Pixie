//*** RenderCall.cpp **

#include "RenderCall.h"


//*** Constructor ***

RenderCall::RenderCall():
	callbackFunction_(0),
	userData_(0)
	{
	}


//*** Constructor ***

RenderCall::RenderCall(SpriteManager* spriteManager):
	Sprite(spriteManager),
	callbackFunction_(0),
	userData_(0)
	{
	}


//*** Destructor ***

RenderCall::~RenderCall()
	{
	}


//*** SetCallback ***

void RenderCall::SetCallback(CallbackFunction callbackFunction, void* userData)
	{
	callbackFunction_=callbackFunction;
	userData_=userData;
	}


//*** Render ***

void RenderCall::Render(Bitmap& bitmap)
	{
	if (IsVisible() && callbackFunction_)
		{
		callbackFunction_(bitmap,this,userData_);
		}

	}
