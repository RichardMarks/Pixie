//*** Platform_Win32_Network_Client.cpp ***

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <winsock2.h>

#pragma warning (push)
#pragma warning( disable: 4127)
#pragma warning( disable: 4706)
#include <ws2tcpip.h>
#pragma warning (pop)

#include "Platform_Win32_Network_Client.h"

#include "Platform_Win32_Network.h"
#include "Debug.h"
#include "StandardLibrary.h"


//*** Constructor (listening) ***

Platform_Win32_Network_Client::Platform_Win32_Network_Client(Platform_Win32_Network* network, Platform_Network::ConnectionMode mode, const char* address, int port):
	network_(network),
	mode_(mode),
	address_(0),
	port_(port),
	socket_(INVALID_SOCKET),
	connected_(false),
	waitingToConnect_(false),
	maxMessageSize_(0)
	{
	// Save the address
	address_=StrDup(address);
	}


//*** Destructor ***

Platform_Win32_Network_Client::~Platform_Win32_Network_Client()
	{
	// Disconnect
	if (socket_!=INVALID_SOCKET)
		{
		if (closesocket(socket_)!=0)
			{
			const char* errorMsg=network_->GetErrorMessage(WSAGetLastError());
			DebugPrint(("closesocket FAILED: %d - %s\n",WSAGetLastError(),errorMsg));
			}
		socket_=INVALID_SOCKET;
		}

	// Notify manager
	network_->ClientDestroyed(this);

	// Free address
	if (address_)
		{
		Free(const_cast<char*>(address_));
		}
	}


//*** GetConnectionMode ***

Platform_Network::ConnectionMode Platform_Win32_Network_Client::GetConnectionMode()
	{
	return mode_;
	}


//*** GetAddress ***

const char* Platform_Win32_Network_Client::GetAddress()
	{
	return address_;
	}


//*** GetPort ***

int Platform_Win32_Network_Client::GetPort()
	{
	return port_;
	}


//*** Connect ***

