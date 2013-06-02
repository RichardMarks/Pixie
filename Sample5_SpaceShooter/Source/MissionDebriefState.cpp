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
    missionBox.SetPosition(-800, 0);

    SpriteController* controller = siSpriteControllerManager->AcquireSpriteController(&missionBox);
    controller->AddAction(new SpriteAction_SmoothArrival(0, 0, 4), 0.25f);
    siSpriteControllerManager->ReleaseSpriteController(controller);

    // TODO: need to FGT a 16px font for mission text

    declineButton.SetEventId("Decline");
    declineButton.SetBitmap(Button::State_Normal, "Assets/MissionDebrief/Decline_Normal.pix");
    declineButton.SetBitmap(Button::State_Highlighted, "Assets/MissionDebrief/Decline_Highlighted.pix");
    declineButton.SetBitmap(Button::State_Pressed, "Assets/MissionDebrief/Decline_Highlighted.pix");

    acceptButton.SetEventId("Accept");
    acceptButton.SetBitmap(Button::State_Normal, "Assets/MissionDebrief/Accept_Normal.pix");
    acceptButton.SetBitmap(Button::State_Highlighted, "Assets/MissionDebrief/Accept_Highlighted.pix");
    acceptButton.SetBitmap(Button::State_Pressed, "Assets/MissionDebrief/Accept_Highlighted.pix");

    declineButton.UseBoundingBox(true);
    acceptButton.UseBoundingBox(true);
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