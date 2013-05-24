/**
 * \class	Platform_Time
 * 
 * \ingroup	platform
 * \brief	Platform independent abstraction of time functions
 * \author	Mattias Gustavsson	
 * 
 * Provides functionality to get the current time, in various ways.
 */

#ifndef __Platform_Time_H__
#define __Platform_Time_H__

// Includes
#include "Platform.h"

// Forward declares

// Platform_Time
class Platform_Time
	{
	public:
		/**
		 * Destructor
		 */
		virtual ~Platform_Time() { };


		/**
		 * Retrieves the current time, as measured from the start of the application
		 * 
		 * \returns	The number of seconds since the application was started
		 */
		virtual float GetTime() = 0;


		/**
		 * Resulting structure from a call to GetSystemTime
		 */
		struct SystemTime
			{
			short year;
			char month;
			char day;
			char dayOfWeek;
			char hour;
			char minute;
			char second;
			};
		
		/**
		 * Retrieves the local current date and time. If the utcTime parameter
		 * is set to true, the UTC (universal time) equivalent to the local time
		 * is returned.
		 * 
		 * \returns	The current date and time (local or UTC)
		 */
		virtual SystemTime GetSystemTime(
			bool utcTime = false	///< When set to true, specifies that UTC time should be retrieved instead of local time
			) = 0;
	};



#endif /* __Platform_Time_H__ */
