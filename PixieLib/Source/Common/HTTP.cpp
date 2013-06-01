//*** HTTP.cpp ***

#include "HTTP.h"
#include "Platform_Network.h"
#include "Platform_Network_Client.h"
#include "StandardLibrary.h"
#include "HTTP_Resource.h"
#include "BagIterator.h"



//*** Constructor***

HTTP::HTTP():
	currentRequestHandle_(0)
	{
	}


//*** Destructor ***

HTTP::~HTTP()
	{
	while (requests_.GetItemCount()>0)
		{
		DiscardRequest(requests_.Get(0)->handle);
		}

	}


//*** Update ***

void HTTP::Update(float deltaTime)
	{
	for (int i=0; i<requests_.GetItemCount(); i++)
		{
		Request* request=requests_.Get(i);
		
		// Process the request depending on its current status
		switch (request->status)
			{
			case Status_Connecting:
				{
				if (request->connection && request->connection->IsConnected())
					{
					// Define the template for HTTP Get header
					const char* headerGet="GET %s HTTP/1.0\r\nHost: %s:%d\r\n\r\n";
					const char* headerPost="POST %s HTTP/1.0\r\nHost: %s:%d\r\nContent-Length: %d\r\n\r\n";
					const char* header=0;
					if (request->usingGetMethod)
						{
						header=headerGet;
						}
					else
						{
						header=headerPost;
						}


					// Calculate maximum length of request string
					int maxLength=StrLen(header)+StrLen(request->requestString)+StrLen(request->connection->GetAddress())+10;

					// Build request string
					char* requestString=new char[maxLength];
					SNPrintF(requestString,maxLength,header,request->requestString,request->connection->GetAddress(),request->connection->GetPort(),request->postData.GetSize());

					// Send HTTP request
					request->connection->SendData(requestString,StrLen(requestString));
					if (!request->usingGetMethod)
						{
						request->connection->SendData(request->postData.GetPointer(),request->postData.GetSize());
						}

					// Release request string
					delete[] requestString;

					// Change request status to wait for result
					request->status=Status_Pending;
					}
				} break;

			case Status_Pending:
				{
				// Are there any data to receive?
				if (request->connection && request->connection->GetReceivedDataSize()>0)
					{
					// Is this the first chunk of data we receive?
					bool firstChunk=false;
					if (request->receivedData.GetSize()==0)
						{
						firstChunk=true;
						}

					// Receive the data (will be added to the end of the buffer)
					request->connection->GetReceivedData(&request->receivedData);

					// If this is the first chunk we receive, we need to process the header
					if (firstChunk)
						{
						// Check Status Line
						char* statusLine=static_cast<char*>(request->receivedData.GetPointer());

						// Get header size
						int headerSize=0;
						const char* headerEnd=StrStr(statusLine,"\r\n\r\n");
						if (headerEnd)
							{
							headerEnd+=4;
							headerSize=(int)(headerEnd-statusLine);
							}

						// Extract httpVersion
						char httpVersion[256];
						const char* ptr=StrChr(statusLine,' ');
						StrNCpy(httpVersion,statusLine,Min((int)(ptr-statusLine),255));
						httpVersion[ptr-statusLine]=0;
						ptr++;
						
						// Extract statusCode
						char statusCode[256];
						const char* ptr2=StrChr(ptr,' ');
						StrNCpy(statusCode,ptr,Min((int)(ptr2-ptr),255));
						statusCode[ptr2-ptr]=0;
						ptr2++;

						// Extract reasonPhrase
						char reasonPhrase[256];
						const char* ptr3=StrStr(ptr2,"\r\n");
						StrNCpy(reasonPhrase,ptr2,Min((int)(ptr3-ptr2),255));
						reasonPhrase[ptr3-ptr2]=0;
						ptr3++;

                        // Extract contentLength
						char contentLengthStr[256];
						StrCpy(contentLengthStr,"0");
						const char* ptr4=StrStr(ptr3,"Content-Length: ");
						if (ptr4)
							{
							ptr4+=16;
							const char* ptr5=StrStr(ptr4,"\r\n");
							StrNCpy(contentLengthStr,ptr4,Min((int)(ptr5-ptr4),255));
							contentLengthStr[ptr5-ptr4]=0;
							}

						char contentTypeStr[256];
						StrCpy(contentTypeStr,"");
						const char* ptr6=StrStr(ptr3,"Content-Type: ");
						if (ptr6)
							{
							ptr6+=14;
							const char* ptr7=StrStr(ptr6,"\r\n");
							StrNCpy(contentTypeStr,ptr6,Min((int)(ptr7-ptr6),255));
							contentTypeStr[ptr7-ptr6]=0;
							}


						// Check status
						int status=StringToInt(statusCode);
						int contentLength=StringToInt(contentLengthStr);
						StringId contentType=StringId(contentTypeStr);

						if (status!=200 || contentLength==0 || contentType==StringId(""))
							{
							request->status=Status_Failed;
							}
						else
							{
							request->headerLength=headerSize;
							request->contentType=contentType;
							request->contentLength=contentLength;
							}                  
						}

					// Check for dropped connection
					if (!request->connection->IsConnected())
						{
						request->status=Status_Failed;
						}

					// Check if we've received the whole resource
					if (request->status!=Status_Failed && request->receivedData.GetSize()==(unsigned int)request->contentLength+request->headerLength)
						{
						request->status=Status_Completed;

						if (request->resource)
							{
							delete request->resource;
							}
						char* data=static_cast<char*>(request->receivedData.GetPointer());
						data+=request->headerLength;
						StaticBuffer buffer(data,request->contentLength);
						request->resource=new HTTP_Resource(request->handle,request->contentType,buffer);
						}

					}
				} break;

			}

			if (request->status==Status_Connecting || request->status==Status_Pending)
				{
				// Increase the elapsed time
				request->elapsedTime_+=deltaTime;
			
				if (request->timeOut_>0.0f && request->elapsedTime_>request->timeOut_)
					{
					request->status=Status_TimedOut;
					}
				}
		}

	}


