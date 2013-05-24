//*** Platform_NULL_Input_KeyboardDevice.cpp ***

#include "Platform_NULL_Input_KeyboardDevice.h"

//*** Constructor ***

Platform_NULL_Input_KeyboardDevice::Platform_NULL_Input_KeyboardDevice()
	{
	}



//*** IsKeyDown ***

bool Platform_NULL_Input_KeyboardDevice::IsKeyDown(KeyCode keyCode) const
	{
	return false;
	}


//*** IsCharDown ***

bool Platform_NULL_Input_KeyboardDevice::IsCharDown(char ascii) const
	{
	return false;
	}


//*** ClearBufferedCharacters ***

void Platform_NULL_Input_KeyboardDevice::ClearBufferedCharacters() const
	{

	}


//*** GetBufferedCharacterCount ***

int Platform_NULL_Input_KeyboardDevice::GetBufferedCharacterCount() const
	{
	return 0;
	}


//*** GetBufferedCharacter ***

char Platform_NULL_Input_KeyboardDevice::GetBufferedCharacter(int index) const
	{
	return 0;
	}
