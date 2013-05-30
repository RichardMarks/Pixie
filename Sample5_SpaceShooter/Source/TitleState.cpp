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
    menuText.SetFont("MainframeBB30_ds.fnt");
    menuText.SetText("start");
    //menuText.SetColor(0x0);
    menuText.SetPosition(285, 337);
}

void TitleState::Update(float deltaTime)
{

}
