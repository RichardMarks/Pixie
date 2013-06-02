//*** SpriteManager.cpp ***
 
#include "SpriteManager.h"
#include "Sprite.h"
#include "ArrayIterator.h"
#include "InputManager.h"
#include "SpriteSystem.h"
#include "SpriteEventListener.h"


//*** Constructor ***

SpriteManager::SpriteManager():
	originX_(0),
	originY_(0),
	priority_(0),
	currentHighlighted_( 0 ),
	activatedHighlighted_( false )
	{
	}


//*** Destructor ***

SpriteManager::~SpriteManager()
	{
	}


//*** AddSprite ***

void SpriteManager::AddSprite(Sprite* sprite)
	{
	// Need to find the right slot to insert into, to keep the list sorted
	for (int i=0; i<sprites_.GetItemCount(); i++)
		{
		Sprite* existingSprite=sprites_.Get(i);
		if (sprite->GetPriority()<existingSprite->GetPriority())
			{
			sprites_.InsertBefore(i,sprite);
			return;
			}
		}

	sprites_.Add(sprite);
	}


//*** RemoveSprite ***

void SpriteManager::RemoveSprite(Sprite* sprite)
	{
	if( currentHighlighted_ == sprite )
		{
		currentHighlighted_ = 0;
		}
	ArrayIterator<Sprite*> it(sprites_);
	if (it.Find(sprite))
		{
		sprites_.Remove(it);
		}
	}


//*** Update ***

void SpriteManager::Update(float deltaTime)
	{
	float x=siInputManager->GetCursorX();
	float y=siInputManager->GetCursorY();
	bool button=siInputManager->IsKeyDown(KEY_LBUTTON);
	bool clicked=siInputManager->WasKeyReleased(KEY_LBUTTON) || activatedHighlighted_;
	bool foundMouseOver = false;

	for (int i=sprites_.GetItemCount()-1; i>=0; i--)
		{
		Sprite* sprite=sprites_.Get(i);
		StringId eventId;
		void* userData=0;
		bool mouseOver=sprite->OnMouseOver((int)(x-originX_),(int)(y-originY_),button,eventId,userData, false);
		if( mouseOver ) 
			{
			foundMouseOver = true;
			}
		if (mouseOver && clicked && eventId.GetString())
			{
			if (SpriteSystem::IsInstanceCreated() && siSpriteSystem->HasSpriteManager(this))
				{
				siSpriteSystem->SendEvent(this,eventId,userData);
				}
			else
				{
				SendEvent(eventId,userData);
				}
			
			break;
			}
		}

	if( !foundMouseOver )
		{
		for (int i=sprites_.GetItemCount()-1; i>=0; i--)
			{
			Sprite* sprite=sprites_.Get(i);
			StringId eventId;
			void* userData=0;
			if( currentHighlighted_ && currentHighlighted_->GetSelectionIndex() == sprite->GetSelectionIndex() )
				{
				bool mouseOver=sprite->OnMouseOver((int)(sprite->GetX()+1.0f-originX_),(int)(sprite->GetY()+1.0f-originY_),button,eventId,userData, true);
				if (mouseOver && activatedHighlighted_ && eventId.GetString())
					{
					activatedHighlighted_ = false;
					if (SpriteSystem::IsInstanceCreated() && siSpriteSystem->HasSpriteManager(this))
						{
						siSpriteSystem->SendEvent(this,eventId,userData);
						}
					else
						{
						SendEvent(eventId,userData);
						}

					return;
					}
				}
			}
		}
	else if( currentHighlighted_ )
		{
		StringId eventId;
		void* userData=0;
		currentHighlighted_->OnMouseOver(0,0,button,eventId,userData, false);
		}

	activatedHighlighted_ = false;
	}


//*** Render ***

void SpriteManager::Render(Bitmap& bitmap)
	{
	for (int i=0; i<sprites_.GetItemCount(); i++)
		{
		Sprite* sprite=sprites_.Get(i);
		float x=sprite->GetX();
		float y=sprite->GetY();
		sprite->SetX(x+originX_);
		sprite->SetY(y+originY_);
		sprite->Render(bitmap);
		sprite->SetX(x);
		sprite->SetY(y);
		}
	}


//*** AddEventListener ***

void SpriteManager::AddEventListener(SpriteEventListener* listener)
	{
	eventListeners_.Add(listener);
	}


//*** RemoveEventListener ***

void SpriteManager::RemoveEventListener(SpriteEventListener* listener)
	{
	ArrayIterator<SpriteEventListener*> it(eventListeners_);
	if (it.Find(listener))
		{
		eventListeners_.Remove(it);
		}
	}


//*** GetPriority ***

float SpriteManager::GetPriority() const
	{
	return priority_;
	}


//*** SetPriority***

