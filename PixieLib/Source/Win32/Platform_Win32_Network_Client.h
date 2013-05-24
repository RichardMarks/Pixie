/**
 * \class	Platform_Win32_Network_Client
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_Win32_Network_Client_H__
#define __Platform_Win32_Network_Client_H__

// Includes
#include "Platform_Network.h"
#include "Platform_Network_Client.h"
#include "DynamicBuffer.h"

// Forward declares
class Platform_Win32_Network;

// Platform_Win32_Network_Client
class Platform_Win32_Network_Client: public Platform_Network_Client
	{
	public:
		Platform_Win32_Network_Client(Platform_Win32_Network* network, Platform_Network::ConnectionMode mode, const char* address, int port);
		virtual ~Platform_Win32_Network_Client();

		virtual Platform_Network::ConnectionMode GetConnectionMode();
		virtual const char* GetAddress();
		virtual int GetPort();

		virtual bool Connect();
		virtual bool Disconnect();
		virtual bool IsConnected();

		virtual void SendData(
			const void* data, 
			int size
			);

		virtual int GetReceivedDataSize();

		virtual void GetReceivedData(
			DynamicBuffer* buffer
			);

		void Update();
	private:
		Platform_Win32_Network* network_;
		Platform_Network::ConnectionMode mode_;
		int port_;
		const char* address_;
		SOCKET socket_;
		bool waitingToConnect_;
		bool connected_;
		DynamicBuffer receivedData_;
		DynamicBuffer dataToSend_;
		int maxMessageSize_;
	};

#endif /* __Platform_Win32_Network_Client_H__ */
