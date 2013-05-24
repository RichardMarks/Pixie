//*** SpriteEventListener.cpp ***
 
#include "SpriteEventListener.h"


//*** Constructor ***

SpriteEventListener::SpriteEventListener()
	{
	if (SpriteSystem::IsInstanceCreated())
		{
		siSpriteSystem->AddEventListener(this);
		}
	}


//*** Destructor ***

SpriteEventListener::~SpriteEventListener()
	{
	if (SpriteSystem::IsInstanceCreated())
		{
		siSpriteSystem->RemoveEventListener(this);
		}
	}


