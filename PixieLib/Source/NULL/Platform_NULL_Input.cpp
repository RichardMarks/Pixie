//*** Platform_NULL_Input ***

#include "Platform_NULL_Input.h"
#include "Platform_NULL_Input_KeyboardDevice.h"
#include "Platform_NULL_Input_MouseDevice.h"


//*** Constructor ***

Platform_NULL_Input::Platform_NULL_Input():
	keyboardDevice_(0),
	mouseDevice_(0)
	{
	}


//*** Destructor ***

Platform_NULL_Input::~Platform_NULL_Input()
	{
	if (keyboardDevice_)
		{
		delete keyboardDevice_;
		keyboardDevice_=0;
		}

	if (mouseDevice_)
		{
		delete mouseDevice_;
		mouseDevice_=0;
		}
	}


//*** GetKeyboardDevice ***

const Platform_Input_KeyboardDevice* Platform_NULL_Input::GetKeyboardDevice()
	{
	if (!keyboardDevice_)
		{
		keyboardDevice_=new Platform_NULL_Input_KeyboardDevice();
		}

	return keyboardDevice_;
	}


//*** GetMouseDevice ***

const Platform_Input_MouseDevice* Platform_NULL_Input::GetMouseDevice()
	{
	if (!mouseDevice_)
		{
		mouseDevice_=new Platform_NULL_Input_MouseDevice();
		}

	return mouseDevice_;
	}


//*** CreateMouseCursor ***

int Platform_NULL_Input::CreateMouseCursor(int width, int height, int hotspotX, int hotspotY, unsigned short* colorData, unsigned char* alphaData)
	{
	return 0;
	}


//*** SetMouseCursor ***

void Platform_NULL_Input::SetMouseCursor(int handle)
	{

	}


//*** SetDefaultMouseCursor ***

void Platform_NULL_Input::SetDefaultMouseCursor()
	{

	}
