// MissionDebriefState.cpp

#include "Constants.h"

#include "MissionDebriefState.h"
#include "TitleState.h"
#include "InGameState.h"

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
    //136, 200
    //w 524
    
    //missionText.SetPosition(136, 200);
    missionText.SetPosition(136 - 800, 200);
    missionText.SetWidth(524, 0);
    missionText.SetFont("Assets/MainframeBB16.fnt");
    missionText.SetText(
        "your mission\n"
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed at urna imperdiet nibh viverra varius. Morbi fringilla scelerisque tempor. Curabitur sed pulvinar felis. Quisque odio tellus, volutpat a interdum quis,\n\n"
        "arcu magna tincidunt diam, a malesuada tortor nulla sit amet sem. Maecenas sed volutpat erat. Sed id iaculis urna. Nullam eget odio eros, ac imperdiet felis.");
    missionText.SetAlpha(0);

    controller = siSpriteControllerManager->AcquireSpriteController(&missionText);
    controller->AddAction(new SpriteAction_SmoothArrival(136, 200, 4), 0.25f);
    controller->AddAction(new SpriteAction_AlphaFade(255, 0.3f), 1.5f);

    siSpriteControllerManager->ReleaseSpriteController(controller);


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

    acceptButton.SetSelectionIndex(0);
    declineButton.SetSelectionIndex(1);
    SpriteManager& sm = *siSpriteSystem->GetDefaultSpriteManager();
    sm.HighlightNext();
}

void MissionDebriefState::Update(float deltaTime)
{
    InputManager& input = *siInputManager;
    SpriteManager& sm = *siSpriteSystem->GetDefaultSpriteManager();

    if (input.WasKeyPressed(KEY_LEFT))
    {
        sm.HighlightPrevious();
    }
    else if (input.WasKeyPressed(KEY_RIGHT))
    {
        sm.HighlightNext();
    }
    else if (input.WasKeyPressed(KEY_SPACE) || input.WasKeyPressed(KEY_RETURN))
    {
        sm.SelectHighlighted();
    }
}

void MissionDebriefState::OnSpriteEvent(StringId eventId, void* userData)
{
    if (eventId == "Accept")
    {
        SwitchState(InGameState::ID, 1, 0.5);
    }
    else if (eventId == "Decline")
    {
        SwitchState(TitleState::ID, 1, 0.5);
    }
}
