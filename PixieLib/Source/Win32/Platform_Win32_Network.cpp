//*** Platform_Win32_Network.cpp ***

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <winsock2.h>

#include "Platform_Win32_Network.h"
#include "Platform_Win32_Network_Server.h"
#include "Platform_Win32_Network_Client.h"
#include "Debug.h"
#include "ArrayIterator.h"

#pragma comment (lib, "Ws2_32.lib") 



//*** Constructor ***

Platform_Win32_Network::Platform_Win32_Network()
	{
	Platform::RegisterEventListener(this);

	networkingEnabled_=false;
	usingWinsock2_=false;


	WSADATA wsaData;

    // Initialize Winsock. Try the highest version first, then fall back 
	// to older and older versions until we get one which works
	if (WSAStartup(MAKEWORD(2,2), &wsaData)==0)
		{
		// Found Winsock 2.2
		DebugPrint(("WSAStartup - Using Winsock 2.2\n"));
		networkingEnabled_=true;
		usingWinsock2_=true;
		}
	else if (WSAStartup(MAKEWORD(2,1), &wsaData)==0)
		{
		// Found Winsock 2.1
		DebugPrint(("WSAStartup - Using Winsock 2.1\n"));
		networkingEnabled_=true;
		usingWinsock2_=true;
		}
	else if (WSAStartup(MAKEWORD(2,0), &wsaData)==0)
		{
		// Found Winsock 2.0
		DebugPrint(("WSAStartup - Using Winsock 2.0\n"));
		networkingEnabled_=true;
		usingWinsock2_=true;
		}
	else if (WSAStartup(MAKEWORD(1,1), &wsaData)==0)
		{
		// Found Winsock 1.1
		DebugPrint(("WSAStartup - Using Winsock 1.1\n"));
		networkingEnabled_=true;
		usingWinsock2_=false;
		}
	else if (WSAStartup(MAKEWORD(1,0), &wsaData)==0)
		{
		// Found Winsock 1.0
		DebugPrint(("WSAStartup - Using Winsock 1.0\n"));
		networkingEnabled_=true;
		usingWinsock2_=false;
		}
	else
		{
		// WSAStartup failed, no winsock available
		DebugPrint(("WSAStartup - FAILED\n"));
		DebugPrint(("Networking not available\n"));
		networkingEnabled_=false;
		usingWinsock2_=false;
		return;
		}

	}


//*** Destructor ***

Platform_Win32_Network::~Platform_Win32_Network()
	{
	for (int i=0; i<clients_.GetItemCount(); i++)
		{
		clients_.Get(i)->Disconnect();
		}

	for (int i=0; i<servers_.GetItemCount(); i++)
		{
		servers_.Get(i)->Disconnect();
		}

	// Shutdown Winsock
	if (WSACleanup()==0)
		{
		DebugPrint(("WSACleanup done\n"));
		}
	else
		{
		DebugPrint(("WSACleanup FAILED\n"));
		}


	Platform::UnregisterEventListener(this);
	}



//*** OnOsYield ***

void Platform_Win32_Network::OnOsYield()
	{
	for (int i=0; i<servers_.GetItemCount(); i++)
		{
		servers_.Get(i)->Update();
		}

	for (int i=0; i<clients_.GetItemCount(); i++)
		{
		clients_.Get(i)->Update();
		}
	}


//*** IsNetworkingEnabled ***

bool Platform_Win32_Network::IsNetworkingEnabled()
	{
	return networkingEnabled_;
	}


//*** UsingWinsock2 ***

bool Platform_Win32_Network::UsingWinsock2()
	{
	return usingWinsock2_;
	}


//*** DisableNetworking ***

void Platform_Win32_Network::DisableNetworking()
	{
	DebugPrint(("An error occured in Winsock - Networking is disabled"));
	networkingEnabled_=false;
	usingWinsock2_=false;
	}


//*** ClientDestroyed ***

void Platform_Win32_Network::ClientDestroyed(Platform_Win32_Network_Client* client)
	{
	ArrayIterator<Platform_Win32_Network_Client*> it(clients_);
	if (it.Find(client))
		{
		clients_.Remove(it);
		}
	}


//*** ServerDestroyed ***

void Platform_Win32_Network::ServerDestroyed(Platform_Win32_Network_Server* server)
	{
	ArrayIterator<Platform_Win32_Network_Server*> it(servers_);
	if (it.Find(server))
		{
		servers_.Remove(it);
		}
	}


//*** CreateServer ***

Platform_Network_Server* Platform_Win32_Network::CreateServer(ConnectionMode mode, int port)
	{
	// Check to make sure networking is available
	if (!networkingEnabled_)
		{
		return 0;
		}

	// Create the connection
	Platform_Win32_Network_Server* server=new Platform_Win32_Network_Server(this,mode,port);

	// Add the connection to the list of active clients
	servers_.Add(server);

	// Return the connection
	return server;
	}


