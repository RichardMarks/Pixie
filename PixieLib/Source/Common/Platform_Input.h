/**
 * \class	Platform_Input
 * 
 * \ingroup	platform
 * \brief	Platform independent abstraction of input devices
 * \author	Mattias Gustavsson	
 * 
 * Provides platform independent access to input keyboard and mouse.
 */

#ifndef __Platform_Input_H__
#define __Platform_Input_H__

// Includes
#include "Platform.h"

// Forward declares
class Platform_Input_KeyboardDevice;
class Platform_Input_MouseDevice;

// Platform_Input
class Platform_Input
	{
	public:
		/**
		 * Destructor
		 */
		virtual ~Platform_Input() { };


		/**
		 * Retrieves the keyboard device instance. This pointer is owned and managed
		 * by Platform_Input, and should not be deleted by the caller.
		 *
		 * \returns	The keyboard device for the platform
		 */
		virtual const Platform_Input_KeyboardDevice* GetKeyboardDevice() = 0;
		

		/**
		 * Retrieves the mouse device instance. This pointer is owned and managed
		 * by Platform_Input, and should not be deleted by the caller.
		 *
		 * \returns	The mouse device for the platform
		 */
		virtual const Platform_Input_MouseDevice* GetMouseDevice() = 0;

		/**
		 * Creates a system mouse cursor from the specified bitmap data. Different systems
		 * might support different number of colors and transparency, so it is not guaranteed
		 * that the cursor will look identical to to the specified bitmap data.
		 * 
		 * \returns	A handle to the new cursor, to be used with SetMouseCursor or 0 if it couldn't be created
		 */
		virtual int CreateMouseCursor(
			int width,	///< Width of the bitmap data
			int height, ///< Height of the bitmap data
			int hotspotX, ///< Horizontal offset indicating cursor center
			int hotspotY, ///< Vertical offset indicating cursor center
			unsigned short* colorData, ///< 16-bit color bitmap data
			unsigned char* alphaData ///< 8-bit alpha (transparency) bitmap data
			) = 0;


		/** 
		 * Tells the system to use the specified mouse cursor, created through a call 
		 * to CreateMouseCursor. Can also be used to hide the mouse cursor, by passing 
		 * a value of 0 for handle.
		 */
		virtual void SetMouseCursor(
			int handle	///< Handle of mouse cursor to use, or 0 to hide cursor
			) = 0;


		/** 
		 * Tells the system to use the default mouse cursor for the system (the
		 * standard arrow for windows)
		 */
		virtual void SetDefaultMouseCursor() = 0;
	};

#endif /* __Platform_Input_H__ */
