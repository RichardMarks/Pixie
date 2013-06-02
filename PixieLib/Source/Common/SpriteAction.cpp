//*** SpriteAction.cpp ***

#include "SpriteAction.h"

namespace pixie {

//*** Constructor ***

SpriteAction::SpriteAction():
	actionCompleted_(false)
	{
	}


//*** ActionCompleted ***

void SpriteAction::ActionCompleted()
	{
	actionCompleted_=true;
	}


//*** IsActionCompleted ***

bool SpriteAction::IsActionCompleted() const
	{
	return actionCompleted_;
	}

	}; // namespace pixie