//*** CreateClient ***

Platform_Network_Client* Platform_Win32_Network::CreateClient(ConnectionMode mode, const char* address, int port)
	{
	// Check to make sure networking is available
	if (!networkingEnabled_)
		{
		return 0;
		}

	// Create the connection
	Platform_Win32_Network_Client* client=new Platform_Win32_Network_Client(this,mode,address,port);

	// Add the connection to the list of active clients
	clients_.Add(client);

	// Return the connection
	return client;
	}


//*** GetErrorMessage ***

const char* Platform_Win32_Network::GetErrorMessage(unsigned int errorCode)
	{
	// Define all error messages as an array of unnamed structs, mapping error code to error message
	struct 
		{ 
		unsigned int errorCode; 
		const char* errorMessage; 
		} errorTexts[] = 	
		{
			{ WSAEINTR, "A blocking operation was interrupted by a call to WSACancelBlockingCall.." },
			{ WSAEBADF, "The file handle supplied is not valid." },
			{ WSAEACCES, "An attempt was made to access a socket in a way forbidden by its access permissions." },
			{ WSAEFAULT, "The system detected an invalid pointer address in attempting to use a pointer argument in a call." },
			{ WSAEINVAL, "An invalid argument was supplied." },
			{ WSAEMFILE, "Too many open sockets." },
			{ WSAEWOULDBLOCK, "A non-blocking socket operation could not be completed immediately." },
			{ WSAEINPROGRESS, "A blocking operation is currently executing." },
			{ WSAEALREADY, "An operation was attempted on a non-blocking socket that already had an operation in progress." },
			{ WSAENOTSOCK, "An operation was attempted on something that is not a socket." },
			{ WSAEDESTADDRREQ, "A required address was omitted from an operation on a socket." },
			{ WSAEMSGSIZE, "A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram into was smaller than the datagram itself." },
			{ WSAEPROTOTYPE, "A protocol was specified in the socket function call that does not support the semantics of the socket type requested." },
			{ WSAENOPROTOOPT, "An unknown, invalid, or unsupported option or level was specified in a getsockopt or setsockopt call." },
			{ WSAEPROTONOSUPPORT, "The requested protocol has not been configured into the system, or no implementation for it exists." },
			{ WSAESOCKTNOSUPPORT, "The support for the specified socket type does not exist in this address family." },
			{ WSAEOPNOTSUPP, "The attempted operation is not supported for the type of object referenced." },
			{ WSAEPFNOSUPPORT, "The protocol family has not been configured into the system or no implementation for it exists." },
			{ WSAEAFNOSUPPORT, "An address incompatible with the requested protocol was used." },
			{ WSAEADDRINUSE, "Only one usage of each socket address (protocol/network address/port) is normally permitted." },
			{ WSAEADDRNOTAVAIL, "The requested address is not valid in its context." },
			{ WSAENETDOWN, "A socket operation encountered a dead network." },
			{ WSAENETUNREACH, "A socket operation was attempted to an unreachable network." },
			{ WSAENETRESET, "The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress." },
			{ WSAECONNABORTED, "An established connection was aborted by the software in your host machine." },
			{ WSAECONNRESET, "An existing connection was forcibly closed by the remote host." },
			{ WSAENOBUFS, "An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full." },
			{ WSAEISCONN, "A connect request was made on an already connected socket." },
			{ WSAENOTCONN, "A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied." },
			{ WSAESHUTDOWN, "A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call." },
			{ WSAETOOMANYREFS, "Too many references to some kernel object." },
			{ WSAETIMEDOUT, "A connection attempt failed because the connected party did not properly respond after a period of time, or established connection failed because connected host has failed to respond." },
			{ WSAECONNREFUSED, "No connection could be made because the target machine actively refused it." },
			{ WSAELOOP, "Cannot translate name." },
			{ WSAENAMETOOLONG, "Name component or name was too long." },
			{ WSAEHOSTDOWN, "A socket operation failed because the destination host was down." },
			{ WSAEHOSTUNREACH, "A socket operation was attempted to an unreachable host." },
			{ WSAENOTEMPTY, "Cannot remove a directory that is not empty." },
			{ WSAEPROCLIM, "A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously." },
			{ WSAEUSERS, "Ran out of quota." },
			{ WSAEDQUOT, "Ran out of disk quota." },
			{ WSAESTALE, "File handle reference is no longer available." },
			{ WSAEREMOTE, "Item is not available locally." },
			{ WSASYSNOTREADY, "WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable." },
			{ WSAVERNOTSUPPORTED, "The Windows Sockets version requested is not supported." },
			{ WSANOTINITIALISED, "Either the application has not called WSAStartup, or WSAStartup failed." },
			{ WSAEDISCON, "Returned by WSARecv or WSARecvFrom to indicate the remote party has initiated a graceful shutdown sequence." },
			{ WSAENOMORE, "No more results can be returned by WSALookupServiceNext." },
			{ WSAECANCELLED, "A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled." },
			{ WSAEINVALIDPROCTABLE, "The procedure call table is invalid." },
			{ WSAEINVALIDPROVIDER, "The requested service provider is invalid." },
			{ WSAEPROVIDERFAILEDINIT, "The requested service provider could not be loaded or initialized." },
			{ WSASYSCALLFAILURE, "A system call that should never fail has failed." },
			{ WSASERVICE_NOT_FOUND, "No such service is known. The service cannot be found in the specified name space." },
			{ WSATYPE_NOT_FOUND, "The specified class was not found." },
			{ WSA_E_NO_MORE, "No more results can be returned by WSALookupServiceNext." },
			{ WSA_E_CANCELLED, "A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled." },
			{ WSAEREFUSED, "A database query failed because it was actively refused." },
			{ WSAHOST_NOT_FOUND, "No such host is known." },
			{ WSATRY_AGAIN, "This is usually a temporary error during hostname resolution and means that the local server did not receive a response from an authoritative server." },
			{ WSANO_RECOVERY, "A non-recoverable error occurred during a database lookup." },
			{ WSANO_DATA, "The requested name is valid, but no data of the requested type was found." },
			{ WSA_QOS_RECEIVERS, "At least one reserve has arrived." },
			{ WSA_QOS_SENDERS, "At least one path has arrived." },
			{ WSA_QOS_NO_SENDERS, "There are no senders." },
			{ WSA_QOS_NO_RECEIVERS, "There are no receivers." },
			{ WSA_QOS_REQUEST_CONFIRMED, "Reserve has been confirmed." },
			{ WSA_QOS_ADMISSION_FAILURE, "Error due to lack of resources." },
			{ WSA_QOS_POLICY_FAILURE, "Rejected for administrative reasons - bad credentials." },
			{ WSA_QOS_BAD_STYLE, "Unknown or conflicting style." },
			{ WSA_QOS_BAD_OBJECT, "Problem with some part of the filterspec or providerspecific buffer in general." },
			{ WSA_QOS_TRAFFIC_CTRL_ERROR, "Problem with some part of the flowspec." },
			{ WSA_QOS_GENERIC_ERROR, "General QOS error." },
			{ WSA_QOS_ESERVICETYPE, "An invalid or unrecognized service type was found in the flowspec." },
			{ WSA_QOS_EFLOWSPEC, "An invalid or inconsistent flowspec was found in the QOS structure." },
			{ WSA_QOS_EPROVSPECBUF, "Invalid QOS provider-specific buffer." },
			{ WSA_QOS_EFILTERSTYLE, "An invalid QOS filter style was used." },
			{ WSA_QOS_EFILTERTYPE, "An invalid QOS filter type was used." },
			{ WSA_QOS_EFILTERCOUNT, "An incorrect number of QOS FILTERSPECs were specified in the FLOWDESCRIPTOR." },
			{ WSA_QOS_EOBJLENGTH, "An object with an invalid ObjectLength field was specified in the QOS provider-specific buffer." },
			{ WSA_QOS_EFLOWCOUNT, "An incorrect number of flow descriptors was specified in the QOS structure." },
			{ WSA_QOS_EUNKOWNPSOBJ, "An unrecognized object was found in the QOS provider-specific buffer." },
			{ WSA_QOS_EPOLICYOBJ, "An invalid policy object was found in the QOS provider-specific buffer." },
			{ WSA_QOS_EFLOWDESC, "An invalid QOS flow descriptor was found in the flow descriptor list." },
			{ WSA_QOS_EPSFLOWSPEC, "An invalid or inconsistent flowspec was found in the QOS provider specific buffer." },
			{ WSA_QOS_EPSFILTERSPEC, "An invalid FILTERSPEC was found in the QOS provider-specific buffer." },
			{ WSA_QOS_ESDMODEOBJ, "An invalid shape discard mode object was found in the QOS provider specific buffer." },
			{ WSA_QOS_ESHAPERATEOBJ, "An invalid shaping rate object was found in the QOS provider-specific buffer." },
			{ WSA_QOS_RESERVED_PETYPE, "A reserved policy element was found in the QOS provider-specific buffer." },
		} ;

	// Find the entry with matching error code
	for (int i=0; i<sizeof(errorTexts)/sizeof(errorTexts[0]); i++)
		{
		if (errorTexts[i].errorCode==errorCode)
			{
			// Return the corresponding error message
			return errorTexts[i].errorMessage;
			}
		}

	// If an unknown error code was passed in, return a default message
	return "Unknown error!";
	};