//*** Request_Get ***

int HTTP::Request_Get(const char* url, float timeOut)
	{
	// Create new request entry, and add it to list of request entries
	Request* request=new Request;
	currentRequestHandle_++;
	request->handle=currentRequestHandle_;
	request->usingGetMethod=true;
	request->status=Status_Connecting;
	request->timeOut_=timeOut;
	request->elapsedTime_=0;
	request->connection=0;
	request->requestString=0;
	request->contentLength=0;
	request->resource=0;
	requests_.Add(request);

	// Make sure URL starts with http://
	if (StrNICmp(url,"http://",7)!=0)
		{
		request->status=Status_Failed;
		return request->handle;
		}

	// Split URL string
	char* address=0;
	char* port=0;
	char* requestString=0;
	SplitURL(url,address,port,requestString);

	// Create connection
	if (Platform::GetPlatform_Network())
		{
		request->connection=Platform::GetPlatform_Network()->CreateClient(Platform_Network::ConnectionMode_TCP,address,StringToInt(port));
		}
	if (!request->connection->Connect())
		{
		request->status=Status_Failed;
		}

	// Store request string
	request->requestString=StrDup(requestString);

	// Clean up after SplitURL
	delete[] requestString;
	delete[] port;
	delete[] address;

	// Return handle to this request
	return request->handle;
	}


//*** Request_Post ***

