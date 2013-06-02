//*** Sprite.cpp **

#include "Sprite.h"
#include "Bitmap.h"
#include "BitmapStrip.h"
#include "SpriteManager.h"
#include "SpriteControllerManager.h"
#include "Resource_BitmapStrip.h"
#include "SpriteSystem.h"
#include "StandardLibrary.h"

namespace pixie {

//*** Constructor ***

Sprite::Sprite():
	spriteManager_(0),
	x_(0),
	y_(0),
	originX_(0),
	originY_(0),
	visible_(true),
	color_(0xffff),
	alpha_(0xff),
	priority_(0),
	cel_(0),
	selectionIndex_(-1)
	{
	if (SpriteSystem::IsInstanceCreated())
		{
		spriteManager_=siSpriteSystem->GetDefaultSpriteManager();
		spriteManager_->AddSprite(this);
		}
	}


//*** Copy Constructor ***

Sprite::Sprite(const Sprite& spriteToCopy):
	spriteManager_(spriteToCopy.spriteManager_),
	x_(spriteToCopy.x_),
	y_(spriteToCopy.y_),
	originX_(spriteToCopy.originX_),
	originY_(spriteToCopy.originY_),
	visible_(spriteToCopy.visible_),
	color_(spriteToCopy.color_),
	alpha_(spriteToCopy.alpha_),
	priority_(spriteToCopy.priority_),
	cel_(spriteToCopy.cel_),
	bitmapStrip_(spriteToCopy.bitmapStrip_)
	{
	if (spriteManager_)
		{
		spriteManager_->AddSprite(this);
		}
	}

//*** Assignment operator ***

const Sprite& Sprite::operator = (const Sprite& spriteToCopy)
	{
	if (spriteManager_)
		{
		spriteManager_->RemoveSprite(this);
		}

	if (SpriteControllerManager::IsInstanceCreated())
		{
		siSpriteControllerManager->SpriteDeleted(this);
		}

	spriteManager_ = spriteToCopy.spriteManager_;
	x_ = spriteToCopy.x_;
	y_ = spriteToCopy.y_;
	originX_ = spriteToCopy.originX_;
	originY_ = spriteToCopy.originY_;
	visible_ = spriteToCopy.visible_;
	color_ = spriteToCopy.color_;
	alpha_ = spriteToCopy.alpha_;
	priority_ = spriteToCopy.priority_;
	cel_ = spriteToCopy.cel_;
	bitmapStrip_ = spriteToCopy.bitmapStrip_;

	if (spriteManager_)
		{
		spriteManager_->AddSprite(this);
		}

	return *this;
	}

//*** Constructor ***

Sprite::Sprite(SpriteManager* spriteManager):
	spriteManager_(spriteManager),
	x_(0),
	y_(0),
	originX_(0),
	originY_(0),
	visible_(true),
	color_(0xffff),
	alpha_(0xff),
	priority_(0),
	cel_(0)
	{
	Assert(spriteManager_,"Sprite manager not valid");
	if (spriteManager_)
		{
		spriteManager_->AddSprite(this);
		}
	}


//*** Destructor ***

Sprite::~Sprite()
	{

	if (spriteManager_)
		{
		spriteManager_->RemoveSprite(this);
		}

	if (SpriteControllerManager::IsInstanceCreated())
		{
		siSpriteControllerManager->SpriteDeleted(this);
		}
	}


//*** GetSpriteManager ***

SpriteManager* Sprite::GetSpriteManager() const
	{
	return spriteManager_;
	}


//*** SetSpriteManager ***

void Sprite::SetSpriteManager(SpriteManager* spriteManager)
	{
	if (spriteManager==spriteManager_)
		{
		return;
		}

	if (spriteManager_)
		{
		spriteManager_->RemoveSprite(this);
		}

	spriteManager_=spriteManager;

	if (spriteManager_)
		{
		spriteManager_->AddSprite(this);
		}
	}


//*** GetX ***

float Sprite::GetX() const
	{
	return x_;
	}


//*** GetY ***

float Sprite::GetY() const
	{
	return y_;
	}


//*** SetPosition ***

void Sprite::SetPosition(float x, float y)
	{
	x_=x;
	y_=y;
	}


//*** SetX ***

void Sprite::SetX(float x)
	{
	x_=x;
	}


//*** SetY ***

void Sprite::SetY(float y)
	{
	y_=y;
	}


//*** GetOriginX ***

float Sprite::GetOriginX() const
	{
	return originX_;
	}


//*** GetOriginY ***

float Sprite::GetOriginY() const
	{
	return originY_;
	}


//*** SetOrigin ***

void Sprite::SetOrigin(float x, float y)
	{
	originX_=x;
	originY_=y;
	}


//*** SetOriginX ***

void Sprite::SetOriginX(float x)
	{
	originX_=x;
	}


//*** SetOriginY ***

void Sprite::SetOriginY(float y)
	{
	originY_=y;
	}


//*** IsVisible ***

bool Sprite::IsVisible() const
	{
	return visible_;
	}


//*** SetVisible ***

void Sprite::SetVisible(bool visible)
	{
	visible_=visible;
	}


//*** GetColor ***

unsigned short Sprite::GetColor() const
	{
	return color_;
	}


//*** SetColor ***

void Sprite::SetColor(unsigned short color)
	{
	color_=color;
	}


//*** GetAlpha ***

unsigned char Sprite::GetAlpha() const
	{
	return alpha_;
	}


//*** SetAlpha ***

void Sprite::SetAlpha(unsigned char alpha)
	{
	alpha_=alpha;
	}


//*** GetPriority ***

float Sprite::GetPriority() const
	{
	return priority_;
	}


//*** SetPriority***

void Sprite::SetPriority(float priority)
	{
	if (priority==priority_)
		{
		return;
		}

	if (spriteManager_)
		{
		spriteManager_->RemoveSprite(this);
		}

	priority_=priority;

	if (spriteManager_)
		{
		spriteManager_->AddSprite(this);
		}
	}


//*** GetCel ***

float Sprite::GetCel() const
	{
	return cel_;
	}


//*** SetCel ***

void Sprite::SetCel(float cel)
	{
	cel_=cel;
	}


//*** GetBitmap ***

const Resource_BitmapStrip& Sprite::GetBitmap() const
	{
	return bitmapStrip_;
	}


//*** SetBitmap ***

void Sprite::SetBitmap(const Resource_BitmapStrip& bitmapStrip)
	{
	bitmapStrip_=bitmapStrip;
	}


//*** PickPixel ***

bool Sprite::PickPixel(int x, int y) const
	{
	if (GetBitmap().GetCelCount()!=0)
		{
		const Bitmap& bitmap=GetBitmap().GetCel((int)GetCel());
		if (bitmap.GetPixelAlpha(x-(int)(GetX()-GetOriginX()),y-(int)(GetY()-GetOriginY()))>64)
			{
			return true;
			}
		}

	return false;
	}


//*** Render ***

void Sprite::Render(Bitmap& bitmap)
	{
	if (!visible_)
		{
		return;
		}

	GetBitmap().Blit((int)GetCel(),bitmap,Round(GetX()-GetOriginX()),Round(GetY()-GetOriginY()),GetColor(),GetAlpha());
	}


//*** OnMouseOver ***

bool Sprite::OnMouseOver(int x, int y, bool button, StringId& eventId, void*& userData, bool forcehit)
	{
	return false;
	}


//*** SetSelectionIndex ***

void Sprite::SetSelectionIndex( int index )
	{
	selectionIndex_ = index;
	}


//*** FetSelectionIndex ***

int Sprite::GetSelectionIndex() const
	{
	return selectionIndex_;
	}

}; // namespace pixie
