/**
 * \class	Platform_NULL_Input
 * 
 * \ingroup	platform_null
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_NULL_Input_H__
#define __Platform_NULL_Input_H__

// Includes
#include "Platform_Input.h"

// Forward declares

// Platform_NULL_Input
class Platform_NULL_Input:public Platform_Input
	{

	public:
		Platform_NULL_Input();
		~Platform_NULL_Input();

		virtual const Platform_Input_KeyboardDevice* GetKeyboardDevice();
		virtual const Platform_Input_MouseDevice* GetMouseDevice();

		virtual int CreateMouseCursor(int width, int height, int hotspotX, int hotspotY, unsigned short* colorData, unsigned char* alphaData);
		virtual void SetMouseCursor(int handle);

		virtual void SetDefaultMouseCursor();
	private:
		Platform_Input_KeyboardDevice* keyboardDevice_; 
		Platform_Input_MouseDevice* mouseDevice_; 
	};

#endif /* __Platform_NULL_Input_H__ */
