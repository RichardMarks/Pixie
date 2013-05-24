//*** SpriteControllerManager.cpp ***

#include "SpriteControllerManager.h"
#include "Debug.h"
#include "SpriteController.h"
#include "ArrayIterator.h"

//*** Constructor ***

SpriteControllerManager::SpriteControllerManager()
	{
	}


//*** Destructor ***

SpriteControllerManager::~SpriteControllerManager()
	{
	// Delete all controllers
	for (int i=0; i<spriteControllers_.GetItemCount(); i++)
		{
		SpriteController* spriteController=spriteControllers_.Get(i);
		delete spriteController;
		}
	}


//*** Update ***

void SpriteControllerManager::Update(float deltaTime)
	{
	// Update all controllers
	for (int i=0; i<spriteControllers_.GetItemCount(); i++)
		{
		SpriteController* spriteController=spriteControllers_.Get(i);
		spriteController->Update(deltaTime);
		}

	// Find controllers to delete
	for (int i=spriteControllers_.GetItemCount()-1; i>=0; i--)
		{
		SpriteController* spriteController=spriteControllers_.Get(i);
		if (spriteController->GetReferenceCount()==0)
			{
			ArrayIterator<SpriteController*> it(spriteControllers_);
			if (it.Find(spriteController))
				{
				spriteControllers_.Remove(it);
				}
			delete spriteController;
			}		
		}
	}


//*** AcquireSpriteController ***

SpriteController* SpriteControllerManager::AcquireSpriteController(Sprite* sprite)
	{
	for (int i=0; i<spriteControllers_.GetItemCount(); i++)
		{
		SpriteController* spriteController=spriteControllers_.Get(i);
		if (spriteController->GetSprite()==sprite)
			{
			spriteController->IncreaseReferenceCount();
			return spriteController;
			}
		}

	SpriteController* spriteController=new SpriteController(sprite);
	spriteController->IncreaseReferenceCount();
	spriteControllers_.Add(spriteController);

	return spriteController;
	}


//*** ReleaseSpriteController ***

void SpriteControllerManager::ReleaseSpriteController(SpriteController* spriteController)
	{
	spriteController->DecreaseReferenceCount();
	if (spriteController->GetReferenceCount()==0)
		{
		ArrayIterator<SpriteController*> it(spriteControllers_);
		if (it.Find(spriteController))
			{
			spriteControllers_.Remove(it);
			}
		delete spriteController;
		}
	}


//*** GetActionCount ***

int SpriteControllerManager::GetActionCount(Sprite* sprite) const
	{
	for (int i=0; i<spriteControllers_.GetItemCount(); i++)
		{
		SpriteController* spriteController=spriteControllers_.Get(i);
		if (spriteController->GetSprite()==sprite)
			{
			return spriteController->GetActionCount();
			}
		}

	return 0;
	}


//*** SpriteDeleted ***

void SpriteControllerManager::SpriteDeleted(Sprite* sprite)
	{
	for (int i=0; i<spriteControllers_.GetItemCount(); i++)
		{
		SpriteController* spriteController=spriteControllers_.Get(i);
		if (spriteController->GetSprite()==sprite)
			{
			spriteControllers_.Remove(i);
			delete spriteController;
			return;
			}
		}
	}