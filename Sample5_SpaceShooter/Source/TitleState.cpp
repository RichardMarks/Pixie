// TitleState.cpp

#include "TitleState.h"
#include "MissionDebriefState.h"

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
    InputManager::GetInstance()->SetCursor(0);

    bg.SetBitmap("Assets/MenuBackground.pix");

    startButton.SetEventId("Start");
    startButton.SetBitmap(Button::State_Normal, "Assets/Title/Start_Normal.pix");
    startButton.SetBitmap(Button::State_Highlighted, "Assets/Title/Start_Highlighted.pix");
    startButton.SetBitmap(Button::State_Pressed, "Assets/Title/Start_Highlighted.pix");

    continueButton.SetEventId("Continue");
    continueButton.SetBitmap(Button::State_Normal, "Assets/Title/Continue_Normal.pix");
    continueButton.SetBitmap(Button::State_Highlighted, "Assets/Title/Continue_Highlighted.pix");
    continueButton.SetBitmap(Button::State_Pressed, "Assets/Title/Continue_Highlighted.pix");

    quitButton.SetEventId("Quit");
    quitButton.SetBitmap(Button::State_Normal, "Assets/Title/Quit_Normal.pix");
    quitButton.SetBitmap(Button::State_Highlighted, "Assets/Title/Quit_Highlighted.pix");
    quitButton.SetBitmap(Button::State_Pressed, "Assets/Title/Quit_Highlighted.pix");
}

void TitleState::Update(float deltaTime)
{

}

void TitleState::OnSpriteEvent(StringId eventId, void* userData)
{
    if (eventId == "Start")
    {
        SwitchState(MissionDebriefState::ID, 1, 0.5);
    }
    else if (eventId == "Continue")
    {
    }
    else if (eventId == "Quit")
    {
        siGameStateManager->ExitApplication();
    }
}
