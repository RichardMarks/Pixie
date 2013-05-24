/**
 * \class	Platform_Network
 * 
 * \ingroup	platform
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	Documentation
 */

#ifndef __Platform_Network_H__
#define __Platform_Network_H__

// Includes
#include "Platform.h"

// Forward declares
class Platform_Network_Server;
class Platform_Network_Client;

// Platform_Network
class Platform_Network
	{
	public:
		virtual ~Platform_Network() { };

		enum ConnectionMode
			{
			ConnectionMode_TCP,	
			ConnectionMode_UDP,
			};

		virtual Platform_Network_Server* CreateServer(
			ConnectionMode mode, 
			int port
			) = 0;

		virtual Platform_Network_Client* CreateClient(
			ConnectionMode mode, 
			const char* address, 
			int port
			) = 0;
	};

#endif /* __Platform_Network_H__ */
