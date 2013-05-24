//*** SpriteController.cpp **

#include "SpriteController.h"
#include "Debug.h"
#include "SpriteAction.h"
#include "Sprite.h"

//*** Constructor ***

SpriteController::SpriteController(Sprite* sprite):
	sprite_(sprite),
	referenceCount_(0),
	velocityX_(0),
	velocityY_(0),
	offsetX_(0),
	offsetY_(0),
	previousOffsetX_(0),
	previousOffsetY_(0)
	{
	}


//*** Destructor ***

SpriteController::~SpriteController()
	{
	for (int i=0; i<currentActions_.GetItemCount(); i++)
		{
		ActionEntry* entry=currentActions_.Get(i);
		delete entry->action;
		delete entry;
		}

	for (int i=0; i<queuedActions_.GetItemCount(); i++)
		{
		ActionEntry* entry=queuedActions_.Get(i);
		delete entry->action;
		delete entry;
		}
	}


//*** Update ***

void SpriteController::Update(float deltaTime)
	{
	// Reset offsets and velocities
	offsetX_=0;
	offsetY_=0;
	velocityX_=0;
	velocityY_=0;

	// Get sprite position
	float x=sprite_->GetX()-previousOffsetX_;
	float y=sprite_->GetY()-previousOffsetY_;

	// Reset sprite position
	sprite_->SetPosition(x,y);

	// Update all actions
	for (int i=0; i<currentActions_.GetItemCount(); i++)
		{
		ActionEntry* entry=currentActions_.Get(i);
		if (entry->timeLeftToStart<=0)
			{
			entry->action->Update(this,GetSprite(),deltaTime);
			}
		else
			{
			entry->timeLeftToStart-=deltaTime;
			if (entry->timeLeftToStart<=0)
				{
				entry->action->Setup(this,GetSprite());
				}
			}
		}


	// Do physics
	x+=velocityX_*deltaTime;
	y+=velocityY_*deltaTime;

	// Apply offsets
	sprite_->SetPosition((x+offsetX_), (y+offsetY_));
	previousOffsetX_=offsetX_;
	previousOffsetY_=offsetY_;



	// Trigger queued actions
	for (int i=0; i<currentActions_.GetItemCount(); i++)
		{
		ActionEntry* entry=currentActions_.Get(i);
		if (entry->action->IsActionCompleted() && entry->endTrigger!=0)
			{
			for (int j=queuedActions_.GetItemCount()-1; j>=0; j--)
				{
				ActionEntry* queuedEntry=queuedActions_.Get(j);
				if (queuedEntry->startTrigger==entry->endTrigger)
					{
					queuedEntry->action->Setup(this,GetSprite());
					queuedActions_.Remove(j);
					currentActions_.Add(queuedEntry);
					}

				}
			}
		}

	// Remove completed actions
	for (int i=currentActions_.GetItemCount()-1; i>=0; i--)
		{
		ActionEntry* entry=currentActions_.Get(i);
		if (entry->action->IsActionCompleted())
			{
			currentActions_.Remove(i);
			delete entry->action;
			delete entry;
			DecreaseReferenceCount();
			}
		}
	}


//*** GetSprite ***

Sprite* SpriteController::GetSprite() const
	{
	return sprite_;
	}



//*** AddAction ***

void SpriteController::AddAction(SpriteAction* action, float delay, StringId startTrigger, StringId endTrigger, StringId actionId)
	{
	Assert(action,"Invalid action");
	if (!action)
		{
		return;
		}

	ActionEntry* entry=new ActionEntry();
	entry->action=action;
	entry->delay=delay;
	entry->startTrigger=startTrigger;
	entry->endTrigger=endTrigger;
	entry->actionId=actionId;
	entry->timeLeftToStart=delay;


	// If this action has no start trigger, add it directly to the list of current actions
	if (startTrigger==0)
		{
		currentActions_.Add(entry);
		if (delay==0)
			{
			action->Setup(this,GetSprite());
			}
		}
	else
		{
		queuedActions_.Add(entry);
		}

	IncreaseReferenceCount();
	}
	

//*** RemoveAction ***

void SpriteController::RemoveAction(SpriteAction* action)
	{
	for (int i=currentActions_.GetItemCount()-1; i>=0; i--)
		{
		ActionEntry* entry=currentActions_.Get(i);
		if (entry->action==action)
			{
			currentActions_.Remove(i);
			delete entry->action;
			delete entry;
			DecreaseReferenceCount();
			}
		}
	}


//*** RemoveAction ***

void SpriteController::RemoveAction(StringId actionId)
	{
	for (int i=currentActions_.GetItemCount()-1; i>=0; i--)
		{
		ActionEntry* entry=currentActions_.Get(i);
		if (entry->actionId==actionId)
			{
			currentActions_.Remove(i);
			delete entry->action;
			delete entry;
			DecreaseReferenceCount();
			}
		}
	}


//*** AddOffset ***

void SpriteController::AddOffset(float x, float y)
	{
	offsetX_+=x;
	offsetY_+=y;
	}


//*** AddVelocity ***

void SpriteController::AddVelocity(float x, float y)
	{
	velocityX_+=x;
	velocityY_+=y;
	}


//*** GetReferenceCount ***

int SpriteController::GetReferenceCount() const
	{
	return referenceCount_;
	}


//*** IncreaseReferenceCount ***

void SpriteController::IncreaseReferenceCount()
	{
	referenceCount_++;
	}


//*** DecreaseReferenceCount ***

void SpriteController::DecreaseReferenceCount()
	{
	Assert(referenceCount_>0,"Trying to decrease reference count to below 0");
	if (referenceCount_<=0)
		{
		return;
		}

	referenceCount_--;
	}


//*** GetActionCount ***

int SpriteController::GetActionCount() const
	{
	return currentActions_.GetItemCount()+queuedActions_.GetItemCount();
	}

