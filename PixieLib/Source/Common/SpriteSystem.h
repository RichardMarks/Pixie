/**
 * \class	SpriteSystem
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	Add new sprite types for basic shapes: Rectangle and maybe line, circle etc...
 */

#ifndef __SpriteSystem_H__
#define __SpriteSystem_H__

// Includes
#include "Singleton.h"
#include "StringId.h"
#include "Array.h"

// Forward declares
class SpriteManager;
class SpriteEventListener;
class Bitmap;

// SpriteSystem
class SpriteSystem:public Singleton<SpriteSystem>
	{
	public:
		SpriteSystem();
		virtual ~SpriteSystem();

		void Update(float deltaTime);
		void Render(Bitmap& bitmap);

		SpriteManager* GetDefaultSpriteManager();
		void SetDefaultSpriteManager(SpriteManager* spriteManager);

		int GetSpriteManagerCount() const;
		SpriteManager* GetSpriteManager(int index) const;
		void AddSpriteManager(SpriteManager* spriteManager);
		void RemoveSpriteManager(SpriteManager* spriteManager);

		void UpdatePriority(SpriteManager* spriteManager);

		void AddEventListener(SpriteEventListener* listener);
		void RemoveEventListener(SpriteEventListener* listener);

		bool HasSpriteManager(SpriteManager* spriteManager) const;
	private:
		friend class SpriteManager;
		void SendEvent(SpriteManager* spriteManager, StringId event, void* userData);

	private:
		Array<SpriteManager*> spriteManagers_;

		SpriteManager* defaultSpriteManager_;

		Array<SpriteEventListener*> eventListeners_;
	};

#define siSpriteSystem SpriteSystem::GetInstance()

#endif /* __SpriteSystem_H__ */

