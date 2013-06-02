//*** DragAndDropTarget.cpp **

#include "DragAndDropTarget.h"
#include "BitmapStrip.h"
#include "SpriteManager.h"
#include "SpriteControllerManager.h"
#include "Resource_BitmapStrip.h"

namespace pixie {
//*** Constructor ***

DragAndDropTarget::DragAndDropTarget()
	{
	}


//*** Constructor ***

DragAndDropTarget::DragAndDropTarget(SpriteManager* spriteManager):
	Sprite(spriteManager)
	{
	}


//*** Destructor ***

DragAndDropTarget::~DragAndDropTarget()
	{
	}
}; // namespace pixie


