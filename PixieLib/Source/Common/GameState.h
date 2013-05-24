/**
 * \class	GameState
 * 
 * \ingroup	core
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __GameState_H__
#define __GameState_H__

// Includes
#include "GameStateManager.h"

// Forward declares


// GameState
class GameState
	{
	public:
		virtual ~GameState();

		virtual void Update(float deltaTime);

		void SwitchState(StringId state,  float transitionOutTime = 0, float transitionInTime = 0);
	};


// Helper macros for creating gamestates
#define GAMESTATE_IMPLEMENTATION(className)							\
	private:														\
		static GameState* Create##className##Instance()				\
			{														\
			return new className;									\
			}														\
	public:															\
		static void Register()										\
			{														\
			GameStateManager::GetInstance()->RegisterGameState(		\
							StringId(#className),					\
							Create##className##Instance);	\
			}														\

#endif /* __GameState_H__ */
