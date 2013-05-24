/**
 * \class	Platform_Network_Client
 *
 * \ingroup	platform
 * \brief	Platform independent abstraction of a Platform_Network_Client
 * \author	Mattias Gustavsson	
 * 
 * \todo	Documentation
 */

#ifndef __Platform_Network_Client_H__
#define __Platform_Network_Client_H__

// Includes
#include "Platform_Network.h"

// Forward declares
class DynamicBuffer;

// Platform_Network_Client
class Platform_Network_Client
	{
	public:
		virtual ~Platform_Network_Client() { };
		
		virtual Platform_Network::ConnectionMode GetConnectionMode() = 0;
		virtual const char* GetAddress() = 0;
		virtual int GetPort() = 0;

		virtual bool IsConnected() = 0;
		virtual bool Connect() = 0;
		virtual bool Disconnect() = 0;

		virtual void SendData(
			const void* data, 
			int size
			) = 0;

		virtual int GetReceivedDataSize() = 0;

		virtual void GetReceivedData(
			DynamicBuffer* buffer
			) = 0;
	};

#endif /* __Platform_Network_Client_H__ */
