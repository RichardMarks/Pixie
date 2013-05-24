/**
 * \class	SpriteControllerManager
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __SpriteControllerManager_H__
#define __SpriteControllerManager_H__

// Includes
#include "Singleton.h"
#include "Array.h"

// Forward declares
class SpriteController;
class Sprite;

// SpriteControllerManager
class SpriteControllerManager:public Singleton<SpriteControllerManager>
	{
	public:
		SpriteControllerManager();
		virtual ~SpriteControllerManager();

		void Update(float deltaTime);

		SpriteController* AcquireSpriteController(Sprite* sprite);
		void ReleaseSpriteController(SpriteController* spriteController);

		int GetActionCount(Sprite* sprite) const;

		void SpriteDeleted(Sprite* sprite);
	private:
		Array<SpriteController*> spriteControllers_;
	};

#define siSpriteControllerManager SpriteControllerManager::GetInstance()

#endif /* __SpriteControllerManager_H__ */

