// Main.cpp

#include "Pixie.h"
#include <windows.h>

#include "Constants.h"
#include "TitleState.h"
#include "MissionDebriefState.h"
#include "InGameState.h"

using namespace pixie;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DefaultWinMain(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
    GameHelper gameHelper("Space Shooter");
    Screen screen(constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT);
    screen.MatchWindowToScreenSize();
	FrameTime time;

    InputManager& input = *InputManager::GetInstance();
    GameStateManager& gsm = *GameStateManager::GetInstance();

    TitleState::Register(gsm);
    MissionDebriefState::Register(gsm);
    InGameState::Register(gsm);

    input.CreateCursor(0, "Assets/PlanetCursor.pix", 16, 16);

    //gsm.SwitchState(TitleState::ID);
    //gsm.SwitchState(MissionDebriefState::ID);
    gsm.SwitchState(InGameState::ID);

    bool running = true;

	while (running)
	{
	    float deltaTime = time.Update();
	    running = gameHelper.UpdateGame(deltaTime, screen);

#if 0
        if (input.WasKeyPressed(KEY_ESCAPE))
		{
		    running = false;
		}
#endif
	}

    return 0;
}
