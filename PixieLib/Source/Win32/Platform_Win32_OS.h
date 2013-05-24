/**
 * \class	Platform_Win32_OS
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 * \todo	Have mouse cursor revert to default when placed over the window title bar or borders
 */

#ifndef __Platform_Win32_OS_H__
#define __Platform_Win32_OS_H__

// Includes
#include "Platform_OS.h"
#include "PlatformEventListener.h"

// Forward declares
#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#include <windows.h>

// Platform_Win32_OS
class Platform_Win32_OS:public Platform_OS, PlatformEventListener
	{

	public:
		Platform_Win32_OS(HINSTANCE hInstance, const char* commandLine);

		virtual ~Platform_Win32_OS();

		virtual void SetApplicationName(const char* applicationName);

		virtual const char* GetCommandLineString();

		virtual const char* GetExecutablePath();

		virtual bool HasFocus();

		virtual void OsYield();

		virtual void DisplayAssertMessage(const char* expression, const char* message, const char* file, int line);

		virtual void OutputDebugText(const char * formatstr, ...);

		virtual void ReportFatalError(const char* message, const char* file, int line);

		virtual bool ExitRequested();

		virtual void OnCustomEvent(const char* eventId, void* userData);
		
		HWND GetWindowHandle();

	private:
		void ParseCommandLine(const char* commandLine);

		// Windows creation
		bool RegisterWindowClass(HINSTANCE instance, const char* className);
		HWND CreateWindowInstance(HINSTANCE instance, const char* className, const char* title);

		// Windows message handling
		static LRESULT CALLBACK WindowProcedure_Static(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT WindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
		void ProcessMessages();

		// Message receivers
		LRESULT WmSize(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT WmSysCommand(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT WmActivateApp(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT WmClose(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		char* applicationName_;
		char* executablePath_;
		char* commandLine_;
		HINSTANCE hInstance_;
		HWND windowHandle_;
		bool applicationHasFocus_;
		HCURSOR cursor_;
		bool exitRequested_;
	};


#endif /* __Platform_Win32_OS_H__ */
