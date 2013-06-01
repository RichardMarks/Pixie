/**
 * \class	HTTP
 * 
 * \ingroup	network
 * \brief	Functionality to request HTTP resources
 * \author	Mattias Gustavsson	
 * 
 * The HTTP system provides high-level functionality for executing HTTP GET and HTTP POST
 * requests, and to receive the response from the server.
 * 
 * All network requests are done asynchronously, so when you issue a request, you're given
 * a handle, and using this handle, 
 */

#ifndef __HTTP_H__
#define __HTTP_H__

// Includes
#include "Singleton.h"
#include "StringId.h"
#include "Bag.h"
#include "DynamicBuffer.h"

// Forward declares
class HTTP_Resource;
class Platform_Network_Client;

// HTTP
class HTTP : public Singleton<HTTP>
	{
	public:
		HTTP();

		~HTTP();

		void Update(
			float deltaTime
			);

		enum RequestStatus
			{
			Status_Connecting,
			Status_Pending,
			Status_Completed,
			Status_Failed,
			Status_TimedOut,
			Status_Invalid,
			};


		int Request_Get(
			const char* url, 
			float timeOut = 0.0f
			);

		int Request_Post(
			const char* url, 
			const void* data, 
			int size, 
			float timeOut = 0.0f
			);

		RequestStatus GetRequestStatus(
			int requestHandle
			);

		float GetPercentageReceived(
			int requestHandle
			);

		HTTP_Resource* GetResource(
			int requestHandle
			);

		void DiscardRequest(
			int requestHandle
			);

	private:
		struct Request;
		
		void SplitURL(
			const char* url, 
			char*& address, 
			char*& port, 
			char*& resource
			);
		
		Request* GetRequest(
			int requestHandle
			);

	private:
		int currentRequestHandle_;
		
		struct Request 
			{
			int handle;
			bool usingGetMethod;
			DynamicBuffer postData;
			RequestStatus status;
			float timeOut_;
			float elapsedTime_;
			Platform_Network_Client* connection;
			char* requestString;
			DynamicBuffer receivedData;
			StringId contentType;
			int contentLength;
			int headerLength;
			HTTP_Resource* resource;
			};

		Bag<Request*> requests_;
	};


#define siHTTP HTTP::GetInstance()

#endif /* __HTTP_H__ */
