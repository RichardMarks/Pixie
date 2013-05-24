/**
 * \class	Platform
 * 
 * \ingroup	platform
 * \brief	Singleton class for handling platform abstraction systems
 * \author	Mattias Gustavsson	
 * 
 * The Platform system consists of several different systems, which defines
 * the interface to different parts of the OS or hardware. When the application
 * starts, you must define which implementation to use for each of the platform 
 * systems (interfaces) that you want to utilize in your application. This is 
 * done by creating an instance of the platform specific implementation, and 
 * passing it to the Platform singleton, from which it can be retrieved whevenever
 * needed.
 * 
 * A typical way of setting up the platform system could look like this:
 * int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
 * 	{
 * 	Platform_Win32_OS* os=new Platform_Win32_OS(hInstance,lpCmdLine);
 * 	Platform::SetPlatform_OS(os);
 * 	Platform::SetPlatform_Time(new Platform_Win32_Time());
 * 	Platform::SetPlatform_Sound(new Platform_Win32_Sound(os));
 * 	Platform::SetPlatform_Input(new Platform_Win32_Input(os));
 * 	Platform::SetPlatform_Screen(new Platform_Win32_Screen(os));
 * 	Platform::SetPlatform_FileSystem(new Platform_Win32_FileSystem());
 *  ...
 *  }
 *
 * And any system can later be accessed by calling, for example: Platform::GetPlatform_OS()
 *
 * The platform system has been designed to be easy to port, rather than to be easy to use,
 * so it only provides a thin abstraction layer. More user-friendly functionality is built
 * on top of this in the Pixie game engine, and you rarely, if ever, need to use the Platform
 * systemes directly (thouhgh the Pixie engine makes frequent use of it).
 *
 * There's a NULL platform implementation provided, wich implements empty functions for all the
 * systems, which can be useful when porting or debugging. The NULL implementations will return
 * valid objects and have all functions callable, but they will not perform any functionality 
 * and will mostly return 0 values.
 */
 

#ifndef __Platform_H__
#define __Platform_H__

// Includes
#include "PlatformEventListener.h"

// Forward declares
class Platform_OS;
class Platform_Time;
class Platform_Sound;
class Platform_Input;
class Platform_FileSystem;
class Platform_Screen;
class Platform_Network;
class Platform_3D;


