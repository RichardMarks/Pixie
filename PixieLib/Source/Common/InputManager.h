/**
 * \class	InputManager
 * 
 * \ingroup	input
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 * \todo	Gamepad support
 * \todo	Mapping system (from QtickyEngine?)
 * \todo	Event system, where gamestates can register for the Input events it's interested in, and be called when they 
 *			are triggered only. This is so we can have a fully event-driven gamestates (no need to poll input manager in 
 *			the Update function, which should make things cleaner, and make most GameStates not need an Update function)
 */
 
#ifndef __InputManager_H__
#define __InputManager_H__

// Includes
#include "Singleton.h"
#include "Array.h"
#include "KeyCodes.h"

// Forward declares
class Platform_Input_MouseDevice;
class Platform_Input_KeyboardDevice;


// InputManager
class InputManager:public Singleton<InputManager>
	{
	public:	
		InputManager();
		virtual ~InputManager();
	
		void Update();

		float GetCursorX();

		float GetCursorY();


		bool IsKeyDown(
			KeyCode key
			);

		bool WasKeyPressed(
			KeyCode key
			);

		bool WasKeyReleased(
			KeyCode key
			);

		bool IsAnyKeyDown();

		bool WasAnyKeyPressed();

		bool WasAnyKeyReleased();

		bool IsCharDown(
			unsigned char ascii
			);

		bool WasCharPressed(
			unsigned char ascii
			);

		bool WasCharReleased(
			unsigned char ascii
			);


		void ClearCharacters();

		int GetCharacterCount();

		char GetCharacter(
			int index
			);

		void CreateCursor(int id, const char* image, int hotspotX, int hotspotY);

		void SetCursor(int id);

		void ShowCursor();

		void HideCursor();

		void RestoreCursor();
		
	private:
		const Platform_Input_MouseDevice* mouseDevice_;
		const Platform_Input_KeyboardDevice* keyboardDevice_;
		float cursorPosX_;
		float cursorPosY_;
		bool keyStates_[KEYCODE_COUNT];
		bool previousKeyStates_[KEYCODE_COUNT];
		bool charStates_[256];
		bool previousCharStates_[256];
		
		Array<char> characterBuffer_;

		struct Cursor
			{
			int id;
			int handle;
			};
		Array<Cursor> cursors_;
		int currentCursor_;
		bool cursorHidden_;
	};

#define siInputManager InputManager::GetInstance()


#endif /* __INPUTHANDLER_H__ */
