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
	priority_(0)
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
			sprites_.InsertBefore(sprite,i);
			return;
			}
		}

	sprites_.Add(sprite);
	}


//*** RemoveSprite ***

void SpriteManager::RemoveSprite(Sprite* sprite)
	{
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
	bool clicked=siInputManager->WasKeyReleased(KEY_LBUTTON);

	for (int i=sprites_.GetItemCount()-1; i>=0; i--)
		{
		Sprite* sprite=sprites_.Get(i);
		StringId eventId;
		void* userData=0;
		bool mouseOver=sprite->OnMouseOver((int)(x-originX_),(int)(y-originY_),button,eventId,userData);
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


