//*** GameStateManager.cpp ***

#include "GameStateManager.h"
#include "Debug.h"
#include "Asset.h"
#include "GameState.h"
#include "ColorHelper.h"
#include "HashTableIterator.h"


//*** Constructor ***

GameStateManager::GameStateManager():
	state_("STATE_NOT_SET"),
	stateInstance_(0),
	transitioningOut_(false),
	transitioningIn_(false),
	transitionInTime_(0),
	transitionInTimeElapsed_(0),
	transitionOutTime_(0),
	transitionOutTimeElapsed_(0),
	newTransitionInTime_(0),
	exitRequested_(false),
	exitFlag_(false)
	{
	}


//*** Destructor ***

GameStateManager::~GameStateManager()
	{
	ExitCurrentState();
	}


//*** SetState ***

void GameStateManager::SetState(StringId newState)
	{
	// Leave current state
	if (stateInstance_)
		{
		delete stateInstance_;
		stateInstance_=0;
		state_=StringId(0);
		}

	// If there's no new state, we're done
	if (newState.GetString()==0)
		{
		return;
		}


	// Change current state
	typedef GameState* (*CreateFunction)();
	HashTableIterator<HashTableKey_StringId,CreateFunction> it(gameStates_);
	CreateFunction createFunction=0;
	if (it.Find(HashTableKey_StringId(newState)))
		{
		createFunction=it.GetCurrent();
		}
	Assert(createFunction,"State not recognized!");
	if (!createFunction)
		{
		return;
		}
	state_=newState;
	stateInstance_=createFunction();
	}


//*** GetState ***

StringId GameStateManager::GetState()
	{
	return state_;
	}


//*** Update ***

void GameStateManager::Update(float deltaTime)
	{

	// Update transition out
	if (transitioningOut_)
		{
		if (transitionOutTimeElapsed_>=transitionOutTime_)
			{
			transitioningOut_=false;
			}
		else
			{
			transitionOutTimeElapsed_+=deltaTime;
			if (transitionOutTimeElapsed_>=transitionOutTime_)
				{
				transitionOutTimeElapsed_=transitionOutTime_;
				}
			}
		}

	// Update transition in
	if (transitioningIn_)
		{
		if (transitionInTimeElapsed_>=transitionInTime_)
			{
			transitioningIn_=false;
			}
		else
			{
			transitionInTimeElapsed_+=deltaTime;
			if (transitionInTimeElapsed_>=transitionInTime_)
				{
				transitionInTimeElapsed_=transitionInTime_;
				}
			}
		}

	// Handle exit requests
	if (exitRequested_ && !transitioningOut_ && !transitioningIn_)
		{
		ExitCurrentState();
		exitFlag_=true;
		return;
		}

	// Perfrom state switch, if requested
	static StringId empty;
	if (stateToSwitchTo_!=empty)
		{
		if (!transitioningOut_ && !transitioningIn_)
			{
			StringId newState=stateToSwitchTo_;
			stateToSwitchTo_=empty;
			SetState(newState);
			if (newTransitionInTime_>0)
				{
				transitionInTime_=newTransitionInTime_;
				transitionInTimeElapsed_=0;
				transitioningIn_=true;
				}
			}
		}

	// Update state instance
	if (stateInstance_)
		{ 
		stateInstance_->Update(deltaTime);
		}

	}


//*** RegisterGameState ***

void GameStateManager::RegisterGameState(StringId id, CreateFunction createFunction)
	{
	HashTableIterator<HashTableKey_StringId,CreateFunction> it(gameStates_);
	if (!it.Find(HashTableKey_StringId(id)))
		{
		gameStates_.Insert(HashTableKey_StringId(id),createFunction);
		}
	else
		{
		Assert(false,"State already registered!");
		}
	}


//*** SwitchState ***

void GameStateManager::SwitchState(StringId state, float transitionOutTime, float transitionInTime)
	{
	stateToSwitchTo_=state;

	newTransitionInTime_=transitionInTime;

	if (!transitioningOut_ && transitionOutTime>0)
		{
		transitioningOut_=true;
		transitionOutTime_=transitionOutTime;
		transitionOutTimeElapsed_=0;
		if (transitioningIn_)
			{
			transitioningIn_=false;
			transitionOutTimeElapsed_=(transitionInTime_-transitionInTimeElapsed_);
			}
		}

	}


//*** ExitCurrentState ***

void GameStateManager::ExitCurrentState()
	{
	if (stateInstance_)
		{
		delete stateInstance_;
		stateInstance_=0;
		state_=StringId(0);
		}
	}


//*** IsTransitioning ***

bool GameStateManager::IsTransitioning()
	{
	return transitioningIn_ || transitioningOut_;
	}


//*** IsTransitioningOut ***

bool GameStateManager::IsTransitioningOut()
	{
	return !transitioningIn_ && transitioningOut_;
	}


//*** IsTransitioningIn ***

bool GameStateManager::IsTransitioningIn()
	{
	return transitioningIn_ && !transitioningOut_;
	}


//*** GetTransitionValue ***

float GameStateManager::GetTransitionValue()
	{
	if (IsTransitioningIn())
		{
		return transitionInTimeElapsed_/transitionInTime_;
		}
	else if (IsTransitioningOut())
		{
		return transitionOutTimeElapsed_/transitionOutTime_;
		}
	else
		{
		return 0;
		}
	}


//*** ExitApplication ***

void GameStateManager::ExitApplication()
	{
	exitRequested_=true;
	}
	

//*** IsExitFlagSet ***

bool GameStateManager::IsExitFlagSet()
	{
	return exitFlag_;
	}
