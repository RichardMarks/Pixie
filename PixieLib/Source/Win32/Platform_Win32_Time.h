/**
 * \class	Platform_Win32_Time
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_Win32_Time_H__
#define __Platform_Win32_Time_H__

// Includes
#include "Platform_Time.h"

// Forward declares

// Platform_Win32_Time
class Platform_Win32_Time:public Platform_Time
	{

	public:

		Platform_Win32_Time();

		virtual float GetTime();

		virtual SystemTime GetSystemTime(
			bool utcTime = false
			);

	private:
		double performanceFrequency_;
		double initialValue_;
	};


#endif /* __Platform_Win32_Time_H__ */
