// Main.cpp

#include "Pixie.h"
#include <windows.h>

#include "Constants.h"
#include "TitleState.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DefaultWinMain(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
    GameHelper gameHelper("Space Shooter");
    Screen screen(constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT, false);
	FrameTime time;

    InputManager& input = *InputManager::GetInstance();
    GameStateManager& gsm = *GameStateManager::GetInstance();

    TitleState::Register(gsm);

    gsm.SwitchState(TitleState::ID);

    bool running = true;

	while (running)
	{
	    float deltaTime = time.Update();
	    running = gameHelper.UpdateGame(deltaTime, screen);

        if (input.WasKeyPressed(KEY_ESCAPE))
		{
		    running = false;
		}
	}

    return 0;
}
