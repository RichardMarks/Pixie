/**
 * \class	Platform_Win32_Network
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_Win32_Network_H__
#define __Platform_Win32_Network_H__

// Includes
#include "Platform_Network.h"
#include "Array.h"

// Forward declares
class Platform_Win32_Network_Server;
class Platform_Win32_Network_Client;

// Platform_Win32_Network
class Platform_Win32_Network:public Platform_Network,PlatformEventListener
	{

	public:
		Platform_Win32_Network();
		~Platform_Win32_Network();

		virtual void OnOsYield();

		virtual Platform_Network_Server* CreateServer(
			ConnectionMode mode, 
			int port
			);
		
		virtual Platform_Network_Client* CreateClient(
			ConnectionMode mode, 
			const char* address, 
			int port
			);


		bool IsNetworkingEnabled();
		void DisableNetworking();

		bool UsingWinsock2();

		void ClientDestroyed(Platform_Win32_Network_Client* client);
		void ServerDestroyed(Platform_Win32_Network_Server* server);

		const char* GetErrorMessage(unsigned int errorCode);
	private:
		bool networkingEnabled_;
		bool usingWinsock2_;
		Array<Platform_Win32_Network_Server*> servers_;
		Array<Platform_Win32_Network_Client*> clients_;
	};

#endif /* __Platform_Win32_Network_H__ */
