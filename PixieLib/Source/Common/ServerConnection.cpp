//*** ServerConnection.cpp ***

#include "ServerConnection.h"
#include "ClientConnection.h"
#include "DynamicBuffer.h"
#include "StaticBuffer.h"
#include "Debug.h"


// Static memory buffer used when running in local communications (non networked) mode
DynamicBuffer* ServerConnection::localServerCommunicationsBuffer_ = 0;
int ServerConnection::localServerCommunicationsBuffer_referenceCount_ = 0;


//*** Constructor ***

ServerConnection::ServerConnection():
	localCommunications_(false),
	localCommunications_OnConnected_Done_(true)
	{

	}


//*** Destructor ***

ServerConnection::~ServerConnection()
	{
	if (localCommunications_)
		{
		localServerCommunicationsBuffer_referenceCount_--;
		if (localServerCommunicationsBuffer_referenceCount_==0)
			{
			delete localServerCommunicationsBuffer_;
			localServerCommunicationsBuffer_=0;
			}
		}
	}


//*** Connect ***

void ServerConnection::Connect(int port, bool useTCP, bool useUDP)
	{
	Assert(false,"Not implemented yet");
	}


//*** ConnectSP ***

void ServerConnection::ConnectSP()
	{
	Assert(!localCommunications_,"Already connected");
	if (localCommunications_)
		{
		return;
		}
	
	localCommunications_=true;
	
	if (!localServerCommunicationsBuffer_)
		{
		localServerCommunicationsBuffer_=new DynamicBuffer(1024);
		localServerCommunicationsBuffer_referenceCount_++;
		}

	localCommunications_OnConnected_Done_=false;
	}


//*** Disconnect ***

void ServerConnection::Disconnect()
	{
	if (localCommunications_)
		{
		localServerCommunicationsBuffer_referenceCount_--;
		if (localServerCommunicationsBuffer_referenceCount_==0)
			{
			delete localServerCommunicationsBuffer_;
			localServerCommunicationsBuffer_=0;
			}

		localCommunications_=false;

		OnClientDisconnected(0);
		localCommunications_OnConnected_Done_=true;
		}
	
	}


//*** Update ***


void ServerConnection::Update(float deltaTime)
	{
	// This is for local communications mode (non networked mode)
	if (localCommunications_)
		{
		// Send the OnConnected event if this is the first time we Update after a Connect
		if (!localCommunications_OnConnected_Done_)
			{
			OnClientConnected(0);
			localCommunications_OnConnected_Done_=true;
			}

		// Process all messages in the buffer
		if (localServerCommunicationsBuffer_)
			{
			localServerCommunicationsBuffer_->SetPosition(0);
			while (localServerCommunicationsBuffer_->GetPosition()<localServerCommunicationsBuffer_->GetSize())
				{
				int size=0;
				localServerCommunicationsBuffer_->Read(&size);
				OnReceiveData(0,StaticBuffer(static_cast<unsigned char*>(localServerCommunicationsBuffer_->GetPointer())+localServerCommunicationsBuffer_->GetPosition(),size));
				localServerCommunicationsBuffer_->SetPosition(localServerCommunicationsBuffer_->GetPosition()+size);
				}

			localServerCommunicationsBuffer_->Clear(false);
			}
		}
	}	


//*** TransmitTCP ***

void ServerConnection::TransmitTCP(int clientId, const StaticBuffer& data)
	{
	Assert(localCommunications_,"Network support not implemented yet");

	if (localCommunications_)
		{
		Assert(ClientConnection::localClientCommunicationsBuffer_,"Local client communications buffer not created");
		if (!ClientConnection::localClientCommunicationsBuffer_)
			{
			return;
			}

		int size=data.GetSize();
		ClientConnection::localClientCommunicationsBuffer_->Write(&size);
		ClientConnection::localClientCommunicationsBuffer_->Write(static_cast<const unsigned char*>(data.GetPointer()), size);
		}
	}


//*** TransmitTCP ***

void ServerConnection::TransmitTCP(const StaticBuffer& data)
	{
	Assert(localCommunications_,"Network support not implemented yet");

	if (localCommunications_)
		{
		Assert(ClientConnection::localClientCommunicationsBuffer_,"Local client communications buffer not created");
		if (!ClientConnection::localClientCommunicationsBuffer_)
			{
			return;
			}
		
		int size=data.GetSize();
		ClientConnection::localClientCommunicationsBuffer_->Write(&size);
		ClientConnection::localClientCommunicationsBuffer_->Write(static_cast<const unsigned char*>(data.GetPointer()), size);
		}
	}


//*** TransmitUDP ***

void ServerConnection::TransmitUDP(int clientId, const StaticBuffer& data)
	{
	Assert(localCommunications_,"Network support not implemented yet");

	if (localCommunications_)
		{
		Assert(ClientConnection::localClientCommunicationsBuffer_,"Local client communications buffer not created");
		if (!ClientConnection::localClientCommunicationsBuffer_)
			{
			return;
			}
		
		int size=data.GetSize();
		ClientConnection::localClientCommunicationsBuffer_->Write(&size);
		ClientConnection::localClientCommunicationsBuffer_->Write(static_cast<const unsigned char*>(data.GetPointer()), size);
		}
	}


//*** TransmitUDP ***

void ServerConnection::TransmitUDP(const StaticBuffer& data)
	{
	Assert(localCommunications_,"Network support not implemented yet");

	if (localCommunications_)
		{
		Assert(ClientConnection::localClientCommunicationsBuffer_,"Local client communications buffer not created");
		if (!ClientConnection::localClientCommunicationsBuffer_)
			{
			return;
			}
		
		int size=data.GetSize();
		ClientConnection::localClientCommunicationsBuffer_->Write(&size);
		ClientConnection::localClientCommunicationsBuffer_->Write(static_cast<const unsigned char*>(data.GetPointer()), size);
		}
	}
