/**
 * \class	SpriteController
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __SpriteController_H__
#define __SpriteController_H__

// Includes
#include "Array.h"
#include "StringId.h"

// Forward declares
class Sprite;
class SpriteAction;

// SpriteController
class SpriteController
	{
	public:
		void Update(float deltaTime);

		Sprite* GetSprite() const;

		void AddAction(SpriteAction* action, float delay = 0.0f, StringId startTrigger = 0, StringId endTrigger = 0, StringId actionId = 0);

		void RemoveAction(SpriteAction* action);

		void RemoveAction(StringId actionId);

		void AddOffset(float x, float y);

		void AddVelocity(float x, float y);

		int GetActionCount() const;
	private:
		friend class SpriteControllerManager;

		SpriteController(Sprite* sprite);
		
		~SpriteController();
		
		int GetReferenceCount() const;

		void IncreaseReferenceCount();
		
		void DecreaseReferenceCount();

	private:
		Sprite* sprite_;

		int referenceCount_;

		struct ActionEntry
			{
			SpriteAction* action;
			float delay;
			StringId startTrigger;
			StringId endTrigger;
			StringId actionId;
			float timeLeftToStart;
			};

		Array<ActionEntry*> currentActions_;
		Array<ActionEntry*> queuedActions_;

		float velocityX_;
		float velocityY_;
		float offsetX_;
		float offsetY_;
		float previousOffsetX_;
		float previousOffsetY_;
	};

#endif /* __SpriteController_H__ */

