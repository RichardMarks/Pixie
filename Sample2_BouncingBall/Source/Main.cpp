
#include "Pixie.h"
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
	// Setup the Pixie system to be using Windows
	DefaultWinMain(hInstance,hPrevInstance,lpCmdLine,nCmdShow);

	// The easiest way to initialize and run Pixie is by declaring a GameHelper object to do it
	GameHelper gameHelper("Bouncing Ball"); // "Bouncing Ball" will be shown in the title bar
	
	// We need to set up the screen, and the easiest way is by declaring a Screen object.
	Screen screen(640,480);	// 640,480 is the resolution we want our game to be using

	// For every run through the game loop, we need to calculate the time elapsed
	FrameTime time;	// A FrameTime object is the easiest way to measure the frames time

	// We can declare any custom variables here
	Sprite background;	// Create a sprite for the background - it will just sit still
	background.SetBitmap("Background.png");	// And specify which picture it should display

	Sprite ball; // Create a sprite for the ball - we'll be moving this one around
	ball.SetBitmap("Ball.png");	// And specify which picture it should display
	ball.SetOrigin(40,40); // This tells the sprite to have its center at the bitmaps center
	float ballX_=320; // We'll use this variable to track the X-position of the ball
	float ballY_=240; // We'll use this variable to track the Y-position of the ball
	float speedX_=3; // This keeps track of the current speed (and direction) of the ball
	float speedY_=3; // And so does this, but for the vertical direction rather than horizontal

	// The main game loop, which updates a frame of the game
	bool isGameRunning=true; // This flag is true while the game is still running

	while (isGameRunning)
		{
		float deltaTime=time.Update(); // Get the time elapsed for the last frame

		// Run Pixie for one game frame, and check the result to see if we should keep running
		isGameRunning=gameHelper.UpdateGame(deltaTime,screen);

		// We can do any additional processing we like here
		ball.SetPosition(ballX_,ballY_); // Set the ball-sprites position to the ball position
		
		ballX_+=speedX_; // Move the ball horizontally by its current horizontal speed
		ballY_+=speedY_; // Move the ball vertically by its current vertical speed

		if (ballX_>=584) // If the ball is at the right edge, we need to bounce off it
			{
			speedX_=-speedX_; // So change the horizontal direction
			ballX_=584;	// And make sure the ball isn't moved past the edge
			}

		if (ballX_<=56) // If the ball is at the left edge, we need to bounce off it
			{
			speedX_=-speedX_; // So change the horizontal direction
			ballX_=56;
			}

		if (ballY_>=424) // If the ball is at the bottom edge, we need to bounce off it
			{
			speedY_=-speedY_; // So change the vertical direction
			ballY_=424;
			}

		if (ballY_<56) // If the ball is at the top edge, we need to bounce off it
			{
			speedY_=-speedY_; // So change the vertical direction
			ballY_=56;	// And make sure the ball isn't moved past the edge
			}

		if (siInputManager->WasKeyPressed(KEY_ESCAPE))	// Check if user pressed Escape key
			{
			isGameRunning=false; // Flag the game to exit
			}
		}

	// Return the result (always 0, as this was a controlled shutdown)
	return 0;
	}
