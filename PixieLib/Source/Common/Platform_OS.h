/**
 * \class	Platform_OS
 * 
 * \ingroup	platform
 * \brief	Platform independent abstraction of common OS functionality
 * \author	Mattias Gustavsson	
 * 
 * Platform_OS is a system wich will be in use on all platforms. This is
 * where the main window and message pump is handled under Windows, and
 * similar things is done for other platforms. It implements various OS
 * specific, basic functionality.
 */

#ifndef __Platform_OS_H__
#define __Platform_OS_H__

// Includes
#include "Platform.h"

// Forward declares

// Platform_OS
class Platform_OS
	{
	public:
		/**
		 * Destructor
		 */
		virtual ~Platform_OS() { };


		/**
		 * Sets the name of the application. This is what's displayed in the title bar
		 * of the window, if the current platform has a window. On some platforms, this
		 * method will have no effect, but it can still be called with no error. If 0
		 * is passed to the method, the default name "Pixie Game Engine" will be used.
		 */		
		virtual void SetApplicationName(
			const char* applicationName	///< The name of the application, or 0 for default name
			) = 0;


		/**
		 * If the platform supports command line parameters, and some were passed in to
		 * the application, they can be retrieved using this method. The platform specific
		 * implementation may reformat the original command line strings, such that all 
		 * flags will have a dash (-) as a prefix, that long filenames containing spaces
		 * will be surrounded by quotation marks (") and that directory separators for 
		 * specified paths will be forward slash (/) regardless of whatever convention was
		 * originally used on the platform. This is to make it easy to parse the command
		 * line string without knowing which platform we're currently running on.
		 *
		 * \returns	The command line string in platform independent format
		 */
		virtual const char* GetCommandLineString() = 0;


		/**
		 * Sometimes, it's useful to find the path where the applications executable is 
		 * stored, in case it is different from the current working directory. This method
		 * will return the full path to the executable, not including the name of the
		 * executable itself. On some platforms, this information might not be available,
		 * in which case this method will return 0.
		 *
		 * \returns	The path to the executable
		 */
		virtual const char* GetExecutablePath() = 0;
	

		/**
		 * For windowed systems, it is possible for the app to not be the one currently
		 * receiving input. This method will return true if the application has input
		 * focus, and false if it has not. On non-windowed platforms, this method will
		 * always return true.
		 *
		 * \returns True if the application has focus, false if it has not.
		 */
		virtual bool HasFocus() = 0;


		/**
		 * On some platforms, it is necessary to hand control over to the OS at regular
		 * intervals (typically once per frame), for example the windows message pump.
		 * This method should be called by the game once per frame, but on some platforms,
		 * it might not do anything, while on others it might do a lot.
		 *
		 * Not calling this method might lead to unresponsive application or unexpected
		 * side effects. When called, this method sends the "OsYield" platform event, which
		 * will be received by all platform systems which have registered for platform events.
		 */
		virtual void OsYield() = 0;


		/**
		 * The way Assert failures are reported is different on different platforms. On some,
		 * it might be an output to stdout, on others it might be a message box providing
		 * options to abort execution or break into the debugger. This method will display the
		 * Assert message in the platform specific way. It won't test the expression, or check
		 * if this is a debug build - that's all up to the higher-level code.
		 */
		virtual void DisplayAssertMessage(
			const char* expression,	///< The expression which failed and caused the Assert to trigger 
			const char* message,	///< User-friendly message to display, explaining what the problem is
			const char* file,	///< Filename of the source file where the Assert was triggered
			int line	///< Line number of the source file where the Assert was triggered
			) = 0;


		/**
		 * It is often useful to be able to print various informational messages for debug purposes.
		 * The way this is done will be different for different platforms, so this method abstracts
		 * that functionality. OutputDebugText works just like printf, where you supply a format
		 * string, indicating where values are to be inserted using %d, %f etc, followed by all the
		 * values to replace those markers.
		 */
		virtual void OutputDebugText(
			const char* formatstr,	... ///< Format string, followed by all it's values, just like the standard printf
			) = 0;


		/**
		 * When something goes so wrong that it is impossible to continue with the execution,
		 * this method should be called. It will display the message, in whatever way makes
		 * sense for the platform, and then terminate the application. This should be reserved 
		 * for really severe errors, which can't be handled by the application code.
		 */
		virtual void ReportFatalError(
			const char* message,	///< User-friendly message to display, explaining what the problem is
			const char* file,	///< Filename of the source file where the error occurred
			int line	///< Line number of the source file where the error occurred
			) = 0;


		/**
		 * Shutdown of the application is controlled by the application main-loop. But on some platforms,
		 * a request to terminate can be issued on OS-level, like when you click the Close-button on a 
		 * window. When that happens, this method will return true from that point onwards, but before that,
		 * it will return false. If the application wants to handle OS-level shutdown request, it needs to
		 * check this method regularly. On some platforms, this method might never return true.
		 *
		 * \returns	True if the OS are requesting that the application is to be terminated, otherwise false
		 */
		virtual bool ExitRequested() = 0;

	};

#endif /* __Platform_OS_H__ */
