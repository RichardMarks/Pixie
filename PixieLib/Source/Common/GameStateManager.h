/**
 * \class	GameStateManager
 * 
 * \ingroup	core
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __GameStateManager_H__
#define __GameStateManager_H__

// Includes
#include "Singleton.h"
#include "Array.h"
#include "HashTable.h"
#include "HashTableKey_StringId.h"

// Forward declares
class GameState;
class GameStateDefinition;

// GameStateManager
class GameStateManager:public Singleton<GameStateManager>
	{
	public:
		GameStateManager();
		virtual ~GameStateManager();
		
		void Update(float deltaTime);

		void SetState(StringId state);
		StringId GetState();

		typedef GameState* (*CreateFunction)();
		void RegisterGameState(StringId id, CreateFunction createFunction);
		
		void ExitCurrentState();

		void SwitchState(StringId state, float transitionOutTime = 0, float transitionInTime = 0);

		bool IsTransitioning();
		bool IsTransitioningOut();
		bool IsTransitioningIn();
		float GetTransitionValue();

		void ExitApplication();
		bool IsExitFlagSet();

	private:
		friend class GameState;
		GameState* CreateGameState(StringId id);

	private:
		bool exitRequested_;
		bool exitFlag_;

		StringId state_;
		GameState* stateInstance_;
		StringId stateToSwitchTo_;
		
		bool transitioningOut_;
		bool transitioningIn_;
		float transitionInTime_;
		float transitionInTimeElapsed_;
		float transitionOutTime_;
		float transitionOutTimeElapsed_;
		float newTransitionInTime_;

		HashTable<HashTableKey_StringId,CreateFunction> gameStates_;
	};

#define siGameStateManager GameStateManager::GetInstance()

#endif /* __GameStateManager_H__ */
