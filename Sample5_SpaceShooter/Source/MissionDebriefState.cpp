// MissionDebriefState.cpp

#include "Constants.h"

#include "MissionDebriefState.h"
#include "TitleState.h"


const char* MissionDebriefState::ID = "Mission Debrief State";

void MissionDebriefState::Register(GameStateManager& gsm) 
{
    gsm.RegisterGameState(MissionDebriefState::ID, MissionDebriefState::CreateInstance);
}

GameState* MissionDebriefState::CreateInstance() 
{ 
    return new MissionDebriefState; 
}

MissionDebriefState::MissionDebriefState()
{
    InputManager::GetInstance()->SetCursor(0);

    bg.SetBitmap("Assets/MenuBackground.pix");

    heading.SetBitmap("Assets/MissionDebrief/Heading.pix");

    missionBox.SetBitmap("Assets/MissionDebrief/MissionBox.pix");
    //float cx = (constants::WINDOW_WIDTH - missionBox.GetBitmap().GetWidth()) * 0.5f;
    //float cy = (constants::WINDOW_HEIGHT - missionBox.GetBitmap().GetHeight()) * 0.5f;
    //missionBox.SetPosition(cx, cy);

#if 0
    SpriteController* controller = siSpriteControllerManager->AcquireSpriteController(&missionBox);
    controller->AddAction(new SpriteAction_SmoothArrival(0, 0, 4), 0.5f);
    siSpriteControllerManager->ReleaseSpriteController(controller);
#endif
    // TODO: need to FGT a 16px font for mission text

    declineButton.SetEventId("Decline");
    declineButton.SetBitmap(Button::State_Normal, "Assets/MissionDebrief/Decline_Normal.pix");
    declineButton.SetBitmap(Button::State_Highlighted, "Assets/MissionDebrief/Decline_Highlighted.pix");
    declineButton.SetBitmap(Button::State_Pressed, "Assets/MissionDebrief/Decline_Highlighted.pix");

    acceptButton.SetEventId("Accept");
    acceptButton.SetBitmap(Button::State_Normal, "Assets/MissionDebrief/Accept_Normal.pix");
    acceptButton.SetBitmap(Button::State_Highlighted, "Assets/MissionDebrief/Accept_Highlighted.pix");
    acceptButton.SetBitmap(Button::State_Pressed, "Assets/MissionDebrief/Accept_Highlighted.pix");
}

void MissionDebriefState::Update(float deltaTime)
{

}

void MissionDebriefState::OnSpriteEvent(StringId eventId, void* userData)
{
    if (eventId == "Accept")
    {
    }
    else if (eventId == "Decline")
    {
        SwitchState(TitleState::ID, 1, 0.5);
    }
}