//*** Platform_Win32_Input_KeyboardDevice.cpp ***

#include "Platform_Win32_Input_KeyboardDevice.h"
#include "Platform_OS.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <windows.h>

//*** Constructor ***

Platform_Win32_Input_KeyboardDevice::Platform_Win32_Input_KeyboardDevice():
	characterBufferCount_(0)
	{
	for (int i=0; i<256; i++)
		{
		charStates_[i]=false;
		charKeycodes_[i]=0;
		}
	}


//*** Destructor ***

Platform_Win32_Input_KeyboardDevice::~Platform_Win32_Input_KeyboardDevice()
	{
	}


//*** IsKeyDown ***

bool Platform_Win32_Input_KeyboardDevice::IsKeyDown(KeyCode keyCode) const
	{
	if ((Platform::GetPlatform_OS() && Platform::GetPlatform_OS()->HasFocus()) && GetAsyncKeyState(keyCode)!=0)
		{
		return true;
		}

	return false;
	}


//*** IsCharDown ***

bool Platform_Win32_Input_KeyboardDevice::IsCharDown(char ascii) const
	{
	return charStates_[(unsigned char)ascii];
	}


//*** SetCharPressed ***

void Platform_Win32_Input_KeyboardDevice::SetCharPressed(char ascii, unsigned char  keycode)
	{
	charStates_[(unsigned char)ascii]=true;
	charKeycodes_[(unsigned char)ascii]=keycode;
	AddBufferedCharacter(ascii);
	}


//*** SetCharReleased ***

void Platform_Win32_Input_KeyboardDevice::SetCharReleased(unsigned char  keycode)
	{
	for (int i=0; i<256; i++)
		{
		if (charKeycodes_[i]==keycode)
			{
			charStates_[i]=false;
			return;
			}
		}
	}


//*** AddBufferedCharacter ***

void Platform_Win32_Input_KeyboardDevice::AddBufferedCharacter(char ascii)
	{
	while (characterBufferCount_>=256)
		{
		for (int i=1; i<characterBufferCount_; i++)
			{
			characterBuffer_[i-1]=characterBuffer_[i];
			}
		characterBufferCount_--;
		}

	characterBuffer_[characterBufferCount_]=ascii;
	characterBufferCount_++;
	}


//*** ClearBufferedCharacters ***

void Platform_Win32_Input_KeyboardDevice::ClearBufferedCharacters() const
	{
	characterBufferCount_=0;
	}


//*** GetBufferedCharacterCount ***

int Platform_Win32_Input_KeyboardDevice::GetBufferedCharacterCount() const
	{
	return characterBufferCount_;
	}


//*** GetBufferedCharacter ***

char Platform_Win32_Input_KeyboardDevice::GetBufferedCharacter(int index) const
	{
	if (index<0 || index>=characterBufferCount_)
		{
		return 0;
		}

	return characterBuffer_[index];
	}
