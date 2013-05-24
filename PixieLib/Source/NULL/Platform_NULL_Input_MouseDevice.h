/**
 * \class	Platform_NULL_Input_MouseDevice
 * 
 * \ingroup	platform_null
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_NULL_Input_MouseDevice_H__
#define __Platform_NULL_Input_MouseDevice_H__

// Includes
#include "Platform_Input_MouseDevice.h"

// Forward declares


// Platform_NULL_Input_MouseDevice
class Platform_NULL_Input_MouseDevice: public Platform_Input_MouseDevice
	{
	public:
		Platform_NULL_Input_MouseDevice();

		virtual void GetPosition(float& x, float& y) const;
		virtual bool IsLeftButtonDown() const;
		virtual bool IsRightButtonDown() const;

	};

#endif /* __Platform_NULL_Input_MouseDevice_H__ */
