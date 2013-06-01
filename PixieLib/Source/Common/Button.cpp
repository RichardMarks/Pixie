//*** Button.cpp **

#include "Button.h"
#include "BitmapStrip.h"
#include "Bitmap.h"
#include "SpriteControllerManager.h"
#include "SpriteController.h"
#include "SpriteAction_ButtonStateCrossFade.h"


//*** Default Constructor ***

Button::Button():
	state_(State_Normal),
	enabled_(true),
	highlighted_(false),
	eventUserData_(0),
	stateCrossFadeTime_(0.25f),
	stateCrossFadeFromState_(State_Normal),
	stateCrossFadeFromAlpha_(0),
	stateCrossFadeToAlpha_(0)
	{
	}


//*** Constructor (SpriteManager) ***

Button::Button(SpriteManager* spriteManager):
	Sprite(spriteManager),
	state_(State_Normal),
	enabled_(true),
	highlighted_(false),
	eventUserData_(0),
	stateCrossFadeTime_(0.25f),
	stateCrossFadeFromState_(State_Normal),
	stateCrossFadeFromAlpha_(0),
	stateCrossFadeToAlpha_(0)
	{
	}


//*** Destructor ***

Button::~Button()
	{
	}


//*** GetBitmap ***

const Resource_BitmapStrip& Button::GetBitmap() const
	{
	if (bitmapStrips_[GetState()].GetCelCount()!=0)
		{
		return bitmapStrips_[GetState()];
		}

	if (bitmapStrips_[State_Normal].GetCelCount()!=0)
		{
		return bitmapStrips_[State_Normal];
		}

	return Sprite::GetBitmap();
	}


//*** GetBitmap ***

const Resource_BitmapStrip& Button::GetBitmap(Button::State state) const
	{
	return bitmapStrips_[state];
	}


//*** SetBitmap ***

void Button::SetBitmap(Button::State state, const Resource_BitmapStrip& bitmapStrip)
	{
	bitmapStrips_[state]=bitmapStrip;
	}


//*** GetState ***

Button::State Button::GetState() const
	{
	return state_;
	}


//*** SetState ***

void Button::SetState(Button::State state)
	{
	if (state==state_)
		{
		return;
		}

	if (SpriteControllerManager::IsInstanceCreated() && stateCrossFadeTime_ && bitmapStrips_[state].GetCelCount()!=0)
		{
		SpriteController* controller=siSpriteControllerManager->AcquireSpriteController(this);
		static StringId idButtonStateCrossFadeAction("ButtonStateCrossFadeAction");
		controller->RemoveAction(idButtonStateCrossFadeAction);
		controller->AddAction(new SpriteAction_ButtonStateCrossFade(stateCrossFadeTime_),0,0,0,idButtonStateCrossFadeAction);
		siSpriteControllerManager->ReleaseSpriteController(controller);
		}
	else
		{
		stateCrossFadeFromAlpha_=0;
		}
	stateCrossFadeFromState_=state_;
	state_=state;
	}


//*** IsEnabled ***

bool Button::IsEnabled() const
	{
	return enabled_;
	}


//*** SetEnabled ***

void Button::SetEnabled(bool enabled)
	{
	if (enabled_==enabled)
		{
		return;
		}

	enabled_=enabled;
	
	if (enabled_)
		{
		SetState(State_Normal);
		}
	else 
		{
		SetState(State_Disabled);
		}
	}


//*** GetEventId ***

StringId Button::GetEventId() const
	{
	return eventId_;
	}


//*** SetEventId ***

void Button::SetEventId(StringId eventId)
	{
	eventId_=eventId;
	}


//*** GetEventUserData ***

void* Button::GetEventUserData() const
	{
	return eventUserData_;
	}


//*** SetEventUserData ***

void Button::SetEventUserData(void* userData)
	{
	eventUserData_=userData;
	}


//*** GetCrossFadeTime ***

float Button::GetCrossFadeTime() const
	{
	return stateCrossFadeTime_;
	}


//*** SetCrossFadeTime ***

void Button::SetCrossFadeTime(float time)
	{
	stateCrossFadeTime_=time;
	}


//*** SetStateCrossFadeAlpha ***

void Button::SetStateCrossFadeAlpha(unsigned char fromAlpha, unsigned char toAlpha)
	{
	stateCrossFadeFromAlpha_=fromAlpha;
	stateCrossFadeToAlpha_=toAlpha;
	}	


//*** Render ***

void Button::Render(Bitmap& bitmap)
	{
	Sprite::Render(bitmap);

	if (!IsVisible())
		{
		return;
		}

	if (IsEnabled() && !highlighted_ && (GetState()==State_Highlighted || GetState()==State_Pressed))
		{
		SetState(State_Normal);
		}

	highlighted_=false;

	if (stateCrossFadeFromAlpha_>0)
		{
		unsigned char fromAlpha=(unsigned char)((((int)stateCrossFadeFromAlpha_)*GetAlpha())/256);
		unsigned char toAlpha=(unsigned char)((((int)stateCrossFadeToAlpha_)*GetAlpha())/256);
		GetBitmap(stateCrossFadeFromState_).Blit((int)GetCel(),bitmap,(int)(GetX()-GetOriginX()),(int)(GetY()-GetOriginY()),GetColor(),fromAlpha);
		GetBitmap().Blit((int)GetCel(),bitmap,(int)(GetX()-GetOriginX()),(int)(GetY()-GetOriginY()),GetColor(),toAlpha);
		}
	else
		{
		GetBitmap().Blit((int)GetCel(),bitmap,(int)(GetX()-GetOriginX()),(int)(GetY()-GetOriginY()),GetColor(),GetAlpha());
		}
	}


//*** OnMouseOver ***

bool Button::OnMouseOver(int x, int y, bool button, StringId& eventId, void*& userData)
	{
	if (!enabled_)
		{
		return false;
		}

	if (GetBitmap().GetCelCount()==0)
		{
		return false;
		}

	const Bitmap& bitmap=GetBitmap().GetCel((int)GetCel());
	int xp = x-(int)(GetX()-GetOriginX());
	int yp = y-(int)(GetY()-GetOriginY());
	unsigned char alpha = bitmap.GetPixelAlpha(xp,yp);
	if (alpha>128)
		{
		if (enabled_)
			{
			highlighted_=true;
			if (button)
				{
				SetState(State_Pressed);
				}
			else
				{
				SetState(State_Highlighted);
				}
			}

		eventId=eventId_;
		userData=eventUserData_;

		return true;
		}

	return false;
	}


