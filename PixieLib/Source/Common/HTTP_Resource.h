/**
 * \class HTTP_Resource
 * 
 * \ingroup	network
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __HTTP_Resource_H__
#define __HTTP_Resource_H__

// Includes
#include "StaticBuffer.h"
#include "StringId.h"

// Forward declares
class StaticBuffer;

// HTTP_Resource
class HTTP_Resource
	{
	public:
		HTTP_Resource(int requestHandle, StringId contentType, const StaticBuffer& data);

		~HTTP_Resource();

		StringId GetType();

		int GetRequestHandle();

		const StaticBuffer& GetData();

	private:
		int requestHandle_;
		StringId type_;
		StaticBuffer data_;
	};


#endif /* __HTTP_Resource_H__ */

