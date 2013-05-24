/**
 * \class	DragAndDropTarget
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __DragAndDropTarget_H__
#define __DragAndDropTarget_H__

// Includes
#include "Sprite.h"

// Forward declares


// DragAndDropTarget
class DragAndDropTarget:public Sprite
	{
	public:
		DragAndDropTarget();

		DragAndDropTarget(SpriteManager* spriteManager);
		
		virtual ~DragAndDropTarget();

		enum State
			{
			State_Normal,
			State_Highlighted,
			State_Disabled,
			
			State_Max_Count,
			};

	private:
		
	};

#endif /* __DragAndDropTarget_H__ */

