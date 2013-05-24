/**
 * \class ClientConnection
 * 
 * \ingroup	network
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __ClientConnection_H__
#define __ClientConnection_H__

// Includes

// Forward declares
class StaticBuffer;
class DynamicBuffer;
class Platform_Network_Client;

// ClientConnection
class ClientConnection
	{
	public:
		/**
		 * Constructor
		 */
		ClientConnection();


		/**
		 * Destructor
		 */
		virtual ~ClientConnection();

		
		/**
		 * Creates a client connection to the server on the specified 
		 * address on the specified port, using TCP or UDP or both.
		 */
		void Connect(
			const char* address,	///< IP or URL of server to connect to
			int port,	///< Port to connect on
			bool useTCP, ///< Set to true to enable the TCP protocol
			bool useUDP	///< Set to true to enable the UDP protocol
			);


		/**
		 * Creates a client connection to the server using a memory
		 * buffer to transfer data. Used for single player modes.
		 */
		void ConnectSP();


		/**
		 * Creates a client connection to the server using the HTTP
		 * protocol. A connection is actually established every time
		 * data is transferred, and is not kept alive in between. It
		 * can still be used in the same way as the other modes, and
		 * you only call ConnectHTTP once, and clean it up by calling
		 * Disconnect (actual connections are handled automatically).
		 */
		void ConnectHTTP(
			const char* address,	///< IP or URL of server to connect to
			int port,	///< Port to connect on
			const char* resource = "/"  ///< The server resource to connect to. This is typically 
										///< the absolute path to a php file. Defaults to server root.
			);

		/** 
		 * Terminates the connection
		 */
		void Disconnect();


		/** 
		 * Should be called every frame to process the connection
		 */ 
		void Update(float deltaTime);


		/**
		 * Sends a data packet to the server using TCP protocol, 
		 * ensuring that the data will arrive, and that data is
		 * received by the server in the same order as it was 
		 * transmitted. TCP is slower than UDP, but reliable.
		 */
		void TransmitTCP(
			const StaticBuffer& data ///< Data to send
			);
	

		/**
		 * Sends a data packet through UDP. It is not guaranteed
		 * to arrive at all, and if it does, data will not be 
		 * received by the server in the same order as it was
		 * transmitted. UDP is faster than TCP, but unreliable.
		 */
		void TransmitUDP(
			const StaticBuffer& data ///< Data to send
			);

		/**
		 * When a connection has been established, this method
		 * is called. Implement it in your derived class to do
		 * what you want done when the client has been connected.
		 */
		virtual void OnConnected() = 0;


		/**
		 * When a connection has been terminated, this methods 
		 * called. Implement it in your derived class to do what 
		 * you want done when the client has been disconnected.
		 */
		virtual void OnDisconnected() = 0;


		/**
		 * Whenever a data package is received, this method is
		 * called. Implement it in your derived class to handle
		 * network messages from the server.
		 */
		virtual void OnReceiveData(
			const StaticBuffer& data	///< Data being received
			) = 0;


	private:
		bool localCommunications_;	///< Set to true if using a memory buffer to transfer data instead of a network protocol
		bool localCommunications_OnConnected_Done_;	///< Set to true if the OnConnected message have been sent
		Platform_Network_Client* connectionTCP_;	///< If TCP is being used, this is the active TCP connection.
		Platform_Network_Client* connectionUDP_;	///< If UDP us being used, this is the active UDP connection.
		Platform_Network_Client* connectionHTTP_;	///< If using the HTTP protocol to transfer data, this is the TCP connection.

	private:
		friend class ServerConnection;
		static DynamicBuffer* localClientCommunicationsBuffer_;
		static int localClientCommunicationsBuffer_referenceCount_;
	};


#endif /* __ClientConnection_H__ */

