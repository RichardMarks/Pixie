//*** Platform_Win32_Network_Server.cpp ***

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <winsock2.h>

#pragma warning (push)
#pragma warning( disable: 4127)
#pragma warning( disable: 4706)
#include <ws2tcpip.h>
#pragma warning (pop)

#include "Platform_Win32_Network_Server.h"

#include "Platform_Win32_Network.h"
#include "Debug.h"
#include "StandardLibrary.h"


//*** Constructor (listening) ***

Platform_Win32_Network_Server::Platform_Win32_Network_Server(Platform_Win32_Network* network, Platform_Network::ConnectionMode mode, int port):
	network_(network),
	isListening_(false),
	isConnected_(false),
	mode_(mode),
	port_(port),
	socket_(0)
	{
	/*
	-1. Initialize Winsock. 
	-2. Create a socket. 
	-3. Bind the socket. 
	4. Listen on the socket for a client. 
	5. Accept a connection from a client. 
	6. Receive and send data. 
	7. Disconnect. 
	*/


	// Set up hints for getaddrinfo
	addrinfo hints;
	MemSet(&hints,0,sizeof(hints));
	hints.ai_family = AF_INET;	// The Internet Protocol version 4 (IPv4) address family.
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

	// Resolve the local address and port to be used by the server
	addrinfo* result = 0;
	char portString[256];
	SNPrintF(portString,256,"%d",port_); // We need the port as a string
	if (getaddrinfo(0, portString, &hints, &result)!=0)
		{
		DebugPrint(("getaddrinfo - FAILED\n"));
		network_->DisableNetworking();
		return;
		}
	

	// Create the socket
	socket_ = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (socket_==INVALID_SOCKET)
		{
		freeaddrinfo(result);
		DebugPrint(("FAILED creating socket!\n"));
		network_->DisableNetworking();
		return;
		}


	// Bind the socket to our local server address
	if (bind(socket_, result->ai_addr, (int)result->ai_addrlen)==SOCKET_ERROR)
		{
		freeaddrinfo(result);
		closesocket(socket_);
		socket_=INVALID_SOCKET;
		DebugPrint(("FAILED binding socket!\n"));
		network_->DisableNetworking();
		return;
		}

	// Make the socket listen
	if (listen(socket_, SOMAXCONN)==SOCKET_ERROR)	// Set the maximum number of connections to lots
		{
		freeaddrinfo(result);
		closesocket(socket_);
		socket_=INVALID_SOCKET;
		DebugPrint(("listen FAILED!\n"));
		network_->DisableNetworking();
		return;
		}

	freeaddrinfo(result);
	}


//*** Constructor ***
/*
Platform_Win32_Network_Server::Platform_Win32_Network_Server(Platform_Network::ConnectionMode mode, const char* address, int port):
	isListening_(false),
	mode_(mode),
	port_(port),
	address_(0),
	socket_(0)
	{
	address_=StrDup(address);

	//
	//1. Initialize Winsock. 
	//2. Create a socket. 
	//3. Connect to the server. 
	//4. Send and receive data. 
	//5. Disconnect. 
	///

	// Store information about the server
	LPHOSTENT hostEntry;

	hostEntry = gethostbyname(address_);	// Specifying the server by its name;
							// another option is gethostbyaddr() (see below)

	if (!hostEntry) {
		int nret = WSAGetLastError();
		nret; // Avoid warning: unreferenced formal parameter

	}
	// USE THIS INSTEAD!! The other one is deprecated
//	LPHOSTENT hostEntry;
//	in_addr iaHost;
//
//	iaHost.s_addr = inet_addr("204.52.135.52");
//
//	hostEntry = gethostbyaddrstatic_cast<const char*>(&iaHost), sizeof(struct in_addr), AF_INET);
//
//	if (!hostEntry) {
//		nret = WSAGetLastError();
//		ReportError(nret, "gethostbyaddr()");	// Report the error as before
//
//		WSACleanup();
//		return NETWORK_ERROR;
//	}
//


	// Create the socket
	if (mode_==Platform_Network::ConnectionMode_TCP)
		{
		socket_=socket(	AF_INET,		// The Internet Protocol version 4 (IPv4) address family.
						SOCK_STREAM,	// Socket type that provides sequenced, reliable, two-way, connection-based byte streams with an OOB data transmission mechanism. 
						IPPROTO_TCP);	// Use Transmission Control Protocol (TCP).

		}
	else
		{
		socket_=socket(	AF_INET,		// The Internet Protocol version 4 (IPv4) address family.
						SOCK_DGRAM,		// Socket type that supports datagrams, which are connectionless, unreliable buffers of a fixed (typically small) maximum length.
						IPPROTO_UDP);	// Use TUser Datagram Protocol (UDP). 

		}

	Assert(socket_!=INVALID_SOCKET,"Failed to create socket"); 


	// Fill a SOCKADDR_IN struct with address information
	SOCKADDR_IN serverInfo;

	serverInfo.sin_family = AF_INET;

	// At this point, we've successfully retrieved vital information about the server,
	// including its hostname, aliases, and IP addresses.  Wait; how could a single
	// computer have multiple addresses, and exactly what is the following line doing?
	// See the explanation below.

	serverInfo.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);

	serverInfo.sin_port = htons(80);		// Change to network-byte order and
							// insert into port field


	// Connect to the server
	int nret = connect(socket_,
		       (LPSOCKADDR)&serverInfo,
		       sizeof(struct sockaddr));

	if (nret == SOCKET_ERROR) {
		nret = WSAGetLastError();
	}


	// Successfully connected!


	// Send/receive, then cleanup:
	closesocket(socket_);
	WSACleanup();
	}
*/


