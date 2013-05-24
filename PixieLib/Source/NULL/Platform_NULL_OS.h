/**
 * \class	Platform_NULL_OS
 * 
 * \ingroup	platform_null
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_NULL_OS_H__
#define __Platform_NULL_OS_H__

// Includes
#include "Platform_OS.h"

// Forward declares

// Platform_NULL_OS
class Platform_NULL_OS:public Platform_OS
	{

	public:
		Platform_NULL_OS();

		virtual void SetApplicationName(const char* applicationName);

		virtual const char* GetCommandLineString();

		virtual const char* GetExecutablePath();

		virtual bool HasFocus();

		virtual void OsYield();

		virtual void DisplayAssertMessage(const char* expression, const char* message, const char* file, int line);

		virtual void OutputDebugText(const char * formatstr, ...);

		virtual void ReportFatalError(const char* message, const char* file, int line);

		virtual bool ExitRequested();


	private:
	};


#endif /* __Platform_NULL_OS_H__ */
