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

    myClearScreenRect.SetColor(0);
    myClearScreenRect.SetPriority(0);
    myClearScreenRect.SetAlpha(255);
    myClearScreenRect.SetSize(1.0f * constants::WINDOW_WIDTH, 1.0f * constants::WINDOW_HEIGHT);

    myStarsRenderer.SetCallback(InGameState::StarsRenderCallback, this);
    myStarsRenderer.SetPriority(0);

    myPlayer.SetPosition(constants::WINDOW_WIDTH * 0.5f, constants::WINDOW_HEIGHT * 0.5f);

    myStars = new Star[constants::STARFIELD_DENSITY];
    
    Random rng;

    for (unsigned i = 0; i < constants::STARFIELD_DENSITY; i++)
    {
        Star& star = myStars[i];
        star.sprite.SetBitmap("Assets/InGame/Star.pix");
        star.velocityX = 0;
        star.velocityY = 0;
        float x = (float)rng.RandomRanged(-800, 1600);
        float y = (float)rng.RandomRanged(-800, 1600);
        star.sprite.SetPosition(x, y);
        star.sprite.SetPriority(0);
        star.sprite.SetColor((unsigned short)rng.RandomRanged(0, 0xffff));
    }
}

InGameState::~InGameState()
{
    delete [] myStars;
}

void InGameState::Update(float deltaTime)
{
    myDT = deltaTime;
    InputManager& input = *siInputManager;

    if (input.WasKeyPressed(KEY_ESCAPE))
    {
        SwitchState(MissionDebriefState::ID, 1, 0.5);
    }

    if (input.IsKeyDown(KEY_UP))
    {
        myPlayer.Accelerate();
    }
    else
    {
        myPlayer.Drag();
    }
    
    if (input.IsKeyDown(KEY_LEFT))
    {
        myPlayer.YawLeft();
    }
    else if (input.IsKeyDown(KEY_RIGHT))
    {
        myPlayer.YawRight();
    }

    myPlayer.Update(deltaTime);

}

void InGameState::OnSpriteEvent(StringId eventId, void* userData)
{
    
}

void InGameState::StarsRenderCallback(Bitmap& bitmap, RenderCall* sprite, void* userData)
{
    InGameState& state = *(InGameState*)userData;

    // stars movement is in relation to the player's movement
    Player& player = state.myPlayer;

    float velX = player.GetVelocityX() * state.myDT;
    float velY = player.GetVelocityY() * state.myDT;

    Star* stars = state.myStars;
    for (unsigned i = 0; i < constants::STARFIELD_DENSITY; i++)
    {
        Star& star = stars[i];

        float px = star.sprite.GetX();
        float py = star.sprite.GetY();

        px -= velX;
        py -= velY;

        star.sprite.SetPosition(px, py);
    }
}