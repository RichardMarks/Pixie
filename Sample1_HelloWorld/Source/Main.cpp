
#include "Pixie.h"
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
	// Setup the Pixie system to be using Windows
	DefaultWinMain(hInstance,hPrevInstance,lpCmdLine,nCmdShow);

	// The easiest way to initialize and run Pixie is by declaring a GameHelper object to do it
	GameHelper gameHelper("HelloWorld"); // "HelloWorld" will be shown in the window title bar
	
	// We need to set up the screen, and the easiest way is by declaring a Screen object.
	Screen screen(640,480);	// 640,480 is the resolution we want our game to be using
	//screen.MatchWindowToScreenSize();

	// For every run through the game loop, we need to calculate the time elapsed
	FrameTime time;	// A FrameTime object is the easiest way to measure the frames time

	// We can declare any custom variables here
	StaticText helloWorld;	// This actually creates a type of Sprite which can show text
	helloWorld.SetText("Hello World");	// And this specifies which string it should show

	// The main game loop, which updates a frame of the game
	bool isGameRunning=true; // This flag is true while the game is still running

	while (isGameRunning)
		{
		float deltaTime=time.Update(); // Get the time elapsed for the last frame

		// Run Pixie for one game frame, and check the result to see if we should keep running
		isGameRunning=gameHelper.UpdateGame(deltaTime,screen);

		// We can do any additional processing we like here
		if (siInputManager->WasKeyPressed(KEY_ESCAPE))	// Check if user pressed Escape key
			{
			isGameRunning=false; // Flag the game to exit
			}
		}

	// Return the result (always 0, as this was a controlled shutdown)
	return 0;
	}
