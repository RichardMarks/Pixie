//*** SpriteEventListener.cpp ***

#include "SpriteEventListener.h"

namespace pixie {

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


}; // namespace pixie
