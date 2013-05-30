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
    menuText.SetFont("MainframeBB30.fnt");
    menuText.SetText("Testing Pixie Font System");
}

void TitleState::Update(float deltaTime)
{

}
