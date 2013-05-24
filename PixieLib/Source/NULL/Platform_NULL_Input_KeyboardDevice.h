/**
 * \class	Platform_NULL_Input_KeyboardDevice
 * 
 * \ingroup	platform_null
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_NULL_Input_KeyboardDevice_H__
#define __Platform_NULL_Input_KeyboardDevice_H__

// Includes
#include "Platform_Input_KeyboardDevice.h"

// Forward declares

// Platform_NULL_Input_KeyboardDevice
class Platform_NULL_Input_KeyboardDevice: public Platform_Input_KeyboardDevice
	{
	public:
		Platform_NULL_Input_KeyboardDevice();

		virtual bool IsKeyDown(KeyCode keyCode) const;
		virtual bool IsCharDown(char ascii) const;

		virtual void ClearBufferedCharacters() const;
		virtual int GetBufferedCharacterCount() const;
		virtual char GetBufferedCharacter(int index) const;
	};

#endif /* __Platform_NULL_Input_KeyboardDevice_H__ */
