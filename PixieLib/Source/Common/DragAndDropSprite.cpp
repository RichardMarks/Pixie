//*** DragAndDropSprite.cpp **

#include "DragAndDropSprite.h"
#include "BitmapStrip.h"
#include "Bitmap.h"
#include "SpriteControllerManager.h"
#include "SpriteController.h"
#include "SpriteAction_ButtonStateCrossFade.h"

namespace pixie {

//** Constructor ***

DragAndDropSprite::DragAndDropSprite():
	dragging_(false),
	dragOffsetX_(0),
	dragOffsetY_(0)
	{

	}


//** Constructor ***

DragAndDropSprite::DragAndDropSprite(SpriteManager* spriteManager):
	Button(spriteManager),
	dragging_(false),
	dragOffsetX_(0),
	dragOffsetY_(0)
	{

	}


//*** OnMouseOver ***

bool DragAndDropSprite::OnMouseOver(int x, int y, bool button, StringId& eventId, void*& userData)
	{
	if (!IsEnabled())
		{
		dragging_=false;
		return false;
		}

	if (dragging_)
		{
		SetPosition((float)x-dragOffsetX_,(float)y-dragOffsetY_);
		}


	const BitmapStrip& bitmapStrip=GetBitmap();
	if (bitmapStrip.GetCelCount()!=0)
		{
		int currentCel=(int)GetCel();
		const Bitmap& bitmap=bitmapStrip.GetCel(currentCel);
		if (bitmap.GetPixelAlpha(x-(int)(GetX()+GetOriginX()),y-(int)(GetY()+GetOriginY()))>128)
			{
			if (IsEnabled())
				{
			//	highlighted_=true;
				if (button)
					{
					SetState(State_Pressed);
					if (!dragging_)
						{
						dragOffsetX_=x-GetX()+GetOriginX();
						dragOffsetY_=y-GetY()+GetOriginY();
						}
					dragging_=true;
					}
				else
					{
					SetState(State_Highlighted);
					dragging_=false;
					}
				}

			eventId=GetEventId();
			userData=GetEventUserData();

			return true;
			}
		}

	return false;
	}

}; // namespace pixie
