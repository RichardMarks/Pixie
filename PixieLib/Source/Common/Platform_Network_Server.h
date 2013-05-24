/**
 * \class	Platform_Network_Server
 *
 * \ingroup	platform
 * \brief	Platform independent abstraction of a Platform_Network
 * \author	Mattias Gustavsson	
 * 
 * \todo	Documentation
 */

#ifndef __Platform_Network_Server_H__
#define __Platform_Network_Server_H__

// Includes
#include "Platform_Network.h"

// Forward declares


// Platform_Network_Server
class Platform_Network_Server
	{
	public:
		virtual ~Platform_Network_Server() { };
		
		virtual Platform_Network::ConnectionMode GetConnectionMode() = 0;
		virtual int GetPort() = 0;

		virtual bool IsConnected() = 0;

		virtual int SendData(
			void* data, 
			int size
			) = 0;

		virtual int GetReceivedSize() = 0;
		virtual int GetReceivedData(
			void* buffer, 
			int size
			) = 0;
	};

#endif /* __Platform_Network_H__ */
