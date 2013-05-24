/**
 * \class	Platform_Win32_Network_Server
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_Win32_Network_Server_H__
#define __Platform_Win32_Network_Server_H__

// Includes
#include "Platform_Network_Server.h"

// Forward declares
class Platform_Win32_Network;

// Platform_Win32_Network_Server
class Platform_Win32_Network_Server: public Platform_Network_Server
	{
	public:
		Platform_Win32_Network_Server(Platform_Win32_Network* network, Platform_Network::ConnectionMode mode, int port);
		virtual ~Platform_Win32_Network_Server();

		virtual Platform_Network::ConnectionMode GetConnectionMode();
		virtual int GetPort();

		virtual bool Listen();
		virtual bool Disconnect();
		virtual bool IsListening();
		virtual bool IsConnected();

		virtual int SendData(
			void* data, 
			int size
			);

		virtual int GetReceivedSize();
		virtual int GetReceivedData(
			void* buffer, 
			int size
			);

		void Update();

	private:
		Platform_Win32_Network* network_;
		Platform_Network::ConnectionMode mode_;
		int port_;
		SOCKET socket_;
		bool isListening_;
		bool isConnected_;
	};


#endif /* __Platform_Win32_Network_Server_H__ */
