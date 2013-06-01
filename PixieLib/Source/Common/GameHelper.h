/**
 * \class	GameHelper
 * 
 * \ingroup	helpers
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __GameHelper_H__
#define __GameHelper_H__

// Includes
#include "ArchiveManager.h"
#include "InputManager.h"
#include "Audio.h"
#include "MusicManager.h"
#include "SpriteSystem.h"
#include "SpriteControllerManager.h"
#include "OldResourceManager.h"
#include "GameStateManager.h"

// Forward declares
class Screen;

// GameHelper
class GameHelper
	{
	public:
		GameHelper(const char* applicationName);
		~GameHelper();

		bool UpdateGame(float deltaTime, Screen& screen);
	private:
		// Create all the global systems we need - we just create them on the stack like this, as this will keep them 
		// around until the game object is shut down, and since they are singletons, they are globally accessible anyway
		ArchiveManager archiveManager_;
		InputManager inputManager_;
		Audio audio_;
		MusicManager musicManager_;
		SpriteSystem spriteSystem_;
		SpriteControllerManager spriteControllerManager_;
		OldResourceManager resourceManager_;
		GameStateManager gameStateManager_;

	};

#endif /* __GameHelper_H__ */
