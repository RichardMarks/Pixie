//*** ClientConnection.cpp ***

#include "ClientConnection.h"
#include "ServerConnection.h"
#include "Platform_Network_Client.h"
#include "DynamicBuffer.h"
#include "StaticBuffer.h"
#include "Debug.h"


// Static memory buffer used when running in local communications (non networked) mode
DynamicBuffer* ClientConnection::localClientCommunicationsBuffer_ = 0;
int ClientConnection::localClientCommunicationsBuffer_referenceCount_ = 0;


//*** Constructor ***

ClientConnection::ClientConnection():
	localCommunications_(false),
	localCommunications_OnConnected_Done_(true),
	connectionTCP_(0),
	connectionUDP_(0),
	connectionHTTP_(false)
	{

	}


//*** Destructor ***

ClientConnection::~ClientConnection()
	{
	if (connectionTCP_)
		{
		connectionTCP_->Disconnect();
		delete connectionTCP_;
		}

	if (connectionUDP_)
		{
		connectionUDP_->Disconnect();
		delete connectionUDP_;
		}

	if (connectionHTTP_)
		{
		connectionHTTP_->Disconnect();
		delete connectionHTTP_;
		}

	if (localCommunications_)
		{
		localClientCommunicationsBuffer_referenceCount_--;
		if (localClientCommunicationsBuffer_referenceCount_==0)
			{
			delete localClientCommunicationsBuffer_;
			localClientCommunicationsBuffer_=0;
			}
		}	
	}


//*** Connect ***

void ClientConnection::Connect(const char* address, int port,	bool useTCP, bool useUDP)
	{
	Assert(!localCommunications_ && !connectionTCP_ && !connectionUDP_ && !connectionHTTP_,"Already connected");
	if (localCommunications_ || connectionTCP_ || connectionUDP_ || connectionHTTP_)
		{
		return;
		}

	Assert(false,"Not implemented yet");
	}


//*** ConnectSP ***

void ClientConnection::ConnectSP()
	{
	Assert(!localCommunications_ && !connectionTCP_ && !connectionUDP_ && !connectionHTTP_,"Already connected");
	if (localCommunications_ || connectionTCP_ || connectionUDP_ || connectionHTTP_)
		{
		return;
		}
	
	localCommunications_=true;
	
	if (!localClientCommunicationsBuffer_)
		{
		localClientCommunicationsBuffer_=new DynamicBuffer(1024);
		localClientCommunicationsBuffer_referenceCount_++;
		}

	localCommunications_OnConnected_Done_=false;
	}



//*** ConnectHTTP ***

void ClientConnection::ConnectHTTP(const char* address, int port, const char* resource)
	{
	Assert(!localCommunications_ && !connectionTCP_ && !connectionUDP_ && !connectionHTTP_,"Already connected");
	if (localCommunications_ || connectionTCP_ || connectionUDP_ || connectionHTTP_)
		{
		return;
		}

	// Create connection
	if (Platform::GetPlatform_Network())
		{
		connectionHTTP_=Platform::GetPlatform_Network()->CreateClient(Platform_Network::ConnectionMode_TCP,address,port);
		}


	// Send initial request to server, to test connection


	Assert(false,"Not implemented yet");
	}


//*** Disconnect ***

void ClientConnection::Disconnect()
	{
	if (connectionTCP_)
		{
		connectionTCP_->Disconnect();
		delete connectionTCP_;
		connectionTCP_=0;
		}

	if (connectionUDP_)
		{
		connectionUDP_->Disconnect();
		delete connectionUDP_;
		connectionUDP_=0;
		}

	if (connectionHTTP_)
		{
		connectionHTTP_->Disconnect();
		delete connectionHTTP_;
		connectionHTTP_=0;
		}

	if (localCommunications_)
		{
		localClientCommunicationsBuffer_referenceCount_--;
		if (localClientCommunicationsBuffer_referenceCount_==0)
			{
			delete localClientCommunicationsBuffer_;
			localClientCommunicationsBuffer_=0;
			}

		localCommunications_=false;
		OnDisconnected();
		localCommunications_OnConnected_Done_=true;
		}
	
	}


//*** Update ***

void ClientConnection::Update(float deltaTime)
	{
	// This is for local communications mode (non networked mode)
	if (localCommunications_)
		{
		// Send the OnConnected event if this is the first time we Update after a Connect
		if (!localCommunications_OnConnected_Done_)
			{
			OnConnected();
			localCommunications_OnConnected_Done_=true;
			}

		// Process all messages in the buffer
		if (localClientCommunicationsBuffer_)
			{
			localClientCommunicationsBuffer_->SetPosition(0);
			while (localClientCommunicationsBuffer_->GetPosition()<localClientCommunicationsBuffer_->GetSize())
				{
				int size=0;
				localClientCommunicationsBuffer_->Read(&size);
				OnReceiveData(StaticBuffer(static_cast<unsigned char*>(localClientCommunicationsBuffer_->GetPointer())+localClientCommunicationsBuffer_->GetPosition(),size));
				localClientCommunicationsBuffer_->SetPosition(localClientCommunicationsBuffer_->GetPosition()+size);
				}

			localClientCommunicationsBuffer_->Clear(false);
			}
		}
	}


//*** TransmitTCP ***

void ClientConnection::TransmitTCP(const StaticBuffer& data)
	{
	Assert(localCommunications_,"Network support not implemented yet");

	if (localCommunications_)
		{
		Assert(ServerConnection::localServerCommunicationsBuffer_,"Local communications buffer not created");
		if (!ServerConnection::localServerCommunicationsBuffer_)
			{
			return;
			}
		
		int size=data.GetSize();
		ServerConnection::localServerCommunicationsBuffer_->Write(&size);
		ServerConnection::localServerCommunicationsBuffer_->Write(static_cast<const unsigned char*>(data.GetPointer()), size);
		}
	}


//*** TransmitUDP ***

void ClientConnection::TransmitUDP(const StaticBuffer& data)
	{
	Assert(localCommunications_,"Network support not implemented yet");

	if (localCommunications_)
		{
		Assert(ServerConnection::localServerCommunicationsBuffer_,"Local communications buffer not created");
		if (!ServerConnection::localServerCommunicationsBuffer_)
			{
			return;
			}

		int size=data.GetSize();
		ServerConnection::localServerCommunicationsBuffer_->Write(&size);
		ServerConnection::localServerCommunicationsBuffer_->Write(static_cast<const unsigned char*>(data.GetPointer()), size);
		}
	}

