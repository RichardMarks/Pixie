// TitleState.cpp

#include "TitleState.h"

const char* TitleState::ID = "Title State";

void TitleState::Register(GameStateManager& gsm) 
{
    gsm.RegisterGameState(TitleState::ID, TitleState::CreateInstance);
}

GameState* TitleState::CreateInstance() 
{ 
    return new TitleState; 
}

TitleState::TitleState()
{
    titleBg.SetBitmap("titlebg.png");
}

void TitleState::Update(float deltaTime)
{

}