void SpriteManager::SetPriority(float priority)
	{
	if (priority==priority_)
		{
		return;
		}

	priority_=priority;

	if (SpriteSystem::IsInstanceCreated())
		{
		siSpriteSystem->UpdatePriority(this);
		}
	}


//*** GetOriginX ***

float SpriteManager::GetOriginX() const
	{
	return originX_;
	}


//*** GetOriginY ***

float SpriteManager::GetOriginY() const
	{
	return originY_;
	}


//*** SetOrigin ***

void SpriteManager::SetOrigin(float x, float y)
	{
	originX_=x;
	originY_=y;
	}


//*** SetOriginX ***

void SpriteManager::SetOriginX(float x)
	{
	originX_=x;
	}


//*** SetOriginY ***

void SpriteManager::SetOriginY(float y)
	{
	originY_=y;
	}


//*** SendEvent ***

void SpriteManager::SendEvent(StringId event, void* userData)
	{
	for (int i=0; i<eventListeners_.GetItemCount(); i++)
		{
		SpriteEventListener* listener=eventListeners_.Get(i);
		listener->OnSpriteEvent(event,userData);
		}
	}


//*** FindLowestIndex ***

Sprite* SpriteManager::FindLowestIndex()
	{
	Sprite* bestMatch = 0;
	for (int i=0; i<sprites_.GetItemCount(); i++)
		{
		Sprite* sprite=sprites_.Get(i);
		int index = sprite->GetSelectionIndex();
		if( index >=0 )
			{
			if( !bestMatch )
				{
				bestMatch = sprite;
				}
			else if( index < bestMatch->GetSelectionIndex() )
				{
				bestMatch = sprite;
				}
			}
		}
	return bestMatch;
	}


//*** FindHighestIndex ***

Sprite* SpriteManager::FindHighestIndex()
	{
	Sprite* bestMatch = 0;
	for (int i=0; i<sprites_.GetItemCount(); i++)
		{
		Sprite* sprite=sprites_.Get(i);
		int index = sprite->GetSelectionIndex();
		if( index >=0 )
			{
			if( !bestMatch )
				{
				bestMatch = sprite;
				}
			else if( index > bestMatch->GetSelectionIndex() )
				{
				bestMatch = sprite;
				}
			}
		}
	return bestMatch;
	}


//*** HighlightNext ***

void SpriteManager::HighlightNext()
	{
	activatedHighlighted_ = false;
	Sprite* lowest = FindLowestIndex();
	Sprite* highest = FindHighestIndex();
	if( !currentHighlighted_ )
		{
		currentHighlighted_ = lowest;
		}
	else if( currentHighlighted_->GetSelectionIndex() == highest->GetSelectionIndex() )
		{
		currentHighlighted_ = lowest;
		}
	else 
		{
		Sprite* bestMatch = 0;
		for (int i=0; i<sprites_.GetItemCount(); i++)
			{
			Sprite* sprite=sprites_.Get(i);
			if( sprite != currentHighlighted_ && sprite->GetSelectionIndex() >=0 && sprite->GetSelectionIndex() > currentHighlighted_->GetSelectionIndex() )
				{
				if( !bestMatch)
					{
					bestMatch = sprite;
					}
				else if( sprite->GetSelectionIndex() < bestMatch->GetSelectionIndex() )
					{
					bestMatch = sprite;
					}
				}
			}
		currentHighlighted_ = bestMatch;
		}
	}


//*** HighlightPrevious ***

void SpriteManager::HighlightPrevious()
	{
	activatedHighlighted_ = false;
	Sprite* lowest = FindLowestIndex();
	Sprite* highest = FindHighestIndex();
	if( !currentHighlighted_ )
		{
		currentHighlighted_ = highest;
		}
	else if( currentHighlighted_->GetSelectionIndex() == lowest->GetSelectionIndex() )
		{
		currentHighlighted_ = highest;
		}
	else 
		{
		Sprite* bestMatch = 0;
		for (int i=0; i<sprites_.GetItemCount(); i++)
			{
			Sprite* sprite=sprites_.Get(i);
			if( sprite != currentHighlighted_ && sprite->GetSelectionIndex() >=0 && sprite->GetSelectionIndex() < currentHighlighted_->GetSelectionIndex() )
				{
				if( !bestMatch)
					{
					bestMatch = sprite;
					}
				else if( sprite->GetSelectionIndex() > bestMatch->GetSelectionIndex() )
					{
					bestMatch = sprite;
					}
				}
			}
		currentHighlighted_ = bestMatch;
		}
	}


//*** SelectHighlighted ***

void SpriteManager::SelectHighlighted()
	{
	if( currentHighlighted_ )
		{
		activatedHighlighted_ = true;
		}
	}


//*** GetHighlightedIndex ***

int SpriteManager::GetHighlightedIndex()
	{
	if( !currentHighlighted_ )
		{
		return -1;
		}
	
	return currentHighlighted_->GetSelectionIndex();
	}