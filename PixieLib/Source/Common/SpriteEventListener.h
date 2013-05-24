/**
 * \class	SpriteEventListener
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */
 
#ifndef __SpriteEventListener_H__
#define __SpriteEventListener_H__

// Includes
#include "StringId.h"
#include "SpriteSystem.h"

// Forward declares

// SpriteEventListener
class SpriteEventListener
	{
	public:	
		SpriteEventListener();

		virtual ~SpriteEventListener();

		virtual void OnSpriteEvent(StringId event, void* userData) = 0;

	};



#endif /* __SpriteEventListener_H__ */
