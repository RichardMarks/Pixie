
#include "Pixie.h"
#include <windows.h>

// Groups all the data we need for a ball - it's just the sprite and the speeds
struct Ball
	{
	Sprite sprite;	// A sprite instance for the ball - so we can display it
	float speedX;	// The current horizontal speed of the ball
	float speedY;	// The current vertical speed of the ball
	float animationSpeed;	// The current animation speed of the ball
	};

// Each time we create a new ball, we'll call this function to initialize it
void SetupBall(Ball& ball)
	{
	ball.sprite.SetBitmap("Ball.pix");	// Specify which picture it should display
	ball.sprite.SetOrigin(40,40); // Set the sprite to have its center at the bitmaps center
	ball.sprite.SetX(Rand()%528+56.0f);	// Place the ball on a random position (horizontally)
	ball.sprite.SetY(Rand()%368+56.0f);	// Place the ball on a random position (vertically)
	unsigned char r=(unsigned char)(64+Rand()%192); // Generate a random color for the ball,
	unsigned char g=(unsigned char)(64+Rand()%192); // by assigning a value in range 64-255
	unsigned char b=(unsigned char)(64+Rand()%192); // to the red, green and blur components.
	unsigned short color=RGB16(r,g,b); // Create a 16-bit color value from the r/g/b values
	ball.sprite.SetColor(color); // Set the sprite to use the color
	ball.speedX=Rand()%5+1.0f; // Pick a random horizontal speed for the ball in range 1-5
	ball.speedY=Rand()%4+1.0f; // Pick a random vertical speed for the ball in range 1-4
	if (Rand()%1==0) { ball.speedX=-ball.speedX; } // 50% that the x-speed should be negative
	if (Rand()%1==0) { ball.speedY=-ball.speedY; } // 50% that the y-speed should be negative
	ball.animationSpeed=1; // Animation speed should always be 1, to look nice, but with a
	if (Rand()%1==0) { ball.animationSpeed=-ball.animationSpeed; } // 50% chance of being -1
	}

// Every frame update, we'll call this method for each ball, to do all the updates on it
void UpdateBall(Ball& ball)
	{
	float ballX=ball.sprite.GetX();	// Get a local working variable for the balls x-position
	float ballY=ball.sprite.GetY(); // and one for the y-position as well
	float cel=ball.sprite.GetCel(); // and one for which frame of the animation we're at

	ballX+=ball.speedX; // Move the ball horizontally by its current horizontal speed
	ballY+=ball.speedY; // Move the ball vertically by its current vertical speed
	cel+=ball.animationSpeed; // Update the animation by the current animation speed

	if (ballX>=584) // If the ball is at the right edge, we need to bounce off it
		{
		ball.speedX=-ball.speedX; // So change the horizontal direction
		ballX=584;	// And make sure the ball isn't moved past the edge
		ball.animationSpeed=-ball.animationSpeed; // Reverse the animation direction
		}

	if (ballX<=56) // If the ball is at the left edge, we need to bounce off it
		{
		ball.speedX=-ball.speedX; // So change the horizontal direction
		ballX=56; // And make sure the ball isn't moved past the edge
		ball.animationSpeed=-ball.animationSpeed; // Reverse the animation direction
		}

	if (ballY>=434) // If the ball is at the bottom edge, we need to bounce off it
		{
		ball.speedY=-ball.speedY; // So change the vertical direction
		ballY=434; // And make sure the ball isn't moved past the edge
		}

	if (ballY<56) // If the ball is at the top edge, we need to bounce off it
		{
		ball.speedY=-ball.speedY; // So change the vertical direction
		ballY=56;	// And make sure the ball isn't moved past the edge
		}

	ball.sprite.SetPosition(ballX,ballY); // Update the sprite position from the locals
	ball.sprite.SetCel(cel); // And update the animation as well
	}

		
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
	// Setup the Pixie system to be using Windows
	DefaultWinMain(hInstance,hPrevInstance,lpCmdLine,nCmdShow);

	// The easiest way to initialize and run Pixie is by declaring a GameHelper object to do it
	GameHelper gameHelper("Multi Balls"); // "Multi Balls" will be shown in the title bar
	
	// We need to set up the screen, and the easiest way is by declaring a Screen object.
	Screen screen(640,480,false);	// 640,480 is the resolution we want our game to be using

	// For every run through the game loop, we need to calculate the time elapsed
	FrameTime time;	// A FrameTime object is the easiest way to measure the frames time

	// We can declare any custom variables here
	Sprite background;	// Create a sprite for the background - it will just sit still
	background.SetBitmap("Background.pix");	// And specify which picture it should display

	Array<Ball> balls;	// Use a Pixie array (which can grow and shrink) to store the balls
	for (int i=0; i<4; i++) // Start of with 4 balls
		{
		Ball newBall;	 // Create a new ball 
		SetupBall(newBall); // Initialize it
		balls.Add(newBall); // And add it to the array
		}

	Sprite border;	// Create a sprite for the border - it will go on in front of the balls
	border.SetBitmap("border.pix");	// And specify which picture it should display
	border.SetPriority(1); // A higher priority makes the border drawn in front of the balls

	StaticText ballCounter;	// Create a text to display the number of balls
	ballCounter.SetPosition(300,10); // And position it at top-center of the screen
	ballCounter.SetPriority(2); // And make it draw in front of the border sprite

	// The main game loop, which updates a frame of the game
	bool isGameRunning=true; // This flag is true while the game is still running

	while (isGameRunning)
		{
		float deltaTime=time.Update(); // Get the time elapsed for the last frame

		// Run Pixie for one game frame, and check the result to see if we should keep running
		isGameRunning=gameHelper.UpdateGame(deltaTime,screen);

		// We can do any additional processing we like here
		if (siInputManager->IsKeyDown(KEY_UP))	// Check if the player pressed the UP key
			{
			Ball newBall;	 // Create a new ball 
			SetupBall(newBall); // Initialize it
			balls.Add(newBall); // And add it to the array
			}

		if (siInputManager->IsKeyDown(KEY_DOWN)) // Check if the player pressed the DOWN key
			{
			if (balls.GetItemCount()>0) // We only want to remove a ball if there's any left
				{
				Ball& lastBall=balls.Get(balls.GetItemCount()-1); // Get the last ball-entry
				lastBall.sprite.SetVisible(false); // Stop displaying it
				balls.RemoveLast(); // And remove it from the array
				}
			}

		char text[32]; // A temporary string buffer to hold the ball-count string
		SNPrintF(text,32,"Balls: %d",balls.GetItemCount()); // Generate the ball-count string
		ballCounter.SetText(text); // And update the ball-count sprite with the string

		for (int i=0; i<balls.GetItemCount(); i++) // Loop through all the balls we have
			{
			UpdateBall(balls.Get(i)); // And update each one of them
			}

		if (siInputManager->WasKeyPressed(KEY_ESCAPE))	// Check if user pressed Escape key
			{
			isGameRunning=false; // Flag the game to exit
			}
		}

	// Return the result (always 0, as this was a controlled shutdown)
	return 0;
	}