// Platform
class Platform
	{
	public:
		/**
		 * Specifies the Platform_OS instance to use. Ownership of the pointer passed
		 * in will be owned and managed by the Platform system, which will also delete
		 * the instance on termination, by calling delete, so make sure it is allocated
		 * on the heap using the new keyword - but don't delete it yourself.
		 * If an instance have already been specified through a prior call to this method,
		 * this method will have no effect, and the previous instance will still be the
		 * one used, and the pointer passed in will just be immediately deleted.
		 */
		static void SetPlatform_OS(
			Platform_OS* instance	///< Pointer to the instance
			);


		/**
		 * Specifies the Platform_Time instance to use. Ownership of the pointer passed
		 * in will be owned and managed by the Platform system, which will also delete
		 * the instance on termination, by calling delete, so make sure it is allocated
		 * on the heap using the new keyword - but don't delete it yourself.
		 * If an instance have already been specified through a prior call to this method,
		 * this method will have no effect, and the previous instance will still be the
		 * one used, and the pointer passed in will just be immediately deleted.
		 */
		static void SetPlatform_Time(
			Platform_Time* instance	///< Pointer to the instance
			);


		/**
		 * Specifies the Platform_Sound instance to use. Ownership of the pointer passed
		 * in will be owned and managed by the Platform system, which will also delete
		 * the instance on termination, by calling delete, so make sure it is allocated
		 * on the heap using the new keyword - but don't delete it yourself.
		 * If an instance have already been specified through a prior call to this method,
		 * this method will have no effect, and the previous instance will still be the
		 * one used, and the pointer passed in will just be immediately deleted.
		 */
		static void SetPlatform_Sound(
			Platform_Sound* instance	///< Pointer to the instance
			);


		/**
		 * Specifies the Platform_Input instance to use. Ownership of the pointer passed
		 * in will be owned and managed by the Platform system, which will also delete
		 * the instance on termination, by calling delete, so make sure it is allocated
		 * on the heap using the new keyword - but don't delete it yourself.
		 * If an instance have already been specified through a prior call to this method,
		 * this method will have no effect, and the previous instance will still be the
		 * one used, and the pointer passed in will just be immediately deleted.
		 */
		static void SetPlatform_Input(
			Platform_Input* instance	///< Pointer to the instance
			);


		/**
		 * Specifies the Platform_FileSystem instance to use. Ownership of the pointer passed
		 * in will be owned and managed by the Platform system, which will also delete
		 * the instance on termination, by calling delete, so make sure it is allocated
		 * on the heap using the new keyword - but don't delete it yourself.
		 * If an instance have already been specified through a prior call to this method,
		 * this method will have no effect, and the previous instance will still be the
		 * one used, and the pointer passed in will just be immediately deleted.
		 */
		static void SetPlatform_FileSystem(
			Platform_FileSystem* instance	///< Pointer to the instance
			);


		/**
		 * Specifies the Platform_Screen instance to use. Ownership of the pointer passed
		 * in will be owned and managed by the Platform system, which will also delete
		 * the instance on termination, by calling delete, so make sure it is allocated
		 * on the heap using the new keyword - but don't delete it yourself.
		 * If an instance have already been specified through a prior call to this method,
		 * this method will have no effect, and the previous instance will still be the
		 * one used, and the pointer passed in will just be immediately deleted.
		 */
		static void SetPlatform_Screen(
			Platform_Screen* instance	///< Pointer to the instance
			);


		/**
		 * Specifies the Platform_Network instance to use. Ownership of the pointer passed
		 * in will be owned and managed by the Platform system, which will also delete
		 * the instance on termination, by calling delete, so make sure it is allocated
		 * on the heap using the new keyword - but don't delete it yourself.
		 * If an instance have already been specified through a prior call to this method,
		 * this method will have no effect, and the previous instance will still be the
		 * one used, and the pointer passed in will just be immediately deleted.
		 */
		static void SetPlatform_Network(
			Platform_Network* instance	///< Pointer to the instance
			);


		/**
		 * Specifies the Platform_3D instance to use. Ownership of the pointer passed
		 * in will be owned and managed by the Platform system, which will also delete
		 * the instance on termination, by calling delete, so make sure it is allocated
		 * on the heap using the new keyword - but don't delete it yourself.
		 * If an instance have already been specified through a prior call to this method,
		 * this method will have no effect, and the previous instance will still be the
		 * one used, and the pointer passed in will just be immediately deleted.
		 */
		static void SetPlatform_3D(
			Platform_3D* instance	///< Pointer to the instance
			);


		/**
		 * Retrieves the pointer to the global instance of the Platform_OS system.
		 * The value returned might be 0 if no instance have been specified through
		 * a call to SetPlatform_OS, and the caller should test the return value to
		 * make sure it is a valid pointer.
		 *
		 * \returns	A pointer to the Platform_OS instance, or 0 if it hasn't been set
		 */
		static Platform_OS* GetPlatform_OS();

		/**
		 * Retrieves the pointer to the global instance of the Platform_Time system.
		 * The value returned might be 0 if no instance have been specified through
		 * a call to SetPlatform_Time, and the caller should test the return value to
		 * make sure it is a valid pointer.
		 *
		 * \returns	A pointer to the Platform_Time instance, or 0 if it hasn't been set
		 */
		static Platform_Time* GetPlatform_Time();


		/**
		 * Retrieves the pointer to the global instance of the Platform_Sound system.
		 * The value returned might be 0 if no instance have been specified through
		 * a call to SetPlatform_Sound, and the caller should test the return value to
		 * make sure it is a valid pointer.
		 *
		 * \returns	A pointer to the Platform_Sound instance, or 0 if it hasn't been set
		 */
		static Platform_Sound* GetPlatform_Sound();


		/**
		 * Retrieves the pointer to the global instance of the Platform_Input system.
		 * The value returned might be 0 if no instance have been specified through
		 * a call to SetPlatform_Input, and the caller should test the return value to
		 * make sure it is a valid pointer.
		 *
		 * \returns	A pointer to the Platform_Input instance, or 0 if it hasn't been set
		 */
		static Platform_Input* GetPlatform_Input();


		/**
		 * Retrieves the pointer to the global instance of the Platform_FileSystem system.
		 * The value returned might be 0 if no instance have been specified through
		 * a call to SetPlatform_FileSystem, and the caller should test the return value to
		 * make sure it is a valid pointer.
		 *
		 * \returns	A pointer to the Platform_FileSystem instance, or 0 if it hasn't been set
		 */
		static Platform_FileSystem* GetPlatform_FileSystem();


		/**
		 * Retrieves the pointer to the global instance of the Platform_Screen system.
		 * The value returned might be 0 if no instance have been specified through
		 * a call to SetPlatform_Screen, and the caller should test the return value to
		 * make sure it is a valid pointer.
		 *
		 * \returns	A pointer to the Platform_Screen instance, or 0 if it hasn't been set
		 */
		static Platform_Screen* GetPlatform_Screen();


		/**
		 * Retrieves the pointer to the global instance of the Platform_Network system.
		 * The value returned might be 0 if no instance have been specified through
		 * a call to SetPlatform_Network, and the caller should test the return value to
		 * make sure it is a valid pointer.
		 *
		 * \returns	A pointer to the Platform_Network instance, or 0 if it hasn't been set
		 */
		static Platform_Network* GetPlatform_Network();


		/**
		 * Retrieves the pointer to the global instance of the Platform_3D system.
		 * The value returned might be 0 if no instance have been specified through
		 * a call to SetPlatform_3D, and the caller should test the return value to
		 * make sure it is a valid pointer.
		 *
		 * \returns	A pointer to the Platform_3D instance, or 0 if it hasn't been set
		 */
		static Platform_3D* GetPlatform_3D();


		/**
		 * To make it easier for the platform specific implementations to communicate 
		 * without specific knowledge of each other, a simple event system is provided.
		 * By deriving a platform system from PlatformEventListener, and register it for
		 * events using this methods, the system will be notified every time another
		 * platform system sends a platform event.
		 */
		static void RegisterEventListener(
			PlatformEventListener* listener	///< Object that is to receive platform events
			);
		
		/**
		 * Unregisters an event listener that was registered through a call to 
		 * RegisterEventListener. You can call this method to stop receiving events, but
		 * you MUST call it when before the registered object is destroyed (typically from
		 * its destructor).
		 */
		static void UnregisterEventListener(
			PlatformEventListener* listener	///< Object that should stop receiving platform events
			);

		/**
		 * Send the "OsYield" platform event to all registered event listeners. This is a special
		 * event, which is sent by Platform_OS every time the OsYield method is called, and can
		 * be used for systems which needs to do per-frame updates.
		 */
		static void SendEvent_OsYield();

		/**
		 * Sends a custom event to all registered event listeners. A custom event can be anything -
		 * it is defined by an id string, for what type of event it is, and an optional pointer to
		 * user-defined data.
		 */
		static void SendEvent_CustomEvent
			(
			const char* eventId,	///< The id defining the type of event
			void* userData = 0	///< Pointer to optional user data for this event
			);

	private:
		// Constructor/Destructor is private - we only ever want the one static instance of this class
		Platform();
		~Platform();

		static Platform platform_;	///< Singleton instance of the Platform class
		
		static Platform_OS* platform_OS_;	///< Current instance of Platform_OS. Defaults to 0
		static Platform_Time* platform_Time_;	///< Current instance of Platform_Time. Defaults to 0
		static Platform_Sound* platform_Sound_;	///< Current instance of Platform_Sound. Defaults to 0
		static Platform_Input* platform_Input_;	///< Current instance of Platform_Input. Defaults to 0
		static Platform_FileSystem* platform_FileSystem_;	///< Current instance of Platform_FileSystem. Defaults to 0
		static Platform_Screen* platform_Screen_;	///< Current instance of Platform_Screen. Defaults to 0
		static Platform_Network* platform_Network_;	///< Current instance of Platform_Network. Defaults to 0
		static Platform_3D* platform_3D_;	///< Current instance of Platform_3D. Defaults to 0

		static const int MaxEventListeners = 64;	///< The maximum number of event listeners - should be well enough
		static PlatformEventListener* eventListeners_[MaxEventListeners];	///< Array storing the event listeners
		static int eventListenerCount_;	///< The current number of registered event listeners

	private:
		// The following will generate a compiler error if there is an attempt
		// to create a second instance of the class
		Platform(const Platform&);	///< Copy constructor, not implemented, only accessible from within the class 
		const Platform& operator=(const Platform&);	///< Copy assignment, not implemented, only accessible from within the class 

	};

#endif /* __Platform_H__ */
