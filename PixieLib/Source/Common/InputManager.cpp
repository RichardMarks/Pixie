//*** InputManager.cpp ***
 
#include "InputManager.h"

#include "Debug.h"
#include "Platform_Screen.h"
#include "Platform_Input.h"
#include "Platform_Input_KeyboardDevice.h"
#include "Platform_Input_MouseDevice.h"
#include "Bitmap_16bitAlpha.h"
#include "Asset.h"
#include "Filename.h"


//*** Constructor ***

InputManager::InputManager():
	cursorPosX_(-1),
	cursorPosY_(-1),
	keyboardDevice_(0),
	mouseDevice_(0),
	currentCursor_(-1),
	cursorHidden_(false)
	{
	if (Platform::GetPlatform_Input())
		{
		keyboardDevice_=Platform::GetPlatform_Input()->GetKeyboardDevice();
		mouseDevice_=Platform::GetPlatform_Input()->GetMouseDevice();
		}

	if (keyboardDevice_)
		{
		for (int i=0; i<KEYCODE_COUNT; i++)
			{
			keyStates_[i]=keyboardDevice_->IsKeyDown((Platform_Input_KeyboardDevice::KeyCode)i);
			previousKeyStates_[i]=keyStates_[i];
			}

		for (int i=0; i<256; i++)
			{
			charStates_[i]=keyboardDevice_->IsCharDown((unsigned char)i);
			previousCharStates_[i]=charStates_[i];
			}
		}
	}


//*** Destructor ***

InputManager::~InputManager()
	{
	}



//*** Update ***

void InputManager::Update()
	{

	// Store previous key states
	for (int i=0; i<KEYCODE_COUNT; i++)
		{
		previousKeyStates_[i]=keyStates_[i];
		}

	// Store previous char states
	for (int i=0; i<256; i++)
		{
		previousCharStates_[i]=charStates_[i];
		}

	if (keyboardDevice_)
		{
		// Get key states
		for (int i=0; i<KEYCODE_COUNT; i++)
			{
			keyStates_[i]=keyboardDevice_->IsKeyDown((Platform_Input_KeyboardDevice::KeyCode)i);
			}

		// Get char states
		for (int i=0; i<256; i++)
			{
			charStates_[i]=keyboardDevice_->IsCharDown((unsigned char)i);
			}
	
		// Get buffered input
		for (int i=0; i<keyboardDevice_->GetBufferedCharacterCount(); i++)
			{
			characterBuffer_.Add(keyboardDevice_->GetBufferedCharacter(i));
			}
		keyboardDevice_->ClearBufferedCharacters();
		}

	

	// Get cursor position
	if (mouseDevice_)
		{
		mouseDevice_->GetPosition(cursorPosX_,cursorPosY_);
		if (Platform::GetPlatform_Screen())
			{
			Platform::GetPlatform_Screen()->TransformCursorCoordinates(cursorPosX_,cursorPosY_);
			}
		}
	};



//*** GetCursorX ***

float InputManager::GetCursorX()
	{
	return cursorPosX_;
	}


//*** GetMouseY ***

float InputManager::GetCursorY()
	{
	return cursorPosY_;
	}


//*** IsKeyDown ***

bool InputManager::IsKeyDown(KeyCode key)
	{
	Assert(key>=0 && key<KEYCODE_COUNT,"Key code out of range");
	if (key<0 || key>=KEYCODE_COUNT)
		{
		return false;
		}

	return keyStates_[key];
	}


//*** WasKeyPressed ***

bool InputManager::WasKeyPressed(KeyCode key)
	{
	Assert(key>=0 && key<KEYCODE_COUNT,"Key code out of range");
	if (key<0 || key>=KEYCODE_COUNT)
		{
		return false;
		}

	return keyStates_[key] && !previousKeyStates_[key];
	}


//*** WasKeyReleased ***

bool InputManager::WasKeyReleased(KeyCode key)
	{
	Assert(key>=0 && key<KEYCODE_COUNT,"Key code out of range");
	if (key<0 || key>=KEYCODE_COUNT)
		{
		return false;
		}

	return !keyStates_[key] && previousKeyStates_[key];
	}


//*** IsAnyKeyDown ***

bool InputManager::IsAnyKeyDown()
	{
	for (int i=0; i<KEYCODE_COUNT; i++)
		{
		if (keyStates_[i])
			{
			return true;
			}

		}
	return false;
	}


