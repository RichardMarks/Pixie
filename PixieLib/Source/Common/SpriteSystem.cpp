//*** SpriteSystem.cpp ***

#include "SpriteSystem.h"
#include "Debug.h"
#include "SpriteManager.h"
#include "ArrayIterator.h"
#include "SpriteEventListener.h"


//*** Constructor ***

SpriteSystem::SpriteSystem():
	defaultSpriteManager_(0)
	{

	// Add the default sprite manager
	AddSpriteManager(new SpriteManager());
	}


//*** Destructor ***

SpriteSystem::~SpriteSystem()
	{
	// Delete all managers
	for (int i=0; i<spriteManagers_.GetItemCount(); i++)
		{
		SpriteManager* spriteManager=spriteManagers_.Get(i);
		delete spriteManager;
		}

	}


//*** Update ***

void SpriteSystem::Update(float deltaTime)
	{
	// Update all managers
	for (int i=spriteManagers_.GetItemCount()-1; i>=0; i--)
		{
		SpriteManager* spriteManager=spriteManagers_.Get(i);
		spriteManager->Update(deltaTime);
		}
	}


//*** Render ***

void SpriteSystem::Render(Bitmap& bitmap)
	{
	// Render all managers
	for (int i=spriteManagers_.GetItemCount()-1; i>=0; i--)
		{
		SpriteManager* spriteManager=spriteManagers_.Get(i);
		spriteManager->Render(bitmap);
		}
	}


//*** GetDefaultSpriteManager ***

SpriteManager* SpriteSystem::GetDefaultSpriteManager()
	{
	// Make sure there's always a valid defult sprite manager
	if (!defaultSpriteManager_)
		{
		if (spriteManagers_.GetItemCount()>0)
			{
			defaultSpriteManager_=spriteManagers_.Get(spriteManagers_.GetItemCount()-1);
			}
		else
			{
			AddSpriteManager(new SpriteManager());
			}
		}

	Assert(defaultSpriteManager_,"No default sprite manager");
	return defaultSpriteManager_;
	}


//*** SetDefaultSpriteManager ***

void SpriteSystem::SetDefaultSpriteManager(SpriteManager* spriteManager)
	{
	defaultSpriteManager_=spriteManager;

	// Check if the specified sprite manager is already in the manager list
	for (int i=0; i<spriteManagers_.GetItemCount(); i++)
		{
		if (spriteManagers_.Get(i)==spriteManager)
			{
			// Yes, it already exists, so we're done
			return;
			}
		}

	// If not, add it now
	AddSpriteManager(spriteManager);
	}


//*** GetSpriteManagerCount ***

int SpriteSystem::GetSpriteManagerCount() const
	{
	return spriteManagers_.GetItemCount();
	}


//*** GetSpriteManager ***

SpriteManager* SpriteSystem::GetSpriteManager(int index) const
	{
	return spriteManagers_.Get(index);
	}


//*** AddSpriteManager ***

void SpriteSystem::AddSpriteManager(SpriteManager* spriteManager)
	{
	// Check if the specified sprite manager is already in the manager list
	ArrayIterator<SpriteManager*> it(spriteManagers_);
	if (it.Find(spriteManager))
		{
		// Yes, it already exists, so just make sure the priority is correct
		UpdatePriority(spriteManager);
		return;
		}

	// No, it's not already in the manager list, so insert it now
	spriteManagers_.Add(spriteManager);

	// And make sure the priorities are correct
	UpdatePriority(spriteManager);


	// If there's no default sprite manager, make this one the default
	if (!defaultSpriteManager_)
		{
		defaultSpriteManager_=spriteManager;
		}
	}


//*** RemoveSpriteManager ***

void SpriteSystem::RemoveSpriteManager(SpriteManager* spriteManager)
	{
	// Find the specified manager in the manager list
	ArrayIterator<SpriteManager*> it(spriteManagers_);
	if (it.Find(spriteManager))
		{
		// If this was the default one, it is no longer valid
		if (defaultSpriteManager_==spriteManager)
			{
			defaultSpriteManager_=0;
			}

		// Remove the sprite manager
		spriteManagers_.Remove(it);
		
		return;
		}

	// Manager not found, so report this in debug mode
	Assert(false,"Specified sprite manager could not be found");
	}


//*** UpdatePriority ***

void SpriteSystem::UpdatePriority(SpriteManager* spriteManager)
	{
	ArrayIterator<SpriteManager*> it(spriteManagers_);
	if (it.Find(spriteManager))
		{
		spriteManagers_.Remove(it);

		// Insert at the right position
		for (int i=0; i<spriteManagers_.GetItemCount(); i++)
			{
			SpriteManager* existingManager=spriteManagers_.Get(i);
			if (spriteManager->GetPriority()<existingManager->GetPriority())
				{
				spriteManagers_.InsertBefore(i,spriteManager);
				return;
				}
			}

		spriteManagers_.Add(spriteManager);
		return;
		}

	Assert(false,"Sprite manager not found");
	}


//*** AddEventListener ***

void SpriteSystem::AddEventListener(SpriteEventListener* listener)
	{
	eventListeners_.Add(listener);
	}


//*** RemoveEventListener ***

void SpriteSystem::RemoveEventListener(SpriteEventListener* listener)
	{
	ArrayIterator<SpriteEventListener*> it(eventListeners_);
	if (it.Find(listener))
		{
		eventListeners_.Remove(it);
		}
	}


//*** HasSpriteManager ***

bool SpriteSystem::HasSpriteManager(SpriteManager* spriteManager) const
	{
	ArrayIterator<SpriteManager*> it(spriteManagers_);
	return it.Find(spriteManager);
	}


//*** SendEvent ***

void SpriteSystem::SendEvent(SpriteManager* spriteManager, StringId event, void* userData)
	{
	// Only send on the event if the we're controlling the specified manager
	ArrayIterator<SpriteManager*> it(spriteManagers_);
	if (it.Find(spriteManager))
		{
		for (int i=0; i<eventListeners_.GetItemCount(); i++)
			{
			SpriteEventListener* listener=eventListeners_.Get(i);
			listener->OnSpriteEvent(event,userData);
			}
		}
	}
