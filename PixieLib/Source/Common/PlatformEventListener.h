/**
 * \class	PlatformEventListener
 * 
 * \ingroup	platform
 * \brief	Event receiver interface for Platform system
 * \author	Mattias Gustavsson	
 * 
 * When information needs to be sent between different Platform systems,
 * it is done by a basic event system. Systems that listen for platform
 * events, derive from this class to received them.
 */
 

#ifndef __PlatformEventListener_H__
#define __PlatformEventListener_H__

// Includes

// Forward declares


// PlatformEventListener
class PlatformEventListener
	{
	public:
		/**
		 * Receives the OsYield event, sent from Platform_OS when OsYield is called
		 */
		virtual void OnOsYield() { };

		/**
		 * Receives a custom event, sent from a system by calling Platform::SendEvent_CustomEvent
		 */
		virtual void OnCustomEvent(
			const char* eventId,	///< Id-string specifying the type of event
			void* userData	///< Event specific user data 
			) { };
	};

#endif /* __PlatformEventListener_H__ */
