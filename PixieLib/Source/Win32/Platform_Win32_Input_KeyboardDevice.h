/**
 * \class	Platform_Win32_Input_KeyboardDevice
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_Win32_Input_KeyboardDevice_H__
#define __Platform_Win32_Input_KeyboardDevice_H__

// Includes
#include "Platform_Input_KeyboardDevice.h"

// Forward declares

// Platform_Win32_Input_KeyboardDevice
class Platform_Win32_Input_KeyboardDevice: public Platform_Input_KeyboardDevice
	{
	public:
		Platform_Win32_Input_KeyboardDevice();
		virtual ~Platform_Win32_Input_KeyboardDevice();

		virtual bool IsKeyDown(KeyCode keyCode) const;
		virtual bool IsCharDown(char ascii) const;

		virtual void ClearBufferedCharacters() const;
		virtual int GetBufferedCharacterCount() const;
		virtual char GetBufferedCharacter(int index) const;

		void SetCharPressed(char ascii, unsigned char keycode);
		void SetCharReleased(unsigned char keycode);
		void AddBufferedCharacter(char ascii);

	private:
		bool charStates_[256];
		int charKeycodes_[256];
		
		char characterBuffer_[256];
		mutable int characterBufferCount_;
	};

#endif /* __Platform_Win32_Input_KeyboardDevice_H__ */
