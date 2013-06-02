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
namespace pixie {

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

}; // namespace pixie

#endif /* __DragAndDropTarget_H__ */
