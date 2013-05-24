/**
 * \class	Platform_Input_MouseDevice
 *
 * \ingroup	platform
 * \brief	Platform independent abstraction of a mouse device
 * \author	Mattias Gustavsson	
 * 
 * Provides basic acces to the mouse - position and button states
 */

#ifndef __Platform_Input_MouseDevice_H__
#define __Platform_Input_MouseDevice_H__

// Includes

// Forward declares

// Platform_Input_MouseDevice
class Platform_Input_MouseDevice
	{
	public:
		/**
		 * Destructor
		 */
		virtual ~Platform_Input_MouseDevice() { };
		

		/**
		 * Retrieves the position of the mouse pointer. This position is in coordinates of
		 * the window, and may need to be transformed to correspond to the area of the
		 * current back buffer being presented using Platform_Screen.
		 */
		virtual void GetPosition(
			float& x,	///< Reference to the variable to store the x-position in 
			float& y	///< Reference to the variable to store the y-position in 
			) const = 0;


		/**
		 * Retrieves the status of the left mouse button
		 *
		 * \returns True if the left mouse button is held down, false if it isn't
		 */
		virtual bool IsLeftButtonDown() const = 0;


		/**
		 * Retrieves the status of the right mouse button
		 *
		 * \returns True if the right mouse button is held down, false if it isn't
		 */
		virtual bool IsRightButtonDown() const = 0;

	};

#endif /* __Platform_Input_MouseDevice_H__ */