bool Platform_Win32_Network_Client::Connect()
	{
	// Close existing connection
	if (socket_!=INVALID_SOCKET)
		{
		if (closesocket(socket_)!=0)
			{
			const char* errorMsg=network_->GetErrorMessage(WSAGetLastError());
			DebugPrint(("closesocket FAILED: %d - %s\n",WSAGetLastError(),errorMsg));
			}
		socket_=INVALID_SOCKET;
		connected_=false;
		waitingToConnect_=false;
		}

	// Reset receive buffer
	receivedData_.Clear(false);

	// Set up hints for getaddrinfo
	addrinfo hints;
	MemSet(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;	// The Internet Protocol version 4 (IPv4) address family.
	hints.ai_flags = AI_PASSIVE;

	if (mode_==Platform_Network::ConnectionMode_TCP)
		{
		hints.ai_socktype = SOCK_STREAM;	// Socket type that provides sequenced, reliable, two-way, connection-based byte streams with an OOB data transmission mechanism. 
		hints.ai_protocol = IPPROTO_TCP;	// Use Transmission Control Protocol (TCP).
		}
	else if (mode_==Platform_Network::ConnectionMode_UDP)
		{
		hints.ai_socktype = SOCK_DGRAM;		// Socket type that supports datagrams, which are connectionless, unreliable buffers of a fixed (typically small) maximum length.
		hints.ai_protocol = IPPROTO_UDP;	// Use User Datagram Protocol (UDP). 
		}

	// Resolve the server address and port
	addrinfo* result = 0;
	char portString[256];
	SNPrintF(portString,256,"%d",port_); // We need the port as a string
	int error=getaddrinfo(address_, portString, &hints, &result);
	if (error!=0)
		{
		const char* errorMsg=network_->GetErrorMessage(error);
		DebugPrint(("getaddrinfo FAILED: %d - %s\n",error,errorMsg));
		return false;
		}

	// Create the socket
	socket_ = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (socket_==INVALID_SOCKET)
		{
		const char* errorMsg=network_->GetErrorMessage(WSAGetLastError());
		DebugPrint(("socket FAILED: %d - %s\n",WSAGetLastError(),errorMsg));
	
		freeaddrinfo(result);

		DebugPrint(("FAILED creating socket!\n"));
		return false;
		}

	// Set socket to nonblocking mode
	u_long nonblocking = 1;
	if (ioctlsocket(socket_, FIONBIO, &nonblocking)==SOCKET_ERROR)
		{
		const char* errorMsg=network_->GetErrorMessage(WSAGetLastError());
		DebugPrint(("ioctlsocket FAILED: %d - %s\n",WSAGetLastError(),errorMsg));

		freeaddrinfo(result);
		if (closesocket(socket_)!=0)
			{
			const char* errorMsg=network_->GetErrorMessage(WSAGetLastError());
			DebugPrint(("closesocket FAILED: %d - %s\n",WSAGetLastError(),errorMsg));
			}

		socket_=INVALID_SOCKET;
		DebugPrint(("FAILED setting nonblocking mode on socket!\n"));
		return false;
		}

	// Get maximum message size for this socket
	int optLen=sizeof(maxMessageSize_);
	if (getsockopt(socket_,SOL_SOCKET,SO_MAX_MSG_SIZE,reinterpret_cast<char*>(&maxMessageSize_),&optLen)==SOCKET_ERROR)
		{
		const char* errorMsg=network_->GetErrorMessage(WSAGetLastError());
		DebugPrint(("getsockopt FAILED: %d - %s\n",WSAGetLastError(),errorMsg));

		freeaddrinfo(result);
		if (closesocket(socket_)!=0)
			{
			const char* errorMsg=network_->GetErrorMessage(WSAGetLastError());
			DebugPrint(("closesocket FAILED: %d - %s\n",WSAGetLastError(),errorMsg));
			}

		socket_=INVALID_SOCKET;
		DebugPrint(("FAILED retrieving maximum message size\n"));
		return false;
		}

	// Connect to server
	if (connect(socket_, result->ai_addr, (int)result->ai_addrlen)==SOCKET_ERROR)
		{
		if (WSAGetLastError()!=WSAEWOULDBLOCK)
			{
			const char* errorMsg=network_->GetErrorMessage(WSAGetLastError());
			DebugPrint(("connect FAILED: %d - %s\n",WSAGetLastError(),errorMsg));

			freeaddrinfo(result);
			
			if (closesocket(socket_)!=0)
				{
				const char* errorMsg=network_->GetErrorMessage(WSAGetLastError());
				DebugPrint(("closesocket FAILED: %d - %s\n",WSAGetLastError(),errorMsg));
				}

			socket_=INVALID_SOCKET;
			DebugPrint(("FAILED connecting socket!\n"));
			return false;
			}
		waitingToConnect_=true;
		}
	else
		{
		connected_=true;
		}

	freeaddrinfo(result);
	return true;
	}


//*** Disconnect ***

bool Platform_Win32_Network_Client::Disconnect()
	{
	if (socket_!=INVALID_SOCKET)
		{
		closesocket(socket_);
		socket_=INVALID_SOCKET;
		}

	return true;
	}


//*** IsConnected ***

bool Platform_Win32_Network_Client::IsConnected()
	{
	return connected_;
	}


//*** SendData ***

void Platform_Win32_Network_Client::SendData(const void* buffer, int size)
	{
	dataToSend_.Write(static_cast<const char*>(buffer),size);
	}


//*** GetReceivedDataSize ***

int Platform_Win32_Network_Client::GetReceivedDataSize()
	{
	return receivedData_.GetSize();
	}


//*** GetReceivedData ***

void Platform_Win32_Network_Client::GetReceivedData(DynamicBuffer* buffer)
	{
	// Get data size
	int size=receivedData_.GetSize();

	// Copy data to callers buffer
	buffer->Write(static_cast<char*>(receivedData_.GetPointer()),size);

	// Reset buffer
	receivedData_.Clear(false);
	}


//*** Update ***

void Platform_Win32_Network_Client::Update()
	{
	// Detect connection established
	if (waitingToConnect_)
		{
		fd_set socketsToCheck;
		socketsToCheck.fd_count=1;
		socketsToCheck.fd_array[0]=socket_;

		timeval timeout;
		timeout.tv_sec=0;
		timeout.tv_usec=0;

		// Check if socket is ready for send
		if (select(0,0,&socketsToCheck,0,&timeout)==1)
			{
			// If it is, we're connected
			connected_=true;
			waitingToConnect_=false;
			}

		}
	
	// Send and receive data
	if (connected_)
		{
		timeval timeout;
		timeout.tv_sec=0;
		timeout.tv_usec=0;

		fd_set socketsToCheck;

		// Check if socket is ready for send, and we have data buffered for send
		socketsToCheck.fd_count=1;
		socketsToCheck.fd_array[0]=socket_;
		if (select(0,0,&socketsToCheck,0,&timeout)==1 && dataToSend_.GetSize()>0)
			{
			// Attempt to send all buffered data
			int dataSize=dataToSend_.GetSize();
			int result=send(socket_,static_cast<char*>(dataToSend_.GetPointer()),Min(dataSize,1024*1024),0);

			// On fail, we close the socket and mark the connection as lost
			if (result==SOCKET_ERROR)
				{
				const char* errorMsg=network_->GetErrorMessage(WSAGetLastError());
				DebugPrint(("senddata FAILED: %d - %s\n",WSAGetLastError(),errorMsg));

				if (closesocket(socket_)!=0)
					{
					const char* errorMsg=network_->GetErrorMessage(WSAGetLastError());
					DebugPrint(("closesocket FAILED: %d - %s\n",WSAGetLastError(),errorMsg));
					}
				socket_=INVALID_SOCKET;
				connected_=false;
				waitingToConnect_=false;
				DebugPrint(("FAILED sending data!\n"));
				return;
				}

			// If we managed to send all buffered data, empty the send buffer
			if (dataSize==result)
				{
				dataToSend_.Clear(false);
				}
			else if (result>0)
				{
				// Remove the data that was successfully sent from the send buffer
				char* data=static_cast<char*>(dataToSend_.GetPointer());
				MemCpy(data,data+result,dataSize-result);
				dataToSend_.SetPosition(dataSize-result);
				dataToSend_.SetSize(dataSize-result);
				}
			}

		// Check if socket is ready for recv
		socketsToCheck.fd_count=1;
		socketsToCheck.fd_array[0]=socket_;
		if (select(0,&socketsToCheck,0,0,&timeout)==1)
			{
			char buffer[4096];

			// Receive data
			int size=recv(socket_,buffer,4096,0);
			if (size==SOCKET_ERROR)
				{
				const char* errorMsg=network_->GetErrorMessage(WSAGetLastError());
				DebugPrint(("recv FAILED: %d - %s\n",WSAGetLastError(),errorMsg));

				if (closesocket(socket_)!=0)
					{
					const char* errorMsg=network_->GetErrorMessage(WSAGetLastError());
					DebugPrint(("closesocket FAILED: %d - %s\n",WSAGetLastError(),errorMsg));
					}

				socket_=INVALID_SOCKET;
				connected_=false;
				waitingToConnect_=false;
				DebugPrint(("FAILED receiving data!\n"));
				}
			else if (size>0)
				{
				// Copy data to buffer
				receivedData_.Write(buffer,size);
				}
			}

		}

	}