//*** Destructor ***

Platform_Win32_Network_Server::~Platform_Win32_Network_Server()
	{
	
	if (socket_!=INVALID_SOCKET)
		{
		closesocket(socket_);
		socket_=INVALID_SOCKET;
		}
	}


//*** IsListening ***

bool Platform_Win32_Network_Server::IsListening()
	{
	return isListening_;
	}


//*** GetConnectionMode ***

Platform_Network::ConnectionMode Platform_Win32_Network_Server::GetConnectionMode()
	{
	return mode_;
	}


//*** GetAddress ***
/*
const char* Platform_Win32_Network_Server::GetAddress()
	{
	return address_;
	}
*/

//*** GetPort ***

int Platform_Win32_Network_Server::GetPort()
	{
	return port_;
	}


//*** Connect ***
/*
bool Platform_Win32_Network_Server::Connect()
	{
	return false;
	}
*/

//*** Disconnect ***

bool Platform_Win32_Network_Server::Disconnect()
	{
	return false;
	}


//*** IsConnected ***

bool Platform_Win32_Network_Server::IsConnected()
	{
	return false;
	}


//*** SendData ***

int Platform_Win32_Network_Server::SendData(void* buffer, int size)
	{
//	char buffer[256];		// Declaring a buffer on the stack
//	char *buffer = new char[256];	// or on the heap

	ZeroMemory(buffer, 256);
//	strcpy(buffer, "Pretend this is important data.");

	int nret = send(socket_,
			static_cast<char*>(buffer),
			size,	// Note that this specifies the length of the string; not
					// the size of the entire buffer
			0);			// Most often is zero, but see MSDN for other options

	delete [] buffer;		// If and only if the heap declaration was used

	if (nret == SOCKET_ERROR) {
		// Get a specific code
		// Handle accordingly
		return 0;
	} else {
		// nret contains the number of bytes sent
	}

	return 0;
	}


//*** GetReceivedData ***

int Platform_Win32_Network_Server::GetReceivedSize()
	{
	return 0;
	}


//*** GetReceivedData ***

int Platform_Win32_Network_Server::GetReceivedData(void* buffer, int size)
	{
	int nret = recv(socket_,
			static_cast<char*>(buffer),
			size,		// Complete size of buffer
			0);

	delete [] buffer;		// Manipulate buffer, then delete if and only if
					// buffer was allocated on heap

	if (nret == SOCKET_ERROR) {
		// Get a specific code
		// Handle accordingly
		return 0;
	} else {
		// nret contains the number of bytes received
	}


	return 0;
	}


//*** Update ***

void Platform_Win32_Network_Server::Update()
	{
/*	if (isListening_)
		{
		// Wait for a client
		SOCKET theClient;

		theClient = accept(socket_,
				NULL,			// Address of a sockaddr structure (see explanation below)
				NULL);			// Address of a variable containing size of sockaddr struct

		if (theClient == INVALID_SOCKET) {
			nret = WSAGetLastError();
		}


		// Send and receive from the client, and finally,
		closesocket(theClient);
		closesocket(socket_);
		}

*/	}
	

bool Platform_Win32_Network_Server::Listen()
	{
	return true;
	}