//*** WasAnyKeyPressed ***

bool InputManager::WasAnyKeyPressed()
	{
	for (int i=0; i<KEYCODE_COUNT; i++)
		{
		if (keyStates_[i] && !previousKeyStates_[i])
			{
			return true;
			}
		}
	return false;
	}


//*** WasAnyKeyReleased ***

bool InputManager::WasAnyKeyReleased()
	{
	for (int i=0; i<KEYCODE_COUNT; i++)
		{
		if (!keyStates_[i] && previousKeyStates_[i])
			{
			return true;
			}
		}
	return false;
	}


//*** IsCharDown ***

bool InputManager::IsCharDown(unsigned char ascii)
	{
	return charStates_[ascii];
	}


//*** WasCharPressed ***

bool InputManager::WasCharPressed(unsigned char ascii)
	{
	return charStates_[ascii] && !previousCharStates_[ascii];
	}


//*** WasCharReleased ***

bool InputManager::WasCharReleased(unsigned char ascii)
	{
	return !charStates_[ascii] && previousCharStates_[ascii];
	}


//*** ClearCharacters ***

void InputManager::ClearCharacters()
	{
	characterBuffer_.Clear(false);
	}


//*** GetCharacterCount ***

int InputManager::GetCharacterCount()
	{
	return characterBuffer_.GetItemCount();
	}


//*** GetCharacter ***

char InputManager::GetCharacter(int index)
	{
	if (index<0 || index>=characterBuffer_.GetItemCount())
		{
		return 0;
		}

	return characterBuffer_.Get(index);
	}


//*** CreateCursor ***

void InputManager::CreateCursor(int id, const char* image, int hotspotX, int hotspotY)
	{
	for (int i=0; i<cursors_.GetItemCount(); i++)
		{
		Assert(cursors_.Get(i).id!=id,"A cursor with that ID already exists");
		if (cursors_.Get(i).id==id)
			{
			return;
			}
		}
	
	Cursor cursor;
	cursor.id=id;
	cursor.handle=0;
	if (image && Platform::GetPlatform_Input())
		{
		Bitmap_16bitAlpha cursorImage(image);
		cursor.handle=Platform::GetPlatform_Input()->CreateMouseCursor(cursorImage.GetHPitch(),cursorImage.GetVPitch(),hotspotX,hotspotY,cursorImage.GetColorData(),cursorImage.GetAlphaData());
		}
	cursors_.Add(cursor);
	}


///*** SetCursor ***

void InputManager::SetCursor(int id)
	{
	for (int i=0; i<cursors_.GetItemCount(); i++)
		{
		if (cursors_.Get(i).id==id)
			{
			currentCursor_=id;
			if (Platform::GetPlatform_Input())
				{
				Platform::GetPlatform_Input()->SetMouseCursor(cursors_.Get(i).handle);
				}
			}
		}
	}


//*** ShowCursor ***

void InputManager::ShowCursor()
	{
	cursorHidden_=false;
	if (currentCursor_==-1)
		{
		if (Platform::GetPlatform_Input())
			{
			Platform::GetPlatform_Input()->SetDefaultMouseCursor();
			}
		return;
		}

	SetCursor(currentCursor_);
	}


//*** HideCursor ***

void InputManager::HideCursor()
	{
	cursorHidden_=true;
	
	if (Platform::GetPlatform_Screen())
		{
		if (!Platform::GetPlatform_Screen()->GetFullscreen())
			{
			return;
			}
		}

	if (Platform::GetPlatform_Input())
		{
		Platform::GetPlatform_Input()->SetMouseCursor(0);
		}
	}


//*** RestoreCursor ***

void InputManager::RestoreCursor()
	{
	bool windowed=true;
	if (Platform::GetPlatform_Screen())
		{
		windowed=!Platform::GetPlatform_Screen()->GetFullscreen();
		}

	if (windowed)
		{
		if (currentCursor_==-1)
			{
			if (Platform::GetPlatform_Input())
				{
				Platform::GetPlatform_Input()->SetDefaultMouseCursor();
				}
			return;
			}

		SetCursor(currentCursor_);
		}
	else
		{
		if (cursorHidden_)
			{
			if (Platform::GetPlatform_Input())
				{
				Platform::GetPlatform_Input()->SetMouseCursor(0);
				}
			}
		else
			{
			ShowCursor();
			}
		}
	}
