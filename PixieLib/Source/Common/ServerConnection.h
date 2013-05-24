/**
 * \class ServerConnection
 * 
 * \ingroup	network
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __ServerConnection_H__
#define __ServerConnection_H__

// Includes

// Forward declares
class StaticBuffer;
class DynamicBuffer;

// ServerConnection
class ServerConnection
	{
	public:

		/**
		 * Constructor
		 */
		ServerConnection();


		/**
		 * Destructor
		 */
		virtual ~ServerConnection();

		
		/**
		 * Creates a server connection listening for incoming client
		 * connections on the specified port, using TCP or UDP or both.
		 */
		void Connect(
			int port,
			bool useTCP, 
			bool useUDP
			);


		/**
		 * Creates a server connection listening for incoming client
		 * connections using a memory buffer to transfer data. Used
		 * for single player modes.
		 */
		void ConnectSP();


		/** 
		 * Terminates the connection
		 */
		void Disconnect();


		/** 
		 * Should be called every frame to process the connection
		 */ 
		void Update(float deltaTime);


		/**
		 * Sends a data packet to the specified client using TCP 
		 * protocol, ensuring that the data will arrive, and that 
		 * data is received by the client in the same order as it 
		 * was transmitted. TCP is slower than UDP, but reliable.
		 */
		void TransmitTCP(
			int clientId,	///< Client to send data to
			const StaticBuffer& data ///< Data to send
			);
		
		/**
		 * Sends a data packet to all the clients using TCP 
		 * protocol, ensuring that the data will arrive, and that 
		 * data is received by the clients in the same order as it 
		 * was transmitted. TCP is slower than UDP, but reliable.
		 */
		void TransmitTCP(
			const StaticBuffer& data ///< Data to send
			);


		/**
		 * Sends a data packet to the specified client using UDP. 
		 * It is not guaranteed to arrive at all, and if it does, 
		 * data will not be received by the client in the same order 
		 * as it was transmitted. UDP is faster than TCP, but is not
		 * reliable.
		 */
		void TransmitUDP(
			int clientId,	///< Client to send data to
			const StaticBuffer& data ///< Data to send
			);


		/**
		 * Sends a data packet to all clients through UDP. It is 
		 * not guaranteed to arrive at all, and if it does, data 
		 * will not be received by the clients in the same order 
		 * as it was transmitted. UDP is faster than TCP, but not 
		 * reliable.
		 */
		void TransmitUDP(
			const StaticBuffer& data ///< Data to send
			);


		/**
		 * When a client has connected to the server, this method
		 * is called. Implement it in your derived class to do
		 * what you want done when a client has connected.
		 */
		virtual void OnClientConnected(
			int clientId	///< Id of the connected client
			) = 0;


		/**
		 * When a client has disconnected to the server, this method
		 * is called. Implement it in your derived class to do
		 * what you want done when a client has disconnected.
		 */
		virtual void OnClientDisconnected(
			int clientId	///< Id of the disconnected client
			) = 0;

		/**
		 * Whenever a data package is received, this method is
		 * called. Implement it in your derived class to handle
		 * network messages.
		 */
		virtual void OnReceiveData(
			int clientId,	///< Id of the client sending the data 
			const StaticBuffer& data	///< Data being received
			) = 0;


	private:
		bool localCommunications_;	///< Set to true if using a memory buffer to transfer data instead of a network protocol
		bool localCommunications_OnConnected_Done_;	///< Set to true if the OnClientConnected message have been sent

	private:
		friend class ClientConnection;
		static DynamicBuffer* localServerCommunicationsBuffer_;
		static int localServerCommunicationsBuffer_referenceCount_;
	};


#endif /* __ServerConnection_H__ */

