//*** GameHelper.cpp ***

#include "GameHelper.h"

#include "Platform_OS.h"
#include "Screen.h"
#include "ColorHelper.h"
#include "Bitmap.h"

#include "AudioFormat_OGG.h"
#include "AudioFormat_YM.h"
#include "AudioFormat_MOD.h"
#include "ImageFormat_TGA.h"
#include "ImageFormat_GIF.h"
#include "ImageFormat_JPG.h"
#include "ImageFormat_PNG.h"


//*** Constructor ***

GameHelper::GameHelper(const char* applicationName)
	{
	// Set the window/application title
	if (Platform::GetPlatform_OS())
		{
		Platform::GetPlatform_OS()->SetApplicationName(applicationName);
		}

	// Register the sound formats we will be using - better register all of them, as we want this game to be easy to 
	// mod, and we don't know which formats the modders will want to use
	AudioFormat_OGG::Register();
	AudioFormat_YM::Register();
	AudioFormat_MOD::Register();

	// Register the image formats we will be using - better register all of them, as we want this game to be easy to 
	// mod, and we don't know which formats the modders will want to use
	ImageFormat_TGA::Register();
	ImageFormat_GIF::Register();
	ImageFormat_JPG::Register();
	ImageFormat_PNG::Register();
	}


//*** Destructor ***

GameHelper::~GameHelper()
	{
	gameStateManager_.ExitCurrentState();
	}


//*** UpdateGame ***

bool GameHelper::UpdateGame(float deltaTime, Screen& screen)
	{
	// Update systems
	inputManager_.Update();
	musicManager_.Update(deltaTime);
	audio_.Update();
	gameStateManager_.Update(deltaTime);
	spriteControllerManager_.Update(deltaTime);
	spriteSystem_.Update(deltaTime);
	spriteSystem_.Render(screen.GetBackBuffer());

	// Toggle between fullscreen and windowed mode on Alt+Enter
	if (inputManager_.IsKeyDown(KEY_MENU) && inputManager_.WasKeyPressed(KEY_RETURN))
		{
		screen.SetFullscreen(!screen.GetFullscreen());
		inputManager_.RestoreCursor();
		}

	// Handle screen fade when transitioning between game states
	unsigned short screenModulate=0xffff;
	if (gameStateManager_.IsTransitioningIn())
		{
		float c=gameStateManager_.GetTransitionValue();
		screenModulate=FLOATTORGB16(c,c,c);
		}
	else if (gameStateManager_.IsTransitioningOut())
		{
		float c=1-gameStateManager_.GetTransitionValue();
		screenModulate=FLOATTORGB16(c,c,c);
		}
	if (screenModulate==0)
		{
		screen.GetBackBuffer().Clear();
		}

	// Surround screen with a one pixel black border - makes it look a bit nicer when it is scaled up
	int w=screen.GetBackBuffer().GetWidth()-1;
	int h=screen.GetBackBuffer().GetHeight()-1;
	screen.GetBackBuffer().Fill(0,0,w,0,0x0);
	screen.GetBackBuffer().Fill(0,h,w,h,0x0);
	screen.GetBackBuffer().Fill(0,0,0,h,0x0);
	screen.GetBackBuffer().Fill(w,0,w,h,0x0);

	// Show the screen buffer, modifying its content for the state transition fading
	screen.Present(screenModulate);

	// Give some time to the OS. If we didn't, things like moving the window around would feel quite sluggish.
	if (Platform::GetPlatform_OS())
		{
		Platform::GetPlatform_OS()->OsYield();
		}

	// Check if exit was requested on OS level (the main window was closed or Alt+F4 pressed)
	if (Platform::GetPlatform_OS() && Platform::GetPlatform_OS()->ExitRequested())
		{
		// If so, tell game to exit
		gameStateManager_.ExitApplication();
		}

	return !gameStateManager_.IsExitFlagSet();
	}
