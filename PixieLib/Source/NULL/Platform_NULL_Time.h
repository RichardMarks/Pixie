/**
 * \class	Platform_NULL_Time
 * 
 * \ingroup	platform_null
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_NULL_Time_H__
#define __Platform_NULL_Time_H__

// Includes
#include "Platform_Time.h"

// Forward declares

// Platform_NULL_Time
class Platform_NULL_Time:public Platform_Time
	{

	public:

		Platform_NULL_Time();

		virtual float GetTime();

		virtual SystemTime GetSystemTime(
			bool utcTime = false
			);

	};


#endif /* __Platform_NULL_Time_H__ */
