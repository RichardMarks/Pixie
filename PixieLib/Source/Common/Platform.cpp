//*** Platform.cpp ***

#include "Platform.h"
#include "Platform_OS.h"
#include "Platform_Time.h"
#include "Platform_Sound.h"
#include "Platform_Input.h"
#include "Platform_FileSystem.h"
#include "Platform_Screen.h"
#include "Platform_Network.h"
#include "Platform_3D.h"

// Singleton instance of the platform system
Platform Platform::platform_;

// Instances of the platform subsystems
Platform_OS* Platform::platform_OS_=0;
Platform_Time* Platform::platform_Time_=0;
Platform_Sound* Platform::platform_Sound_=0;
Platform_Input* Platform::platform_Input_=0;
Platform_FileSystem* Platform::platform_FileSystem_=0;
Platform_Screen* Platform::platform_Screen_=0;
Platform_Network* Platform::platform_Network_=0;
Platform_3D* Platform::platform_3D_=0;
PlatformEventListener* Platform::eventListeners_[64];
int Platform::eventListenerCount_=0;


///*** Constructor ***

Platform::Platform()
	{
	}


///*** Destructor ***

Platform::~Platform()
	{
	if (platform_Time_)
		{
		delete platform_Time_;
		platform_Time_=0;
		}

	if (platform_Input_)
		{
		delete platform_Input_;
		platform_Input_=0;
		}

	if (platform_FileSystem_)
		{
		delete platform_FileSystem_;
		platform_FileSystem_=0;
		}

	if (platform_Network_)
		{
		delete platform_Network_;
		platform_Network_=0;
		}

	if (platform_Sound_)
		{
		delete platform_Sound_;
		platform_Sound_=0;
		}

	if (platform_Screen_)
		{
		delete platform_Screen_;
		platform_Screen_=0;
		}

	if (platform_3D_)
		{
		delete platform_3D_;
		platform_3D_=0;
		}

	if (platform_OS_)
		{
		delete platform_OS_;
		platform_OS_=0;
		}
	}


//*** RegisterEventListener ***

void Platform::RegisterEventListener(PlatformEventListener* listener)
	{
	// Add the listener to the end of the listener array, if there's room
	if (eventListenerCount_<MaxEventListeners-1)
		{
		eventListeners_[eventListenerCount_]=listener;
		eventListenerCount_++;
		}
	}


//*** UnregisterEventListener ***

void Platform::UnregisterEventListener(PlatformEventListener* listener)
	{
	// Go through all registered event listeners, to find the one we need to remove
	for (int i=0; i<eventListenerCount_; i++)
		{
		if (eventListeners_[i]==listener)
			{
			// Remove it from the list, and fill its slot with the last one
			eventListeners_[i]=eventListeners_[eventListenerCount_-1];
			eventListenerCount_--;
			return;
			}
		}
	}


//*** SendEvent_OsYield ***

void Platform::SendEvent_OsYield()
	{
	// Send the event to each registered listener
	for (int i=0; i<eventListenerCount_; i++)
		{
		eventListeners_[i]->OnOsYield();
		}
	}


//*** Event_CustomEvent ***

void Platform::SendEvent_CustomEvent(const char* eventId,void* userData)
	{
	// Send the event to each registered listener
	for (int i=0; i<eventListenerCount_; i++)
		{
		eventListeners_[i]->OnCustomEvent(eventId,userData);
		}
	}


		
//*** GetPlatform_OS ***

Platform_OS* Platform::GetPlatform_OS()
	{
	return platform_OS_;
	}


//*** SetPlatform_OS ***

void Platform::SetPlatform_OS(Platform_OS* instance)
	{
	if (platform_OS_==0)
		{
		platform_OS_=instance;
		}
	else
		{
		delete instance;
		}
	}


//*** GetPlatform_Time ***

Platform_Time* Platform::GetPlatform_Time()
	{
	return platform_Time_;
	}


//*** SetPlatform_Time ***

void Platform::SetPlatform_Time(Platform_Time* instance)
	{
	if (platform_Time_==0)
		{
		platform_Time_=instance;
		}
	else
		{
		delete instance;
		}
	}


//*** GetPlatform_Sound ***

Platform_Sound* Platform::GetPlatform_Sound()
	{
	return platform_Sound_;
	}


//*** SetPlatform_Sound ***

void Platform::SetPlatform_Sound(Platform_Sound* instance)
	{
	if (platform_Sound_==0)
		{
		platform_Sound_=instance;
		}
	else
		{
		delete instance;
		}
	}


//*** GetPlatform_Input ***

Platform_Input* Platform::GetPlatform_Input()
	{
	return platform_Input_;
	}


//*** SetPlatform_Input ***

void Platform::SetPlatform_Input(Platform_Input* instance)
	{
	if (platform_Input_==0)
		{
		platform_Input_=instance;
		}
	else
		{
		delete instance;
		}
	}


//*** GetPlatform_FileSystem ***

Platform_FileSystem* Platform::GetPlatform_FileSystem()
	{
	return platform_FileSystem_;
	}


//*** SetPlatform_FileSystem ***

void Platform::SetPlatform_FileSystem(Platform_FileSystem* instance)
	{
	if (platform_FileSystem_==0)
		{
		platform_FileSystem_=instance;
		}
	else
		{
		delete instance;
		}
	}


//*** GetPlatform_Screen ***

Platform_Screen* Platform::GetPlatform_Screen()
	{
	return platform_Screen_;
	}


//*** SetPlatform_Screen ***

void Platform::SetPlatform_Screen(Platform_Screen* instance)
	{
	if (platform_Screen_==0)
		{
		platform_Screen_=instance;
		}
	else
		{
		delete instance;
		}
	}


//*** GetPlatform_Network ***

Platform_Network* Platform::GetPlatform_Network()
	{
	return platform_Network_;
	}


//*** SetPlatform_Network ***

void Platform::SetPlatform_Network(Platform_Network* instance)
	{
	if (platform_Network_==0)
		{
		platform_Network_=instance;
		}
	else
		{
		delete instance;
		}
	}


//*** GetPlatform_3D ***

Platform_3D* Platform::GetPlatform_3D()
	{
	return platform_3D_;
	}


//*** SetPlatform_3D ***

void Platform::SetPlatform_3D(Platform_3D* instance)
	{
	if (platform_3D_==0)
		{
		platform_3D_=instance;
		}
	else
		{
		delete instance;
		}
	}