int HTTP::Request_Post(const char* url, const void* data, int size, float timeOut)
	{
	// Create new request entry, and add it to list of request entries
	Request* request=new Request;
	currentRequestHandle_++;
	request->handle=currentRequestHandle_;
	request->usingGetMethod=false;
	request->status=Status_Connecting;
	request->timeOut_=timeOut;
	request->elapsedTime_=0;
	request->connection=0;
	request->requestString=0;
	request->contentLength=0;
	request->resource=0;
	request->postData.Write(static_cast<const char*>(data),size);
	requests_.Add(request);

	// Make sure URL starts with http://
	if (StrNICmp(url,"http://",7)!=0)
		{
		request->status=Status_Failed;
		return request->handle;
		}

	// Split URL string
	char* address=0;
	char* port=0;
	char* requestString=0;
	SplitURL(url,address,port,requestString);

	// Create connection
	if (Platform::GetPlatform_Network())
		{
		request->connection=Platform::GetPlatform_Network()->CreateClient(Platform_Network::ConnectionMode_TCP,address,StringToInt(port));
		}
	if (!request->connection->Connect())
		{
		request->status=Status_Failed;
		}

	// Store request string
	request->requestString=StrDup(requestString);

	// Clean up after SplitURL
	delete[] requestString;
	delete[] port;
	delete[] address;

	// Return handle to this request
	return request->handle;
	}


//*** GetRequestStatus ***

HTTP::RequestStatus HTTP::GetRequestStatus(int requestHandle)
	{
	Request* request=GetRequest(requestHandle);
	
	if (request)
		{
		return request->status;
		}

	return Status_Invalid;
	}


//*** GetPercentageReceived ***

float HTTP::GetPercentageReceived(int requestHandle)
	{
	Request* request=GetRequest(requestHandle);
	
	if (request && request->contentLength>0 && request->headerLength>0)
		{
		return (float)request->receivedData.GetSize()/(float)(request->contentLength+request->headerLength);
		}

	return 0.0f;
	}


//*** GetResource ***

HTTP_Resource* HTTP::GetResource(int requestHandle)
	{
	Request* request=GetRequest(requestHandle);
	return request->resource;
	}


//*** DiscardRequest ***

void HTTP::DiscardRequest(int requestHandle)
	{
	Request* request=GetRequest(requestHandle);

	if (request)
		{
		if (request->connection)
			{
			delete request->connection;
			}
		if (request->requestString)
			{
			Free(request->requestString);
			}
		if (request->resource)
			{
			delete request->resource;
			}
		BagIterator<Request*> it(requests_);
		if (it.Find(request))
			{
			requests_.Remove(it);
			}
		delete request;
		}

	}


//*** GetRequest ***

HTTP::Request* HTTP::GetRequest(int requestHandle)
	{
	for (int i=0; i<requests_.GetItemCount(); i++)
		{
		Request* request=requests_.Get(i);
		if (request->handle==requestHandle)
			{
			return request;
			}
		}

	return 0;
	}


//*** SplitURL ***

void HTTP::SplitURL(const char* url, char*& address, char*&port, char*& resource)
	{
	// Skip http:// part of url
	url+=7;

	// Find end of address part of URL
	const char* addressEnd=StrChr(url,':');
	if (addressEnd==0)
		{
		addressEnd=StrChr(url,'/');
		}
	if (addressEnd==0)
		{
		addressEnd=url+StrLen(url);
		}

	// Extract address
	address=new char[addressEnd-url+1];
	StrNCpy(address,url,(int)(addressEnd-url));
	address[addressEnd-url]=0;

	// Check if there's a port defined
	port=new char[StrLen(addressEnd)+5];
	const char* portEnd=addressEnd;
	StrCpy(port,"80");
	if (*addressEnd==':')
		{
		addressEnd++;
		portEnd=StrChr(addressEnd,'/');
		if (portEnd==0)
			{
			portEnd=addressEnd+StrLen(addressEnd);
			}
		StrNCpy(port,addressEnd,(int)(portEnd-addressEnd));
		port[portEnd-addressEnd]=0;
		}


	// Check if there's a resource defined
	resource=new char[StrLen(portEnd)+5];
	StrCpy(resource,"/");
	if (*portEnd=='/')
		{
		StrCpy(resource,portEnd);
		}

	}
