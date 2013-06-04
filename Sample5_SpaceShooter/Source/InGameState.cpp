// InGameState.cpp

#include "InGameState.h"
#include "MissionDebriefState.h"
#include "Constants.h"
#include "GameMaths.h"

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
    gamemaths::tables::Init();

    InputManager::GetInstance()->HideCursor();

    clearScreenRect.SetColor(0);
    clearScreenRect.SetPriority(0);
    clearScreenRect.SetAlpha(255);
    clearScreenRect.SetSize(1.0f * constants::WINDOW_WIDTH, 1.0f * constants::WINDOW_HEIGHT);

    player.SetPosition(constants::WINDOW_WIDTH * 0.5f, constants::WINDOW_HEIGHT * 0.5f);

}

void InGameState::Update(float deltaTime)
{
    InputManager& input = *siInputManager;

    if (input.WasKeyPressed(KEY_ESCAPE))
    {
        SwitchState(MissionDebriefState::ID, 1, 0.5);
    }

    if (input.IsKeyDown(KEY_UP))
    {
        player.Accelerate();
    }
    else
    {
        player.Drag();
    }
    
    if (input.IsKeyDown(KEY_LEFT))
    {
        player.YawLeft();
    }
    else if (input.IsKeyDown(KEY_RIGHT))
    {
        player.YawRight();
    }

    player.Update(deltaTime);

}

void InGameState::OnSpriteEvent(StringId eventId, void* userData)
{
    
}
