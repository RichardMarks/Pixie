//*** GameState.cpp ***

#include "GameState.h"
#include "GameStateManager.h"



//*** Destructor ***

GameState::~GameState()
	{

	}


//*** SwitchState ***

void GameState::Update(float deltaTime)
	{

	}


//*** SwitchState ***

void GameState::SwitchState(StringId state,  float transitionOutTime, float transitionInTime)
	{
	siGameStateManager->SwitchState(state,transitionOutTime,transitionInTime);
	}

