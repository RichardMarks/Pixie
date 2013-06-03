// InGameState.cpp

#include "InGameState.h"
#include "MissionDebriefState.h"

using namespace pixie;

const char* InGameState::ID = "In Game State";

void InGameState::Register(GameStateManager& gsm) 
{
    gsm.RegisterGameState(InGameState::ID, InGameState::CreateInstance);
}

GameState* InGameState::CreateInstance() 
{ 
    return new InGameState; 
}

InGameState::InGameState()
{
    InputManager::GetInstance()->HideCursor();

    clearScreenRect.SetColor(0);
    clearScreenRect.SetPriority(0);
    clearScreenRect.SetAlpha(255);
}

void InGameState::Update(float deltaTime)
{
    InputManager& input = *siInputManager;

    if (input.WasKeyPressed(KEY_ESCAPE))
    {
        SwitchState(MissionDebriefState::ID, 1, 0.5);
    }


}

void InGameState::OnSpriteEvent(StringId eventId, void* userData)
{
    
}
